#include "ofMain.h"
#include "testApp.h"
#ifdef TARGET_ANDROID
	#include "ofAppAndroidWindow.h"
#else
	#include "ofAppGlutWindow.h"
#endif



int main(){

#ifdef TARGET_ANDROID
	ofAppAndroidWindow *window = new ofAppAndroidWindow;
#else
	ofAppGlutWindow *window = new ofAppGlutWindow;
#endif
	ofSetupOpenGL(window, 1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp() );
	return 0;
}


#ifdef TARGET_ANDROID
#include <jni.h>

//========================================================================
extern "C"{
	void Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jobject  thiz ){
		main();
	}
}
#endif
