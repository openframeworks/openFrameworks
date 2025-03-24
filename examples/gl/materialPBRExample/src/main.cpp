#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
// PBR only works with the programmable renderer
#ifdef TARGET_EMSCRIPTEN
	ofGLESWindowSettings settings;
	settings.glesVersion = 3;
#else
	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1200, 768);
	settings.setGLVersion(3,2);
#endif
	
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN
	auto window = ofCreateWindow(settings);
	
	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
	
}
