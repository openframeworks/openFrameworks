#pragma once

#include "ofConstants.h"
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofTypes.h"

class ofAppBaseWindow;
class ofBaseApp;
class ofBaseRenderer;

void 		ofSetupOpenGL(ofPtr<ofAppBaseWindow> windowPtr, int w, int h, int screenMode);	// sets up the opengl context!
void 		ofSetupOpenGL(int w, int h, int screenMode);	// sets up the opengl context!
void 		ofSetupOpenGL(ofAppBaseWindow * windowPtr, int w, int h, int screenMode);  // will be deprecated

void 		ofRunApp(ofPtr<ofBaseApp> OFSA);
void 		ofRunApp(ofBaseApp * OFSA = NULL); // will be deprecated


ofBaseApp * ofGetAppPtr();
void ofSetAppPtr(ofPtr<ofBaseApp> appPtr);

void		ofExit(int status=0);

//-------------------------- time
float 		ofGetFrameRate();
float 		ofGetTargetFrameRate();
int			ofGetFrameNum();
void 		ofSetFrameRate(int targetRate);
void		ofSleepMillis(int millis);
double		ofGetLastFrameTime();

void		ofSetOrientation(ofOrientation orientation, bool vFlip=true);
ofOrientation			ofGetOrientation();

//-------------------------- cursor
void 		ofHideCursor();
void 		ofShowCursor();
//-------------------------- window / screen
int 		ofGetWindowPositionX();
int 		ofGetWindowPositionY();
int 		ofGetScreenWidth();
int 		ofGetScreenHeight();
int			ofGetWindowMode();
int 		ofGetWidth();			// ofGetWidth is correct for orientation
int 		ofGetHeight();
int 		ofGetWindowWidth();			// ofGetWindowWidth is correct for actual window coordinates - so doesn't change with orientation. 
int 		ofGetWindowHeight();
bool		ofDoesHWOrientation();
ofPoint		ofGetWindowSize();
ofRectangle	ofGetWindowRect();
ofAppBaseWindow * ofGetWindowPtr();

void 		ofSetWindowPosition(int x, int y);
void 		ofSetWindowShape(int width, int height);
void 		ofSetWindowTitle(string title);
void		ofEnableSetupScreen();
void		ofDisableSetupScreen();
void		ofSetFullscreen(bool fullscreen);
void		ofToggleFullscreen();
//-------------------------- sync
void 		ofSetVerticalSync(bool bSync);

//-------------------------- native window handles
#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI)
#include <X11/Xlib.h>
	Display* ofGetX11Display();
	Window  ofGetX11Window();
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
	GLXContext ofGetGLXContext();
#endif

#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
	EGLDisplay ofGetEGLDisplay();
	EGLContext ofGetEGLContext();
	EGLSurface ofGetEGLSurface();
#endif

#if defined(TARGET_OSX)
	void * ofGetNSGLContext();
	void * ofGetCocoaWindow();
#endif

#if defined(TARGET_WIN32)
	HGLRC ofGetWGLContext();
	HWND ofGetWin32Window();
#endif
