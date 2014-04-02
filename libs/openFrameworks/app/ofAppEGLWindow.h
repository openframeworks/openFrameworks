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

// TODO: this shold be passed in with the other window settings, like window alpha, etc.
enum ofAppEGLWindowType {
	OF_APP_WINDOW_AUTO,
	OF_APP_WINDOW_NATIVE,
	OF_APP_WINDOW_X11
};

typedef map<EGLint,EGLint> ofEGLAttributeList;
typedef map<EGLint,EGLint>::iterator ofEGLAttributeListIterator;

class ofAppEGLWindow : public ofAppBaseWindow, public ofThread {
public:

	struct Settings;

	ofAppEGLWindow();
	ofAppEGLWindow(Settings settings);
	virtual ~ofAppEGLWindow();

	void exit(ofEventArgs &e);
    void setThreadTimeout(long timeOut){ threadTimeout = timeOut; }
    void setGLESVersion(int glesVersion);
	virtual void setupOpenGL(int w, int h, int screenMode);

	virtual void initializeWindow();
	virtual void runAppViaInfiniteLoop(ofBaseApp * appPtr);

	virtual void hideCursor();
	virtual void showCursor();

	virtual void	setWindowPosition(int x, int y);
	virtual void	setWindowShape(int w, int h);

	virtual ofPoint	getWindowPosition();
	virtual ofPoint	getWindowSize();
	virtual ofPoint	getScreenSize();

	virtual void			setOrientation(ofOrientation orientation);
	virtual ofOrientation	getOrientation();
	virtual bool			doesHWOrientation();

	//this is used by ofGetWidth and now determines the window width based on orientation
	virtual int		getWidth();
	virtual int		getHeight();

	virtual void	setWindowTitle(string title); // TODO const correct

	virtual int		getWindowMode(); // TODO use enum

	virtual void	setFullscreen(bool fullscreen);
	virtual void	toggleFullscreen();

	virtual void	enableSetupScreen();
	virtual void	disableSetupScreen();

	virtual void	setVerticalSync(bool enabled);
	
	struct Settings {
		ofAppEGLWindowType eglWindowPreference;  // what window type is preferred?
		EGLint eglWindowOpacity; // 0-255 window alpha value

		ofEGLAttributeList frameBufferAttributes;
		// surface creation
		ofEGLAttributeList windowSurfaceAttributes;

		ofColor initialClearColor;

		int screenNum;
		int layer;

		Settings();
	};

	EGLDisplay getEglDisplay() const;
	EGLSurface getEglSurface() const;
	EGLContext getEglContext() const;

#ifndef TARGET_RASPBERRY_PI
	Display* 	getX11Display();
	Window  	getX11Window();
#endif

	EGLConfig  getEglConfig() const;

	EGLint getEglVersionMajor () const;
	EGLint getEglVersionMinor() const;


protected:
	void init(Settings settings = Settings());

	void idle();
	void display();

	void setWindowRect(const ofRectangle& requestedWindowRect);


//	bool create

	virtual void setupPeripherals();

	virtual ofRectangle getScreenRect();

	int getWindowWidth();
	int getWindowHeight();

	bool     terminate;

	int      windowMode;
	bool     bNewScreenMode;
	int      buttonInUse;
	bool     bEnableSetupScreen;
	bool	 bShowCursor;

	string   eglDisplayString;
	int      nFramesSinceWindowResized;
	ofOrientation orientation;
	ofBaseApp *  ofAppPtr;


	void threadedFunction();
	queue<ofMouseEventArgs> mouseEvents;
	queue<ofKeyEventArgs>   keyEvents;
	void checkEvents();
	ofImage mouseCursor;

	// TODO: getters and setters?  OR automatically set based on 
	// OS or screen size?  Should be changed when screen is resized?
	float mouseScaleX;
	float mouseScaleY;


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

	bool isUsingX11;

	bool isWindowInited;
	bool isSurfaceInited;

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

	DISPMANX_CLAMP_T 		  dispman_clamp;
	DISPMANX_TRANSFORM_T 	  dispman_transform;
    VC_DISPMANX_ALPHA_T       dispman_alpha;
	
	bool createRPiNativeWindow(const ofRectangle& requestedWindowRect);

#else
	// if you are not raspberry pi, you will not be able to
	// create a window without using x11.
#endif

	Display*	x11Display;
	Screen*		x11Screen;
	Window		x11Window;
	long 		x11ScreenNum;
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

	void handleX11Event(const XEvent& event);

private:
	Settings 			settings;
	int glesVersion;
	bool keyboardDetected;
	bool mouseDetected;
	long threadTimeout;
};
