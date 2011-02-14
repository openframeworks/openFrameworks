#pragma once

#include "ofMesh.h"
#include "ofVbo.h"

enum polyMode{
	OF_MESH_POINTS,
	OF_MESH_WIREFRAME,
	OF_MESH_FILL
};

inline GLuint ofGetGLPolyMode(polyMode m){
#ifndef TARGET_OPENGLES
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
#else
	return 0;
#endif
}

inline polyMode ofGetOFPolyMode(GLuint m){
#ifndef TARGET_OPENGLES
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
#else
	return OF_MESH_FILL;
#endif
}

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
	
	void draw(ofMesh* meshToDraw, polyMode mode);
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
