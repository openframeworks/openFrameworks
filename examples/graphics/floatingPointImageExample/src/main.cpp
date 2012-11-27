#include "ofMain.h"
#include "testApp.h"
#ifndef TARGET_OPENGLES
#include "ofAppGlutWindow.h"
#endif

//========================================================================
int main( ){
	#ifndef TARGET_OPENGLES
	ofAppGlutWindow window;

	// antialiasing hacks
	//window.setGlutDisplayString("rgba double samples>=4 depth"); //uncomment for mac fsaa
	//window.setGlutDisplayString("rgb double depth alpha samples>=4") //uncomment for pc fsaa

	ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context
	#else
	ofSetupOpenGL(1024,768,OF_WINDOW);
	#endif

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
