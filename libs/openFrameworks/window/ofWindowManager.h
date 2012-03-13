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
	ofWindow* createWindow(int x=0, int y=0, int width=800, int height=600);
	virtual void processEvents()=0;
	virtual void postDraw(){};
	void initializeWindow();
	
	void setupOpenGL(int w, int h, int screenMode);
	void runAppViaInfiniteLoop(ofBaseApp * appPtr);
	void update();
	void draw();
	ofWindow* getLastCreatedWindow();

	void setFrameRate(float targetRate);

	void setActiveWindow(ofWindow* win);

protected:
	void addWindow(ofWindow* win);
	virtual ofWindow* createSystemWindow() = 0;

private:
	ofWindowList windows;
	ofWindow* mainWindow;
	ofWindow* activeWindow;
};


ofWindowManager* ofGetWindowManager();
