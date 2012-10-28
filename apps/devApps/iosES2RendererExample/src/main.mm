#include "ofMain.h"
#include "testApp.h"
#include "ofGLES2Renderer.h"

int main(){
    ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLES2Renderer()));
	ofSetupOpenGL(1024,768, OF_FULLSCREEN);			// <-------- setup the GL context
	ofRunApp(new testApp);
}
