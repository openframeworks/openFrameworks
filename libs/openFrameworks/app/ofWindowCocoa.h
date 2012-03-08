#pragma once

#include "ofWindow.h"
#include <Cocoa/Cocoa.h>

class ofWindowCocoa: public ofWindow{
public:
      ofWindowCocoa();
      ~ofWindowCocoa();
	
	bool open();
protected:
private:
	NSWindow* windowPtr;
};
