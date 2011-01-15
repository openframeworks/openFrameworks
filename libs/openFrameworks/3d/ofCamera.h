/*
 *  ofCamera.h
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 10/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

// TODO: 
// add off-axis projection options (eventually tile render support)
// fix ortho projection
// add toWorld, toScreen methods
// add get/set projection matrix
// support for left handed or right handed?

#pragma once


#include "ofNode.h"
#include "ofRectangle.h"
#include "ofAppRunner.h"


// Use the public API of ofNode for all transformations
class ofCamera : public ofNode {
public:
	ofCamera();
	
	// projection properties:
	void setFov(float f);
	void setNearClip(float f);
	void setFarClip(float f);
	
	void enableOrtho();
	void disableOrtho();
	bool getOrtho() const;
	
	// set the matrices
	virtual void begin(ofRectangle rect = ofGetWindowRect());
	virtual void end();
	
	// for hardcore peeps, access to the projection matrix()
	//	ofMatrix4x4 &getProjectionMatrix();	
	
protected:
	bool isOrtho;
	float fov;
	float nearClip;
	float farClip;
	bool isActive;
};

