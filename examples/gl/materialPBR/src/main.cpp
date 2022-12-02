#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1200, 768);
	// PBR Materials only work with programmable renderer
	settings.setGLVersion(3,2);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN
	
	auto window = ofCreateWindow(settings);
	
	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}
