#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"


class testApp : public ofSimpleApp{

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	void setupArduino();
	void updateArduino();

	ofImage				bgImage;
	ofTrueTypeFont		font;
	ofArduino	ard;
	bool		bSetupArduino;			// flag variable for setting up arduino once

};

#endif

