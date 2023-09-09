#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main() {

	ofGLESWindowSettings settings;
	settings.setGLESVersion(2);

	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
}
