/*
 *  Grid.h
 *  advanced3dExample
 *
 *  Created by Elliot Woods on 19/01/2011.
 *  Copyright 2011 Kimchi and Chips. All rights reserved.
 *
 */

// Grid is a custom 3D
// object that we've
// created in this
// example.
//
// We inherit from ofNode

#include "ofMain.h"

class Grid : public ofNode
{
public:
	void	customDraw();
};