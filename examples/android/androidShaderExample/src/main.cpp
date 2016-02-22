#include "ofMain.h"
#include "ofApp.h"
#include "ofGLProgrammableRenderer.h"

int main(){
	ofGLESWindowSettings settings;
	settings.setGLESVersion(2);
	ofCreateWindow(settings);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	return ofRunApp( new ofApp() );
}


#ifdef TARGET_ANDROID
void ofAndroidApplicationInit()
{
    //application scope init
}

void ofAndroidActivityInit()
{
	//activity scope init
	main();
}
#endif
