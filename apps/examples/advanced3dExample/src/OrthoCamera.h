/*
 *  OrthoCamera.h
 *  advanced3dExample
 *
 *  Created by Elliot Woods on 19/01/2011.
 *  Copyright 2011 Kimchi and Chips. All rights reserved.
 *
 */

// Ortho camera is a custom
// camera we've created in
// this example
//
// We inherit from ofCamera

#include "ofMain.h"

class OrthoCamera : public ofCamera
{
public:
	OrthoCamera();
	
	void	begin(ofRectangle rect = ofGetWindowRect());
	
	float	scale;
};