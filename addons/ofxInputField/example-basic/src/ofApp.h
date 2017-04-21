#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxInputField.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);		

	void circleResolutionChanged(int & circleResolution);

	bool bHide;

	ofxFloatField radius;
	ofxIntField circleResolution;
	ofxTextField textField;

	ofxColorSlider color;
	ofxVec2Slider center;
	ofxToggle filled;
	ofxButton twoCircles;

	ofxPanel gui;
};

