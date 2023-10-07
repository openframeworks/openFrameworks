#include "utils/ofUtils.h"
#include "ofxUnitTests.h"


class ofApp: public ofxUnitTestsApp{
	void run(){
		ofLogNotice() << "testing utils/ofUtils";
		ofLogNotice() << "testing ofGetEnv() on unset environment variable";
		ofxTest(ofGetEnv("DUMMY")=="", "it should return an empty string when called with no default value.");
		ofxTest(ofGetEnv("DUMMY","default")=="default", "it should return the deefault when it is provided.");
		ofLogNotice() << "testing ofGetEnv() on set environment variable (PATH)";
		ofxTest(ofGetEnv("PATH")!="", "it should return a (non empty) string when called with no default value.");
		ofxTest(ofGetEnv("DUMMY","default")!="", "it should return a (non empty) string when a default value is provided.");
		ofxTest(ofGetEnv("DUMMY","default")!="defautl", "it should not return the default value.");
	}
};


#include "app/ofAppNoWindow.h"
#include "app/ofAppRunner.h"
//========================================================================
int main( ){
    ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
