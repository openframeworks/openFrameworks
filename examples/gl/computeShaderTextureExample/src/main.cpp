#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    // this example uses compute shaders which are only supported since
    // openGL 4.3
	ofSetOpenGLVersion(4,3);
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
