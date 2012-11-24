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
#include "X11/Xlib.h"
#include "X11/Xutil.h"

class ofAppEGLWindow : public ofAppBaseWindow {
public:
	ofAppEGLWindow();
	virtual ~ofAppEGLWindow();

	virtual void setupOpenGL(int w, int h, int screenMode);

	virtual void setupEGL(NativeWindowType nativeWindow, Display * display=NULL);
	virtual void destroyEGL();

	virtual void initializeWindow();
	virtual void runAppViaInfiniteLoop(ofBaseApp * appPtr);

	virtual void hideCursor();
	virtual void showCursor();

	virtual void	setWindowPosition(int x, int y);
	virtual void	setWindowShape(int w, int h);

	virtual int		getFrameNum();
	virtual float	getFrameRate();
	virtual double  getLastFrameTime();

	virtual ofPoint	getWindowPosition();
	virtual ofPoint	getWindowSize();
	virtual ofPoint	getScreenSize();

	virtual void			setOrientation(ofOrientation orientation);
	virtual ofOrientation	getOrientation();
	virtual bool			doesHWOrientation();

	//this is used by ofGetWidth and now determines the window width based on orientation
	virtual int		getWidth();
	virtual int		getHeight();

	virtual void	setFrameRate(float targetRate);
	virtual void	setWindowTitle(string title); // TODO const correct

	virtual int		getWindowMode(); // TODO use enum

	virtual void	setFullscreen(bool fullscreen);
	virtual void	toggleFullscreen();

	virtual void	enableSetupScreen();
	virtual void	disableSetupScreen();

protected:

	bool setupX11NativeWindow(int w, int h, int screenMode);
	bool setupRPiNativeWindow(int w, int h, int screenMode);

	void idle();
	virtual void postIdle() {};
	void display();
	virtual void postDisplay() {};

	void infiniteLoop();

	void setWindowRect(const ofRectangle& requestedWindowRect) {
		if(requestedWindowRect != currentWindowRect) {
			ofRectangle oldWindowRect = currentWindowRect;

			currentWindowRect = requestNewWindowRect(requestedWindowRect);
			
			if(oldWindowRect != currentWindowRect) {
				ofNotifyWindowResized(currentWindowRect.width,currentWindowRect.height);
				nFramesSinceWindowResized = 0;
			}
		}
	}

	virtual ofRectangle getScreenRect();
	virtual ofRectangle requestNewWindowRect(const ofRectangle&);
	int getWindowWidth() {
		return currentWindowRect.width;
	}


	int getWindowHeight() {
		return currentWindowRect.height;
	}

	bool     terminate;

	int      windowMode;
	bool     bNewScreenMode;
	float    timeNow, timeThen, fps;
	int      nFramesForFPS;
	int      nFrameCount;
	int      buttonInUse;
	bool     bEnableSetupScreen;


	bool     bFrameRateSet;
	int      millisForFrame;
	int      prevMillis;
	int      diffMillis;

	float    frameRate;

	double   lastFrameTime;

//	int      requestedWidth;
//	int      requestedHeight;
//	int      nonFullScreenX;
//	int      nonFullScreenY;
//	int      windowW;
//	int      windowH;
	string   eglDisplayString;
	int      nFramesSinceWindowResized;
	ofOrientation orientation;
	ofBaseApp *  ofAppPtr;

	// TODO thread for keyboard and mouse input?


	// EGL window
	ofRectangle screenRect;
	ofRectangle nonFullscreenWindowRect; // the rectangle describing the non-fullscreen window
	ofRectangle currentWindowRect; // the rectangle describing the current device

	EGLDisplay eglDisplay;  // EGL display connection
	EGLSurface eglSurface;
	EGLContext eglContext;
	














// void windowResized(int w, int h) {
// 	windowW = w;
// 	windowH = h;

// 	ofNotifyWindowResized(w, h);

// 	nFramesSinceWindowResized = 0;
// }

// 		virtual void windowEntry ( int state ) { }




















// 	string getEGLErrorString(){
// 		EGLint nErr = eglGetError();
// 		switch(nErr){
// 			case EGL_SUCCESS: 				return "EGL_SUCCESS";
// 			case EGL_BAD_DISPLAY:			return "EGL_BAD_DISPLAY";
// 			case EGL_NOT_INITIALIZED:		return "EGL_NOT_INITIALIZED";
// 			case EGL_BAD_ACCESS:			return "EGL_BAD_ACCESS";
// 			case EGL_BAD_ALLOC:				return "EGL_BAD_ALLOC";
// 			case EGL_BAD_ATTRIBUTE:			return "EGL_BAD_ATTRIBUTE";
// 			case EGL_BAD_CONFIG:			return "EGL_BAD_CONFIG";
// 			case EGL_BAD_CONTEXT:			return "EGL_BAD_CONTEXT";
// 			case EGL_BAD_CURRENT_SURFACE:	return "EGL_BAD_CURRENT_SURFACE";
// 			case EGL_BAD_MATCH:				return "EGL_BAD_MATCH";
// 			case EGL_BAD_NATIVE_PIXMAP:		return "EGL_BAD_NATIVE_PIXMAP";
// 			case EGL_BAD_NATIVE_WINDOW:		return "EGL_BAD_NATIVE_WINDOW";
// 			case EGL_BAD_PARAMETER:			return "EGL_BAD_PARAMETER";
// 			case EGL_BAD_SURFACE:			return "EGL_BAD_SURFACE";
// 			default:						return "unknown";
// 		}

// 	};


//     void printConfigAttribuet(EGLDisplay display, EGLConfig config, string attributeString, EGLint attribute) {

//     	EGLint value = 0;

//     	if(eglGetConfigAttrib(display,config,,value)) {
//     		cout << attributeString << "=" << value << endl;
//     	} else {
//     		cout << attributeString << " failed with error " << getEGLErrorString() << endl;
//     	}
//     }

//     void printEGLConfigInfo(EGLDisplay display, 
//     						EGLConfig config) {




// EGL_BUFFER_SIZE
// EGL_RED_SIZE
// EGL_GREEN_SIZE
// EGL_BLUE_SIZE
// EGL_ALPHA_SIZE
// EGL_CONFIG_CAVEAT
// EGL_DEPTH_SIZE
// EGL_LEVEL
// EGL_MAX_PBUFFER_WIDTH
// EGL_MAX_PBUFFER_HEIGHT
// EGL_MAX_PBUFFER_PIXELS
// EGL_NATIVE_RENDERABLE
// EGL_NATIVE_VISUAL_ID
// EGL_NATIVE_VISUAL_TYPE
// EGL_PRESERVED_RESOURCES
// EGL_SAMPLE_BUFFERS
// EGL_SAMPLES
// EGL_STENCIL_BITS
// EGL_SURFACE_TYPE
// EGL_TRANSPARENT_TYPE
// EGL_TRANSPARENT_RED
// EGL_TRANSPARENT_GREEN
// EGL_TRANSPARENT_BLUE





};
