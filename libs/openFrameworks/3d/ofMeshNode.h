/*
 *  ofMeshNode.h
 *  cameraTest
 *
 *  Created by Memo Akten on 12/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#pragma once

#include "ofNode3d.h"
#include "ofMain.h"

class ofMesh;

class ofMeshNode : public ofNode3d {
public:
	ofMesh *mesh;
	
	void customDraw() {
		ofSetColor(255, 128, 255);
		ofBox(0, 0, 0, 10);
		
		float axisLength = 15;
		// draw world x axis
		ofSetColor(255, 0, 0);
		glPushMatrix();
		glTranslatef(axisLength/2, 0, 0);
		glScalef(axisLength, 2, 2);
		ofBox(0, 0, 0, 1);
		glPopMatrix();
		
		// draw world y axis
		ofSetColor(0, 255, 0);
		glPushMatrix();
		glTranslatef(0, axisLength/2, 0);
		glScalef(2, axisLength, 2);
		ofBox(0, 0, 0, 1);
		glPopMatrix();
		
		// draw world z axis
		ofSetColor(0, 0, 2550);
		glPushMatrix();
		glTranslatef(0, 0, axisLength/2);
		glScalef(2, 2, axisLength);
		ofBox(0, 0, 0, 1);
		glPopMatrix();	
	}
};