#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	ofGLWindowSettings settings;
	settings.setSize(720, 240);
	settings.windowMode = OF_WINDOW;

	auto window = ofCreateWindow(settings);
	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
}
