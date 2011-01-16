/*
 *  ofCamera.cpp
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 10/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#include "ofCamera.h"
#include "ofMain.h"


ofCamera::ofCamera()
:fov(60),
nearClip(0),
farClip(0),
isOrtho(false),
isActive(false)
{
}

//----------------------------------------
void ofCamera::setFov(float f) {
	fov = f;
}

//----------------------------------------
void ofCamera::setNearClip(float f) {
	nearClip = f;
}

//----------------------------------------
void ofCamera::setFarClip(float f) {
	farClip = f;
}

//----------------------------------------
void ofCamera::enableOrtho() {
	isOrtho = true;
}

//----------------------------------------
void ofCamera::disableOrtho() {
	isOrtho = false;
}

//----------------------------------------
bool ofCamera::getOrtho() const {
	return isOrtho;
}


//----------------------------------------
void ofCamera::begin(ofRectangle rect) {
	if(!isActive) ofPushView();
	isActive = true;
	
	ofSetCoordHandedness(OF_RIGHT_HANDED);
	
	// autocalculate near/far clip planes if not set by user
	float nc, fc;
	if(nearClip == 0 || farClip == 0) {
		float dist = rect.height * 0.5f / tanf(PI * fov / 360.0f);
		nc = (nearClip == 0) ? dist / 100.0f : nearClip;
		fc = (farClip == 0) ? dist * 10.0f : farClip;
	}
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(isOrtho) {
		//			if(vFlip) glOrtho(0, width, height, 0, nearDist, farDist);
		//			else 
		glOrtho(0, rect.width, 0, rect.height, nc, fc);
	} else {
		gluPerspective(fov, rect.width/rect.height, nc, fc);
	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(ofMatrix4x4::getInverseOf(getGlobalTransformMatrix()).getPtr());
	ofViewport(rect.x, rect.y, rect.width, rect.height);
}

// if begin(); pushes first, then we need an end to pop
//----------------------------------------
void ofCamera::end() {
	ofPopView();
	isActive = false;
}