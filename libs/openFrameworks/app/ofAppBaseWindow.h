#pragma once

#include "ofPoint.h"
#include "ofTypes.h"

class ofBaseApp;

class ofAppBaseWindow{

public:

	ofAppBaseWindow(){};
	virtual ~ofAppBaseWindow(){};

	virtual void setupOpenGL(int w, int h, int screenMode) {}
	virtual void initializeWindow() {}
	virtual void runAppViaInfiniteLoop(ofBaseApp * appPtr) {}

	virtual void hideCursor() {}
	virtual void showCursor() {}

	virtual void	setWindowPosition(int x, int y) {}
	virtual void	setWindowShape(int w, int h) {}

	virtual int		getFrameNum() { return 0; }
	virtual	float	getFrameRate() {return 0; }
	virtual double  getLastFrameTime(){ return 0.0; }

	virtual ofPoint	getWindowPosition() {return ofPoint(); }
	virtual ofPoint	getWindowSize(){return ofPoint(); }
	virtual ofPoint	getScreenSize(){return ofPoint(); }

	virtual void			setOrientation(ofOrientation orientation){ }
	virtual ofOrientation	getOrientation(){ return OF_ORIENTATION_DEFAULT; }
	virtual bool	doesHWOrientation(){return false;}

	//this is used by ofGetWidth and now determines the window width based on orientation
	virtual int		getWidth(){ return 0; }
	virtual int		getHeight(){ return 0; }

	virtual void	setFrameRate(float targetRate){}
	virtual void	setWindowTitle(string title){}

	virtual int		getWindowMode() {return 0;}

	virtual void	setFullscreen(bool fullscreen){}
	virtual void	toggleFullscreen(){}

	virtual void	enableSetupScreen(){}
	virtual void	disableSetupScreen(){}
	
};

