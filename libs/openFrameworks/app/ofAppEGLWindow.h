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

#ifndef TARGET_RASPBERRY_PI
	#include "X11/Xlib.h"
	#include "X11/Xutil.h"
#endif

class ofAppEGLWindow : public ofAppBaseWindow {
public:
	ofAppEGLWindow();
	virtual ~ofAppEGLWindow();

	virtual void setupOpenGL(int w, int h, int screenMode);

	virtual bool setupEGL(NativeWindowType nativeWindow, NativeDisplayType * display=NULL);
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
	



#ifdef TARGET_RASPBERRY_PI
  // NOTE: EGL_DISPMANX_WINDOW_T nativeWindow is a var that must stay in scope
  EGL_DISPMANX_WINDOW_T nativeWindow; // rpi
#else
  Window nativeWindow; // x11
#endif







};