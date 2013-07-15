/*
 *  ofCamera.cpp
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 10/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#include "ofCamera.h"
#include "ofLog.h"

//----------------------------------------
ofCamera::ofCamera() :
isOrtho(false),
fov(60),
nearClip(0),
farClip(0),
lensOffset(0.0f, 0.0f),
forceAspectRatio(false),
isActive(false),
vFlip(false)
{
}

//----------------------------------------
void ofCamera::setFov(float f) {
	fov = f;
}

//----------------------------------------
void ofCamera::setNearClip(float f) {
	nearClip = f;
}

//----------------------------------------
void ofCamera::setFarClip(float f) {
	farClip = f;
}

//----------------------------------------
void ofCamera::setLensOffset(const ofVec2f & lensOffset){
	this->lensOffset = lensOffset;
}

//----------------------------------------
void ofCamera::setAspectRatio(float aspectRatio){
	this->aspectRatio = aspectRatio;
	setForceAspectRatio(true);
}

//----------------------------------------
void ofCamera::setForceAspectRatio(bool forceAspectRatio){
	this->forceAspectRatio = forceAspectRatio;
}

//----------------------------------------
void ofCamera::setupPerspective(bool _vFlip, float fov, float nearDist, float farDist, const ofVec2f & lensOffset){
	ofRectangle orientedViewport = ofGetNativeViewport();
	float eyeX = orientedViewport.width / 2;
	float eyeY = orientedViewport.height / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;

	setFov(fov);
	setNearClip(nearDist);
	setFarClip(farDist);
	setLensOffset(lensOffset);
	setForceAspectRatio(false);

	setPosition(eyeX,eyeY,dist);
	lookAt(ofVec3f(eyeX,eyeY,0),ofVec3f(0,1,0));
	vFlip = _vFlip;
}

//----------------------------------------
void ofCamera::setupOffAxisViewPortal(const ofVec3f & topLeft, const ofVec3f & bottomLeft, const ofVec3f & bottomRight){
	ofVec3f bottomEdge = bottomRight - bottomLeft; // plane x axis
	ofVec3f leftEdge = topLeft - bottomLeft; // plane y axis
	ofVec3f bottomEdgeNorm = bottomEdge.normalized();
	ofVec3f leftEdgeNorm = leftEdge.normalized();
	ofVec3f bottomLeftToCam = this->getPosition() - bottomLeft;
	
	ofVec3f cameraLookVector = leftEdgeNorm.getCrossed(bottomEdgeNorm);
	
	ofVec3f cameraUpVector = bottomEdgeNorm.getCrossed(cameraLookVector);
	
	lookAt(cameraLookVector + this->getPosition(), cameraUpVector);

	//lensoffset
	ofVec2f lensOffset;
	lensOffset.x = -bottomLeftToCam.dot(bottomEdgeNorm) * 2.0f / bottomEdge.length() + 1.0f;
	lensOffset.y = -bottomLeftToCam.dot(leftEdgeNorm) * 2.0f / leftEdge.length() + 1.0f;
	setLensOffset(lensOffset);
	setAspectRatio( bottomEdge.length() / leftEdge.length() );
	float distanceAlongOpticalAxis = abs(bottomLeftToCam.dot(cameraLookVector));
	setFov(2.0f * RAD_TO_DEG * atan( (leftEdge.length() / 2.0f) / distanceAlongOpticalAxis));
}


//----------------------------------------
void ofCamera::setVFlip(bool vflip){
	vFlip = vflip;
}

//----------------------------------------
bool ofCamera::isVFlipped(){
	return vFlip;
}

//----------------------------------------
void ofCamera::enableOrtho() {
	isOrtho = true;
}

//----------------------------------------
void ofCamera::disableOrtho() {
	isOrtho = false;
}

//----------------------------------------
bool ofCamera::getOrtho() const {
	return isOrtho;
}

//----------------------------------------
float ofCamera::getImagePlaneDistance(ofRectangle viewport) const {
	return viewport.height / (2.0f * tanf(PI * fov / 360.0f));
}

//----------------------------------------
void ofCamera::begin(ofRectangle viewport) {
	if(!isActive) ofPushView();
	isActive = true;

	// autocalculate near/far clip planes if not set by user
	calcClipPlanes(viewport);

	ofViewport(viewport.x,viewport.y,viewport.width,viewport.height);
	ofSetOrientation(ofGetOrientation(),vFlip);

	ofSetMatrixMode(OF_MATRIX_PROJECTION);
	ofLoadMatrix( getProjectionMatrix(viewport) );

	ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	ofLoadMatrix( getModelViewMatrix() );
}

// if begin(); pushes first, then we need an end to pop
//----------------------------------------
void ofCamera::end() {
	if (isActive)
	{
		ofPopView();
		isActive = false;
	}
}

//----------------------------------------
ofMatrix4x4 ofCamera::getProjectionMatrix(ofRectangle viewport) const {
	if(isOrtho) {
		return ofMatrix4x4::newOrthoMatrix(0, viewport.width, 0, viewport.height, nearClip, farClip);
	}else{
		float aspect = forceAspectRatio ? aspectRatio : viewport.width/viewport.height;
		ofMatrix4x4 matProjection;
		matProjection.makePerspectiveMatrix(fov, aspect, nearClip, farClip);
		matProjection.translate(-lensOffset.x, -lensOffset.y, 0);
		return matProjection;
	}
}

//----------------------------------------
ofMatrix4x4 ofCamera::getModelViewMatrix() const {
	return ofMatrix4x4::getInverseOf(getGlobalTransformMatrix());
}

//----------------------------------------
ofMatrix4x4 ofCamera::getModelViewProjectionMatrix(ofRectangle viewport) const {
	return getModelViewMatrix() * getProjectionMatrix(viewport);
}

//----------------------------------------
ofVec3f ofCamera::worldToScreen(ofVec3f WorldXYZ, ofRectangle viewport) const {

	ofVec3f CameraXYZ = WorldXYZ * getModelViewProjectionMatrix(viewport);
	ofVec3f ScreenXYZ;

	ScreenXYZ.x = (CameraXYZ.x + 1.0f) / 2.0f * viewport.width + viewport.x;
	ScreenXYZ.y = (1.0f - CameraXYZ.y) / 2.0f * viewport.height + viewport.y;

	ScreenXYZ.z = CameraXYZ.z;

	return ScreenXYZ;

}

//----------------------------------------
ofVec3f ofCamera::screenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport) const {

	//convert from screen to camera
	ofVec3f CameraXYZ;
	CameraXYZ.x = 2.0f * (ScreenXYZ.x - viewport.x) / viewport.width - 1.0f;
	CameraXYZ.y = 1.0f - 2.0f *(ScreenXYZ.y - viewport.y) / viewport.height;
	CameraXYZ.z = ScreenXYZ.z;

	//get inverse camera matrix
	ofMatrix4x4 inverseCamera;
	inverseCamera.makeInvertOf(getModelViewProjectionMatrix(viewport));

	//convert camera to world
	return CameraXYZ * inverseCamera;

}

//----------------------------------------
ofVec3f ofCamera::worldToCamera(ofVec3f WorldXYZ, ofRectangle viewport) const {
	return WorldXYZ * getModelViewProjectionMatrix(viewport);
}

//----------------------------------------
ofVec3f ofCamera::cameraToWorld(ofVec3f CameraXYZ, ofRectangle viewport) const {

	ofMatrix4x4 inverseCamera;
	inverseCamera.makeInvertOf(getModelViewProjectionMatrix(viewport));

	return CameraXYZ * inverseCamera;
}

//----------------------------------------
void ofCamera::calcClipPlanes(ofRectangle viewport) {
	// autocalculate near/far clip planes if not set by user
	if(nearClip == 0 || farClip == 0) {
		float dist = getImagePlaneDistance(viewport);
		nearClip = (nearClip == 0) ? dist / 100.0f : nearClip;
		farClip = (farClip == 0) ? dist * 10.0f : farClip;
	}
}
