#pragma once

#include "ofMain.h"
#include "ofVectorMath.h"
#include "ofGraphics.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ofImage ofLogo;

	void keyPressed(int key);
};
