#include "ofAppRunner.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

class ofApp: public ofxUnitTestsApp{
	void run(){

	}
};


int main(){

	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();

	ofRunApp(window,app);

	return ofRunMainLoop();
}
