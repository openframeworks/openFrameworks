#include "ofMain.h"
#include "ofApp.h"

// comment out the line below if you want to use a fixed pipeline opengl renderer,
// otherwise leave this line uncommented if you want to use a programmable pipeline opengl renderer.
#define USE_PROGRAMMABLE_RENDERER

#ifdef USE_PROGRAMMABLE_RENDERER
#include "ofGLProgrammableRenderer.h"
#endif

//========================================================================
int main( ){
    
#ifdef USE_PROGRAMMABLE_RENDERER
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
#endif
    
	ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
    
}
