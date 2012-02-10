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
	
	ofVec3f getVertex(int i) const;
	void addVertex(const ofVec3f& v);
	void addVertices(const vector<ofVec3f>& verts);
	void addVertices(const ofVec3f* verts, int amt);
	void removeVertex(int index);
	void setVertex(int index, const ofVec3f& v);
	void clearVertices();
	
	ofVec3f getNormal(int i) const;
	void addNormal(const ofVec3f& n);
	void addNormals(const vector<ofVec3f>& norms);
	void addNormals(const ofVec3f* norms, int amt);
	void removeNormal(int index);
	void setNormal(int index, const ofVec3f& n);
	void clearNormals();
	
	ofFloatColor getColor(int i) const;
	void addColor(const ofFloatColor& c);
	void addColors(const vector<ofFloatColor>& cols);
	void addColors(const ofFloatColor* cols, int amt);
	void removeColor(int index);
	void setColor(int index, const ofFloatColor& c);
	void clearColors();
	
	ofVec2f getTexCoord(int i) const;
	void addTexCoord(const ofVec2f& t);
	void addTexCoords(const vector<ofVec2f>& tCoords);
	void addTexCoords(const ofVec2f* tCoords, int amt);
	void removeTexCoord(int index);
	void setTexCoord(int index, const ofVec2f& t);
	void clearTexCoords();
	
	ofIndexType getIndex(int i) const;
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
	ofFloatColor* getColorsPointer();
	ofVec3f* getNormalsPointer();
	ofVec2f* getTexCoordsPointer();
	ofIndexType* getIndexPointer();
	
	const ofVec3f* getVerticesPointer() const;
	const ofFloatColor* getColorsPointer() const;
	const ofVec3f* getNormalsPointer() const;
	const ofVec2f* getTexCoordsPointer() const;
	const ofIndexType* getIndexPointer() const;

	vector<ofVec3f> & getVertices();
	vector<ofFloatColor> & getColors();
	vector<ofVec3f> & getNormals();
	vector<ofVec2f> & getTexCoords();
	vector<ofIndexType> & getIndices();

	const vector<ofVec3f> & getVertices() const;
	const vector<ofFloatColor> & getColors() const;
	const vector<ofVec3f> & getNormals() const;
	const vector<ofVec2f> & getTexCoords() const;
	const vector<ofIndexType> & getIndices() const;

	vector<int>& getFace(int faceId);
	
	ofVec3f getCentroid() const;

	void setName(string name_);

	bool haveVertsChanged();
	bool haveColorsChanged();
	bool haveNormalsChanged();
	bool haveTexCoordsChanged();
	bool haveIndicesChanged();
	
	bool hasVertices();
	bool hasColors();
	bool hasNormals();
	bool hasTexCoords();
	bool hasIndices();
	
	void drawVertices();
	void drawWireframe();
	void drawFaces();
	void draw();


	void load(string path);
	void save(string path, bool useBinary = false);

protected:
	virtual void draw(ofPolyRenderMode renderType);

private:

	vector<ofVec3f> vertices;
	vector<ofFloatColor> colors;
	vector<ofVec3f> normals;
	vector<ofVec2f> texCoords;
	vector<ofIndexType> indices;
	bool bVertsChanged, bColorsChanged, bNormalsChanged, bTexCoordsChanged, bIndicesChanged;
	ofPrimitiveMode mode;
	string name;
	
//	ofMaterial *mat;
};
