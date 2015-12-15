#include "ofApp.h"

int main(){
	ofGLWindowSettings settings;
	settings.setGLVersion(3,3);
	ofCreateWindow(settings);
	ofRunApp(std::make_shared<ofApp>());
}
