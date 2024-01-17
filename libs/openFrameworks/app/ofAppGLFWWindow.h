#pragma once

#include "ofAppBaseWindow.h"
// MARK: Optimize to Pointer
#include "ofRectangle.h"
// MARK: Target
#include "ofConstants.h"

#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY)
typedef struct _XIM * XIM;
typedef struct _XIC * XIC;
#endif

class ofBaseApp;
struct GLFWwindow;
struct GLFWmonitor;
class ofCoreEvents;

template <typename T>
class ofPixels_;
typedef ofPixels_<unsigned char> ofPixels;


class ofAppGLFWWindow : public ofAppBaseWindow {
public:
	ofAppGLFWWindow();
	~ofAppGLFWWindow();
	
//	ofWindowMode getWindowMode() { return settings.windowMode; }
	
	// Can't be copied, use shared_ptr
	ofAppGLFWWindow(ofAppGLFWWindow & w) = delete;
	ofAppGLFWWindow & operator=(ofAppGLFWWindow & w) = delete;

	static void loop() {};
	static bool doesLoop() { return false; }
	static bool allowsMultiWindow() { return true; }
	static bool needsPolling() { return true; }
	static void pollEvents();

	// this functions are only meant to be called from inside OF don't call them from your code
//	using ofAppBaseWindow::setup;

	void setup(const ofWindowSettings & settings);
	void update();
	void draw();
	bool getWindowShouldClose();
	void setWindowShouldClose();

	void hideCursor();
	void showCursor();

	int getHeight();
	int getWidth();

	ofCoreEvents & events();
	std::shared_ptr<ofBaseRenderer> & renderer();

	GLFWwindow * getGLFWWindow();
	void * getWindowContext() { return getGLFWWindow(); }
	ofWindowSettings getSettings() { return settings; }

	glm::ivec2 getScreenSize();
	glm::ivec2 getWindowSize();
	glm::ivec2 getWindowPosition();
	ofRectangle getWindowRect();

	void setWindowTitle(const std::string & title);

	void setWindowRect(const ofRectangle & rect);
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);

	void setFullscreen(bool fullscreen);
	void toggleFullscreen();

	void enableSetupScreen();
	void disableSetupScreen();

	void setVerticalSync(bool bSync);

	void setClipboardString(const std::string & text);
	std::string getClipboardString();

	int getPixelScreenCoordScale();

	void makeCurrent();
	void swapBuffers();
	void startRender();
	void finishRender();

	static void listVideoModes();
	static void listMonitors();
	bool isWindowIconified();
	bool isWindowActive();
	bool isWindowResizeable();
	void iconify(bool bIconify);

	// window settings, this functions can only be called from main before calling ofSetupOpenGL
	// TODO: remove specialized version of ofSetupOpenGL when these go away
	[[deprecated("use ofGLFWWindowSettings to create the window")]]
	void setNumSamples(int samples);
	[[deprecated("use ofGLFWWindowSettings to create the window")]]
	void setDoubleBuffering(bool doubleBuff);
	[[deprecated("use ofGLFWWindowSettings to create the window")]]
	void setColorBits(int r, int g, int b);
	[[deprecated("use ofGLFWWindowSettings to create the window")]]
	void setAlphaBits(int a);
	[[deprecated("use ofGLFWWindowSettings to create the window")]]
	void setDepthBits(int depth);
	[[deprecated("use ofGLFWWindowSettings to create the window")]]
	void setStencilBits(int stencil);
	[[deprecated("use ofGLFWWindowSettings to create the window")]]
	void setMultiDisplayFullscreen(bool bMultiFullscreen); //note this just enables the mode, you have to toggle fullscreen to activate it.

#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY)
	Display * getX11Display();
	Window getX11Window();
	XIC getX11XIC();

	void setWindowIcon(const of::filesystem::path & path);
	void setWindowIcon(const ofPixels & iconPixels);
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
	GLXContext getGLXContext();
#endif

#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
	EGLDisplay getEGLDisplay();
	EGLContext getEGLContext();
	EGLSurface getEGLSurface();
#endif

#if defined(TARGET_OSX)
	void * getNSGLContext();
	void * getCocoaWindow();
#endif

#if defined(TARGET_WIN32)
	HGLRC getWGLContext();
	HWND getWin32Window();
#endif

private:
	static ofAppGLFWWindow * setCurrent(GLFWwindow* windowP);
	static void mouse_cb(GLFWwindow* windowP_, int button, int state, int mods);
	static void motion_cb(GLFWwindow* windowP_, double x, double y);
	static void entry_cb(GLFWwindow* windowP_, int entered);
	static void keyboard_cb(GLFWwindow* windowP_, int key, int scancode, int action, int mods);
	static void char_cb(GLFWwindow* windowP_, uint32_t key);
	static void resize_cb(GLFWwindow* windowP_, int w, int h);
	static void position_cb(GLFWwindow* windowP_, int x, int y);
	static void framebuffer_size_cb(GLFWwindow* windowP_, int w, int h);
	static void exit_cb(GLFWwindow * windowP_);
	static void scroll_cb(GLFWwindow * windowP_, double x, double y);
	static void drop_cb(GLFWwindow* windowP_, int numFiles, const char** dropString);
	static void error_cb(int errorCode, const char * errorDescription);
	static void refresh_cb(GLFWwindow * windowP_);
	static void monitor_cb(GLFWmonitor * monitor, int event);
	
	void close();

#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY)
	XIM xim;
	XIC xic;
#endif

	std::unique_ptr<ofCoreEvents> coreEvents;
	std::shared_ptr<ofBaseRenderer> currentRenderer;

	ofWindowMode targetWindowMode;

	bool bEnableSetupScreen;
	// FIXME remove
	float pixelScreenCoordScale; /// Scale factor from virtual operating-system defined client area extents (as seen in currentW, currentH) to physical framebuffer pixel coordinates (as seen in windowW, windowH).

	ofRectangle windowRect { 20, 20, 1024, 768 };

	int buttonInUse;
	bool buttonPressed;

	int nFramesSinceWindowResized;
	bool bWindowNeedsShowing;

	GLFWwindow * windowP;
	ofBaseApp * ofAppPtr;

	bool iconSet;

#ifdef TARGET_WIN32
	LONG lExStyle, lStyle;
#endif // TARGET_WIN32
	
	void setFSTarget(ofWindowMode targetWindowMode);
};






using std::cout;
using std::endl;

// TEMPORARY
#include "GLFW/glfw3.h"


static struct ofMonitors {
public:
	ofMonitors() {}
	~ofMonitors() {}
	std::vector <ofRectangle> rects;
	ofRectangle allMonitorsRect { 0, 0, 0, 0 };
//	ofRectangle rectWindow;
//	bool changed = true;
	GLFWmonitor** monitors;

	ofRectangle getRectMonitorForScreenRect(const ofRectangle & rect) {
		for (unsigned int a=0; a<rects.size(); a++) {
			if (rects[a].inside(rect.getCenter())) {
				return rects[a];
				break;
			}
		}
		return { 0, 0, 0, 0 };
	}
	
	ofRectangle getRectForAllMonitors() {
//		cout << "getRectForAllMonitors " << allMonitorsRect << endl;
		return allMonitorsRect;
	}
	
	void update() {
		rects.clear();
		allMonitorsRect = { 0, 0, 0, 0 }; // reset ofRectangle;

		int numberOfMonitors;
		monitors = glfwGetMonitors(&numberOfMonitors);

		for (int i=0; i < numberOfMonitors; i++){
			glm::ivec2 pos;
			glfwGetMonitorPos(monitors[i], &pos.x, &pos.y);
			const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[i]);
			ofRectangle rect = ofRectangle( pos.x, pos.y, desktopMode->width, desktopMode->height );
			rects.emplace_back(rect);
			allMonitorsRect = allMonitorsRect.getUnion(rect);
		}
	}

} allMonitors;

// TODO: Remove.
//static bool updatePixelScreenCoordScale = true;
//#endif
