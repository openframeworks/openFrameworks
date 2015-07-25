/*==============================================================================

 Copyright (c) 2011, 2012 Christopher Baker <http://christopherbaker.net>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 modified by Philip Whitfield (http://www.undef.ch)

 ==============================================================================*/

#pragma once

#include "ofBaseApp.h"

#include "ofAppBaseWindow.h"
#include "ofThread.h"
#include "ofImage.h"
#include "ofBaseTypes.h"
#include "ofEvents.h"
#include "ofConstants.h"
#include "ofTypes.h"

// include includes for both native and X11 possibilities
#include <libudev.h>
#include <stdbool.h>
#include <stdio.h> // sprintf
#include <stdlib.h>  // malloc
#include <math.h>
#include <fcntl.h>  // open fcntl
#include <unistd.h> // read close 
#include <linux/joystick.h>

#include "linux/kd.h"	// keyboard stuff...
#include "termios.h"
#include "sys/ioctl.h"

#include <dirent.h>  // scandir
#include <string.h> // strlen

// x11
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <queue>
#include <map>

#include <EGL/egl.h>

// TODO: this shold be passed in with the other window settings, like window alpha, etc.
enum ofAppEGLWindowType {
	OF_APP_WINDOW_AUTO,
	OF_APP_WINDOW_NATIVE,
	OF_APP_WINDOW_X11
};

typedef map<EGLint,EGLint> ofEGLAttributeList;
typedef map<EGLint,EGLint>::iterator ofEGLAttributeListIterator;

class ofAppEGLWindow : public ofAppBaseGLESWindow, public ofThread {
public:

	struct Settings;

	ofAppEGLWindow();
	virtual ~ofAppEGLWindow();

	static void loop(){};
	static bool doesLoop(){ return false; }
	static bool allowsMultiWindow(){ return false; }
	static bool needsPolling(){ return true; }
	static void pollEvents();

	void setup(const Settings & settings);
	void setup(const ofGLESWindowSettings & settings);
	void update();
	void draw();
	void close();
	void makeCurrent();

	ofCoreEvents & events();
	shared_ptr<ofBaseRenderer> & renderer();

	void setThreadTimeout(long timeOut){ threadTimeout = timeOut; }

	virtual void hideCursor();
	virtual void showCursor();

	virtual void setWindowPosition(int x, int y);
	virtual void setWindowShape(int w, int h);

	virtual ofPoint	getWindowPosition();
	virtual ofPoint	getWindowSize();
	virtual ofPoint	getScreenSize();

	virtual void setOrientation(ofOrientation orientation);
	virtual ofOrientation getOrientation();
	virtual bool doesHWOrientation();

	//this is used by ofGetWidth and now determines the window width based on orientation
	virtual int	getWidth();
	virtual int	getHeight();

	virtual void setWindowTitle(string title); // TODO const correct

	virtual ofWindowMode getWindowMode();

	virtual void setFullscreen(bool fullscreen);
	virtual void toggleFullscreen();

	virtual void enableSetupScreen();
	virtual void disableSetupScreen();

	virtual void setVerticalSync(bool enabled);
	
	struct Settings: public ofGLESWindowSettings {
		public:
		ofAppEGLWindowType eglWindowPreference;  // what window type is preferred?
		EGLint eglWindowOpacity; // 0-255 window alpha value

		ofEGLAttributeList frameBufferAttributes;
		// surface creation
		ofEGLAttributeList windowSurfaceAttributes;

		ofColor initialClearColor;

		int screenNum;
		int layer;

		Settings();
		Settings(const ofGLESWindowSettings & settings);
	};

	EGLDisplay getEglDisplay() const;
	EGLSurface getEglSurface() const;
	EGLContext getEglContext() const;

#ifndef TARGET_RASPBERRY_PI
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

	string eglDisplayString;
	int nFramesSinceWindowResized;  ///< \brief The number of frames passed/shown since the window got resized.
	ofOrientation orientation;


	void threadedFunction();
	queue<ofMouseEventArgs> mouseEvents;
	queue<ofKeyEventArgs>   keyEvents;
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

#ifdef TARGET_RASPBERRY_PI
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
	
	void setupNativeMouse();
	void setupNativeKeyboard();

	void destroyNativeMouse();
	void destroyNativeKeyboard();

	void readNativeMouseEvents();
	void readNativeKeyboardEvents();
	void readNativeUDevEvents();

	static void handleX11Event(const XEvent& event);

private:
	Settings settings;
	int glesVersion;  ///< \brief Indicate the version of OpenGL for Embedded Systems.
	bool keyboardDetected;
	bool mouseDetected;
	long threadTimeout;
	ofCoreEvents coreEvents;
	shared_ptr<ofBaseRenderer> currentRenderer;
	static ofAppEGLWindow * instance;
};
