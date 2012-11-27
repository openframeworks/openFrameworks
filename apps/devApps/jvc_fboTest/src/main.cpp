#include "ofMain.h"
#include "testApp.h"
#include "ofAppRaspberryPiWindow.h"
#include "ofGLES2Renderer.h"

//========================================================================
int main( ){
    ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLES2Renderer()));

	ofAppRaspberryPiWindow window;
	ofSetupOpenGL(&window, 1280,720, OF_WINDOW);// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
