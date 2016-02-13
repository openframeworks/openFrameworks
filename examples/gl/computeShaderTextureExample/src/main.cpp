#include "ofAppRunner.h"
#include "ofApp.h"

int main(){
	// This example uses compute shaders which are only supported since
	// OpenGL 4.3.
	ofGLWindowSettings settings;
	settings.setGLVersion(4,3);

	auto window = ofCreateWindow(settings);
	auto app = std::make_shared<ofApp>();

	ofRunApp(window, app);

	return ofRunMainLoop();
}
