#include "ofApp.h"

//--------------------------------------------------------------
int main(){
	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(830, 660);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN
	
	auto window = ofCreateWindow(settings);
	
	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
}
