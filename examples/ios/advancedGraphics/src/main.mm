#include "ofMain.h"
#include "testApp.h"
#include "ofxiPhoneExtras.h"

int main(){
	//ofSetupOpenGL(1024,768, OF_FULLSCREEN);			// <-------- setup the GL context

	ofAppiPhoneWindow * iOSWindow = new ofAppiPhoneWindow();
	
	iOSWindow->enableDepthBuffer();
	//iOSWindow->enableAntiAliasing(4);
	
	iOSWindow->enableRetinaSupport();
	
	ofSetupOpenGL(iOSWindow, 480, 320, OF_FULLSCREEN);
	ofRunApp(new testApp);
}
