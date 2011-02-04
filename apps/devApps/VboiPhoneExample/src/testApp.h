#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#define GRID_WIDTH  16
#define GRID_HEIGHT 16
#define LENGTH		2 * 8

class testApp : public ofxiPhoneApp {
	
public:
	void setup();
	void update();
	void draw();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	void touchCancelled(ofTouchEventArgs &touch);
	
	ofVbo	vbo;
	ofVec3f pos[GRID_WIDTH*GRID_HEIGHT*LENGTH];
	ofVec3f center;

	float   restLength;
	int		total;
	int		space;
	int		count;
	bool	bPause;
	int		zoom, zoomTarget;
};


