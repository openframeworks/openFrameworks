#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void reset();
	
	ofImage img;
	int maxSize;
	int generation;
	ofImageQualityType quality;
	int addX, addY, subX, subY;
	
	void mousePressed(int x, int y, int button);
};
