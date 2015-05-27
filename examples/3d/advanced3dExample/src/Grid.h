#pragma once
#include "ofMain.h"

// grid is a custom 3D
//  object that we've
//  created in this
//  example.
//
// We inherit from ofNode

class grid : public ofNode {
	public:
		void    customDraw();
};