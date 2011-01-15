#pragma once

#include "ofMesh.h"
#include "ofVbo.h"

class ofVboMesh{
public:
	ofVboMesh();
	~ofVboMesh();
	
	void setMesh(ofMesh* m);
	
	ofMesh* getMesh();
	
	void setupVertices(int usage);
	void setupColors(int usage);
	void setupIndices();
	
//	void setupNormals(int usage);
//	void setupTextures(int usage);
	 
	void drawVertices();
	void drawWireframe();
	void drawFaces();
	
	void addTriangles(const vector<ofVec3f>& verts);
	void addTriangleFan(const vector<ofVec3f>& verts);
	void addTriangleStrip(const vector<ofVec3f>& verts);
	
	// build index array from faces as int* 
	vector<int> indices;
	
	//TODO: wrapper classes for ofMesh, so we don't have to use ->
	//TODO: update methods, generic update
	
    ofMesh* mesh;
	ofVbo vbo;
};
