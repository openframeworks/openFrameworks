#include "ofMain.h"
#include "ofApp.h"

int main(int argc, char ** argv) {
	aquariumConfigure(argc, argv);

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(1024, 1024);
	settings.windowMode = OF_WINDOW;
	ofCreateWindow(settings);
	ofRunApp(std::make_shared<ofApp>());
	ofRunMainLoop();
}
