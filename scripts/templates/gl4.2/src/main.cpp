#include "ofApp.h"

int main(){
	ofGLWindowSettings settings;
	settings.setGLVersion(4,2);
	ofCreateWindow(settings);
	ofRunApp(std::make_shared<ofApp>());
}
