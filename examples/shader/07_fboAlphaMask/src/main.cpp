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

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();
}
