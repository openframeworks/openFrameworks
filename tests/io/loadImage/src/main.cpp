#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

class ofApp: public ofxUnitTestsApp{
	void run(){
		ofImage img;
		img.setUseTexture(false);
		ofxTest(img.load("indispensable.jpg"), "load from fs");
		ofxTest(img.load(ofToDataPath("indispensable.jpg", true)), "load from fs");
		ofxTest(img.load("http://openframeworks.cc/about/0.jpg"), "load from http");
		ofxTest(img.load("https://forum.openframeworks.cc/user_avatar/forum.openframeworks.cc/arturo/45/3965_1.png"), "load from https");
	}
};

//========================================================================
int main( ){
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(window, app);
	return ofRunMainLoop();

}
