#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

class ofApp: public ofxUnitTestsApp{
	void run(){

	}
};

int main(){
	// To create an application with no Window, we pass pointers to our
	// ofAppNoWindow class and our ofApp to ofRunApp. Then we call the
	// ofRunMainLoop() function to start the program.
	ofRunApp(std::make_shared<ofAppNoWindow>(), std::make_shared<ofApp>());
	ofRunMainLoop();
}
