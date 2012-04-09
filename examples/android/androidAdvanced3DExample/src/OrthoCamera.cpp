/*
 *  OrthoCamera.cpp
 *  advanced3dExample
 *
 *  Created by Elliot Woods on 19/01/2011.
 *  Copyright 2011 Kimchi and Chips. All rights reserved.
 *
 */

#include "OrthoCamera.h"

OrthoCamera::OrthoCamera()
{
	enableOrtho();
	scale = 1;
}

void OrthoCamera::begin(ofRectangle rect)
{
	ofCamera::begin(rect);
	
	//////////////////////////////
	// CALCULATE ASPECT RATIO
	//////////////////////////////
	//
	float vw = ofGetViewportWidth();
	float vh = ofGetViewportHeight();
	
	//aspect ratio
	float ar = vw/vh;
	
	float scalex, scaley;
	
	if (ar > 1.0f)
	{
		//wide
		scalex = scale * ar;
		scaley = scale;
	} else {
		//tall
		scalex = scale;
		scaley = scale / ar;
	}
	//
	//////////////////////////////
	
	//////////////////////////////
	// SETUP PROJECTION
	//////////////////////////////
	//
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#ifndef TARGET_OPENGLES
	glOrtho(-scalex, scalex, -scaley, scaley, -20 * scale, 20 * scale);
#else
	// note that bottom and top are switched compared to glOrtho
	ofMatrix4x4 ortho = ofMatrix4x4::newOrthoMatrix(-scalex, scalex, scaley, -scaley,
			-20 * scale, 20 * scale);
	glMultMatrixf(ortho.getPtr());
#endif
	glMatrixMode(GL_MODELVIEW);
	//
	//////////////////////////////
}
