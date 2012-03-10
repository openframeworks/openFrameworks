#ifndef OFWINDOWMANAGERX11_H
#define OFWINDOWMANAGERX11_H

#include "ofWindowManager.h" // Base class: ofWindowManager

class ofWindowManagerX11 : public ofWindowManager {

public:
	ofWindowManagerX11();
	~ofWindowManagerX11();

	ofWindow* createWindow();
	void init();
	void processEvents();
};

#endif // OFWINDOWMANAGERX11_H
