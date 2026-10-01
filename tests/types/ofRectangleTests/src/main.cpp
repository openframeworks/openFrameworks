#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include <cmath>

class ofApp: public ofxUnitTestsApp{
	void run(){
		ofRectangle a(0, 0, 1, 1);
		ofRectangle b(0, 0, 1, 1);
		ofxTest(a == b, "identical rectangles should compare equal");
		ofxTest(!(a != b), "identical rectangles should not compare unequal");

		// width differs from a's by exactly one float ULP, well inside the
		// epsilon tolerance that operator== uses (ofIsFloatEqual), but
		// operator!= used to compare the raw fields directly, so the two
		// operators disagreed and both returned true for the same pair.
		ofRectangle c(0, 0, std::nextafter(1.0f, 2.0f), 1);
		ofxTest(a == c, "rectangles differing by one float ULP should compare equal under epsilon tolerance");
		ofxTest(!(a != c), "operator!= must be the logical negation of operator==");

		ofRectangle d(0, 0, 2, 2);
		ofxTest(a != d, "clearly different rectangles should compare unequal");
		ofxTest(!(a == d), "clearly different rectangles should not compare equal");
	}
};

//========================================================================
int main( ){
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
