#pragma once
#include "ofMain.h"
#include "ofxAssimpModel.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void renderScene();
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
	
	ofEasyCam cam;
	ofLight	light;
	ofx::assimp::Model wizardModel;
	
	ofx::assimp::Model logoModel;
	shared_ptr<ofx::assimp::Model> srcFoxModel;
	vector< shared_ptr<ofx::assimp::Model> > foxes;
	
	shared_ptr<ofMaterial> logoMaterial;
	
	ofxPanel gui;
	ofParameter<glm::vec3> wizardPos;
	ofParameter<glm::vec3> wizardHandOffset;
	ofVboMesh groundMesh;
	ofMaterial groundMaterial;
	
	ofCubeMap cubeMap;
	ofParameter<float> cubeMapExposure;
	ofParameter<bool> mBDebug;
	
	ofParameterGroup mShadowParams;
	ofParameter<float> mShadowStrength;
	
	float mTimeSinceStaffHitGround = 100.0f;
	bool bFiredHitGroundEvent = false;
	
	bool bDrawGui = false;
};
