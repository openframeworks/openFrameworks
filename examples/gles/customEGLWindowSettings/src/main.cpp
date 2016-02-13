#include "ofAppRunner.h"
#include "ofAppEGLWindow.h"
#include "ofApp.h"

int main(){
	ofAppEGLWindow::Settings settings;

	settings.eglWindowOpacity = 127;
	settings.frameBufferAttributes[EGL_DEPTH_SIZE]   = 0; // 0 bits for depth
	settings.frameBufferAttributes[EGL_STENCIL_SIZE] = 0; // 0 bits for stencil

	ofAppEGLWindow window;
	window.setup(settings);

	ofSetupOpenGL(&window, 1024,768, OF_FULLSCREEN);
	return ofRunApp(std::make_shared<ofApp>());
}
