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
		// ofxTest(img.load("https://forum.openframeworks.cc/user_avatar/forum.openframeworks.cc/arturo/45/3965_1.png"), "load from https");
		// ar 48240 / th 144000 / dm 58289
		ofxTest(img.load("https://avatars.githubusercontent.com/u/48240?v=4"), "load from https");
		
	}

};

//========================================================================
int main( ){
	ofInit();
	
	ofGLWindowSettings settings;
	settings.setSize(1200, 600);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

//	auto window = ofCreateWindow(settings);
	auto window = std::make_shared<ofAppNoWindow>(); // cant be no window doing GL stuff or bug
	auto app = std::make_shared<ofApp>();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(window, app);
	return ofRunMainLoop();

}
