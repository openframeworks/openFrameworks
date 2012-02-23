#include "ofAppGlutWindow.h"
#include "testApp.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 800, 800, OF_WINDOW);
	ofRunApp(new testApp());
}
