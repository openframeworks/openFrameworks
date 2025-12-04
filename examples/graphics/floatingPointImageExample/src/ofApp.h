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
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		// helper functions
		void addFace(ofMesh& mesh, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
		void addFace(ofMesh& mesh, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d);
		glm::vec3 getVertexFromImg(ofFloatImage& img, int x, int y);

		ofFloatImage img;
		ofEasyCam easyCam;
		ofVboMesh mesh;
		ofMaterial material;
		ofLight light;
};
