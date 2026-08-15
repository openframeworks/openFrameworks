#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

class ofApp: public ofxUnitTestsApp{
	void run(){
		ofImage img;
		img.setUseTexture(false);
		ofxTest(img.load("indispensable.jpg"), "load from fs");
		ofxTest(img.load(ofToDataPath("indispensable.jpg", true)), "load from fs");
		// verbose http debug - logs status/error/data.size for CI 2/4 diagnosis (safe, no version->host)
		{
			ofHttpRequest req("http://openframeworks.cc/about/0.jpg", "http", false, true, true);
			auto resp = ofURLFileLoader().handleRequest(req);
			ofLogNotice() << "http resp status=" << resp.status << " error='" << resp.error << "' data.size=" << resp.data.size();
			ofxTest(resp.status==200 && resp.data.size()>0, "load from http");
		}
		{
			ofHttpRequest req("https://avatars.githubusercontent.com/u/48240?v=4", "https", false, true, true);
			auto resp = ofURLFileLoader().handleRequest(req);
			ofLogNotice() << "https resp status=" << resp.status << " error='" << resp.error << "' data.size=" << resp.data.size();
			ofxTest(resp.status==200 && resp.data.size()>0, "load from https");
		}
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
