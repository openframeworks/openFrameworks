// AI Token Backburn - what your AI coding tools have used, and what's left.
// Copyright (c) 2026 Daniel Rosser. MIT License.

#include "ProviderApi.h"
#include "Sources.h"
#include "../release/include/Obfuscate.h"

#include "ofURLFileLoader.h"
#include "ofUtils.h"

#include <ctime>
#include <functional>
#include <map>

namespace backburn {
namespace providerApi {

const char * keyName(const std::string & id) {
	if (id == "claude") return "Anthropic Admin API key";
	if (id == "codex") return "OpenAI admin key";
	if (id == "grok") return "xAI management key";
	if (id == "muse") return "Meta API key";
	return "API key";
}

const char * keyUse(const std::string & id) {
	if (id == "claude")
		return "sk-ant-admin..., from a Claude Console organization (Settings > Admin keys). Adds a Claude API tab with the "
		       "organization's API use for the last 31 days by model, from the Admin API usage report. Pro and Max plans "
		       "aren't Console organizations: their limits come from Claude Code, above.";
	if (id == "codex")
		return "sk-admin-..., from an OpenAI organization (Settings > Admin keys). Adds an OpenAI API tab with the "
		       "organization's API tokens for the last 31 days by model, and what was billed each day. Codex on a "
		       "ChatGPT plan isn't billed through the API, so its limits come from Codex itself, above.";
	if (id == "grok")
		return "From console.x.ai, with the team id. Shows the team's prepaid credit balance from the Management API, "
		       "as xAI reports it, next to the balance-exhausted errors.";
	if (id == "muse") return "Kept for when Meta publishes a usage API. Not called.";
	return "";
}

bool keyUsed(const std::string & id) { return id == "claude" || id == "codex" || id == "grok"; }
bool needsAccount(const std::string & id) { return id == "grok"; }

std::string keyPage(const std::string & id) {
	if (id == "claude") return OBFSTR("https://console.anthropic.com/settings/admin-keys");
	if (id == "codex") return OBFSTR("https://platform.openai.com/settings/organization/admin-keys");
	if (id == "grok") return OBFSTR("https://console.x.ai");
	return "";
}

namespace {

ofHttpResponse get(const std::string & url, const std::map<std::string, std::string> & headers) {
	ofHttpRequest req(url, "backburn");
	req.method = ofHttpRequest::GET;
	req.headers = headers;
	req.headers["User-Agent"] = "AITokenBackburn/1.0";
	req.timeoutSeconds = 20;
	ofURLFileLoader loader;
	return loader.handleRequest(req);
}

std::string failure(const ofHttpResponse & res) {
	std::string body = res.data.getText();
	ofJson j = ofJson::parse(body, nullptr, false);
	// {"error": {"message": ...}} on Anthropic's API
	if (j.is_object() && j.contains("error") && j["error"].is_object() && j["error"].contains("message"))
		return ofToString(res.status) + ": " + j["error"]["message"].get<std::string>();
	if (res.status <= 0) return res.error.empty() ? "no connection" : res.error;
	return ofToString(res.status) + " " + res.error + (body.empty() ? "" : ": " + body.substr(0, 160));
}

int64_t number(const ofJson & j, const char * key) {
	if (!j.is_object() || !j.contains(key) || !j[key].is_number()) return 0;
	return j[key].get<int64_t>();
}

} // namespace

bool anthropicUsage(const std::string & key, Provider & out, std::string & error) {
	out.id = "claude-api";
	out.name = "Claude API";
	out.source = "Anthropic Admin API usage report, last 31 days";
	out.plan = "Console organization";
	// from midnight UTC 30 days ago: the report's 1-day buckets start at midnight UTC
	std::time_t t = std::time(nullptr) - 30 * 86400;
	char start[32];
	std::strftime(start, sizeof(start), "%Y-%m-%dT00:00:00Z", std::gmtime(&t));
	std::map<std::string, UsageRow> rows;
	std::string page;
	for (int pages = 0; pages < 10; pages++) {
		std::string url = OBFSTR("https://api.anthropic.com/v1/organizations/usage_report/messages?starting_at=") + std::string(start)
		                  + "&bucket_width=1d&limit=31&group_by%5B%5D=model" + (page.empty() ? "" : "&page=" + page);
		ofHttpResponse res = get(url, { { "x-api-key", key }, { "anthropic-version", "2023-06-01" } });
		if (res.status != 200) {
			error = failure(res);
			return false;
		}
		ofJson j = ofJson::parse(res.data.getText(), nullptr, false);
		if (!j.is_object() || !j.contains("data")) {
			error = "the reply wasn't a usage report";
			return false;
		}
		for (const auto & bucket : j["data"]) {
			// UTC days, as the report buckets them
			std::string day = bucket.value("starting_at", std::string()).substr(0, 10);
			if (!bucket.contains("results")) continue;
			for (const auto & r : bucket["results"]) {
				std::string model = r.contains("model") && r["model"].is_string() ? r["model"].get<std::string>() : "all models";
				UsageRow & row = rows[day + "|" + model];
				row.day = day;
				row.project = "Anthropic API";
				row.model = model;
				row.input += number(r, "uncached_input_tokens");
				row.cacheRead += number(r, "cache_read_input_tokens");
				if (r.contains("cache_creation")) {
					row.cacheWrite1h += number(r["cache_creation"], "ephemeral_1h_input_tokens");
					row.cacheWrite5m += number(r["cache_creation"], "ephemeral_5m_input_tokens");
				}
				row.output += number(r, "output_tokens");
			}
		}
		if (!j.value("has_more", false) || !j.contains("next_page") || !j["next_page"].is_string()) break;
		page = j["next_page"].get<std::string>();
	}
	for (auto & [k, row] : rows) {
		if (row.output == 0 && row.allInput() == 0) continue;
		// the report counts tokens, not requests
		row.calls = 0;
		if (const Price * price = claudePrice(row.model))
			row.cost = (row.input * price->input + row.output * price->output + row.cacheRead * price->cacheRead
			            + row.cacheWrite5m * price->input * 1.25 + row.cacheWrite1h * price->input * 2.0) / 1e6;
		out.rows.push_back(row);
	}
	out.found = true;
	out.summarize();
	out.findings.push_back({ Finding::Level::Info, money(out.total.cost) + " at list prices, last 31 days",
	                         "Tokens from the Admin API usage report, priced at Anthropic's list prices. Batch, priority and "
	                         "discounts aren't applied: the Claude Console's Cost page has the billed amount. Days are UTC." });
	return true;
}

bool openaiUsage(const std::string & key, Provider & out, std::string & error) {
	out.id = "openai-api";
	out.name = "OpenAI API";
	out.source = "OpenAI organization usage and costs, last 31 days";
	out.plan = "API organization";
	const std::map<std::string, std::string> auth = { { "Authorization", "Bearer " + key } };
	// midnight UTC 30 days ago: both reports bucket by UTC day
	std::time_t t = std::time(nullptr) - 30 * 86400;
	char day0[32];
	std::strftime(day0, sizeof(day0), "%Y-%m-%d", std::gmtime(&t));
	std::tm tm = {};
	strptime(day0, "%Y-%m-%d", &tm);
	const std::string start = ofToString(int64_t(timegm(&tm)));
	auto utcDay = [](const ofJson & bucket) {
		std::time_t at = std::time_t(number(bucket, "start_time"));
		char day[16];
		std::strftime(day, sizeof(day), "%Y-%m-%d", std::gmtime(&at));
		return std::string(day);
	};
	// pages of one report, each bucket handed to take
	auto read = [&](const std::string & path, const std::function<void(const ofJson &)> & take) {
		std::string page;
		for (int pages = 0; pages < 10; pages++) {
			ofHttpResponse res = get(OBFSTR("https://api.openai.com/v1/organization/") + path + "?start_time=" + start
			                             + "&bucket_width=1d&limit=31" + (page.empty() ? "" : "&page=" + page), auth);
			if (res.status != 200) {
				error = failure(res);
				return false;
			}
			ofJson j = ofJson::parse(res.data.getText(), nullptr, false);
			if (!j.is_object() || !j.contains("data") || !j["data"].is_array()) {
				error = "the reply wasn't a usage page";
				return false;
			}
			for (const auto & bucket : j["data"]) take(bucket);
			if (!j.value("has_more", false) || !j.contains("next_page") || !j["next_page"].is_string()) break;
			page = j["next_page"].get<std::string>();
		}
		return true;
	};
	std::map<std::string, UsageRow> rows;
	const bool used = read("usage/completions", [&](const ofJson & bucket) {
		if (!bucket.contains("results")) return;
		const std::string day = utcDay(bucket);
		for (const auto & r : bucket["results"]) {
			std::string model = r.contains("model") && r["model"].is_string() ? r["model"].get<std::string>() : "all models";
			UsageRow & row = rows[day + "|" + model];
			row.day = day;
			row.project = "OpenAI API";
			row.model = model;
			// input_tokens includes the cached ones
			row.cacheRead += number(r, "input_cached_tokens");
			row.input += number(r, "input_tokens") - number(r, "input_cached_tokens");
			row.output += number(r, "output_tokens");
			row.calls += number(r, "num_model_requests");
		}
	});
	if (!used) return false;
	// the costs report is per day, not per model: one row a day carries what was billed
	std::string costError;
	const bool billed = read("costs", [&](const ofJson & bucket) {
		if (!bucket.contains("results")) return;
		const std::string day = utcDay(bucket);
		for (const auto & r : bucket["results"]) {
			if (!r.contains("amount") || !r["amount"].contains("value") || !r["amount"]["value"].is_number()) continue;
			UsageRow & row = rows[day + "|billed"];
			row.day = day;
			row.project = "OpenAI API";
			row.model = "billed, all models";
			row.cost = std::max(0.0, row.cost) + r["amount"]["value"].get<double>();
		}
	});
	if (!billed) {
		costError = error;
		error.clear();
	}
	for (auto & [k, row] : rows)
		if (row.output || row.allInput() || row.cost > 0) out.rows.push_back(row);
	out.found = true;
	out.costReported = billed;
	out.summarize();
	out.findings.push_back({ Finding::Level::Info,
	                         billed ? money(out.total.cost) + " billed, last 31 days" : "Tokens only: the costs report failed",
	                         billed ? "From the organization's usage and costs reports. Cost is per day, so it isn't split "
	                                  "by model. Days are UTC."
	                                : "The usage report worked; the costs report said: " + costError });
	return true;
}

bool xaiBalance(const std::string & key, const std::string & team, std::string & balance, std::string & error) {
	if (team.empty()) {
		error = "needs the team id as well";
		return false;
	}
	ofHttpResponse res = get(OBFSTR("https://management-api.x.ai/v1/billing/teams/") + team + OBFSTR("/prepaid/balance"),
	                         { { "Authorization", "Bearer " + key } });
	if (res.status != 200) {
		error = failure(res);
		return false;
	}
	ofJson j = ofJson::parse(res.data.getText(), nullptr, false);
	if (!j.is_object() || !j.contains("total") || !j["total"].contains("val")) {
		error = "the reply had no total balance";
		return false;
	}
	const ofJson & val = j["total"]["val"];
	balance = val.is_string() ? val.get<std::string>() : val.dump();
	return true;
}

} // namespace providerApi
} // namespace backburn
