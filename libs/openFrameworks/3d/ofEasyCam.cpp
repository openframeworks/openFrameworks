#include "ofEasyCam.h"
#include "ofMath.h"
#include "ofUtils.h"

// when an ofEasyCam is moving due to momentum, this keeps it
// from moving forever by assuming small values are zero.
static const float minDifference = 0.1e-5f;

// this is the default on windows os
static const unsigned long doubleclickTime = 200;

//----------------------------------------
ofEasyCam::ofEasyCam(){
	lastTap	= 0;
	lastDistance = 0;
	drag = 0.9f;
	sensitivityRot = 1.0f;//when 1 moving the mouse from one side to the other of the arcball (min(viewport.width, viewport.height)) will rotate 180degrees. when .5, 90 degrees.
	sensitivityXY = .5f;
	sensitivityZ= .7f;
	
	bDistanceSet = false; 
	bMouseInputEnabled = false;
	bDoRotate = false;
	bApplyInertia =false;
	bDoTranslate = false;
	bDoScrollZoom = false;
	bInsideArcball = true;
	bEnableMouseMiddleButton = true;
	bAutoDistance = true;
	doTranslationKey = 'm';
	bEventsSet = false;
	events = NULL;
	
	reset();

}

//----------------------------------------
ofEasyCam::~ofEasyCam(){
	disableMouseInput();
}

//----------------------------------------
void ofEasyCam::update(ofEventArgs & args){
	viewport = getViewport(this->viewport);
    if(!bDistanceSet && bAutoDistance){
        setDistance(getImagePlaneDistance(viewport), true);
    }
    if(bMouseInputEnabled){
	
		rotationFactor = sensitivityRot * 180 / min(viewport.width, viewport.height);

		if(events->getMousePressed()) prevMouse = ofVec2f(events->getMouseX(),events->getMouseY());
		
		if (bDoRotate) {
			updateRotation();
		}else if (bDoTranslate || bDoScrollZoom) {
			updateTranslation(); 
			bDoScrollZoom = false;
		}
	}	
}

//----------------------------------------
void ofEasyCam::begin(ofRectangle _viewport){
	if(!bEventsSet){
		setEvents(ofEvents());
	}
	viewport = getViewport(_viewport);
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

	bApplyInertia = false;
	bDoTranslate = false;
	bDoRotate = false;
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
void ofEasyCam::setAutoDistance(bool bAutoDistance){
    this->bAutoDistance = bAutoDistance;
    if (bAutoDistance) {
        bDistanceSet = false;
    }
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
		bEventsSet = true;
		ofAddListener(events->update, this, &ofEasyCam::update);
		ofAddListener(events->mouseDragged , this, &ofEasyCam::mouseDragged);
		ofAddListener(events->mousePressed, this, &ofEasyCam::mousePressed);
		ofAddListener(events->mouseReleased, this, &ofEasyCam::mouseReleased);
		ofAddListener(events->mouseScrolled, this, &ofEasyCam::mouseScrolled);
	}
}

//----------------------------------------
void ofEasyCam::disableMouseInput(){
	if(bMouseInputEnabled){
		bMouseInputEnabled = false;
		ofRemoveListener(events->update, this, &ofEasyCam::update);
		ofRemoveListener(events->mouseDragged, this, &ofEasyCam::mouseDragged);
		ofRemoveListener(events->mousePressed, this, &ofEasyCam::mousePressed);
		ofRemoveListener(events->mouseReleased, this, &ofEasyCam::mouseReleased);
		ofRemoveListener(events->mouseScrolled, this, &ofEasyCam::mouseScrolled);
	}
}

void ofEasyCam::setEvents(ofCoreEvents & _events){
	disableMouseInput();
	bEventsSet = true;
	events = &_events;
	enableMouseInput();
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
		move((getXAxis() * moveX) + (getYAxis() * moveY) + (getZAxis() * moveZ));
	}else{
		setPosition(prevPosition + ofVec3f(prevAxisX * moveX) + (prevAxisY * moveY) + (prevAxisZ * moveZ));
	}
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
		curRot = ofQuaternion(xRot, getXAxis(), yRot, getYAxis(), zRot, getZAxis());
		setPosition((getGlobalPosition()-target.getGlobalPosition())*curRot +target.getGlobalPosition());
		rotate(curRot);
	}else{
		curRot = ofQuaternion(xRot, prevAxisX, yRot, prevAxisY, zRot, prevAxisZ);
		setPosition((prevPosition-target.getGlobalPosition())*curRot +target.getGlobalPosition());
		setOrientation(prevOrientation * curRot);
	}
}

void ofEasyCam::mousePressed(ofMouseEventArgs & mouse){
	ofRectangle viewport = getViewport(this->viewport);
	if(viewport.inside(mouse.x, mouse.y)){
		lastMouse = mouse;
		prevMouse = mouse;
		prevAxisX = getXAxis();
		prevAxisY = getYAxis();
		prevAxisZ = getZAxis();
		prevPosition = ofCamera::getGlobalPosition();
		prevOrientation = ofCamera::getGlobalOrientation();

		if ((bEnableMouseMiddleButton && mouse.button == OF_MOUSE_BUTTON_MIDDLE) || events->getKeyPressed(doTranslationKey)  || mouse.button == OF_MOUSE_BUTTON_RIGHT){
			bDoTranslate = true;
			bDoRotate = false;
		}else if (mouse.button == OF_MOUSE_BUTTON_LEFT) {
			bDoTranslate = false;
			bDoRotate = true;
			if(ofVec2f(mouse.x - viewport.x - (viewport.width/2), mouse.y - viewport.y - (viewport.height/2)).length() < min(viewport.width/2, viewport.height/2)){
				bInsideArcball = true;
			}else {
				bInsideArcball = false;
			}
		}
		bApplyInertia = false;
	}
}

void ofEasyCam::mouseReleased(ofMouseEventArgs & mouse){
	unsigned long curTap = ofGetElapsedTimeMillis();
	ofRectangle viewport = getViewport(this->viewport);
	if(lastTap != 0 && curTap - lastTap < doubleclickTime){
		reset();
		return;
	}
	lastTap = curTap;
	bApplyInertia = true;
	mouseVel = mouse  - prevMouse;

	updateMouse(mouse);
	ofVec2f center(viewport.width/2, viewport.height/2);
	zRot = - isVFlipped() * ofVec2f(mouse.x - viewport.x - center.x, mouse.y - viewport.y - center.y).angle(prevMouse - ofVec2f(viewport.x, viewport.y) - center);
}

void ofEasyCam::mouseDragged(ofMouseEventArgs & mouse){
	mouseVel = mouse  - lastMouse;

	updateMouse(mouse);
}

void ofEasyCam::mouseScrolled(ofMouseEventArgs & mouse){
	ofRectangle viewport = getViewport(this->viewport);
	prevPosition = ofCamera::getGlobalPosition();
	prevAxisZ = getZAxis();
	moveZ = mouse.y * 30 * sensitivityZ * (getDistance() + FLT_EPSILON)/ viewport.height;
	bDoScrollZoom = true;
}


void ofEasyCam::updateMouse(const ofMouseEventArgs & mouse){
	ofRectangle viewport = getViewport(this->viewport);
	int vFlip;
	if(isVFlipped()){
		vFlip = -1;
	}else{
		vFlip =  1;
	}
	if (bDoTranslate) {
		moveX = 0;
		moveY = 0;
		moveZ = 0;
		if (mouse.button == OF_MOUSE_BUTTON_RIGHT) {
			moveZ = mouseVel.y * sensitivityZ * (getDistance() + FLT_EPSILON)/ viewport.height;
		}else {
			moveX = -mouseVel.x * sensitivityXY * (getDistance() + FLT_EPSILON)/viewport.width;
			moveY = vFlip * mouseVel.y * sensitivityXY * (getDistance() + FLT_EPSILON)/viewport.height;
		}
	}else {
		xRot = 0;
		yRot = 0;
		zRot = 0;
		if (bInsideArcball) {
			xRot = vFlip * -mouseVel.y * rotationFactor;
			yRot = -mouseVel.x * rotationFactor;
		}else {
			ofVec2f center(viewport.width/2, viewport.height/2);
			zRot = - vFlip * ofVec2f(mouse.x - viewport.x - center.x, mouse.y - viewport.y - center.y).angle(lastMouse - ofVec2f(viewport.x, viewport.y) - center);
		}
	}
}
