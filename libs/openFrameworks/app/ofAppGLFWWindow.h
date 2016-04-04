#pragma once

#include "ofConstants.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "ofAppBaseWindow.h"
#include "ofEvents.h"
#include "ofPixels.h"
#include "ofRectangle.h"

class ofBaseApp;

#ifdef TARGET_OPENGLES
class ofGLFWWindowSettings: public ofGLESWindowSettings{
#else
class ofGLFWWindowSettings: public ofGLWindowSettings{
#endif
public:
	ofGLFWWindowSettings(){}

#ifdef TARGET_OPENGLES
	ofGLFWWindowSettings(const ofGLESWindowSettings & settings)
	:ofGLESWindowSettings(settings){}
#else
	ofGLFWWindowSettings(const ofGLWindowSettings & settings)
	:ofGLWindowSettings(settings){}
#endif

	int numSamples = 4;
	bool doubleBuffering = true;
	int redBits = 8;
	int greenBits = 8;
	int blueBits = 8;
	int alphaBits = 8;
	int depthBits = 24;
	int stencilBits = 0;
	bool stereo = false;
	bool visible = true;
	bool iconified = false;
	bool decorated = true;
	bool resizable = true;
	int monitor = 0;
	bool multiMonitorFullScreen = false;
	shared_ptr<ofAppBaseWindow> shareContextWith;
};

#ifdef TARGET_OPENGLES
class ofAppGLFWWindow : public ofAppBaseGLESWindow{
#else
class ofAppGLFWWindow : public ofAppBaseGLWindow {
#endif

public:

	ofAppGLFWWindow();
	~ofAppGLFWWindow();

	// Can't be copied, use shared_ptr
	ofAppGLFWWindow(ofAppGLFWWindow & w) = delete;
	ofAppGLFWWindow & operator=(ofAppGLFWWindow & w) = delete;

	static void loop(){};
	static bool doesLoop(){ return false; }
	static bool allowsMultiWindow(){ return true; }
	static bool needsPolling(){ return true; }
	static void pollEvents(){ glfwPollEvents(); }


    // this functions are only meant to be called from inside OF don't call them from your code
    using ofAppBaseWindow::setup;
#ifdef TARGET_OPENGLES
	void setup(const ofGLESWindowSettings & settings);
#else
	void setup(const ofGLWindowSettings & settings);
#endif
	void setup(const ofGLFWWindowSettings & settings);
	void update();
	void draw();
	bool getWindowShouldClose();
	void setWindowShouldClose();

	void close();

	void hideCursor();
	void showCursor();

	int getHeight();
	int getWidth();

	ofCoreEvents & events();
	shared_ptr<ofBaseRenderer> & renderer();
    
    GLFWwindow* getGLFWWindow();
    void * getWindowContext(){return getGLFWWindow();}
	ofGLFWWindowSettings getSettings(){ return settings; }

	ofVec3f		getWindowSize();
	ofVec3f		getScreenSize();
	ofVec3f 	getWindowPosition();

	void setWindowTitle(string title);
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);

	void			setOrientation(ofOrientation orientation);
	ofOrientation	getOrientation();

	ofWindowMode	getWindowMode();

	void		setFullscreen(bool fullscreen);
	void		toggleFullscreen();

	void		enableSetupScreen();
	void		disableSetupScreen();

	void		setVerticalSync(bool bSync);

    void        setClipboardString(const string& text);
    string      getClipboardString();

    int         getPixelScreenCoordScale();

    void 		makeCurrent();
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
	OF_DEPRECATED_MSG("use ofGLFWWindowSettings to create the window instead", void setNumSamples(int samples));
	OF_DEPRECATED_MSG("use ofGLFWWindowSettings to create the window instead", void setDoubleBuffering(bool doubleBuff));
	OF_DEPRECATED_MSG("use ofGLFWWindowSettings to create the window instead", void setColorBits(int r, int g, int b));
	OF_DEPRECATED_MSG("use ofGLFWWindowSettings to create the window instead", void setAlphaBits(int a));
	OF_DEPRECATED_MSG("use ofGLFWWindowSettings to create the window instead", void setDepthBits(int depth));
	OF_DEPRECATED_MSG("use ofGLFWWindowSettings to create the window instead", void setStencilBits(int stencil));
	OF_DEPRECATED_MSG("use ofGLFWWindowSettings to create the window instead", void setMultiDisplayFullscreen(bool bMultiFullscreen)); //note this just enables the mode, you have to toggle fullscreen to activate it.

#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI)
	Display* 	getX11Display();
	Window  	getX11Window();
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
	GLXContext 	getGLXContext();
#endif

#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
	EGLDisplay 	getEGLDisplay();
	EGLContext 	getEGLContext();
	EGLSurface 	getEGLSurface();
#endif

#if defined(TARGET_OSX)
	void *		getNSGLContext();
	void *		getCocoaWindow();
#endif

#if defined(TARGET_WIN32)
	HGLRC 		getWGLContext();
	HWND 		getWin32Window();
#endif

private:
	static ofAppGLFWWindow * setCurrent(GLFWwindow* windowP);
	static void 	mouse_cb(GLFWwindow* windowP_, int button, int state, int mods);
	static void 	motion_cb(GLFWwindow* windowP_, double x, double y);
	static void 	entry_cb(GLFWwindow* windowP_, int entered);
	static void 	keyboard_cb(GLFWwindow* windowP_, int key, int scancode, unsigned int codepoint, int action, int mods);
	static void 	resize_cb(GLFWwindow* windowP_, int w, int h);
	static void 	exit_cb(GLFWwindow* windowP_);
	static void		scroll_cb(GLFWwindow* windowP_, double x, double y);
	static void 	drop_cb(GLFWwindow* windowP_, int numFiles, const char** dropString);
	static void		error_cb(int errorCode, const char* errorDescription);

#ifdef TARGET_LINUX
	void setWindowIcon(const string & path);
	void setWindowIcon(const ofPixels & iconPixels);
#endif

	ofCoreEvents coreEvents;
	shared_ptr<ofBaseRenderer> currentRenderer;
	ofGLFWWindowSettings settings;

	ofWindowMode	windowMode;

	bool			bEnableSetupScreen;
	int				windowW, windowH;
	int				currentW, currentH;

	ofRectangle windowRect;

	int				buttonInUse;
	bool			buttonPressed;

	int 			nFramesSinceWindowResized;
	bool			bWindowNeedsShowing;

	GLFWwindow* 	windowP;

	int				getCurrentMonitor();

	ofBaseApp *	ofAppPtr;

    int pixelScreenCoordScale; 

	ofOrientation orientation;

	bool iconSet;

    #ifdef TARGET_WIN32
    LONG lExStyle, lStyle;
    #endif // TARGET_WIN32
};


//#endif
