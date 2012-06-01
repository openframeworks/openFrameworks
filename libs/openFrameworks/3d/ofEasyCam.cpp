#include "ofEasyCam.h"
#include "ofMath.h"
#include "ofUtils.h"

// when an ofEasyCam is moving due to momentum, this keeps it
// from moving forever by assuming small values are zero.
float epsilonTransform = 1e-7;

// this is the default on windows os
unsigned long doubleclickTime = 500;

//----------------------------------------
ofEasyCam::ofEasyCam(){
	drag = 0.1f;
	zoomSpeed = 2.0f;
	bMouseInputEnabled = false;
	mousePosViewPrev.set(0, 0);
	lastFrame = 0;
	lastTap	= 0;
	bDistanceSet = false; 
	lastDistance = 0;
	distanceScaleVelocity = 0;

	target.setPosition(0, 0, 0);

	reset();
	setParent(target);
	
	enableMouseInput();
}

//----------------------------------------
ofEasyCam::~ofEasyCam(){
	//this causes a crash on exit - not needed anymore?
	//disableMouseInput();
}

//----------------------------------------
void ofEasyCam::begin(ofRectangle viewport){
	if(bMouseInputEnabled){
		if(!bDistanceSet){
			setDistance(getImagePlaneDistance(viewport), true);
		}
		
		// it's important to check whether we've already accounted for the mouse
		// just in case you use the camera multiple times in a single frame
		if (lastFrame != ofGetFrameNum()){
			lastFrame = ofGetFrameNum();
			
			// if there is some smart way to use dt to scale the values drag, we should do it
			// you can't simply multiply drag etc because the behavior is unstable at high framerates
			// float dt = ofGetLastFrameTime();
			
			ofVec2f mousePosScreen = ofVec3f(ofGetMouseX() - viewport.width/2 - viewport.x, viewport.height/2 - (ofGetMouseY() - viewport.y), 0);
			ofVec2f mouseVelScreen = ofVec2f((mousePosScreen - mousePosScreenPrev).lengthSquared());
			
			ofVec3f targetPos =  target.getGlobalPosition();
			ofVec3f mousePosXYZ = ofVec3f(mousePosScreen.x, mousePosScreen.y, targetPos.z);
			
			float sphereRadius = min(viewport.width, viewport.height)/2;
			float diffSquared = sphereRadius * sphereRadius - (targetPos - mousePosXYZ).lengthSquared();
			if(diffSquared <= 0){
				mousePosXYZ.z = 0;
			} else {
				mousePosXYZ.z = sqrtf(diffSquared);
			}
			mousePosXYZ.z += targetPos.z;
			ofVec3f mousePosView = ofMatrix4x4::getInverseOf(target.getGlobalTransformMatrix()) * mousePosXYZ;
			
			bool mousePressedCur[] = {ofGetMousePressed(0), ofGetMousePressed(2)};
			
			//calc new rotation velocity
			ofQuaternion newRotation;
			if(mousePressedPrev[0] && mousePressedCur[0]){
				newRotation.makeRotate(mousePosViewPrev, mousePosView);
			}
			
			//calc new scale velocity
			float newDistanceScaleVelocity = 0.0f;
			if(mousePressedPrev[1] && mousePressedCur[1]){
				newDistanceScaleVelocity = zoomSpeed * (mousePosScreenPrev.y - mousePosScreen.y) / viewport.height;
			}
			
			mousePressedPrev[0] = mousePressedCur[0];
			mousePressedPrev[1] = mousePressedCur[1];
			
			ofVec3f newTranslation;
			// TODO: this doesn't work at all. why not?
			if(ofGetMousePressed() && ofGetKeyPressed(OF_KEY_SHIFT)){
				newTranslation = mousePosScreenPrev - mousePosScreen;
			}
			
			//apply drag towards new velocities
			distanceScaleVelocity = ofLerp(distanceScaleVelocity, newDistanceScaleVelocity, drag); // TODO: add dt
			rotation.slerp(drag, rotation, newRotation); // TODO: add dt		
			translation.interpolate(newTranslation, drag);
			
			mousePosViewPrev = ofMatrix4x4::getInverseOf(target.getGlobalTransformMatrix()) * mousePosXYZ;
			
			// apply transforms if they're big enough
			// TODO: these should be scaled by dt
			if(translation.lengthSquared() > epsilonTransform){
				// TODO: this isn't quite right, it needs to move wrt the rotation
				target.move(translation);
			}
			if (rotation.asVec3().lengthSquared() > epsilonTransform){
				target.rotate(rotation.conj());
			}
			if (abs(distanceScaleVelocity - 1.0f) > epsilonTransform){
				setDistance(getDistance() * (1.0f + distanceScaleVelocity), false);
			}
			
			mousePosScreenPrev = mousePosScreen;
		}
	}
	ofCamera::begin(viewport);
}

//----------------------------------------
void ofEasyCam::reset(){
	target.resetTransform();
	setDistance(lastDistance, false);
	rotation = ofQuaternion(0,0,0,1);
	distanceScaleVelocity = 0;
}

//----------------------------------------
void ofEasyCam::setTarget(const ofVec3f& targetPoint){
	target.setPosition(targetPoint);
}

//----------------------------------------
void ofEasyCam::setTarget(ofNode& targetNode){
	target.setPosition(ofVec3f(0, 0, 0));
	target.setParent(targetNode);
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
		setPosition(0, 0, distance);
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
void ofEasyCam::enableMouseInput(){
	if(!bMouseInputEnabled){
		bMouseInputEnabled = true;
		ofRegisterMouseEvents(this);
	}
}


//----------------------------------------
void ofEasyCam::disableMouseInput(){
	if(bMouseInputEnabled){
		bMouseInputEnabled = false;
		ofUnregisterMouseEvents(this);
	}
}

//----------------------------------------
bool ofEasyCam::getMouseInputEnabled(){
	return bMouseInputEnabled;
}


//----------------------------------------
void ofEasyCam::mouseDragged(ofMouseEventArgs& mouse){
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
}

//----------------------------------------
void ofEasyCam::mouseReleased(ofMouseEventArgs& mouse){
}
