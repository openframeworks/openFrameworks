#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	void loadLUT(string path);
	void applyLUT(ofPixelsRef pix);
	
	bool doLUT;
	ofVideoGrabber 		vidGrabber;
	int dirLoadIndex;
	ofDirectory dir;
	ofPoint lutPos;
	ofPoint thumbPos;
	
	bool LUTloaded;
	ofVec3f lut[32][32][32];
	
	ofImage lutImg;
	
	
	
};
