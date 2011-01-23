/*
 *  ofEasyCam.cpp
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 14/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#include "ofEasyCam.h"
#include "ofEvents.h"
#include "ofMain.h"

//----------------------------------------
ofEasyCam::ofEasyCam():
distance(OF_EASYCAM_DEFAULT_DISTANCE),
mousePosViewPrev(0, 0), 
lastMouseActionFrame(0),
speed(0.1),
drag(0.1),
distanceScaleVelocity(0)
{
	target.setPosition(0, 0, 0);
	setPosition(0, 0, distance);
	setParent(target);
	
	oldMousePress[0]=false;
	oldMousePress[1]=false;
}


//----------------------------------------
void ofEasyCam::begin(ofRectangle rect) {
	
	//check whether we've already interacted
	//with this camera this frame
	if (lastMouseActionFrame != ofGetFrameNum())
	{
		lastMouseActionFrame = ofGetFrameNum();
		
		if(ofGetMousePressed(0) || ofGetMousePressed(2)) {
			ofVec3f targetPos =  target.getGlobalPosition();
			ofVec3f mousePos(ofGetMouseX() - rect.width/2, rect.height/2 - ofGetMouseY(), targetPos.z);
			ofVec3f mousePosView;
			
			float sphereRadius = min(rect.width, rect.height)/2;
			float diffSquared = sphereRadius * sphereRadius - (targetPos - mousePos).lengthSquared();
			if(diffSquared <= 0) {
				mousePos.z = 0;
			} else {
				mousePos.z = sqrtf(diffSquared);
			}
			mousePos.z += targetPos.z;
			mousePosView = ofMatrix4x4::getInverseOf(target.getGlobalTransformMatrix()) * mousePos;
			
			if(oldMousePress[0]) {
				rotation.makeRotate(mousePosViewPrev, mousePosView);
				target.rotate(rotation.conj());
			}
			
			if(oldMousePress[1]) {
				//maths for dolly				
				distanceScaleVelocity = 2 * (mousePos.y - mousePosScreenPrevY) / rect.height;
				setDistance(distance * (1.0f + distanceScaleVelocity));
			}
			
			mousePosViewPrev = ofMatrix4x4::getInverseOf(target.getGlobalTransformMatrix()) * mousePos;
			mousePosScreenPrevY = mousePos.y;
		}
		else
		{
			// no mouse button pressed
			//
			// we keep values from previous frame
			// and damp the values
			//
			float dampFactor = 1.0f - drag;
			rotation.x() *= dampFactor;
			rotation.y() *= dampFactor;
			rotation.z() *= dampFactor;
			rotation.w() = 1;
			
			distanceScaleVelocity = exp(log((distanceScaleVelocity * dampFactor) + 1.0f)) - 1.0f;
			
			//apply transforms if they're big enough
			if (rotation.asVec3().lengthSquared() > 1e-5)
				target.rotate(rotation.conj());
			//
			if (abs(distanceScaleVelocity - 1.0f) > 1e-5)
				setDistance(distance * (1.0f + distanceScaleVelocity));
		}
		
		oldMousePress[0] = ofGetMousePressed(0);
		oldMousePress[1] = ofGetMousePressed(2);
		
		//reset view on press key 'r'
		if (ofGetKeyPressed('r'))
			reset();
		
	}

	
	ofCamera::begin(rect);
}

//----------------------------------------
void ofEasyCam::reset() {
	target.resetTransform();
	distance = OF_EASYCAM_DEFAULT_DISTANCE;
}


//----------------------------------------
void ofEasyCam::setTarget(const ofVec3f& targetPoint) {
	target.setPosition(targetPoint);
}

//----------------------------------------
void ofEasyCam::setTarget(ofNode& targetNode) {
	target.setPosition(ofVec3f(0, 0, 0));
	target.setParent(targetNode);
}

//----------------------------------------
ofNode& ofEasyCam::getTarget() {
	return target;
}


//----------------------------------------
void ofEasyCam::setDistance(float f) {
	if (f > 0.0f)
	{
		distance = f;
		setPosition(0, 0, f);
	}
}

//----------------------------------------
float ofEasyCam::getDistance() const {
	return distance;
}


//----------------------------------------
void ofEasyCam::setSpeed(float s) {
	speed = s;
}


//----------------------------------------
float ofEasyCam::getSpeed() const {
	return speed;
}

//----------------------------------------
void ofEasyCam::setDrag(float f) {
	drag = f;
}


//----------------------------------------
float ofEasyCam::getDrag() const {
	return drag;
}