#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	
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
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void gotMessage(ofMessage msg);
	void dragEvent(ofDragInfo dragInfo);
	
	void loadLUT(string path);
	void applyLUT(ofPixelsRef pix);
	
	bool doLUT;
	ofVideoGrabber 		vidGrabber;
	int dirLoadIndex;
	ofDirectory dir;
	glm::vec3 lutPos;
	glm::vec3 thumbPos;
	
	bool LUTloaded;
	glm::vec3 lut[32][32][32];
	
	ofImage lutImg;
	
	
	
};
