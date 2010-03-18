#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main( ){

	ofSetupOpenGL(1024,768, OF_FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
