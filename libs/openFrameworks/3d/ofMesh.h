#pragma once

#include "ofVec3f.h"
#include "ofVec2f.h"
#include "ofColor.h"
#include "ofUtils.h"

#define OF_MESH_MAX_VERTS_PER_FACE 3
#define OF_MAX_TEX_MAPPINGS 1
#define OF_MAX_COLOR_MAPPINGS 1

class ofFace{
public:
	ofFace();
	ofFace(const ofFace& f);
	ofFace(int vi0, int vi1, int vi2);
	~ofFace();
	
	/* unchecked operators
	inline bool operator==(const ofFace& f);
	inline bool operator!= (const ofFace& f);
	*/
	bool operator=(const ofFace& f);
	
	//TODO: hard-lock this to three?
	vector<int> indices;
};

class ofMesh{
public:
	ofMesh();
	~ofMesh();

	void clear();
	int addVertex(ofVec3f pos, ofVec3f normal, ofColor color, ofVec2f texCoord);
	int addVertex(const ofVec3f& pos);
	int addFace(int vi1, int vi2, int vi3);
	void flipNormals();
	int getFaceVertexId(const ofFace& f, int index);
	//would like to be reference, but what if there is none?
	ofVec3f* getFaceVertex(const ofFace& f, int index);

	int numVertices();
	int numFaces();
	int numNormals();
	int numTexCoords();
	int numColors();

	/*
	int numTangents(){
	return tangents.size();
	}

	int numBiTangents(){
	return biTangents.size();
	}

	int getNumColorMaps(){
	return colors.size();
	}

	int numTexMaps(){
	return texCoords.size();
	}
	*/

	vector<ofVec3f> vertices;
	vector<ofVec3f> normals;
	vector<ofVec2f> texCoords;
	vector<ofColor> colors;

	vector<ofFace> faces;

	vector<ofVec3f>::iterator it;

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