/*
 *  ofCamera.h
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 10/01/2011.
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
	
	/// \brief Set the field of view for a perspective camera.
	///
	/// This sets the horizontal field of view for the camera, in degrees.
	/// This only operates with perspective cameras, and will have no effect 
	/// with cameras in orthographic mode. 
	///
	/// \param f The desired field of view for the camera, in degrees.
	void setFov(float f);
	
	
	void setNearClip(float f);
	void setFarClip(float f);
	void setLensOffset(const ofVec2f & lensOffset);
	
	/// \brief Set the recommended aspect ratio for a perspective camera.
	///
	/// Sets the aspect ratio of the camera to the desired float, and forces the use of aspect ratio calculations. 
	/// Currently only used with perspective cameras. The default value (and the value used with orthographic cameras)
	/// is the ratio of the viewport's width to the viewport's height. 
	/// 
	/// \param aspectRatio The desired aspect ratio, e.g. 1.3333, 1.6, 1.7777, etc. 
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
	
	/// \brief Obtain the screen coordinates of a point in the 3D world. 
	///
	/// Takes a three-dimensional (X,Y,Z) position, encoded as an ofVec3f, 
	/// and returns the location where it would appear on your (two-dimensional) display.
	/// Function returns this location as an ofVec3f, whose Z value is zero. 
	///
	/// \param WorldXYZ A 3D point in the world, whose screen coordinates you wish to know. 
	/// \param viewport (Optional) A viewport. The default is ofGetCurrentViewport(). 
	/// \returns An ofVec3f containing the screen coordinates of your 3D point of interest. 
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

