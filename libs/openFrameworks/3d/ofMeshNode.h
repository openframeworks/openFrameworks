/*
 *  ofMeshNode.h
 *  cameraTest
 *
 *  Created by Memo Akten on 12/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#pragma once

#include "ofNode.h"
#include "ofMain.h"

class ofMesh;

class ofMeshNode : public ofNode {
public:
	ofMesh *mesh;
	
//	void customDraw();
};