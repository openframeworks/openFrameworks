#include "ofMain.h"
#include "ofxUnitTests.h"
#include "ofAppNoWindow.h"


class ofApp: public ofxUnitTestsApp{
public:
    void run(){
        ofLogNotice() << "testing #3160";
        ofxTest(true,"compiled a test with space in its path");
        ofLogNotice() << "end testing #3160";
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

