//
//  of3dModel.h
//  openFrameworksLib
//
//  Created by Nick Hardeman on 9/14/12.
//
//
#pragma once

#include "ofMesh.h"
#include "ofRectangle.h"
#include "ofNode.h"
#include "ofTexture.h"
#include <map>

enum of3dPrimitiveType {
	OF_3D_PRIMITIVE_PLANE,
    OF_3D_PRIMITIVE_SPHERE,
    OF_3D_PRIMITIVE_ICO_SPHERE,
	OF_3D_PRIMITIVE_BOX,
	OF_3D_PRIMITIVE_CONE,
    OF_3D_PRIMITIVE_CYLINDER
};

class of3dTriangle {
public:
    of3dTriangle() {
        bHasNormals = bHasColors = bHasTexcoords = false;
    }
    
    ofPoint points[3];
    ofVec3f faceNormal;
    ofVec3f normals[3];
    ofFloatColor colors[3];
    ofVec2f texcoords[3];
    
    void setHasColors( bool bColors ) {bHasColors = bColors; }
    void setHasNormals( bool bNormals ) {bHasNormals = bNormals; }
    void setHasTexcoords( bool bTexcoords ) {bHasTexcoords = bTexcoords; }
    
    bool hasColors() { return bHasColors; }
    bool hasNormals() {return bHasNormals; }
    bool hasTexcoords() { return bHasTexcoords; }
    
protected:
    bool bHasNormals, bHasColors, bHasTexcoords;
    
};


class ofPrimitiveBase : public ofNode {
public:
    ofPrimitiveBase();
    virtual ~ofPrimitiveBase();
    
    // applies to all the meshes evenly //
    void setTexCoords( float u1, float v1, float u2, float v2 );
    // apply to a specific mesh //
    //void setTexCoords( int meshindex, float u1, float v1, float u2, float v2 );
    // does not store texture. Creates tex coords from texture, if texture is
    // non-arb, then it will create normalized tex coords //
    // defaults to index 0 
    void setTexCoordsFromTexture( ofTexture& inTexture );
    // useful when creating a new model, since it uses normalized tex coords //
    void normalizeAndApplySavedTexCoords();
    
    ofMesh* getMeshPtr();
    ofMesh& getMesh();
    
    int getNumTexcoords();
    ofVec4f* getTexCoordsPtr();
    ofVec4f& getTexCoords();
    
    bool hasScaling();
    bool hasNormalsEnabled();
    ofVec3f getResolution() const;
    
    void mergeDuplicateVerticies();
    
    void enableNormals();
    void enableTextures();
    void enableColors();
    
    void disableNormals();
    void disableTextures();
    void disableColors();
    
    // return a list of triangles that do not share verticies or indicies //
    ofVec3f getFaceNormal(of3dTriangle& tri);
    vector<of3dTriangle> getUniqueTriangles();
    vector<ofVec3f> getFaceNormals( bool perVetex=false);
    void setFromTriangles( vector<of3dTriangle>& tris, bool bUseFaceNormal=false );
    
    void smoothNormals( float angle );
    
    void setResolution( int resX, int resY, int resZ );
    
    void removeMesh( int index );
    void removeTexture( int index );
    void clear();
    
    void drawVertices();
    void drawWireframe();
    void drawFaces();
    void draw();
    void draw(ofPolyRenderMode renderType);
    void drawNormals( float length, bool bFaceNormals=false );
    void drawAxes(float a_size);
    
protected:
    ofMesh  _mesh;
    ofVec4f _texCoords;
    
    ofVec3f _resolution;
    vector<ofIndexType> getIndicies( int startIndex, int endIndex );
    void setColorForIndicies( int startIndex, int endIndex, ofColor color );
    ofMesh getMeshForIndexes( int startIndex, int endIndex, int startVertIndex, int endVertIndex );
};

class ofPlanePrimitive : public ofPrimitiveBase {
public:
    ofPlanePrimitive();
    ofPlanePrimitive( float width, float height, int columns, int rows, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    ~ofPlanePrimitive();
    
    void set(float width, float height, int columns, int rows, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    void set( float width, float height );
    void resizeToTexture( ofTexture& inTexture, float scale=1.f );
    void setWidth( float width );
    void setHeight( float height );
    
    void setResolution( int columns, int rows );
    void setResolution(int resX, int resY, int resZ);
    void setMode( ofPrimitiveMode mode );
    
    float getWidth();
    float getHeight();
    
protected:
    float _width;
    float _height;
};

class ofSpherePrimitive : public ofPrimitiveBase {
public:
    ofSpherePrimitive();
    ofSpherePrimitive( float radius, int res, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    ~ofSpherePrimitive();
    
    void set( float radius, int resolution, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    void setResolution( int res );
    void setResolution( int resX, int resY, int resZ );
    
    void setMode( ofPrimitiveMode mode );
    
    void setRadius(float radius);
    float getRadius();
    
protected:
    float _radius;
};

class ofIcoSpherePrimitive : public ofPrimitiveBase {
public:
    ofIcoSpherePrimitive();
    ofIcoSpherePrimitive( float radius, int iterations );
    ~ofIcoSpherePrimitive();
    
    void set(float radius, int res );
    void setResolution( int iterations );
    void setResolution( int resX, int resY, int resZ );
    void setRadius(float radius);
    void setMode( ofPrimitiveMode mode );
    
    float getRadius();
    
protected:
    float _radius;
};

class ofCylinderPrimitive : public ofPrimitiveBase {
public:
    ofCylinderPrimitive();
    ofCylinderPrimitive( float radius, float height, int radiusSegments, int heightSegments, int numCapSegments=2, bool bCapped = true,ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    ~ofCylinderPrimitive();
    
    void set( float radius, float height, int radiusSegments, int heightSegments, int numCapSegments=2, bool bCapped=true,ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    void set( float radius, float height, bool bCapped=true );
    void setRadius( float radius );
    void setHeight( float height );
    void setCapped( bool bCapped );
    
    void setResolution( int radiusSegments, int heightSegments, int capSegments=2 );
    void setMode( ofPrimitiveMode mode );
    
    void setTopCapColor( ofColor color );
    void setCylinderColor( ofColor color );
    void setBottomCapColor( ofColor color );
    
    vector<ofIndexType> getTopCapIndicies();
    ofMesh getTopCapMesh();
    vector<ofIndexType> getCylinderIndicies();
    ofMesh getCylinderMesh();
    vector<ofIndexType> getBottomCapIndicies();
    ofMesh getBottomCapMesh();
    
    float getHeight();
    float getRadius();
    bool getCapped();
protected:
    float _radius;
    float _height;
    bool _bCapped;
    int _strides[3][2];
    int _verticies[3][2];
};

class ofConePrimitive : public ofPrimitiveBase {
public:
    
    ofConePrimitive();
    ofConePrimitive( float radius, float height, int radiusSegments, int heightSegments, int capSegments=2, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    ~ofConePrimitive();
    
    void set( float radius, float height, int radiusSegments, int heightSegments, int capSegments=2, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    void set( float radius, float height );
    void setResolution( int radiusSegments, int heightSegments );
    void setResolution( int resX, int resY, int resZ );
    void setMode( ofPrimitiveMode mode );
    void setRadius( float radius );
    void setHeight( float height );
    
    void setTopColor( ofColor color );
    void setCapColor( ofColor color );
    
    vector<ofIndexType> getConeIndicies();
    ofMesh getConeMesh();
    vector<ofIndexType> getCapIndicies();
    ofMesh getCapMesh();
    
    
    float getRadius();
    float getHeight();
    
protected:
    float _radius;
    float _height;
    
    int _strides[2][2];
    int _verticies[2][2];
};

class ofBoxPrimitive : public ofPrimitiveBase {
public:
    
    enum BoxSides {
        SIDE_FRONT,
        SIDE_RIGHT,
        SIDE_LEFT,
        SIDE_BACK,
        SIDE_TOP,
        SIDE_BOTTOM,
        SIDES_TOTAL
    };
    
    ofBoxPrimitive();
    ofBoxPrimitive( float width, float height, float depth, int resWidth=2, int resHeight=2, int resDepth=2 );
    ~ofBoxPrimitive();
    
    void set( float width, float height, float depth, int resWidth, int resHeight, int resDepth);
    void set( float width, float height, float depth );
    void set( float size ); // all sides the same dimensions //
    
    void setWidth( float a_width );
    void setHeight( float a_height );
    void setDepth( float a_depth );
    
    void resizeToTexture( ofTexture& inTexture );
    
    vector<ofIndexType> getSideIndicies( int sideIndex );
    ofMesh getSideMesh( int sideIndex );
    
    void setResolution( int res ); // same resolution for all sides //
    void setResolution( int resX, int resY, int resZ );
    void setMode( ofPrimitiveMode mode );
    void setSideColor( int sideIndex, ofColor color );
    
    float getWidth();
    float getHeight();
    float getDepth();
    ofVec3f getSize() const;
protected:
    ofVec3f _size;
    // indicies strides for faces //
    int _strides[6][2];
    int _verticies[6][2];
};


ofMesh      ofGetPlaneMesh(float width, float height, int columns, int rows, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP);
ofMesh      ofGetSphereMesh(float radius, int res, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
ofMesh      ofGetIcosahedronMesh(float radius);
ofMesh      ofGetIcoSphereMesh(float radius, int iterations);
ofMesh      ofGetCylinderMesh( float radius, float height, int radiusSegments, int heightSegments, int numCapSegments=2, bool bCapped = true, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP  );
ofMesh      ofGetConeMesh( float radius, float height, int radiusSegments, int heightSegments, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
ofMesh      ofGetBoxMesh( float width, float height, float depth, int resX, int resY, int resZ );















