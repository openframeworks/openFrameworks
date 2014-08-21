#include "ofMain.h"
#include "ofApp.h"

int main(){
	//ofSetupOpenGL(1024,768, OF_FULLSCREEN);			// <-------- setup the GL context

	//ofRunApp(new ofApp);
    
    ofAppiOSWindow * window = new ofAppiOSWindow();
    window->enableHardwareOrientation();
    window->enableOrientationAnimation();
    
    ofSetupOpenGL(window, 1024,768, OF_FULLSCREEN);
    ofRunApp(new ofApp);
}
