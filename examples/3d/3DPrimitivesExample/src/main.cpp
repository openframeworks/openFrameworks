#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new testApp()); // start the app
}
