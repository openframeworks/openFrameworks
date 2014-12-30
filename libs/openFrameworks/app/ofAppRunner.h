#pragma once

#include "ofConstants.h"
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofTypes.h"
#include "ofWindowSettings.h"
#include "ofMainLoop.h"

class ofAppBaseWindow;
class ofAppBaseGLWindow;
class ofAppBaseGLESWindow;
class ofBaseApp;
class ofBaseRenderer;
class ofCoreEvents;

void ofInit();
void 		ofSetupOpenGL(int w, int h, ofWindowMode screenMode);	// sets up the opengl context!
shared_ptr<ofAppBaseWindow> ofCreateWindow(const ofWindowSettings & settings);	// sets up the opengl context!

shared_ptr<ofMainLoop> ofGetMainLoop();

template<typename Window>
void ofSetupOpenGL(shared_ptr<Window> windowPtr, int w, int h, ofWindowMode screenMode){
	ofInit();
	ofWindowSettings settings;
	settings.width = w;
	settings.height = h;
	settings.windowMode = screenMode;
	ofGetMainLoop()->addWindow(windowPtr);
	windowPtr->setup(settings);
}


template<typename Window>
static void noopDeleter(Window*){}

template<typename Window>
void ofSetupOpenGL(Window * windowPtr, int w, int h, ofWindowMode screenMode){
	shared_ptr<Window> window = shared_ptr<Window>(windowPtr,std::ptr_fun(noopDeleter<Window>));
	ofSetupOpenGL(window,w,h,screenMode);
}


int 		ofRunApp(shared_ptr<ofBaseApp> OFSA);
int 		ofRunApp(ofBaseApp * OFSA = NULL); // will be deprecated
void ofRunApp(shared_ptr<ofAppBaseWindow> window, shared_ptr<ofBaseApp> app);
int ofRunMainLoop();


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

/// \returns a random number between 0 and the width of the window.
float ofRandomWidth();

/// \returns a random number between 0 and the height of the window.
float ofRandomHeight();
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

ofCoreEvents & ofEvents();
void ofSetCurrentRenderer(shared_ptr<ofBaseRenderer> renderer,bool setDefaults=false);
shared_ptr<ofBaseRenderer> & ofGetCurrentRenderer();
void ofSetEscapeQuitsApp(bool bQuitOnEsc);

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
