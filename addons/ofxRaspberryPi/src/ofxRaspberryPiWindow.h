/*==============================================================================
 
 Copyright (c) 2011, 2012 Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ==============================================================================*/

#pragma once

#include "ofAppBaseWindow.h"

class ofAppAndroidWindow: public ofAppBaseWindow {
public:
	ofAppAndroidWindow();
	virtual ~ofAppAndroidWindow();

	void setupOpenGL(int w, int h, int screenMode) {}
	void initializeWindow() {}
	void runAppViaInfiniteLoop(ofBaseApp * appPtr);

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

	void	setThreadedEvents(bool threadedEvents);

};