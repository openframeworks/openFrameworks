#pragma once
#include "ofMain.h"
#include "ofVbo.h"

#define NUM_PTS			100000
#define WORM_LEN		5500
#define	NUM_PARTICLES	500000
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
	
	
	int			vboMode;
	ofVec2f		cameraRotation;	

	// particles
	int			particleCount;
	ofVec2f		particlePos[NUM_PARTICLES];
	ofVec2f		particleVel[NUM_PARTICLES];
	ofFloatColor		particleColor[NUM_PARTICLES];
	float		particleTime[NUM_PARTICLES];
	ofVbo		particleVbo;
	
	// the stars
	ofVbo		ptsVbo;
	ofVec3f		pts[NUM_PTS];
	ofFloatColor		color[NUM_PTS];
	float	    fadeSpeed[NUM_PTS];
	bool		fadeFlip[NUM_PTS];
	
	// the worm
	int			wormDrawMode;
	ofVbo		wormVbo;
	ofVec3f		worm[WORM_LEN];
	ofFloatColor		wormColor[WORM_LEN];
	
};









