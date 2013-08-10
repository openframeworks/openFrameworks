#include "ofMain.h"
#include "testApp.h"
#include "ofGLProgrammableRenderer.h"

//========================================================================
int main( ){
	
	// tig: we are using the programmable GL renderer for this, it's more fun to write shaders in GLSL 150 =)
	// but note that it should be no problem to backport the shaders to GLSL 120.
	// For legacy shaders, see also: http://poniesandlight.co.uk/code/ofxVboMeshInstanced/
	
	ofPtr<ofBaseRenderer> renderer(new ofGLProgrammableRenderer(false));
	ofSetCurrentRenderer(renderer, false);
	
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
