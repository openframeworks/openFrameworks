#include "ofMain.h"
#include "ofApp.h"

#ifdef TARGET_ANDROID

std::shared_ptr<ofApp> * ofapp;
std::shared_ptr<ofAppBaseWindow> baseWindow;

int main(int argc, char ** argv) {
	baseWindow = std::make_shared<ofAppAndroidWindow>();
	ofxAndroidWindowSettings settings;
	settings.glesVersion = 3;
	settings.glesVersionMinor = 0;
	settings.setSize(1280, 720);
	settings.windowMode = OF_WINDOW;
	// Deliberately allow context loss so pause/resume exercises resource reloads.
	settings.preserveContextOnPause = false;
	baseWindow = ofCreateWindow(settings);
	ofapp = new std::shared_ptr<ofApp>(new ofApp());
	ofRunApp(baseWindow, *ofapp);
	return 0;
}

void ofAndroidApplicationInit() {}

void ofAndroidActivityInit() {
	main(0, nullptr);
}

extern "C" JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_init(JNIEnv *, jclass) {
	ofAndroidApplicationInit();
}

extern "C" JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onCreate(JNIEnv *, jclass) {
	ofAndroidActivityInit();
}

#endif
