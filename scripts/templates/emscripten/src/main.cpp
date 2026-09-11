#include "ofMain.h"
#include "ofApp.h"

#include <cstdio>
#include <emscripten/version.h>

//========================================================================
int main( ){

#if defined(__EMSCRIPTEN_MAJOR__)
	printf("emscripten:v%d.%d.%d\n", __EMSCRIPTEN_MAJOR__, __EMSCRIPTEN_MINOR__, __EMSCRIPTEN_TINY__);
#elif defined(__EMSCRIPTEN_major__)
	printf("emscripten:v%d.%d.%d\n", __EMSCRIPTEN_major__, __EMSCRIPTEN_minor__, __EMSCRIPTEN_tiny__);
#else
	printf("emscripten:unknown version\n");
#endif

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();

}
