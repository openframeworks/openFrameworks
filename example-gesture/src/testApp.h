#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void updateGestures();
	
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	
	ofPolyline polyline;
	cv::RotatedRect ellipse, rect;
	ofVec2f linePoint, lineDirection;
};
