#include "ofMain.h"
#include "testApp.h"
#include "ofAppiPhoneWindow.h"

int main(){
	ofAppiPhoneWindow * window = new ofAppiPhoneWindow();
    window->enableRendererES2();

	ofSetupOpenGL(window, 1024, 768, OF_FULLSCREEN);    // setup the GL context
	ofRunApp(new testApp());                            // run app.
}
