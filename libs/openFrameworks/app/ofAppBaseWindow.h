#pragma once

#include "ofWindowSettings.h"
// MARK: Target
#include "ofConstants.h"
#include "ofRectangle.h"

class ofBaseApp;
class ofBaseRenderer;
class ofCoreEvents;

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
struct __GLXcontextRec;
typedef __GLXcontextRec * GLXContext;
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY)
typedef unsigned long Window;
struct _XDisplay;
typedef struct _XDisplay Display;
#endif

class ofAppBaseWindow {
public:

	ofAppBaseWindow(){}
//	virtual ~ofAppBaseWindow(){}
	~ofAppBaseWindow(){}

	virtual void setup(const ofWindowSettings & settings)=0;
	virtual void update()=0;
	virtual void draw()=0;
	virtual std::shared_ptr<ofBaseRenderer> & renderer() = 0;
	virtual ofCoreEvents & events() = 0;

	virtual bool getWindowShouldClose(){
		return false;
	}
	virtual void setWindowShouldClose(){}
	virtual void close(){}

	virtual void hideCursor(){}
	virtual void showCursor(){}

	virtual void setWindowPosition(int x, int y){}
	virtual void setWindowShape(int w, int h){}
	virtual void setWindowRect(const ofRectangle & rect){}

	virtual glm::ivec2 getWindowPosition() { return glm::ivec2(); }
	virtual glm::ivec2 getWindowSize() { return glm::ivec2(); }
	virtual glm::ivec2 getFramebufferSize() { return glm::ivec2(); }
	virtual glm::ivec2 getScreenSize() { return glm::ivec2(); }
	virtual ofRectangle getWindowRect() { return ofRectangle(); }

	virtual void setOrientation(ofOrientation orientationIn) { orientation = orientationIn; }
	virtual ofOrientation getOrientation() { return orientation; }
	virtual bool doesHWOrientation(){ return false; }

	//this is used by ofGetWidth and now determines the window width based on orientation
	virtual int getWidth(){ return 0; }
	virtual int getHeight(){ return 0; }

	virtual void setWindowTitle(const std::string & title){}

	virtual ofWindowMode getWindowMode() { return settings.windowMode; }

	virtual void setFullscreen(bool fullscreen){}
	virtual void toggleFullscreen(){}
	virtual void setWindowMousePassthrough(bool allowPassthrough){}

	virtual void enableSetupScreen(){}
	virtual void disableSetupScreen(){}

	virtual void setVerticalSync(bool enabled){}
	virtual void setClipboardString(const std::string& text){}
	virtual std::string getClipboardString(){ return ""; }

	virtual void makeCurrent(){}
	virtual void swapBuffers(){}
	virtual void startRender(){}
	virtual void finishRender(){}

	virtual void * getWindowContext(){ return nullptr; }
	
	virtual void beginDraw(){}
	virtual void endDraw(){}

#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY)
	virtual Display* getX11Display(){ return nullptr; }
	virtual Window  getX11Window(){ return 0; }
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
	virtual GLXContext getGLXContext(){ return 0; }
#endif

#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
	virtual EGLDisplay getEGLDisplay(){ return 0; }
	virtual EGLContext getEGLContext(){ return 0; }
	virtual EGLSurface getEGLSurface(){ return 0; }
#endif

#if defined(TARGET_OSX)
	virtual void * getNSGLContext(){ return nullptr; }
	virtual void * getCocoaWindow(){ return nullptr; }
#endif

#if defined(TARGET_WIN32)
	virtual HGLRC getWGLContext(){ return 0; }
	virtual HWND getWin32Window(){ return 0; }
#endif
	
	ofWindowSettings settings;

//private:
	ofOrientation orientation = OF_ORIENTATION_DEFAULT;
	ofWindowMode windowMode = OF_WINDOW;
	
	std::string name { "virgem" };
};
