#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 3);

	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
}
