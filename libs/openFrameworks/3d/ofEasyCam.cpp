#include "ofEasyCam.h"
#include "ofEvents.h"
#include "ofMain.h"

#define OF_EASYCAM_DEFAULT_DISTANCE 100.0f

// when an ofEasyCam is moving due to momentum, this keeps it
// from moving forever by assuming small values are zero.
float minimumRotation = 1e-7;

//----------------------------------------
ofEasyCam::ofEasyCam():
mousePosViewPrev(0, 0), 
lastFrame(0),
drag(0.5),
thrust(3.0f),
distanceScaleVelocity(0) {
	target.setPosition(0, 0, 0);
	reset();
	setParent(target);
	
	lastMousePressed[0] = false;
	lastMousePressed[1] = false;
}

//----------------------------------------
void ofEasyCam::begin(ofRectangle rect) {
	// it's important to check whether we've already accounted for the mouse
	// just in case you use the camera multiple times in a single frame
	if (lastFrame != ofGetFrameNum()) {
		lastFrame = ofGetFrameNum();
		float dt = ofGetLastFrameTime();
		
		ofVec2f mousePosScreen = ofVec3f(ofGetMouseX() - rect.width/2 - rect.x, rect.height/2 - (ofGetMouseY() - rect.y), 0);
		ofVec2f mouseVelScreen = (mousePosScreen - mousePosScreenPrev).lengthSquared();
		
		if(ofGetMousePressed(0) || ofGetMousePressed(2) && mouseVelScreen.lengthSquared() > 1.0f ) {
			ofVec3f targetPos =  target.getGlobalPosition();
			ofVec3f mousePosXYZ = ofVec3f(mousePosScreen.x, mousePosScreen.y, targetPos.z);
			
			float sphereRadius = min(rect.width, rect.height)/2;
			float diffSquared = sphereRadius * sphereRadius - (targetPos - mousePosXYZ).lengthSquared();
			if(diffSquared <= 0) {
				mousePosXYZ.z = 0;
			} else {
				mousePosXYZ.z = sqrtf(diffSquared);
			}
			mousePosXYZ.z += targetPos.z;
			ofVec3f mousePosView = ofMatrix4x4::getInverseOf(target.getGlobalTransformMatrix()) * mousePosXYZ;
			
			//calc new rotation velocity
			ofQuaternion newRotation;
			if(lastMousePressed[0]) {
				newRotation.makeRotate(mousePosViewPrev, mousePosView);
			}

			//calc new scale velocity
			float newDistanceScaleVelocity = 0.0f;
			if(lastMousePressed[1]) {
				newDistanceScaleVelocity = 2 * (mousePosScreenPrev.y - mousePosScreen.y) / rect.height;
			}
			
			//apply thrust towards new velocities
			distanceScaleVelocity = ofLerp(distanceScaleVelocity, newDistanceScaleVelocity, thrust*dt);
			rotation.slerp(thrust*dt, rotation, newRotation); 			
			
			
			mousePosViewPrev = ofMatrix4x4::getInverseOf(target.getGlobalTransformMatrix()) * mousePosXYZ;
		}
		
		// apply transforms if they're big enough
		// TODO: this should be scaled by dt
		if (rotation.asVec3().lengthSquared() > minimumRotation) {
			target.rotate(rotation.conj());
		}
		if (abs(distanceScaleVelocity - 1.0f) > minimumRotation) {
			setDistance(getDistance() * (1.0f + distanceScaleVelocity));
		}

		//perform drag
		rotation.slerp(drag*dt, rotation, ofQuaternion(0,0,0,1));
		distanceScaleVelocity = distanceScaleVelocity * (1-drag*dt);
		lastMousePressed[0] = ofGetMousePressed(0);
		lastMousePressed[1] = ofGetMousePressed(2);
		
		mousePosScreenPrev = mousePosScreen;
		
		//reset view on press key 'r'
		if (ofGetKeyPressed('r')) {
			reset();
		}
	}
	
	ofCamera::begin(rect);
}

//----------------------------------------
void ofEasyCam::reset() {
	target.resetTransform();
	// TODO: this should be handled with a proper tanf() by ofCamera ideally
	setDistance(OF_EASYCAM_DEFAULT_DISTANCE);
	rotation = ofQuaternion(0,0,0,1);
	distanceScaleVelocity = 0;
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
void ofEasyCam::setDistance(float distance) {
	if (distance > 0.0f) {
		setPosition(0, 0, distance);
	}
}

//----------------------------------------
float ofEasyCam::getDistance() const {
	return position.z;
}

//----------------------------------------
void ofEasyCam::setThrust(float thrust) {
	this->thrust = thrust;
}

//----------------------------------------
float ofEasyCam::getThrust() const {
	return thrust;
}

//----------------------------------------
void ofEasyCam::setDrag(float drag) {
	this->drag = drag;
}

//----------------------------------------
float ofEasyCam::getDrag() const {
	return drag;
}