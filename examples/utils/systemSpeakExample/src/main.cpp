#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 500, 500, OF_WINDOW);
	ofRunApp(new testApp());
}
