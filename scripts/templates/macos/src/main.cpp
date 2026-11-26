#include "ofMain.h"
#include "ofApp.h"

#if defined(TARGET_OF_IOS)
#include "ofAppiOSWindow.h"
#endif

//========================================================================
int main( ){

#if defined(TARGET_OF_IOS)
	ofiOSWindowSettings settings;
	settings.windowControllerType = ofxiOSWindowControllerType::GL_KIT;
	settings.windowMode = OF_FULLSCREEN;
	ofCreateWindow(settings);
	return ofRunApp(new ofApp);
#else
	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
#endif

}
