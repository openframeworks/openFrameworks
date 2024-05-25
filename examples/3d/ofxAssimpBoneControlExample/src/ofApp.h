#pragma once

#include "ofMain.h"
#include "ofxAssimpModel.h"
#include "ofVboMesh.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
	
		void loadModel(int aindex);
        void loadModel(string filename);

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

		bool bHelpText = false;
		bool bDrawBones = false;
		bool bDrawWireframe = false;
		bool bDrawMeshes = true;
		bool bFollowTarget = true;
		bool bAutoRotate = true;
        
		ofMesh mesh;
		ofLight	light;
        ofEasyCam cam;
	
		ofRectangle mTextRect, mViewRect, mTimelineRect;
		std::string mSceneString;
	
		glm::quat mLocalQuat;
		float lookStrength = 0.f;
		glm::vec3 mLookAtPos = {0.f, 0.f, 0.f};
		
};

