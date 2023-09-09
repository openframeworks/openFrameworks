#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(4, 0);

	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
}
