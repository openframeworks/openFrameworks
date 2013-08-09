#include "ofMain.h"
#include "testApp.h"


// by default this example will run in OpenGL 2.0, see testApp.h for running it
// in OpenGL 3.2

#ifdef USE_PROGRAMMABLE_GL
// tig: using the programmable GL renderer is more fun, since we can use GLSL 150 =)
// define USE_PROGRAMMABLE_GL in testApp.h to run this example in OpenGL 3.2 if your
// system provides it...
#include "ofGLProgrammableRenderer.h"
#endif

//========================================================================
int main( ){
	
if( glDrawElementsInstanced == 0 ){
    ofLogFatalError("App") << " glDrawElementsInstanced is needed for this example but it is not supported by your graphics card. Exiting App.";
    return -1;  
}
	
#ifdef USE_PROGRAMMABLE_GL
	ofPtr<ofBaseRenderer> renderer(new ofGLProgrammableRenderer(false));
	ofSetCurrentRenderer(renderer, false);
#endif
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
