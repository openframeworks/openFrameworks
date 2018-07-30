#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

class ofApp: public ofxUnitTestsApp{
	void run(){
		ofParameter<float> p1{"p1", 0, 0, 1000};
		ofParameter<float> p2{"p2", 0, 0, 1000};
		ofParameter<float> p3{"p3", 0, 0, 1000};
		ofParameter<float> p4{"p4", 0, 0, 1000};
		ofParameterGroup group{
			"group",
			p1, p2, p3, p4
		};
		group.remove(p1);
		ofxTest(!group.contains("p1"), "Group shouldn't contain p1 after remove");
		ofxTestEq(group.get("p2").getName(), "p2", "p2 name should be p2, probably index map is corrupt"); //Issue #6016
		ofxTestEq(group.get("p3").getName(), "p3", "p3 name should be p3, probably index map is corrupt"); //Issue #6016
		ofxTestEq(group.get("p4").getName(), "p4", "p4 name should be p4, probably index map is corrupt"); //Issue #6016
		group.remove(p2);
		ofxTest(!group.contains("p2"), "Group shouldn't contain p2 after remove");
		ofxTestEq(group.get("p3").getName(), "p3", "p3 name should be p3, probably index map is corrupt"); //Issue #6016
		ofxTestEq(group.get("p4").getName(), "p4", "p4 name should be p4, probably index map is corrupt"); //Issue #6016
		group.remove(p3);
		ofxTest(!group.contains("p3"), "Group shouldn't contain p2 after remove");
		ofxTestEq(group.get("p4").getName(), "p4", "p4 name " + group.get("p4").getName() + " should be p4, probably index map is corrupt"); //Issue #6016
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
