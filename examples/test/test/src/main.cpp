#include "ofApp.h"
#include "GLFW/glfw3.h"

//--------------------------------------------------------------
int main(){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

	cout << "GLFW test" << endl;
	cout << GLFW_VERSION_MAJOR << endl;
	cout << GLFW_VERSION_MINOR << endl;
	cout << GLFW_VERSION_REVISION << endl;

}
