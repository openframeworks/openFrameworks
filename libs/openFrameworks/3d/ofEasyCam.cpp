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
//	
//	ab_quat[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
//	ab_last[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
//	ab_next[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
//	
//	// the distance from the origin to the eye
//	ab_zoom = 1.0;
//	ab_zoom2 = 1.0;
//	// the radius of the arcball
//	ab_sphere = 1.0;
//	ab_sphere2 = 1.0;
//	// the distance from the origin of the plane that intersects
//	// the edge of the visible sphere (tangent to a ray from the eye)
//	ab_edge = 1.0;
//	// whether we are using a sphere or plane
//	ab_planar = false;
//	 ab_planedist = 0.5;
//	
//	vec ab_start = vec(0,0,1);
//	vec ab_curr = vec(0,0,1);
//	vec ab_eye = vec(0,0,1);
//	vec ab_eyedir = vec(0,0,1);
//	vec ab_up = vec(0,1,0);
//	vec ab_out = vec(1,0,0);
//	
//	GLdouble ab_glp[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
//	GLdouble ab_glm[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
//	int ab_glv[4] = {0,0,640,480};
	
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
