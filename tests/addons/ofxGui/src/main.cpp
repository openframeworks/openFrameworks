#include "ofMain.h"
#include "ofxUnitTests.h"
#include "ofAppNoWindow.h"
#include "ofxGui.h"

class ofApp : public ofxUnitTestsApp{
    void parameterChanged(ofAbstractParameter & parameter){
        param.getFirstParent();
    }
    
    ofxPanel gui;
    ofParameterGroup group;
    ofParameter<int> param;
    
    void run(){
        group.setName("myGroup");
        group.add(param.set("myParam", 1));
        gui.setup(group);
        ofAddListener(group.parameterChangedE(),this,&ofApp::parameterChanged);
        param.set(2); // if this don't crash, test succeeds
        test_eq(param.get(), 2, "callback listener corrupted param.obj->parents");
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
