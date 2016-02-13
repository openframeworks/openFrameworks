#include "ofAppRunner.h"
#include "ofApp.h"

int main(){
	// ofSetupOpenGL sets up a default OpenGL window for ofApp. The initial
	// window width, height and fullscreen status (OF_WINDOW or OF_FULLSCREEN)
	// can be set here. Window shape and fullscreen status can changed elsewhere
	// with ofSetWindowShape(x, y) and ofSetFullscreen(fullscreen) respectively.
	ofSetupOpenGL(900, 600, OF_WINDOW);
	return ofRunApp(std::make_shared<ofApp>());
}
