/*
 *  of3dUtils.cpp
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 15/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */


#include "of3dUtils.h"
#include "ofGraphics.h"

void ofDrawAxis(float size) {
	
	// draw x axis
	ofSetColor(255, 0, 0);
	glPushMatrix();
	glTranslatef(size/2, 0, 0);
	glScalef(size, 2, 2);
	ofBox(0, 0, 0, 1);
	glPopMatrix();
	
	// draw y axis
	ofSetColor(0, 255, 0);
	glPushMatrix();
	glTranslatef(0, size/2, 0);
	glScalef(2, size, 2);
	ofBox(0, 0, 0, 1);
	glPopMatrix();
	
	// draw z axis
	ofSetColor(0, 0, 2550);
	glPushMatrix();
	glTranslatef(0, 0, size/2);
	glScalef(2, 2, size);
	ofBox(0, 0, 0, 1);
	glPopMatrix();	
}