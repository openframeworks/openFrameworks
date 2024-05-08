#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.setSize(600, 400);
	settings.windowMode = OF_WINDOW;
//	settings.doubleBuffering = false;
	auto mainApp { make_shared<ofApp>() };
	auto window { ofCreateWindow(settings) };

	ofRunApp(window, mainApp);
	ofRunMainLoop();
}
