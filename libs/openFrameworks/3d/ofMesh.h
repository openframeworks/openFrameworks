//Keith Pasko, Memo Akten

#pragma once

#include "ofVertexData.h"
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
	
	void addElement(ofPrimitiveMode mode, const vector<ofVec3f>& verts);

	vector<ofVertexData> elements;
};
