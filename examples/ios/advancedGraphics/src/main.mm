#include "ofMain.h"
#include "ofApp.h"
#include "ofxiOSExtras.h"

int main(){
	//ofSetupOpenGL(1024,768, OF_FULLSCREEN);			// <-------- setup the GL context

	ofAppiOSWindow * iOSWindow = new ofAppiOSWindow();
	
	iOSWindow->enableDepthBuffer();
	//iOSWindow->enableAntiAliasing(4);
	
	iOSWindow->enableRetina();
	
	ofSetupOpenGL(iOSWindow, 480, 320, OF_FULLSCREEN);
	ofRunApp(new ofApp);
}
