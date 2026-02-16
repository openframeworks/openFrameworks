#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

class ofApp: public ofxUnitTestsApp{
	void run(){
		// Test case from Issue #4871
		// ofRectangle(0, 0, 100, 100).intersects(ofPoint(0, 50), ofPoint(50, 0))
		// should return true because the line segment has both endpoints on the rectangle boundary
		ofRectangle rect1(0, 0, 100, 100);
		ofxTest(rect1.intersects(glm::vec2(0, 50), glm::vec2(50, 0)),
			"Issue #4871: Line segment with endpoints on rectangle boundary should intersect");

		// Additional boundary tests
		// Points exactly on corners
		ofRectangle rect2(0, 0, 100, 100);
		ofxTest(rect2.inside(glm::vec2(0, 0)), "Bottom-left corner should be inside");
		ofxTest(rect2.inside(glm::vec2(100, 0)), "Bottom-right corner should be inside");
		ofxTest(rect2.inside(glm::vec2(0, 100)), "Top-left corner should be inside");
		ofxTest(rect2.inside(glm::vec2(100, 100)), "Top-right corner should be inside");

		// Points on edges
		ofRectangle rect3(0, 0, 100, 100);
		ofxTest(rect3.inside(glm::vec2(50, 0)), "Point on bottom edge should be inside");
		ofxTest(rect3.inside(glm::vec2(50, 100)), "Point on top edge should be inside");
		ofxTest(rect3.inside(glm::vec2(0, 50)), "Point on left edge should be inside");
		ofxTest(rect3.inside(glm::vec2(100, 50)), "Point on right edge should be inside");

		// Points clearly inside
		ofRectangle rect4(0, 0, 100, 100);
		ofxTest(rect4.inside(glm::vec2(50, 50)), "Center point should be inside");
		ofxTest(rect4.inside(glm::vec2(25, 25)), "Interior point should be inside");

		// Points clearly outside
		ofRectangle rect5(0, 0, 100, 100);
		ofxTest(!rect5.inside(glm::vec2(-1, 50)), "Point left of rectangle should be outside");
		ofxTest(!rect5.inside(glm::vec2(101, 50)), "Point right of rectangle should be outside");
		ofxTest(!rect5.inside(glm::vec2(50, -1)), "Point below rectangle should be outside");
		ofxTest(!rect5.inside(glm::vec2(50, 101)), "Point above rectangle should be outside");

		// Line intersection tests - both endpoints inside
		ofRectangle rect6(0, 0, 100, 100);
		ofxTest(rect6.intersects(glm::vec2(25, 25), glm::vec2(75, 75)),
			"Line with both endpoints inside should intersect");

		// Line intersection tests - both endpoints outside, but line crosses
		ofRectangle rect7(0, 0, 100, 100);
		ofxTest(rect7.intersects(glm::vec2(-10, 50), glm::vec2(110, 50)),
			"Line crossing horizontally should intersect");
		ofxTest(rect7.intersects(glm::vec2(50, -10), glm::vec2(50, 110)),
			"Line crossing vertically should intersect");

		// Line intersection tests - one endpoint inside, one outside
		ofRectangle rect8(0, 0, 100, 100);
		ofxTest(rect8.intersects(glm::vec2(50, 50), glm::vec2(150, 150)),
			"Line with one endpoint inside should intersect");

		// Line intersection tests - both endpoints outside, line doesn't cross
		ofRectangle rect9(0, 0, 100, 100);
		ofxTest(!rect9.intersects(glm::vec2(-10, -10), glm::vec2(-5, -5)),
			"Line completely outside should not intersect");

		// Diagonal line through corners (boundary case)
		ofRectangle rect10(0, 0, 100, 100);
		ofxTest(rect10.intersects(glm::vec2(0, 0), glm::vec2(100, 100)),
			"Diagonal line through corners should intersect");
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
