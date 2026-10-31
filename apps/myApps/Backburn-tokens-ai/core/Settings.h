// AI Token Backburn - what your AI coding tools have used, and what's left.
// Copyright (c) 2026 Daniel Rosser. MIT License.

#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace backburn {

namespace fs = std::filesystem;

// Where things live: the tools' own files (read only, desktop) and this app's data.
struct Paths {
	static fs::path home();
	// ~/Library/Application Support/AI Token Backburn, %LOCALAPPDATA%\AI Token Backburn, $XDG_DATA_HOME/ai-token-backburn;
	// ofToDataPath on iOS / Android
	static fs::path appData();
	static fs::path settingsFile();
	static fs::path ledgerFile();   // usage rows, kept after the tools delete their logs
	static fs::path logFile();
	static fs::path defaultExports();   // ~/Documents/BackBurnTokens, or exports/ in the app data on a phone
	// a different app data folder for this run (--simulate-sandbox uses a fresh one, like a new container)
	static void overrideAppData(const fs::path & folder);
};

struct ProviderSettings {
	std::string id;
	bool enabled = true;
	std::string path;        // the tool's folder, if not the usual one (e.g. CLAUDE_CONFIG_DIR); "" for the default
	double planPrice = -1;   // what the plan costs a month, to compare the usage against; negative: not set
	// for the provider's own API (the key itself is in the Keychain, see Secrets.h): xAI's team id
	std::string accountId;
	// how use is shown: "auto" (a subscription when the tool reports plan limits), "subscription" (% of the
	// weekly limit), "tokens" (pay per token: dollars)
	std::string billing = "auto";
	// the folder (and Claude's ~/.claude.json) as picked on the Sources page; in the sandboxed build these are
	// security-scoped bookmarks, resumed at launch
	std::string bookmark, fileBookmark;
	// a monthly budget for the provider's API organization, to show this month's spend as a share of; negative: none
	double apiBudget = -1;
};

// A saved question about usage: which tools, projects, models and days, grouped how, exported as what.
struct Query {
	std::string name = "Usage by project";
	std::vector<std::string> providers;   // empty: all
	std::string project;                  // part of the folder path; empty: all
	bool projectExact = false;            // the whole path, for one project's own export
	std::string model;                    // part of the model name; empty: all
	int lastDays = 0;                     // the last N days; 0: every day
	std::string groupBy = "project";      // project, day, model, provider, row (day x project x model)
	std::string format = "csv";           // csv, json, md
	bool autoExport = false;              // export once a day while the app runs
	int64_t lastExport = 0;
};

// Saved to settings.json in the app data folder. Missing keys keep their defaults.
struct Settings {
	// General
	int refreshSeconds = 60;
	bool animations = true;
	std::string theme = "neon";      // "neon" (glowing type on black, drifting data) or "classic"
	float glow = 0.8f;               // bloom over the whole window in the neon theme, 0 for none
	bool dataRain = true;            // the drifting columns of digits behind the neon theme
	bool redAlert = true;            // the neon theme turns red while something's gone bad
	bool hints = false;              // grey explanations and explaining tooltips (Settings > General > Advanced)
	bool showAdvanced = false;       // the Advanced settings page (every setting, listed) is offered
	bool welcomed = false;           // the Sources page has been seen (it opens first until then)
	// the 3D city is paused for now (AGENTS.md, plan 1): off unless turned on in Settings > General > Look
	bool intro = false;              // fly into the city at launch
	bool city = false;               // the city stays behind the panels (else the theme's backdrop)
	bool flyTabs = false;            // opening a tool's tab flies to its tower
	// Graphics: speed, sharpness and size
	int frameRate = 30;              // the most frames a second; 0 for no limit
	bool vsync = true;
	bool showFps = false;            // frames a second, at the top right of the window
	// in the background
	std::string unfocused = "slow";  // when another app is in front: "full", "slow" (unfocusedRate), "freeze" (slow, still)
	int unfocusedRate = 10;
	bool stopHidden = true;          // no drawing while minimised, on another Space or fully covered
	bool readHidden = true;          // keep reading the logs while hidden (the phone API stays fresh)
	// the window (GLFW)
	bool resizable = true;
	bool titleBar = true;            // GLFW's decorated
	bool fullscreen = false;
	bool rememberWindow = true;      // size and position, restored at launch
	int windowX = -1, windowY = -1, windowW = -1, windowH = -1;   // in screen points; -1 until saved
	int windowAntialias = 0;         // the window's multisampling, set when it's made: from the next start
	float uiScale = 1.0f;            // text and controls, 0.8..1.6
	int glowPasses = 2;              // blur passes for the glow: more is wider and softer, and costs more
	int cityAntialias = 4;           // the 3D city's multisampling: 0, 2, 4 or 8
	// the city in a second GLFW window, for debugging (from the next start): "" none, "texture" (the main window's
	// frame, through the shared context), "native" (drawn again in the second window's own context)
	std::string cityWindow;
	std::string language;            // interface labels (lang/<code>.json); "" for English
	// the window (macOS, GLFW): the whole window faded, the desktop showing behind the panels, kept on top
	float windowOpacity = 1.0f;      // 0.3..1, live
	bool seeThrough = false;         // a transparent framebuffer: set when the window is made, so from the next start
	float backgroundAlpha = 0.35f;   // with seeThrough: how much of the background is drawn, live
	bool alwaysOnTop = false;        // live
	std::string startTab = "overview";

	// Providers: shown and read in this order (Claude first)
	std::vector<ProviderSettings> providers;

	// Alerts
	int warnPercent = 80;            // a limit counts as nearly used from here
	bool toastLimits = true;         // a toast when a limit crosses warnPercent
	bool toastProblems = true;       // a toast for a new problem (a balance running out, sign-in failing)
	int staleHours = 1;              // limits older than this are flagged

	// Export
	std::string exportFolder;        // "" for Paths::defaultExports()
	std::string fileName = "{query} {date}";   // {query} {date} {time} {provider}
	bool exportTotals = true;        // a totals row / object at the end
	bool revealAfterExport = true;   // show the file in Finder
	int keepExports = 0;             // per query, oldest deleted beyond this; 0 keeps all
	std::vector<Query> queries;

	// Privacy: task titles are the tools' names for sessions, often your own first words to them
	bool showTitles = true;          // in this window
	bool exportTitles = true;        // in exports
	bool serveTitles = false;        // over the API (the phones): off until asked for
	// the prompts themselves, in a task's breakdown: read from the tool's file when a task is opened, never kept in
	// the ledger, never served; each off until asked for
	bool readPrompts = false;        // show each prompt's words
	bool exportPrompts = false;      // put them in a task's export

	// Provider APIs, with the keys in Settings > Providers
	int apiMinutes = 15;             // how often they're asked; the Anthropic report allows once a minute

	// Data
	int ledgerDays = 0;              // forget ledger rows older than this; 0 keeps all

	// API for phones: GET /api/usage, /api/query with "Authorization: Bearer <apiToken>"
	bool serveApi = true;
	bool allowLan = false;   // listen on every interface, not just this machine
	bool trustLocal = true;  // this computer needs no token (a browser here can open the page); other devices do
	int apiPort = 8790;
	std::string apiToken;    // generated on first run

	// set on a phone, or on a desktop showing another computer: read that computer's API instead of local logs
	std::string remoteUrl;   // http://192.168.1.20:8790
	std::string remoteToken;

	bool enabled(const std::string & id) const;
	const ProviderSettings & provider(const std::string & id) const;
	ProviderSettings & provider(const std::string & id);
	fs::path exports() const;

	static std::string newToken();
	static Settings load();
	void save() const;
};

} // namespace backburn
