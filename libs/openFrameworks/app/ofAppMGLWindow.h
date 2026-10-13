#pragma once

#include "ofConstants.h"
#if OF_USE_ANGLE && defined(TARGET_OSX) && !defined(TARGET_OF_IOS)

#include "ofAppBaseWindow.h"
#include "ofWindowSettings.h"
#include "ofEvents.h"
#include <memory>

class ofBaseRenderer;

/// Desktop window that presents OpenGL ES through MetalANGLE + MGLKit.
class ofAppMGLWindow : public ofAppBaseWindow {
public:
	ofAppMGLWindow();
	~ofAppMGLWindow() override;

	ofAppMGLWindow(const ofAppMGLWindow &) = delete;
	ofAppMGLWindow & operator=(const ofAppMGLWindow &) = delete;

	static void loop() {}
	static bool doesLoop() { return false; }
	static bool allowsMultiWindow() { return true; }
	static bool needsPolling() { return true; }
	static void pollEvents();

	void setup(const ofWindowSettings & settings) override;
	void update() override;
	void draw() override;
	void close() override;

	/// Called by the MGLKView delegate. The OF loop calls draw(), which presents the view.
	void renderFrame();

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
	void setFullscreen(bool fullscreen) override;
	void toggleFullscreen() override;
	ofWindowMode getWindowMode() override;

	void enableSetupScreen() override;
	void disableSetupScreen() override;
	void setVerticalSync(bool enabled) override;

	void * getWindowContext() override;
	void * getCocoaWindow() override;
	void makeCurrent() override;

	void updateDrawableSize();

private:
	void showWindow();

	ofWindowSettings settings;
	std::unique_ptr<ofCoreEvents> coreEvents;
	std::shared_ptr<ofBaseRenderer> currentRenderer;
	void * owner = nullptr;
	int windowW = 0;
	int windowH = 0;
	int glesMajor = 2;
	int glesMinor = 0;
	float pixelScale = 1.f;
	bool bEnableSetupScreen = true;
	bool bWindowNeedsShowing = true;
	bool bShouldClose = false;
	bool bSetup = false;
};

#endif
