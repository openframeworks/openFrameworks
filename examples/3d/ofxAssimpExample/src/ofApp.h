#pragma once

#include "ofMain.h"
#include "ofxAssimpModel.h"
#include "ofVboMesh.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
	
		void loadModel(int aindex);
        void loadModel(string filename);
	
		void renderScene();

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

		ofx::assimp::Model model;

		bool bDrawWireframe = false;
		bool bDrawMeshes = true;
		bool bDrawBounds = true;
		
		ofLight	light;
        ofEasyCam cam;
		ofCamera camOnSecurityCam;
		ofCubeMap cubeMap;
	
		ofRectangle mTextRect, mViewRect;
		std::string mSceneString;
	
		glm::quat mLocalQuat;
	
		ofxPanel gui;
		
		ofParameter<float> shadowStrength;
		ofParameter<float> panDeg,tiltDeg;
		ofParameter<float> cameraOffsetZ, bottomOffsetY,bottomOffsetZ;
		ofParameter<bool> bDebug;
		ofParameter<float> shadowBias, shadowNormalBias;
		
		ofVboMesh wallMesh;
		ofMaterial wallMaterial;
		
		float panDirection = 0.f;
		float tiltDirection = 0.f;
		
};

