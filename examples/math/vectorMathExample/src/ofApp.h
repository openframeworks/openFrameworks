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

		glm::vec3 pts[MAX_N_PTS];
		int nPts;

		glm::vec3 rotateAmount;  // amount to rotate in x,y,z;
		float speedOfRotation; // speed;

		// a grid helpful for seeing the rotation
		glm::vec3 xAxisMin;
		glm::vec3 xAxisMax;
		glm::vec3 yAxisMin;
		glm::vec3 yAxisMax;
		glm::vec3 zAxisMin;
		glm::vec3 zAxisMax;

		bool bDrawnAnything;

};

