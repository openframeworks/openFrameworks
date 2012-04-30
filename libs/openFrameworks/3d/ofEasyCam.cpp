#include "ofEasyCam.h"
#include "ofMath.h"
#include "ofUtils.h"

// when an ofEasyCam is moving due to momentum, this keeps it
// from moving forever by assuming small values are zero.
float minDifference = 0.1e-5;

// this is the default on windows os
unsigned long doubleclickTime = 200;

//----------------------------------------
ofEasyCam::ofEasyCam(){
	drag = 0.9f;
	bMouseInputEnabled = false;
	lastTap	= 0;
	bDistanceSet = false; 
	lastDistance = 0;
	
	reset();
	
	//bInsideArcball=true;
	sensitivity = .2;
	sensitivityZ= .7;
	enableMouseInput();	
	doRotate = false;
	bApplyInertia =false;
	bDoTranslate = false;
	doTranslationKey = 'm';
	
}

//----------------------------------------
ofEasyCam::~ofEasyCam(){
}
//----------------------------------------
void ofEasyCam::begin(ofRectangle viewport){
	if(bMouseInputEnabled){
		if(!bDistanceSet){
			setDistance(getImagePlaneDistance(viewport), true);
		}
	}	
	if (doRotate) {
		updateRotation();
	}else if (bDoTranslate) {
		updateTranslation(); 
	}
	
	ofCamera::begin(viewport);
	
	target.draw();
	
}

//----------------------------------------
void ofEasyCam::reset(){
	target.resetTransform();
	
	target.setPosition(0,0, 0);
	lookAt(target);
	
	resetTransform();
	setPosition(0, 0, lastDistance);
	
		
	xRot = 0;
	yRot = 0;
	zRot = 0;
	
	moveX = 0;
	moveY = 0;
	moveZ = 0;
	
	
}
//----------------------------------------
void ofEasyCam::setTarget(const ofVec3f& targetPoint){
	target.setPosition(targetPoint);
	lookAt(target);
}
//----------------------------------------
void ofEasyCam::setTarget(ofNode& targetNode){
	target = targetNode;
	lookAt(target);
}
//----------------------------------------
ofNode& ofEasyCam::getTarget(){
	return target;
}
//----------------------------------------
void ofEasyCam::setDistance(float distance){
	setDistance(distance, true);
}
//----------------------------------------
void ofEasyCam::setDistance(float distance, bool save){
	if (distance > 0.0f){
		if(save){
			this->lastDistance = distance;
		}
		setPosition( getPosition() + (distance * getZAxis()));
//		setPosition(0, 0, distance);
		bDistanceSet = true;
	}
}
//----------------------------------------
float ofEasyCam::getDistance() const {
	return getPosition().z;
}
//----------------------------------------
void ofEasyCam::setDrag(float drag){
	this->drag = drag;
}
//----------------------------------------
float ofEasyCam::getDrag() const {
	return drag;
}
//----------------------------------------
void ofEasyCam::setTranslationKey(char key){
	doTranslationKey = key;
}
//----------------------------------------
char ofEasyCam::getTranslationKey(){
	return doTranslationKey;
}
//----------------------------------------
void ofEasyCam::enableMouseInput(){
	if(!bMouseInputEnabled){
		bMouseInputEnabled = true;
		ofRegisterMouseEvents(this);
		ofRegisterKeyEvents(this);
	}
}
//----------------------------------------
void ofEasyCam::disableMouseInput(){
	if(bMouseInputEnabled){
		bMouseInputEnabled = false;
		ofUnregisterMouseEvents(this);
		ofUnregisterKeyEvents(this);
	}
}
//----------------------------------------
bool ofEasyCam::getMouseInputEnabled(){
	return bMouseInputEnabled;
}
//----------------------------------------
void ofEasyCam::updateTranslation(){
	if (bApplyInertia) {
		moveX *= drag;
		moveY *= drag;
		moveZ *= drag;
		if (ABS(moveX) <= minDifference && ABS(moveY) <= minDifference && ABS(moveZ) <= minDifference) {
			bApplyInertia = false;
			bDoTranslate = false;
		}
	}
	
	move((getXAxis() * moveX) + (getYAxis() * moveY) + (getZAxis() * moveZ));
}
//----------------------------------------
void ofEasyCam::updateRotation(){
	if (bApplyInertia) {
		xRot *=drag; 
		yRot *=drag;
		zRot *=drag;
		
		if (ABS(xRot) <= minDifference && ABS(yRot) <= minDifference && ABS(zRot) <= minDifference) {
			bApplyInertia = false;
			doRotate = false;
		}
	}
	
	curRot = ofQuaternion(xRot, ofCamera::getXAxis(), yRot, ofCamera::getYAxis(), zRot, ofCamera::getZAxis());
	ofCamera::setPosition((ofCamera::getGlobalPosition()-target.getGlobalPosition())*curRot +target.getGlobalPosition());
	ofNode::rotate(curRot);	
}
//----------------------------------------
void ofEasyCam::mouseDragged(ofMouseEventArgs& mouse){
	mouseVel = ofVec2f(mouse.x, mouse.y) - lastMouse;
	
	if (bDoTranslate) {
		moveX = 0;
		moveY = 0;
		moveZ = 0;
		if (mouse.button == 0) {
			moveX = mouseVel.x * sensitivity;
			moveY = mouseVel.y * sensitivity;
		}else {
			moveZ = mouseVel.y * sensitivityZ;
		}
	}else {
		xRot = 0;
		yRot = 0;
		zRot = 0;
		if (mouse.button == 0) {
			xRot = -mouseVel.y * sensitivity;
			yRot = -mouseVel.x * sensitivity;
		}else {
			ofVec2f center(ofGetWidth()/2, ofGetHeight()/2);
			zRot = - ofVec2f(mouse.x - center.x, mouse.y -center.y).angle(lastMouse - center);
		}
		doRotate = true;
	}
	lastMouse = ofVec2f(mouse.x,mouse.y);
}

//----------------------------------------
void ofEasyCam::mouseMoved(ofMouseEventArgs& mouse){
}

//----------------------------------------
void ofEasyCam::mousePressed(ofMouseEventArgs& mouse){
	unsigned long curTap = ofGetElapsedTimeMillis();
	if(lastTap != 0 && curTap - lastTap < doubleclickTime){
		reset();
	}
	lastTap = curTap;
	lastMouse = ofVec2f(mouse.x,mouse.y);
}

//----------------------------------------
void ofEasyCam::mouseReleased(ofMouseEventArgs& mouse){
	lastMouse = ofVec2f(mouse.x,mouse.y); 
	bApplyInertia = true;	
	
}

//----------------------------------------
void ofEasyCam::keyPressed(ofKeyEventArgs& key){
	if(key.key == doTranslationKey){
		bDoTranslate=true;
		doRotate = false;
	}
}

//----------------------------------------
void ofEasyCam::keyReleased(ofKeyEventArgs& key){
	if(key.key == doTranslationKey){
		bDoTranslate=false;
	}
}
