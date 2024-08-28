#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	
	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased (int key);

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
			
		ofShader shader;
		bool doShader;
		vector<glm::vec3> points;
		vector<ofColor> colors;
		ofEasyCam camera;
		ofMesh mesh;
	
		float baseAngleSpeed = 1.0;
		float baseAngle = 0;
		bool bUseMesh = false;
		bool bUseColors = false;
	
		float xAngleIncrement = 0.1;
		float yAngleIncrement = 0.05;
		float zAngleIncrement = 0.15;
};

