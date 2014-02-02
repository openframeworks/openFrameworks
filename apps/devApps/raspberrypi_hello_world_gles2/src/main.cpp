#include "ofMain.h"
#include "ofApp.h"
#include "ofGLES2Renderer.h"

//========================================================================
int main( ){

    ofGLES2Renderer * renderer = new ofGLES2Renderer();
	ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(renderer)); // switch to GLES2 renderer.

	ofSetupOpenGL(1024,768, OF_WINDOW);// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
