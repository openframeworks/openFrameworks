#pragma once

#define OF_EASYCAM_DEFAULT_DISTANCE 100.0f

#include "ofCamera.h"

class ofEasyCam : public ofCamera {
public:
	ofEasyCam();
	
	virtual void begin(ofRectangle rect = ofGetWindowRect());
	void reset();

	//----------------------------------------
	// advanced functions
	
	void setTarget(const ofVec3f& target);
	void setTarget(ofNode& target);
	ofNode& getTarget();
	
	void setDistance(float f);
	float getDistance() const;
	
	void setDrag(float f);
	float getDrag() const;
	
	void setThrust(float f);
	float getThrust() const;
	

	
protected:
	ofNode target;
	float distance;

	ofVec3f mousePosViewPrev;
	ofVec3f mousePosScreenPrev;
	bool oldMousePress[2];
	int lastMouseActionFrame;

	float drag;
	float thrust;

	//momentum
	ofQuaternion rotation;
	float distanceScaleVelocity;
};