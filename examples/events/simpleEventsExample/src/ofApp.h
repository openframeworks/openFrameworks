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
		void onMouseInCircle(glm::vec2 & e);
		void onMouseInAnyCircle(glm::vec2 & e);
		Circle redCircle;
		Circle grayCircles [TOTAL_GRAY_CIRCLES];
		ofColor bg;
		glm::vec2 clickedPoint;
		
};
