#pragma once

#include "ofMain.h"
//#include "GLFW/glfw3.h"

#include "structs.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofPolyline poly;
	ofPath path;
	ofPolyline p1, p2;

	fpsCounter count;
	fpsToggler fpsToggle;
	
	grapher g1, g2;
//	grapher g1 ( ofRectangle(0, 0, ofGetWindowWidth() * .5, ofGetWindowHeight()));
//	grapher g2 ( ofRectangle(ofGetWindowWidth() * .5, 0, ofGetWindowWidth() * .5, ofGetWindowHeight()));

};
