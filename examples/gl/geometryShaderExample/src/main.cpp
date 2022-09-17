#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN
	settings.setGLVersion(3, 2);
	auto window = ofCreateWindow(settings);
	
	if(!ofGLCheckExtension("GL_ARB_geometry_shader4") && !ofGLCheckExtension("GL_EXT_geometry_shader4") && !ofIsGLProgrammableRenderer()){
		ofLogFatalError() << "geometry shaders not supported on this graphics card";
		return 1;
	}

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}
