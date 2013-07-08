#include "ofMain.h"
#include "testApp.h"

int main(){
    ofAppiPhoneWindow * iOSWindow = new ofAppiPhoneWindow();
    iOSWindow->enableDepthBuffer();
    ofSetupOpenGL(iOSWindow, 1024, 768, OF_FULLSCREEN);
    
    ofRunApp(new testApp);
}
