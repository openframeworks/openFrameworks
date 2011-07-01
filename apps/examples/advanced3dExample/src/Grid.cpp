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
	///////////////////////////////////
	// DRAW GRID
	///////////////////////////////////
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
	// xz is our common variable
	// in the first loop (0), xz means x
	// in the second loop (1), xz means z
	//
	// we reuse the same variable so we can use
	// the same code twice (*)
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
			// Draw thicker lines
			// at whole numbers
			// on the grid
			//
			
			//if xz
			if (fabs(xz) == 50 || xz == 0)
				ofSetLineWidth(4);
			
			else if ( xz / 10.0f == floor(xz / 10.0f) )
				ofSetLineWidth(2);
			
			else
				ofSetLineWidth(1);
			//
			/////////////////////
			
			/////////////////////
			// Draw lines
			//
			
			//here we check which
			//dimension we're
			//drawing in (*)
			//
			if (iDimension==0)
				ofLine(xz, 0, -50, xz, 0, 50);
			else
				ofLine(-50, 0, xz, 50, 0, xz);
			//
			/////////////////////
		}
	}
	
	////////////////////////
	// Draw numbers on axes
	//
	
	ofSetColor(255, 255, 255);
	
	//this line ensures
	//that we're drawing
	//on top of
	//everything else.
	//
	//The DEPTH FUNCTION
	//tells OpenGL whether
	//to draw a new pixel
	//when it is compared
	//to the DEPTH BUFFER
	//
	glDepthFunc(GL_ALWAYS);
	
	//if we want to make
	//our text always face 
	//towards the camera
	//then we need to turn
	//on billboard mode
	//
	//this mode is cleared
	//at ofPopStyle()
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	
	for (float xz = -50; xz<=50; xz+=10)
	{
		ofDrawBitmapString(ofToString(xz, 0), xz, 0, 0);
		ofDrawBitmapString(ofToString(xz, 0), 0, 0, xz);		
	}
	
	//this line restores
	//the depth function
	//to the default value
	//for 3D functions
	glDepthFunc(GL_LESS);
	
	//
	////////////////////////

	
	ofPopStyle();
	//
	//
	///////////////////////////////////
	
	ofDrawAxis(10);
	
}