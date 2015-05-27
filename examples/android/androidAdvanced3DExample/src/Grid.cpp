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
	ofPushStyle();

	//give a saturation and lightness
	ofSetColor(255,100,100);
	
	ofDrawGrid(100.0f);
	
	ofPopStyle();

}