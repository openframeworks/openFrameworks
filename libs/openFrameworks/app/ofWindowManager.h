#pragma once

#include "ofWindow.h"
#include "ofBaseTypes.h"

typedef ofPtr<ofWindow> ofWindowPtr;
typedef vector< ofWindowPtr > ofWindowList;

class ofWindowManager: public ofAppBaseWindow{
public:
	ofWindowManager();
	~ofWindowManager();
	
	virtual void init() = 0;
	virtual ofWindow* createWindow() = 0;
	virtual void processEvents()=0;
	
	void setupOpenGL(int w, int h, int screenMode);
	void runAppViaInfiniteLoop(ofBaseApp * appPtr);
	void update();
	
protected:
	void addWindow(ofWindow* win);
private:
	ofWindowList windows;
};


ofWindowManager* ofGetWindowManager();