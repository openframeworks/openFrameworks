//Keith Pasko, Memo Akten

// TODO:
// add/remove/edit vertices, 
// get face info
// make this smarter so we don't need wire indices, but can use edge flags
// figure out if we want to return references for getVertex etc, i.e. we want to allow them to access the info directly or not

#pragma once

#include "ofVec3f.h"
#include "ofVec2f.h"
#include "ofColor.h"
#include "ofUtils.h"

enum ofTriangleMode{
	OF_TRIANGLES_ELEMENT,
	OF_TRIANGLE_STRIP_ELEMENT,
	OF_TRIANGLE_FAN_ELEMENT,
	OF_LINE_LOOP_ELEMENT
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
		case OF_LINE_LOOP_ELEMENT:
			return GL_LINE_LOOP;
			break;
		default:
			return GL_TRIANGLES;
			break;
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
		case GL_LINE_LOOP:
			return OF_LINE_LOOP_ELEMENT;
			break;
		default:
			ofLog(OF_LOG_ERROR,"asked for non existant triangle mode " + ofToString(mode) + " returning GL_TRIANGLES");
			return OF_TRIANGLES_ELEMENT;
			break;
	}	
}

class ofMeshElement{
public:
	
	ofMeshElement();
	~ofMeshElement();
	
	void setMode(ofTriangleMode mode);
	ofTriangleMode getMode();
	
	void clear();

	void setupIndices();
//	void setupIndicesSolid();
//	void setupIndicesWire();
	
	ofVec3f getVertex(int i);
	void addVertex(const ofVec3f& v);
	void addVertices(const vector<ofVec3f>& verts);
	void removeVertex(int index);
	void setVertex(int index, const ofVec3f& v);
	
	ofVec3f getNormal(int i);
	void addNormal(const ofVec3f& n);
	void addNormals(const vector<ofVec3f>& norms);
	void removeNormal(int index);
	void setNormal(int index, const ofVec3f& n);
	
	ofColor getColor(int i);
	void addColor(const ofColor& c);
	void addColors(const vector<ofColor>& cols);
	void removeColor(int index);
	void setColor(int index, const ofColor& c);
	
	ofVec2f getTexCoord(int i);
	void addTexCoords(const vector<ofVec2f>& tCoords);
	void addTexCoord(const ofVec2f& t);
	void removeTexCoord(int index);
	void setTexCoord(int index, const ofVec2f& t);
	
	int getIndex(int i);
	void addIndex(int i);
	void addIndices(const vector<GLuint>& inds);
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
	
	vector<int>& getFace(int faceId);
	
	bool haveVertsChanged();
	bool haveColorsChanged();
	bool haveNormalsChanged();
	bool haveTexCoordsChanged();
	bool haveIndicesChanged();
	
protected:
	vector<ofVec3f> vertices;
	vector<ofColor> colors;
	vector<ofVec3f> normals;
	vector<ofVec2f> texCoords;
	vector<GLuint> indices;
	bool bVertsChanged, bColorsChanged, bNormalsChanged, bTexCoordsChanged, bIndicesChanged;
	ofTriangleMode mode;
	
//	vector<GLuint> indicesSolid;
//	vector<GLuint> indicesWire;
//	ofMaterial *mat;
};
