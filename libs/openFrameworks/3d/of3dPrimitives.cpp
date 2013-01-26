//
//  ofPrimitiveBase.cpp
//  openFrameworksLib
//
//  Created by Nick Hardeman on 9/14/12.
//
//

#include "of3dPrimitives.h"
#include "ofGraphics.h"

ofPrimitiveBase::ofPrimitiveBase() {
    setScale(1.0, 1.0, 1.0);
}
ofPrimitiveBase::~ofPrimitiveBase() {
    
}

// GETTERS //
//----------------------------------------------------------
ofMesh* ofPrimitiveBase::getMeshPtr() {
    return& _mesh;
}
//----------------------------------------------------------
ofMesh& ofPrimitiveBase::getMesh() {
    return _mesh;
}

//----------------------------------------------------------
ofVec4f* ofPrimitiveBase::getTexCoordsPtr() {
    return& _texCoords;
}

//----------------------------------------------------------
ofVec4f& ofPrimitiveBase::getTexCoords() {
    return _texCoords;
}

//----------------------------------------------------------
vector<ofIndexType> ofPrimitiveBase::getIndices( int startIndex, int endIndex ) {
    vector<ofIndexType> indices;
    indices.assign( getMesh().getIndices().begin()+startIndex, getMesh().getIndices().begin()+endIndex );
    return indices;
}

//----------------------------------------------------------
void ofPrimitiveBase::setColorForIndices( int startIndex, int endIndex, ofColor color ) {
    if(!getMesh().hasColors()) {
        // no colors for vertices, so we must set them here //
        getMesh().getColors().resize(getMesh().getNumVertices());
    }
    
    for(int i = startIndex; i < endIndex; i++) {
        getMesh().setColor( getMesh().getIndex(i), color);
    }
}

//----------------------------------------------------------
ofMesh ofPrimitiveBase::getMeshForIndexes( int startIndex, int endIndex, int startVertIndex, int endVertIndex ) {
    
    ofMesh mesh;
    mesh.setMode( getMesh().getMode() );
    
    vector<ofVec3f> vertices;
    vertices.assign( getMesh().getVertices().begin()+startVertIndex, getMesh().getVertices().begin()+endVertIndex );
    mesh.addVertices( vertices );
    
    if(getMesh().hasColors()) {
        vector<ofFloatColor> colors;
        colors.assign( getMesh().getColors().begin()+startVertIndex, getMesh().getColors().begin()+endVertIndex );
        mesh.addColors( colors );
        if(getMesh().usingColors()) mesh.enableColors();
        else mesh.disableColors();
    }
    
    if(getMesh().hasTexCoords()) {
        vector<ofVec2f> texcoords;
        texcoords.assign( getMesh().getTexCoords().begin()+startVertIndex, getMesh().getTexCoords().begin()+endVertIndex );
        mesh.addTexCoords( texcoords );
        if(getMesh().usingTextures()) mesh.enableTextures();
        else mesh.disableTextures();
    }
    
    if(getMesh().hasNormals()) {
        vector<ofVec3f> normals;
        normals.assign( getMesh().getNormals().begin()+startVertIndex, getMesh().getNormals().begin()+endVertIndex );
        mesh.addNormals( normals );
        if(getMesh().usingNormals()) mesh.enableNormals();
        else mesh.disableNormals();
    }
    
    int offsetIndex = getMesh().getIndex(startIndex);
    for(int i = startIndex; i < endIndex; i++) {
        int index = getMesh().getIndex(i) - offsetIndex;
        mesh.addIndex( index );
    }
    
    return mesh;
}


//----------------------------------------------------------
bool ofPrimitiveBase::hasScaling() {
    ofVec3f scale = getScale();
    return (scale.x != 1.f || scale.y != 1.f || scale.z != 1.f);
}
//----------------------------------------------------------
bool ofPrimitiveBase::hasNormalsEnabled() {
    return getMesh().hasNormals();
}
//----------------------------------------------------------
ofVec3f ofPrimitiveBase::getResolution() const {
    return _resolution;
}

//----------------------------------------------------------
void ofPrimitiveBase::enableNormals() {
    getMesh().enableNormals();
}
//----------------------------------------------------------
void ofPrimitiveBase::enableTextures() {
    getMesh().enableTextures();
}
//----------------------------------------------------------
void ofPrimitiveBase::enableColors() {
    getMesh().enableColors();
}
//----------------------------------------------------------
void ofPrimitiveBase::disableNormals() {
    getMesh().disableNormals();
}
//----------------------------------------------------------
void ofPrimitiveBase::disableTextures() {
    getMesh().disableTextures();
}
//----------------------------------------------------------
void ofPrimitiveBase::disableColors() {
    getMesh().disableColors();
}




// SETTERS //

//----------------------------------------------------------
void ofPrimitiveBase::setResolution( int resX, int resY, int resZ ) {
    _resolution.set( resX, resY, resZ );
}

//----------------------------------------------------------
void ofPrimitiveBase::normalizeAndApplySavedTexCoords() {
    ofVec4f tcoords = getTexCoords();
    // when a new mesh is created, it uses normalized tex coords, we need to reset them
    // but save the ones used previously //
    _texCoords.set(0,0,1,1);
    setTexCoords(tcoords.x, tcoords.y, tcoords.z, tcoords.w);
}

// applies to all the meshes evenly //
//----------------------------------------------------------
void ofPrimitiveBase::setTexCoords( float u1, float v1, float u2, float v2 ) {
    //setTexCoords( u1, v1, u2, v2 );
    ofVec4f prevTcoord = getTexCoords();
    
    for(int j = 0; j < getMesh().getNumTexCoords(); j++ ) {
        ofVec2f tcoord = getMesh().getTexCoord(j);
        tcoord.x = ofMap(tcoord.x, prevTcoord.x, prevTcoord.z, u1, u2);
        tcoord.y = ofMap(tcoord.y, prevTcoord.y, prevTcoord.w, v1, v2);
        
        getMesh().setTexCoord(j, tcoord);
    }
    
    _texCoords.set(u1, v1, u2, v2);
}

//----------------------------------------------------------
void ofPrimitiveBase::setTexCoordsFromTexture( ofTexture& inTexture ) {
    bool bNormalized = (inTexture.getTextureData().textureTarget!=GL_TEXTURE_RECTANGLE_ARB);
    ofTextureData& tdata = inTexture.getTextureData();
    if(bNormalized)
        setTexCoords( 0, 0, tdata.tex_t, tdata.tex_u );
    else
        setTexCoords(0, 0, inTexture.getWidth(), inTexture.getHeight());
    
    ofVec4f tcoords = getTexCoords();
    setTexCoords(tcoords.x, tcoords.y, tcoords.z, tcoords.w);
}




//--------------------------------------------------------------
void ofPrimitiveBase::drawVertices() {
	draw(OF_MESH_POINTS);
}

//--------------------------------------------------------------
void ofPrimitiveBase::drawWireframe() {
	draw(OF_MESH_WIREFRAME);
}

//--------------------------------------------------------------
void ofPrimitiveBase::drawFaces() {
	draw(OF_MESH_FILL);
}

//--------------------------------------------------------------
void ofPrimitiveBase::draw() {
	draw(OF_MESH_FILL);
}

//--------------------------------------------------------------
void ofPrimitiveBase::draw(ofPolyRenderMode renderType) {
    // ofNode applies all of the tranformations needed, included scale //
    ofNode::transformGL();
    ofGetCurrentRenderer()->draw(*this, renderType);
    ofNode::restoreTransformGL();
}

//--------------------------------------------------------------
void ofPrimitiveBase::drawNormals(float length, bool bFaceNormals) {
    ofNode::transformGL();
    
    float fixLength = getScale().length();
    
    if(getMesh().usingNormals()) {
        vector<ofVec3f>& normals = getMesh().getNormals();
        vector<ofVec3f>& vertices = getMesh().getVertices();
        ofVec3f normal;
        ofVec3f vert;
        glBegin(GL_LINES);
        
        if(bFaceNormals) {
            for(int i = 0; i < normals.size(); i += 3) {
                vert = (vertices[i+0]+vertices[i+1]+vertices[i+2]) / 3;
                glVertex3f(vert.x, vert.y, vert.z);
                normal = normals[i];
                normal *= length;
                glVertex3f(normal.x+vert.x, normal.y+vert.y, normal.z+vert.z);
            }
        } else {
            for(int i = 0; i < normals.size(); i++) {
                vert = vertices[i];
                normal = normals[i].normalized();
                glVertex3f(vert.x, vert.y, vert.z);
                normal *= length;
                glVertex3f(normal.x+vert.x, normal.y+vert.y, normal.z+vert.z);
            }
        }
        glEnd();
    } else {
        ofLog(OF_LOG_WARNING, "ofPrimitiveBase :: drawNormals()") << " : mesh normals are disabled";
    }
    
    
    ofNode::restoreTransformGL();
}

//--------------------------------------------------------------
void ofPrimitiveBase::drawAxes(float a_size) {
    ofNode::transformGL();
    ofDrawAxis(a_size);
    ofNode::restoreTransformGL();
}



// PLANE PRIMITIVE //
//--------------------------------------------------------------
ofPlanePrimitive::ofPlanePrimitive() {
    _texCoords = ofVec4f(0,0,1,1);
    set( 200, 100, 6, 3);
}

//--------------------------------------------------------------
ofPlanePrimitive::ofPlanePrimitive(float width, float height, int columns, int rows, ofPrimitiveMode mode) {
    _texCoords = ofVec4f(0,0,1,1);
    set(width, height, columns, rows, mode);
}

//--------------------------------------------------------------
ofPlanePrimitive::~ofPlanePrimitive() {}

//--------------------------------------------------------------
void ofPlanePrimitive::set(float width, float height, int columns, int rows, ofPrimitiveMode mode) {
    
    _width  = width;
    _height = height;
    ofPrimitiveBase::setResolution(columns, rows, 0);
    
    _mesh.clear();
    //_mesh = ofGetPlaneMesh( getWidth(), getHeight(), getResolution().x, getResolution().y, mode );
    _mesh = ofMesh::plane( getWidth(), getHeight(), getResolution().x, getResolution().y, mode );
    
    normalizeAndApplySavedTexCoords();
    
}

//--------------------------------------------------------------
void ofPlanePrimitive::set( float width, float height ) {
    _width = width;
    setHeight(height);
}

//--------------------------------------------------------------
void ofPlanePrimitive::resizeToTexture( ofTexture& inTexture, float scale ) {
    set(inTexture.getWidth() * scale, inTexture.getHeight() * scale);
    setTexCoordsFromTexture( inTexture );
}

//--------------------------------------------------------------
void ofPlanePrimitive::setResolution( int columns, int rows ) {
    setResolution( columns,  rows,  0);
}

//--------------------------------------------------------------
void ofPlanePrimitive::setResolution(int resX, int resY, int resZ) {
    
    ofPrimitiveBase::setResolution(resX, resY, 0); // no z value //
    ofPrimitiveMode mode = OF_PRIMITIVE_TRIANGLE_STRIP;
    mode = getMesh().getMode();
    
    set( getWidth(), getHeight(), getResolution().x, getResolution().y, mode );
}

//--------------------------------------------------------------
void ofPlanePrimitive::setMode(ofPrimitiveMode mode) {
    ofPrimitiveMode currMode = OF_PRIMITIVE_TRIANGLE_STRIP;
    currMode = getMesh().getMode();
    
    if( mode != currMode )
        set( getWidth(), getHeight(), getResolution().x, getResolution().y, mode );
}

//--------------------------------------------------------------
void ofPlanePrimitive::setWidth( float width ) {
    _width = width;
    setResolution( getResolution().x, getResolution().y );
}

//--------------------------------------------------------------
void ofPlanePrimitive::setHeight(float height) {
    _height = height;
    setResolution( getResolution().x, getResolution().y );
}

//--------------------------------------------------------------
float ofPlanePrimitive::getWidth() {
    return _width;
}

//--------------------------------------------------------------
float ofPlanePrimitive::getHeight() {
    return _height;
}





// SPHERE PRIMITIVE //
//----------------------------------------------------------
ofSpherePrimitive::ofSpherePrimitive() {
    _texCoords = ofVec4f(0,0,1,1);
    _radius = 20;
    setResolution(16);
}

//----------------------------------------------------------
ofSpherePrimitive::ofSpherePrimitive( float radius, int res, ofPrimitiveMode mode ) {
    _radius = radius;
    _texCoords = ofVec4f(0,0,1,1);
    setResolution(res, res, res);
}

//----------------------------------------------------------
ofSpherePrimitive::~ofSpherePrimitive() {
    
}

//----------------------------------------------------------
void ofSpherePrimitive::set(float radius, int res, ofPrimitiveMode mode ) {
    _radius = radius;
    ofPrimitiveBase::setResolution(res, res, res);
    getMesh().clear();
    //_mesh = ofGetSphereMesh( getRadius(), getResolution().x, mode );
    _mesh = ofMesh::sphere( getRadius(), getResolution().x, mode );
    
    normalizeAndApplySavedTexCoords();
}

//----------------------------------------------------------
void ofSpherePrimitive::setResolution(int res) {
    setResolution(res, res, res);
}

//----------------------------------------------------------
void ofSpherePrimitive::setResolution(int resX, int resY, int resZ) {
    ofPrimitiveBase::setResolution(resX, resY, resZ);
    ofPrimitiveMode mode = OF_PRIMITIVE_TRIANGLE_STRIP;
    
    mode = getMesh().getMode();
    
    set(getRadius(), getResolution().x, mode );
}

//----------------------------------------------------------
void ofSpherePrimitive::setMode( ofPrimitiveMode mode ) {
    ofPrimitiveMode currMode = OF_PRIMITIVE_TRIANGLE_STRIP;
    mode = getMesh().getMode();
    if(currMode != mode)
        set(getRadius(), getResolution().x, mode );
}

//----------------------------------------------------------
void ofSpherePrimitive::setRadius(float radius) {
    _radius = radius;
    setResolution(getResolution().x, getResolution().y, getResolution().z);
}

//----------------------------------------------------------
float ofSpherePrimitive::getRadius() {
    return _radius;
}



// ICO SPHERE //
//----------------------------------------------------------
ofIcoSpherePrimitive::ofIcoSpherePrimitive() {
    _texCoords = ofVec4f(0,0,1,1);
    _radius = 20;
    setResolution(2);
}

//----------------------------------------------------------
ofIcoSpherePrimitive::ofIcoSpherePrimitive( float radius, int iterations ) {
    _texCoords = ofVec4f(0,0,1,1);
    _radius = radius;
    setResolution(iterations);
}

//----------------------------------------------------------
ofIcoSpherePrimitive::~ofIcoSpherePrimitive() {
    
}

//----------------------------------------------------------
void ofIcoSpherePrimitive::set(float radius, int res ) {
    _radius = radius;
    setResolution(res);
}

//----------------------------------------------------------
void ofIcoSpherePrimitive::setResolution( int iterations ) {
    setResolution(iterations, iterations, iterations);
}

//----------------------------------------------------------
void ofIcoSpherePrimitive::setResolution( int resX, int resY, int resZ ) {
    ofPrimitiveBase::setResolution(resX, resY, resZ);
    
    getMesh().clear();
    // store the number of iterations in the resolution //
    //_mesh = ofGetIcoSphereMesh( getRadius(), getResolution().x );
    _mesh = ofMesh::icosphere( getRadius(), getResolution().x );
    normalizeAndApplySavedTexCoords();
}

//----------------------------------------------------------
void ofIcoSpherePrimitive::setMode( ofPrimitiveMode mode ) {
    // ofIcoSpherePrimitive only works with OF_PRIMITIVE_TRIANGLES //
    setResolution( (int)getResolution().x );
}

//----------------------------------------------------------
void ofIcoSpherePrimitive::setRadius(float radius) {
    _radius = radius;
    setResolution(getResolution().x);
}

//----------------------------------------------------------
float ofIcoSpherePrimitive::getRadius() {
    return _radius;
}




//--------------------------------------------------------------
ofCylinderPrimitive::ofCylinderPrimitive() {
    _texCoords = ofVec4f(0,0,1,1);
    set( 60, 80, 6, 3, 2, true );
}

//--------------------------------------------------------------
ofCylinderPrimitive::ofCylinderPrimitive( float radius, float height, int radiusSegments, int heightSegments, int numCapSegments, bool bCapped, ofPrimitiveMode mode ) {
    _texCoords = ofVec4f(0,0,1,1);
    set( radius, height, radiusSegments, heightSegments, numCapSegments, bCapped, mode );
}

//--------------------------------------------------------------
ofCylinderPrimitive::~ofCylinderPrimitive() {}

//--------------------------------------------------------------
void ofCylinderPrimitive::set(float radius, float height, int radiusSegments, int heightSegments, int numCapSegments, bool bCapped, ofPrimitiveMode mode) {
    _radius = radius;
    _height = height;
    _bCapped = bCapped;
    ofPrimitiveBase::setResolution( radiusSegments, heightSegments, numCapSegments );
    
    int resX = getResolution().x;
    int resY = getResolution().y-1;
    int resZ = getResolution().z-1;
    
    int indexStep = 2;
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        indexStep = 6;
        resX = resX-1;
    }
    
    // 0 -> top cap
    _strides[0][0] = 0;
    _strides[0][1] = resX * resZ * indexStep;
    _vertices[0][0] = 0;
    _vertices[0][1] = getResolution().x * getResolution().z;
    
    // 1 -> cylinder //
    if(bCapped) {
        _strides[1][0] = _strides[0][0] + _strides[0][1];
        _vertices[1][0] = _vertices[0][0] + _vertices[0][1];
    } else {
        _strides[1][0] = 0;
        _vertices[1][0] = 0;
    }
    _strides[1][1] = resX * resY * indexStep;
    _vertices[1][1] = getResolution().x * getResolution().y;
    
    // 2 -> bottom cap
    _strides[2][0] = _strides[1][0] + _strides[1][1];
    _strides[2][1] = resX * resZ * indexStep;
    _vertices[2][0] = _vertices[1][0]+_vertices[1][1];
    _vertices[2][1] = getResolution().x * getResolution().z;
    
    
    getMesh().clear();
    //_mesh = ofGetCylinderMesh( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, getCapped(), mode );
    _mesh = ofMesh::cylinder( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, getCapped(), mode );
    
    normalizeAndApplySavedTexCoords();
    
}

//--------------------------------------------------------------
void ofCylinderPrimitive::set( float radius, float height, bool bCapped ) {
    _radius = radius;
    _bCapped = bCapped;
    setHeight( height );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setRadius( float radius ) {
    _radius = radius;
    setResolution(getResolution().x, getResolution().y, getResolution().z);
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setHeight( float height ) {
    _height = height;
    setResolution(getResolution().x, getResolution().y, getResolution().z);
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setCapped(bool bCapped) {
    _bCapped = bCapped;
    setResolution( getResolution().x, getResolution().y, getResolution().z );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setResolution( int radiusSegments, int heightSegments, int capSegments ) {
    ofPrimitiveMode mode = OF_PRIMITIVE_TRIANGLE_STRIP;
    mode = getMesh().getMode();
    set( getRadius(), getHeight(), radiusSegments, heightSegments, capSegments, getCapped(), mode );
}

//----------------------------------------------------------
void ofCylinderPrimitive::setMode( ofPrimitiveMode mode ) {
    ofPrimitiveMode currMode = OF_PRIMITIVE_TRIANGLE_STRIP;
    currMode = getMesh().getMode();
    if(currMode != mode)
        set( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, getCapped(), mode );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setTopCapColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
    }
    setColorForIndices( _strides[0][0], _strides[0][0]+_strides[0][1], color );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setCylinderColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
    }
    setColorForIndices( _strides[1][0], _strides[1][0]+_strides[1][1], color );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setBottomCapColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
    }
    setColorForIndices( _strides[2][0], _strides[2][0]+_strides[2][1], color );
}

//--------------------------------------------------------------
vector<ofIndexType> ofCylinderPrimitive::getTopCapIndices() {
    return ofPrimitiveBase::getIndices( _strides[0][0], _strides[0][0] + _strides[0][1] );
}

//--------------------------------------------------------------
ofMesh ofCylinderPrimitive::getTopCapMesh() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
        return ofMesh();
    }
    return getMeshForIndexes( _strides[0][0], _strides[0][0]+_strides[0][1],
                             _vertices[0][0], _vertices[0][0]+_vertices[0][1] );
}

//--------------------------------------------------------------
vector<ofIndexType> ofCylinderPrimitive::getCylinderIndices() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
    }
    return ofPrimitiveBase::getIndices( _strides[1][0], _strides[1][0] + _strides[1][1] );
}

//--------------------------------------------------------------
ofMesh ofCylinderPrimitive::getCylinderMesh() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
        return ofMesh();
    }
    return getMeshForIndexes( _strides[1][0], _strides[1][0]+_strides[1][1],
                             _vertices[1][0], _vertices[1][0]+_vertices[1][1] );
}

//--------------------------------------------------------------
vector<ofIndexType> ofCylinderPrimitive::getBottomCapIndices() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
    }
    return ofPrimitiveBase::getIndices( _strides[2][0], _strides[2][0] + _strides[2][1] );
}

//--------------------------------------------------------------
ofMesh ofCylinderPrimitive::getBottomCapMesh() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
        return ofMesh();
    }
    return getMeshForIndexes( _strides[2][0], _strides[2][0]+_strides[2][1],
                             _vertices[2][0], _vertices[2][0]+_vertices[2][1] );
}

//--------------------------------------------------------------
float ofCylinderPrimitive::getHeight() {
    return _height;
}

//--------------------------------------------------------------
float ofCylinderPrimitive::getRadius() {
    return _radius;
}

//--------------------------------------------------------------
bool ofCylinderPrimitive::getCapped() {
    return _bCapped;
}





// Cone Primitive //
//--------------------------------------------------------------
ofConePrimitive::ofConePrimitive() {
    _texCoords = ofVec4f(0,0,1,1);
    set( 20, 70, 8, 3, 2 );
}

//--------------------------------------------------------------
ofConePrimitive::ofConePrimitive( float radius, float height, int radiusSegments, int heightSegments, int capSegments, ofPrimitiveMode mode ) {
    _texCoords = ofVec4f(0,0,1,1);
    set( radius, height, radiusSegments, heightSegments, capSegments, mode );
}

//--------------------------------------------------------------
ofConePrimitive::~ofConePrimitive() {}

//--------------------------------------------------------------
void ofConePrimitive::set( float radius, float height, int radiusSegments, int heightSegments, int capSegments, ofPrimitiveMode mode ) {
    _radius = radius;
    _height = height;
    ofPrimitiveBase::setResolution(radiusSegments, heightSegments, capSegments);
    
    int resX = getResolution().x;
    int resY = getResolution().y-1;
    int resZ = getResolution().z-1;
    
    int indexStep = 2;
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        indexStep = 6;
        resX = resX-1;
    }
    
    _strides[ 0 ][0] = 0;
    _strides[ 0 ][1] = (resX)*(resY) * indexStep;
    _vertices[0][0] = 0;
    _vertices[0][1] = getResolution().x * getResolution().y;
    
    _strides[ 1 ][0] = _strides[ 0 ][0] + _strides[ 0 ][1];
    _strides[ 1 ][1] = (resX)*(resZ) * indexStep;
    _vertices[1][0] = _vertices[0][0] + _vertices[0][1];
    _vertices[1][1] = getResolution().x * getResolution().z;
    
    getMesh().clear();
    //_mesh = ofGetConeMesh( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, mode );
    _mesh = ofMesh::cone( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, mode );
    
    normalizeAndApplySavedTexCoords();
    
}

//--------------------------------------------------------------
void ofConePrimitive::set( float radius, float height ) {
    _radius = radius;
    _height = height;
    setResolution( getResolution().x, getResolution().y, getResolution().z );
}

//--------------------------------------------------------------
void ofConePrimitive::setResolution( int radiusSegments, int heightSegments ) {
    setResolution(radiusSegments, heightSegments, getResolution().z);
}

//--------------------------------------------------------------
void ofConePrimitive::setResolution( int resX, int resY, int resZ ) {
    ofPrimitiveMode mode = OF_PRIMITIVE_TRIANGLE_STRIP;
    mode = getMesh().getMode();
    set( getRadius(), getHeight(), resX, resY, resZ, mode );
}

//----------------------------------------------------------
void ofConePrimitive::setMode( ofPrimitiveMode mode ) {
    ofPrimitiveMode currMode = OF_PRIMITIVE_TRIANGLE_STRIP;
    currMode = getMesh().getMode();
    if(currMode != mode)
        set( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, mode );
}

//--------------------------------------------------------------
void ofConePrimitive::setRadius( float radius ) {
    _radius = radius;
    setResolution(getResolution().x, getResolution().y, getResolution().z);
}

//--------------------------------------------------------------
void ofConePrimitive::setHeight(float height) {
    _height = height;
    setResolution(getResolution().x, getResolution().y, getResolution().z);
}

//--------------------------------------------------------------
void ofConePrimitive::setTopColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofConePrimitive : must be in triangle strip mode" << endl;
    }
    setColorForIndices( _strides[0][0], _strides[0][0]+_strides[0][1], color );
}

//--------------------------------------------------------------
void ofConePrimitive::setCapColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofConePrimitive : must be in triangle strip mode" << endl;
    }
    setColorForIndices( _strides[1][0], _strides[1][0]+_strides[1][1], color );
}

//--------------------------------------------------------------
vector<ofIndexType> ofConePrimitive::getConeIndices() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofConePrimitive : must be in triangle strip mode" << endl;
    }
    return ofPrimitiveBase::getIndices(_strides[0][0], _strides[0][0]+_strides[0][1]);
}

//--------------------------------------------------------------
ofMesh ofConePrimitive::getConeMesh() {
    int startIndex  = _strides[0][0];
    int endIndex    = startIndex + _strides[0][1];
    
    int startVertIndex  = _vertices[0][0];
    int endVertIndex    = startVertIndex + _vertices[0][1];
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofConePrimitive : must be in triangle strip mode" << endl;
        return ofMesh();
    }
    return getMeshForIndexes( startIndex, endIndex, startVertIndex, endVertIndex );
}

//--------------------------------------------------------------
vector<ofIndexType> ofConePrimitive::getCapIndices() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofConePrimitive : must be in triangle strip mode" << endl;
    }
    return ofPrimitiveBase::getIndices( _strides[1][0], _strides[1][0] + _strides[1][1] );
}

//--------------------------------------------------------------
ofMesh ofConePrimitive::getCapMesh() {
    int startIndex  = _strides[1][0];
    int endIndex    = startIndex + _strides[1][1];
    
    int startVertIndex  = _vertices[1][0];
    int endVertIndex    = startVertIndex + _vertices[1][1];
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofConePrimitive : must be in triangle strip mode" << endl;
        return ofMesh();
    }
    return getMeshForIndexes( startIndex, endIndex, startVertIndex, endVertIndex );
}

//--------------------------------------------------------------
float ofConePrimitive::getRadius() {
    return _radius;
}

//--------------------------------------------------------------
float ofConePrimitive::getHeight() {
    return _height;
}





// BOX PRIMITIVE //
//--------------------------------------------------------------
ofBoxPrimitive::ofBoxPrimitive() {
    _texCoords = ofVec4f(0,0,1,1);
    set(100, 100, 100, 2, 2, 2);
}

//--------------------------------------------------------------
ofBoxPrimitive::ofBoxPrimitive( float width, float height, float depth, int resWidth, int resHeight, int resDepth ) {
    _texCoords = ofVec4f(0,0,1,1);
    set(width, height, depth, resWidth, resHeight, resDepth );
}

//--------------------------------------------------------------
ofBoxPrimitive::~ofBoxPrimitive() {}

//--------------------------------------------------------------
void ofBoxPrimitive::set( float width, float height, float depth, int resWidth, int resHeight, int resDepth) {
    
    _size.x = width;
    _size.y = height;
    _size.z = depth;
    
    ofPrimitiveBase::setResolution( resWidth, resHeight, resDepth);
    
    int resX = getResolution().x;
    int resY = getResolution().y;
    int resZ = getResolution().z;
    
    //FRONT, resY, resX
    _strides[ SIDE_FRONT ][0] = 0;
    _strides[ SIDE_FRONT ][1] = (resY-1)*(resX-1)*6;
    _vertices[SIDE_FRONT][0] = 0;
    _vertices[SIDE_FRONT][1] = resX * resY;
    
    //RIGHT, resY, resZ
    _strides[ SIDE_RIGHT ][0] = _strides[ SIDE_FRONT ][0] + _strides[ SIDE_FRONT ][1];
    _strides[ SIDE_RIGHT ][1] = (resY-1)*(resZ-1)*6;
    _vertices[SIDE_RIGHT][0] = _vertices[SIDE_FRONT][0] + _vertices[SIDE_FRONT][1];
    _vertices[SIDE_RIGHT][1] = resY * resZ;
    
    //LEFT, resY, resZ
    _strides[ SIDE_LEFT ][0] = _strides[ SIDE_RIGHT ][0] + _strides[ SIDE_RIGHT ][1];
    _strides[ SIDE_LEFT ][1] = (resY-1)*(resZ-1)*6;
    _vertices[SIDE_LEFT][0] = _vertices[SIDE_RIGHT][0] + _vertices[SIDE_RIGHT][1];
    _vertices[SIDE_LEFT][1] = resY * resZ;
    
    //BACK, resY, resX
    _strides[ SIDE_BACK ][0] = _strides[ SIDE_LEFT ][0] + _strides[ SIDE_LEFT ][1];
    _strides[ SIDE_BACK ][1] = (resY-1)*(resX-1)*6;
    _vertices[SIDE_BACK][0] = _vertices[SIDE_LEFT][0] + _vertices[SIDE_LEFT][1];
    _vertices[SIDE_BACK][1] = resY * resX;
    
    //TOP, resZ, resX
    _strides[ SIDE_TOP ][0] = _strides[ SIDE_BACK ][0] + _strides[ SIDE_BACK ][1];
    _strides[ SIDE_TOP ][1] = (resZ-1)*(resX-1)*6;
    _vertices[SIDE_TOP][0] = _vertices[SIDE_BACK][0] + _vertices[SIDE_BACK][1];
    _vertices[SIDE_TOP][1] = resZ * resX;
    
    //BOTTOM, resZ, resX
    _strides[ SIDE_BOTTOM ][0] = _strides[ SIDE_TOP ][0]+_strides[ SIDE_TOP ][1];
    _strides[ SIDE_BOTTOM ][1] = (resZ-1)*(resX-1)*6;
    _vertices[SIDE_BOTTOM][0] = _vertices[SIDE_TOP][0] + _vertices[SIDE_TOP][1];
    _vertices[SIDE_BOTTOM][1] = resZ * resX;
    
    _mesh.clear();
    //_mesh = ofGetBoxMesh( getWidth(), getHeight(), getDepth(), getResolution().x, getResolution().y, getResolution().z );
    _mesh = ofMesh::box( getWidth(), getHeight(), getDepth(), getResolution().x, getResolution().y, getResolution().z );
    
    normalizeAndApplySavedTexCoords();
}

//--------------------------------------------------------------
void ofBoxPrimitive::set( float width, float height, float depth ) {
    set( width, height, depth, getResolution().x, getResolution().y, getResolution().z );
}

//--------------------------------------------------------------
void ofBoxPrimitive::set( float size ) {
    set( size, size, size );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setWidth( float a_width ) {
    _size.x = a_width;
    set( getWidth(), getHeight(), getDepth() );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setHeight( float a_height ) {
    _size.y = a_height;
    set( getWidth(), getHeight(), getDepth() );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setDepth( float a_depth ) {
    _size.z = a_depth;
    set( getWidth(), getHeight(), getDepth() );
}

//--------------------------------------------------------------
void ofBoxPrimitive::resizeToTexture( ofTexture& inTexture ) {
    set(inTexture.getWidth(), inTexture.getHeight(), inTexture.getWidth());
    setTexCoordsFromTexture( inTexture );
}

//--------------------------------------------------------------
vector<ofIndexType> ofBoxPrimitive::getSideIndices( int sideIndex ) {
    
    if(sideIndex < 0 || sideIndex >= SIDES_TOTAL) {
        ofLog(OF_LOG_WARNING) << "ofBoxPrimitive :: getSideIndices : faceIndex out of bounds, returning SIDE_FRONT ";
        sideIndex = SIDE_FRONT;
    }
    
    return getIndices(_strides[sideIndex][0], _strides[sideIndex][0]+_strides[sideIndex][1]);
}

//--------------------------------------------------------------
ofMesh ofBoxPrimitive::getSideMesh( int sideIndex ) {
    
    if(sideIndex < 0 || sideIndex > SIDES_TOTAL) {
        ofLog(OF_LOG_WARNING) << "ofBoxPrimitive :: getSideMesh : faceIndex out of bounds, using SIDE_FRONT ";
        sideIndex = SIDE_FRONT;
    }
    int startIndex  = _strides[sideIndex][0];
    int endIndex    = startIndex+_strides[sideIndex][1];
    
    int startVertIndex  = _vertices[sideIndex][0];
    int endVertIndex    = startVertIndex + _vertices[sideIndex][1];
    
    return getMeshForIndexes( startIndex, endIndex, startVertIndex, endVertIndex );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setResolution( int res ) {
    setResolution(res, res, res);
}

//--------------------------------------------------------------
void ofBoxPrimitive::setResolution( int resX, int resY, int resZ ) {
    ofPrimitiveBase::setResolution(resX, resY, resZ);
    set( getWidth(), getHeight(), getDepth() );
}

//----------------------------------------------------------
void ofBoxPrimitive::setMode( ofPrimitiveMode mode ) {
    // only supports triangles //
    setResolution( getResolution().x, getResolution().y, getResolution().z );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setSideColor( int sideIndex, ofColor color ) {
    if(sideIndex < 0 || sideIndex >= SIDES_TOTAL) {
        ofLog(OF_LOG_WARNING) << "ofBoxPrimitive :: setSideColor : sideIndex out of bounds, setting SIDE_FRONT ";
        sideIndex = SIDE_FRONT;
    }
    setColorForIndices( _strides[sideIndex][0], _strides[sideIndex][0]+_strides[sideIndex][1], color );
}

//--------------------------------------------------------------
float ofBoxPrimitive::getWidth() {
    return _size.x;
}

//--------------------------------------------------------------
float ofBoxPrimitive::getHeight() {
    return _size.y;
}

//--------------------------------------------------------------
float ofBoxPrimitive::getDepth() {
    return _size.z;
}

//--------------------------------------------------------------
ofVec3f ofBoxPrimitive::getSize() const {
    return _size;
}

















