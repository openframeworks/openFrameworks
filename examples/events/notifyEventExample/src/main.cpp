#include "ofApp.h"
#include "ofMain.h"

int main() {

	ofGLWindowSettings settings;
	auto window = ofCreateWindow(settings);
	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
}
