//Keith Pasko, Memo Akten

#pragma once

#include "ofMeshElement.h"
#include "ofVbo.h"
#include "ofShape.h"
#include "ofBaseTypes.h"

//TODO:
// vector of VBOs, 2 per element

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
	
	void setMeshElement(ofMeshElement* m);
	void setUseIndices(bool useIndices=true);
	
	ofMeshElement* getMeshElement();
	const ofMeshElement* getMeshElement() const;

	void setDrawType(int drawType);

	bool setupVertices(int usage);
	bool setupColors(int usage);
	bool setupNormals(int usage);
	bool setupTexCoords(int usage);
	bool setupIndices(int indexMode);
	
//	void setupNormals(int usage);
//	void setupTextures(int usage);
	 
	void drawVertices();
	void drawWireframe();
	void drawFaces();
	
	void addTriangles(const vector<ofVec3f>& verts);
	void addTriangleFan(const vector<ofVec3f>& verts);
	void addTriangleStrip(const vector<ofVec3f>& verts);
	
	void addMeshVertices(const vector<ofVec3f>& verts);
	

	//TODO: update methods, generic update
	
    ofMeshElement* meshElement;
	bool bUseIndices;
	ofVbo vbo;
	int drawType;
};
