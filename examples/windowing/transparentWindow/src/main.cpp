#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLFWWindowSettings settings;
	settings.transparent = true;
	settings.mousePassThrough = true;
	settings.floating = true;
	settings.decorated = false;
	settings.windowMode = OF_WINDOW;
	settings.setGLVersion(4, 1);
	settings.setSize(1280, 720);
	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();

}
