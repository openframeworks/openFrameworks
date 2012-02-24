#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVec2f getField(ofVec2f position);
	
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	
	ofEasyCam cam;
	vector<ofVec2f> points;
	ofMesh cloud;
	float t;
	float width, height;
};
