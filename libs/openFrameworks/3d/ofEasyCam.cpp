#include "ofEasyCam.h"
#include "ofMain.h"

// when an ofEasyCam is moving due to momentum, this keeps it
// from moving forever by assuming small values are zero.
float minimumRotation = 1e-7;

// this is the default on windows os
int doubleclickTime = 500;

//----------------------------------------
ofEasyCam::ofEasyCam():
mousePosViewPrev(0, 0), 
lastFrame(0),
drag(0.1f),
zoomSpeed(2.0f),
bDistanceSet(false),
lastDistance(0),
lastTap(0),
distanceScaleVelocity(0) {
	target.setPosition(0, 0, 0);
	reset();
	setParent(target);
	
	ofRegisterMouseEvents(this);
	
	lastMousePressed[0] = false;
	lastMousePressed[1] = false;
}

//----------------------------------------
void ofEasyCam::begin(ofRectangle rect) {
	if(!bDistanceSet) {
		setDistance(getImagePlaneDistance(rect), true);
	}

	// it's important to check whether we've already accounted for the mouse
	// just in case you use the camera multiple times in a single frame
	if (lastFrame != ofGetFrameNum()) {
		lastFrame = ofGetFrameNum();
		
		// if there is some smart way to use dt to scale the values drag, we should do it
		// you can't simply multiply drag etc because the behavior is unstable at high framerates
		// float dt = ofGetLastFrameTime();
		
		ofVec2f mousePosScreen = ofVec3f(ofGetMouseX() - rect.width/2 - rect.x, rect.height/2 - (ofGetMouseY() - rect.y), 0);
		ofVec2f mouseVelScreen = (mousePosScreen - mousePosScreenPrev).lengthSquared();
		
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
			newDistanceScaleVelocity = zoomSpeed * (mousePosScreenPrev.y - mousePosScreen.y) / rect.height;
		}
		
		//apply drag towards new velocities
		distanceScaleVelocity = ofLerp(distanceScaleVelocity, newDistanceScaleVelocity, drag); // TODO: add dt
		rotation.slerp(drag, rotation, newRotation); // TODO: add dt		
		
		mousePosViewPrev = ofMatrix4x4::getInverseOf(target.getGlobalTransformMatrix()) * mousePosXYZ;
		
		// apply transforms if they're big enough
		// TODO: this should be scaled by dt
		if (rotation.asVec3().lengthSquared() > minimumRotation) {
			target.rotate(rotation.conj());
		}
		if (abs(distanceScaleVelocity - 1.0f) > minimumRotation) {
			setDistance(getDistance() * (1.0f + distanceScaleVelocity), false);
		}

		// note the mouse button states
		lastMousePressed[0] = ofGetMousePressed(0);
		lastMousePressed[1] = ofGetMousePressed(2);
		
		mousePosScreenPrev = mousePosScreen;
	}
	
	ofCamera::begin(rect);
}

//----------------------------------------
void ofEasyCam::reset() {
	target.resetTransform();
	setDistance(lastDistance, false);
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
	setDistance(distance, true);
}

//----------------------------------------
void ofEasyCam::setDistance(float distance, bool save) {
	if (distance > 0.0f) {
		if(save) {
			this->lastDistance = distance;
		}
		setPosition(0, 0, distance);
		bDistanceSet = true;
	}
}

//----------------------------------------
float ofEasyCam::getDistance() const {
	return position.z;
}

//----------------------------------------
void ofEasyCam::setDrag(float drag) {
	this->drag = drag;
}

//----------------------------------------
float ofEasyCam::getDrag() const {
	return drag;
}

//----------------------------------------
void ofEasyCam::mouseDragged(ofMouseEventArgs& mouse) {
}

//----------------------------------------
void ofEasyCam::mouseMoved(ofMouseEventArgs& mouse) {
}

//----------------------------------------
void ofEasyCam::mousePressed(ofMouseEventArgs& mouse) {
	unsigned long curTap = ofGetElapsedTimeMillis();
	if(lastTap != 0 && curTap - lastTap < doubleclickTime) {
		reset();
	}
	lastTap = curTap;
}

//----------------------------------------
void ofEasyCam::mouseReleased(ofMouseEventArgs& mouse) {
}