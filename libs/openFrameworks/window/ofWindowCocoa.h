#pragma once

#include "ofWindow.h"
#include <Cocoa/Cocoa.h>

class ofWindowCocoa: public ofWindow{
public:	
	void initializeWindow();
	void enableContext();
	void processEvents();
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);
	
	NSWindow* getNSWindow();
protected:
	void postDraw();
private:
	NSWindow* nsWindow;
	NSOpenGLView* openGLView;
};
