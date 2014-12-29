#pragma once

#include "ofConstants.h"
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofTypes.h"

class ofAppBaseWindow;
class ofAppBaseGLWindow;
class ofAppBaseGLESWindow;
class ofBaseApp;
class ofBaseRenderer;

void 		ofSetupOpenGL(int w, int h, ofWindowMode screenMode);	// sets up the opengl context!
#ifdef TARGET_OPENGLES
void		ofSetOpenGLESVersion(int version);
int			ofGetOpenGLESVersion();
string		ofGetGLSLVersion();
void 		ofSetupOpenGL(shared_ptr<ofAppBaseGLESWindow> windowPtr, int w, int h, ofWindowMode screenMode);	// sets up the opengl context!
void 		ofSetupOpenGL(ofAppBaseGLESWindow * windowPtr, int w, int h, ofWindowMode screenMode);  // will be deprecated
#else
void		ofSetOpenGLVersion(int major, int minor);
int			ofGetOpenGLVersionMajor();
int			ofGetOpenGLVersionMinor();
string		ofGetGLSLVersion();
void 		ofSetupOpenGL(shared_ptr<ofAppBaseGLWindow> windowPtr, int w, int h, ofWindowMode screenMode);	// sets up the opengl context!
void 		ofSetupOpenGL(ofAppBaseGLWindow * windowPtr, int w, int h, ofWindowMode screenMode);  // will be deprecated
#endif
void		ofSetWindow(ofAppBaseWindow * windowPtr);
void		ofSetWindow(shared_ptr<ofAppBaseWindow> windowPtr);
void		OF_DEPRECATED_MSG("use ofSetWindow for non GL windows",ofSetupOpenGL(ofAppBaseWindow * windowPtr, int w, int h, ofWindowMode screenMode));

void 		ofRunApp(shared_ptr<ofBaseApp> OFSA);
void 		ofRunApp(ofBaseApp * OFSA = NULL); // will be deprecated


ofBaseApp * ofGetAppPtr();
void ofSetAppPtr(shared_ptr<ofBaseApp> appPtr);

void		ofExit(int status=0);

//-------------------------- time
float 		ofGetFrameRate();
float 		ofGetTargetFrameRate();
int			ofGetFrameNum();
void 		ofSetFrameRate(int targetRate);
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
