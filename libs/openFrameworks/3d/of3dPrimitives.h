#pragma once

#include "ofMesh.h"
#include "ofNode.h"
#include <map>

class ofTexture;
class ofVboMesh;
class ofRectangle;

/// \brief A class representing a 3d primitive.
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
    void mapTexCoordsFromTexture( const ofTexture& inTexture );


    ofMesh* getMeshPtr();
    ofMesh& getMesh();

    const ofMesh* getMeshPtr() const;
    const ofMesh& getMesh() const;

	glm::vec4* getTexCoordsPtr();
	glm::vec4& getTexCoords();

	const glm::vec4* getTexCoordsPtr() const;
	const glm::vec4& getTexCoords() const;

    bool hasScaling() const;
    bool hasNormalsEnabled() const;

    void enableNormals();
    void enableTextures();
    void enableColors();

    void disableNormals();
    void disableTextures();
    void disableColors();

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

	glm::vec4 texCoords;
    bool usingVbo;
    std::shared_ptr<ofMesh>  mesh;
    mutable ofMesh normalsMesh;

    std::vector<ofIndexType> getIndices( int startIndex, int endIndex ) const;

};


/// \brief The ofPlanePrimitive allows you to create an UV plane.
/// Can be used for example as simple canvas for projecting simple texture .
/// 
/// Like all primitives it allows you to set the size, in this case height 
/// and width, draw it, set positions, etc, as a simple example:
/// 
/// ~~~~{.cpp}
/// 
/// ofPlanePrimitive plane;
/// 
/// void setup()
/// {
/// /// let's say we have a window set to be at 
/// // resolution 640x480...
/// 
///     plane.set(640, 480);   ///dimensions for width and height in pixels
/// plane.setPosition(320, 240, 0); /// position in x y z
/// plane.setResolution(2, 2); /// this resolution (as columns and rows) is enough
/// }
/// 
/// void draw()
/// {
/// plane.drawWireframe();
/// }
/// ~~~~
/// As you can see, plane is constructed from two triangles.
/// To draw a texture over any primitive, simply bind the ofTexture instance 
/// and then draw your primitive:
/// 
/// ~~~~{.cpp}
/// 
/// texture.getTextureReference().bind();
/// // now draw filled...
/// plane.draw();
/// 
/// ~~~~
/// 
/// The plane primitive also allows you to simply pass an ofTexture to the 
/// plane and generate the texture coordinates from that texture so that the 
/// ofTexture fills the plane when it's drawn. This saves you the hassle of 
/// creating all the texture coordinates for each vertex, which is nice.

class ofPlanePrimitive : public of3dPrimitive {
public:
    ofPlanePrimitive();
    ofPlanePrimitive( float width, float height, int columns, int rows, 
        ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    ~ofPlanePrimitive();

    void set(float width, float height, int columns, int rows, 
        ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
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
	glm::vec2 getResolution() const;

    float getWidth() const;
    float getHeight() const;

protected:
    float width;
    float height;
	glm::vec2 resolution;
};


/// \brief The ofSpherePrimitive allows you to create a UV sphere.
/// A UV spherewhich is a sphere based on UV slices rather than faces like 
/// the ofIcoSpherePrimitive. Note this:
/// 
/// [IMG](3d/spheretypes.png)
/// 
/// On the left is a UV sphere and on the right is an ICO sphere.
/// 
/// Like all primitives it allows you to set the size (radius), draw it, 
/// set positions, etc, as a simple example:
/// 
/// ~~~~{.cpp}
/// 
/// void setup()
/// {
/// 
///     sphere.setRadius( width );
/// }
/// 
/// void draw()
/// {
/// 
///     sphere.setPosition(ofGetWidth()*.2, ofGetHeight()*.75, 0);
///     sphere.rotate(spinX, 1.0, 0.0, 0.0);
///     sphere.rotate(spinY, 0, 1.0, 0.0);
/// 
///     // get all the faces from the icoSphere, handy when you want to copy
///     // individual vertices or tweak them a little ;)
///     vector<ofMeshFace> triangles = sphere.getMesh().getUniqueFaces();
/// 
///     // now draw
///     sphere.draw();
/// }
/// ~~~~
/// 
/// To draw a texture over any primitive, simply bind the ofTexture instance 
/// and then draw your primitive:
/// 
/// ~~~~{.cpp}
/// 
/// texture.getTextureReference().bind();
/// // now draw
/// sphere.draw();
/// 
/// ~~~~
/// 
/// The sphere can look a little weird if you don't do ofEnableDepthTest() 
/// and ofDisableAlphaBlending().
/// 

class ofSpherePrimitive : public of3dPrimitive {
public:
    ofSpherePrimitive();
    ofSpherePrimitive( float radius, int res, 
        ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    ~ofSpherePrimitive();

    void set( float radius, int resolution, 
        ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    void setResolution( int res );
    void setRadius(float radius);
    void setMode( ofPrimitiveMode mode );

    float getRadius() const;
    int getResolution() const;

protected:
    float radius;
    int resolution;
};

/// \brief The ofIcoSpherePrimitive allows you to create an icosphere. 
/// An icosphere is a sphere not based on UV slices like the ofSpherePrimitive,
/// but rather faces. Like this:
/// 
/// Like all primitives it allows you to set the radius, 
/// draw it, set positions, etc.
/// 
/// ~~~~{.cpp}
/// 
/// void setup()
/// {
/// 
///     icoSphere.setRadius( width );
/// }
/// 
/// void draw()
/// {
/// 
///     icoSphere.setPosition(ofGetWidth()*.2, ofGetHeight()*.75, 0);
///     icoSphere.rotate(spinX, 1.0, 0.0, 0.0);
///     icoSphere.rotate(spinY, 0, 1.0, 0.0);
/// 
///     // get all the faces from the icoSphere, handy when you want to copy
///     // individual vertices or tweak them a little ;)
///     vector<ofMeshFace> triangles = icoSphere.getMesh().getUniqueFaces();
/// 
///     // now draw
///     icoSphere.draw();
/// }
/// ~~~~

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


/// \brief The ofCylinderPrimitive allows you to create an cylinder mesh.
/// 
/// Like all primitives it allows you to set the size, for the cylinder 
/// a radius and height, draw it, set positions, etc, as a simple example:
/// 
/// ~~~~{.cpp}
/// 
/// void setup()
/// {
/// 
///     cylinder.set( cylinderRadius, cylinderHeight );
/// }
/// 
/// void draw()
/// {
/// 
///     cylinder.setPosition(ofGetWidth()*.2, ofGetHeight()*.75, 0);
///     cylinder.rotate(spinX, 1.0, 0.0, 0.0);
///     cylinder.rotate(spinY, 0, 1.0, 0.0);
/// 
///     // get all the faces from the icoSphere, handy when you want to copy
///     // individual vertices or tweak them a little ;)
///     vector<ofMeshFace> triangles = cylinder.getMesh().getUniqueFaces();
/// 
///     // now draw
///     cylinder.draw();
/// }
/// ~~~~
/// 
/// To draw a texture over any primitive, simply bind the ofTexture instance 
/// and then draw your primitive:
/// 
/// ~~~~{.cpp}
/// 
/// texture.getTextureReference().bind();
/// // now draw
/// cylinder.draw();
/// 
/// ~~~~

class ofCylinderPrimitive : public of3dPrimitive {
public:
    ofCylinderPrimitive();
    ofCylinderPrimitive( float radius, float height, int radiusSegments, 
        int heightSegments, int capSegments=2, bool bCapped = true,
        ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    ~ofCylinderPrimitive();

    void set( float radius, float height, int radiusSegments, 
        int heightSegments, int capSegments=2, bool bCapped=true,
        ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
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

    std::vector<ofIndexType> getTopCapIndices() const;
    ofMesh getTopCapMesh() const;
    std::vector<ofIndexType> getCylinderIndices() const;
    ofMesh getCylinderMesh() const;
    std::vector<ofIndexType> getBottomCapIndices() const;
    ofMesh getBottomCapMesh() const;

    int getResolutionRadius() const;
    int getResolutionHeight() const;
    int getResolutionCap() const;
	glm::vec3 getResolution() const;

    float getHeight() const;
    float getRadius() const;
    bool getCapped() const;
protected:
    float radius;
    float height;
    bool bCapped;
    int strides[3][2];
    int vertices[3][2];
	glm::vec3 resolution;
};

/// \brief The ofConePrimitive allows you to create a 3D cone. 
/// Like all primitives it allows you to set the size, draw it, 
/// set positions, etc, as a simple example:
/// 
/// ~~~~{.cpp}
/// 
/// void setup()
/// {
/// 
///     cone.set( coneRadius, coneHeight, coneRadiusSegments, coneHeightSegments );
/// }
/// 
/// void draw()
/// {
/// 
///     cone.setPosition(ofGetWidth()*.2, ofGetHeight()*.75, 0);
///     cone.rotate(spinX, 1.0, 0.0, 0.0);
///     cone.rotate(spinY, 0, 1.0, 0.0);
/// 
///     // get all the faces from the cpme, handy when you want to copy
///     // individual vertices or tweak them a little ;)
///     vector<ofMeshFace> triangles = cone.getMesh().getUniqueFaces();
/// 
///     // now draw
///     cone.draw();
/// }
/// ~~~~
/// 
/// To draw a texture over any primitive, simply bind the ofTexture instance 
/// and then draw your primitive:
/// 
/// ~~~~{.cpp}
/// 
/// texture.getTextureReference().bind();
/// // now draw
/// cone.draw();
/// 
/// ~~~~
/// 
/// Beware of the cone texture coordinates, they're a little strange.
/// 

class ofConePrimitive : public of3dPrimitive {
public:

    ofConePrimitive();
    ofConePrimitive( float radius, float height, int radiusSegments, 
        int heightSegments, int capSegments=2, 
        ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
    ~ofConePrimitive();

    void set( float radius, float height, int radiusSegments, int heightSegments, 
        int capSegments=2, ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP );
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

    /// \return a vector of the indices of vertices that make up the cone (as opposed to the cap indices).
    std::vector<ofIndexType> getConeIndices() const;

    /// \return This returns an ofMesh made up of the cone (as opposed to the cap).
    ofMesh getConeMesh() const;

    /// \return a vector of the indices of vertices that make up the cap (as opposed to the cone indices).
    std::vector<ofIndexType> getCapIndices() const;

    /// \return an ofMesh made up of the cap (as opposed to the cone).
    ofMesh getCapMesh() const;

    int getResolutionRadius() const;
    int getResolutionHeight() const;

    /// \return the resolution of the cap (rather than the cone).
    int getResolutionCap() const;

    /// \return the resolution of the cone (rather than the cap).
	glm::vec3 getResolution() const;

    /// \return the radius of the cap.
    float getRadius() const;

    /// \return the height of the cone.
    float getHeight() const;

protected:
    float radius;
    float height;

	glm::vec3 resolution;

    int strides[2][2];
    int vertices[2][2];
};


/// \brief The ofBoxPrimitive allows you to create cubes and cuboids.
/// 
/// Like other primitives, it allows you to set its dimensions and position, draw it, etc. As a simple example:
/// 
/// ~~~~{.cpp}
/// 
/// void setup()
/// {
/// 
///     box.set( boxSize );
///     // or
///     // box.set( boxHeight, boxWidth, boxDepth );
/// }
/// 
/// void draw()
/// {
/// 
///     box.setPosition(ofGetWidth()*.2, ofGetHeight()*.75, 0);
///     box.rotate(spinX, 1.0, 0.0, 0.0);
///     box.rotate(spinY, 0, 1.0, 0.0);
/// 
///     // get all the faces from the icoSphere, handy when you want to copy
///     // individual vertices or tweak them a little ;)
///     vector<ofMeshFace> triangles = box.getMesh().getUniqueFaces();
/// 
///     // now draw
///     box.draw();
/// }
/// ~~~~
/// 
/// To draw a texture over any primitive, simply bind the ofTexture instance and then draw your primitive:
/// 
/// ~~~~{.cpp}
/// 
/// texture.getTextureReference().bind();
/// // now draw
/// box.draw();
/// 
/// ~~~~
/// 

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
    ofBoxPrimitive( float width, float height, float depth, int resWidth=2, 
        int resHeight=2, int resDepth=2 );
    ~ofBoxPrimitive();

    void set( float width, float height, float depth, int resWidth, 
        int resHeight, int resDepth);
    void set( float width, float height, float depth );
    void set( float size ); // all sides the same dimensions //

    void setWidth( float a_width );
    void setHeight( float a_height );
    void setDepth( float a_depth );

    void resizeToTexture( ofTexture& inTexture );

    std::vector<ofIndexType> getSideIndices( int sideIndex ) const;
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
	glm::vec3 getResolution() const;

    float getWidth() const;
    float getHeight() const;
    float getDepth() const;
	glm::vec3 getSize() const;
protected:
	glm::vec3 size;
	glm::vec3 resolution;
    // indices strides for faces //
    int strides[6][2];
    int vertices[6][2];

};
