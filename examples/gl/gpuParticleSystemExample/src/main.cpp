#include "ofAppRunner.h"
#include "ofApp.h"

int main(){
	ofGLWindowSettings settings;
	settings.setGLVersion(2, 1);  // Fixed pipeline.
//    settings.setGLVersion(3, 2);  // Programmable pipeline.
	settings.width = 1024;
	settings.height = 768;

	auto window = ofCreateWindow(settings);

	if(!ofGLCheckExtension("GL_ARB_geometry_shader4") &&
	   !ofGLCheckExtension("GL_EXT_geometry_shader4") &&
	   !ofIsGLProgrammableRenderer())
	{
		ofLogFatalError("App") << "Geometry shaders not supported on this graphics card.";
		return 1;
	}
	else
	{
		auto app = std::make_shared<ofApp>();
		ofRunApp(window, app);
		return ofRunMainLoop();
	}
}
