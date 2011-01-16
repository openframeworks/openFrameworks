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

#include "ofNode.h"

class ofScene3d {
public:
	
	void addNode(ofNode& n);
	void removeNode(ofNode &n);
	
	ofNode& getNode(int i);
	int getNumNodes() const;
	vector<ofNode*>& getNodes();
	
	void draw();
	
	friend ostream& operator<<(ostream& os, const ofScene3d& scene);
	
protected:
	vector<ofNode*>nodes;
};

inline ostream& operator<<(ostream& os, const ofScene3d& scene) {
	vector<ofNode*>::const_iterator it = scene.nodes.begin();
	while(it != scene.nodes.end()) {
		os << *(*it) << std::endl;
		++it;
	}
	return os;
}
