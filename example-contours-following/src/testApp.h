#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class Glow : public ofxCv::RectFollower {
protected:
	ofColor color;
	ofVec2f cur, smooth;
	float startedDying;
	ofPolyline all;
public:
	Glow()
		:startedDying(0) {
	}
	void setup(const cv::Rect& track);
	void update(const cv::Rect& track);
	void kill();
	void draw();
};

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVideoPlayer movie;
	ofxCv::ContourFinder contourFinder;
	ofxCv::RectTrackerFollower<Glow> tracker;
};
