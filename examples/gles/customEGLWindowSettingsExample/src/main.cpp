#include "ofMain.h"
#include "ofApp.h"
#include "ofAppEGLWindow.h"

//========================================================================
int main( ){
	ofAppEGLWindow::Settings settings;

	settings.eglWindowOpacity = 127;
	settings.frameBufferAttributes[EGL_DEPTH_SIZE]   = 0; // 0 bits for depth
	settings.frameBufferAttributes[EGL_STENCIL_SIZE] = 0; // 0 bits for stencil

	settings.width = 1024;
	settings.height = 768;
	settings.windowMode = OF_FULLSCREEN;
	ofCreateWindow(settings);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
