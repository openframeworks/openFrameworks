#pragma once

#include "ofWindow.h"
#include <Cocoa/Cocoa.h>

class ofWindowCocoa: public ofWindow{
public:	
	void initializeWindow();
	void enableContext();
	void processEvents();
	
	NSWindow* getNSWindow();
protected:
	void postDraw();
private:
	NSWindow* nsWindow;
	NSOpenGLView* openGLView;
};
