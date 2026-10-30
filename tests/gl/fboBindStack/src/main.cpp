#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include "fboBindCases.h"

class ofApp : public ofxUnitTestsApp {
	void run() {
		std::string details;
		const int failed = runFboBindCases(details);
		ofxTestEq(failed, 0, details.empty() ? "framebuffer bind stack" : details);
	}
};

int main() {
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
