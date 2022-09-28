#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofSetLogLevel(OF_LOG_VERBOSE);
	int windowWidth = 1024;
	int windowHeight = 500;
	
	#ifdef TARGET_OPENGLES
    	ofGLESWindowSettings settings;
		settings.width = windowWidth;
		settings.height = windowHeight;
		settings.setGLESVersion(2);
		
		auto window = ofCreateWindow(settings);
		ofRunApp(window, make_shared<ofApp>());
	#else
		ofGLWindowSettings settings;
		settings.setSize(windowWidth, windowHeight);
		settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

		auto window = ofCreateWindow(settings);
		ofRunApp(window, make_shared<ofApp>());
	#endif
	
	ofRunMainLoop();

}
