#include "ofMain.h"
#include "testApp.h"
#include "ofAppiPhoneWindow.h"

int main() {
    ofAppiPhoneWindow * window = new ofAppiPhoneWindow();
    window->enableHardwareOrientation();

	ofSetupOpenGL(window, 1024,768, OF_FULLSCREEN);
	ofRunApp(new testApp);
}
