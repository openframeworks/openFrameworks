#pragma once

#include "ofWindow.h"
#include "ofBaseTypes.h"

typedef ofPtr<ofWindow> ofWindowPtr;
typedef vector< ofWindowPtr > ofWindowList;

class ofWindowManager: public ofAppBaseWindow{
public:
	ofWindowManager();
	~ofWindowManager();
	
	virtual ofWindow* createWindow() = 0;
protected:
	void addWindow(ofWindow* win);
private:
	ofWindowList windows;
};