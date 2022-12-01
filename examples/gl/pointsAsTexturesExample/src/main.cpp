#include "ofMain.h"
#include "ofApp.h"
#if defined TARGET_OPENGLES && not defined TARGET_EMSCRIPTEN
	#include "ofGLProgrammableRenderer.h"
#endif
//========================================================================
int main( ){

	ofSetLogLevel(OF_LOG_VERBOSE);
	#if defined TARGET_OPENGLES && not defined TARGET_EMSCRIPTEN
		ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	#endif

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}
