/*
 * ofAppEmscriptenWindow.h
 *
 *  Created on: May 8, 2014
 *      Author: arturo
 */

#ifndef OFAPPEMSCRIPTENWINDOW_H_
#define OFAPPEMSCRIPTENWINDOW_H_

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofEvents.h"
#include "EGL/egl.h"
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <webgl/webgl1.h> // For Emscripten WebGL API headers (see also webgl/webgl1_ext.h and webgl/webgl2.h)

class ofxAppEmscriptenWindow: public ofAppBaseGLESWindow {
public:
	ofxAppEmscriptenWindow();
	~ofxAppEmscriptenWindow();

	static bool allowsMultiWindow(){ return false; }
	static bool doesLoop(){ return true; }
	static bool needsPolling(){ return false; }
	static void pollEvents(){}
	static void loop();


	void setup(const ofGLESWindowSettings & settings);

	void hideCursor();
	// void showCursor();

	// void	setWindowPosition(int x, int y);
	void	setWindowShape(int w, int h);

	glm::vec2	getWindowPosition();
	glm::vec2	getWindowSize();
	glm::vec2	getScreenSize();

	
	ofOrientation	getOrientation();
	bool	doesHWOrientation();

	//this is used by ofGetWidth and now determines the window width based on orientation
	int		getWidth();
	int		getHeight();

	ofWindowMode 	getWindowMode();

	void	setFullscreen(bool fullscreen);
	void	toggleFullscreen();

	void	enableSetupScreen();
	void	disableSetupScreen();

	void	setVerticalSync(bool enabled){}
	
	/// \brief enable the first touch to also pass as mouse events.
	/// \param ab enable or
	void 	setTouchSimulatesMouse(bool abEnable);
	/// \brief prevent the browser from using the touch move behavior; call the function to disable the touch and drag that moves the entire web page.
	void 	preventDefaultBrowserTouchMoveBehavior();
	//https://emscripten.org/docs/api_reference/html5.h.html?highlight=emscripten_fullscreen_filtering_default#c.EMSCRIPTEN_FULLSCREEN_SCALE_DEFAULT
	/// \brief set the full screen scale strategy using Emscripten defines;
	/// set to -1 to use default OF strategy. Must apply before entering full screen.
	void 	setFullscreenScaleStrategy(int aStrategy);

	ofCoreEvents & events();
	std::shared_ptr<ofBaseRenderer> & renderer();
	
	void update();
	void draw();

    virtual void makeCurrent();
	virtual void startRender();
	virtual void finishRender();

	bool bIsSetup = false;
private:
	static ofxAppEmscriptenWindow * instance;

	// static int getUniqueId();
	static void display_cb();
	static int keydown_cb(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData);
	static int keyup_cb(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData);
	
	static int mousedown_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
	static int mouseup_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
	static int mousemoved_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
	static int mouseenter_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
	static int mouseleave_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
	
	static int touch_cb(int eventType, const EmscriptenTouchEvent *touchEvent, void *userData);
	
	static int mousescrolled_cb(int eventType, const EmscriptenWheelEvent *wheelEvent, void *userData);
		
	static EM_BOOL emscripten_game_window_resized_callback(int eventType, const void *reserved, void *userData);

	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE  context = 0;

	bool bSetMainLoopTiming = true;
	bool bEnableSetupScreen = true;
	ofCoreEvents _events;
	std::shared_ptr<ofBaseRenderer> _renderer;
	
	unsigned int nFramesSinceWindowResized = 0;
	
	ofWindowMode mStartWindowMode = OF_WINDOW;
	ofWindowMode mCurrentWindowMode = OF_WINDOW;
	ofWindowMode mTargetWindowMode = OF_WINDOW;
	bool bHasResizeListeners = false;
	uint64_t mLastResizeFrameNum = 0;
	
	int mCachedWidth = -1;
	int mCachedHeight = -1;
	
	bool mBTouchSimulatesMouse = true;
	int mFullscreenScaleStrategy = -1;
};

#endif /* OFAPPEMSCRIPTENWINDOW_H_ */

