#ifndef OFWINDOWX11_H
#define OFWINDOWX11_H

#include "ofWindow.h" // Base class: ofWindow
#include "X11/X.h"

class ofWindowX11 : public ofWindow {

public:
	ofWindowX11();
	~ofWindowX11();
	virtual void enableContext();
	void initializeWindow();
	void processEvents();
	void postDraw();

private:
	Window window;
	GLXContext glxContext;
	Display* display;
};

#endif // OFWINDOWX11_H
