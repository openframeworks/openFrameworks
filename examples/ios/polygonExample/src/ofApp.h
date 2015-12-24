#pragma once

#include "ofxiOS.h"

typedef struct{

	float x;
	float y;
	bool bBeingDragged;
	bool bOver;
	float radius;
	
} draggableVertex;


class ofApp : public ofxiOSApp{
	
    public:
        void setup();
        void update();
        void draw();
        void exit();
    
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);
	
        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);

        int nCurveVertices;
        draggableVertex curveVertices[7];
        draggableVertex bezierVertices[4];

        float appIphoneScale;

};


