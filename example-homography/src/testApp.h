#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	bool movePoint(vector<ofVec2f>& points, ofVec2f point);
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void keyPressed(int key);
	
	ofImage left, right, warpedColor;
	vector<ofVec2f> leftPoints, rightPoints;
	bool movingPoint;
	ofVec2f* curPoint;
	bool saveMatrix;
	bool homographyReady;
	
	cv::Mat homography;
};
