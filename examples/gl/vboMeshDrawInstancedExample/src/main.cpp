#include "ofAppRunner.h"
#include "ofApp.h"


// By default this example will run in OpenGL 2.0, see ofApp.h for running it
// in OpenGL 3.2.

#ifdef USE_PROGRAMMABLE_GL
// Using the programmable GL renderer is more fun, since we can use GLSL 150
// =) define USE_PROGRAMMABLE_GL in ofApp.h to run this example in OpenGL 3.2
// if your system provides it ...
#include "ofGLProgrammableRenderer.h"
#endif

int main(){
	ofGLWindowSettings glWindowSettings;

#ifdef USE_PROGRAMMABLE_GL
	// We are using the programmable gl renderer.
	glWindowSettings.setGLVersion(3, 2);
	auto window = ofCreateWindow(glWindowSettings);
#else
	// We are not using the progammable gl renderer.

	// When we are not using the programmable gl renderer it is not safe to
	// assume our current PpenGl context provides us with the necessary openGL
	// extensions to run this example.

	// Let's check if the current openGL context provides us with
	// glDrawElementsInstanced we do this after we have initialised our OpenGL
	// context.

	glWindowSettings.setGLVersion(2, 1);
	auto window = ofCreateWindow(glWindowSettings);

	std::ostringstream extStr;
	extStr << (char*)glGetString(GL_EXTENSIONS); // First get all available extensions.
	std::string extensionsAvailable = extStr.str();

	if (ofStringTimesInString(extensionsAvailable, "GL_ARB_draw_instanced") == 0)
	{
		ofLogFatalError("App") << " GL_ARB_draw_instanced is needed for this example but it is not supported by your graphics card. Exiting App.";
		return -1;
	} else {
		// GL_ARB_draw_instanced is available... so far so good.

		// Whether one of these is needed, too:
		if (ofStringTimesInString(extensionsAvailable, "GL_EXT_gpu_shader4") == 0 &&
			ofStringTimesInString(extensionsAvailable, "NV_vertex_program4") == 0 )
		{
			ofLogFatalError("App") << " GL_EXT_gpu_shader4 or NV_vertex_program4 is needed for this example but it is not supported by your graphics card. Exiting App.";
			return -1;
		}
	}
#endif

	auto app = std::make_shared<ofApp>();

	ofRunApp(window, app);

	return ofRunMainLoop();
}
