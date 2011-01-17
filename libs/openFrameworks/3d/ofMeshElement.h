//Keith Pasko, Memo Akten

// TODO:
// add/remove/edit vertices, 
// get face info
// make this smarter so we don't need wire indices, but can use edge flags

#pragma once

#include "ofVec3f.h"
#include "ofVec2f.h"
#include "ofColor.h"

enum{
	OF_TRIANGLES_ELEMENT,
	OF_TRIANGLE_STRIP_ELEMENT,
	OF_TRIANGLE_FAN_ELEMENT,
};	

class ofMeshElement{
public:
	
	ofMeshElement();
	~ofMeshElement();
	
	void setMode(int mode);

	void setupIndices();
//	void setupIndicesSolid();
//	void setupIndicesWire();
	
	void addVertex(const ofVec3f& v);
	void removeVertex(int index);
	void setVertex(int index, const ofVec3f& v);
	
	void addNormal(const ofVec3f& v);
	void removeNormal(int index);
	void setNormal(int index, const ofVec3f& v);
	
	void addColor(const ofColor& c);
	void removeColor(int index);
	void setColor(int index, const ofColor& v);
	
	void addTexCoord(const ofVec2f& t);
	void removeTexCoord(int index);
	void setTexCoord(int index, const ofVec2f& v);
	
	void addIndex(int i);
	void removeIndex(int i);
	void setIndex(int i, int val);
	
	int getNumVertices();
	int getNumColors();
	int getNumNormals();
	int getNumTexCoords();
	int getNumIndices();
//	int getNumIndicesSolid();
//	int getNumIndicesWire();
	
	ofVec3f* getVerticesPointer();
	ofColor* getColorsPointer();
	ofVec3f* getNormalsPointer();
	ofVec2f* getTexCoordsPointer();
	GLuint* getIndexPointer();
//	GLuint* getSolidIndexPointer();
//	GLuint* getWireIndexPointer();
	
	vector<int>& getFace(int faceId);
	
protected:
	vector<ofVec3f> vertices;
	vector<ofColor> colors;
	vector<ofVec3f> normals;
	vector<ofVec2f> texCoords;
	vector<GLuint> indices;
//	vector<GLuint> indicesSolid;
//	vector<GLuint> indicesWire;
	int mode;
	//ofMaterial *mat;
};
