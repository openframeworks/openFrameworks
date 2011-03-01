#pragma once

#include "ofMesh.h"
#include "ofVbo.h"


enum meshRenderMethod{
	OF_MESH_USING_VBO,
	OF_MESH_USING_DEFAULT_RENDERER
};


class ofMeshRenderer{
public:
	ofMeshRenderer();
	~ofMeshRenderer();

	/*
	ofMeshRenderer(const ofMeshRenderer& v);
	ofMeshRenderer& operator=(const ofMeshRenderer& v);
	void clone(const ofMeshRenderer& v);
	*/
	
	void setMesh(ofMesh* m);
	
	ofMesh* getMesh();
	const ofMesh* getMesh() const;
	
	void setDrawType(int drawType);
	void setRenderMethod(meshRenderMethod m);
	
	bool enableColors();
	void disableColors();
	bool enableNormals();
	void disableNormals();
	bool enableTexCoords();
	void disableTexCoords();
	bool enableIndices();
	void disableIndices();
	
	bool getTexCoordsEnabled();
	bool getNormalsEnabled();
	bool getColorsEnabled();
	bool getIndicesEnabled();
	
	void draw(ofMesh* meshToDraw, ofPolyRenderMode mode);
	void drawVertices(ofMesh* meshToDraw);
	void drawWireframe(ofMesh* meshToDraw);
	void drawFaces(ofMesh* meshToDraw);
	
protected:
	bool setupVerticesForVbo(int id, int usage);
	bool setupColorsForVbo(int id, int usage);
	bool setupNormalsForVbo(int id, int usage);
	bool setupTexCoordsForVbo(int id, int usage);
	bool setupIndicesForVbo(int id, int usage);
	
	void setupVbos();
	void setupVertexArrays();
	
    ofMesh* mesh;
	vector<ofVbo> vbos;
	int drawType;	
	bool bEnableIndices, bEnableColors, bEnableNormals, bEnableTexCoords;
	meshRenderMethod renderMethod;
};
