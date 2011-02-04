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


ofCamera::ofCamera() :
cacheMatrices(false),
isOrtho(false),
fov(60),
nearClip(0),
farClip(0),
isActive(false),
hasStoredMatrices(false) {
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
	
	ofSetCoordHandedness(OF_RIGHT_HANDED);
	
	// autocalculate near/far clip planes if not set by user
	calcClipPlanes(viewport);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(isOrtho) {
		//			if(vFlip) glOrtho(0, width, height, 0, nearDist, farDist);
		//			else 
#ifndef TARGET_OPENGLES
		glOrtho(0, viewport.width, 0, viewport.height, nearClip, farClip);
#endif		
	} else {
		gluPerspective(fov, viewport.width/viewport.height, nearClip, farClip);
	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(ofMatrix4x4::getInverseOf(getGlobalTransformMatrix()).getPtr());
	ofViewport(viewport);
	
	//store current matrices
	if (cacheMatrices)
	{
		glGetFloatv(GL_PROJECTION_MATRIX, matProjection.getPtr());
		glGetFloatv(GL_MODELVIEW_MATRIX, matModelView.getPtr());
		hasStoredMatrices = true;
	}
	
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
ofMatrix4x4 ofCamera::getProjectionMatrix(ofRectangle viewport) {
	
	if (cacheMatrices)
		return matProjection;
	else
	{
		OF_CAMERA_MATRIX_CACHE_WARNING
		matProjection.makePerspectiveMatrix(fov, viewport.width/viewport.height, nearClip, farClip);
		return matProjection;
	}

}
//----------------------------------------
ofMatrix4x4 ofCamera::getModelViewMatrix() {

	if (cacheMatrices)
		return matModelView;
	else
	{
		OF_CAMERA_MATRIX_CACHE_WARNING
		matModelView.makeInvertOf(getGlobalTransformMatrix());
		return matModelView;
	}

}
//----------------------------------------
ofMatrix4x4 ofCamera::getModelViewProjectionMatrix(ofRectangle viewport) {
	return getModelViewMatrix() * getProjectionMatrix(viewport);
}
//----------------------------------------
ofVec3f ofCamera::worldToScreen(ofVec3f WorldXYZ, ofRectangle viewport) {
	
	ofVec3f CameraXYZ = WorldXYZ * getModelViewProjectionMatrix();
	ofVec3f ScreenXYZ;
	
	ScreenXYZ.x = (CameraXYZ.x + 1.0f) / 2.0f * viewport.width + viewport.x;
	ScreenXYZ.y = (1.0f - CameraXYZ.y) / 2.0f * viewport.height + viewport.y;
	
	ScreenXYZ.z = CameraXYZ.z;
	
	return ScreenXYZ;
	
}
//----------------------------------------
ofVec3f ofCamera::screenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport) {
	
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
ofVec3f ofCamera::worldToCamera(ofVec3f WorldXYZ, ofRectangle viewport) {
	return WorldXYZ * getModelViewProjectionMatrix(viewport);
}
//----------------------------------------
ofVec3f ofCamera::cameraToWorld(ofVec3f CameraXYZ, ofRectangle viewport) {
	
	ofMatrix4x4 inverseCamera;
	inverseCamera.makeInvertOf(getModelViewProjectionMatrix(viewport));
	
	return CameraXYZ * inverseCamera;
}
//----------------------------------------
void ofCamera::calcClipPlanes(ofRectangle viewport)
{
	// autocalculate near/far clip planes if not set by user
	if(nearClip == 0 || farClip == 0) {
		float dist = getImagePlaneDistance(viewport);
		nearClip = (nearClip == 0) ? dist / 100.0f : nearClip;
		farClip = (farClip == 0) ? dist * 10.0f : farClip;
	}
}
