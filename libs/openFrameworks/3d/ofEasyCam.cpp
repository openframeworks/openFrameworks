/*
 *  ofEasyCam.cpp
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 14/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#include "ofEasyCam.h"


//----------------------------------------
ofEasyCam::ofEasyCam():
targetNode(NULL), 
distance(100),
pos(0, 0),
vel(0, 0),
pmouse(0, 0), 
speed(0.01),
drag(0.01),
oldMousePress(false)
{
}


//----------------------------------------
void ofEasyCam::begin(ofRectangle rect) {
	//	orbit(ofMap(ofGetMouseX(), 0, ofGetWidth(), 180, -180), ofMap(ofGetMouseY(), 0, ofGetHeight(), 90, -90), orbitRadius, testNodes[targetIndex[1]]);
	
	if(ofGetMousePressed(0)) {
		ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());
		if(oldMousePress) {
			vel -= (mousePos - pmouse) * speed;
		}
		pmouse = mousePos;
	}
	
	pos += vel;
	vel *= (1-drag);

	orbit(pos.x, pos.y, distance, (targetNode ? targetNode->getGlobalPosition() : targetPoint));
	
	
	oldMousePress = ofGetMousePressed(0);
	
	ofCamera::begin(rect);
}

//----------------------------------------
void ofEasyCam::reset() {
	pos.set(0, 0);
	vel.set(0, 0);
	distance = 100;
}


//----------------------------------------
void ofEasyCam::setTarget(const ofVec3f& target) {
	targetPoint = target;
	targetNode = NULL;
}

//----------------------------------------
void ofEasyCam::setTarget(ofNode& target) {
	targetNode = &target;
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
