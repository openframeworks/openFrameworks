#include "ofMain.h"
#include "testApp.h"
#include "ofGLES2Renderer.h"

int main(){
    ofAppiPhoneWindow * window;
    window = new ofAppiPhoneWindow();
    window->enableES2Renderer();
    
	ofSetupOpenGL(window, 1024,768, OF_FULLSCREEN);			// <-------- setup the GL context
	ofRunApp(new testApp);
}
