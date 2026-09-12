#include "ofMain.h"
#include "ofApp.h"
#if defined(TARGET_GLFW_WINDOW)
#include "ofAppGLFWWindow.h"
#endif

int main() {
	ofGLWindowSettings settings;
	settings.setSize(320, 240);
	settings.windowMode = OF_WINDOW;

	auto window = ofCreateWindow(settings);
#if defined(TARGET_GLFW_WINDOW)
	auto glfw = dynamic_cast<ofAppGLFWWindow *>(window.get());
	if (!glfw || !glfw->getGLFWWindow()) {
		ofLogWarning("glRenderSmokeTest") << "no GL window (headless / GLFW init failed) — skip";
		return 0;
	}
#endif

	ofRunApp(window, std::make_shared<ofApp>());
	return ofRunMainLoop();
}
