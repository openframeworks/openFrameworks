#pragma once

#include "ofWindow.h"
#include <Cocoa/Cocoa.h>

class ofWindowCocoa: public ofWindow{
public:
      ofWindowCocoa();
      ~ofWindowCocoa();
	
	void initializeWindow();
	void enableContext();
	void processEvents();
protected:
	void postDraw();
private:
	NSWindow* nsWindow;
	NSOpenGLView* openGLView;
};
