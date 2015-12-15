#include "ofApp.h"

int main(){
	ofGLWindowSettings settings;
	settings.setGLVersion(4,0);
	ofCreateWindow(settings);
	ofRunApp(std::make_shared<ofApp>());
}
