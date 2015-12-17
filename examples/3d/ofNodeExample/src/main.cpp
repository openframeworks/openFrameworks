#include "ofAppRunner.h"
#include "ofApp.h"

int main(){
	ofGLWindowSettings settings;
	settings.setGLVersion(3,2);
	settings.width = 1280;
	settings.height = 720;

	auto window = ofCreateWindow(settings);
	auto app = std::make_shared<ofApp>();

	ofRunApp(window, app);

	return ofRunMainLoop();
}
