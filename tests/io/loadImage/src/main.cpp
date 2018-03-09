#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

class ofApp: public ofxUnitTestsApp{
	void run(){
		ofImage img;
		img.setUseTexture(false);
		test(img.load("indispensable.jpg"), "load from fs");
		test(img.load(ofToDataPath("indispensable.jpg", true)), "load from fs");
		test(img.load("http://openframeworks.cc/about/0.jpg"), "load from http");
		test(img.load("https://forum.openframeworks.cc/user_avatar/forum.openframeworks.cc/arturo/45/3965_1.png"), "load from https");
	}
};

//========================================================================
int main( ){
	ofInit();
	auto window = make_shared<ofAppNoWindow>();
	auto app = make_shared<ofApp>();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(window, app);
	return ofRunMainLoop();

}
