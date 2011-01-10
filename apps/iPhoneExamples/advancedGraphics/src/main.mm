#include "ofMain.h"
#include "testApp.h"

int main(){
	//ofSetupOpenGL(1024,768, OF_FULLSCREEN);			// <-------- setup the GL context

	ofAppiPhoneWindow * iOSWindow = new ofAppiPhoneWindow();
	
	iOSWindow->depthEnabled			= true;
	iOSWindow->antiAliasingEnabled	= true;
	iOSWindow->useRetina			= true;
	
	ofSetupOpenGL(iOSWindow, 480, 320, OF_FULLSCREEN);
	ofRunApp(new testApp);
}
