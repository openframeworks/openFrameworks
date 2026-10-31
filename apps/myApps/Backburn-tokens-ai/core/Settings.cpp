// AI Token Backburn - what your AI coding tools have used, and what's left.
// Copyright (c) 2026 Daniel Rosser. MIT License.

#include "Settings.h"

#include "ofJson.h"
#include "ofLog.h"
#include "ofUtils.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <random>

#if !defined(TARGET_WIN32)
#include <pwd.h>
#include <unistd.h>
#endif

namespace backburn {

static fs::path ensure(const fs::path & dir) {
	std::error_code ec;
	fs::create_directories(dir, ec);
	return dir;
}

// The real home folder. Inside the macOS sandbox $HOME is the app's container, and the tools' folders are in
// the real one (reading them still needs the user's grant: see release/README.md).
fs::path Paths::home() {
#if defined(TARGET_WIN32)
	const char * home = std::getenv("USERPROFILE");
	return fs::path(home ? home : ".");
#elif defined(TARGET_OF_IOS) || defined(TARGET_ANDROID)
	// no tools' folders on a phone: only the app's own
	const char * home = std::getenv("HOME");
	return fs::path(home ? home : ".");
#else
	if (const passwd * pw = getpwuid(getuid()); pw && pw->pw_dir) return fs::path(pw->pw_dir);
	const char * home = std::getenv("HOME");
	return fs::path(home ? home : ".");
#endif
}

static fs::path appDataOverride;

void Paths::overrideAppData(const fs::path & folder) { appDataOverride = folder; }

fs::path Paths::appData() {
	if (!appDataOverride.empty()) return ensure(appDataOverride);
#if defined(TARGET_OF_IOS)
	// the app's own container: $HOME there is the container, and Documents is writable (the bundle isn't)
	const char * home = std::getenv("HOME");
	return ensure(fs::path(home ? home : ".") / "Documents" / "Backburn");
#elif defined(TARGET_ANDROID)
	return ensure(fs::path(ofToDataPath("", true)));
#elif defined(TARGET_WIN32)
	const char * base = std::getenv("LOCALAPPDATA");
	return ensure(fs::path(base ? base : ".") / "AI Token Backburn");
#elif defined(TARGET_OSX)
	// $HOME, not the real home: in the sandbox that's the app's container, the one place it may write; outside it
	// they're the same folder
	const char * own = std::getenv("HOME");
	return ensure((own && *own ? fs::path(own) : home()) / "Library" / "Application Support" / "AI Token Backburn");
#else
	const char * xdg = std::getenv("XDG_DATA_HOME");
	fs::path base = xdg && *xdg ? fs::path(xdg) : home() / ".local" / "share";
	return ensure(base / "ai-token-backburn");
#endif
}

fs::path Paths::settingsFile() { return appData() / "settings.json"; }
fs::path Paths::ledgerFile() { return appData() / "ledger.json"; }
fs::path Paths::logFile() { return appData() / "backburn.log"; }

fs::path Paths::defaultExports() {
#if defined(TARGET_ANDROID)
	return appData() / "exports";
#elif defined(TARGET_OF_IOS)
	// the app's Documents, which the Files app shows
	const char * own = std::getenv("HOME");
	return fs::path(own ? own : ".") / "Documents" / "BackBurnTokens";
#else
	// sandboxed, ~/Documents is out of reach: the container's Documents, unless a folder is picked in Settings
	const char * own = std::getenv("HOME");
	if (std::getenv("APP_SANDBOX_CONTAINER_ID") && own) return fs::path(own) / "Documents" / "BackBurnTokens";
	return home() / "Documents" / "BackBurnTokens";
#endif
}

std::string Settings::newToken() {
	static const char alphabet[] = "abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ23456789";
	std::random_device rd;
	std::uniform_int_distribution<size_t> pick(0, sizeof(alphabet) - 2);
	std::string out;
	for (int i = 0; i < 32; i++) out += alphabet[pick(rd)];
	return out;
}

template <typename T>
static void read(const ofJson & j, const char * key, T & value) {
	if (j.is_object() && j.contains(key) && !j.at(key).is_null()) {
		try { value = j.at(key).get<T>(); } catch (...) { ofLogWarning("Settings") << "ignoring bad value for " << key; }
	}
}

bool Settings::enabled(const std::string & id) const { return provider(id).enabled; }

const ProviderSettings & Settings::provider(const std::string & id) const {
	static const ProviderSettings none;
	for (const auto & p : providers)
		if (p.id == id) return p;
	return none;
}

ProviderSettings & Settings::provider(const std::string & id) {
	for (auto & p : providers)
		if (p.id == id) return p;
	providers.push_back({ id });
	return providers.back();
}

fs::path Settings::exports() const { return exportFolder.empty() ? Paths::defaultExports() : fs::path(exportFolder); }

static ofJson queryJson(const Query & q) {
	return { { "name", q.name }, { "providers", q.providers }, { "project", q.project }, { "projectExact", q.projectExact },
		     { "model", q.model }, { "lastDays", q.lastDays }, { "groupBy", q.groupBy }, { "format", q.format },
		     { "autoExport", q.autoExport }, { "lastExport", q.lastExport } };
}

static Query queryFrom(const ofJson & j) {
	Query q;
	read(j, "name", q.name);
	read(j, "providers", q.providers);
	read(j, "project", q.project);
	read(j, "projectExact", q.projectExact);
	read(j, "model", q.model);
	read(j, "lastDays", q.lastDays);
	read(j, "groupBy", q.groupBy);
	read(j, "format", q.format);
	read(j, "autoExport", q.autoExport);
	read(j, "lastExport", q.lastExport);
	return q;
}

Settings Settings::load() {
	Settings s;
	std::ifstream in(Paths::settingsFile());
	if (in) {
		try {
			ofJson j = ofJson::parse(in);
			read(j, "refreshSeconds", s.refreshSeconds);
			read(j, "animations", s.animations);
			read(j, "startTab", s.startTab);
			read(j, "theme", s.theme);
			read(j, "glow", s.glow);
			read(j, "dataRain", s.dataRain);
			read(j, "redAlert", s.redAlert);
			read(j, "welcomed", s.welcomed);
			read(j, "hints", s.hints);
			read(j, "showAdvanced", s.showAdvanced);
			read(j, "intro", s.intro);
			read(j, "city", s.city);
			read(j, "flyTabs", s.flyTabs);
			read(j, "frameRate", s.frameRate);
			read(j, "vsync", s.vsync);
			read(j, "showFps", s.showFps);
			read(j, "unfocused", s.unfocused);
			read(j, "unfocusedRate", s.unfocusedRate);
			read(j, "stopHidden", s.stopHidden);
			read(j, "readHidden", s.readHidden);
			read(j, "resizable", s.resizable);
			read(j, "titleBar", s.titleBar);
			read(j, "fullscreen", s.fullscreen);
			read(j, "rememberWindow", s.rememberWindow);
			read(j, "windowX", s.windowX);
			read(j, "windowY", s.windowY);
			read(j, "windowW", s.windowW);
			read(j, "windowH", s.windowH);
			read(j, "windowAntialias", s.windowAntialias);
			read(j, "uiScale", s.uiScale);
			read(j, "glowPasses", s.glowPasses);
			read(j, "cityAntialias", s.cityAntialias);
			read(j, "cityWindow", s.cityWindow);
			read(j, "language", s.language);
			read(j, "windowOpacity", s.windowOpacity);
			read(j, "seeThrough", s.seeThrough);
			read(j, "backgroundAlpha", s.backgroundAlpha);
			read(j, "alwaysOnTop", s.alwaysOnTop);
			if (j.contains("providers") && j["providers"].is_array())
				for (const auto & pj : j["providers"]) {
					ProviderSettings p;
					read(pj, "id", p.id);
					read(pj, "enabled", p.enabled);
					read(pj, "path", p.path);
					read(pj, "planPrice", p.planPrice);
					read(pj, "accountId", p.accountId);
					read(pj, "billing", p.billing);
					read(pj, "bookmark", p.bookmark);
					read(pj, "fileBookmark", p.fileBookmark);
					read(pj, "apiBudget", p.apiBudget);
					if (!p.id.empty()) s.providers.push_back(p);
				}
			// the first version kept an order and a list of disabled providers
			std::vector<std::string> order, disabled;
			read(j, "order", order);
			read(j, "disabled", disabled);
			for (const auto & id : order) s.provider(id).enabled = std::find(disabled.begin(), disabled.end(), id) == disabled.end();
			read(j, "warnPercent", s.warnPercent);
			read(j, "toastLimits", s.toastLimits);
			read(j, "toastProblems", s.toastProblems);
			read(j, "staleHours", s.staleHours);
			read(j, "exportFolder", s.exportFolder);
			read(j, "fileName", s.fileName);
			read(j, "exportTotals", s.exportTotals);
			read(j, "revealAfterExport", s.revealAfterExport);
			read(j, "keepExports", s.keepExports);
			if (j.contains("queries") && j["queries"].is_array())
				for (const auto & qj : j["queries"]) s.queries.push_back(queryFrom(qj));
			read(j, "showTitles", s.showTitles);
			read(j, "exportTitles", s.exportTitles);
			read(j, "serveTitles", s.serveTitles);
			read(j, "readPrompts", s.readPrompts);
			read(j, "exportPrompts", s.exportPrompts);
			read(j, "apiMinutes", s.apiMinutes);
			read(j, "ledgerDays", s.ledgerDays);
			read(j, "serveApi", s.serveApi);
			read(j, "allowLan", s.allowLan);
			read(j, "trustLocal", s.trustLocal);
			read(j, "apiPort", s.apiPort);
			read(j, "apiToken", s.apiToken);
			read(j, "remoteUrl", s.remoteUrl);
			read(j, "remoteToken", s.remoteToken);
		} catch (const std::exception & e) {
			ofLogError("Settings") << "settings.json unreadable, using defaults: " << e.what();
		}
	}
	// a provider added in a later version goes at the end
	for (const char * id : { "claude", "codex", "grok", "muse" }) s.provider(id);
	s.refreshSeconds = std::max(10, s.refreshSeconds);
	if (s.queries.empty()) {
		// two to start from: what each project cost, and every day
		Query byProject;
		Query byDay;
		byDay.name = "Daily usage, last 30 days";
		byDay.groupBy = "day";
		byDay.lastDays = 30;
		s.queries = { byProject, byDay };
	}
	if (s.apiToken.empty()) {
		s.apiToken = newToken();
		s.save();
	}
	return s;
}

void Settings::save() const {
	ofJson providerList = ofJson::array(), queryList = ofJson::array();
	for (const auto & p : providers)
		providerList.push_back({ { "id", p.id }, { "enabled", p.enabled }, { "path", p.path }, { "planPrice", p.planPrice },
		                         { "accountId", p.accountId }, { "billing", p.billing }, { "bookmark", p.bookmark },
		                         { "fileBookmark", p.fileBookmark }, { "apiBudget", p.apiBudget } });
	for (const auto & q : queries) queryList.push_back(queryJson(q));
	ofJson j = {
		{ "refreshSeconds", refreshSeconds },
		{ "animations", animations },
		{ "startTab", startTab },
		{ "theme", theme },
		{ "glow", glow },
		{ "dataRain", dataRain },
		{ "redAlert", redAlert },
		{ "welcomed", welcomed },
		{ "hints", hints },
		{ "showAdvanced", showAdvanced },
		{ "intro", intro },
		{ "city", city },
		{ "flyTabs", flyTabs },
		{ "frameRate", frameRate },
		{ "vsync", vsync },
		{ "showFps", showFps },
		{ "unfocused", unfocused },
		{ "unfocusedRate", unfocusedRate },
		{ "stopHidden", stopHidden },
		{ "readHidden", readHidden },
		{ "resizable", resizable },
		{ "titleBar", titleBar },
		{ "fullscreen", fullscreen },
		{ "rememberWindow", rememberWindow },
		{ "windowX", windowX },
		{ "windowY", windowY },
		{ "windowW", windowW },
		{ "windowH", windowH },
		{ "windowAntialias", windowAntialias },
		{ "uiScale", uiScale },
		{ "glowPasses", glowPasses },
		{ "cityAntialias", cityAntialias },
		{ "cityWindow", cityWindow },
		{ "language", language },
		{ "windowOpacity", windowOpacity },
		{ "seeThrough", seeThrough },
		{ "backgroundAlpha", backgroundAlpha },
		{ "alwaysOnTop", alwaysOnTop },
		{ "providers", providerList },
		{ "warnPercent", warnPercent },
		{ "toastLimits", toastLimits },
		{ "toastProblems", toastProblems },
		{ "staleHours", staleHours },
		{ "exportFolder", exportFolder },
		{ "fileName", fileName },
		{ "exportTotals", exportTotals },
		{ "revealAfterExport", revealAfterExport },
		{ "keepExports", keepExports },
		{ "queries", queryList },
		{ "showTitles", showTitles },
		{ "exportTitles", exportTitles },
		{ "serveTitles", serveTitles },
		{ "readPrompts", readPrompts },
		{ "exportPrompts", exportPrompts },
		{ "apiMinutes", apiMinutes },
		{ "ledgerDays", ledgerDays },
		{ "serveApi", serveApi },
		{ "allowLan", allowLan },
		{ "trustLocal", trustLocal },
		{ "apiPort", apiPort },
		{ "apiToken", apiToken },
		{ "remoteUrl", remoteUrl },
		{ "remoteToken", remoteToken },
	};
	std::ofstream out(Paths::settingsFile());
	out << j.dump(2) << "\n";
	out.close();
	std::error_code ec;
	fs::permissions(Paths::settingsFile(), fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace, ec);
}

} // namespace backburn
