//Keith Pasko, Memo Akten

#pragma once

#include "ofMeshElement.h"
#include "ofUtils.h"

//TODO: implement via this
// add/edit/remove elements

class ofMesh{
public:
	ofMesh();
	~ofMesh();

	void clear();

	int numVertices();
	int numFaces();
	int numNormals();
	int numTexCoords();
	int numColors();
	
	void addElement(ofTriangleMode mode, const vector<ofVec3f>& verts);

	vector<ofMeshElement> elements;
};
