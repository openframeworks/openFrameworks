//Keith Pasko, Memo Akten

// TODO:
// add/remove/edit vertices, 
// get face info
// make this smarter so we don't need wire indices, but can use edge flags

#pragma once

#include "ofVec3f.h"
#include "ofVec2f.h"
#include "ofColor.h"
#include "ofUtils.h"

enum ofTriangleMode{
	OF_TRIANGLES_ELEMENT,
	OF_TRIANGLE_STRIP_ELEMENT,
	OF_TRIANGLE_FAN_ELEMENT,
};	

inline GLuint ofGetGLTriangleMode(ofTriangleMode mode){
	switch(mode){
	case OF_TRIANGLES_ELEMENT:
		return GL_TRIANGLES;
		break;
	case OF_TRIANGLE_STRIP_ELEMENT:
		return GL_TRIANGLE_STRIP;
		break;
	case OF_TRIANGLE_FAN_ELEMENT:
		return GL_TRIANGLE_FAN;
		break;
	default:
		return GL_TRIANGLES;
	}
}

inline ofTriangleMode ofGetOFTriangleMode(GLuint mode){
	switch(mode){
	case GL_TRIANGLES:
		return OF_TRIANGLES_ELEMENT;
		break;
	case GL_TRIANGLE_STRIP:
		return OF_TRIANGLE_STRIP_ELEMENT;
		break;
	case GL_TRIANGLE_FAN:
		return OF_TRIANGLE_FAN_ELEMENT;
		break;
	default:
		ofLogError("of") << "asked for non existant triangle mode " << mode << " returning GL_TRIANGLES";
		return OF_TRIANGLES_ELEMENT;
	}
}

class ofMeshElement{
public:
	
	ofMeshElement();
	~ofMeshElement();
	
	void setMode(ofTriangleMode mode);
	ofTriangleMode getMode();

	void setupIndices();
//	void setupIndicesSolid();
//	void setupIndicesWire();
	
	void addVertex(const ofVec3f& v);
	void addVertices(const vector<ofVec3f> & vertices);
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
	
	int getNumVertices() const;
	int getNumColors() const;
	int getNumNormals() const;
	int getNumTexCoords() const;
	int getNumIndices() const;
//	int getNumIndicesSolid();
//	int getNumIndicesWire();
	
	ofVec3f* getVerticesPointer();
	ofColor* getColorsPointer();
	ofVec3f* getNormalsPointer();
	ofVec2f* getTexCoordsPointer();
	GLuint* getIndexPointer();
//	GLuint* getSolidIndexPointer();
//	GLuint* getWireIndexPointer();
	
	void clear();

	vector<int>& getFace(int faceId);
	
	bool hasChanged();

protected:
	vector<ofVec3f> vertices;
	vector<ofColor> colors;
	vector<ofVec3f> normals;
	vector<ofVec2f> texCoords;
	vector<GLuint> indices;
//	vector<GLuint> indicesSolid;
//	vector<GLuint> indicesWire;
	ofTriangleMode mode;
	bool bHasChanged;
	//ofMaterial *mat;
};
