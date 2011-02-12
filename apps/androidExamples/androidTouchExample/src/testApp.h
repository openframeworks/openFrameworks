#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofxAccelerometer.h"
#include "Ball.h"

#define NUM_POINTS				10

class testApp : public ofxAndroidApp {
	
public:
	void setup();
	void update();
	void draw();
	void exit();

	void touchDown(int x, int y, int id);
	void touchMoved(int x, int y, int id);
	void touchUp(int x, int y, int id);
	
	
	Ball balls[NUM_POINTS];
};
