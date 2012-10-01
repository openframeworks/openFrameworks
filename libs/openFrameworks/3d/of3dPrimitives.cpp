//
//  of3dModel.cpp
//  openFrameworksLib
//
//  Created by Nick Hardeman on 9/14/12.
//
//

#include "of3dPrimitives.h"
#include "ofGraphics.h"

of3dModel::of3dModel() {
    setScale(1.0, 1.0, 1.0);
}
of3dModel::~of3dModel() {
    
}

// ADDERS //
//----------------------------------------------------------
void of3dModel::addMesh( ofMesh& mesh) {
    _meshes.push_back(mesh);
}


// GETTERS //
//----------------------------------------------------------
int of3dModel::getNumMeshes() {
    return (int)_meshes.size();
}
//----------------------------------------------------------
ofMesh* of3dModel::getMeshPtr(int meshIndex) {
    if(meshIndex < 0 || meshIndex >= _meshes.size() ) {
        ofLog(OF_LOG_ERROR, "of3dModel :: getMeshPtr()") << " : Mesh index "<<meshIndex<<" out of bounds, returning NULL";
        return NULL;
    }
    return& _meshes[meshIndex];
}
//----------------------------------------------------------
ofMesh& of3dModel::getMesh( int meshIndex ) {
    if(meshIndex < 0 || meshIndex >= _meshes.size() ) {
        ofLog(OF_LOG_ERROR, "of3dModel :: getMesh()") << " : Mesh index "<<meshIndex<<" out of bounds, returning";
        return;
    }
    return _meshes[meshIndex];
}
//----------------------------------------------------------
vector<ofMesh>& of3dModel::getMeshes() {
    return _meshes;
}


//----------------------------------------------------------
int of3dModel::getNumTextures() {
    return (int)_textures.size();
}
//----------------------------------------------------------
ofTexture* of3dModel::getTexturePtr(int texIndex) {
    if( _textures.find(texIndex) == _textures.end() ) {
        ofLog(OF_LOG_ERROR, "of3dModel :: getTexturePtr()") << " : Mesh index "<<texIndex<<" out of bounds, returning NULL";
        return NULL;
    }
    return& _textures.find(texIndex)->second;
}
//----------------------------------------------------------
ofTexture& of3dModel::getTexture( int texIndex) {
    if( _textures.find(texIndex) == _textures.end() ) {
        ofLog(OF_LOG_ERROR, "of3dModel :: getTexture()") << " : texIndex "<<texIndex<<" out of bounds, returning";
        return;
    }
    return _textures[ texIndex ];
}
//----------------------------------------------------------
map<int, ofTexture>& of3dModel::getTextures() {
    return _textures;
}

//----------------------------------------------------------
int of3dModel::getNumTexcoords() {
    return (int)_texCoords.size();
}

//----------------------------------------------------------
ofVec4f* of3dModel::getTexCoordPtr( int texCoordIndex ) {
    if( _texCoords.find(texCoordIndex) == _texCoords.end() ) {
        ofLog(OF_LOG_ERROR, "of3dModel :: getTexCoordPtr()") << " : texCoordIndex "<<texCoordIndex<<" out of bounds, returning NULL";
        return NULL;
    }
    return&_texCoords[texCoordIndex];
}

//----------------------------------------------------------
ofVec4f& of3dModel::getTexCoord( int texCoordIndex ) {
    if( _texCoords.find(texCoordIndex) == _texCoords.end() ) {
        ofLog(OF_LOG_WARNING, "of3dModel :: getTexCoord()") << " : texCoordIndex "<<texCoordIndex<<" not found, setting normalized tex coords";
        _texCoords[texCoordIndex] = ofVec4f(0,0,1,1);
    }
    return _texCoords[texCoordIndex];
}

//----------------------------------------------------------
map<int, ofVec4f>& of3dModel::getTexCoords() {
    return _texCoords;
}

//----------------------------------------------------------
vector<ofIndexType> of3dModel::getIndicies( int startIndex, int endIndex ) {
    vector<ofIndexType> indicies;
    indicies.assign( getMesh().getIndices().begin()+startIndex, getMesh().getIndices().begin()+endIndex );
    return indicies;
}

//----------------------------------------------------------
void of3dModel::setColorForIndicies( int meshIndex, int startIndex, int endIndex, ofColor color ) {
    if(!getMesh(meshIndex).hasColors()) {
        // no colors for verticies, so we must set them here //
        getMesh(meshIndex).getColors().resize(getMesh(meshIndex).getNumVertices());
    }
    
    for(int i = startIndex; i < endIndex; i++) {
        getMesh(meshIndex).setColor( getMesh(meshIndex).getIndex(i), color);
    }
}

//----------------------------------------------------------
ofMesh of3dModel::getMeshForIndexes(int meshIndex, int startIndex, int endIndex, int startVertIndex, int endVertIndex ) {
    
    ofMesh mesh;
    mesh.setMode( getMesh(meshIndex).getMode() );
    
    vector<ofVec3f> verticies;
    verticies.assign( getMesh(meshIndex).getVertices().begin()+startVertIndex, getMesh(meshIndex).getVertices().begin()+endVertIndex );
    mesh.addVertices( verticies );
    
    if(getMesh(meshIndex).hasColors()) {
        vector<ofFloatColor> colors;
        colors.assign( getMesh(meshIndex).getColors().begin()+startVertIndex, getMesh(meshIndex).getColors().begin()+endVertIndex );
        mesh.addColors( colors );
        if(getMesh(meshIndex).usingColors()) mesh.enableColors();
        else mesh.disableColors();
    }
    
    if(getMesh(meshIndex).hasTexCoords()) {
        vector<ofVec2f> texcoords;
        texcoords.assign( getMesh(meshIndex).getTexCoords().begin()+startVertIndex, getMesh(meshIndex).getTexCoords().begin()+endVertIndex );
        mesh.addTexCoords( texcoords );
        if(getMesh(meshIndex).usingTextures()) mesh.enableTextures();
        else mesh.disableTextures();
    }
    
    if(getMesh(meshIndex).hasNormals()) {
        vector<ofVec3f> normals;
        normals.assign( getMesh(meshIndex).getNormals().begin()+startVertIndex, getMesh(meshIndex).getNormals().begin()+endVertIndex );
        mesh.addNormals( normals );
        if(getMesh(meshIndex).usingNormals()) mesh.enableNormals();
        else mesh.disableNormals();
    }
    
    int offsetIndex = getMesh(meshIndex).getIndex(startIndex);
    for(int i = startIndex; i < endIndex; i++) {
        int index = getMesh(meshIndex).getIndex(i) - offsetIndex;
        mesh.addIndex( index );
    }
    
    return mesh;
}


//----------------------------------------------------------
//ofVec3f of3dModel::getScale() {
//    return _scale;
//}
//----------------------------------------------------------
bool of3dModel::hasScaling() {
    ofVec3f scale = getScale();
    return (scale.x != 1.f || scale.y != 1.f || scale.z != 1.f);
}
//----------------------------------------------------------
bool of3dModel::hasNormalsEnabled() {
    for(int i = 0; i < getNumMeshes(); i++) {
        if(getMesh(i).hasNormals()) return true;
    }
    return false;
}
//----------------------------------------------------------
ofVec3f of3dModel::getResolution() const {
    return _resolution;
}

//----------------------------------------------------------
void of3dModel::enableNormals() {
    for(int i = 0; i < getNumMeshes(); i++) {
        getMesh(i).enableNormals();
    }
}
//----------------------------------------------------------
void of3dModel::enableTextures() {
    for(int i = 0; i < getNumMeshes(); i++) {
        getMesh(i).enableTextures();
    }
}
//----------------------------------------------------------
void of3dModel::disableNormals() {
    for(int i = 0; i < getNumMeshes(); i++) {
        getMesh(i).disableNormals();
    }
}
//----------------------------------------------------------
void of3dModel::disableTextures() {
    for(int i = 0; i < getNumMeshes(); i++) {
        getMesh(i).disableTextures();
    }
}


// SETTERS //
//----------------------------------------------------------
void of3dModel::setResolution( int resX, int resY, int resZ ) {
    _resolution.set( resX, resY, resZ );
}

//----------------------------------------------------------
void of3dModel::normalizeAndApplySavedTexCoords( int meshIndex ) {
    ofVec4f tcoords = getTexCoord(meshIndex);
    // when a new mesh is created, it uses normalized tex coords, we need to reset them
    // but save the ones used previously //
    _texCoords[meshIndex].set(0,0,1,1);
    setTexCoords(tcoords.x, tcoords.y, tcoords.z, tcoords.w);
}

// applies to all the meshes evenly //
//----------------------------------------------------------
void of3dModel::setTexCoords( float u1, float v1, float u2, float v2 ) {
    
    for(int i = 0; i < _meshes.size(); i++) {
        setTexCoords(i, u1, v1, u2, v2);
    }
}

// apply to a specific mesh //
//----------------------------------------------------------
void of3dModel::setTexCoords( int meshindex, float u1, float v1, float u2, float v2 ) {
    
    //if( _texCoords.find(meshindex) == _texCoords.end() ) {
    //    ofLog(OF_LOG_VERBOSE, "of3dModel :: setTexCoords : adding normalized tex coords for mesh index "+ofToString(meshindex,0));
    //    _texCoords[meshindex] = ofVec4f(0,0,1,1);
    //}
    
    ofVec4f prevTcoord = getTexCoord( meshindex );
    
    for(int j = 0; j < _meshes[meshindex].getNumTexCoords(); j++ ) {
        ofVec2f tcoord = _meshes[meshindex].getTexCoord(j);
        tcoord.x = ofMap(tcoord.x, prevTcoord.x, prevTcoord.z, u1, u2);
        tcoord.y = ofMap(tcoord.y, prevTcoord.y, prevTcoord.w, v1, v2);
        
        _meshes[meshindex].setTexCoord(j, tcoord);
    }
    
    _texCoords[meshindex].set(u1, v1, u2, v2);
    
}

//----------------------------------------------------------
void of3dModel::setTexCoordsFromTexture( ofTexture& inTexture, int tCoordsIndex ) {
    bool bNormalized = (inTexture.getTextureData().textureTarget!=GL_TEXTURE_RECTANGLE_ARB);
    ofTextureData& tdata = inTexture.getTextureData();
    if(bNormalized)
        setTexCoords( 0, 0, tdata.tex_t, tdata.tex_u );
    else
        setTexCoords(0, 0, inTexture.getWidth(), inTexture.getHeight());
    
    ofVec4f tcoords = getTexCoord(tCoordsIndex);
    setTexCoords(tcoords.x, tcoords.y, tcoords.z, tcoords.w);
}



// REMOVE //
//----------------------------------------------------------
void of3dModel::removeMesh( int index ) {
    if(index > 0 && index < _meshes.size())
        _meshes.erase( _meshes.begin()+index );
}

//----------------------------------------------------------
void of3dModel::removeTexture( int index ) {
    if( _textures.find(index) != _textures.end() )
        _textures.erase( index );
}

//----------------------------------------------------------
void of3dModel::clear() {
    _meshes.clear();
    _textures.clear();
}



//--------------------------------------------------------------
void of3dModel::drawVertices() {
	draw(OF_MESH_POINTS);
}

//--------------------------------------------------------------
void of3dModel::drawWireframe() {
	draw(OF_MESH_WIREFRAME);
}

//--------------------------------------------------------------
void of3dModel::drawFaces() {
	draw(OF_MESH_FILL);
}

//--------------------------------------------------------------
void of3dModel::draw() {
	draw(OF_MESH_FILL);
}

//--------------------------------------------------------------
void of3dModel::draw(ofPolyRenderMode renderType) {
    // ofNode applies all of the tranformations needed, included scale //
    ofNode::transformGL();
    ofGetCurrentRenderer()->draw(*this, renderType);
    ofNode::restoreTransformGL();
}

//--------------------------------------------------------------
void of3dModel::drawNormals(float length) {
    ofNode::transformGL();
    
    float fixLength = getScale().length();
    
    for(int j = 0; j < getNumMeshes(); j++) {
        if(getMesh(j).usingNormals()) {
            vector<ofVec3f> normals = getMesh(j).getNormals();
            vector<ofVec3f> vertices = getMesh(j).getVertices();
            ofVec3f normal;
            ofVec3f vert;
            glBegin(GL_LINES);
            for(int i = 0; i < normals.size(); i++) {
                vert = vertices[i];
                normal = normals[i].normalized();
                glVertex3f(vert.x, vert.y, vert.z);
                normal *= length;
                glVertex3f(normal.x+vert.x, normal.y+vert.y, normal.z+vert.z);
            }
            glEnd();
        } else {
            ofLog(OF_LOG_WARNING, "of3dModel :: drawNormals()") << " : mesh["<<j<<"] normals are disabled";
        }
    }
    
    ofNode::restoreTransformGL();
}


// PLANE MESH //
//--------------------------------------------------------------
ofMesh ofGetPlaneMesh(float width, float height, int columns, int rows, ofPrimitiveMode mode ) {
    ofMesh mesh;
    
    if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
        ofLog( OF_LOG_WARNING ) << "ofGetPlaneMesh :: mode not supported, setting to OF_PRIMITIVE_TRIANGLES";
        mode = OF_PRIMITIVE_TRIANGLES;
    }
    
    mesh.setMode(mode);
    
    ofVec3f vert;
    ofVec3f normal(0, 0, 1); // always facing forward //
    ofVec2f texcoord;
    
    // the origin of the plane is the center //
    float halfW = width/2.f;
    float halfH = height/2.f;
    // add the vertexes //
    for(int iy = 0; iy < rows; iy++) {
        for(int ix = 0; ix < columns; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)columns-1.f));
            texcoord.y = ((float)iy/((float)rows-1.f));
            
            vert.x = texcoord.x * width - halfW;
            vert.y = texcoord.y * height - halfH;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    if(mode == OF_PRIMITIVE_TRIANGLE_STRIP) {
        for(int y = 0; y < rows-1; y++) {
            // even rows //
            if((y&1)==0) {
                for(int x = 0; x < columns; x++) {
                    mesh.addIndex( (y) * columns + x );
                    mesh.addIndex( (y+1) * columns + x);
                }
            } else {
                for(int x = columns-1; x >0; x--) {
                    mesh.addIndex( (y+1) * columns + x );
                    mesh.addIndex( y * columns + x-1 );
                }
            }
        }
        
        if(rows%2!=0) mesh.addIndex(mesh.getNumVertices()-columns);
    } else {
        // Triangles //
        for(int y = 0; y < rows-1; y++) {
            for(int x = 0; x < columns-1; x++) {
                // first triangle //
                mesh.addIndex((y)*columns + x);
                mesh.addIndex((y)*columns + x+1);
                mesh.addIndex((y+1)*columns + x);
                
                // second triangle //
                mesh.addIndex((y)*columns + x+1);
                mesh.addIndex((y+1)*columns + x+1);
                mesh.addIndex((y+1)*columns + x);
            }
        }
    }
    
    return mesh;
}


// PLANE PRIMITIVE //
//--------------------------------------------------------------
ofPlanePrimitive::ofPlanePrimitive() {
    set( 200, 100, 6, 3);
}

//--------------------------------------------------------------
ofPlanePrimitive::ofPlanePrimitive(float width, float height, int columns, int rows, ofPrimitiveMode mode) {
    set(width, height, columns, rows, mode);
}

//--------------------------------------------------------------
ofPlanePrimitive::~ofPlanePrimitive() {}

//--------------------------------------------------------------
void ofPlanePrimitive::set(float width, float height, int columns, int rows, ofPrimitiveMode mode) {
    
    _width  = width;
    _height = height;
    of3dModel::setResolution(columns, rows, 0);
    
    _meshes.clear();
    ofMesh mesh = ofGetPlaneMesh( getWidth(), getHeight(), getResolution().x, getResolution().y, mode );
    addMesh( mesh );
    
    if(_texCoords.size()>0)
        normalizeAndApplySavedTexCoords(0);
    
}

//--------------------------------------------------------------
void ofPlanePrimitive::setDimensions( float width, float height ) {
    _width = width;
    setHeight(height);
}

//--------------------------------------------------------------
void ofPlanePrimitive::resizeToTexture( ofTexture& inTexture ) {
    setDimensions(inTexture.getWidth(), inTexture.getHeight());
    setTexCoordsFromTexture( inTexture );
}

//--------------------------------------------------------------
void ofPlanePrimitive::setResolution( int columns, int rows ) {
    setResolution( columns,  rows,  0);
}

//--------------------------------------------------------------
void ofPlanePrimitive::setResolution(int resX, int resY, int resZ) {
    
    of3dModel::setResolution(resX, resY, 0); // no z value //
    ofPrimitiveMode mode = OF_PRIMITIVE_TRIANGLE_STRIP;
    if(_meshes.size() > 0 ) {
        mode = _meshes[0].getMode();
    }
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








// Original code by Paul Bourke
// A more efficient contribution by Federico Dosil
// Use CCW facet ordering
// http://paulbourke.net/texture_colour/texturemap/
//----------------------------------------------------------
ofMesh ofGetSphereMesh(float radius, int res ) {

    ofMesh sphereMesh;
    
    float theta1 = 0.f;
    float theta2 = TWO_PI;
    float phi1 = -HALF_PI;
    float phi2 = HALF_PI;
    float r = radius; // normalize the verts //
    
    sphereMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    int i, j;
    
    int n = res * 2;
    float ndiv2=(float)n/2.f;
    
    float jdivn,j1divn,idivn,dosdivn,t1,t2,t3,cost1,cost2,cte1,cte3;

    float unodivn=1.f/(float)(n);

    cte3 = (theta2-theta1)/n;
    cte1 = (phi2-phi1)/ndiv2;
    dosdivn = 2 * (1.f/(float)n);
    ofVec3f e,p,e2,p2;
    
    if (n < 0){
        n = -n;
        ndiv2 = -ndiv2;
    }
    if (n < 4) {n = 4; ndiv2=(float)n/2;}
    if(r <= 0) r = 1;
    
    t2=phi1;
    cost2=cos(phi1);
    j1divn=0;
    
    ofVec3f vert, normal;
    ofVec2f tcoord;
    
    for (j=0; j < ndiv2;j++) {
        t1 = t2;
        t2 += cte1;
        t3 = theta1 - cte3;
        cost1 = cost2;
        cost2 = cos(t2);
        e.y = sin(t1);
        e2.y = sin(t2);
        p.y = r * e.y;
        p2.y = r * e2.y;
        
        idivn=0;
        jdivn=j1divn;
        j1divn+=dosdivn;
        
        for (i=0;i <= n;i++) {
            t3 += cte3;
            e.x = cost1 * cos(t3);
            e.z = cost1 * sin(t3);
            p.x = r * e.x;
            p.z = r * e.z;
            
            // reverse the u coord, so the default is texture mapped left to
            // right on the outside of a sphere //
            normal.set( e.x, e.y, e.z );
            tcoord.set( 1.0-idivn, jdivn );
            vert.set( p.x, p.y, p.z );
            
            sphereMesh.addNormal(normal);
            sphereMesh.addTexCoord(tcoord);
            sphereMesh.addVertex(vert);
            
            
            e2.x = cost2 * cos(t3);
            e2.z = cost2 * sin(t3);
            p2.x = r * e2.x;
            p2.z = r * e2.z;
            
            normal.set(e2.x, e2.y, e2.z);
            tcoord.set(1.0-idivn, j1divn);
            vert.set(p2.x, p2.y, p2.z);
            
            sphereMesh.addNormal(normal);
            sphereMesh.addTexCoord(tcoord);
            sphereMesh.addVertex(vert);
            
            idivn += unodivn;
            if(idivn > 1) idivn = 1;
            
            
        }
    }
    
    return sphereMesh;
}

// SPHERE PRIMITIVE //
//----------------------------------------------------------
ofSpherePrimitive::ofSpherePrimitive() {
    _radius = 20;
    setResolution(32, 32, 32);
}

//----------------------------------------------------------
ofSpherePrimitive::ofSpherePrimitive( float radius, int res ) {
    _radius = radius;
    setResolution(res, res, res);
}

//----------------------------------------------------------
ofSpherePrimitive::~ofSpherePrimitive() {
    
}

//----------------------------------------------------------
void ofSpherePrimitive::set(float radius, int res ) {
    _radius = radius;
    setResolution(res);
}

//----------------------------------------------------------
void ofSpherePrimitive::setResolution(int res) {
    setResolution(res, res, res);
}

//----------------------------------------------------------
void ofSpherePrimitive::setResolution(int resX, int resY, int resZ) {
    of3dModel::setResolution(resX, resY, resZ);
    _meshes.clear();
    ofMesh sphereMesh = ofGetSphereMesh( getRadius(), resX );
    addMesh( sphereMesh );
    if(_texCoords.size()>0)
        normalizeAndApplySavedTexCoords(0);
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


// based on code by Michael Broutin for the ogre-procedural library //
// http://code.google.com/p/ogre-procedural/source/browse/library/src/ProceduralIcoSphereGenerator.cpp
// For the latest info, see http://code.google.com/p/ogre-procedural/ //
// NO texture coords or normals
// use ofGetIcoSphere(radius, 0) // 0 iterations will return Icosahedron //
//----------------------------------------------------------
ofMesh ofGetIcosahedronMesh(float radius) {
    ofMesh mesh;
    
    const float sqrt5 = sqrt(5.0f);
    const float phi = (1.0f + sqrt5) * 0.5f;
    
    /// Step 1 : Generate icosahedron
    float invnorm = 1/sqrt(phi*phi+1);
    
    mesh.addVertex(invnorm*ofVec3f(-1,  phi, 0));//0
    mesh.addVertex(invnorm*ofVec3f( 1,  phi, 0));//1
    mesh.addVertex(invnorm*ofVec3f(0,   1,  -phi));//2
    mesh.addVertex(invnorm*ofVec3f(0,   1,   phi));//3
    mesh.addVertex(invnorm*ofVec3f(-phi,0,  -1));//4
    mesh.addVertex(invnorm*ofVec3f(-phi,0,   1));//5
    mesh.addVertex(invnorm*ofVec3f( phi,0,  -1));//6
    mesh.addVertex(invnorm*ofVec3f( phi,0,   1));//7
    mesh.addVertex(invnorm*ofVec3f(0,   -1, -phi));//8
    mesh.addVertex(invnorm*ofVec3f(0,   -1,  phi));//9
    mesh.addVertex(invnorm*ofVec3f(-1,  -phi,0));//10
    mesh.addVertex(invnorm*ofVec3f( 1,  -phi,0));//11
    
    int firstFaces[] = {
        0,1,2,
        0,3,1,
        0,4,5,
        1,7,6,
        1,6,2,
        1,3,7,
        0,2,4,
        0,5,3,
        2,6,8,
        2,8,4,
        3,5,9,
        3,9,7,
        11,6,7,
        10,5,4,
        10,4,8,
        10,9,5,
        11,8,6,
        11,7,9,
        10,8,11,
        10,11,9
    };
    
    for(int i = 0; i < mesh.getNumVertices(); i++) {
        mesh.setVertex(i, mesh.getVertex(i) * radius);
    }
    
    for(int i = 0; i < 60; i+=3) {
        mesh.addTriangle(firstFaces[i], firstFaces[i+1], firstFaces[i+2]);
    }
    
    return mesh;
}



// based on code by Michael Broutin for the ogre-procedural library //
// http://code.google.com/p/ogre-procedural/source/browse/library/src/ProceduralIcoSphereGenerator.cpp
// For the latest info, see http://code.google.com/p/ogre-procedural/ //
//----------------------------------------------------------
ofMesh ofGetIcoSphereMesh(float radius, int iterations) {
    
    ofMesh icosahedron = ofGetIcosahedronMesh( 1.f );
    vector<ofVec3f> vertices = icosahedron.getVertices();
    vector<ofIndexType> faces = icosahedron.getIndices();
    
    int size = faces.size();
    
    /// Step 2 : tessellate
    for (unsigned short iteration = 0; iteration < iterations; iteration++)
    {
        size*=4;
        vector<ofIndexType> newFaces;
        newFaces.clear();
        //newFaces.resize(size);
        for (int i=0; i<size/12; i++)
        {
            int i1 = faces[i*3];
            int i2 = faces[i*3+1];
            int i3 = faces[i*3+2];
            int i12 = vertices.size();
            int i23 = i12+1;
            int i13 = i12+2;
            ofVec3f v1 = vertices[i1];
            ofVec3f v2 = vertices[i2];
            ofVec3f v3 = vertices[i3];
            //make 1 vertice at the center of each edge and project it onto the sphere
            vertices.push_back((v1+v2).normalized());
            vertices.push_back((v2+v3).normalized());
            vertices.push_back((v1+v3).normalized());
            //now recreate indices
            newFaces.push_back(i1);
            newFaces.push_back(i12);
            newFaces.push_back(i13);
            newFaces.push_back(i2);
            newFaces.push_back(i23);
            newFaces.push_back(i12);
            newFaces.push_back(i3);
            newFaces.push_back(i13);
            newFaces.push_back(i23);
            newFaces.push_back(i12);
            newFaces.push_back(i23);
            newFaces.push_back(i13);
        }
        faces.swap(newFaces);
    }
    
    /// Step 3 : generate texcoords
    vector<ofVec2f> texCoords;
    for (unsigned short i=0;i<vertices.size();i++)
    {
        const ofVec3f& vec = vertices[i];
        float u, v;
        float r0 = sqrtf(vec.x*vec.x+vec.z*vec.z);
        float alpha;
        alpha = atan2f(vec.z,vec.x);
        u = alpha/TWO_PI+.5f;
        v = atan2f(vec.y, r0)/PI + .5f;
        // reverse the u coord, so the default is texture mapped left to
        // right on the outside of a sphere //
        texCoords.push_back(ofVec2f(1.0-u,v));
    }
    
    /// Step 4 : fix texcoords
    // find vertices to split
    std::vector<int> indexToSplit;
    
    for (unsigned int i=0;i<faces.size()/3;i++)
    {
        ofVec2f& t0 = texCoords[faces[i*3+0]];
        ofVec2f& t1 = texCoords[faces[i*3+1]];
        ofVec2f& t2 = texCoords[faces[i*3+2]];
        if (abs(t2.x-t0.x)>0.5)
        {
            if (t0.x<0.5)
                indexToSplit.push_back(faces[i*3]);
            else
                indexToSplit.push_back(faces[i*3+2]);
        }
        if (abs(t1.x-t0.x)>0.5)
        {
            if (t0.x<0.5)
                indexToSplit.push_back(faces[i*3]);
            else
                indexToSplit.push_back(faces[i*3+1]);
        }
        if (abs(t2.x-t1.x)>0.5)
        {
            if (t1.x<0.5)
                indexToSplit.push_back(faces[i*3+1]);
            else
                indexToSplit.push_back(faces[i*3+2]);
        }
    }
    
    //split vertices
    for (unsigned short i=0;i<indexToSplit.size();i++)
    {
        int index = indexToSplit[i];
        //duplicate vertex
        ofVec3f v = vertices[index];
        ofVec2f t = texCoords[index] + ofVec2f(1.f, 0.f);
        vertices.push_back(v);
        texCoords.push_back(t);
        int newIndex = vertices.size()-1;
        //reassign indices
        for (unsigned short j=0;j<faces.size();j++)
        {
            if (faces[j]==index)
            {
                int index1 = faces[(j+1)%3+(j/3)*3];
                int index2 = faces[(j+2)%3+(j/3)*3];
                if ((texCoords[index1].x>0.5) || (texCoords[index2].x>0.5))
                {
                    faces[j] = newIndex;
                }
            }
        }
    }
    
    
    ofMesh sphere;
    
    for(int i = 0; i < faces.size(); i++ ) {
        sphere.addIndex(faces[i]);
    }
    
    for(int i = 0; i < vertices.size(); i++ ) {
        sphere.addVertex(vertices[i]*radius);
        sphere.addNormal(vertices[i]);
        sphere.addTexCoord(texCoords[i]);
    }
    
    return  sphere;
}



// ICO SPHERE //
//----------------------------------------------------------
ofIcoSpherePrimitive::ofIcoSpherePrimitive() {
    _radius = 20;
    setResolution(2);
}

//----------------------------------------------------------
ofIcoSpherePrimitive::ofIcoSpherePrimitive( float radius, int iterations ) {
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
    of3dModel::setResolution(resX, resY, resZ);
    //_radius = 1.f;
    _meshes.clear();
    // store the number of iterations in the resolution //
    ofMesh sphereMesh = ofGetIcoSphereMesh( getRadius(), getResolution().x );
    addMesh( sphereMesh );
    
    if(_texCoords.size()>0)
        normalizeAndApplySavedTexCoords(0);
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







// Cylinder Mesh
//----------------------------------------------------------
ofMesh ofGetCylinderMesh( float radius, float height, int radiusSegments, int heightSegments, int numCapSegments, bool bCapped, ofPrimitiveMode mode ) {
    ofMesh mesh;
    if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
        mode = OF_PRIMITIVE_TRIANGLE_STRIP;
    }
    mesh.setMode(mode);
    
    int capSegs = numCapSegments;
    if(heightSegments < 2) heightSegments = 2;
    if(capSegs < 2) capSegs = 2;
    if(!bCapped) capSegs=1;
    
    float angleIncRadius = -1.f * (TWO_PI/((float)radiusSegments-1.f));
    float heightInc = height/((float)heightSegments-1.f);
    float halfH = height*.5f;
    
    float newRad;
    ofVec3f vert;
    ofVec2f tcoord;
    ofVec3f normal;
    ofVec3f up(0,1,0);
    
    int vertOffset = 0;
    
    float maxTexY           = heightSegments-1.f + (capSegs*2)-2.f;
    float maxTexYNormalized = (capSegs-1.f) / maxTexY;
    
    // add the top cap //
    if(bCapped) {
        normal.set(0,-1,0);
        for(int iy = 0; iy < capSegs; iy++) {
            for(int ix = 0; ix < radiusSegments; ix++) {
                newRad = ofMap((float)iy, 0, capSegs-1, 0.0, radius);
                vert.x = cos((float)ix*angleIncRadius) * newRad;
                vert.z = sin((float)ix*angleIncRadius) * newRad;
                vert.y = -halfH;
                
                tcoord.x = (float)ix/((float)radiusSegments-1.f);
                tcoord.y = ofMap(iy, 0, capSegs-1, 0, maxTexYNormalized);
                
                mesh.addTexCoord( tcoord );
                mesh.addVertex( vert );
                mesh.addNormal( normal );
            }
        }
        
        if(mode == OF_PRIMITIVE_TRIANGLES) {
            for(int y = 0; y < capSegs-1; y++) {
                for(int x = 0; x < radiusSegments-1; x++) {
                    // first triangle //
                    mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                    mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                    
                    // second triangle //
                    mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                }
            }
        } else {
            for(int y = 0; y < capSegs-1; y++) {
                for(int x = 0; x < radiusSegments; x++) {
                    mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                    mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                }
            }
        }
        
        vertOffset = mesh.getNumVertices();
        
    }
    
    //maxTexY             = heightSegments-1.f + capSegs-1.f;
    float minTexYNormalized = 0;
    if(bCapped) minTexYNormalized = maxTexYNormalized;
    maxTexYNormalized   = 1.f;
    if(bCapped) maxTexYNormalized = (heightSegments) / maxTexY;
    
    // cylinder verticies //
    for(int iy = 0; iy < heightSegments; iy++) {
        normal.set(1,0,0);
        for(int ix = 0; ix < radiusSegments; ix++) {
            
            //newRad = ofMap((float)iy, 0, heightSegments-1, 0.0, radius);
            vert.x = cos((float)ix*angleIncRadius) * radius;
            vert.y = heightInc*((float)iy) - halfH;
            vert.z = sin((float)ix*angleIncRadius) * radius;
            
            tcoord.x = (float)ix/((float)radiusSegments-1.f);
            tcoord.y = ofMap(iy, 0, heightSegments-1, minTexYNormalized, maxTexYNormalized );
            
            mesh.addTexCoord( tcoord );
            mesh.addVertex( vert );
            mesh.addNormal( normal );
            
            normal.rotateRad(-angleIncRadius, up);
            
        }
    }
    
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        for(int y = 0; y < heightSegments-1; y++) {
            for(int x = 0; x < radiusSegments-1; x++) {
                // first triangle //
                mesh.addIndex( (y)*radiusSegments + x + vertOffset);
                mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset );
                mesh.addIndex( (y+1)*radiusSegments + x + vertOffset );
                
                // second triangle //
                mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset );
                mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset );
                mesh.addIndex( (y+1)*radiusSegments + x + vertOffset );
            }
        }
    } else {
        for(int y = 0; y < heightSegments-1; y++) {
            for(int x = 0; x < radiusSegments; x++) {
                mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                mesh.addIndex( (y+1)*radiusSegments + x + vertOffset );
            }
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    // add the bottom cap
    if(bCapped) {
        //float minTexYNormalized = 0;
        minTexYNormalized = maxTexYNormalized;
        maxTexYNormalized   = 1.f;
        
        normal.set(0, 1,0);
        for(int iy = 0; iy < capSegs; iy++) {
            for(int ix = 0; ix < radiusSegments; ix++) {
                newRad = ofMap((float)iy, 0, capSegs-1, radius, 0.0);
                vert.x = cos((float)ix*angleIncRadius) * newRad;
                vert.z = sin((float)ix*angleIncRadius) * newRad;
                vert.y = halfH;
                
                tcoord.x = (float)ix/((float)radiusSegments-1.f);
                tcoord.y = ofMap(iy, 0, capSegs-1, minTexYNormalized, maxTexYNormalized);
                
                mesh.addTexCoord( tcoord );
                mesh.addVertex( vert );
                mesh.addNormal( normal );
            }
        }
        
        if(mode == OF_PRIMITIVE_TRIANGLES) {
            for(int y = 0; y < capSegs-1; y++) {
                for(int x = 0; x < radiusSegments-1; x++) {
                    // first triangle //
                    mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                    mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                    
                    // second triangle //
                    mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                }
            }
        } else {
            for(int y = 0; y < capSegs-1; y++) {
                for(int x = 0; x < radiusSegments; x++) {
                    mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                    mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                }
            }
        }
        
        vertOffset = mesh.getNumVertices();
        
    }
    
    return mesh;
}



//--------------------------------------------------------------
ofCylinderPrimitive::ofCylinderPrimitive() {
    set( 60, 80, 6, 3, 2, true );
}

//--------------------------------------------------------------
ofCylinderPrimitive::ofCylinderPrimitive( float radius, float height, int radiusSegments, int heightSegments, int numCapSegments, bool bCapped, ofPrimitiveMode mode ) {
    set( radius, height, radiusSegments, heightSegments, numCapSegments, bCapped, mode );
}

//--------------------------------------------------------------
ofCylinderPrimitive::~ofCylinderPrimitive() {}

//--------------------------------------------------------------
void ofCylinderPrimitive::set(float radius, float height, int radiusSegments, int heightSegments, int numCapSegments, bool bCapped, ofPrimitiveMode mode) {
    _radius = radius;
    _height = height;
    _bCapped = bCapped;
    of3dModel::setResolution( radiusSegments, heightSegments, numCapSegments );
    
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
    _verticies[0][0] = 0;
    _verticies[0][1] = getResolution().x * getResolution().z;
    
    // 1 -> cylinder //
    if(bCapped) {
        _strides[1][0] = _strides[0][0] + _strides[0][1];
        _verticies[1][0] = _verticies[0][0] + _verticies[0][1];
    } else {
        _strides[1][0] = 0;
        _verticies[1][0] = 0;
    }
    _strides[1][1] = resX * resY * indexStep;
    _verticies[1][1] = getResolution().x * getResolution().y;
    
    // 2 -> bottom cap
    _strides[2][0] = _strides[1][0] + _strides[1][1];
    _strides[2][1] = resX * resZ * indexStep;
    _verticies[2][0] = _verticies[1][0]+_verticies[1][1];
    _verticies[2][1] = getResolution().x * getResolution().z;
    
    
    _meshes.clear();
    ofMesh mesh = ofGetCylinderMesh( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, getCapped(), mode );
    addMesh( mesh );
    
    if(_texCoords.size()>0)
        normalizeAndApplySavedTexCoords(0);
    
}

//--------------------------------------------------------------
void ofCylinderPrimitive::set( float radius, float height ) {
    _radius = radius;
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
    if(_meshes.size() > 0 ) {
        mode = _meshes[0].getMode();
    }
    set( getRadius(), getHeight(), radiusSegments, heightSegments, capSegments, getCapped(), mode );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setTopCapColor( ofColor color ) {
    setColorForIndicies( 0, _strides[0][0], _strides[0][0]+_strides[0][1], color );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setCylinderColor( ofColor color ) {
    setColorForIndicies( 0, _strides[1][0], _strides[1][0]+_strides[1][1], color );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setBottomCapColor( ofColor color ) {
    setColorForIndicies( 0, _strides[2][0], _strides[2][0]+_strides[2][1], color );
}

//--------------------------------------------------------------
vector<ofIndexType> ofCylinderPrimitive::getTopCapIndicies() {
    return of3dModel::getIndicies( _strides[0][0], _strides[0][0] + _strides[0][1] );
}

//--------------------------------------------------------------
ofMesh ofCylinderPrimitive::getTopCapMesh() {
    return getMeshForIndexes( 0, _strides[0][0], _strides[0][0]+_strides[0][1],
                             _verticies[0][0], _verticies[0][0]+_verticies[0][1] );
}

//--------------------------------------------------------------
vector<ofIndexType> ofCylinderPrimitive::getCylinderIndicies() {
    return of3dModel::getIndicies( _strides[1][0], _strides[1][0] + _strides[1][1] );
}

//--------------------------------------------------------------
ofMesh ofCylinderPrimitive::getCylinderMesh() {
    return getMeshForIndexes( 0, _strides[1][0], _strides[1][0]+_strides[1][1],
                             _verticies[1][0], _verticies[1][0]+_verticies[1][1] );
}

//--------------------------------------------------------------
vector<ofIndexType> ofCylinderPrimitive::getBottomCapIndicies() {
    return of3dModel::getIndicies( _strides[2][0], _strides[2][0] + _strides[2][1] );
}

//--------------------------------------------------------------
ofMesh ofCylinderPrimitive::getBottomCapMesh() {
    return getMeshForIndexes( 0, _strides[2][0], _strides[2][0]+_strides[2][1],
                             _verticies[2][0], _verticies[2][0]+_verticies[2][1] );
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









// Cone Mesh //
//--------------------------------------------------------------
ofMesh ofGetConeMesh( float radius, float height, int radiusSegments, int heightSegments, int capSegments, ofPrimitiveMode mode ) {
    ofMesh mesh;
    if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
        mode = OF_PRIMITIVE_TRIANGLE_STRIP;
    }
    mesh.setMode(mode);
    
    if(heightSegments < 2) heightSegments = 2;
    int capSegs = capSegments;
    if(capSegs < 2) capSegs = 2;

    
    float angleIncRadius    = (TWO_PI/((float)radiusSegments-1.f));
    float heightInc         = height/((float)heightSegments-1);
    float halfH             = height*.5f;
    
    float newRad;
    ofVec3f vert;
    ofVec3f normal;
    ofVec2f tcoord;
    ofVec3f up(0,1,0);
    
        
    int vertOffset = 0;
    
    float maxTexY = heightSegments-1.f + capSegs-1.f;
    
    // cone verticies //
    for(int iy = 0; iy < heightSegments; iy++) {
        normal.set(1,0,0);
        for(int ix = 0; ix < radiusSegments; ix++) {
            
            newRad = ofMap((float)iy, 0, heightSegments-1, 0.0, radius);
            vert.x = cos((float)ix*angleIncRadius) * newRad;
            vert.y = heightInc*((float)iy) - halfH;
            vert.z = sin((float)ix*angleIncRadius) * newRad;
            
            tcoord.x = (float)ix/((float)radiusSegments-1.f);
            tcoord.y = (float)iy/((float)maxTexY);
            
            mesh.addTexCoord( tcoord );
            mesh.addVertex( vert );
            mesh.addNormal( normal );
            
            normal.rotateRad(-angleIncRadius, up);
            
        }
    }
    
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        for(int y = 0; y < heightSegments-1; y++) {
            for(int x = 0; x < radiusSegments-1; x++) {
                // first triangle //
                mesh.addIndex( (y)*radiusSegments + x );
                mesh.addIndex( (y)*radiusSegments + x+1 );
                mesh.addIndex( (y+1)*radiusSegments + x );
                
                // second triangle //
                mesh.addIndex( (y)*radiusSegments + x+1 );
                mesh.addIndex( (y+1)*radiusSegments + x+1 );
                mesh.addIndex( (y+1)*radiusSegments + x );
            }
        }
    } else {
        for(int y = 0; y < heightSegments-1; y++) {
            for(int x = 0; x < radiusSegments; x++) {
                mesh.addIndex( (y)*radiusSegments + x );
                mesh.addIndex( (y+1)*radiusSegments + x );
            }
        }
    }
        
    vertOffset = mesh.getNumVertices();
    float maxTexYNormalized = (heightSegments-1.f) / maxTexY;
    
    // add the cap //
    normal.set(0,1,0);
    for(int iy = 0; iy < capSegs; iy++) {
        for(int ix = 0; ix < radiusSegments; ix++) {
            newRad = ofMap((float)iy, 0, capSegs-1, radius, 0.0);
            vert.x = cos((float)ix*angleIncRadius) * newRad;
            vert.z = sin((float)ix*angleIncRadius) * newRad;
            vert.y = halfH;
            
            tcoord.x = (float)ix/((float)radiusSegments-1.f);
            tcoord.y = ofMap(iy, 0, capSegs-1, maxTexYNormalized, 1.f);
            
            mesh.addTexCoord( tcoord );
            mesh.addVertex( vert );
            mesh.addNormal( normal );
        }
    }
        
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        for(int y = 0; y < capSegs-1; y++) {
            for(int x = 0; x < radiusSegments-1; x++) {
                // first triangle //
                mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                
                // second triangle //
                mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset);
                mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
            }
        }
    } else {
        for(int y = 0; y < capSegs-1; y++) {
            for(int x = 0; x < radiusSegments; x++) {
                mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
            }
        }
    }
    
    return mesh;
}





// Cone Primitive //
//--------------------------------------------------------------
ofConePrimitive::ofConePrimitive() {
    set( 20, 70, 8, 3, 2 );
}

//--------------------------------------------------------------
ofConePrimitive::ofConePrimitive( float radius, float height, int radiusSegments, int heightSegments, int capSegments, ofPrimitiveMode mode ) {
    set( radius, height, radiusSegments, heightSegments, capSegments, mode );
}

//--------------------------------------------------------------
ofConePrimitive::~ofConePrimitive() {}

//--------------------------------------------------------------
void ofConePrimitive::set( float radius, float height, int radiusSegments, int heightSegments, int capSegments, ofPrimitiveMode mode ) {
    _radius = radius;
    _height = height;
    of3dModel::setResolution(radiusSegments, heightSegments, capSegments);
    
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
    _verticies[0][0] = 0;
    _verticies[0][1] = getResolution().x * getResolution().y;
    
    _strides[ 1 ][0] = _strides[ 0 ][0] + _strides[ 0 ][1];
    _strides[ 1 ][1] = (resX)*(resZ) * indexStep;
    _verticies[1][0] = _verticies[0][0] + _verticies[0][1];
    _verticies[1][1] = getResolution().x * getResolution().z;
    
    _meshes.clear();
    ofMesh mesh = ofGetConeMesh( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, mode );
    addMesh( mesh );
    
    if(_texCoords.size()>0)
        normalizeAndApplySavedTexCoords(0);
    
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
    if(_meshes.size() > 0 ) {
        mode = _meshes[0].getMode();
    }
    set( getRadius(), getHeight(), resX, resY, resZ, mode );
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
    setColorForIndicies( 0, _strides[0][0], _strides[0][0]+_strides[0][1], color );
}

//--------------------------------------------------------------
void ofConePrimitive::setCapColor( ofColor color ) {
    setColorForIndicies( 0, _strides[1][0], _strides[1][0]+_strides[1][1], color );
}

//--------------------------------------------------------------
vector<ofIndexType> ofConePrimitive::getTopIndicies() {
    return of3dModel::getIndicies(_strides[0][0], _strides[0][0]+_strides[0][1]);
}

//--------------------------------------------------------------
ofMesh ofConePrimitive::getTopMesh() {
    int startIndex  = _strides[0][0];
    int endIndex    = startIndex + _strides[0][1];
    
    int startVertIndex  = _verticies[0][0];
    int endVertIndex    = startVertIndex + _verticies[0][1];
    return getMeshForIndexes( 0, startIndex, endIndex, startVertIndex, endVertIndex );
}

//--------------------------------------------------------------
vector<ofIndexType> ofConePrimitive::getCapIndicies() {
    return of3dModel::getIndicies( _strides[1][0], _strides[1][0] + _strides[1][1] );
}

//--------------------------------------------------------------
ofMesh ofConePrimitive::getCapMesh() {
    int startIndex  = _strides[1][0];
    int endIndex    = startIndex + _strides[1][1];
    
    int startVertIndex  = _verticies[1][0];
    int endVertIndex    = startVertIndex + _verticies[1][1];
    return getMeshForIndexes( 0, startIndex, endIndex, startVertIndex, endVertIndex );
}

//--------------------------------------------------------------
float ofConePrimitive::getRadius() {
    return _radius;
}

//--------------------------------------------------------------
float ofConePrimitive::getHeight() {
    return _height;
}









// Box Mesh //
ofMesh ofGetBoxMesh( float width, float height, float depth, int resX, int resY, int resZ ) {
    // mesh only available as triangles //
    ofMesh mesh;
    mesh.setMode( OF_PRIMITIVE_TRIANGLES );
    
    // halves //
    float halfW = width * .5f;
    float halfH = height * .5f;
    float halfD = depth * .5f;
    
    ofVec3f vert;
    ofVec2f texcoord;
    ofVec3f normal;
    int vertOffset = 0;
    
    // TRIANGLES //
    
    // Front Face //
    normal.set(0, 0, 1);
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            vert.x = texcoord.x * width - halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    
    // Right Side Face //
    normal.set(1, 0, 0);
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resZ; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resZ-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            //vert.x = texcoord.x * width - halfW;
            vert.x = halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = texcoord.x * -depth + halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resZ-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resZ + x + vertOffset);
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    // Left Side Face //
    normal.set(-1, 0, 0);
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resZ; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resZ-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            //vert.x = texcoord.x * width - halfW;
            vert.x = -halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = texcoord.x * depth - halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resZ-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resZ + x + vertOffset);
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    
    // Back Face //
    normal.set(0, 0, -1);
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            vert.x = texcoord.x * -width + halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = -halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    
    
    // Top Face //
    normal.set(0, -1, 0);
    // add the vertexes //
    for(int iy = 0; iy < resZ; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resZ-1.f));
            
            vert.x = texcoord.x * width - halfW;
            //vert.y = texcoord.y * height - halfH;
            vert.y = -halfH;
            vert.z = texcoord.y * depth - halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resZ-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    
    // Bottom Face //
    normal.set(0, 1, 0);
    // add the vertexes //
    for(int iy = 0; iy < resZ; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resZ-1.f));
            
            vert.x = texcoord.x * width - halfW;
            //vert.y = texcoord.y * height - halfH;
            vert.y = halfH;
            vert.z = texcoord.y * -depth + halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resZ-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }
    
    
    
    return mesh;
}




// BOX PRIMITIVE //
//--------------------------------------------------------------
ofBoxPrimitive::ofBoxPrimitive() {
    set(100, 100, 100, 10, 10, 10);
}

//--------------------------------------------------------------
ofBoxPrimitive::ofBoxPrimitive( float width, float height, float depth, int resWidth, int resHeight, int resDepth ) {
    set(width, height, depth, resWidth, resHeight, resDepth );
}

//--------------------------------------------------------------
ofBoxPrimitive::~ofBoxPrimitive() {}

//--------------------------------------------------------------
void ofBoxPrimitive::set( float width, float height, float depth, int resWidth, int resHeight, int resDepth) {
    
    _size.x = width;
    _size.y = height;
    _size.z = depth;
    
    of3dModel::setResolution( resWidth, resHeight, resDepth);
    
    int resX = getResolution().x;
    int resY = getResolution().y;
    int resZ = getResolution().z;
    
    //FRONT, resY, resX
    _strides[ SIDE_FRONT ][0] = 0;
    _strides[ SIDE_FRONT ][1] = (resY-1)*(resX-1)*6;
    _verticies[SIDE_FRONT][0] = 0;
    _verticies[SIDE_FRONT][1] = resX * resY;
    
    //RIGHT, resY, resZ
    _strides[ SIDE_RIGHT ][0] = _strides[ SIDE_FRONT ][0] + _strides[ SIDE_FRONT ][1];
    _strides[ SIDE_RIGHT ][1] = (resY-1)*(resZ-1)*6;
    _verticies[SIDE_RIGHT][0] = _verticies[SIDE_FRONT][0] + _verticies[SIDE_FRONT][1];
    _verticies[SIDE_RIGHT][1] = resY * resZ;
    
    //LEFT, resY, resZ
    _strides[ SIDE_LEFT ][0] = _strides[ SIDE_RIGHT ][0] + _strides[ SIDE_RIGHT ][1];
    _strides[ SIDE_LEFT ][1] = (resY-1)*(resZ-1)*6;
    _verticies[SIDE_LEFT][0] = _verticies[SIDE_RIGHT][0] + _verticies[SIDE_RIGHT][1];
    _verticies[SIDE_LEFT][1] = resY * resZ;
    
    //BACK, resY, resX
    _strides[ SIDE_BACK ][0] = _strides[ SIDE_LEFT ][0] + _strides[ SIDE_LEFT ][1];
    _strides[ SIDE_BACK ][1] = (resY-1)*(resX-1)*6;
    _verticies[SIDE_BACK][0] = _verticies[SIDE_LEFT][0] + _verticies[SIDE_LEFT][1];
    _verticies[SIDE_BACK][1] = resY * resX;
    
    //TOP, resZ, resX
    _strides[ SIDE_TOP ][0] = _strides[ SIDE_BACK ][0] + _strides[ SIDE_BACK ][1];
    _strides[ SIDE_TOP ][1] = (resZ-1)*(resX-1)*6;
    _verticies[SIDE_TOP][0] = _verticies[SIDE_BACK][0] + _verticies[SIDE_BACK][1];
    _verticies[SIDE_TOP][1] = resZ * resX;
    
    //BOTTOM, resZ, resX
    _strides[ SIDE_BOTTOM ][0] = _strides[ SIDE_TOP ][0]+_strides[ SIDE_TOP ][1];
    _strides[ SIDE_BOTTOM ][1] = (resZ-1)*(resX-1)*6;
    _verticies[SIDE_BOTTOM][0] = _verticies[SIDE_TOP][0] + _verticies[SIDE_TOP][1];
    _verticies[SIDE_BOTTOM][1] = resZ * resX;
    
    _meshes.clear();
    ofMesh mesh = ofGetBoxMesh( getWidth(), getHeight(), getDepth(), getResolution().x, getResolution().y, getResolution().z );
    addMesh( mesh );
    
    if(_texCoords.size()>0)
        normalizeAndApplySavedTexCoords(0);
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
vector<ofIndexType> ofBoxPrimitive::getSideIndicies( int sideIndex ) {
    
    if(sideIndex < 0 || sideIndex >= SIDES_TOTAL) {
        ofLog(OF_LOG_WARNING) << "ofBoxPrimitive :: getSideIndicies : faceIndex out of bounds, returning SIDE_FRONT ";
        sideIndex = SIDE_FRONT;
    }
    
    return getIndicies(_strides[sideIndex][0], _strides[sideIndex][0]+_strides[sideIndex][1]);
}

//--------------------------------------------------------------
ofMesh ofBoxPrimitive::getSideMesh( int sideIndex ) {
    
    if(sideIndex < 0 || sideIndex > SIDES_TOTAL) {
        ofLog(OF_LOG_WARNING) << "ofBoxPrimitive :: getSideMesh : faceIndex out of bounds, using SIDE_FRONT ";
        sideIndex = SIDE_FRONT;
    }
    int startIndex  = _strides[sideIndex][0];
    int endIndex    = startIndex+_strides[sideIndex][1];
    
    int startVertIndex  = _verticies[sideIndex][0];
    int endVertIndex    = startVertIndex + _verticies[sideIndex][1];
    
    return getMeshForIndexes( 0, startIndex, endIndex, startVertIndex, endVertIndex );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setResolution( int res ) {
    setResolution(res, res, res);
}

//--------------------------------------------------------------
void ofBoxPrimitive::setResolution( int resX, int resY, int resZ ) {
    of3dModel::setResolution(resX, resY, resZ);
    set( getWidth(), getHeight(), getDepth() );
}

//--------------------------------------------------------------
void ofBoxPrimitive::setSideColor( int sideIndex, ofColor color ) {
    if(sideIndex < 0 || sideIndex >= SIDES_TOTAL) {
        ofLog(OF_LOG_WARNING) << "ofBoxPrimitive :: setSideColor : sideIndex out of bounds, setting SIDE_FRONT ";
        sideIndex = SIDE_FRONT;
    }
    setColorForIndicies( 0, _strides[sideIndex][0], _strides[sideIndex][0]+_strides[sideIndex][1], color );
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

















