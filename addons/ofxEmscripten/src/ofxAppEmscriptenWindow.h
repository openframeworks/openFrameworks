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
	void showCursor();

	void	setWindowPosition(int x, int y);
	void	setWindowShape(int w, int h);

	ofPoint	getWindowPosition();
	ofPoint	getWindowSize();
	ofPoint	getScreenSize();

	void			setOrientation(ofOrientation orientation);
	ofOrientation	getOrientation();
	bool	doesHWOrientation();

	//this is used by ofGetWidth and now determines the window width based on orientation
	int		getWidth();
	int		getHeight();

	void	setWindowTitle(string title);

	ofWindowMode 	getWindowMode();

	void	setFullscreen(bool fullscreen);
	void	toggleFullscreen();

	void	enableSetupScreen();
	void	disableSetupScreen();

	void	setVerticalSync(bool enabled);

	EGLDisplay getEGLDisplay();
	EGLContext getEGLContext();
	EGLSurface getEGLSurface();

	ofCoreEvents & events();
	shared_ptr<ofBaseRenderer> & renderer();


private:
	static void display_cb();
	static int keydown_cb(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData);
	static int keyup_cb(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData);
	static int mousedown_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
	static int mouseup_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
	static int mousemoved_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
	void update();
	void draw();
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
    static ofxAppEmscriptenWindow * instance;
    bool bEnableSetupScreen;
    ofCoreEvents _events;
    shared_ptr<ofBaseRenderer> _renderer;
};

#endif /* OFAPPEMSCRIPTENWINDOW_H_ */
