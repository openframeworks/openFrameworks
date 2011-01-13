#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		ofImage img;
		ofxCvHaarFinder finder;
};
