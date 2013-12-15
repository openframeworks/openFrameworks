/*
 *  ofCamera.h
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 10/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

// TODO: 
// add set projection matrix
// support for left handed or right handed?

#pragma once


#include "ofRectangle.h"
#include "ofAppRunner.h"
#include "ofNode.h"

// Use the public API of ofNode for all transformations
//class ofCamera : public ofNodeWithTarget {
class ofCamera : public ofNode {
public:
	ofCamera();
	virtual ~ofCamera(){};
	
	// projection properties:
	void setFov(float f);
	void setNearClip(float f);
	void setFarClip(float f);
	void setLensOffset(const ofVec2f & lensOffset);
	void setAspectRatio(float aspectRatio);
	void setForceAspectRatio(bool forceAspectRatio);

	float getFov() const { return fov; };
	float getNearClip() const { return nearClip; };
	float getFarClip() const { return farClip; };
	ofVec2f getLensOffset() const { return lensOffset; };
	bool getForceAspectRatio() const {return forceAspectRatio;};
    float getAspectRatio() const {return aspectRatio; };
	void setupPerspective(bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0, const ofVec2f & lensOffset = ofVec2f(0.0f, 0.0f));
	void setupOffAxisViewPortal(const ofVec3f & topLeft, const ofVec3f & bottomLeft, const ofVec3f & bottomRight);
	
	void setVFlip(bool vflip);
	bool isVFlipped();

	void enableOrtho();
	void disableOrtho();
	bool getOrtho() const;
	
	float getImagePlaneDistance(ofRectangle viewport = ofGetCurrentViewport()) const;
	
	// set the matrices
	virtual void begin(ofRectangle viewport = ofGetCurrentViewport());
	virtual void end();
	
	// for hardcore peeps, access to the projection matrix
	ofMatrix4x4 getProjectionMatrix(ofRectangle viewport = ofGetCurrentViewport()) const;
	ofMatrix4x4 getModelViewMatrix() const;
	ofMatrix4x4 getModelViewProjectionMatrix(ofRectangle viewport = ofGetCurrentViewport()) const;
	
	// convert between spaces
	ofVec3f worldToScreen(ofVec3f WorldXYZ, ofRectangle viewport = ofGetCurrentViewport()) const;
	ofVec3f screenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport = ofGetCurrentViewport()) const;
	ofVec3f worldToCamera(ofVec3f WorldXYZ, ofRectangle viewport = ofGetCurrentViewport()) const;
	ofVec3f cameraToWorld(ofVec3f CameraXYZ, ofRectangle viewport = ofGetCurrentViewport()) const;
	
	
protected:
	void calcClipPlanes(ofRectangle viewport);
	
	bool isOrtho;
	float fov;
	float nearClip;
	float farClip;
	ofVec2f lensOffset;
	bool forceAspectRatio;
	float aspectRatio; // only used when forceAspect=true, = w / h
	bool isActive;
	bool vFlip;
};

