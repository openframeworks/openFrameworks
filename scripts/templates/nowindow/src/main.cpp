#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofApp.h"

//========================================================================
int main( ){
	auto window = make_shared<ofAppNoWindow>();
	auto app = make_shared<ofApp>();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(window, app);
	ofRunMainLoop();

}
