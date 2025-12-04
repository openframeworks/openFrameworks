#include "ofMain.h"
#include "ofApp.h"

#ifdef TARGET_ANDROID

#include "ofWindowSettings.h"
#include "ofGLProgrammableRenderer.h"

shared_ptr<ofApp> *ofapp;
std::shared_ptr<ofAppBaseWindow> baseWindow;

//--------------------------------------------------------------
int main(int argc, char **argv) {
    baseWindow = std::make_shared<ofAppAndroidWindow>();
    ofxAndroidWindowSettings settings;
    settings.glesVersion = 2;
    settings.setSize(1920, 1080);
    settings.windowMode = OF_WINDOW;
    settings.preserveContextOnPause = true;
    baseWindow = ofCreateWindow(settings);
    ofapp = new shared_ptr<ofApp>(new ofApp());
    ofRunApp(baseWindow, *ofapp);
    return 0;
}

void ofAndroidApplicationInit()
{
    //application scope init
}
void ofAndroidActivityInit()
{
    //activity scope init - call main
    main(0, nullptr);
}

// Callbacks from Android Layer
extern "C" JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jclass  clazz)
{
    ofAndroidApplicationInit();
}

extern "C" JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onCreate( JNIEnv*  env, jclass  clazz)
{
    ofAndroidActivityInit();
}

#endif
