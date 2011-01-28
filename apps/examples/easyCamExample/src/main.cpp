#include "ofAppGlutWindow.h"
#include "testApp.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 200, 200, OF_WINDOW);
	ofRunApp(new testApp());
}
