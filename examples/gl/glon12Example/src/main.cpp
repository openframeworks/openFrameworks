#include "ofMain.h"
#include "ofApp.h"

//========================================================================
// GLon12: OpenGL on Direct3D 12 (Windows). Generate with the projectGenerator renderer "GLon12".
// --warp (or settings.glon12Warp) runs on WARP, software D3D12; w restarts in the other mode.
int main(int argc, char * argv[]) {
	bool warp = false;
	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]) == "--warp") warp = true;
	}

#ifdef TARGET_GLFW_WINDOW
	ofGLFWWindowSettings settings;
	settings.glon12Warp = warp; // must be set on the first window
#else
	ofGLWindowSettings settings;
#endif
	settings.setGLVersion(3, 3);
	settings.setSize(1024, 768);
	settings.windowMode = OF_WINDOW;

	auto window = ofCreateWindow(settings);

	auto app = std::make_shared<ofApp>();
	app->warpRequested = warp;
	ofRunApp(window, app);
	ofRunMainLoop();
}
