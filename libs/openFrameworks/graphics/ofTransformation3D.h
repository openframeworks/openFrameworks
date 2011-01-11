///*
// *  ofTransformation3D.h
// *  openFrameworksLib
// *
// *  Created by Memo Akten on 10/01/2011.
// *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
// *
// */
//
//#pragma once
//
//#include "ofMain.h"
//
//// a 3d transformation
//class ofTransformation3D {
//public:
//	ofTransformation3D();
//	~ofTransformation3D();
//	
//	// set position
//	void setPosition(float px, float py, float pz);
//	void setPosition(ofVec3f const p);
//
//	// get position
//	ofVec3f getPosition() const;
//
//	
//	// move sideways (in object X axis)
//	void truck(float amount);
//	
//	// move up/down (in object Y axis)
//	void boom(float amount);
//	
//	// move forward/backward (in object Z axis)
//	void dolly(float amount);
//	
//	// if you have a desired target, use this:
//	void setLookAt(ofVec3f lookAtPosition);
//	
//	// or set orientation as a quat if you have it
//	void setOrientation(ofQuaternion quat);
//	
//	// or euler can be useful, but prepare for gimbal lock
//	void setOrientation(ofPoint eulerAngles);
//	
//	void setPitch(float a);
//	void setHeading(float a); 
//	void setRoll(float a);
//	
//	void getPitch(float amount);
//	void getHeading(float amount);
//	void getRoll(float amount);
//	
//	// if you have a rotation matrix, give it that
//	void setOrientation(ofMatrix3x3 &orientation);
//	
//	// add pan, truck etc. 
//	
//	
//	// SCALE
//	void setScale(float s);
//	void setScale(float sx, float sy, float sz);
//	void setScale(ofVec3f const scale);
//
//	ofVec3f getScale() const;
//	
//protected:
//	ofMatrix44 m44;
//};
