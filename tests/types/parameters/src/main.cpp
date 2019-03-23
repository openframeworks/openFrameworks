#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

class ofApp: public ofxUnitTestsApp{
	void run(){
		ofParameter<float> p1{"p>1", 0, 0, 1000};
		ofParameter<float> p2{"p>2", 0, 0, 1000};
		ofParameter<float> p3{"p>3", 0, 0, 1000};
		ofParameter<float> p4{"p>4", 0, 0, 1000};
		ofParameterGroup group{
			"group",
			p1, p2, p3, p4
		};
		group.remove(p1);
		ofxTest(!group.contains("p>1"), "Group shouldn't contain p1 after remove");
		ofxTestEq(group.get("p>2").getName(), "p>2", "p2 name " + group.get("p>2").getName() + " should be p>2, probably index map is corrupt"); //Issue #6016
		ofxTestEq(group.get("p>3").getName(), "p>3", "p3 name " + group.get("p>3").getName() + " should be p>3, probably index map is corrupt"); //Issue #6016
		ofxTestEq(group.get("p>4").getName(), "p>4", "p4 name " + group.get("p>4").getName() + " should be p>4, probably index map is corrupt"); //Issue #6016
		group.remove(p2);
		ofxTest(!group.contains("p>2"), "Group shouldn't contain p2 after remove");
		ofxTestEq(group.get("p>3").getName(), "p>3", "p3 name " + group.get("p>3").getName() + " should be p>3, probably index map is corrupt"); //Issue #6016
		ofxTestEq(group.get("p>4").getName(), "p>4", "p4 name " + group.get("p>4").getName() + " should be p>4, probably index map is corrupt"); //Issue #6016
		group.remove(p3);
		ofxTest(!group.contains("p>3"), "Group shouldn't contain p2 after remove");
		ofxTestEq(group.get("p>4").getName(), "p>4", "p4 name " + group.get("p>4").getName() + " should be p>4, probably index map is corrupt"); //Issue #6016
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
