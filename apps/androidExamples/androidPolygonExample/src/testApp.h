#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

typedef struct {

	float 	x;
	float 	y;
	bool 	bBeingDragged;
	bool 	bOver;
	float 	radius;
	
}	draggableVertex;


class testApp : public ofxAndroidApp {
	
public:
	void setup();
	void update();
	void draw();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	void touchCancelled(ofTouchEventArgs &touch);

	int nCurveVertexes;
	draggableVertex curveVertices[7];
	draggableVertex bezierVertices[4];

	float appIphoneScale;

};


