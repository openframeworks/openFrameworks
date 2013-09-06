#pragma once

#include "ofMain.h"

#define kNumTestNodes 4
#define kNumCameras 2
#define kNumLights 3

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg); 
		
		void reset(); 
		
		ofNode testNodes[kNumTestNodes];
		ofLight light[kNumLights];
		ofCamera cam[kNumCameras];
		int lookatIndex[kNumCameras]; // which test node to lookat (one for each camera)
		int parentIndex[kNumCameras]; // which test node to link (parent) to (one for each camera)
		bool doMouseOrbit[kNumCameras]; // whether camera is in mouseOrbit mode or not
		float orbitRadius;
		
		int camToView; // which camera index are we looking through
		int camToConfigure; // which camera index we are configuring
};
