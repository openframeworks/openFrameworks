#include "ofMain.h"
#include "ofApp.h"

// by default this example will run in OpenGL 2.0, see ofApp.h for running it
// in OpenGL 3.2

#ifdef USE_PROGRAMMABLE_GL
// tig: using the programmable GL renderer is more fun, since we can use GLSL 150 =)
// define USE_PROGRAMMABLE_GL in ofApp.h to run this example in OpenGL 3.2 if your
// system provides it...
#include "ofGLProgrammableRenderer.h"
#endif

//========================================================================
int main( ){
	

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	
	#ifdef USE_PROGRAMMABLE_GL
		settings.setGLVersion(3,2);
	#endif
	
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();
}
