#pragma once

#include "ofCamera.h"

class ofEasyCam : public ofCamera {
public:
	ofEasyCam();

	// TODO: this should be ofGetViewRect() eventually
	virtual void begin(ofRectangle rect = ofGetWindowRect());
	void reset();

	//----------------------------------------
	// advanced functions

	void setTarget(const ofVec3f& target);
	void setTarget(ofNode& target);
	ofNode& getTarget();

	void setDistance(float distance);
	float getDistance() const;

	// drag is how quickly the camera picks up and slows down
	// it is a normalized value between 0-1
	void setDrag(float drag);
	float getDrag() const;

protected:
	ofNode target;

	ofVec3f mousePosViewPrev;
	ofVec3f mousePosScreenPrev;
	bool lastMousePressed[2];
	int lastFrame;

	//momentum
	float drag;
	ofQuaternion rotation;
	float distanceScaleVelocity;
};