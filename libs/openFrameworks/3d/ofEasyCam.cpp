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
	reset();
	sensitivityTranslate = {1,1,1};
	sensitivityRot = {1,1,1};
}
//----------------------------------------
void ofEasyCam::enableOrtho(){
	if (!getOrtho()) {
		prevFarClip = getFarClip();
		prevNearClip = getNearClip();
	}
	setNearClip(-10000);
	setFarClip(10000);
	ofCamera::enableOrtho();	
}
//----------------------------------------
void ofEasyCam::disableOrtho(){
	if(getOrtho()){
		setNearClip(prevNearClip);
		setFarClip(prevFarClip);
	}
	ofCamera::disableOrtho();
}
//----------------------------------------
void ofEasyCam::update(ofEventArgs & args){
	viewport = getViewport(this->viewport);
	if(!bDistanceSet && bAutoDistance){
		setDistance(getImagePlaneDistance(viewport), true);
	}
	if(bMouseInputEnabled && events){
		if(events->getMousePressed()) {
			updateMouse(glm::vec2(events->getMouseX(),events->getMouseY()));
		}
		if (bDoRotate) {
			updateRotation();
		}else if (bDoTranslate || bDoZoom) {
			updateTranslation();
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

	target.setPosition(0, 0, 0);
	lookAt(target);

	resetTransform();
	setPosition(0, 0, lastDistance);

	rot = {0,0,0};
	translate = {0,0,0};

	bApplyInertia = false;
	bDoTranslate = false;
	bDoRotate = false;
}

//----------------------------------------
void ofEasyCam::setTarget(const glm::vec3& targetPoint){
	target.setPosition(targetPoint);
	lookAt(target);
}

//----------------------------------------
void ofEasyCam::setTarget(ofNode& targetNode){
	target = targetNode;
	lookAt(target);
}

//----------------------------------------
const ofNode& ofEasyCam::getTarget() const{
	return target;
}

//----------------------------------------
void ofEasyCam::setDistance(float distance){
	setDistance(distance, true);
}

//----------------------------------------
void ofEasyCam::setDistance(float distance, bool save){//should this be the distance from the camera to the target?
	if(distance > 0.0f){
		if(save){
			this->lastDistance = distance;
		}
		setPosition(target.getPosition() + (distance * getZAxis()));
		bDistanceSet = true;
	}
}

//----------------------------------------
float ofEasyCam::getDistance() const{
	return glm::distance(target.getPosition(), getPosition());
}

//----------------------------------------
void ofEasyCam::setAutoDistance(bool bAutoDistance){
	this->bAutoDistance = bAutoDistance;
	if(bAutoDistance){
		bDistanceSet = false;
	}
}

//----------------------------------------
void ofEasyCam::setDrag(float drag){
	this->drag = drag;
}

//----------------------------------------
float ofEasyCam::getDrag() const{
	return drag;
}

//----------------------------------------
void ofEasyCam::setTranslationKey(char key){
	doTranslationKey = key;
}

//----------------------------------------
char ofEasyCam::getTranslationKey() const{
	return doTranslationKey;
}

//----------------------------------------
void ofEasyCam::enableMouseInput(){
	if(!bMouseInputEnabled && events){
		listeners.push_back(events->update.newListener(this, &ofEasyCam::update));
		listeners.push_back(events->mousePressed.newListener(this, &ofEasyCam::mousePressed));
		listeners.push_back(events->mouseReleased.newListener(this, &ofEasyCam::mouseReleased));
		listeners.push_back(events->mouseScrolled.newListener(this, &ofEasyCam::mouseScrolled));
	}
	// if enableMouseInput was called within ofApp::setup()
	// `events` will still carry a null pointer, and bad things
	// will happen. Therefore we only update the flag. 
	bMouseInputEnabled = true;
	// setEvents() is called upon first load, and will make sure 
	// to enable the mouse input once the camera is fully loaded.
}

//----------------------------------------
void ofEasyCam::disableMouseInput(){
	if(bMouseInputEnabled && events){
		listeners.clear();
	}
	// if disableMouseInput was called within ofApp::setup()
	// `events` will still carry a null pointer, and bad things
	// will happen. Therefore we only update the flag. 
	bMouseInputEnabled = false;
	// setEvents() is called upon first load, and will make sure 
	// to enable the mouse input once the camera is fully loaded.
}
//----------------------------------------
bool ofEasyCam::getMouseInputEnabled() const{
	return bMouseInputEnabled;
}

//----------------------------------------
void ofEasyCam::setEvents(ofCoreEvents & _events){
	// If en/disableMouseInput were called within ofApp::setup(),
	// bMouseInputEnabled will tell us about whether the camera
	// mouse input needs to be initialised as enabled or disabled.
	// we will still set `events`, so that subsequent enabling
	// and disabling can work.

	// we need a temporary copy of bMouseInputEnabled, since it will 
	// get changed by disableMouseInput as a side-effect.
	bool wasMouseInputEnabled = bMouseInputEnabled || !events;
	disableMouseInput();
	events = &_events;
	if (wasMouseInputEnabled) {
		// note: this will set bMouseInputEnabled to true as a side-effect.
		enableMouseInput();
	}
	bEventsSet = true;
}

//----------------------------------------
void ofEasyCam::setRotationSensitivity(const glm::vec3& sens){
	sensitivityRot = sens;
}
//----------------------------------------
void ofEasyCam::setRotationSensitivity(float x, float y, float z){
	setRotationSensitivity({x,y,z});
}
//----------------------------------------
void ofEasyCam::setTranslationSensitivity(const glm::vec3& sens){
	sensitivityTranslate = sens;
}
//----------------------------------------
void ofEasyCam::setTranslationSensitivity(float x, float y, float z){
	sensitivityTranslate = {x,y,z};
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
bool ofEasyCam::getMouseMiddleButtonEnabled() const{
	return bEnableMouseMiddleButton;
}

//----------------------------------------
glm::vec3 ofEasyCam::up() const{
	if(bRelativeYAxis){
		if(bApplyInertia){
			return getYAxis();
		}else{
			return onPressAxisY;
		}
	}else{
		return upAxis;
	}
}

//----------------------------------------
void ofEasyCam::setRelativeYAxis(bool relative){
	bRelativeYAxis = relative;
}

//----------------------------------------
bool ofEasyCam::getRelativeYAxis() const{
	return bRelativeYAxis;
}

//----------------------------------------
void ofEasyCam::setUpAxis(const glm::vec3 & _up){
	upAxis = _up;
}

//----------------------------------------
const glm::vec3 & ofEasyCam::getUpAxis() const{
	return upAxis;
}

//----------------------------------------
void ofEasyCam::enableInertia(){
	doInertia = true;
}

//----------------------------------------
void ofEasyCam::disableInertia(){
	doInertia = false;
}

//----------------------------------------
bool ofEasyCam::getInertiaEnabled() const{
	return doInertia;
}

//----------------------------------------
void ofEasyCam::updateTranslation(){
	if(bApplyInertia){
		translate *= drag;
		bDoZoom = (ABS(translate.z) >= minDifference);
		if(ABS(translate.x) <= minDifference && ABS(translate.y) <= minDifference && ABS(translate.z) <= minDifference){
			translate = {0,0,0};
			bApplyInertia = false;
			bDoTranslate = false;
			bDoZoom = false;
			bIsScrolling = false;
			return;
		}
		move((getXAxis() * translate.x) + (getYAxis() * translate.y) + (getZAxis() * translate.z));
	}
	if(bDoTranslate || bDoZoom){
		if(getOrtho()){
			glm::vec3 mousePre ;
			if (bDoZoom){
				mousePre = screenToWorld(glm::vec3((bIsScrolling?mouseAtScroll:onPressMouse),0));
			}
			move(glm::vec3(onPressAxisX * translate.x) + (onPressAxisY * translate.y));
			if (bDoZoom) {
				setScale(getScale() + translate.z);
				move(mousePre - screenToWorld(glm::vec3((bIsScrolling?mouseAtScroll:onPressMouse),0)));
			}
		}else{
			move(glm::vec3(onPressAxisX * translate.x) + (onPressAxisY * translate.y) + (onPressAxisZ * translate.z));
		}
	}
	bDoZoom = false;
	bIsScrolling = false;
}	

//----------------------------------------
void ofEasyCam::updateRotation(){
	if(bApplyInertia){
		rot *=drag;
		if(ABS(rot.x) <= minDifference && ABS(rot.y) <= minDifference && ABS(rot.z) <= minDifference){
			rot = {0,0,0};
			bApplyInertia = false;
			bDoRotate = false;
			return;
		}
		
	}
	if(bDoRotate){
		if (bApplyInertia) {
			curRot = glm::angleAxis(rot.z, getZAxis()) * glm::angleAxis(rot.y, up()) * glm::angleAxis(rot.x, getXAxis());
		}else{
			curRot = glm::angleAxis(rot.z, onPressAxisZ) * glm::angleAxis(rot.y, up()) * glm::angleAxis(rot.x, onPressAxisX);
		}
		rotateAround(curRot, target.getGlobalPosition());
		rotate(curRot);
	}
}

//----------------------------------------
void ofEasyCam::setControlArea(const ofRectangle & _controlArea) {
	controlArea = _controlArea;
}

//----------------------------------------
void ofEasyCam::clearControlArea() {
	controlArea = ofRectangle();
}

//----------------------------------------
ofRectangle ofEasyCam::getControlArea() const {
	if (controlArea.isZero()) {
		if (viewport.isZero()) {
			return getRenderer()->getCurrentViewport();
		}
		return viewport;
	}
	return controlArea;
}

//----------------------------------------
void ofEasyCam::mousePressed(ofMouseEventArgs & mouse){
	ofRectangle area = getControlArea();
	if(area.inside(mouse.x, mouse.y)){
		onPressMouse = mouse;
		prevMouse = mouse;
		onPressAxisX = getXAxis();
		onPressAxisY = getYAxis();
		onPressAxisZ = getZAxis();
		onPressPosition = ofCamera::getGlobalPosition();
		onPressOrientation = ofCamera::getGlobalOrientation();

		bool bMouseLeft = mouse.button == OF_MOUSE_BUTTON_LEFT;
		bool bMouseRight = mouse.button == OF_MOUSE_BUTTON_RIGHT;
		bool bMouseMiddle = (bEnableMouseMiddleButton && mouse.button == OF_MOUSE_BUTTON_MIDDLE);
		bool bTransKey = events->getKeyPressed(doTranslationKey) ;

		bool bSetRotation = false;
		
		if(getOrtho()){
			if (!bTransKey) {
				if(bMouseLeft){
					bDoTranslate = true;
					bDoRotate = false;
				}else if(bMouseRight){
					bDoTranslate = true;
					bDoRotate = false;
					bDoZoom = true;
					mouseAtScroll = mouse;
				}else{
					bSetRotation = true;
				}
			}else{
				bSetRotation = true;
			}
		}else{
			if( bMouseMiddle|| bTransKey || bMouseRight){
				bDoTranslate = true;
				bDoRotate = false;
			}else if(bMouseLeft){
				bSetRotation = true;
			}
		}
		if(bSetRotation){
			bDoTranslate = false;
			bDoRotate = true;
			bInsideArcball = glm::length(mouse - area.getCenter()) < std::min(area.width/2, area.height/2);
		}
		bApplyInertia = false;
	}
}

//----------------------------------------
void ofEasyCam::mouseReleased(ofMouseEventArgs & mouse){
	ofRectangle area = getControlArea();

	if(area.inside(mouse)){
		// Check if it's double click
		unsigned long curTap = ofGetElapsedTimeMillis();
		if(lastTap != 0 && curTap - lastTap < doubleclickTime){
			reset();
			return;
		}
		lastTap = curTap;
	}

	if(doInertia){
		bApplyInertia = true;
	}else{
		bDoRotate = false;
		rot = {0,0,0};
		bDoTranslate = false;
		translate = {0,0,0};
	}
}
//----------------------------------------
void ofEasyCam::mouseScrolled(ofMouseEventArgs & mouse){
	ofRectangle area = getControlArea();
	if(area.inside(mouse)){
		mouseVel = mouse  - prevMouse;
		prevMouse = mouse;
		if (doInertia) {
			bApplyInertia = true;
		}
		onPressPosition = ofCamera::getGlobalPosition();
		onPressAxisZ = getZAxis();
		if (getOrtho()) {
			translate.z = sensitivityScroll * mouse.scrollY / ofGetHeight();
			mouseAtScroll = mouse;
		}else{
			translate.z = mouse.scrollY * 30 * sensitivityTranslate.z * (getDistance() + FLT_EPSILON)/ area.height;
		}
		bDoZoom = true;
		bIsScrolling = true;
	}
}

//----------------------------------------
void ofEasyCam::updateMouse(const glm::vec2 & mouse){
	ofRectangle area = getControlArea();
	int vFlip =(isVFlipped()?-1:1);

	mouseVel = mouse  - prevMouse;

	rot = {0,0,0};
	translate = {0,0,0};
	if(bDoTranslate){
		if (getOrtho()) {
			if(ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)){
				translate.z = mouseVel.y * sensitivityScroll / area.height;
				bDoZoom = true;
			}else{
				translate.x = -mouseVel.x * getScale().z;
				translate.y = vFlip * mouseVel.y * getScale().z;
			}
		}else{
			if(ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)){
				translate.z = mouseVel.y * (sensitivityTranslate.z * 0.7f) * (getDistance() + FLT_EPSILON)/ area.height;
			}else{
				translate.x = -mouseVel.x * sensitivityTranslate.x * 0.5f * (getDistance() + FLT_EPSILON)/ area.width;
				translate.y = vFlip * mouseVel.y * sensitivityTranslate.y* 0.5f * (getDistance() + FLT_EPSILON)/ area.height;
			}
		}
	}else if(bDoRotate){

		if(bInsideArcball){
			rot.x = vFlip * -mouseVel.y * sensitivityRot.x * glm::pi<float>() / std::min(area.width, area.height);
			rot.y = -mouseVel.x * sensitivityRot.y * glm::pi<float>() / std::min(area.width, area.height);
		}else{
			glm::vec3 center = area.getCenter();
			rot.z = sensitivityRot.z * -vFlip * glm::orientedAngle(glm::normalize(mouse - center),
												glm::normalize(prevMouse - center));
		}
	}
	prevMouse = mouse;
}