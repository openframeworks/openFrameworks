#include "ofApp.h"

int main(){

	//Use ofGLFWWindowSettings for more options like OpenGL version and multi-monitor fullscreen
	ofWindowSettings settings;
	settings.setSize(640, 480);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();
}
