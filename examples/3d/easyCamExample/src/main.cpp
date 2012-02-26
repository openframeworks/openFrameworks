#include "ofAppGlutWindow.h"
#include "testApp.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 640, 480, OF_WINDOW);
	ofRunApp(new testApp());
}
