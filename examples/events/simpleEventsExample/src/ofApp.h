#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "Circle.h"

#define TOTAL_GRAY_CIRCLES 10

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
    void onMouseInCircle(ofVec2f & e);
    void onMouseInAnyCircle(ofVec2f & e);
    Circle redCircle;
    Circle grayCircles [TOTAL_GRAY_CIRCLES];
    ofColor bg;
    ofVec2f clickedPoint;
    
};
