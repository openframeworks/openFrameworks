#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#define NUM_PTS 3000

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
	
	// for moving the world aroudn
	ofVec2f finger;
	ofVec3f	rot, vel;
	float	zoom;
	
	// the vbo and texture 
	ofVbo	vbo;
	ofVec3f pts[NUM_PTS];
	ofImage texture;
};


