#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofVec2f getField(ofVec2f position);
	
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	
	vector<ofVec2f> points;
	ofMesh cloud;
	float t;
	float width, height;
	
};
