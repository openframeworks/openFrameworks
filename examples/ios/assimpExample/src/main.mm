#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofAppiOSWindow * window = new ofAppiOSWindow();
    window->enableRetina();
	window->enableDepthBuffer();
	ofSetupOpenGL(window, 1024,768, OF_FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
