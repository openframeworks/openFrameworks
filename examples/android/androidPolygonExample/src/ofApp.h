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


class ofApp : public ofxAndroidApp {

	public:
		void setup();
		void exit();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

		void deviceRefreshRateChanged(int refreshRate);
		void deviceHighestRefreshRateChanged(int refreshRate);
		void deviceRefreshRateChangedEvent(int &refreshRate);
		void deviceHighestRefreshRateChangedEvent(int & refreshRate);


	    int nCurveVertices;
		draggableVertex curveVertices[7];
		draggableVertex bezierVertices[4];

		float appIphoneScale;

};
