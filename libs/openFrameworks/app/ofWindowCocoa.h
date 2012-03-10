#pragma once

#include "ofWindow.h"
#include <Cocoa/Cocoa.h>

class ofWindowCocoa: public ofWindow{
public:
      ofWindowCocoa();
      ~ofWindowCocoa();
	
	bool open();
	void enableContext();
protected:
private:
	NSWindow* nsWindow;
	NSOpenGLView* openGLView;
};
