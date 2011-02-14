/*
 *  ofCamera.h
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 10/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

// TODO: 
// add off-axis projection options (eventually tile render support)
// fix ortho projection
// add get/set projection matrix
// support for left handed or right handed?

#pragma once


#include "ofRectangle.h"
#include "ofAppRunner.h"
#include "ofNode.h"

#define OF_CAMERA_MATRIX_CACHE_WARNING ofLog(OF_LOG_WARNING, "ofCamera::getXXXXMatrix : If you want to call this function a lot, then make sure ofCamera::cacheMatrices = true, then we'll cache the data from the draw loop");

// Use the public API of ofNode for all transformations
//class ofCamera : public ofNodeWithTarget {
class ofCamera : public ofNode {
public:
    ofCamera();
    
    enum SetupMode {
        DEFAULT_VIEW,   // conforms to OF defaults:     (0, 0) in upper left. +x:right, +y:down, +z:out. 1 unit == 1 pixel. 
        OPENGL_VIEW     // conforms to opengl defaults: (0, 0) in screen center. +x:right, +y:up, +z:out. window is 2 units wide
    };
    
    // TODO: implement this. complication with left handed vs right handed (need to support that first)
    // initial setup for camera. see enum SetupMode for details
    void setup(SetupMode setupMode, ofRectangle viewport = ofGetCurrentViewport());
    
    // projection properties:
    void setFov(float f);
    void setNearClip(float f);
    void setFarClip(float f);
    
    void enableOrtho();
    void disableOrtho();
    bool getOrtho() const;
    
    float getImagePlaneDistance(ofRectangle viewport = ofGetCurrentViewport()) const;
    
    // set the matrices
    virtual void begin(ofRectangle viewport = ofGetCurrentViewport());
    virtual void end();
    
    // for hardcore peeps, access to the projection matrix
    bool cacheMatrices;
    ofMatrix4x4 getProjectionMatrix(ofRectangle viewport = ofGetCurrentViewport());    
    ofMatrix4x4 getModelViewMatrix();
    ofMatrix4x4 getModelViewProjectionMatrix(ofRectangle viewport = ofGetCurrentViewport());
    
    // convert between spaces
    ofVec3f worldToScreen(ofVec3f WorldXYZ, ofRectangle viewport = ofGetCurrentViewport()); 
    ofVec3f screenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport = ofGetCurrentViewport());
    ofVec3f worldToCamera(ofVec3f WorldXYZ, ofRectangle viewport = ofGetCurrentViewport());
    ofVec3f cameraToWorld(ofVec3f CameraXYZ, ofRectangle viewport = ofGetCurrentViewport());
    
    
protected:
    void calcClipPlanes(ofRectangle viewport);
    
    bool isOrtho;
    float fov;
    float nearClip;
    float farClip;
    bool isActive;
    
    bool hasStoredMatrices;
    ofMatrix4x4 matProjection;
    ofMatrix4x4 matModelView;
};

