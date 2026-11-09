// AI Token Backburn - what your AI coding tools have used, and what's left.
// Copyright (c) 2026 Daniel Rosser. MIT License.

#pragma once

#include "Access.h"
#include "City.h"
#include "Fonts.h"
#include "Collector.h"
#include "QueryExport.h"
#include "Prompts.h"
#include "LogBuffer.h"
#include "Settings.h"
#include "Usage.h"

#include "ofMain.h"
#include "ofxImGui.h"

#include <array>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <set>
#include <vector>

class ofApp : public ofBaseApp {
public:
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;
	void keyPressed(int key) override;
	void mousePressed(int x, int y, int button) override;

	// CONNECTION TERMINATED over the whole window; quit: exit when it's done
	void terminate(bool quit);

	std::string screenshotPath;   // --screenshot=file: save the window once the first report is in
	std::string startTab;         // --tab=claude, --tab=queries, --tab=settings/export: open on that tab
	std::string startDay;         // --day=2026-10-26 with --tab=claude: open that day's tasks
	bool forceAlert = false;      // --alert: show red alert, to see it
	std::string startTheme;       // --theme=matrix: this run only, the saved theme is left alone
	std::string startTask;        // --task=<session id> with --tab=claude: open that task's prompts
	float screenshotAt = 4;       // --screenshot-at=seconds: when to save it
	std::string simulatedGrants;  // --simulate-grant=claude,codex: as if those folders were picked on the Sources page
	// a second window showing just the city (Settings > Graphics, or --city-window[=native]); native: drawn in its
	// own context rather than shown from the main window's frame
	void attachCityWindow(std::shared_ptr<ofAppBaseWindow> window, bool native);

private:
	void drawHeader();
	void drawSidebar();
	void drawOverview();
	void drawProvider(const backburn::Provider & p);
	void drawProject(const backburn::Provider & p, const backburn::ProjectUsage & project);
	// what ran on a day; project: only that project's
	void drawDayTasks(const backburn::Provider & p, const std::string & day, const std::string * project);
	void drawTasks(const backburn::Provider & p, const std::string * project);
	// one task, broken down by the prompts in it (read from the tool's file on a background thread)
	void drawTask(const backburn::Provider & p, const backburn::TaskUsage & task);
	void exportPrompts(const backburn::Provider & p, const std::string & format);
	std::string taskName(const backburn::Provider & p, const std::string & session) const;
	void drawQueries();
	void drawQueryPreview(const backburn::Query & q);
	void drawLog();
	void drawSettings();
	void drawGeneralSettings();
	void drawGraphicsSettings();
	void drawProviderSettings();
	void drawAlertSettings();
	void drawExportSettings();
	void drawPrivacySettings();
	void drawApiSettings();
	void drawDataSettings();
	void drawCopyrightSettings();
	void drawAdvancedSettings();
	void drawHow();
	void toast(const std::string & text, const ImVec4 & colour);
	// the look: the theme's colours and faces, the glow and the data behind
	void setTheme();
	void applyWindow();
	// focus and visibility, each frame: frame rate, vsync, animation and reading follow Settings > Graphics
	void pace();
	void saveWindow();
	bool seeThroughNow = false;   // whether this window was made see-through (Settings change it from the next start)
	void drawGlow(bool withCity);
	// the city: fed from the report, drawn behind the interface
	void feedCity();
	void cityColours();
	void drawCity();
	void bloomCity();            // the city's own bloom: blur its frame and keep it for compositing
	ofFbo cityBloom;             // quarter-size blurred city, added back over the city when drawn
	bool cityShown() const;
	void drawCityWindow(ofEventArgs &);
	void cityWindowClosed(ofEventArgs &);
	std::shared_ptr<ofAppBaseWindow> cityWindow;
	bool cityNative = false;
	// the Sources page: each tool's folder, found or not, and access granted only on a click
	void drawSources(bool firstRun);
	// a folder picked on the Sources page: remember it, let the collector read it, say so
	void granted(backburn::ProviderSettings & ps, const std::string & path, const std::string & bookmark);
	void resumeAccess();
	void drawDataRain();
	void drawStreams();
	void drawSmoke();
	std::string rainLine(std::mt19937 & rng);
	void drawToasts();
	// save, and tell the collector; restartApi when the port, LAN or token changed
	void applySettings(bool restartApi = false);
	// export and say where it went; path: a file the user picked, else the export folder
	void runExport(const backburn::Query & q, const std::string & path = "");
	void autoExport();
	void selectQuery(int index);

	backburn::LogBuffer log;
	backburn::Settings settings;
	std::unique_ptr<backburn::Collector> collector;
	std::unique_ptr<backburn::Api> api;
	backburn::Report report;
	std::string lanIp;

	ofxImGui::Gui gui;
	std::string shownTab;
	int tabSwitches = 0;
	std::string startSubTab;      // a Settings page to open on
	int64_t seenReport = 0;
	// limits over the warning level and problems at the last report, to toast only what's new
	std::vector<std::string> warned;
	std::set<std::string> problemsSeen;
	bool warnPrimed = false;
	bool alarm = false;           // red alert: something has gone bad

	// the project open in each provider's tab
	std::map<std::string, std::string> openProject;
	// the day open under a chart: by provider, or provider|project
	std::map<std::string, std::string> openDay;
	char taskFilter[128] = "";
	std::map<std::string, std::string> openTask;   // provider -> session shown in its tab
	std::future<backburn::PromptBreakdown> promptLoad;
	backburn::PromptBreakdown prompts;
	std::string promptsKey;                        // provider|session|text: what prompts holds or is loading
	int openPrompt = -1;                           // its index, expanded under the table
	bool promptsByCost = true;
	std::string promptFormat = "csv";
	char projectFilter[128] = "", logFilter[128] = "";
	bool followLog = true;
	bool showToken = false;

	// the query being edited in Queries; saved to Settings by Save
	int selectedQuery = 0;
	backburn::Query editing;
	char queryName[128] = "", queryProject[256] = "", queryModel[128] = "";
	std::string lastExport;       // the last file written, for Reveal

	// Settings text fields, applied when they lose focus
	char remoteUrl[256] = "", remoteToken[128] = "";
	char exportFolder[512] = "", fileName[128] = "";
	std::map<std::string, std::array<char, 512>> folderFields;
	std::map<std::string, std::array<char, 256>> keyFields, accountFields;   // keys go to the Keychain, fields cleared
	double clearLedgerAsked = -10;

	void drawTerminated();
	double terminatedAt = -1;     // ofGetElapsedTimef when it started; -1 when not showing
	bool quitAfter = false;

	ofFbo scene, glowA, glowB, cityFbo;
	City city;
	bool sourcesOpen = false;
	// Donate: the author's PayPal and Pepe QR codes (data/donate, as on danoli3.com); not in the App Store build
#if !BACKBURN_APP_STORE
	void drawDonate();
	bool donateOpen = false;
	ofTexture paypalQr, pepeQr;
#endif
	int cityAntialias = -1;       // what cityFbo was made with
	bool focused = true, visible = true, drawing = true;
	int pacedRate = -1, pacedVsync = -1;
	float uiFade = 1;             // the interface's alpha: 0 at the start of the intro, and through a flight
	bool sidebarFolded = false, mainFolded = false;   // each main panel folds from its own top-right corner
	std::string flewTo;           // the tower the camera last flew to (or is heading to)
	// fly-through sequence: 0 idle, 1 fading the UI out, 2 flying with the UI off, 3 fading the UI back in
	int flyStage = 0;
	std::string pendingFlight;    // where stage 2 flies, once the UI is fully off
	ofShader blur;
	struct Column {
		float x = 0, depth = 0, speed = 0, offset = 0, size = 12;
		std::vector<std::string> lines;
	};
	std::vector<Column> rain;
	ImVec2 rainSize;
	struct Fall {
		float x = 0, head = 0, speed = 10;
		int length = 12;
		std::vector<std::string> chars;
	};
	std::vector<Fall> falls;
	ImVec2 fallsSize;
	struct Puff {
		float x = 0, y = 0, age = 0, life = 10, rise = 20, seed = 0;
		bool ember = false;
	};
	std::vector<Puff> puffs;

	struct Toast {
		std::string text;
		ImVec4 colour;
		double start;
		uint64_t id;
	};
	std::mutex toastMutex;
	std::vector<Toast> toasts;
	uint64_t toastCount = 0;
};
