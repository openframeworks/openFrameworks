/*
 * ofAppAndroidWindow.h
 *
 *  Created on: 24/05/2010
 *      Author: arturo
 */

#ifndef OFAPPANDROIDWINDOW_H_
#define OFAPPANDROIDWINDOW_H_

#include "ofAppBaseWindow.h"

class ofAppAndroidWindow: public ofAppBaseWindow {
public:
	ofAppAndroidWindow();
	virtual ~ofAppAndroidWindow();

	void setupOpenGL(int w, int h, int screenMode) {}
	void initializeWindow() {}
	void runAppViaInfiniteLoop(ofPtr<ofBaseApp> appPtr);

	void hideCursor() {}
	void showCursor() {}

	void	setWindowPosition(int x, int y) {}
	void	setWindowShape(int w, int h) {}

	int		getFrameNum();
	float	getFrameRate();
	double  getLastFrameTime();

	ofPoint	getWindowPosition() {return ofPoint(); }
	ofPoint	getWindowSize();
	ofPoint	getScreenSize(){return getWindowSize(); }

	int		getWidth();
	int		getHeight();

	bool	doesHWOrientation(){return true;}

	void	setFrameRate(float targetRate);
	void	setWindowTitle(string title){}

	int		getWindowMode() {return 0;}

	void	setFullscreen(bool fullscreen);
	void	toggleFullscreen();

	void	enableSetupScreen();
	void	disableSetupScreen();

	void			setOrientation(ofOrientation orientation);
	ofOrientation	getOrientation();

};

#endif /* OFAPPANDROIDWINDOW_H_ */
