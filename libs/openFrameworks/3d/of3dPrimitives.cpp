//
//  ofPrimitiveBase.cpp
//  openFrameworksLib
//
//  Created by Nick Hardeman on 9/14/12.
//
//

#include "of3dPrimitives.h"
#include "ofGraphics.h"

ofPrimitiveBase::ofPrimitiveBase()
:usingVbo(true)
,_mesh(new ofVboMesh)
{
    setScale(1.0, 1.0, 1.0);
}

//----------------------------------------------------------
ofPrimitiveBase::~ofPrimitiveBase() {
    
}

//----------------------------------------------------------
ofPrimitiveBase::ofPrimitiveBase(const ofPrimitiveBase & mom){
    _texCoords = mom._texCoords;
    usingVbo = mom.usingVbo;
	if(usingVbo){
		_mesh = ofPtr<ofMesh>(new ofVboMesh);
	}else{
		_mesh = ofPtr<ofMesh>(new ofMesh);
	}
	*_mesh = *mom._mesh;
}

//----------------------------------------------------------
ofPrimitiveBase & ofPrimitiveBase::operator=(const ofPrimitiveBase & mom){
	if(&mom!=this){
		_texCoords = mom._texCoords;
		setUseVbo(mom.usingVbo);
		*_mesh = *mom._mesh;
	}
    return *this;
}

// GETTERS //
//----------------------------------------------------------
ofMesh* ofPrimitiveBase::getMeshPtr() {
    return _mesh.get();
}
//----------------------------------------------------------
ofMesh& ofPrimitiveBase::getMesh() {
    return *_mesh;
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
bool ofPrimitiveBase::hasScaling() {
    ofVec3f scale = getScale();
    return (scale.x != 1.f || scale.y != 1.f || scale.z != 1.f);
}
//----------------------------------------------------------
bool ofPrimitiveBase::hasNormalsEnabled() {
    return getMesh().hasNormals();
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
//void ofPrimitiveBase::setResolution( int resX, int resY, int resZ ) {
//    _resolution.set( resX, resY, resZ );
//}

//----------------------------------------------------------
void ofPrimitiveBase::mapTexCoords( float u1, float v1, float u2, float v2 ) {
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
void ofPrimitiveBase::mapTexCoordsFromTexture( ofTexture& inTexture ) {
    bool bNormalized = true;
#ifndef TARGET_OPENGLES
    bNormalized = (inTexture.getTextureData().textureTarget!=GL_TEXTURE_RECTANGLE_ARB);
#endif
    
    ofTextureData& tdata = inTexture.getTextureData();
    if(bNormalized)
        mapTexCoords( 0, 0, tdata.tex_t, tdata.tex_u );
    else
        mapTexCoords(0, 0, inTexture.getWidth(), inTexture.getHeight());
    
    ofVec4f tcoords = getTexCoords();
    mapTexCoords(tcoords.x, tcoords.y, tcoords.z, tcoords.w);
}

//----------------------------------------------------------
void ofPrimitiveBase::normalizeAndApplySavedTexCoords() {
    ofVec4f tcoords = getTexCoords();
    // when a new mesh is created, it uses normalized tex coords, we need to reset them
    // but save the ones used previously //
    _texCoords.set(0,0,1,1);
    mapTexCoords(tcoords.x, tcoords.y, tcoords.z, tcoords.w);
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
    
    if(getMesh().usingNormals()) {
        vector<ofVec3f>& normals    = getMesh().getNormals();
        vector<ofVec3f>& vertices   = getMesh().getVertices();
        ofVec3f normal;
        ofVec3f vert;
        
        normalsMesh.setMode( OF_PRIMITIVE_LINES );
        normalsMesh.getVertices().resize( normals.size() * 2);
        
        if(bFaceNormals) {
            for(int i = 0; i < (int)normals.size(); i++ ) {
                if(i % 3 == 0) {
                    vert = (vertices[i]+vertices[i+1]+vertices[i+2]) / 3;
                } else if(i % 3 == 1) {
                    vert = (vertices[i-1]+vertices[i]+vertices[i+1]) / 3;
                } else if ( i % 3 == 2) {
                    vert = (vertices[i-2]+vertices[i-1]+vertices[i]) / 3;
                }
                //vert = (vertices[i+0]+vertices[i+1]+vertices[i+2]) / 3;
                //vert = vertices[i];
                normalsMesh.setVertex(i*2, vert);
                normal = normals[i].getNormalized();
                normal *= length;
                normalsMesh.setVertex(i*2+1, normal+vert);
            }
        } else {
            for(int i = 0; i < (int)normals.size(); i++) {
                vert = vertices[i];
                normal = normals[i].normalized();
                normalsMesh.setVertex( i*2, vert);
                normal *= length;
                normalsMesh.setVertex(i*2+1, normal+vert);
            }
        }
        normalsMesh.draw();
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


void ofPrimitiveBase::setUseVbo(bool useVbo){
	if(useVbo!=usingVbo){
		ofPtr<ofMesh> newMesh;
		if(useVbo){
			newMesh = ofPtr<ofMesh>(new ofVboMesh);
		}else{
			newMesh = ofPtr<ofMesh>(new ofMesh);
		}
		*newMesh = *_mesh;
		_mesh = newMesh;
	}
	usingVbo = useVbo;
}

bool ofPrimitiveBase::isUsingVbo(){
	return usingVbo;
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
    _resolution.set( columns, rows );
    
    getMesh().clear();
    //_mesh = ofGetPlaneMesh( getWidth(), getHeight(), getResolution().x, getResolution().y, mode );
    *_mesh = ofMesh::plane( getWidth(), getHeight(), getResolution().x, getResolution().y, mode );
    
    normalizeAndApplySavedTexCoords();
    
}

//--------------------------------------------------------------
void ofPlanePrimitive::set( float width, float height ) {
    _width = width;
    setHeight(height);
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
void ofPlanePrimitive::resizeToTexture( ofTexture& inTexture, float scale ) {
    set(inTexture.getWidth() * scale, inTexture.getHeight() * scale);
    mapTexCoordsFromTexture( inTexture );
}

//--------------------------------------------------------------
void ofPlanePrimitive::setColumns( int columns ) {
    setResolution( columns, getNumRows() );
}

//--------------------------------------------------------------
void ofPlanePrimitive::setRows( int rows ) {
    setResolution( getNumColumns(), rows );
}

//--------------------------------------------------------------
void ofPlanePrimitive::setResolution( int columns, int rows ) {
    _resolution.set( columns, rows );
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
int ofPlanePrimitive::getNumColumns() {
    return (int)_resolution.x;
}

//--------------------------------------------------------------
int ofPlanePrimitive::getNumRows() {
    return (int)_resolution.y;
}

//--------------------------------------------------------------
ofVec2f ofPlanePrimitive::getResolution() {
    return _resolution;
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
    setResolution( 16 );
}

//----------------------------------------------------------
ofSpherePrimitive::ofSpherePrimitive( float radius, int res, ofPrimitiveMode mode ) {
    _radius = radius;
    _texCoords = ofVec4f(0,0,1,1);
    setResolution( res );
}

//----------------------------------------------------------
ofSpherePrimitive::~ofSpherePrimitive() {
    
}

//----------------------------------------------------------
void ofSpherePrimitive::set( float radius, int res, ofPrimitiveMode mode ) {
    _radius     = radius;
    _resolution = res;
    getMesh().clear();
    *_mesh = ofMesh::sphere( getRadius(), getResolution(), mode );
    
    normalizeAndApplySavedTexCoords();
}

//----------------------------------------------------------
void ofSpherePrimitive::setResolution( int res ) {
    //ofPrimitiveBase::setResolution(resX, resY, resZ);
    _resolution             = res;
    ofPrimitiveMode mode    = OF_PRIMITIVE_TRIANGLE_STRIP;
    
    mode = getMesh().getMode();
    
    set(getRadius(), getResolution(), mode );
}

//----------------------------------------------------------
void ofSpherePrimitive::setMode( ofPrimitiveMode mode ) {
    ofPrimitiveMode currMode = OF_PRIMITIVE_TRIANGLE_STRIP;
    mode = getMesh().getMode();
    if(currMode != mode)
        set(getRadius(), getResolution(), mode );
}

//----------------------------------------------------------
void ofSpherePrimitive::setRadius(float radius) {
    _radius = radius;
    setResolution( getResolution() );
}

//----------------------------------------------------------
float ofSpherePrimitive::getRadius() {
    return _radius;
}

//----------------------------------------------------------
int ofSpherePrimitive::getResolution() {
    return _resolution;
}


// ICO SPHERE //
//----------------------------------------------------------
ofIcoSpherePrimitive::ofIcoSpherePrimitive() {
    _texCoords  = ofVec4f(0,0,1,1);
    _radius     = 20;
    setResolution( 2 );
}

//----------------------------------------------------------
ofIcoSpherePrimitive::ofIcoSpherePrimitive( float radius, int iterations ) {
    _texCoords  = ofVec4f(0,0,1,1);
    _radius     = radius;
    setResolution( iterations );
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
    // store the number of iterations in the resolution //
    _resolution = iterations;
    
    getMesh().clear();
    
    *_mesh = ofMesh::icosphere( getRadius(), getResolution() );
    normalizeAndApplySavedTexCoords();
}

//----------------------------------------------------------
void ofIcoSpherePrimitive::setMode( ofPrimitiveMode mode ) {
    // ofIcoSpherePrimitive only works with OF_PRIMITIVE_TRIANGLES //
    setResolution( getResolution() );
}

//----------------------------------------------------------
void ofIcoSpherePrimitive::setRadius(float radius) {
    _radius = radius;
    setResolution( getResolution() );
}

//----------------------------------------------------------
float ofIcoSpherePrimitive::getRadius() {
    return _radius;
}

//----------------------------------------------------------
int ofIcoSpherePrimitive::getResolution() {
    return _resolution;
}



//--------------------------------------------------------------
ofCylinderPrimitive::ofCylinderPrimitive() {
    _texCoords = ofVec4f(0,0,1,1);
    set( 60, 80, 6, 3, 2, true );
}

//--------------------------------------------------------------
ofCylinderPrimitive::ofCylinderPrimitive( float radius, float height, int radiusSegments, int heightSegments, int capSegments, bool bCapped, ofPrimitiveMode mode ) {
    _texCoords = ofVec4f(0,0,1,1);
    set( radius, height, radiusSegments, heightSegments, capSegments, bCapped, mode );
}

//--------------------------------------------------------------
ofCylinderPrimitive::~ofCylinderPrimitive() {}

//--------------------------------------------------------------
void ofCylinderPrimitive::set(float radius, float height, int radiusSegments, int heightSegments, int capSegments, bool bCapped, ofPrimitiveMode mode) {
    _radius = radius;
    _height = height;
    _bCapped = bCapped;
    //ofPrimitiveBase::setResolution( radiusSegments, heightSegments, numCapSegments );
    _resolution.set( radiusSegments, heightSegments, capSegments );
    
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
    *_mesh = ofMesh::cylinder( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, getCapped(), mode );
    
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
    setResolution( getResolution().x, getResolution().y, getResolution().z );
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
void ofCylinderPrimitive::setResolutionRadius( int radiusRes ) {
    setResolution( radiusRes, getResolutionHeight(), getResolutionCap() );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setResolutionHeight( int heightRes ) {
    setResolution( getResolutionRadius(), heightRes, getResolutionCap() );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setResolutionCap( int capRes ) {
    setResolution( getResolutionRadius(), getResolutionHeight(), capRes );
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
    getMesh().setColorForIndices( _strides[0][0], _strides[0][0]+_strides[0][1], color );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setCylinderColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
    }
    getMesh().setColorForIndices( _strides[1][0], _strides[1][0]+_strides[1][1], color );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setBottomCapColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
    }
    getMesh().setColorForIndices( _strides[2][0], _strides[2][0]+_strides[2][1], color );
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
    return getMesh().getMeshForIndices( _strides[0][0], _strides[0][0]+_strides[0][1],
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
    return getMesh().getMeshForIndices( _strides[1][0], _strides[1][0]+_strides[1][1],
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
    return getMesh().getMeshForIndices( _strides[2][0], _strides[2][0]+_strides[2][1],
                             _vertices[2][0], _vertices[2][0]+_vertices[2][1] );
}

//--------------------------------------------------------------
int ofCylinderPrimitive::getResolutionRadius() {
    return (int)_resolution.x;
}

//--------------------------------------------------------------
int ofCylinderPrimitive::getResolutionHeight() {
    return (int)_resolution.y;
}

//--------------------------------------------------------------
int ofCylinderPrimitive::getResolutionCap() {
    return (int)_resolution.z;
}

//--------------------------------------------------------------
ofVec3f ofCylinderPrimitive::getResolution() {
    return _resolution;
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
    _resolution.set(radiusSegments, heightSegments, capSegments);
    
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
    *_mesh = ofMesh::cone( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, mode );
    
    normalizeAndApplySavedTexCoords();
    
}

//--------------------------------------------------------------
void ofConePrimitive::set( float radius, float height ) {
    _radius = radius;
    _height = height;
    setResolution( getResolution().x, getResolution().y, getResolution().z );
}

//--------------------------------------------------------------
void ofConePrimitive::setResolutionRadius( int radiusRes ) {
    setResolution( radiusRes, getResolutionHeight(), getResolutionCap() );
}

//--------------------------------------------------------------
void ofConePrimitive::setResolutionHeight( int heightRes ) {
    setResolution( getResolutionRadius(), heightRes, getResolutionCap() );
}

//--------------------------------------------------------------
void ofConePrimitive::setResolutionCap( int capRes ) {
    setResolution( getResolutionRadius(), getResolutionHeight(), capRes );
}

//--------------------------------------------------------------
void ofConePrimitive::setResolution( int radiusRes, int heightRes, int capRes ) {
    ofPrimitiveMode mode = OF_PRIMITIVE_TRIANGLE_STRIP;
    mode = getMesh().getMode();
    set( getRadius(), getHeight(), radiusRes, heightRes, capRes, mode );
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
    getMesh().setColorForIndices( _strides[0][0], _strides[0][0]+_strides[0][1], color );
}

//--------------------------------------------------------------
void ofConePrimitive::setCapColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofConePrimitive : must be in triangle strip mode" << endl;
    }
    getMesh().setColorForIndices( _strides[1][0], _strides[1][0]+_strides[1][1], color );
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
    return getMesh().getMeshForIndices( startIndex, endIndex, startVertIndex, endVertIndex );
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
    return getMesh().getMeshForIndices( startIndex, endIndex, startVertIndex, endVertIndex );
}

//--------------------------------------------------------------
int ofConePrimitive::getResolutionRadius() {
    return (int)_resolution.x;
}

//--------------------------------------------------------------
int ofConePrimitive::getResolutionHeight() {
    return (int)_resolution.y;
}

//--------------------------------------------------------------
int ofConePrimitive::getResolutionCap() {
    return (int)_resolution.z;
}

//--------------------------------------------------------------
ofVec3f ofConePrimitive::getResolution() {
    return _resolution;
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
    
    _resolution.set( resWidth, resHeight, resDepth );
    
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
    
    _mesh->clear();
    //_mesh = ofGetBoxMesh( getWidth(), getHeight(), getDepth(), getResolution().x, getResolution().y, getResolution().z );
    *_mesh = ofMesh::box( getWidth(), getHeight(), getDepth(), getResolution().x, getResolution().y, getResolution().z );
    
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
    mapTexCoordsFromTexture( inTexture );
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
    
    return getMesh().getMeshForIndices( startIndex, endIndex, startVertIndex, endVertIndex );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setResolution( int res ) {
    setResolution(res, res, res);
}

//--------------------------------------------------------------
void ofBoxPrimitive::setResolutionWidth( int widthRes ) {
    setResolution( widthRes, getResolutionHeight(), getResolutionDepth() );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setResolutionHeight( int heightRes ) {
    setResolution( getResolutionWidth(), heightRes, getResolutionDepth() );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setResolutionDepth( int depthRes ) {
    setResolution( getResolutionWidth(), getResolutionHeight(), depthRes );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setResolution( int resWidth, int resHeight, int resDepth ) {
    _resolution.set( resWidth, resHeight, resDepth );
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
    getMesh().setColorForIndices( _strides[sideIndex][0], _strides[sideIndex][0]+_strides[sideIndex][1], color );
}

//--------------------------------------------------------------
int ofBoxPrimitive::getResolutionWidth() {
    return (int)_resolution.x;
}

//--------------------------------------------------------------
int ofBoxPrimitive::getResolutionHeight() {
    return (int)_resolution.y;
}

//--------------------------------------------------------------
int ofBoxPrimitive::getResolutionDepth() {
    return (int)_resolution.z;
}

//--------------------------------------------------------------
ofVec3f ofBoxPrimitive::getResolution() {
    return _resolution;
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

















