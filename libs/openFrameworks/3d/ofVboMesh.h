#pragma once

#include "ofMesh.h"
#include "ofVbo.h"

//TODO: implement multiple index arrays to eliminate the need to calculate


//enum for drawing mode status
enum{
	OF_MESH_POINTS,
	OF_MESH_WIREFRAME,
	OF_MESH_FILL
};


class ofVboMesh{
public:
	ofVboMesh();
	~ofVboMesh();
	
	ofVboMesh(const ofVboMesh& v);
	ofVboMesh& operator=(const ofVboMesh& v);
	void clone(const ofVboMesh& v);
	
	void setMesh(ofMesh* m);
	
	ofMesh* getMesh();
	ofMesh* getMesh() const;
	
	void clear();
	
	void setupVertices(int usage);
	void setupColors(int usage);
	void setupIndices(int indexMode);
	
//	void setupNormals(int usage);
//	void setupTextures(int usage);
	 
	void drawVertices();
	void drawWireframe();
	void drawFaces();
	
	void addTriangles(const vector<ofVec3f>& verts);
	void addTriangleFan(const vector<ofVec3f>& verts);
	void addTriangleStrip(const vector<ofVec3f>& verts);
	
	void addMeshVertices(const vector<ofVec3f>& verts);
	
	// build index array from faces as int* 
	vector<GLuint> indices;
	
	//TODO: wrapper classes for ofMesh, so we don't have to use ->
	//TODO: update methods, generic update
	
    ofMesh* mesh;
	ofVbo vbo;
	int mode;
	bool bIsInited;
};
