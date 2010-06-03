#include "ofMain.h"
#include "testApp.h"
#include <jni.h>
#include "ofAppAndroidWindow.h"

//========================================================================
extern "C"{
void Java_cc_openframeworks_OFAndroidWindow_init( JNIEnv*  env, jobject  thiz ){

    ofAppAndroidWindow *window = new ofAppAndroidWindow;
	ofSetupOpenGL(window, 1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp() );
}
}

int main(){
	return 0;
}
