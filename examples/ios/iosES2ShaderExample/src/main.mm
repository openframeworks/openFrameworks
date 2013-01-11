#include "ofMain.h"
#include "testApp.h"
#include "ofGLES2Renderer.h"

int main(){
    string vertShader = "shaders/noise.vert";
    string fragShader = "shaders/noise.frag";
    ofGLES2Renderer * renderer = new ofGLES2Renderer(vertShader, fragShader);
	ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(renderer));                      // switch to GLES2 renderer.

	ofSetupOpenGL(1024, 768, OF_WINDOW);                                        // setup the GL context
	ofRunApp(new testApp());                                                    // run app.
}
