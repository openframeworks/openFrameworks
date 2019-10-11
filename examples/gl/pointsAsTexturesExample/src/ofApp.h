#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
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
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void addPoint(float x, float y, float z);
		
		// vector to store all values
		vector <glm::vec3> points;
		vector <glm::vec3> sizes;
		
		ofVbo vbo;
		ofShader shader;
		ofEasyCam camera;
		
		float camDist;
		
		ofTexture texture;
};
