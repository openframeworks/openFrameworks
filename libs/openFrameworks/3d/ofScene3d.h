/*
 *  ofScene3d.h
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 14/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

// TODO:
// make sure same node isn't added more than once (use set?)
// ofNodes have children vector so childs don't need to be added to scene?
// camera / light management?


#pragma once

#include "ofNode3d.h"

class ofScene3d {
public:
	
	void enableDebugDraw();
	void disableDebugDraw();
	bool getDebugDraw() const;
	
	void addNode(ofNode3d& n);
	void removeNode(ofNode3d &n);
	
	ofNode3d& getNode(int i);
	int getNumNodes() const;
	vector<ofNode3d*>& getNodes();
	
	void draw();
	
protected:
	vector<ofNode3d*>nodes;
	bool bDrawDebug;
};