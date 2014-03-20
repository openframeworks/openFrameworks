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

class ofMeshFace;

class ofMesh{
public:
	
	ofMesh();
	ofMesh(ofPrimitiveMode mode, const vector<ofVec3f>& verts);
	virtual ~ofMesh();
	
	void setMode(ofPrimitiveMode mode);
	ofPrimitiveMode getMode() const;
	
	void clear();

	void setupIndicesAuto();
	
	ofVec3f getVertex(ofIndexType i) const;
	void addVertex(const ofVec3f& v);
	void addVertices(const vector<ofVec3f>& verts);
	void addVertices(const ofVec3f* verts, int amt);
	void removeVertex(ofIndexType index);
	void setVertex(ofIndexType index, const ofVec3f& v);
	void clearVertices();
	
	ofVec3f getNormal(ofIndexType i) const;
	void addNormal(const ofVec3f& n);
	void addNormals(const vector<ofVec3f>& norms);
	void addNormals(const ofVec3f* norms, int amt);
	void removeNormal(ofIndexType index);
	void setNormal(ofIndexType index, const ofVec3f& n);
	void clearNormals();
	
	ofFloatColor getColor(ofIndexType i) const;
	void addColor(const ofFloatColor& c);
	void addColors(const vector<ofFloatColor>& cols);
	void addColors(const ofFloatColor* cols, int amt);
	void removeColor(ofIndexType index);
	void setColor(ofIndexType index, const ofFloatColor& c);
	void clearColors();
	
	ofVec2f getTexCoord(ofIndexType i) const;
	void addTexCoord(const ofVec2f& t);
	void addTexCoords(const vector<ofVec2f>& tCoords);
	void addTexCoords(const ofVec2f* tCoords, int amt);
	void removeTexCoord(ofIndexType index);
	void setTexCoord(ofIndexType index, const ofVec2f& t);
	void clearTexCoords();
	
	ofIndexType getIndex(ofIndexType i) const;
	void addIndex(ofIndexType i);
	void addIndices(const vector<ofIndexType>& inds);
	void addIndices(const ofIndexType* inds, int amt);
	void removeIndex(ofIndexType index);
	void setIndex(ofIndexType index, ofIndexType val);
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

	bool haveVertsChanged();
	bool haveColorsChanged();
	bool haveNormalsChanged();
	bool haveTexCoordsChanged();
	bool haveIndicesChanged();
	
	bool hasVertices() const;
	bool hasColors() const;
	bool hasNormals() const;
	bool hasTexCoords() const;
	bool hasIndices() const;
	
	void drawVertices();
	void drawWireframe();
	void drawFaces();
	void draw();

	void load(string path);
	void save(string path, bool useBinary = false) const;
    
    virtual void enableColors();
    virtual void enableTextures();
    virtual void enableNormals();
    virtual void enableIndices();
    
    virtual void disableColors();
    virtual void disableTextures();
    virtual void disableNormals();
    virtual void disableIndices();
    
    virtual bool usingColors() const;
    virtual bool usingTextures() const;
    virtual bool usingNormals() const;
    virtual bool usingIndices() const;

    void append(ofMesh & mesh);
    
    void setColorForIndices( int startIndex, int endIndex, ofColor color );
    ofMesh getMeshForIndices( int startIndex, int endIndex ) const;
    ofMesh getMeshForIndices( int startIndex, int endIndex, int startVertIndex, int endVertIndex ) const;
    void mergeDuplicateVertices();
    // return a list of triangles that do not share vertices or indices //
    const vector<ofMeshFace> & getUniqueFaces() const;
    vector<ofVec3f> getFaceNormals( bool perVetex=false) const;
    void setFromTriangles( const vector<ofMeshFace>& tris, bool bUseFaceNormal=false );
    void smoothNormals( float angle );
    
    static ofMesh plane(float width, float height, int columns=2, int rows=2, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP);
    static ofMesh sphere(float radius, int res=12, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP);
    static ofMesh icosahedron(float radius);
    static ofMesh icosphere(float radius, int iterations=2);
    static ofMesh cylinder(float radius, float height, int radiusSegments=12, int heightSegments=6, int numCapSegments=2, bool bCapped = true, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP);
    static ofMesh cone(float radius, float height, int radiusSegments=12, int heightSegments=6, int capSegments=2, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP);
    static ofMesh box(float width, float height, float depth, int resX=2, int resY=2, int resZ=2);
    static ofMesh axis(float size=1.0);
	
	virtual void draw(ofPolyRenderMode renderType);

private:

	vector<ofVec3f> vertices;
	vector<ofFloatColor> colors;
	vector<ofVec3f> normals;
	vector<ofVec2f> texCoords;
	vector<ofIndexType> indices;

	// this variables are only caches and returned always as const
	// mutable allows to change them from const methods
	mutable vector<ofMeshFace> faces;
	mutable bool bFacesDirty;

	bool bVertsChanged, bColorsChanged, bNormalsChanged, bTexCoordsChanged, bIndicesChanged;
	ofPrimitiveMode mode;
    
    bool useColors;
    bool useTextures;
    bool useNormals;
    bool useIndices;
	
//	ofMaterial *mat;
};

// this is always a triangle //
class ofMeshFace {
public:
    ofMeshFace();

    const ofVec3f & getFaceNormal() const;

    void setVertex( int index, const ofVec3f& v );
    const ofVec3f& getVertex( int index ) const;

    void setNormal( int index, const ofVec3f& n );
    const ofVec3f& getNormal( int index ) const;

    void setColor( int index, const ofFloatColor& color );
    const ofFloatColor& getColor(int index) const;

    void setTexCoord( int index, const ofVec2f& tCoord );
    const ofVec2f& getTexCoord( int index ) const;

    void setHasColors( bool bColors );
    void setHasNormals( bool bNormals );
    void setHasTexcoords( bool bTexcoords );

    bool hasColors() const;
    bool hasNormals() const;
    bool hasTexcoords() const;

private:
    void calculateFaceNormal() const;
    bool bHasNormals, bHasColors, bHasTexcoords;

	// this variables are only caches and returned always as const
	// mutable allows to change them from const methods
    mutable bool bFaceNormalDirty;
    mutable ofVec3f faceNormal;
    ofVec3f vertices[3];
    ofVec3f normals[3];
    ofFloatColor colors[3];
    ofVec2f texCoords[3];
};
