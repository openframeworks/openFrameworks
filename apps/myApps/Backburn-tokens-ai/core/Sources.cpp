// AI Token Backburn - what your AI coding tools have used, and what's left.
// Copyright (c) 2026 Daniel Rosser. MIT License.

#include "Sources.h"
#include "../release/include/Obfuscate.h"

#include "ofUtils.h"

#include "ofURLFileLoader.h"

#if defined(TARGET_OSX)
#include <sqlite3.h>
#endif

#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unordered_set>

namespace backburn {

const char * providerName(const std::string & id) {
	if (id == "claude") return "Claude Code";
	if (id == "claude-api") return "Claude API";
	if (id == "codex") return "Codex";
	if (id == "grok") return "Grok Build";
	if (id == "muse") return "Muse";
	return "Unknown";
}

std::string defaultFolder(const std::string & id) {
	if (id == "claude") {
		// Claude Code's own override
		const char * dir = std::getenv("CLAUDE_CONFIG_DIR");
		return dir && *dir ? std::string(dir) : (Paths::home() / ".claude").string();
	}
	if (id == "codex") {
		const char * dir = std::getenv("CODEX_HOME");
		return dir && *dir ? std::string(dir) : (Paths::home() / ".codex").string();
	}
	if (id == "grok") return (Paths::home() / ".grok").string();
	if (id == "muse") return (Paths::home() / ".local" / "share" / "muse").string();
	return "";
}

// $/MTok. Cache writes are 1.25x input for the 5-minute TTL and 2x for the 1-hour one.
const Price * claudePrice(const std::string & model) {
	static const std::map<std::string, Price> prices = {
		{ "claude-fable-5-1", { 10.00, 50.00, 0.25 } },
		{ "claude-fable-5", { 10.00, 50.00, 0.25 } },
		{ "claude-opus-5-5", { 4.00, 20.00, 0.20 } },
		{ "claude-opus-5", { 5.00, 25.00, 0.50 } },
		{ "claude-opus-4-8", { 5.00, 25.00, 0.50 } },
		{ "claude-opus-4-7", { 5.00, 25.00, 0.50 } },
		{ "claude-opus-4-6", { 5.00, 25.00, 0.50 } },
		{ "claude-sonnet-5-5", { 2.00, 10.00, 0.20 } },
		{ "claude-sonnet-5", { 2.00, 10.00, 0.20 } },
		{ "claude-sonnet-4-6", { 3.00, 15.00, 0.30 } },
		{ "claude-haiku-5-5", { 0.10, 0.50, 0.01 } },
		{ "claude-haiku-4-5", { 1.00, 5.00, 0.10 } },
	};
	auto it = prices.find(model);
	return it == prices.end() ? nullptr : &it->second;
}

int64_t parseIso(const std::string & iso) {
	std::tm tm = {};
	std::istringstream in(iso.substr(0, 19));
	in >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
	if (in.fail()) return 0;
#if defined(TARGET_WIN32)
	return _mkgmtime(&tm);
#else
	return timegm(&tm);
#endif
}

// %2F -> /: Grok names its session folders after the project's path
static std::string urlDecode(const std::string & in) {
	std::string out;
	for (size_t i = 0; i < in.size(); i++) {
		if (in[i] == '%' && i + 2 < in.size() && std::isxdigit(static_cast<unsigned char>(in[i + 1])) && std::isxdigit(static_cast<unsigned char>(in[i + 2]))) {
			out += char(std::stoi(in.substr(i + 1, 2), nullptr, 16));
			i += 2;
		} else {
			out += in[i];
		}
	}
	return out;
}

std::string tidyPath(std::string path) {
	std::string home = Paths::home().string();
	if (!home.empty() && path.rfind(home, 0) == 0) path = "~" + path.substr(home.size());
	return path;
}

static int64_t unixTime(fs::file_time_type t) {
	auto sys = std::chrono::system_clock::now() + (t - fs::file_time_type::clock::now());
	return std::chrono::duration_cast<std::chrono::seconds>(sys.time_since_epoch()).count();
}

void JsonlTail::scan(const fs::path & root, const std::string & suffix, const std::vector<std::string> & needles,
                     const std::function<void(const fs::path &, const ofJson &)> & line) {
	std::error_code ec;
	if (!fs::is_directory(root, ec)) return;
	for (auto it = fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied, ec);
	     it != fs::recursive_directory_iterator(); it.increment(ec)) {
		if (ec) break;
		const fs::path & path = it->path();
		const std::string name = path.filename().string();
		if (name.size() < suffix.size() || name.compare(name.size() - suffix.size(), suffix.size(), suffix) != 0) continue;
		uintmax_t size = fs::file_size(path, ec);
		if (ec) continue;
		auto modified = fs::last_write_time(path, ec);
		if (!ec) {
			int64_t t = unixTime(modified);
			if (!oldestFile || t < oldestFile) oldestFile = t;
		}
		uintmax_t & offset = offsets[path.string()];
		if (size < offset) offset = 0;
		if (size == offset) continue;
		std::ifstream in(path, std::ios::binary);
		in.seekg(static_cast<std::streamoff>(offset));
		std::string text;
		while (std::getline(in, text)) {
			// an unfinished last line is read again next time, when it's complete
			if (in.eof()) break;
			offset += text.size() + 1;
			if (std::none_of(needles.begin(), needles.end(), [&](const std::string & n) { return text.find(n) != std::string::npos; })) continue;
			ofJson j = ofJson::parse(text, nullptr, false);
			if (!j.is_discarded()) line(path, j);
		}
	}
}

namespace {

template <typename T>
T get(const ofJson & j, const char * key, T fallback) {
	if (!j.is_object()) return fallback;
	auto it = j.find(key);
	if (it == j.end() || it->is_null()) return fallback;
	try { return it->get<T>(); } catch (...) { return fallback; }
}

const ofJson & child(const ofJson & j, const char * key) {
	static const ofJson empty = ofJson::object();
	if (!j.is_object()) return empty;
	auto it = j.find(key);
	return it == j.end() || !it->is_object() ? empty : *it;
}

ofJson readJsonFile(const fs::path & path) {
	std::ifstream in(path);
	if (!in) return ofJson();
	return ofJson::parse(in, nullptr, false);
}

// rows keyed by day, project and model; each source adds into its own
class RowTable {
public:
	UsageRow & at(int64_t t, const std::string & project, const std::string & model, const std::string & session) {
		UsageRow key;
		key.day = t > 0 ? localDay(t) : "unknown";
		key.project = project;
		key.model = model;
		key.session = session;
		UsageRow & r = rows[key.key()];
		if (r.day.empty()) r = key;
		return r;
	}
	std::vector<UsageRow> list() const {
		std::vector<UsageRow> out;
		out.reserve(rows.size());
		for (const auto & [k, r] : rows) out.push_back(r);
		return out;
	}

private:
	std::map<std::string, UsageRow> rows;
};

// ---- Claude Code -------------------------------------------------------------------------------------
// <folder>/projects/<project>/<session>.jsonl: one line per message, assistant lines carry the API usage.
// ~/.claude.json (or <folder>/.claude.json with CLAUDE_CONFIG_DIR): cachedUsageUtilization, the 5-hour and
// weekly limits as of Claude Code's last check.

class ClaudeSource : public Source {
public:
	explicit ClaudeSource(const fs::path & folder) : folder(folder) {}

	Provider read(const Settings & settings) override {
		Provider p;
		p.id = "claude";
		p.name = providerName(p.id);
		const fs::path projects = folder / "projects";
		const fs::path config = fs::exists(folder / ".claude.json") ? folder / ".claude.json" : Paths::home() / ".claude.json";
		p.source = tidyPath(projects.string()) + " (transcripts), " + tidyPath(config.string()) + " (limits)";
		p.found = fs::exists(projects) || fs::exists(config);
		if (!p.found) {
			p.error = "Claude Code not found (no " + tidyPath(folder.string()) + ")";
			return p;
		}
		tail.scan(projects, ".jsonl", { "\"usage\"", "-title\"" }, [&](const fs::path &, const ofJson & j) { add(j); });
		p.rows = table.list();
		p.titles = titles;
		// a name given with /rename wins over the one Claude made up
		for (const auto & [session, title] : customTitles) p.titles[session] = title;
		p.oldestLog = oldestMessage ? oldestMessage : tail.oldest();
		readAccount(p, config);
		estimateLimits(p, settings);
		p.summarize();
		audit(p, settings);
		return p;
	}

private:
	void add(const ofJson & j) {
		const std::string type = get<std::string>(j, "type", "");
		const std::string session = get<std::string>(j, "sessionId", "");
		if (type == "ai-title") {
			titles[session] = get<std::string>(j, "aiTitle", "");
			return;
		}
		if (type == "custom-title") {
			customTitles[session] = get<std::string>(j, "customTitle", "");
			return;
		}
		if (type != "assistant") return;
		const ofJson & msg = child(j, "message");
		const ofJson & u = child(msg, "usage");
		if (u.empty()) return;
		// a message with several content blocks is written once per block, each with the same usage
		std::string key = get<std::string>(msg, "id", "") + "|" + get<std::string>(j, "requestId", "");
		if (!seen.insert(key).second) return;
		std::string model = get<std::string>(msg, "model", "unknown");
		if (model == "<synthetic>") return;   // Claude Code's own placeholder messages, no API call

		int64_t t = parseIso(get<std::string>(j, "timestamp", ""));
		if (t && (!oldestMessage || t < oldestMessage)) oldestMessage = t;
		UsageRow & r = table.at(t, tidyPath(get<std::string>(j, "cwd", "")), model, session);
		int64_t in = get<int64_t>(u, "input_tokens", 0), out = get<int64_t>(u, "output_tokens", 0);
		int64_t read = get<int64_t>(u, "cache_read_input_tokens", 0), write = get<int64_t>(u, "cache_creation_input_tokens", 0);
		const ofJson & cc = child(u, "cache_creation");
		int64_t w1h = get<int64_t>(cc, "ephemeral_1h_input_tokens", 0);
		int64_t w5m = cc.empty() ? write : get<int64_t>(cc, "ephemeral_5m_input_tokens", 0);
		r.calls++;
		r.input += in;
		r.output += out;
		r.cacheRead += read;
		r.cacheWrite5m += w5m;
		r.cacheWrite1h += w1h;
		r.reasoning += get<int64_t>(child(u, "output_tokens_details"), "thinking_tokens", 0);
		Totals one;
		one.input = in, one.output = out, one.cacheRead = read, one.cacheWrite5m = w5m, one.cacheWrite1h = w1h;
		if (const Price * price = claudePrice(model)) {
			double cost = (in * price->input + out * price->output + read * price->cacheRead + w5m * price->input * 1.25
			               + w1h * price->input * 2.0) / 1e6;
			r.cost = std::max(0.0, r.cost) + cost;
			one.cost = cost;
		}
		// each reply's time and size, for estimating the limits between Claude Code's checks
		if (t > now() - 9 * 86400) recent.push_back({ t, one.units() });
	}

	// Claude Code saves the limits only when it checks them. Between checks, each window's percent is estimated:
	// the saved reading gives the share one unit of use takes, and the use since is added. A 5-hour window starts
	// with the first message after the last one reset; the weekly one resets at the same time each week.
	void estimateLimits(Provider & p, const Settings & settings) {
		if (p.limitsAt <= 0 || now() - p.limitsAt < settings.staleHours * 3600) return;
		std::sort(recent.begin(), recent.end());
		const int64_t cutoff = now() - 9 * 86400;
		recent.erase(recent.begin(), std::lower_bound(recent.begin(), recent.end(), std::make_pair(cutoff, 0.0)));
		auto used = [&](int64_t from, int64_t to) {
			double u = 0;
			for (auto it = std::lower_bound(recent.begin(), recent.end(), std::make_pair(from, 0.0)); it != recent.end() && it->first < to; ++it)
				u += it->second;
			return u;
		};
		for (Limit & l : p.limits) {
			const bool session = l.name.rfind("Session", 0) == 0;
			const int64_t length = session ? 5 * 3600 : l.name.rfind("Weekly", 0) == 0 ? 7 * 86400 : l.name == "Daily" ? 86400 : 0;
			if (!length || l.resetsAt <= 0 || l.percent < 2) continue;
			const double base = used(l.resetsAt - length, p.limitsAt);
			if (base <= 0) continue;
			const double rate = l.percent / base;
			l.measured = l.percent;
			l.estimate = true;
			if (l.resetsAt > now()) {
				// the same window: what was saved, plus what's been used since
				l.percent = float(l.percent + rate * used(p.limitsAt, now()));
				continue;
			}
			// later windows: roll forward to the one that's open now, if any
			int64_t start = l.resetsAt;
			bool open = false;
			while (start < now()) {
				if (session) {
					auto first = std::lower_bound(recent.begin(), recent.end(), std::make_pair(start, 0.0));
					if (first == recent.end()) break;   // no message since: no window open
					start = first->first;
				}
				if (start + length > now()) {
					open = true;
					break;
				}
				start += length;
			}
			l.percent = open ? float(rate * used(start, now())) : 0;
			l.resetsAt = open ? start + length : 0;
		}
	}

	void readAccount(Provider & p, const fs::path & configFile) {
		ofJson config = readJsonFile(configFile);
		const ofJson & account = child(config, "oauthAccount");
		std::string org = get<std::string>(account, "organizationType", "");
		if (org.rfind("claude_", 0) == 0) org = org.substr(7);
		if (!org.empty()) org[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(org[0])));
		p.plan = org.empty() ? (std::getenv("ANTHROPIC_API_KEY") ? "API key" : "") : org;

		const ofJson & cached = child(config, "cachedUsageUtilization");
		const ofJson & util = child(cached, "utilization");
		p.limitsAt = get<int64_t>(cached, "fetchedAtMs", 0) / 1000;
		auto limit = [&](const char * key, const char * name) {
			const ofJson & w = child(util, key);
			if (w.empty()) return;
			Limit l;
			l.name = name;
			l.percent = get<float>(w, "utilization", -1);
			l.resetsAt = parseIso(get<std::string>(w, "resets_at", ""));
			p.limits.push_back(l);
		};
		limit("five_hour", "Session (5 h)");
		limit("seven_day", "Weekly");
		limit("seven_day_opus", "Weekly, Opus");
		limit("seven_day_sonnet", "Weekly, Sonnet");
		// any other window Claude reports (a daily one, a model's own) is in the limits list
		if (util.contains("limits") && util["limits"].is_array())
			for (const auto & l : util["limits"]) {
				const std::string group = get<std::string>(l, "group", ""), kind = get<std::string>(l, "kind", "");
				if (group == "session" || kind == "weekly_all" || kind == "weekly_opus" || kind == "weekly_sonnet") continue;
				std::string name = group == "daily" || kind.rfind("daily", 0) == 0 ? "Daily" : kind;
				if (name.empty()) continue;
				if (name != "Daily") {
					// weekly_cowork -> Weekly, cowork
					ofStringReplace(name, "_", " ");
					name[0] = char(std::toupper(static_cast<unsigned char>(name[0])));
					size_t space = name.find(' ');
					if (space != std::string::npos) name = name.substr(0, space) + "," + name.substr(space);
				}
				if (p.limit(name)) continue;
				Limit extra;
				extra.name = name;
				extra.percent = get<float>(l, "percent", -1);
				extra.resetsAt = parseIso(get<std::string>(l, "resets_at", ""));
				extra.active = get<bool>(l, "is_active", false);
				p.limits.push_back(extra);
			}
		if (util.contains("limits") && util["limits"].is_array())
			for (const auto & l : util["limits"]) {
				if (!get<bool>(l, "is_active", false)) continue;
				std::string group = get<std::string>(l, "group", "");
				for (auto & mine : p.limits)
					if ((group == "session" && mine.name.rfind("Session", 0) == 0) || (group == "weekly" && mine.name == "Weekly"))
						mine.active = true;
			}

		// extra usage: credits that keep Claude Code going past the plan's limits
		const ofJson & extra = child(util, "extra_usage");
		const ofJson & spend = child(util, "spend");
		if (!extra.empty() || !spend.empty()) {
			Credits & c = p.credits;
			c.known = true;
			c.enabled = get<bool>(extra, "is_enabled", false) || get<bool>(spend, "enabled", false);
			const ofJson & used = child(spend, "used");
			if (!used.empty()) {
				c.used = get<double>(used, "amount_minor", 0) / std::pow(10.0, get<int>(used, "exponent", 2));
				c.currency = get<std::string>(used, "currency", "USD");
			}
			const ofJson & cap = child(spend, "limit");
			if (!cap.empty()) c.limit = get<double>(cap, "amount_minor", 0) / std::pow(10.0, get<int>(cap, "exponent", 2));
			if (extra.contains("monthly_limit") && extra["monthly_limit"].is_number()) c.limit = extra["monthly_limit"].get<double>();
			c.note = get<std::string>(spend, "disclaimer", "");
		}
		// which product used the weekly limit: Claude Code, chats, Cowork
		weeklyShare.clear();
		const ofJson & breakdown = child(util, "seven_day_breakdown");
		if (breakdown.contains("rows") && breakdown["rows"].is_array())
			for (const auto & row : breakdown["rows"]) {
				int pct = get<int>(row, "percent", 0);
				if (pct > 0) weeklyShare += (weeklyShare.empty() ? "" : ", ") + get<std::string>(row, "display_name", "?") + " " + std::to_string(pct) + "%";
			}
		ofJson settings = readJsonFile(folder / "settings.json");
		cleanupDays = get<int>(settings, "cleanupPeriodDays", 30);
	}

	void audit(Provider & p, const Settings & settings) {
		auto note = [&](Finding::Level level, std::string title, std::string detail) { p.findings.push_back({ level, title, detail }); };
		if (p.limitsAt > 0 && now() - p.limitsAt > settings.staleHours * 3600)
			note(Finding::Level::Warning, "Limits saved " + relativeTime(p.limitsAt) + ", estimated since",
			     "Claude Code saves the 5-hour and weekly limits each time it checks them. Until it does again, the ≈ numbers "
			     "add your use since that reading. Run /usage in Claude Code, or just use it, for the real ones.");
		if (p.limits.empty())
			note(Finding::Level::Info, "No plan limits found",
			     "Claude Code hasn't saved them yet (or this login uses an API key, which has no 5-hour or weekly limit).");
		for (const auto & l : p.limits) {
			if (l.percent < settings.warnPercent) continue;
			// a window that has already reset no longer counts
			if (l.resetsAt && l.resetsAt < now()) continue;
			note(l.percent >= 100 ? Finding::Level::Problem : Finding::Level::Warning,
			     l.name + " limit " + (l.estimate ? "\u2248" : "") + ofToString(int(l.percent)) + "% used",
			     "Resets " + relativeTime(l.resetsAt) + "." + (l.estimate ? " Estimated from your use since the last saved reading." : ""));
		}
		if (p.credits.known && !p.credits.enabled)
			note(Finding::Level::Info, "Usage credits are off",
			     "When a limit is reached Claude Code waits for the reset. Usage credits (extra usage) would let it go on, "
			     "billed per token: " + OBFSTR("https://support.claude.com/articles/12429409"));
		else if (p.credits.enabled && p.credits.used > 0)
			note(Finding::Level::Info, "Usage credits used: " + money(p.credits.used),
			     p.credits.limit > 0 ? "Of a " + money(p.credits.limit) + " monthly limit." : "No monthly limit set.");
		if (!weeklyShare.empty()) note(Finding::Level::Info, "Weekly limit used by: " + weeklyShare, "");
		if (cleanupDays <= 30 && p.oldestLog > 0)
			note(Finding::Level::Warning, "Claude Code deletes transcripts after " + std::to_string(cleanupDays) + " days",
			     "The oldest kept is from " + localDay(p.oldestLog) + ". Backburn keeps every day's usage in its own "
			     "ledger, so its history goes on; to keep the transcripts too, set \"cleanupPeriodDays\": 365 in "
			     + tidyPath((folder / "settings.json").string()) + ".");

		double total = p.total.cost;
		if (total > 0) {
			note(Finding::Level::Info, money(total) + " at API prices",
			     p.plan.empty() || p.plan == "API key"
			         ? "What these tokens cost at Anthropic's API prices."
			         : "The " + p.plan + " plan doesn't bill per token: this is what the same tokens would cost on the API, "
			           "to compare against the plan's price.");
			double read = 0, w1h = 0, out = 0;
			for (const auto & m : p.models)
				if (const Price * price = claudePrice(m.model)) {
					read += m.cacheRead * price->cacheRead / 1e6;
					w1h += m.cacheWrite1h * price->input * 2.0 / 1e6;
					out += m.output * price->output / 1e6;
				}
			auto pct = [&](double x) { return ofToString(int(std::round(x / total * 100))) + "%"; };
			note(Finding::Level::Info, "Cache reads " + pct(read) + ", 1-hour cache writes " + pct(w1h) + ", output " + pct(out),
			     "Almost every turn re-reads the conversation from the cache. A new session, /clear, a model switch or a pause "
			     "of over an hour writes it again at twice the input price, so long conversations that keep going are the "
			     "cheapest per turn, and very long ones re-read the most.");
			if (!p.projects.empty() && p.projects.front().cost > total * 0.4)
				note(Finding::Level::Info, p.projects.front().name + " is " + pct(p.projects.front().cost) + " of it", "");
		}
	}

	fs::path folder;
	JsonlTail tail;
	std::unordered_set<std::string> seen;
	RowTable table;
	std::map<std::string, std::string> titles, customTitles;
	std::vector<std::pair<int64_t, double>> recent;   // (time, units) of each reply in the last nine days
	std::string weeklyShare;
	int cleanupDays = 30;
	int64_t oldestMessage = 0;
};

// ---- Codex -------------------------------------------------------------------------------------------
// <folder>/sessions/YYYY/MM/DD/rollout-*.jsonl: token_count events carry the session's running totals
// and the account's rate limits at that moment; session_meta and turn_context the folder and model.

class CodexSource : public Source {
public:
	explicit CodexSource(const fs::path & folder) : folder(folder) {}

	Provider read(const Settings & settings) override {
		Provider p;
		p.id = "codex";
		p.name = providerName(p.id);
		const fs::path root = folder / "sessions";
		p.source = tidyPath(root.string());
		p.found = fs::exists(root);
		if (!p.found) {
			p.error = "Codex not found (no " + p.source + ")";
			return p;
		}
		tail.scan(root, ".jsonl", { "\"type\":\"" }, [&](const fs::path & file, const ofJson & j) { add(file, j); });
		p.rows = table.list();
		// thread names, renamed as the conversation goes; the last line for a thread is its name now
		std::ifstream index(folder / "session_index.jsonl");
		for (std::string line; std::getline(index, line);) {
			ofJson j = ofJson::parse(line, nullptr, false);
			std::string id = get<std::string>(j, "id", ""), name = get<std::string>(j, "thread_name", "");
			if (!id.empty() && !name.empty()) p.titles[id] = name;
		}
		p.oldestLog = tail.oldest();
		p.plan = plan;
		p.limits = limits;
		p.limitsAt = limitsAt;
		p.credits = credits;
		p.summarize();
		if (p.limitsAt && now() - p.limitsAt > settings.staleHours * 3600)
			p.findings.push_back({ Finding::Level::Info, "Limits are from " + relativeTime(p.limitsAt),
			                       "Codex reports them with each request; they update the next time it runs." });
		for (const auto & l : p.limits)
			if (l.percent >= settings.warnPercent && l.resetsAt > now())
				p.findings.push_back({ l.percent >= 100 ? Finding::Level::Problem : Finding::Level::Warning,
				                       l.name + " limit " + ofToString(int(l.percent)) + "% used", "Resets " + relativeTime(l.resetsAt) + "." });
		if (credits.known && !credits.enabled)
			p.findings.push_back({ Finding::Level::Info, "No Codex credits",
			                       "When a limit is reached Codex waits for the reset." });
		return p;
	}

private:
	struct Session {
		std::string id;
		std::string model = "unknown";
		std::string project;
		int64_t input = 0, cached = 0, output = 0, reasoning = 0;
	};

	void add(const fs::path & file, const ofJson & j) {
		const ofJson & payload = child(j, "payload");
		std::string type = get<std::string>(j, "type", "");
		Session & s = sessions[file.string()];
		if (s.id.empty()) {
			// rollout-2026-10-29T08-24-46-<id>.jsonl, until session_meta says
			std::string stem = file.stem().string();
			s.id = stem.size() > 36 ? stem.substr(stem.size() - 36) : stem;
		}
		if (type == "session_meta") {
			std::string id = get<std::string>(payload, "id", "");
			if (!id.empty()) s.id = id;
		}
		if (type == "turn_context" || type == "session_meta") {
			std::string model = get<std::string>(payload, "model", "");
			if (!model.empty()) s.model = model;
			std::string cwd = get<std::string>(payload, "cwd", "");
			if (!cwd.empty()) s.project = tidyPath(cwd);
			return;
		}
		if (get<std::string>(payload, "type", "") != "token_count") return;
		int64_t t = parseIso(get<std::string>(j, "timestamp", ""));
		// running totals for the session: count what each event adds, so repeated events add nothing
		const ofJson & total = child(child(payload, "info"), "total_token_usage");
		if (!total.empty()) {
			int64_t in = get<int64_t>(total, "input_tokens", 0), cached = get<int64_t>(total, "cached_input_tokens", 0);
			int64_t out = get<int64_t>(total, "output_tokens", 0), reasoning = get<int64_t>(total, "reasoning_output_tokens", 0);
			if (in >= s.input && out >= s.output && (in > s.input || out > s.output)) {
				UsageRow & r = table.at(t, s.project, s.model, s.id);
				r.calls++;
				// Codex counts cached input inside input_tokens
				r.cacheRead += cached - s.cached;
				r.input += (in - s.input) - (cached - s.cached);
				r.output += out - s.output;
				r.reasoning += reasoning - s.reasoning;
				s.input = in, s.cached = cached, s.output = out, s.reasoning = reasoning;
			}
		}
		const ofJson & rl = child(payload, "rate_limits");
		if (rl.empty() || t < limitsAt) return;
		limitsAt = t;
		limits.clear();
		for (const char * key : { "primary", "secondary" }) {
			const ofJson & w = child(rl, key);
			if (w.empty()) continue;
			Limit l;
			int minutes = get<int>(w, "window_minutes", 0);
			l.name = minutes == 300 ? "Session (5 h)" : minutes == 10080 ? "Weekly" : minutes == 1440 ? "Daily" : std::to_string(minutes / 60) + " h window";
			l.percent = get<float>(w, "used_percent", -1);
			l.resetsAt = get<int64_t>(w, "resets_at", 0);
			limits.push_back(l);
		}
		plan = get<std::string>(rl, "plan_type", "");
		if (!plan.empty()) plan[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(plan[0])));
		const ofJson & c = child(rl, "credits");
		if (!c.empty()) {
			credits.known = true;
			credits.enabled = get<bool>(c, "has_credits", false) || get<bool>(c, "unlimited", false);
			credits.balance = ofToDouble(get<std::string>(c, "balance", "0"));
			credits.note = get<bool>(c, "unlimited", false) ? "Unlimited" : "";
		}
	}

	fs::path folder;
	JsonlTail tail;
	std::map<std::string, Session> sessions;
	RowTable table;
	std::vector<Limit> limits;
	int64_t limitsAt = 0;
	std::string plan;
	Credits credits;
};

// ---- Grok Build ----------------------------------------------------------------------------------------
// <folder>/sessions/<url-encoded project folder>/<session>/updates.jsonl: turn_completed carries the turn's
// usage, with the cost in USD ticks (1e-10 USD).

class GrokSource : public Source {
public:
	explicit GrokSource(const fs::path & folder) : folder(folder) {}

	Provider read(const Settings &) override {
		Provider p;
		p.id = "grok";
		p.name = providerName(p.id);
		const fs::path root = folder / "sessions";
		p.source = tidyPath(root.string());
		p.costReported = true;
		p.found = fs::exists(root);
		if (!p.found) {
			p.error = "Grok Build not found (no " + p.source + ")";
			return p;
		}
		tail.scan(root, "updates.jsonl", { "turn_completed" }, [&](const fs::path & file, const ofJson & j) { add(file, j); });
		p.rows = table.list();
		// each session's summary.json names it
		std::error_code ec;
		for (auto it = fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied, ec);
		     it != fs::recursive_directory_iterator(); it.increment(ec)) {
			if (ec) break;
			if (it->path().filename() != "summary.json") continue;
			auto modified = fs::last_write_time(it->path(), ec);
			const std::string session = it->path().parent_path().filename().string();
			if (ec || (summaryAt.count(session) && summaryAt[session] == modified)) continue;
			summaryAt[session] = modified;
			std::string title = get<std::string>(readJsonFile(it->path()), "session_summary", "");
			if (!title.empty()) titles[session] = title;
		}
		p.titles = titles;
		p.oldestLog = tail.oldest();
		p.summarize();
		if (outOfBalance)
			p.findings.push_back({ Finding::Level::Problem, "Usage balance ran out " + std::to_string(outOfBalance) + " times",
			                       "Last " + relativeTime(lastOutOfBalance) + ": \"Grok Build usage balance exhausted\" (402). "
			                       "Top up the balance on your xAI account." });
		if (authErrors)
			p.findings.push_back({ Finding::Level::Warning, std::to_string(authErrors) + " turns failed to sign in",
			                       "\"Invalid or expired\" (401): run grok and sign in again." });
		return p;
	}

private:
	void add(const fs::path & file, const ofJson & j) {
		const ofJson & update = child(child(j, "params"), "update");
		if (get<std::string>(update, "sessionUpdate", "") != "turn_completed") return;
		if (!seen.insert(get<std::string>(update, "prompt_id", "")).second) return;
		int64_t t = get<int64_t>(j, "timestamp", 0);
		std::string result = get<std::string>(update, "agent_result", "");
		if (result.find("402") != std::string::npos) {
			outOfBalance++;
			lastOutOfBalance = std::max(lastOutOfBalance, t);
		}
		if (result.find("401") != std::string::npos) authErrors++;
		// the session's folder is named after the project's path, URL-encoded
		const std::string project = tidyPath(urlDecode(file.parent_path().parent_path().filename().string()));
		for (const auto & [model, u] : child(child(update, "usage"), "modelUsage").items()) {
			UsageRow & r = table.at(t, project, model, file.parent_path().filename().string());
			r.calls += get<int64_t>(u, "modelCalls", 0);
			r.input += get<int64_t>(u, "inputTokens", 0) - get<int64_t>(u, "cachedReadTokens", 0);
			r.output += get<int64_t>(u, "outputTokens", 0);
			r.cacheRead += get<int64_t>(u, "cachedReadTokens", 0);
			r.cacheWrite5m += get<int64_t>(u, "cacheCreationTokens", 0);
			r.reasoning += get<int64_t>(u, "reasoningTokens", 0);
			r.cost = std::max(0.0, r.cost) + get<double>(u, "costUsdTicks", 0) / 1e10;
		}
	}

	fs::path folder;
	JsonlTail tail;
	std::unordered_set<std::string> seen;
	RowTable table;
	std::map<std::string, std::string> titles;
	std::map<std::string, fs::file_time_type> summaryAt;
	int outOfBalance = 0, authErrors = 0;
	int64_t lastOutOfBalance = 0;
};

// ---- Muse ------------------------------------------------------------------------------------------------
// <folder>/sessions/<view>/<session>/snapshot-*.json: each snapshot has the session's cost ledger so far and
// its workspace_root; the largest per session and model counts.

class MuseSource : public Source {
public:
	explicit MuseSource(const fs::path & folder) : folder(folder) {}

	Provider read(const Settings &) override {
		Provider p;
		p.id = "muse";
		p.name = providerName(p.id);
		const fs::path root = folder / "sessions";
		p.source = tidyPath(root.string());
		p.found = fs::exists(root);
		if (!p.found) {
			p.error = "Muse not found (no " + p.source + ")";
			return p;
		}
		ofJson config = readJsonFile(Paths::home() / ".config" / "muse" / "settings.json");
		p.plan = get<std::string>(config, "provider", "");
		std::error_code ec;
		for (auto it = fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied, ec);
		     it != fs::recursive_directory_iterator(); it.increment(ec)) {
			if (ec) break;
			const std::string name = it->path().filename().string();
			if (name.rfind("snapshot-", 0) != 0 || it->path().extension() != ".json") continue;
			auto modified = fs::last_write_time(it->path(), ec);
			if (ec) continue;
			const std::string key = it->path().string();
			auto seen = readAt.find(key);
			if (seen != readAt.end() && seen->second == modified) continue;
			readAt[key] = modified;
			addSnapshot(it->path(), unixTime(modified));
		}
		// snapshots are running totals: each one adds what grew since the one before, on the day it was saved
		RowTable table;
		for (const auto & [id, s] : sessions) {
			std::map<std::string, Totals> before;
			for (const auto & [t, models] : s.snapshots)
				for (const auto & [model, cur] : models) {
					Totals & prev = before[model];
					if (cur.allInput() <= prev.allInput() && cur.output <= prev.output) continue;
					UsageRow & r = table.at(t, s.project, model, id);
					r.calls += prev.allInput() == 0 ? 1 : 0;
					r.input += std::max<int64_t>(0, cur.input - prev.input);
					r.cacheRead += std::max<int64_t>(0, cur.cacheRead - prev.cacheRead);
					r.output += std::max<int64_t>(0, cur.output - prev.output);
					r.reasoning += std::max<int64_t>(0, cur.reasoning - prev.reasoning);
					prev = cur;
				}
		}
		p.rows = table.list();
		p.titles = museTitles();
		p.summarize();
		if (!sessions.empty())
			p.findings.push_back({ Finding::Level::Info, std::to_string(sessions.size()) + " sessions",
			                       "Muse keeps token counts but no price or limits in its files, so only tokens are shown. "
			                       "\"Calls\" here counts sessions." });
		return p;
	}

private:
	struct Session {
		std::string project;
		std::map<int64_t, std::map<std::string, Totals>> snapshots;   // by when each was saved
	};

	void addSnapshot(const fs::path & file, int64_t t) {
		std::ifstream in(file);
		std::stringstream buf;
		buf << in.rdbuf();
		const std::string text = buf.str();
		ofJson j = ofJson::parse(text, nullptr, false);
		const ofJson & ledger = child(child(child(j, "continuation"), "state"), "cost_ledger");
		if (ledger.empty()) return;
		Session & s = sessions[file.parent_path().filename().string()];
		// the folder is deep in the snapshot; finding the text is simpler than walking to it
		const std::string marker = "\"workspace_root\":\"";
		size_t at = text.find(marker);
		if (at != std::string::npos) {
			size_t end = text.find('"', at + marker.size());
			if (end != std::string::npos) s.project = tidyPath(text.substr(at + marker.size(), end - at - marker.size()));
		}
		auto & models = s.snapshots[t];
		for (const auto & [model, entry] : ledger.items()) {
			const ofJson & u = child(entry, "usage");
			Totals m;
			m.cacheRead = get<int64_t>(u, "cache_read_tokens", 0);
			m.input = get<int64_t>(u, "input_tokens", 0) - m.cacheRead;
			m.output = get<int64_t>(u, "output_tokens", 0);
			m.reasoning = get<int64_t>(u, "reasoning_tokens", 0);
			models[model] = m;
		}
	}

	// Muse names its sessions in an SQLite index. On macOS the system's SQLite library reads it in-process (no
	// child process, which a sandboxed app can't rely on); elsewhere the sqlite3 tool, if there is one.
	std::map<std::string, std::string> museTitles() {
		std::map<std::string, std::string> out;
		const fs::path db = folder / "session-index.db";
		if (!fs::exists(db)) return out;
		const char * sql = "select session_id, coalesce(session_name, effective_long_title, msp_title, title, '') from sessions";
#if defined(TARGET_OSX)
		sqlite3 * handle = nullptr;
		if (sqlite3_open_v2(db.string().c_str(), &handle, SQLITE_OPEN_READONLY, nullptr) == SQLITE_OK) {
			sqlite3_stmt * stmt = nullptr;
			if (sqlite3_prepare_v2(handle, sql, -1, &stmt, nullptr) == SQLITE_OK) {
				while (sqlite3_step(stmt) == SQLITE_ROW) {
					auto text = [&](int col) {
						const unsigned char * t = sqlite3_column_text(stmt, col);
						return t ? std::string(reinterpret_cast<const char *>(t)) : std::string();
					};
					std::string title = text(1);
					std::replace(title.begin(), title.end(), '\n', ' ');
					if (!title.empty()) out[text(0)] = title;
				}
				sqlite3_finalize(stmt);
			}
		}
		if (handle) sqlite3_close(handle);
#elif defined(TARGET_LINUX)
		if (!fs::exists("/usr/bin/sqlite3")) return out;
		std::string text = ofSystem("/usr/bin/sqlite3 -readonly \"" + db.string() + "\" \"select session_id || '|' || replace(coalesce("
		                            "session_name, effective_long_title, msp_title, title, ''), char(10), ' ') from sessions\" 2>/dev/null");
		for (const auto & line : ofSplitString(text, "\n", true, false)) {
			size_t sep = line.find('|');
			if (sep != std::string::npos && sep + 1 < line.size()) out[line.substr(0, sep)] = line.substr(sep + 1);
		}
#endif
		return out;
	}

	fs::path folder;
	std::map<std::string, fs::file_time_type> readAt;
	std::map<std::string, Session> sessions;
};

} // namespace

std::unique_ptr<Source> makeLocalSource(const std::string & id, const std::string & folder) {
#if defined(TARGET_OF_IOS) || defined(TARGET_ANDROID)
	return nullptr;
#else
	fs::path dir = folder.empty() ? fs::path(defaultFolder(id)) : fs::path(folder);
	if (id == "claude") return std::make_unique<ClaudeSource>(dir);
	if (id == "codex") return std::make_unique<CodexSource>(dir);
	if (id == "grok") return std::make_unique<GrokSource>(dir);
	if (id == "muse") return std::make_unique<MuseSource>(dir);
	return nullptr;
#endif
}

bool fetchRemote(const std::string & url, const std::string & token, Report & out, std::string & error) {
	if (url.rfind("http://", 0) != 0) {
		error = "only http:// addresses on your own network are supported";
		return false;
	}
	// oF's loader (curl), so the phone apps need no other HTTP client
	ofHttpRequest req((url.back() == '/' ? url.substr(0, url.size() - 1) : url) + "/api/usage", "remote");
	if (!token.empty()) req.headers["Authorization"] = "Bearer " + token;
	req.timeoutSeconds = 5;
	ofURLFileLoader loader;
	ofHttpResponse res = loader.handleRequest(req);
	if (res.status != 200) {
		error = res.status > 0 ? ofToString(res.status) + " " + res.error : (res.error.empty() ? "no connection" : res.error);
		return false;
	}
	ofJson j = ofJson::parse(res.data.getText(), nullptr, false);
	if (j.is_discarded()) {
		error = "the reply wasn't JSON";
		return false;
	}
	out = reportFromJson(j);
	return true;
}

} // namespace backburn
