#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"

int main() {
	// if you want to see the window	
	// comment these two lines 
	ofAppNoWindow window;
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);

	// and uncomment this line
	// ofSetupOpenGL(300, 300, OF_WINDOW);
	ofRunApp(new ofApp());
}
