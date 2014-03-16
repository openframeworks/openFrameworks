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

/// \todo
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
	
	/// \todo
	void setNearClip(float f);
	void setFarClip(float f);
	
	/// \brief Set the "lens offset" applied to this camera.
	/// 
	/// Ordinarily, the camera is pointed straight down the center of its view frustum. 
	/// However, it is possible to orient the camera towards a location offset from the center of its frustum. 
	/// This is called an "assymetric frustum" and is used (for example) in stereo views. 
	/// It is acheived by applying an offset to the center of projection. 
	/// This function sets this offset from an ofVec2f argument. 
	/// For more information see http://www.orthostereo.com/geometryopengl.html.
	///
	/// \param lensOffset The "lens offset" to apply to this camera, encoded in an ofVec2f.
	void setLensOffset(const ofVec2f & lensOffset);
	
	/// \brief Set the recommended aspect ratio for a perspective camera.
	///
	/// Sets the aspect ratio of the camera to the desired float, and forces the use of aspect ratio calculations. 
	/// Currently only used with perspective cameras. The default value (and the value used with orthographic cameras)
	/// is the ratio of the viewport's width to the viewport's height. 
	/// 
	/// \param aspectRatio The desired aspect ratio, e.g. 1.3333, 1.6, 1.7777, etc. 
	void setAspectRatio(float aspectRatio);
	
	/// \brief Set whether or not the aspect ratio of this camera is forced to a non-default setting.
	/// 
	/// The camera's aspect ratio, by default, is the aspect ratio of your viewport. 
	/// If you have set a non-default value (with setAspectRatio()), 
	/// you can toggle whether or not this value is applied. 
	/// 
	/// \param forceAspectRatio Whether or not this camera should use an aspect ratio you have set yourself.  
	void setForceAspectRatio(bool forceAspectRatio);

	/// \brief Get the camera's field of view, in degrees.
	///
	/// Get the horizontal camera's field of view, in degrees.
	/// This is only meaningful for perspective cameras.
	///
	/// \returns The camera's field of view, in degrees.
	float getFov() const { return fov; };
	
	/// \todo
	float getNearClip() const { return nearClip; };
	float getFarClip() const { return farClip; };
	
	/// \brief Get the "lens offset" applied to this camera, encoded as an ofVec2f.
	/// 
	/// Ordinarily, the camera is pointed straight down the center of its view frustum. 
	/// However, it is possible to orient the camera towards a location offset from the center of its frustum. 
	/// This is called an "assymetric frustum" and is used (for example) in stereo views. 
	/// It is acheived by applying an offset to the center of projection. 
	/// This function returns the offset that has been applied, as an ofVec2f. 
	/// For more information see http://www.orthostereo.com/geometryopengl.html.
	///
	/// \returns The "lens offset" applied to this camera, encoded in an ofVec2f.
	ofVec2f getLensOffset() const { return lensOffset; };
	
	/// \brief Get the boolean state which indicates whether the aspect ratio of this camera is forced to a non-default setting.
	/// 
	/// \returns A boolean: whether or not this camera's aspect ratio is set to a non-default value. 
	bool getForceAspectRatio() const {return forceAspectRatio;};
	
	/// \brief Get the aspect ratio of this camera's viewport.
	///
	/// Returns the aspect ratio of this camera's viewport. Usually this will be the ratio
	/// of the width to height of your display. Intended for perspective cameras. 
	/// 
	/// \returns The aspect ratio of this camera's viewport.
	float getAspectRatio() const {return aspectRatio; };
	
	/// \todo
	void setupPerspective(bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0, const ofVec2f & lensOffset = ofVec2f(0.0f, 0.0f));
	void setupOffAxisViewPortal(const ofVec3f & topLeft, const ofVec3f & bottomLeft, const ofVec3f & bottomRight);
	
	/// \todo
	void setVFlip(bool vflip);
	bool isVFlipped();

	/// \todo
	void enableOrtho();
	void disableOrtho();
	bool getOrtho() const;
	
	/// \todo
	float getImagePlaneDistance(ofRectangle viewport = ofGetCurrentViewport()) const;
	
	/// \todo
	// set the matrices
	virtual void begin(ofRectangle viewport = ofGetCurrentViewport());
	virtual void end();
	
	/// \todo
	// for hardcore peeps, access to the projection matrix
	ofMatrix4x4 getProjectionMatrix(ofRectangle viewport = ofGetCurrentViewport()) const;
	ofMatrix4x4 getModelViewMatrix() const;
	ofMatrix4x4 getModelViewProjectionMatrix(ofRectangle viewport = ofGetCurrentViewport()) const;
	
	// convert between spaces
	
	/// \brief Obtain the screen coordinates of a point in the 3D world. 
	///
	/// Takes an (X,Y,Z) point in your 3D world, encoded as an ofVec3f, 
	/// and returns the location (also as an ofVec3f) where this point would appear 
	/// on your (two-dimensional) display. The screen position's "Z coordinate" 
	/// is set to be the same as your camera's. 
	///
	/// \param WorldXYZ A 3D point in the world, whose screen coordinates you wish to know. 
	/// \param viewport (Optional) A viewport. The default is ofGetCurrentViewport(). 
	/// \returns An ofVec3f containing the screen coordinates of your 3D point of interest. 
	ofVec3f worldToScreen(ofVec3f WorldXYZ, ofRectangle viewport = ofGetCurrentViewport()) const;
	
	/// \brief Obtain the coordinates, in the 3D world, of a 2D point presumed to be on your screen.
	///
	/// Takes a pixel location on your screen, encoded in an ofVec3f, 
	/// and returns (also as an ofVec3f) the 3D world coordinates of that point.
	/// You'll also need to specify a Z value when providing your screen point. 
	/// This Z value is interpreted as a distance into or away from the screen. 
	///
	/// \param ScreenXYZ A point on your screen, whose 3D world coordinates you wish to know.
	ofVec3f screenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport = ofGetCurrentViewport()) const;
	
	/// \todo
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

