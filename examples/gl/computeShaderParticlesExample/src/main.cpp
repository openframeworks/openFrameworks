#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	
	// this example uses compute shaders which are only supported since
	// openGL 4.3
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.setGLVersion(4,3);
	settings.windowMode = OF_FULLSCREEN;

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}
