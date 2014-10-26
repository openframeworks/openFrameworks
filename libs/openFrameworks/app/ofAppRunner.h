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
class ofCoreEvents;
class ofWindowSettings;

OF_INTERNAL_DEPRECATED(void 		ofSetupOpenGL(int w, int h, ofWindowMode screenMode));	// sets up the opengl context!
OF_INTERNAL_DEPRECATED(shared_ptr<ofAppBaseWindow> ofCreateWindow(const ofWindowSettings & settings));	// sets up the opengl context!
#ifdef TARGET_OPENGLES
void 		ofSetupOpenGL(shared_ptr<ofAppBaseGLESWindow> windowPtr, int w, int h, ofWindowMode screenMode);	// sets up the opengl context!
void 		ofSetupOpenGL(ofAppBaseGLESWindow * windowPtr, int w, int h, ofWindowMode screenMode);  // will be deprecated
#else
OF_INTERNAL_DEPRECATED(void 		ofSetupOpenGL(shared_ptr<ofAppBaseGLWindow> windowPtr, int w, int h, ofWindowMode screenMode));	// sets up the opengl context!
OF_INTERNAL_DEPRECATED(void 		ofSetupOpenGL(ofAppBaseGLWindow * windowPtr, int w, int h, ofWindowMode screenMode));  // will be deprecated
#endif
void		OF_DEPRECATED_MSG("use ofSetWindow for non GL windows",ofSetupOpenGL(ofAppBaseWindow * windowPtr, int w, int h, ofWindowMode screenMode));

OF_INTERNAL_DEPRECATED(int 		ofRunApp(shared_ptr<ofBaseApp> OFSA));
OF_INTERNAL_DEPRECATED(int 		ofRunApp(ofBaseApp * OFSA = NULL)); // will be deprecated
void ofRunApp(shared_ptr<ofAppBaseWindow> window, shared_ptr<ofBaseApp> app);
int ofRunMainLoop();


OF_INTERNAL_DEPRECATED(ofBaseApp * ofGetAppPtr());
OF_INTERNAL_DEPRECATED(void ofSetAppPtr(shared_ptr<ofBaseApp> appPtr));

OF_INTERNAL_DEPRECATED(void		ofExit(int status=0));

//-------------------------- time
OF_INTERNAL_DEPRECATED(float 		ofGetFrameRate());
OF_INTERNAL_DEPRECATED(float 		ofGetTargetFrameRate());
OF_INTERNAL_DEPRECATED(int			ofGetFrameNum());
OF_INTERNAL_DEPRECATED(void 		ofSetFrameRate(int targetRate));
OF_INTERNAL_DEPRECATED(double		ofGetLastFrameTime());

OF_INTERNAL_DEPRECATED(void		ofSetOrientation(ofOrientation orientation, bool vFlip=true));
OF_INTERNAL_DEPRECATED(ofOrientation			ofGetOrientation());

//-------------------------- cursor
OF_INTERNAL_DEPRECATED(void 		ofHideCursor());
OF_INTERNAL_DEPRECATED(void 		ofShowCursor());
//-------------------------- window / screen
OF_INTERNAL_DEPRECATED(int 		ofGetWindowPositionX());
OF_INTERNAL_DEPRECATED(int 		ofGetWindowPositionY());
OF_INTERNAL_DEPRECATED(int 		ofGetScreenWidth());
OF_INTERNAL_DEPRECATED(int 		ofGetScreenHeight());
OF_INTERNAL_DEPRECATED(int			ofGetWindowMode());
OF_INTERNAL_DEPRECATED(int 		ofGetWidth());			// ofGetWidth is correct for orientation
OF_INTERNAL_DEPRECATED(int 		ofGetHeight());
OF_INTERNAL_DEPRECATED(int 		ofGetWindowWidth());			// ofGetWindowWidth is correct for actual window coordinates - so doesn't change with orientation.
OF_INTERNAL_DEPRECATED(int 		ofGetWindowHeight());
OF_INTERNAL_DEPRECATED(bool		ofDoesHWOrientation());
OF_INTERNAL_DEPRECATED(ofPoint		ofGetWindowSize());
OF_INTERNAL_DEPRECATED(ofRectangle	ofGetWindowRect());
OF_INTERNAL_DEPRECATED(ofAppBaseWindow * ofGetWindowPtr());

OF_INTERNAL_DEPRECATED(void 		ofSetWindowPosition(int x, int y));
OF_INTERNAL_DEPRECATED(void 		ofSetWindowShape(int width, int height));
OF_INTERNAL_DEPRECATED(void 		ofSetWindowTitle(string title));
OF_INTERNAL_DEPRECATED(void		ofEnableSetupScreen());
OF_INTERNAL_DEPRECATED(void		ofDisableSetupScreen());
OF_INTERNAL_DEPRECATED(void		ofSetFullscreen(bool fullscreen));
OF_INTERNAL_DEPRECATED(void		ofToggleFullscreen());
//-------------------------- sync
OF_INTERNAL_DEPRECATED(void 		ofSetVerticalSync(bool bSync));

ofCoreEvents & ofEvents();
void ofSetCurrentRenderer(shared_ptr<ofBaseRenderer> renderer,bool setDefaults=false);
shared_ptr<ofBaseRenderer> & ofGetCurrentRenderer();

//-------------------------- native window handles
#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI)
#include <X11/Xlib.h>
OF_INTERNAL_DEPRECATED(Display* ofGetX11Display());
OF_INTERNAL_DEPRECATED(Window  ofGetX11Window());
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
OF_INTERNAL_DEPRECATED(GLXContext ofGetGLXContext());
#endif

#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
OF_INTERNAL_DEPRECATED(EGLDisplay ofGetEGLDisplay());
OF_INTERNAL_DEPRECATED(EGLContext ofGetEGLContext());
OF_INTERNAL_DEPRECATED(EGLSurface ofGetEGLSurface());
#endif

#if defined(TARGET_OSX)
	void * ofGetNSGLContext();
	void * ofGetCocoaWindow();
#endif

#if defined(TARGET_WIN32)
	HGLRC ofGetWGLContext();
	HWND ofGetWin32Window();
#endif
