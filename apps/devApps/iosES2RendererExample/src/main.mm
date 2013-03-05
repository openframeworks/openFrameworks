#include "ofMain.h"
#include "testApp.h"
#include "ofProgrammableGLRenderer.h"

int main(){
    ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofProgrammableGLRenderer()));
	ofSetupOpenGL(1024,768, OF_FULLSCREEN);			// <-------- setup the GL context
	ofRunApp(new testApp);
}
