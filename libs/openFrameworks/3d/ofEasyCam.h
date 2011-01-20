/*
 *  ofEasyCam.h
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 14/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#pragma once

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
	
	void setSpeed(float f);
	float getSpeed() const;
	
	void setDrag(float f);
	float getDrag() const;

	
protected:
	ofNode target;
	float distance;

	ofVec3f mousePosViewPrev;
	bool oldMousePress;
	int lastMouseActionFrame;

	float speed;
	float drag;

	ofVec3f rotAxis;
	float angle;
};