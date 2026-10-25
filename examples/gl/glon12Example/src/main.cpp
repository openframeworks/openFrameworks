#include "ofMain.h"
#include "ofApp.h"

//========================================================================
// GLon12 runs OpenGL on Direct3D 12 through Mesa (Windows). Generate this example with
// the projectGenerator renderer set to "GLon12" (or -DOF_USE_GLON12=1): the build then
// copies opengl32.dll, libgallium_wgl.dll and dxil.dll from libs/glon12 next to the exe.
//
// GLon12 picks the D3D12 adapter when the first GL context is created:
//   - the GPU, by default
//   - WARP, Windows' software D3D12 renderer, with settings.glon12Warp = true
//     (or the projectGenerator renderer "GLon12 - WARP", -DOF_USE_GLON12_WARP=1).
//     Slow, but it works without a working GPU driver: VMs, CI, remote desktops.
//
// Run with --warp to start on WARP. Press w in the app to restart in the other mode.
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
