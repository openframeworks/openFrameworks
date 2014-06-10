#pragma once

#include "ofPoint.h"
#include "ofTypes.h"

#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI)
#include <X11/Xlib.h>
#endif

class ofBaseApp;

class ofAppBaseWindow{

public:

	ofAppBaseWindow(){};
	virtual ~ofAppBaseWindow(){};

	virtual void setupOpenGL(int w, int h, int screenMode) {}
	virtual void initializeWindow() {}
	virtual void runAppViaInfiniteLoop(ofBaseApp * appPtr) {}
	virtual void windowShouldClose(){std::exit(0);}

	virtual void hideCursor() {}
	virtual void showCursor() {}

	virtual void	setWindowPosition(int x, int y) {}
	virtual void	setWindowShape(int w, int h) {}

	virtual ofPoint	getWindowPosition() {return ofPoint(); }
	virtual ofPoint	getWindowSize(){return ofPoint(); }
	virtual ofPoint	getScreenSize(){return ofPoint(); }

	virtual void			setOrientation(ofOrientation orientation){ }
	virtual ofOrientation	getOrientation(){ return OF_ORIENTATION_DEFAULT; }
	virtual bool	doesHWOrientation(){return false;}

	//this is used by ofGetWidth and now determines the window width based on orientation
	virtual int		getWidth(){ return 0; }
	virtual int		getHeight(){ return 0; }

	virtual void	setWindowTitle(string title){}

	virtual int		getWindowMode() {return 0;}

	virtual void	setFullscreen(bool fullscreen){}
	virtual void	toggleFullscreen(){}

	virtual void	enableSetupScreen(){}
	virtual void	disableSetupScreen(){}
	
	virtual void	setVerticalSync(bool enabled){};

#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI)
	virtual Display* getX11Display(){return NULL;}
	virtual Window  getX11Window() {return 0;}
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
	virtual GLXContext getGLXContext(){return 0;}
#endif

#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
	virtual EGLDisplay getEGLDisplay(){return 0;}
	virtual EGLContext getEGLContext(){return 0;}
	virtual EGLSurface getEGLSurface(){return 0;}
#endif

#if defined(TARGET_OSX)
	virtual void * getNSGLContext(){return NULL;}
	virtual void * getCocoaWindow(){return NULL;}
#endif

#if defined(TARGET_WIN32)
	virtual HGLRC getWGLContext(){return 0;}
	virtual HWND getWin32Window(){return 0;}
#endif
};

