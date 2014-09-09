/*
 * ofAppAndroidWindow.h
 *
 *  Created on: 24/05/2010
 *      Author: arturo
 */

#pragma once

#include "ofAppBaseWindow.h"

class ofAppAndroidWindow: public ofAppBaseGLESWindow {
public:
	ofAppAndroidWindow();
	virtual ~ofAppAndroidWindow();

	void setupOpenGL(int w, int h, ofWindowMode screenMode);
	void initializeWindow() {}
	void runAppViaInfiniteLoop(ofBaseApp * appPtr);

	void hideCursor() {}
	void showCursor() {}

	void	setWindowPosition(int x, int y) {}
	void	setWindowShape(int w, int h) {}

	ofPoint	getWindowPosition() {return ofPoint(); }
	ofPoint	getWindowSize();
	ofPoint	getScreenSize(){return getWindowSize(); }

	int		getWidth();
	int		getHeight();

	bool	doesHWOrientation(){return true;}

	void	setWindowTitle(string title){}

	ofWindowMode	getWindowMode() {return OF_WINDOW;}

	void	setFullscreen(bool fullscreen);
	void	toggleFullscreen();

	void	enableSetupScreen();
	void	disableSetupScreen();

	void			setOrientation(ofOrientation orientation);
	ofOrientation	getOrientation();

	void	setThreadedEvents(bool threadedEvents);
	void 	setAccumulateTouchEvents(bool accumEvents);
};
