/*
 *  Grid.cpp
 *  advanced3dExample
 *
 *  Created by Elliot Woods on 19/01/2011.
 *  Copyright 2011 Kimchi and Chips. All rights reserved.
 *
 */

#include "Grid.h"

void Grid::customDraw()
{
	//////////////
	// DRAW GRID
	//////////////
	//
	//
	ofPushStyle();
	
	//gridlines
	//
	// we loop through drawing twice
	// once to draw lines perpendicular to x axis
	// second time to draw perpendicular to z axis
	//
	// x: iDimension = 0
	// z: iDimension = 1
	//
	for (int iDimension=0; iDimension<2; iDimension++)
	{
		if (iDimension==0)
			ofSetColor(255, 200, 200);
		else
			ofSetColor(200, 200, 255);

		for (float xz=-50; xz<=50; xz+= 5)
		{
			
			/////////////////////
			// draw thicker lines
			// at whole numbers
			// on grid
			//
			if (fabs(xz) == 50 || xz == 0)
				ofSetLineWidth(4);
			
			else if ( xz / 10.0f == floor(xz / 10.0f) )
				ofSetLineWidth(2);
			
			else
				ofSetLineWidth(1);
			//
			/////////////////////
			
			//draw line
			if (iDimension==0)
				ofLine(xz, 0, -50, xz, 0, 50);
			else
				ofLine(-50, 0, xz, 50, 0, xz);
		}
	}
	
	//labels
	ofSetColor(0, 0, 0);
	glDepthFunc(GL_ALWAYS);
	for (float xz = -50; xz<=50; xz+=10)
	{
		ofDrawBitmapString(ofToString(xz, 0), xz, 0, 0);
		ofDrawBitmapString(ofToString(xz, 0), 0, 0, xz);		
	}
	glDepthFunc(GL_LESS);
	
	ofPopStyle();
	//
	//
	//////////////	
	
	ofDrawAxis(10);
	
}