#pragma once

#include "ofMain.h"
#include "ofxGui.h"

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
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);		

	void circleResolutionChanged(int & circleResolution);
	void ringButtonPressed();

	bool bHide;

	ofParameter<float> radius;
	ofParameter<ofColor> color;
	ofParameter<ofVec2f> center;
	ofParameter<int> circleResolution;
	ofParameter<bool> filled;
	ofxButton twoCircles;
	ofxButton ringButton;
	ofParameter<string> screenSize;

	ofxPanel gui;

	ofSoundPlayer ring;
};

