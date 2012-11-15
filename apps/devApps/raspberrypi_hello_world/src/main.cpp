#include "ofMain.h"
#include "testApp.h"
#include "ofxRaspberryPiWindow.h"
#include "ofGLES2Renderer.h"

//========================================================================
int main( ){
    ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLES2Renderer()));

	ofxRaspberryPiWindow window;
	ofSetupOpenGL(&window, 1024,768, OF_WINDOW);// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
