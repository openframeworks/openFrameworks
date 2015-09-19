#pragma once

#include "ofMain.h"

#define MAX_N_PTS         1500

class ofApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		ofVec3f	pts[MAX_N_PTS];
		int		nPts;

		ofVec3f	rotateAmount;  // amount to rotate in x,y,z;
		float	speedOfRotation; // speed;

		// a grid helpful for seeing the rotation
		ofVec3f	xAxisMin;
		ofVec3f	xAxisMax;
		ofVec3f	yAxisMin;
		ofVec3f	yAxisMax;
		ofVec3f	zAxisMin;
		ofVec3f	zAxisMax;

		bool bDrawnAnything;

};

