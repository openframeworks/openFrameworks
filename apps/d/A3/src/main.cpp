#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.setSize(600, 400);
//	settings.setPosition({ 120, 100 });
	settings.windowMode = OF_WINDOW;
	settings.doubleBuffering = false;
//	settings.opacity = 0.85;
//	settings.doubleBuffering = true;

//	settings.monitor = 1;
	//	settings.windowMode = OF_FULLSCREEN;
//	settings.setGLVersion(3, 2);
//	settings.maximized = true;
//	settings.iconified = true;
//	settings.multiMonitorFullScreen = true;
	auto mainApp { make_shared<ofApp>() };
	auto window { ofCreateWindow(settings) };
	

#define SECOND
#ifdef SECOND
	settings.windowName = "second";
	settings.shareContextWith = window;
	
	settings.setPosition({ 600, 600 });
	settings.setSize(1200, 300);

//	settings.doubleBuffering = true;
//	settings.windowMode = OF_FULLSCREEN;
//	settings.monitor = 1;
//	settings.windowMode = OF_GAME_MODE;
//	settings.fullscreenDisplays = { 1, 2, 3 };
	settings.showOnlyInSelectedMonitor = true;
	
	auto newWindow { ofCreateWindow(settings) };
//	ofAddListener(newWindow->events().draw, mainApp.get(), &ofApp::drawSecondWindow);
#endif
	
	ofRunApp(window, mainApp);
	ofRunMainLoop();

}
