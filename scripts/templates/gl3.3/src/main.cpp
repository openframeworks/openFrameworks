#include "ofAppRunner.h"
#include "ofApp.h"

int main(){
	ofGLWindowSettings settings;
	settings.setGLVersion(3,3);

	auto window = ofCreateWindow(settings);
	auto app = std::make_shared<ofApp>();

	ofRunApp(window, app);

	return ofRunMainLoop();
}
