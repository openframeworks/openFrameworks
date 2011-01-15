/*
 *  ofScene3d.cpp
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 14/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#include "ofScene3d.h"

//----------------------------------------
void ofScene3d::enableDebugDraw() {
	bDrawDebug = true;
}

//----------------------------------------
void ofScene3d::disableDebugDraw() {
	bDrawDebug = false;
}

//----------------------------------------
bool ofScene3d::getDebugDraw() const {
	return bDrawDebug;
}

//----------------------------------------
void ofScene3d::addNode(ofNode3d& n) {
	nodes.push_back(&n);
}

//----------------------------------------
void ofScene3d::removeNode(ofNode3d& n) {
//	nodes.erase(&n);
}


//----------------------------------------
ofNode3d& ofScene3d::getNode(int i) {
	return *nodes.at(i);
}


//----------------------------------------
int ofScene3d::getNumNodes() const {
	return nodes.size();
}


//----------------------------------------
vector<ofNode3d*>& ofScene3d::getNodes() {
	return nodes;
}

//----------------------------------------
void ofScene3d::draw() {
	for(int i=0; i<nodes.size(); i++) {
		nodes[i]->draw();
	}
	
	if(bDrawDebug) {
		for(int i=0; i<nodes.size(); i++) {
			nodes[i]->debugDraw();
		}
	}

}
