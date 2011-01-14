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

#include "ofNode3d.h"
#include "ofMain.h"

class ofCamera : public ofNode3d {
public:
	ofCamera():fov(60), nearClip(0), farClip(0), isOrtho(false) {}
	
	// projection properties:
	void setFov(float f);
	void setNearClip(float f);
	void setFarClip(float f);
	
	void enableOrtho();
	void disableOrtho();
	bool getOrtho() const;
	
	// set the matrices
	// not sure if this should push first, or if that should be the users responsibility
	void begin(ofRectangle rect = ofGetWindowRect());
	
	// if begin(); pushes first, then we need an end to pop
	void end();
	
	// for hardcore peeps, access to the generated matrix()
	//	ofMatrix4x4 &getProjectionMatrix();	
	
protected:
	bool isOrtho;
	float fov;
	float nearClip;
	float farClip;
};

