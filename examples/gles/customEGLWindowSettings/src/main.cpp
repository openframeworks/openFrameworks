#include "ofMain.h"
#include "ofApp.h"
#include "ofAppEGLWindow.h"

//========================================================================
int main( ){

	ofAppEGLWindow::Settings settings;

	settings.eglWindowOpacity = 127;
        settings.frameBufferAttributes[EGL_DEPTH_SIZE]   = 0; // 0 bits for depth
        settings.frameBufferAttributes[EGL_STENCIL_SIZE] = 0; // 0 bits for stencil
	
	ofAppEGLWindow window;
        window.setup(settings);

	ofSetupOpenGL(&window, 1024,768, OF_FULLSCREEN);// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
