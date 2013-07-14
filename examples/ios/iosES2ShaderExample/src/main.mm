#include "ofMain.h"
#include "testApp.h"
#include "ofGLProgrammableRenderer.h"

int main(){
	ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLProgrammableRenderer()));// switch to GLES2 renderer.

	ofSetupOpenGL(1024, 768, OF_FULLSCREEN);                                        // setup the GL context
	ofRunApp(new testApp());                                                    // run app.
}
