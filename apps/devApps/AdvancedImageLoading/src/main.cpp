#include "ofApp.h"
#include "ofMain.h"

int main() {
	ofGLWindowSettings settings;
	settings.setGLVersion(4, 1);
	settings.setSize(1280, 720);
	settings.windowMode = OF_WINDOW;
	auto window = ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
