#pragma once

#include "ofMain.h"

class windowRenderer: public ofWindowListener {
	void draw() {
		ofDrawBitmapString("THIS IS A SECONDARY WINDOW", 30, 30);

		ofFill();
		ofSetColor(160);
		ofRect(ofGetFrameNum()%ofGetWidth(), 0, 2, ofGetHeight());
	}
};

class testApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void scrolled(float dx, float dy);

	ofVec2f textPos;
};
