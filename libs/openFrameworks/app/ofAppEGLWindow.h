#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofThread.h"
#include "ofImage.h"
#include "ofEvents.h"
#include "ofRectangle.h"


#include <queue>
#include <map>
#include <X11/Xlib.h>

enum ofAppEGLWindowType {
	OF_APP_WINDOW_AUTO,
	OF_APP_WINDOW_NATIVE,
	OF_APP_WINDOW_X11
};

typedef struct _XIM * XIM;
typedef struct _XIC * XIC;
typedef unsigned long Window;
struct _XDisplay;
typedef struct _XDisplay Display;

typedef unsigned int EGLBoolean;
typedef int32_t EGLint;
typedef void *EGLDisplay;
typedef void *EGLConfig;
typedef void *EGLSurface;
typedef void *EGLContext;

typedef std::map<EGLint,EGLint> ofEGLAttributeList;
typedef std::map<EGLint,EGLint>::iterator ofEGLAttributeListIterator;

struct ofAppEGLWindowSettings: public ofGLESWindowSettings {
public:
	ofAppEGLWindowType eglWindowPreference; ///< what window type is preferred?
	EGLint eglWindowOpacity; ///< 0-255 window alpha value

	ofEGLAttributeList frameBufferAttributes;
	// surface creation
	ofEGLAttributeList windowSurfaceAttributes;

	ofColor initialClearColor;

	int screenNum;
	int layer;

	ofAppEGLWindowSettings();
	ofAppEGLWindowSettings(const ofGLESWindowSettings & settings);
};

class ofAppEGLWindow : public ofAppBaseGLESWindow, public ofThread {
public:

	/// ofAppEGLWindow::Settings is currently deprecated in favor of
	/// the ofAppEGLWindowSettings struct
	typedef ofAppEGLWindowSettings Settings;

	ofAppEGLWindow();
	virtual ~ofAppEGLWindow();

	static void loop(){};
	static bool doesLoop(){ return false; }
	static bool allowsMultiWindow(){ return false; }
	static bool needsPolling(){ return true; }
	static void pollEvents();

	using ofAppBaseGLESWindow::setup;
	void setup(const ofAppEGLWindowSettings & settings);
	void setup(const ofGLESWindowSettings & settings);

	void update();
	void draw();
	void close();
	void makeCurrent();
	void swapBuffers();
	void startRender();
	void finishRender();

	ofCoreEvents & events();
	std::shared_ptr<ofBaseRenderer> & renderer();

	void setThreadTimeout(long timeOut){ threadTimeout = timeOut; }

	virtual void hideCursor();
	virtual void showCursor();

	virtual void setWindowPosition(int x, int y);
	virtual void setWindowShape(int w, int h);

	virtual glm::vec2 getWindowPosition();
	virtual glm::vec2 getWindowSize();
	virtual glm::vec2 getScreenSize();

	virtual void setOrientation(ofOrientation orientation);
	virtual ofOrientation getOrientation();
	virtual bool doesHWOrientation();

	//this is used by ofGetWidth and now determines the window width based on orientation
	virtual int	getWidth();
	virtual int	getHeight();

	virtual void setWindowTitle(std::string title); // TODO const correct

	virtual ofWindowMode getWindowMode();

	virtual void setFullscreen(bool fullscreen);
	virtual void toggleFullscreen();

	virtual void enableSetupScreen();
	virtual void disableSetupScreen();

	virtual void setVerticalSync(bool enabled);

	EGLDisplay getEglDisplay() const;
	EGLSurface getEglSurface() const;
	EGLContext getEglContext() const;

#ifndef TARGET_RASPBERRY_PI_LEGACY
	Display* getX11Display();
	Window getX11Window();
#endif

	EGLConfig getEglConfig() const;

	EGLint getEglVersionMajor () const;
	EGLint getEglVersionMinor() const;


protected:
	void setWindowRect(const ofRectangle& requestedWindowRect);


//	bool create

	virtual void setupPeripherals();

	virtual ofRectangle getScreenRect();

	int getWindowWidth();
	int getWindowHeight();

	ofWindowMode windowMode;
	bool bNewScreenMode;  ///< \brief This indicates if a (new) window rectangle has to be adjusted.
	int	buttonInUse;  ///< \brief Mouse button currently in use.
	bool bEnableSetupScreen;  ///< \brief This indicates the need/intent to draw a setup screen.
	bool bShowCursor;  ///< \brief Indicate the visibility of the (mouse) cursor.

	std::string eglDisplayString;
	int nFramesSinceWindowResized;  ///< \brief The number of frames passed/shown since the window got resized.
	ofOrientation orientation;


	void threadedFunction();
	std::queue<ofMouseEventArgs> mouseEvents;
	std::queue<ofKeyEventArgs>   keyEvents;
	std::queue<ofTouchEventArgs> touchEvents;
	void checkEvents();
	ofImage mouseCursor;

	// TODO: getters and setters?  OR automatically set based on
	// OS or screen size?  Should be changed when screen is resized?
	float mouseScaleX;  ///< \brief Amount by which to mouse movements along the X axis.
	float mouseScaleY;  ///< \brief Amount by which to mouse movements along the Y axis.


	// float getMouseScaleX() const;
	// void setMouseScaleX(float x);
	// float getMouseScaleY() const;
	// void setMouseScaleY(float y);

	// For absolute input devices that send ABS_X and ABS_Y events, we want to store
	// information about the min and max axis values.
	int mouseAbsXMin;
	int mouseAbsXMax;
	int mouseAbsYMin;
	int mouseAbsYMax;

	bool hasMouse() { return mouseDetected; }
	bool hasKeyboard() { return keyboardDetected; }


//------------------------------------------------------------
// EGL
//------------------------------------------------------------

	bool createSurface();
	bool destroySurface();

	// bool resizeSurface();

	EGLDisplay eglDisplay;  // EGL display connection
	EGLSurface eglSurface;
	EGLContext eglContext;

	EGLConfig eglConfig;

	EGLint eglVersionMajor;
	EGLint eglVersionMinor;

//------------------------------------------------------------
// PLATFORM SPECIFIC WINDOWING
//------------------------------------------------------------

//------------------------------------------------------------
// WINDOWING
//------------------------------------------------------------
	// EGL window
	ofRectangle nonFullscreenWindowRect; // the rectangle describing the non-fullscreen window
	ofRectangle currentWindowRect; // the rectangle describing the current device

	bool createWindow(const ofRectangle& requestedWindowRect);
	bool destroyWindow();

	bool isUsingX11;  ///< \brief Indicate the use of the X Window System.

	bool isWindowInited;  ///< \brief Indicate that the window is (properly) initialized.
	bool isSurfaceInited;  ///< \brief Indicate that the surface is (properly) initialized.

	void initNative();
	void exitNative();

	EGLNativeWindowType getNativeWindow();
	EGLNativeDisplayType getNativeDisplay();

#ifdef TARGET_RASPBERRY_PI_LEGACY
	void initRPiNative();
	void exitRPiNative();

	EGL_DISPMANX_WINDOW_T dispman_native_window; // rpi

	DISPMANX_UPDATE_HANDLE_T dispman_update;
	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;

	DISPMANX_CLAMP_T  dispman_clamp;
	DISPMANX_TRANSFORM_T dispman_transform;
	VC_DISPMANX_ALPHA_T	dispman_alpha;

	bool createRPiNativeWindow(const ofRectangle& requestedWindowRect);

#else
	// if you are not raspberry pi, you will not be able to
	// create a window without using x11.
#endif

	Display* x11Display;  ///< \brief Indicate which X11 display is in use (currently).
	Screen* x11Screen;  ///< \brief Indicate which X11 screen is in use (currently).
	Window x11Window;
	long x11ScreenNum;  ///< \brief The number of the X11 screen is in use (currently).
	bool createX11NativeWindow(const ofRectangle& requestedWindowRect);

//------------------------------------------------------------
// EVENTS
//------------------------------------------------------------
	void setupNativeEvents();
	void destroyNativeEvents();

	void setupNativeUDev();
	void destroyNativeUDev();

	void setupNativeInput();
	void destroyNativeInput();

	void readNativeUDevEvents();
	void readNativeInputEvents();

	void processInput(int fd, const char * node);	
	void addInput(const char * node, bool isMouse);
	void removeInput(const char * node);
	void printInput();

	static void handleX11Event(const XEvent& event);

private:
	ofAppEGLWindowSettings settings;
	int glesVersion;  ///< \brief Indicate the version of OpenGL for Embedded Systems.
	bool keyboardDetected;
	bool mouseDetected;
	long threadTimeout;
	ofCoreEvents coreEvents;
	std::shared_ptr<ofBaseRenderer> currentRenderer;
	static ofAppEGLWindow * instance;
};
