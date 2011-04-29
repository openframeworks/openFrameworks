// TODO:
// get face info properly
// use edge flags

#pragma once

#include "ofVec3f.h"
#include "ofVec2f.h"
#include "ofColor.h"
#include "ofUtils.h"
#include "ofConstants.h"
#include "ofGLUtils.h"

class ofMesh{
public:
	
	ofMesh();
	ofMesh(ofPrimitiveMode mode, const vector<ofVec3f>& verts);
	virtual ~ofMesh();
	
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
	void clearVertices();
	
	ofVec3f getNormal(int i);
	void addNormal(const ofVec3f& n);
	void addNormals(const vector<ofVec3f>& norms);
	void addNormals(const ofVec3f* norms, int amt);
	void removeNormal(int index);
	void setNormal(int index, const ofVec3f& n);
	void clearNormals();
	
	ofColor getColor(int i);
	void addColor(const ofColor& c);
	void addColors(const vector<ofColor>& cols);
	void addColors(const ofColor* cols, int amt);
	void removeColor(int index);
	void setColor(int index, const ofColor& c);
	void clearColors();
	
	ofVec2f getTexCoord(int i);
	void addTexCoord(const ofVec2f& t);
	void addTexCoords(const vector<ofVec2f>& tCoords);
	void addTexCoords(const ofVec2f* tCoords, int amt);
	void removeTexCoord(int index);
	void setTexCoord(int index, const ofVec2f& t);
	void clearTexCoords();
	
	ofIndexType getIndex(int i);
	void addIndex(ofIndexType i);
	void addIndices(const vector<ofIndexType>& inds);
	void addIndices(const ofIndexType* inds, int amt);
	void removeIndex(int i);
	void setIndex(int i, ofIndexType val);
	void clearIndices();
	
    void addTriangle(ofIndexType index1, ofIndexType index2, ofIndexType index3);
	
	int getNumVertices() const;
	int getNumColors() const;
	int getNumNormals() const;
	int getNumTexCoords() const;
	int getNumIndices() const;
	
	ofVec3f* getVerticesPointer();
	ofColor* getColorsPointer();
	ofVec3f* getNormalsPointer();
	ofVec2f* getTexCoordsPointer();
	ofIndexType* getIndexPointer();
	
	const float* getVerticesPointer() const;
	const float* getColorsPointer() const;
	const float* getNormalsPointer() const;
	const float* getTexCoordsPointer() const;
	const ofIndexType* getIndexPointer() const;

	vector<ofVec3f> & getVertices();
	vector<ofColor> & getColors();
	vector<ofVec3f> & getNormals();
	vector<ofVec2f> & getTexCoords();
	vector<ofIndexType> & getIndices();

	vector<int>& getFace(int faceId);
	
	void setName(string name_);
	
	bool haveVertsChanged();
	bool haveColorsChanged();
	bool haveNormalsChanged();
	bool haveTexCoordsChanged();
	bool haveIndicesChanged();
	
	friend std::ostream& operator<<(std::ostream& os, ofMesh& data);
	
	void drawVertices();
	void drawWireframe();
	void drawFaces();

protected:
	virtual void draw(ofPolyRenderMode renderType);

private:

	vector<ofVec3f> vertices;
	vector<ofColor> colors;
	vector<ofVec3f> normals;
	vector<ofVec2f> texCoords;
	vector<ofIndexType> indices;
	bool bVertsChanged, bColorsChanged, bNormalsChanged, bTexCoordsChanged, bIndicesChanged;
	ofPrimitiveMode mode;
	string name;
	
//	ofMaterial *mat;
};


inline std::ostream& operator<<(std::ostream& os, ofMesh data) {

	//TODO: update when ofMesh/primitives has been worked out
	os << "Vertices" << std::endl << "--------------------" << std::endl;
	for(int i = 0; i < data.getNumVertices(); ++i) {
		os << data.getVertex(i) << std::endl;
	}
	os << std::endl << std::endl;
	
	os << "Normals" << std::endl << "--------------------" << std::endl;
	for(int i = 0; i < data.getNumNormals(); ++i) {
		os << data.getNormal(i) << std::endl;
	}
	os << std::endl << std::endl;
		
	os << "TexCoords" << std::endl << "--------------------" << std::endl;
	for(int i = 0; i < data.getNumTexCoords(); ++i) {
		os << data.getTexCoord(i) << std::endl;
	}
	os << std::endl << std::endl;
		
	os << "Colors" << std::endl << "--------------------" << std::endl;
	for(int i = 0; i < data.getNumVertices(); ++i) {
		os << data.getVertex(i) << std::endl;
	}
	os << std::endl << std::endl;

	return os;
}

