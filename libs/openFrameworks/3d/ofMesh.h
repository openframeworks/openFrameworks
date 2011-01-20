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
//	int addVertex(ofVec3f pos, ofVec3f normal, ofColor color, ofVec2f texCoord);
//	int addVertex(const ofVec3f& pos);
//	int addFace(int vi1, int vi2, int vi3);
	void flipNormals();
//	int getFaceVertexId(const ofFace& f, int index);
	//would like to be reference, but what if there is none?
//	ofVec3f* getFaceVertex(const ofFace& f, int index);

	int numVertices();
	int numFaces();
	int numNormals();
	int numTexCoords();
	int numColors();
	
	void addElement(int mode, const vector<ofVec3f>& verts);
	
	/*
	void addTriangleFan(const vector<ofVec3f>& verts) {
		addElement(OF_MESH_FAN, verts);
	}
	 */

	vector<ofMeshElement> elements;

	string name;
	bool bUsingNormals;
	bool bUsingColors;
	bool bUsingTexCoords;
	int faceType;
	//	int windingType;

	
	/* to be included?
	vector<ofVec3f> tangents;
	vector<ofVec3f> biTangents;

	// TODO: multiple color/texCoord maps?
	vector <vector <ofColor> > colors;
	vector <vector <ofVec2f> > texCoords;
	*/
};
