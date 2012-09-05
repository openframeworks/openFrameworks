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
	lastTap	= 0;
	lastDistance = 0;
	drag = 0.9f;
	sensitivityRot = 1.0f;//when 1 moving the mouse from one side to the other of the arcball (min(viewport.width, viewport.height)) will rotate 180degrees. when .5, 90 degrees.
	sensitivityXY = .5;
	sensitivityZ= .7;
	
	bDistanceSet = false; 
	bMouseInputEnabled = false;
	bDoRotate = false;
	bApplyInertia =false;
	bDoTranslate = false;
	bInsideArcball = true;
	bValidClick = false;
	bEnableMouseMiddleButton = true;
	
	doTranslationKey = 'm';
	
	reset();
	enableMouseInput();	

}

//----------------------------------------
ofEasyCam::~ofEasyCam(){
	disableMouseInput();
}
//----------------------------------------
void ofEasyCam::update(ofEventArgs & args){
	if(bMouseInputEnabled){
		if(!bDistanceSet){
			setDistance(getImagePlaneDistance(viewport), true);
		}
		rotationFactor = sensitivityRot * 180 / min(viewport.width, viewport.height);
		if (bMouseInputEnabled) {
			updateMouse();
		}
		
		if (bDoRotate) {
			updateRotation();
		}else if (bDoTranslate) {
			updateTranslation(); 
		}
	}	
}
//----------------------------------------
void ofEasyCam::begin(ofRectangle viewport){
	this->viewport = viewport;
	ofCamera::begin(viewport);	
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
void ofEasyCam::setDistance(float distance, bool save){//should this be the distance from the camera to the target?
	if (distance > 0.0f){
		if(save){
			this->lastDistance = distance;
		}
		setPosition(target.getPosition() + (distance * getZAxis()));
		bDistanceSet = true;
	}
}
//----------------------------------------
float ofEasyCam::getDistance() const {
	return target.getPosition().distance(getPosition());
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
	//	ofRegisterMouseEvents(this);
		ofAddListener(ofEvents().update , this, &ofEasyCam::update);
	}
}
//----------------------------------------
void ofEasyCam::disableMouseInput(){
	if(bMouseInputEnabled){
		bMouseInputEnabled = false;
		//ofUnregisterMouseEvents(this);
		ofRemoveListener(ofEvents().update, this, &ofEasyCam::update);
	}
}
//----------------------------------------
bool ofEasyCam::getMouseInputEnabled(){
	return bMouseInputEnabled;
}
//----------------------------------------
void ofEasyCam::enableMouseMiddleButton(){
	bEnableMouseMiddleButton = true;
}
//----------------------------------------
void ofEasyCam::disableMouseMiddleButton(){
	bEnableMouseMiddleButton = false;
}
//----------------------------------------
bool ofEasyCam::getMouseMiddleButtonEnabled(){
	return bEnableMouseMiddleButton;
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
			bDoRotate = false;
		}
	}
	curRot = ofQuaternion(xRot, ofCamera::getXAxis(), yRot, ofCamera::getYAxis(), zRot, ofCamera::getZAxis());
	setPosition((ofCamera::getGlobalPosition()-target.getGlobalPosition())*curRot +target.getGlobalPosition());
	rotate(curRot);
}
//----------------------------------------
void ofEasyCam::updateMouse(){
	mouse = ofVec2f(ofGetMouseX(), ofGetMouseY());
	if(viewport.inside(mouse.x, mouse.y) && !bValidClick && ofGetMousePressed()){	
		unsigned long curTap = ofGetElapsedTimeMillis();
		if(lastTap != 0 && curTap - lastTap < doubleclickTime){
			reset();
		}
		if ((bEnableMouseMiddleButton && ofGetMousePressed(1)) || ofGetKeyPressed(doTranslationKey)  || ofGetMousePressed(2)){
			bDoTranslate = true;
			bDoRotate = false;
			bApplyInertia = false;
		}else if (ofGetMousePressed(0)) {
			bDoTranslate = false;
			bDoRotate = true;
			bApplyInertia = false;
			if(ofVec2f(mouse.x - viewport.x - (viewport.width/2), mouse.y - viewport.y - (viewport.height/2)).length() < min(viewport.width/2, viewport.height/2)){
				bInsideArcball = true;
			}else {
				bInsideArcball = false;
			}
		}
		lastTap = curTap;
		//lastMouse = ofVec2f(ofGetPreviousMouseX(),ofGetPreviousMouseY()); //this was causing the camera to have a tiny "random" rotation when clicked.
		lastMouse = mouse;
		bValidClick = true;
		bApplyInertia = false;
	}
	
	if (bValidClick) {
		if (!ofGetMousePressed()) {
			bApplyInertia = true;
			bValidClick = false;
		}else {
			mouseVel = mouse  - lastMouse;
			
			if (bDoTranslate) {
				moveX = 0;
				moveY = 0;
				moveZ = 0;
				if (ofGetMousePressed(2)) {
					moveZ = mouseVel.y * sensitivityZ * getDistance() / viewport.height;				
				}else {
					moveX = -mouseVel.x * sensitivityXY * getDistance() /viewport.width;
					moveY =  mouseVel.y * sensitivityXY * getDistance() /viewport.height;
				}
			}else {
				xRot = 0;
				yRot = 0;
				zRot = 0;
				if (bInsideArcball) {
					xRot = -mouseVel.y * rotationFactor;
					yRot = -mouseVel.x * rotationFactor;
				}else {
					ofVec2f center(viewport.width/2, viewport.height/2);
					zRot = - ofVec2f(mouse.x - viewport.x - center.x, mouse.y -viewport.y -center.y).angle(lastMouse - ofVec2f(viewport.x, viewport.y) - center);
				}
			}
			lastMouse = mouse;
		}
	}
}
