/**
 *
 * James George 
 * Quaternion Examples
 * 
 * A set of examples illustrating useful places to use quaternions
 *
 * ArcBall:
 * This example shows how to rotate a cube by dragging it like you'd imagine it to work physically
 */

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
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
		
	//current state of the rotation  
    ofQuaternion curRot;  
	
    //a place to store the mouse position so we can measure incremental change  
    ofVec2f lastMouse;
	
	//slows down the rotation 1 = 1 degree per pixel
	float dampen;
};
