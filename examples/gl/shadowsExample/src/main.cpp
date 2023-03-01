#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	
#ifdef TARGET_OPENGLES
	ofGLESWindowSettings settings;
	settings.glesVersion = 3;
#else

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1200, 768);
	// shadows only work with programmable renderer
	// point lights work in 3.3, but is limited to 1
//	settings.setGLVersion(3,3);
	// multiple point lights only work with OpenGL 4+
	settings.setGLVersion(4,1);
#endif
	
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN
	auto window = ofCreateWindow(settings);
	
	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}
