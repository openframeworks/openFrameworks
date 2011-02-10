#pragma once

#include "ofVertexData.h"
#include "ofVbo.h"

enum polyMode{
	OF_MESH_POINTS,
	OF_MESH_WIREFRAME,
	OF_MESH_FILL
};

inline GLuint ofGetGLPolyMode(polyMode m){
#ifndef TARGET_OF_IPHONE  
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
#endif
}

inline polyMode ofGetOFPolyMode(GLuint m){
#ifndef TARGET_OF_IPHONE  
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
	#endif
}

enum meshRenderMethod{
	OF_MESH_USING_VBO,
	OF_MESH_USING_VERTEX_ARRAY
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
	
	
    ofVertexData* vertexData;
	ofVbo vbo;
	int drawType;
protected:	
	bool bEnableIndices, bEnableColors, bEnableNormals, bEnableTexCoords;
	meshRenderMethod renderMethod;
};
