#ifndef OFWINDOWMANAGERX11_H
#define OFWINDOWMANAGERX11_H

#include "ofWindowManager.h" // Base class: ofWindowManager

class ofWindowManagerX11 : public ofWindowManager {

public:
	ofWindowManagerX11();
	~ofWindowManagerX11();

	ofWindow* createSystemWindow();
	void init();
	void processEvents();
	void postDraw();
};

#endif // OFWINDOWMANAGERX11_H
