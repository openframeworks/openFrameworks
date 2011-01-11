///*
// *  ofCamera.h
// *  openFrameworksLib
// *
// *  Created by Memo Akten on 10/01/2011.
// *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
// *
// */
//
//
//
//
//ofCamera {
//public:
//	
//	// wrappers to transform
//	// setting position:
//	void setPosition(ofPoint pos);
//	
//	// different ways for setting orientation:
//	
//	// if you have a desired target, use this:
//	void setLookAt(ofPoint lookAtPosition);
//	
//	// or set orientation as a quat if you have it
//	void setOrientation(ofQuad quat);
//	
//	// or euler can be useful, but prepare for gimbal lock
//	void setOrientation(ofPoint eulerAngles);
//	void setPitch(float a);
//	void setHeading(float a); 
//	void setRoll(float a);
//	
//	// if you have a rotation matrix, give it that
//	void setOrientation(ofMatrix33 &orientation);
//	
//	
//	// projection properties:
//	void setFieldOfView(float fov);
//	
//	// not sure if these should be in camera or not, but could be
//	void setNearPlane(float f);
//	void setFarPlane(float f);
//	
//	
//	// set the matrices
//	// not sure if this should push first, or if that should be the users responsibility
//	void begin() {
//		
//	}
//	
//	// if begin(); pushes first, then we need an end to pop
//	void end();
//	
//	
//	// for hardcore peeps, access to the generated matrices()
//	ofMatrix44 &getModelViewMatrix();
//	ofMatrix44 &getProjectionMatrix();	
//	
//protected:
//	ofTransform3D transform;
//	ofPoint lookAt;
//	float fov;
//	float nearPlane;
//	float farDist;
//	ofMatrix44 modelViewMatrix();
//	ofMatrix44 projectionMatrix();
//};
//
