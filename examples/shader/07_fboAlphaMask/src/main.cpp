#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

#ifdef OF_TARGET_OPENGLES
	ofGLESWindowSettings settings;
	settings.glesVersion=2;
#else
	ofGLWindowSettings settings;
	settings.setGLVersion(3,2);
#endif
    // create a window for display
	ofCreateWindow(settings);
    
	// this kicks off the running of my app
	ofRunApp(make_shared<ofApp>());

}
