#pragma once

#include "ofAppBaseWindow.h"
#include "ofRectangle.h"
#include "ofConstants.h"

#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY)
typedef struct _XIM * XIM;
typedef struct _XIC * XIC;
#endif

class ofBaseApp;
struct GLFWwindow;
class ofCoreEvents;
template<typename T>
class ofPixels_;
typedef ofPixels_<unsigned char> ofPixels;

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

#ifdef TARGET_RASPBERRY_PI
	int numSamples = 0;
#else
	int numSamples = 4;
#endif

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
	bool transparent = false;
	int monitor = 0;
	bool multiMonitorFullScreen = false;
	std::shared_ptr<ofAppBaseWindow> shareContextWith;
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
	static void pollEvents();


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

	void hideCursor();
	void showCursor();

	int getHeight();
	int getWidth();

	ofCoreEvents & events();
	std::shared_ptr<ofBaseRenderer> & renderer();
    
    GLFWwindow* getGLFWWindow();
    void * getWindowContext(){return getGLFWWindow();}
	ofGLFWWindowSettings getSettings(){ return settings; }

	glm::vec2	getWindowSize();
	glm::vec2	getScreenSize();
	glm::vec2 	getWindowPosition();

	void setWindowTitle(std::string title);
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

    void        setClipboardString(const std::string& text);
    std::string      getClipboardString();

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

#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY)
	Display* 	getX11Display();
	Window  	getX11Window();
	XIC			getX11XIC();

	void setWindowIcon(const std::string & path);
	void setWindowIcon(const ofPixels & iconPixels);
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
	static void 	keyboard_cb(GLFWwindow* windowP_, int key, int scancode, int action, int mods);
	static void 	char_cb(GLFWwindow* windowP_, uint32_t key);
	static void 	resize_cb(GLFWwindow* windowP_, int w, int h);
	static void 	framebuffer_size_cb(GLFWwindow* windowP_, int w, int h);
	static void 	exit_cb(GLFWwindow* windowP_);
	static void		scroll_cb(GLFWwindow* windowP_, double x, double y);
	static void 	drop_cb(GLFWwindow* windowP_, int numFiles, const char** dropString);
	static void		error_cb(int errorCode, const char* errorDescription);

	void close();

	#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY)
		XIM xim;
		XIC xic;
	#endif

	std::unique_ptr<ofCoreEvents> coreEvents;
	std::shared_ptr<ofBaseRenderer> currentRenderer;
	ofGLFWWindowSettings settings;

	ofWindowMode	targetWindowMode;

	bool			bEnableSetupScreen;
	int				windowW, windowH;		/// Physical framebuffer pixels extents
	int				currentW, currentH;		/// Extents of the window client area, which may be scaled by pixelsScreenCoordScale to map to physical framebuffer pixels.
	float           pixelScreenCoordScale;  /// Scale factor from virtual operating-system defined client area extents (as seen in currentW, currentH) to physical framebuffer pixel coordinates (as seen in windowW, windowH).

	ofRectangle windowRect;

	int				buttonInUse;
	bool			buttonPressed;

	int 			nFramesSinceWindowResized;
	bool			bWindowNeedsShowing;
	
	#ifdef TARGET_RASPBERRY_PI 
	bool			needsResizeCheck = false; /// Just for RPI at this point
	#endif	

	GLFWwindow* 	windowP;

	int				getCurrentMonitor();

	ofBaseApp *	ofAppPtr;


	ofOrientation orientation;

	bool iconSet;

    #ifdef TARGET_WIN32
    LONG lExStyle, lStyle;
    #endif // TARGET_WIN32
};


//#endif
