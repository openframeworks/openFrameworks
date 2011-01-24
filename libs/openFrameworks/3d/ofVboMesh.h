//Keith Pasko, Memo Akten

#pragma once

#include "ofMeshElement.h"
#include "ofVbo.h"

//enum for drawing mode status
enum polyMode{
	OF_MESH_POINTS = GL_POINT,
	OF_MESH_WIREFRAME = GL_LINE,
	OF_MESH_FILL = GL_FILL
};


class ofVboMesh{
public:
	ofVboMesh();
	~ofVboMesh();
	
	/*
	ofVboMesh(const ofVboMesh& v);
	ofVboMesh& operator=(const ofVboMesh& v);
	void clone(const ofVboMesh& v);
	*/
	
	void setMeshElement(ofMeshElement* m);
	void setUseIndices(bool useIndices=true);
	
	ofMeshElement* getMeshElement();
	const ofMeshElement* getMeshElement() const;
	
	void setDrawType(int drawType);
	
	bool setupVertices(int usage);
	bool setupColors(int usage);
	bool setupNormals(int usage);
	bool setupTexCoords(int usage);
	bool setupIndices(int usage);
	 
	void drawVertices();
	void drawWireframe();
	void drawFaces();
	void draw(polyMode mode);
	
	void addTriangles(const vector<ofVec3f>& verts);
	void addTriangleFan(const vector<ofVec3f>& verts);
	void addTriangleStrip(const vector<ofVec3f>& verts);
	void addMeshVertices(const vector<ofVec3f>& verts);
	
	void update();
	
    ofMeshElement* meshElement;
	bool bUseIndices;
	ofVbo vbo;
	int drawType;
};
