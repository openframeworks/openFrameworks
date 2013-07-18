//
//  of3dPrimitive.cpp
//  openFrameworksLib
//
//  Created by Nick Hardeman on 9/14/12.
//
//

#include "of3dPrimitives.h"
#include "ofGraphics.h"

of3dPrimitive::of3dPrimitive()
:usingVbo(true)
,mesh(new ofVboMesh)
{
    setScale(1.0, 1.0, 1.0);
}

//----------------------------------------------------------
of3dPrimitive::~of3dPrimitive() {
    
}

//----------------------------------------------------------
of3dPrimitive::of3dPrimitive(const of3dPrimitive & mom){
    texCoords = mom.texCoords;
    usingVbo = mom.usingVbo;
	if(usingVbo){
		mesh = ofPtr<ofMesh>(new ofVboMesh);
	}else{
		mesh = ofPtr<ofMesh>(new ofMesh);
	}
	*mesh = *mom.mesh;
}

//----------------------------------------------------------
of3dPrimitive & of3dPrimitive::operator=(const of3dPrimitive & mom){
	if(&mom!=this){
		texCoords = mom.texCoords;
		setUseVbo(mom.usingVbo);
		*mesh = *mom.mesh;
	}
    return *this;
}

// GETTERS //
//----------------------------------------------------------
ofMesh* of3dPrimitive::getMeshPtr() {
    return mesh.get();
}
//----------------------------------------------------------
ofMesh& of3dPrimitive::getMesh() {
    return *mesh;
}

//----------------------------------------------------------
ofVec4f* of3dPrimitive::getTexCoordsPtr() {
    return& texCoords;
}

//----------------------------------------------------------
ofVec4f& of3dPrimitive::getTexCoords() {
    return texCoords;
}

//----------------------------------------------------------
vector<ofIndexType> of3dPrimitive::getIndices( int startIndex, int endIndex ) {
    vector<ofIndexType> indices;
    indices.assign( getMesh().getIndices().begin()+startIndex, getMesh().getIndices().begin()+endIndex );
    return indices;
}


//----------------------------------------------------------
bool of3dPrimitive::hasScaling() {
    ofVec3f scale = getScale();
    return (scale.x != 1.f || scale.y != 1.f || scale.z != 1.f);
}
//----------------------------------------------------------
bool of3dPrimitive::hasNormalsEnabled() {
    return getMesh().hasNormals();
}

//----------------------------------------------------------
void of3dPrimitive::enableNormals() {
    getMesh().enableNormals();
}
//----------------------------------------------------------
void of3dPrimitive::enableTextures() {
    getMesh().enableTextures();
}
//----------------------------------------------------------
void of3dPrimitive::enableColors() {
    getMesh().enableColors();
}
//----------------------------------------------------------
void of3dPrimitive::disableNormals() {
    getMesh().disableNormals();
}
//----------------------------------------------------------
void of3dPrimitive::disableTextures() {
    getMesh().disableTextures();
}
//----------------------------------------------------------
void of3dPrimitive::disableColors() {
    getMesh().disableColors();
}




// SETTERS //

//----------------------------------------------------------
void of3dPrimitive::mapTexCoords( float u1, float v1, float u2, float v2 ) {
    //setTexCoords( u1, v1, u2, v2 );
    ofVec4f prevTcoord = getTexCoords();
    
    for(int j = 0; j < getMesh().getNumTexCoords(); j++ ) {
        ofVec2f tcoord = getMesh().getTexCoord(j);
        tcoord.x = ofMap(tcoord.x, prevTcoord.x, prevTcoord.z, u1, u2);
        tcoord.y = ofMap(tcoord.y, prevTcoord.y, prevTcoord.w, v1, v2);
        
        getMesh().setTexCoord(j, tcoord);
    }
    
    texCoords.set(u1, v1, u2, v2);
}

//----------------------------------------------------------
void of3dPrimitive::mapTexCoordsFromTexture( ofTexture& inTexture ) {
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
void of3dPrimitive::normalizeAndApplySavedTexCoords() {
    ofVec4f tcoords = getTexCoords();
    // when a new mesh is created, it uses normalized tex coords, we need to reset them
    // but save the ones used previously //
    texCoords.set(0,0,1,1);
    mapTexCoords(tcoords.x, tcoords.y, tcoords.z, tcoords.w);
}




//--------------------------------------------------------------
void of3dPrimitive::drawVertices() {
	draw(OF_MESH_POINTS);
}

//--------------------------------------------------------------
void of3dPrimitive::drawWireframe() {
	draw(OF_MESH_WIREFRAME);
}

//--------------------------------------------------------------
void of3dPrimitive::drawFaces() {
	draw(OF_MESH_FILL);
}

//--------------------------------------------------------------
void of3dPrimitive::draw() {
	draw(OF_MESH_FILL);
}

//--------------------------------------------------------------
void of3dPrimitive::draw(ofPolyRenderMode renderType) {
    // ofNode applies all of the tranformations needed, included scale //
    ofNode::transformGL();
    ofGetCurrentRenderer()->draw(*this, renderType);
    ofNode::restoreTransformGL();
}

//--------------------------------------------------------------
void of3dPrimitive::drawNormals(float length, bool bFaceNormals) {
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
        ofLogWarning("of3dPrimitive") << "drawNormals(): mesh normals are disabled";
    }
    
    
    ofNode::restoreTransformGL();
}

//--------------------------------------------------------------
void of3dPrimitive::drawAxes(float a_size) {
    ofNode::transformGL();
    ofDrawAxis(a_size);
    ofNode::restoreTransformGL();
}

//--------------------------------------------------------------
void of3dPrimitive::setUseVbo(bool useVbo){
	if(useVbo!=usingVbo){
		ofPtr<ofMesh> newMesh;
		if(useVbo){
			newMesh = ofPtr<ofMesh>(new ofVboMesh);
		}else{
			newMesh = ofPtr<ofMesh>(new ofMesh);
		}
		*newMesh = *mesh;
		mesh = newMesh;
	}
	usingVbo = useVbo;
}

//--------------------------------------------------------------
bool of3dPrimitive::isUsingVbo(){
	return usingVbo;
}

// PLANE PRIMITIVE //
//--------------------------------------------------------------
ofPlanePrimitive::ofPlanePrimitive() {
    texCoords = ofVec4f(0,0,1,1);
    set( 200, 100, 6, 3);
}

//--------------------------------------------------------------
ofPlanePrimitive::ofPlanePrimitive(float width, float height, int columns, int rows, ofPrimitiveMode mode) {
    texCoords = ofVec4f(0,0,1,1);
    set(width, height, columns, rows, mode);
}

//--------------------------------------------------------------
ofPlanePrimitive::~ofPlanePrimitive() {}

//--------------------------------------------------------------
void ofPlanePrimitive::set(float _width, float _height, int columns, int rows, ofPrimitiveMode mode) {
    
    width  = _width;
    height = _height;
    resolution.set( columns, rows );
    
    getMesh() = ofMesh::plane( getWidth(), getHeight(), getResolution().x, getResolution().y, mode );
    
    normalizeAndApplySavedTexCoords();
    
}

//--------------------------------------------------------------
void ofPlanePrimitive::set( float _width, float height ) {
    width = _width;
    setHeight(height);
}

//--------------------------------------------------------------
void ofPlanePrimitive::setWidth( float _width ) {
    width = _width;
    setResolution( getResolution().x, getResolution().y );
}

//--------------------------------------------------------------
void ofPlanePrimitive::setHeight(float _height) {
    height = _height;
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
    resolution.set( columns, rows );
    ofPrimitiveMode mode = getMesh().getMode();
    
    set( getWidth(), getHeight(), getResolution().x, getResolution().y, mode );
}

//--------------------------------------------------------------
void ofPlanePrimitive::setMode(ofPrimitiveMode mode) {
    ofPrimitiveMode currMode = getMesh().getMode();
    
    if( mode != currMode )
        set( getWidth(), getHeight(), getResolution().x, getResolution().y, mode );
}

//--------------------------------------------------------------
int ofPlanePrimitive::getNumColumns() {
    return (int)resolution.x;
}

//--------------------------------------------------------------
int ofPlanePrimitive::getNumRows() {
    return (int)resolution.y;
}

//--------------------------------------------------------------
ofVec2f ofPlanePrimitive::getResolution() {
    return resolution;
}

//--------------------------------------------------------------
float ofPlanePrimitive::getWidth() {
    return width;
}

//--------------------------------------------------------------
float ofPlanePrimitive::getHeight() {
    return height;
}





// SPHERE PRIMITIVE //
//----------------------------------------------------------
ofSpherePrimitive::ofSpherePrimitive() {
    texCoords = ofVec4f(0,0,1,1);
    radius = 20;
    setResolution( 16 );
}

//----------------------------------------------------------
ofSpherePrimitive::ofSpherePrimitive( float _radius, int res, ofPrimitiveMode mode ) {
    radius = _radius;
    texCoords = ofVec4f(0,0,1,1);
    setResolution( res );
}

//----------------------------------------------------------
ofSpherePrimitive::~ofSpherePrimitive() {
    
}

//----------------------------------------------------------
void ofSpherePrimitive::set( float _radius, int res, ofPrimitiveMode mode ) {
    radius     = _radius;
    resolution = res;

    getMesh() = ofMesh::sphere( getRadius(), getResolution(), mode );
    
    normalizeAndApplySavedTexCoords();
}

//----------------------------------------------------------
void ofSpherePrimitive::setResolution( int res ) {
    resolution             = res;
    ofPrimitiveMode mode   = getMesh().getMode();
    
    set(getRadius(), getResolution(), mode );
}

//----------------------------------------------------------
void ofSpherePrimitive::setMode( ofPrimitiveMode mode ) {
    ofPrimitiveMode currMode = getMesh().getMode();
    if(currMode != mode)
        set(getRadius(), getResolution(), mode );
}

//----------------------------------------------------------
void ofSpherePrimitive::setRadius(float _radius) {
    radius = _radius;
    setResolution( getResolution() );
}

//----------------------------------------------------------
float ofSpherePrimitive::getRadius() {
    return radius;
}

//----------------------------------------------------------
int ofSpherePrimitive::getResolution() {
    return resolution;
}


// ICO SPHERE //
//----------------------------------------------------------
ofIcoSpherePrimitive::ofIcoSpherePrimitive() {
    texCoords  = ofVec4f(0,0,1,1);
    radius     = 20;
    setResolution( 2 );
}

//----------------------------------------------------------
ofIcoSpherePrimitive::ofIcoSpherePrimitive( float _radius, int iterations ) {
    texCoords  = ofVec4f(0,0,1,1);
    radius     = _radius;
    setResolution( iterations );
}

//----------------------------------------------------------
ofIcoSpherePrimitive::~ofIcoSpherePrimitive() {
    
}

//----------------------------------------------------------
void ofIcoSpherePrimitive::set(float _radius, int res ) {
    radius = _radius;
    setResolution(res);
}

//----------------------------------------------------------
void ofIcoSpherePrimitive::setResolution( int iterations ) {
    // store the number of iterations in the resolution //
    resolution = iterations;
    
    getMesh() = ofMesh::icosphere( getRadius(), getResolution() );
    normalizeAndApplySavedTexCoords();
}

//----------------------------------------------------------
void ofIcoSpherePrimitive::setMode( ofPrimitiveMode mode ) {
    // ofIcoSpherePrimitive only works with OF_PRIMITIVE_TRIANGLES //
    setResolution( getResolution() );
}

//----------------------------------------------------------
void ofIcoSpherePrimitive::setRadius(float _radius) {
    radius = _radius;
    setResolution( getResolution() );
}

//----------------------------------------------------------
float ofIcoSpherePrimitive::getRadius() {
    return radius;
}

//----------------------------------------------------------
int ofIcoSpherePrimitive::getResolution() {
    return resolution;
}



//--------------------------------------------------------------
ofCylinderPrimitive::ofCylinderPrimitive() {
    texCoords = ofVec4f(0,0,1,1);
    set( 60, 80, 6, 3, 2, true );
}

//--------------------------------------------------------------
ofCylinderPrimitive::ofCylinderPrimitive( float radius, float height, int radiusSegments, int heightSegments, int capSegments, bool bCapped, ofPrimitiveMode mode ) {
    texCoords = ofVec4f(0,0,1,1);
    set( radius, height, radiusSegments, heightSegments, capSegments, bCapped, mode );
}

//--------------------------------------------------------------
ofCylinderPrimitive::~ofCylinderPrimitive() {}

//--------------------------------------------------------------
void ofCylinderPrimitive::set(float _radius, float _height, int radiusSegments, int heightSegments, int capSegments, bool _bCapped, ofPrimitiveMode mode) {
    radius = _radius;
    height = _height;
    bCapped = _bCapped;
    resolution.set( radiusSegments, heightSegments, capSegments );
    
    int resX = getResolution().x;
    int resY = getResolution().y-1;
    int resZ = getResolution().z-1;
    
    int indexStep = 2;
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        indexStep = 6;
        resX = resX-1;
    }
    
    // 0 -> top cap
    strides[0][0] = 0;
    strides[0][1] = resX * resZ * indexStep;
    vertices[0][0] = 0;
    vertices[0][1] = getResolution().x * getResolution().z;
    
    // 1 -> cylinder //
    if(bCapped) {
        strides[1][0] = strides[0][0] + strides[0][1];
        vertices[1][0] = vertices[0][0] + vertices[0][1];
    } else {
        strides[1][0] = 0;
        vertices[1][0] = 0;
    }
    strides[1][1] = resX * resY * indexStep;
    vertices[1][1] = getResolution().x * getResolution().y;
    
    // 2 -> bottom cap
    strides[2][0] = strides[1][0] + strides[1][1];
    strides[2][1] = resX * resZ * indexStep;
    vertices[2][0] = vertices[1][0]+vertices[1][1];
    vertices[2][1] = getResolution().x * getResolution().z;
    
    
    getMesh() = ofMesh::cylinder( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, getCapped(), mode );
    
    normalizeAndApplySavedTexCoords();
    
}

//--------------------------------------------------------------
void ofCylinderPrimitive::set( float _radius, float height, bool _bCapped ) {
    radius = _radius;
    bCapped = _bCapped;
    setHeight( height );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setRadius( float _radius ) {
    radius = _radius;
    setResolution( getResolution().x, getResolution().y, getResolution().z );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setHeight( float _height ) {
    height = _height;
    setResolution(getResolution().x, getResolution().y, getResolution().z);
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setCapped(bool _bCapped) {
    bCapped = _bCapped;
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
    ofPrimitiveMode mode = getMesh().getMode();
    set( getRadius(), getHeight(), radiusSegments, heightSegments, capSegments, getCapped(), mode );
}

//----------------------------------------------------------
void ofCylinderPrimitive::setMode( ofPrimitiveMode mode ) {
    ofPrimitiveMode currMode = getMesh().getMode();
    if(currMode != mode)
        set( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, getCapped(), mode );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setTopCapColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofCylinderPrimitive") << "setTopCapColor(): must be in triangle strip mode";
    }
    getMesh().setColorForIndices( strides[0][0], strides[0][0]+strides[0][1], color );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setCylinderColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofCylinderPrimitive") << "setCylinderMode(): must be in triangle strip mode";
    }
    getMesh().setColorForIndices( strides[1][0], strides[1][0]+strides[1][1], color );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setBottomCapColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofCylinderPrimitive") << "setBottomCapColor(): must be in triangle strip mode";
    }
    getMesh().setColorForIndices( strides[2][0], strides[2][0]+strides[2][1], color );
}

//--------------------------------------------------------------
vector<ofIndexType> ofCylinderPrimitive::getTopCapIndices() {
    return of3dPrimitive::getIndices( strides[0][0], strides[0][0] + strides[0][1] );
}

//--------------------------------------------------------------
ofMesh ofCylinderPrimitive::getTopCapMesh() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofCylinderPrimitive") << "getTopCapMesh(): must be in triangle strip mode";
        return ofMesh();
    }
    return getMesh().getMeshForIndices( strides[0][0], strides[0][0]+strides[0][1],
                             vertices[0][0], vertices[0][0]+vertices[0][1] );
}

//--------------------------------------------------------------
vector<ofIndexType> ofCylinderPrimitive::getCylinderIndices() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofCylinderPrimitive") << "getCylinderIndices(): must be in triangle strip mode";
    }
    return of3dPrimitive::getIndices( strides[1][0], strides[1][0] + strides[1][1] );
}

//--------------------------------------------------------------
ofMesh ofCylinderPrimitive::getCylinderMesh() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofCylinderPrimitive") << "setCylinderMesh(): must be in triangle strip mode";
        return ofMesh();
    }
    return getMesh().getMeshForIndices( strides[1][0], strides[1][0]+strides[1][1],
                             vertices[1][0], vertices[1][0]+vertices[1][1] );
}

//--------------------------------------------------------------
vector<ofIndexType> ofCylinderPrimitive::getBottomCapIndices() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofCylinderPrimitive") << "getBottomCapIndices(): must be in triangle strip mode";
    }
    return of3dPrimitive::getIndices( strides[2][0], strides[2][0] + strides[2][1] );
}

//--------------------------------------------------------------
ofMesh ofCylinderPrimitive::getBottomCapMesh() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofCylinderPrimitive") << "getBottomCapMesh(): must be in triangle strip mode";
        return ofMesh();
    }
    return getMesh().getMeshForIndices( strides[2][0], strides[2][0]+strides[2][1],
                             vertices[2][0], vertices[2][0]+vertices[2][1] );
}

//--------------------------------------------------------------
int ofCylinderPrimitive::getResolutionRadius() {
    return (int)resolution.x;
}

//--------------------------------------------------------------
int ofCylinderPrimitive::getResolutionHeight() {
    return (int)resolution.y;
}

//--------------------------------------------------------------
int ofCylinderPrimitive::getResolutionCap() {
    return (int)resolution.z;
}

//--------------------------------------------------------------
ofVec3f ofCylinderPrimitive::getResolution() {
    return resolution;
}

//--------------------------------------------------------------
float ofCylinderPrimitive::getHeight() {
    return height;
}

//--------------------------------------------------------------
float ofCylinderPrimitive::getRadius() {
    return radius;
}

//--------------------------------------------------------------
bool ofCylinderPrimitive::getCapped() {
    return bCapped;
}





// Cone Primitive //
//--------------------------------------------------------------
ofConePrimitive::ofConePrimitive() {
    texCoords = ofVec4f(0,0,1,1);
    set( 20, 70, 8, 3, 2 );
}

//--------------------------------------------------------------
ofConePrimitive::ofConePrimitive( float radius, float height, int radiusSegments, int heightSegments, int capSegments, ofPrimitiveMode mode ) {
    texCoords = ofVec4f(0,0,1,1);
    set( radius, height, radiusSegments, heightSegments, capSegments, mode );
}

//--------------------------------------------------------------
ofConePrimitive::~ofConePrimitive() {}

//--------------------------------------------------------------
void ofConePrimitive::set( float _radius, float _height, int radiusSegments, int heightSegments, int capSegments, ofPrimitiveMode mode ) {
    radius = _radius;
    height = _height;
    resolution.set(radiusSegments, heightSegments, capSegments);
    
    int resX = getResolution().x;
    int resY = getResolution().y-1;
    int resZ = getResolution().z-1;
    
    int indexStep = 2;
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        indexStep = 6;
        resX = resX-1;
    }
    
    strides[ 0 ][0] = 0;
    strides[ 0 ][1] = (resX)*(resY) * indexStep;
    vertices[0][0] = 0;
    vertices[0][1] = getResolution().x * getResolution().y;
    
    strides[ 1 ][0] = strides[ 0 ][0] + strides[ 0 ][1];
    strides[ 1 ][1] = (resX)*(resZ) * indexStep;
    vertices[1][0] = vertices[0][0] + vertices[0][1];
    vertices[1][1] = getResolution().x * getResolution().z;
    
    getMesh() = ofMesh::cone( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, mode );
    
    normalizeAndApplySavedTexCoords();
    
}

//--------------------------------------------------------------
void ofConePrimitive::set( float _radius, float _height ) {
    radius = _radius;
    height = _height;
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
    ofPrimitiveMode mode = getMesh().getMode();
    set( getRadius(), getHeight(), radiusRes, heightRes, capRes, mode );
}

//----------------------------------------------------------
void ofConePrimitive::setMode( ofPrimitiveMode mode ) {
    ofPrimitiveMode currMode = getMesh().getMode();
    if(currMode != mode)
        set( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, mode );
}

//--------------------------------------------------------------
void ofConePrimitive::setRadius( float _radius ) {
    radius = _radius;
    setResolution(getResolution().x, getResolution().y, getResolution().z);
}

//--------------------------------------------------------------
void ofConePrimitive::setHeight(float _height) {
    height = _height;
    setResolution(getResolution().x, getResolution().y, getResolution().z);
}

//--------------------------------------------------------------
void ofConePrimitive::setTopColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofConePrimitive") << "setTopColor(): must be in triangle strip mode";
    }
    getMesh().setColorForIndices( strides[0][0], strides[0][0]+strides[0][1], color );
}

//--------------------------------------------------------------
void ofConePrimitive::setCapColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofConePrimitive") << "setCapColor(): must be in triangle strip mode";
    }
    getMesh().setColorForIndices( strides[1][0], strides[1][0]+strides[1][1], color );
}

//--------------------------------------------------------------
vector<ofIndexType> ofConePrimitive::getConeIndices() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofConePrimitive") << "getConeIndices(): must be in triangle strip mode";
    }
    return of3dPrimitive::getIndices(strides[0][0], strides[0][0]+strides[0][1]);
}

//--------------------------------------------------------------
ofMesh ofConePrimitive::getConeMesh() {
    int startIndex  = strides[0][0];
    int endIndex    = startIndex + strides[0][1];
    
    int startVertIndex  = vertices[0][0];
    int endVertIndex    = startVertIndex + vertices[0][1];
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofConePrimitive") << "getConeMesh(): must be in triangle strip mode";
        return ofMesh();
    }
    return getMesh().getMeshForIndices( startIndex, endIndex, startVertIndex, endVertIndex );
}

//--------------------------------------------------------------
vector<ofIndexType> ofConePrimitive::getCapIndices() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofConePrimitive") << "getCapIndices(): must be in triangle strip mode";
    }
    return of3dPrimitive::getIndices( strides[1][0], strides[1][0] + strides[1][1] );
}

//--------------------------------------------------------------
ofMesh ofConePrimitive::getCapMesh() {
    int startIndex  = strides[1][0];
    int endIndex    = startIndex + strides[1][1];
    
    int startVertIndex  = vertices[1][0];
    int endVertIndex    = startVertIndex + vertices[1][1];
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLogWarning("ofConePrimitive") << "getCapMesh(): must be in triangle strip mode";
        return ofMesh();
    }
    return getMesh().getMeshForIndices( startIndex, endIndex, startVertIndex, endVertIndex );
}

//--------------------------------------------------------------
int ofConePrimitive::getResolutionRadius() {
    return (int)resolution.x;
}

//--------------------------------------------------------------
int ofConePrimitive::getResolutionHeight() {
    return (int)resolution.y;
}

//--------------------------------------------------------------
int ofConePrimitive::getResolutionCap() {
    return (int)resolution.z;
}

//--------------------------------------------------------------
ofVec3f ofConePrimitive::getResolution() {
    return resolution;
}

//--------------------------------------------------------------
float ofConePrimitive::getRadius() {
    return radius;
}

//--------------------------------------------------------------
float ofConePrimitive::getHeight() {
    return height;
}





// BOX PRIMITIVE //
//--------------------------------------------------------------
ofBoxPrimitive::ofBoxPrimitive() {
    texCoords = ofVec4f(0,0,1,1);
    set(100, 100, 100, 2, 2, 2);
}

//--------------------------------------------------------------
ofBoxPrimitive::ofBoxPrimitive( float width, float height, float depth, int resWidth, int resHeight, int resDepth ) {
    texCoords = ofVec4f(0,0,1,1);
    set(width, height, depth, resWidth, resHeight, resDepth );
}

//--------------------------------------------------------------
ofBoxPrimitive::~ofBoxPrimitive() {}

//--------------------------------------------------------------
void ofBoxPrimitive::set( float width, float height, float depth, int resWidth, int resHeight, int resDepth) {
    
    size.x = width;
    size.y = height;
    size.z = depth;
    
    resolution.set( resWidth, resHeight, resDepth );
    
    int resX = getResolution().x;
    int resY = getResolution().y;
    int resZ = getResolution().z;
    
    //FRONT, resY, resX
    strides[ SIDE_FRONT ][0] = 0;
    strides[ SIDE_FRONT ][1] = (resY-1)*(resX-1)*6;
    vertices[SIDE_FRONT][0] = 0;
    vertices[SIDE_FRONT][1] = resX * resY;
    
    //RIGHT, resY, resZ
    strides[ SIDE_RIGHT ][0] = strides[ SIDE_FRONT ][0] + strides[ SIDE_FRONT ][1];
    strides[ SIDE_RIGHT ][1] = (resY-1)*(resZ-1)*6;
    vertices[SIDE_RIGHT][0] = vertices[SIDE_FRONT][0] + vertices[SIDE_FRONT][1];
    vertices[SIDE_RIGHT][1] = resY * resZ;
    
    //LEFT, resY, resZ
    strides[ SIDE_LEFT ][0] = strides[ SIDE_RIGHT ][0] + strides[ SIDE_RIGHT ][1];
    strides[ SIDE_LEFT ][1] = (resY-1)*(resZ-1)*6;
    vertices[SIDE_LEFT][0] = vertices[SIDE_RIGHT][0] + vertices[SIDE_RIGHT][1];
    vertices[SIDE_LEFT][1] = resY * resZ;
    
    //BACK, resY, resX
    strides[ SIDE_BACK ][0] = strides[ SIDE_LEFT ][0] + strides[ SIDE_LEFT ][1];
    strides[ SIDE_BACK ][1] = (resY-1)*(resX-1)*6;
    vertices[SIDE_BACK][0] = vertices[SIDE_LEFT][0] + vertices[SIDE_LEFT][1];
    vertices[SIDE_BACK][1] = resY * resX;
    
    //TOP, resZ, resX
    strides[ SIDE_TOP ][0] = strides[ SIDE_BACK ][0] + strides[ SIDE_BACK ][1];
    strides[ SIDE_TOP ][1] = (resZ-1)*(resX-1)*6;
    vertices[SIDE_TOP][0] = vertices[SIDE_BACK][0] + vertices[SIDE_BACK][1];
    vertices[SIDE_TOP][1] = resZ * resX;
    
    //BOTTOM, resZ, resX
    strides[ SIDE_BOTTOM ][0] = strides[ SIDE_TOP ][0]+strides[ SIDE_TOP ][1];
    strides[ SIDE_BOTTOM ][1] = (resZ-1)*(resX-1)*6;
    vertices[SIDE_BOTTOM][0] = vertices[SIDE_TOP][0] + vertices[SIDE_TOP][1];
    vertices[SIDE_BOTTOM][1] = resZ * resX;
    
    getMesh() = ofMesh::box( getWidth(), getHeight(), getDepth(), getResolution().x, getResolution().y, getResolution().z );
    
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
    size.x = a_width;
    set( getWidth(), getHeight(), getDepth() );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setHeight( float a_height ) {
    size.y = a_height;
    set( getWidth(), getHeight(), getDepth() );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setDepth( float a_depth ) {
    size.z = a_depth;
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
        ofLogWarning("ofBoxPrimitive") << "getSideIndices(): faceIndex out of bounds, returning SIDE_FRONT";
        sideIndex = SIDE_FRONT;
    }
    
    return getIndices(strides[sideIndex][0], strides[sideIndex][0]+strides[sideIndex][1]);
}

//--------------------------------------------------------------
ofMesh ofBoxPrimitive::getSideMesh( int sideIndex ) {
    
    if(sideIndex < 0 || sideIndex > SIDES_TOTAL) {
        ofLogWarning("ofBoxPrimitive") << "getSideMesh(): faceIndex out of bounds, using SIDE_FRONT";
        sideIndex = SIDE_FRONT;
    }
    int startIndex  = strides[sideIndex][0];
    int endIndex    = startIndex+strides[sideIndex][1];
    
    int startVertIndex  = vertices[sideIndex][0];
    int endVertIndex    = startVertIndex + vertices[sideIndex][1];
    
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
    resolution.set( resWidth, resHeight, resDepth );
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
        ofLogWarning("ofBoxPrimitive") << "setSideColor(): sideIndex out of bounds, setting SIDE_FRONT";
        sideIndex = SIDE_FRONT;
    }
    getMesh().setColorForIndices( strides[sideIndex][0], strides[sideIndex][0]+strides[sideIndex][1], color );
}

//--------------------------------------------------------------
int ofBoxPrimitive::getResolutionWidth() {
    return (int)resolution.x;
}

//--------------------------------------------------------------
int ofBoxPrimitive::getResolutionHeight() {
    return (int)resolution.y;
}

//--------------------------------------------------------------
int ofBoxPrimitive::getResolutionDepth() {
    return (int)resolution.z;
}

//--------------------------------------------------------------
ofVec3f ofBoxPrimitive::getResolution() {
    return resolution;
}

//--------------------------------------------------------------
float ofBoxPrimitive::getWidth() {
    return size.x;
}

//--------------------------------------------------------------
float ofBoxPrimitive::getHeight() {
    return size.y;
}

//--------------------------------------------------------------
float ofBoxPrimitive::getDepth() {
    return size.z;
}

//--------------------------------------------------------------
ofVec3f ofBoxPrimitive::getSize() const {
    return size;
}
