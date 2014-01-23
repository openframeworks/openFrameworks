#include "ofMain.h"
#include "ofApp.h"

int main(){
    ofAppiOSWindow * window = new ofAppiOSWindow();
	ofSetupOpenGL(window, 1024, 768, OF_FULLSCREEN);
	ofRunApp(new ofApp);
}
