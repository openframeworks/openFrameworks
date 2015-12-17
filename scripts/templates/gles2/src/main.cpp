#include "ofAppRunner.h"
#include "ofApp.h"

int main(){
	ofGLESWindowSettings settings;
	settings.setGLESVersion(2);

	auto window = ofCreateWindow(settings);
	auto app = std::make_shared<ofApp>();

	ofRunApp(window, app);

	return ofRunMainLoop();
}
