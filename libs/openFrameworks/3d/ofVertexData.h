// TODO:
// get face info properly
// use edge flags

#pragma once

#include "ofVec3f.h"
#include "ofVec2f.h"
#include "ofColor.h"
#include "ofUtils.h"

enum ofPrimitiveMode{
	OF_TRIANGLES_MODE,
	OF_TRIANGLE_STRIP_MODE,
	OF_TRIANGLE_FAN_MODE,
	OF_LINES_MODE,
	OF_LINE_STRIP_MODE,
	OF_LINE_LOOP_MODE,
};

inline GLuint ofGetGLPrimitiveMode(ofPrimitiveMode mode){
	switch(mode){
		case OF_TRIANGLES_MODE:
			return GL_TRIANGLES;
			break;
		case OF_TRIANGLE_STRIP_MODE:
			return GL_TRIANGLE_STRIP;
			break;
		case OF_TRIANGLE_FAN_MODE:
			return GL_TRIANGLE_FAN;
			break;
		case OF_LINES_MODE:
			return GL_LINES;
			break;
		case OF_LINE_STRIP_MODE:
			return GL_LINE_STRIP;
			break;
		case OF_LINE_LOOP_MODE:
			return GL_LINE_LOOP;
			break;
		default:
			ofLog(OF_LOG_ERROR,"asked for unsupported or non existant primitive mode " + ofToString(mode) + " returning GL_TRIANGLES");
			return GL_TRIANGLES;
			break;
	}
}

inline ofPrimitiveMode ofGetOFPrimitiveMode(GLuint mode){
	switch(mode){
		case GL_TRIANGLES:
			return OF_TRIANGLES_MODE;
			break;
		case GL_TRIANGLE_STRIP:
			return OF_TRIANGLE_STRIP_MODE;
			break;
		case GL_TRIANGLE_FAN:
			return OF_TRIANGLE_FAN_MODE;
			break;
		case GL_LINES:
			return OF_LINES_MODE;
			break;
		case GL_LINE_STRIP:
			return OF_LINE_STRIP_MODE;
			break;
		case GL_LINE_LOOP:
			return OF_LINE_LOOP_MODE;
			break;
		default:
			ofLog(OF_LOG_ERROR,"asked for non existant primitive mode " + ofToString(mode) + " returning OF_TRIANGLES_MODE");
			return OF_TRIANGLES_MODE;
			break;
	}	
}

class ofVertexData{
public:
	
	ofVertexData();
	ofVertexData(ofPrimitiveMode mode, const vector<ofVec3f>& verts);
	~ofVertexData();
	
	void setMode(ofPrimitiveMode mode);
	ofPrimitiveMode getMode() const;
	
	void clear();

	void setupIndicesAuto();
	
	ofVec3f getVertex(int i);
	void addVertex(const ofVec3f& v);
	void addVertices(const vector<ofVec3f>& verts);
	void addVertices(const ofVec3f* verts, int amt);
	void removeVertex(int index);
	void setVertex(int index, const ofVec3f& v);
	
	ofVec3f getNormal(int i);
	void addNormal(const ofVec3f& n);
	void addNormals(const vector<ofVec3f>& norms);
	void addNormals(const ofVec3f* norms, int amt);
	void removeNormal(int index);
	void setNormal(int index, const ofVec3f& n);
	
	ofColor getColor(int i);
	void addColor(const ofColor& c);
	void addColors(const vector<ofColor>& cols);
	void addColors(const ofColor* cols, int amt);
	void removeColor(int index);
	void setColor(int index, const ofColor& c);
	
	ofVec2f getTexCoord(int i);
	void addTexCoord(const ofVec2f& t);
	void addTexCoords(const vector<ofVec2f>& tCoords);
	void addTexCoords(const ofVec2f* tCoords, int amt);
	void removeTexCoord(int index);
	void setTexCoord(int index, const ofVec2f& t);
	
	int getIndex(int i);
	void addIndex(int i);
	void addIndices(const vector<GLuint>& inds);
	void addIndices(const GLuint* inds, int amt);
	void removeIndex(int i);
	void setIndex(int i, int val);
	
	int getNumVertices() const;
	int getNumColors() const;
	int getNumNormals() const;
	int getNumTexCoords() const;
	int getNumIndices() const;
	
	float* getVerticesPointer();
	float* getColorsPointer();
	float* getNormalsPointer();
	float* getTexCoordsPointer();
	GLuint* getIndexPointer();
	
	const float* getVerticesPointer() const;
	const float* getColorsPointer() const;
	const float* getNormalsPointer() const;
	const float* getTexCoordsPointer() const;
	const GLuint* getIndexPointer() const;

	vector<int>& getFace(int faceId);
	
	void setName(string name_);
	
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
	ofPrimitiveMode mode;
	string name;
	
//	ofMaterial *mat;
};

