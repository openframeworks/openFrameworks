#include "ofApp.h"

int main(){
	ofGLESWindowSettings settings;
	settings.setGLESVersion(2);
	ofCreateWindow(settings);
	ofRunApp(std::make_shared<ofApp>());
}
