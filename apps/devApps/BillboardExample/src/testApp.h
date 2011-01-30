#pragma once
#include "ofMain.h"
#include "ofVbo.h"

#define	NUM_BILLBOARDS	10000

class testApp : public ofBaseApp {
	
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
	void windowResized(int w, int h);
	
	ofImage		background;
	ofVec3f		cameraRotation;
	
	
	// billboard particles
	float		billboardSize[NUM_BILLBOARDS];
	ofShader	billboardShader;
	ofImage		texture;
	ofVbo		billboardVbo;
	ofVec3f		billboardVerts[NUM_BILLBOARDS];
	ofVec3f		billboardVels[NUM_BILLBOARDS];
	ofColor		billboardColor[NUM_BILLBOARDS];
	ofVec3f		cubePos[10];
	
};









