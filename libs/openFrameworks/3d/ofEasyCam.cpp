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
distance(100),
//vel(0, 0),
mousePrev(0, 0), 
oldMousePress(false),
speed(0.1),
drag(0.1)
{
	target.setPosition(0, 0, 0);
	setPosition(0, 0, distance);
	setParent(target);
}


//----------------------------------------
void ofEasyCam::begin(ofRectangle rect) {
	if(ofGetMousePressed(0)) {
		ofVec3f targetPos =  target.getGlobalPosition();
		ofVec3f mousePos(ofGetMouseX() - rect.width/2, rect.height/2 - ofGetMouseY(), targetPos.z);
		
		float sphereRadius = min(rect.width, rect.height)/2;
		float diffSquared = sphereRadius * sphereRadius - (targetPos - mousePos).lengthSquared();
		if(diffSquared <= 0) {
			mousePos.z = 0;
		} else {
			mousePos.z = sqrtf(diffSquared);
		}
		mousePos.z += targetPos.z;
		mousePos = ofMatrix4x4::getInverseOf(target.getGlobalTransformMatrix()) * mousePos;
		
		if(oldMousePress) {
			ofQuaternion rotAmount;
			rotAmount.makeRotate(mousePrev, mousePos);
			target.rotate(rotAmount.conj());
			//			ofMatrix4x4	m;
			//			m.makeRotationMatrix(mousePrev, mousePos);
			//			target.rotate(m.getRotate());
			//			target.rotate(ofMatrix4x4::getInverseOf(m).getRotate());
		}
		
		
		printf("mousePos: %f %f %f\n", mousePos.x, mousePos.y, mousePos.z);
		
		if(oldMousePress) {
//			vel -= (mousePos - mousePrev) * speed;
		}
		mousePrev = mousePos;
	}
	
	//	pos += vel;
	//	vel *= (1-drag);
	//	pos.y = ofClamp(pos.y, -89, 89);
	//	orbit(pos.x, pos.y, distance, (targetNode ? targetNode->getGlobalPosition() : targetOffset));
	
	
	oldMousePress = ofGetMousePressed(0);
	
	ofCamera::begin(rect);
}

//----------------------------------------
void ofEasyCam::reset() {
	//	pos.set(0, 0);
	target.resetTransform();
//	vel.set(0, 0);
	distance = 100;
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
	distance = f;
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
