//
//  of3dPrimitives.h
//  openFrameworksLib
//
//  Created by Nick Hardeman on 9/14/12.
//
//
#pragma once

#include "ofVboMesh.h"
#include "ofRectangle.h"
#include "ofNode.h"
#include "ofTexture.h"
#include <map>


class of3dPrimitive : public ofNode {
public:
    of3dPrimitive();
    virtual ~of3dPrimitive();

    of3dPrimitive(const ofMesh & mesh);
    of3dPrimitive(const of3dPrimitive & mom);
    of3dPrimitive & operator=(const of3dPrimitive & mom);

    void mapTexCoords( float u1, float v1, float u2, float v2 );
    //void setTexCoords( int meshindex, float u1, float v1, float u2, float v2 );
    // does not store texture. Creates tex coords from texture, if texture is
    // non-arb, then it will create normalized tex coords //
    // defaults to index 0 
    void mapTexCoordsFromTexture( ofTexture& inTexture );
    
    
    ofMesh* getMeshPtr();
    ofMesh& getMesh();

    const ofMesh* getMeshPtr() const;
    const ofMesh& getMesh() const;
    
    ofVec4f* getTexCoordsPtr();
    ofVec4f& getTexCoords();
    
    const ofVec4f* getTexCoordsPtr() const;
    const ofVec4f& getTexCoords() const;

    bool hasScaling() const;
    bool hasNormalsEnabled() const;
    
    void enableNormals();
    void enableTextures();
    void enableColors();
    
    void disableNormals();
    void disableTextures();
    void disableColors();
    
    void removeMesh( int index );
    void removeTexture( int index );
    
    using ofNode::draw;
    void drawVertices() const;
    void drawWireframe() const;
    void drawFaces() const;
    void draw(ofPolyRenderMode renderType) const;
    void draw() const;
    void drawNormals( float length, bool bFaceNormals=false ) const;
    void drawAxes(float a_size) const;
    
    void setUseVbo(bool useVbo);
    bool isUsingVbo() const;
protected:
    
    // useful when creating a new model, since it uses normalized tex coords //
    void normalizeAndApplySavedTexCoords();
    
    ofVec4f texCoords;
    bool usingVbo;
    shared_ptr<ofMesh>  mesh;
    mutable ofMesh normalsMesh;
    
    vector<ofIndexType> getIndices( int startIndex, int endIndex ) const;
    
};

class ofPlanePrimitive : public of3dPrimitive {
public:
    ofPlanePrimitive();
    ofPlanePrimitive( float width, float height, int columns, int rows, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    ~ofPlanePrimitive();
    
    void set(float width, float height, int columns, int rows, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    void set( float width, float height );
    void resizeToTexture( ofTexture& inTexture, float scale=1.f );
    void setWidth( float width );
    void setHeight( float height );
    
    void setColumns( int columns );
    void setRows( int rows );
    void setResolution( int columns, int rows );
    void setMode( ofPrimitiveMode mode );
    
    int getNumColumns() const;
    int getNumRows() const;
    // x = columns, y = rows //
    ofVec2f getResolution() const;
    
    float getWidth() const;
    float getHeight() const;
    
protected:
    float width;
    float height;
    ofVec2f resolution;
};

class ofSpherePrimitive : public of3dPrimitive {
public:
    ofSpherePrimitive();
    ofSpherePrimitive( float radius, int res, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    ~ofSpherePrimitive();
    
    void set( float radius, int resolution, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    void setResolution( int res );
    void setRadius(float radius);
    void setMode( ofPrimitiveMode mode );
    
    float getRadius() const;
    int getResolution() const;
    
protected:
    float radius;
    int resolution;
};

class ofIcoSpherePrimitive : public of3dPrimitive {
public:
    ofIcoSpherePrimitive();
    ofIcoSpherePrimitive( float radius, int iterations );
    ~ofIcoSpherePrimitive();
    
    void set( float radius, int res );
    void setResolution( int iterations );
    void setRadius( float radius );
    void setMode( ofPrimitiveMode mode );
    
    float getRadius() const;
    int getResolution() const;
    
protected:
    float radius;
    int resolution;
};

class ofCylinderPrimitive : public of3dPrimitive {
public:
    ofCylinderPrimitive();
    ofCylinderPrimitive( float radius, float height, int radiusSegments, int heightSegments, int capSegments=2, bool bCapped = true,ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    ~ofCylinderPrimitive();
    
    void set( float radius, float height, int radiusSegments, int heightSegments, int capSegments=2, bool bCapped=true,ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    void set( float radius, float height, bool bCapped=true );
    void setRadius( float radius );
    void setHeight( float height );
    void setCapped( bool bCapped );
    
    void setResolutionRadius( int radiusRes );
    void setResolutionHeight( int heightRes );
    void setResolutionCap( int capRes );
    void setResolution( int radiusSegments, int heightSegments, int capSegments=2 );
    void setMode( ofPrimitiveMode mode );
    
    void setTopCapColor( ofColor color );
    void setCylinderColor( ofColor color );
    void setBottomCapColor( ofColor color );
    
    vector<ofIndexType> getTopCapIndices() const;
    ofMesh getTopCapMesh() const;
    vector<ofIndexType> getCylinderIndices() const;
    ofMesh getCylinderMesh() const;
    vector<ofIndexType> getBottomCapIndices() const;
    ofMesh getBottomCapMesh() const;
    
    int getResolutionRadius() const;
    int getResolutionHeight() const;
    int getResolutionCap() const;
    ofVec3f getResolution() const;
    
    float getHeight() const;
    float getRadius() const;
    bool getCapped() const;
protected:
    float radius;
    float height;
    bool bCapped;
    int strides[3][2];
    int vertices[3][2];
    ofVec3f resolution;
};

class ofConePrimitive : public of3dPrimitive {
public:
    
    ofConePrimitive();
    ofConePrimitive( float radius, float height, int radiusSegments, int heightSegments, int capSegments=2, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    ~ofConePrimitive();
    
    void set( float radius, float height, int radiusSegments, int heightSegments, int capSegments=2, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    void set( float radius, float height );
    void setResolutionRadius( int radiusRes );
    void setResolutionHeight( int heightRes );
    void setResolutionCap( int capRes );
    void setResolution( int radiusRes, int heightRes, int capRes );
    
    void setMode( ofPrimitiveMode mode );
    void setRadius( float radius );
    void setHeight( float height );
    
    void setTopColor( ofColor color );
    void setCapColor( ofColor color );
    
    vector<ofIndexType> getConeIndices() const;
    ofMesh getConeMesh() const;
    vector<ofIndexType> getCapIndices() const;
    ofMesh getCapMesh() const;
    
    int getResolutionRadius() const;
    int getResolutionHeight() const;
    int getResolutionCap() const;
    ofVec3f getResolution() const;
    
    float getRadius() const;
    float getHeight() const;
    
protected:
    float radius;
    float height;
    
    ofVec3f resolution;
    
    int strides[2][2];
    int vertices[2][2];
};

class ofBoxPrimitive : public of3dPrimitive {
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
    
    vector<ofIndexType> getSideIndices( int sideIndex ) const;
    ofMesh getSideMesh( int sideIndex ) const;
    
    void setResolution( int res ); // same resolution for all sides //
    void setResolutionWidth( int widthRes );
    void setResolutionHeight( int heightRes );
    void setResolutionDepth( int depthRes );
    void setResolution( int resWidth, int resHeight, int resDepth );
    
    void setMode( ofPrimitiveMode mode );
    void setSideColor( int sideIndex, ofColor color );
    
    int getResolutionWidth() const;
    int getResolutionHeight() const;
    int getResolutionDepth() const;
    ofVec3f getResolution() const;
    
    float getWidth() const;
    float getHeight() const;
    float getDepth() const;
    ofVec3f getSize() const;
protected:
    ofVec3f size;
    ofVec3f resolution;
    // indices strides for faces //
    int strides[6][2];
    int vertices[6][2];

};
















