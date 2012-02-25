#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 250, 50, OF_WINDOW);
	ofRunApp(new testApp());
}
