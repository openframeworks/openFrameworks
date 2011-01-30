#pragma once

#include "ofVertexData.h"
#include "ofVbo.h"

enum polyMode{
	OF_MESH_POINTS,
	OF_MESH_WIREFRAME,
	OF_MESH_FILL
};

inline GLuint ofGetGLPolyMode(polyMode m){
	switch(m){
		case(OF_MESH_POINTS):
			return GL_POINT;
			break;
		case(OF_MESH_WIREFRAME):
			return GL_LINE;	
			break;
		case(OF_MESH_FILL):
			return GL_FILL;	
			break;
		default:
			ofLog(OF_LOG_ERROR,"asked for unsupported or non existant poly mode " + ofToString(m) + " returning GL_FILL");
			return GL_FILL;
			break;
	}
}

inline polyMode ofGetOFPolyMode(GLuint m){
	switch(m){
		case(GL_POINT):
			return OF_MESH_POINTS;
			break;
		case(GL_LINE):
			return OF_MESH_WIREFRAME;	
			break;
		case(GL_FILL):
			return OF_MESH_FILL;	
			break;
		default:
			ofLog(OF_LOG_ERROR,"asked for non existant glPolygonMode " + ofToString(m) + " returning OF_MESH_FILL");
			return OF_MESH_FILL;
			break;
	}
}

enum meshRenderMethod{
	OF_MESH_USING_VBO,
	OF_MESH_USING_DEFAULT_RENDERER
};


class ofMesh{
public:
	ofMesh();
	~ofMesh();
	
	/*
	ofMesh(const ofMesh& v);
	ofMesh& operator=(const ofMesh& v);
	void clone(const ofMesh& v);
	*/
	
	void setVertexData(ofVertexData* m);
	void setUseIndices(bool useIndices=true);
	
	ofVertexData* getVertexData();
	const ofVertexData* getVertexData() const;
	
	void setDrawType(int drawType);
	void setRenderMethod(meshRenderMethod m);
	
	bool setupVerticesForVbo(int usage);
	bool setupColorsForVbo(int usage);
	bool setupNormalsForVbo(int usage);
	bool setupTexCoordsForVbo(int usage);
	bool setupIndicesForVbo(int usage);
	 
	void drawVertices();
	void drawWireframe();
	void drawFaces();
	void draw(polyMode mode);
	
	void setupVbo();
	void setupVertexArray();
	
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
	meshRenderMethod renderMethod;
};
