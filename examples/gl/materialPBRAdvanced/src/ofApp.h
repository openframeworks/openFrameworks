#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
	
		void renderScene();
	
		bool reloadShader();

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
	
		ofEasyCam camera;
		ofCubeMap cubeMap;
		ofVboMesh meshLogoHollow;
		ofMaterial matLogo;
		ofMaterial matFloor;
		ofMaterial matPlywood;
		ofMaterial matSphere;
	
		ofVboMesh meshPlySphere;
	
		int mode = 0;
	
		ofLight light;
		bool bDebug = false;
		
};
