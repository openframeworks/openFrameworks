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

	// drag is how quickly the camera slows down
	// it is a normalized value between 0-1
	void setDrag(float drag);
	float getDrag() const;

	// thrust is how quickly the camera picks up
	// it is a normalized value between 0-1
	void setThrust(float thrust);
	float getThrust() const;

protected:
	ofNode target;

	ofVec3f mousePosViewPrev;
	ofVec3f mousePosScreenPrev;
	bool lastMousePressed[2];
	int lastFrame;

	//momentum
	float drag;
	float thrust;
	ofQuaternion rotation;
	float distanceScaleVelocity;
};