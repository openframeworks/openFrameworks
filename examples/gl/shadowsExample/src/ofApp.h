#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	public:
		
		void setup();
		void update();
		void draw();
		
		void renderScene();

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
	
		vector< shared_ptr<ofLight> > lights;
		
		ofEasyCam camera;
		ofMesh logoMesh;
		ofMesh sphereMesh;
		ofMesh boxMesh;
		ofMesh cylinderMesh;
		
		ofMaterial logoMaterial;
		ofMaterial boxMaterial;
		ofMaterial bgMaterial;
		ofMaterial sphereMaterial;
		
		ofColor lightColor;
		float colorHue = 0.0f;
	
		bool bDrawFrustums = false;
		bool bEnableShadows = true;
	
		ofShadowType shadowType;
		float shadowSampleRadius = 2.;
	
};
