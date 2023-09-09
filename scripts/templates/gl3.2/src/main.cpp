#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);

	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
}
