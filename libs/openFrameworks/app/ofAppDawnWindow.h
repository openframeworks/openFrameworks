#pragma once

#include "ofConstants.h"
#if OF_USE_DAWN && !defined(TARGET_OF_IOS)

#include "ofAppBaseWindow.h"
#include "ofWindowSettings.h"
#include "ofEvents.h"
#include <memory>

struct GLFWwindow;
class ofWebGPURenderer;

class ofAppDawnWindow : public ofAppBaseWindow {
public:
	ofAppDawnWindow();
	~ofAppDawnWindow() override;

	ofAppDawnWindow(const ofAppDawnWindow &) = delete;
	ofAppDawnWindow & operator=(const ofAppDawnWindow &) = delete;

	static void loop() {}
	static bool doesLoop() { return false; }
	static bool allowsMultiWindow() { return true; }
	static bool needsPolling() { return true; }
	static void pollEvents();

	using ofAppBaseWindow::setup;
	void setup(const ofWindowSettings & settings) override;
	void setup(const ofDawnWindowSettings & settings);
	void update() override;
	void draw() override;
	void close() override;

	bool getWindowShouldClose() override;
	void setWindowShouldClose() override;

	ofCoreEvents & events() override;
	std::shared_ptr<ofBaseRenderer> & renderer() override;

	glm::vec2 getWindowSize() override;
	glm::vec2 getScreenSize() override;
	glm::vec2 getWindowPosition() override;
	int getWidth() override;
	int getHeight() override;

	void setWindowTitle(std::string title) override;
	void setWindowPosition(int x, int y) override;
	void setWindowShape(int w, int h) override;
	void setVerticalSync(bool enabled) override;

	void * getWindowContext() override;
#if defined(TARGET_OSX)
	void * getCocoaWindow() override;
#endif
	void makeCurrent() override;

	GLFWwindow * getGLFWWindow() { return windowP; }
	ofDawnWindowSettings getSettings() const { return settings; }

private:
	void attachMetalLayer();
	void resizeSurface(int w, int h);

	static ofAppDawnWindow * setCurrent(GLFWwindow * windowP);
	static void mouse_cb(GLFWwindow * windowP_, int button, int state, int mods);
	static void motion_cb(GLFWwindow * windowP_, double x, double y);
	static void entry_cb(GLFWwindow * windowP_, int entered);
	static void keyboard_cb(GLFWwindow * windowP_, int key, int scancode, int action, int mods);
	static void resize_cb(GLFWwindow * windowP_, int w, int h);
	static void framebuffer_cb(GLFWwindow * windowP_, int w, int h);
	static void exit_cb(GLFWwindow * windowP_);
	static void scroll_cb(GLFWwindow * windowP_, double x, double y);
	static void drop_cb(GLFWwindow * windowP_, int numFiles, const char ** dropString);
	static void error_cb(int errorCode, const char * errorDescription);

	ofDawnWindowSettings settings;
	std::unique_ptr<ofCoreEvents> coreEvents;
	std::shared_ptr<ofBaseRenderer> currentRenderer;
	GLFWwindow * windowP = nullptr;
	void * metalLayer = nullptr;
	int windowW = 0;
	int windowH = 0;
	float pixelScale = 1.f;
	bool bWindowNeedsShowing = true;
};

#endif
