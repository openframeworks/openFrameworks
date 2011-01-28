#pragma once

#include "ofVertexData.h"
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
	
	void setVertexData(ofVertexData* m);
	void setUseIndices(bool useIndices=true);
	
	ofVertexData* getVertexData();
	const ofVertexData* getVertexData() const;
	
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
	
	void update();
	
	void enableColors();
	void disableColors();
	void enableNormals();
	void disableNormals();
	void enableTexCoords();
	void disableTexCoords();
	
    ofVertexData* vertexData;
	bool bEnableIndices, bEnableColors, bEnableNormals, bEnableTexCoords;
	ofVbo vbo;
	int drawType;
};
