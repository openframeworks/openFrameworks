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
void of3dModel::addTexture( ofTexture& tex ) {
    _textures.push_back( tex );
}
void of3dModel::addTexCoords( ofVec4f& tCoords ) {
    _texCoords.push_back(tCoords);
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
    if(texIndex < 0 || texIndex >= _textures.size() ) {
        ofLog(OF_LOG_ERROR, "of3dModel :: getTexturePtr()") << " : Mesh index "<<texIndex<<" out of bounds, returning NULL";
        return NULL;
    }
    return& _textures[texIndex];
}
//----------------------------------------------------------
ofTexture& of3dModel::getTexture( int texIndex) {
    if(texIndex < 0 || texIndex >= _textures.size() ) {
        ofLog(OF_LOG_ERROR, "of3dModel :: getTexture()") << " : Mesh index "<<texIndex<<" out of bounds, returning";
        return;
    }
    return _textures[ texIndex ];
}
//----------------------------------------------------------
vector<ofTexture>& of3dModel::getTextures() {
    return _textures;
}

//----------------------------------------------------------
int of3dModel::getNumTexcoords() {
    return (int)_texCoords.size();
}

//----------------------------------------------------------
ofVec4f* of3dModel::getTexCoordPtr( int texCoordIndex ) {
    if(texCoordIndex < 0 || texCoordIndex >= _texCoords.size() ) {
        ofLog(OF_LOG_ERROR, "of3dModel :: getTexturePtr()") << " : Mesh index "<<texCoordIndex<<" out of bounds, returning NULL";
        return NULL;
    }
    return&_texCoords[texCoordIndex];
}

//----------------------------------------------------------
ofVec4f& of3dModel::getTexCoord( int texCoordIndex ) {
    if(texCoordIndex < 0 || texCoordIndex >= _texCoords.size() ) {
        ofLog(OF_LOG_ERROR, "of3dModel :: getTexturePtr()") << " : Mesh index "<<texCoordIndex<<" out of bounds, returning";
        return;
    }
    return _texCoords[texCoordIndex];
}

//----------------------------------------------------------
vector<ofVec4f>& of3dModel::getTexCoords() {
    return _texCoords;
}



//----------------------------------------------------------
ofVec3f of3dModel::getScale() {
    return _scale;
}
//----------------------------------------------------------
bool of3dModel::hasScaling() {
    return (_scale.x != 1.f || _scale.y != 1.f || _scale.z != 1.f);
}
//----------------------------------------------------------
ofVec3f of3dModel::getResolution() const {
    return _resolution;
}



// SETTERS //
//----------------------------------------------------------
void of3dModel::setScale(float scale) {
    setScale(scale, scale, scale);
}
//----------------------------------------------------------
void of3dModel::setScale( float scaleX, float scaleY, float scaleZ ) {
    _scale.set( scaleX, scaleY, scaleZ );
}

//----------------------------------------------------------
void of3dModel::setResolution( int resX, int resY, int resZ ) {
    _resolution.set( resX, resY, resZ );
}

// applies to all the meshes evenly //
//----------------------------------------------------------
void of3dModel::setTexCoords( float u1, float v1, float u2, float v2 ) {
    
    if(_texCoords.size() == 0) {
        _texCoords.resize(_meshes.size());
        for(int i = 0; i < _texCoords.size(); i++) {
            // tex coords have not been set, assuming normalized //
            _texCoords[i].set(0, 0, 1, 1);
        }
    }
    
    for(int i = 0; i < _meshes.size(); i++) {
        setTexCoords(i, u1, v1, u2, v2);
    }
}

// apply to a specific mesh //
//----------------------------------------------------------
void of3dModel::setTexCoords( int meshindex, float u1, float v1, float u2, float v2 ) {
    if(meshindex >= _texCoords.size() || meshindex < 0) {
        ofLog(OF_LOG_ERROR, "of3dModel :: setTexCoords : meshindex "+ofToString(meshindex,0)+" out of bounds.");
        return;
    }
    
    ofVec4f prevTcoord = _texCoords[meshindex];
    
    for(int j = 0; j < _meshes[meshindex].getNumTexCoords(); j++ ) {
        ofVec2f tcoord = _meshes[meshindex].getTexCoord(j);
        tcoord.x = ofMap(tcoord.x, prevTcoord.x, prevTcoord.z, u1, u2);
        tcoord.y = ofMap(tcoord.y, prevTcoord.y, prevTcoord.w, v1, v2);
        _meshes[meshindex].setTexCoord(j, tcoord);
    }
    
    _texCoords[meshindex].set(u1, v1, u2, v2);
    cout << endl;
}


// REMOVE //
//----------------------------------------------------------
void of3dModel::removeMesh( int index ) {
    if(index > 0 && index < _meshes.size())
        _meshes.erase( _meshes.begin()+index );
}

//----------------------------------------------------------
void of3dModel::removeTexture( int index ) {
    if(index > 0 && index < _textures.size())
        _textures.erase( _textures.begin()+index );
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
    ofGetCurrentRenderer()->draw(*this, renderType);
}


// PLANE MESH //
//--------------------------------------------------------------
ofMesh ofGetPlaneMesh(float width, float height, int columns, int rows ) {
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
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
    
    return mesh;
}


// PLANE PRIMITIVE //
//--------------------------------------------------------------
ofPlanePrimitive::ofPlanePrimitive() {
    set( 200, 100, 6, 3);
}

//--------------------------------------------------------------
ofPlanePrimitive::ofPlanePrimitive(float width, float height, int columns, int rows) {
    set(width, height, columns, rows);
}

//--------------------------------------------------------------
ofPlanePrimitive::~ofPlanePrimitive() {}

//--------------------------------------------------------------
void ofPlanePrimitive::set(float width, float height, int columns, int rows) {
    if(width != _width || height != _height || getResolution().x != rows || getResolution().y != columns) {
        _width = width;
        _height = height;
        setResolution(columns, rows);
    }
}

//--------------------------------------------------------------
void ofPlanePrimitive::setDimensions( float width, float height ) {
    _width = width;
    setHeight(height);
}

//--------------------------------------------------------------
void ofPlanePrimitive::resizeToTexture( ofTexture& inTexture ) {
    setDimensions(inTexture.getWidth(), inTexture.getHeight());
    
    bool bNormalized = (inTexture.getTextureData().textureTarget!=GL_TEXTURE_RECTANGLE_ARB);
    ofTextureData& tdata = inTexture.getTextureData();
    
    if(bNormalized)
        setTexCoords( 0, 0, tdata.tex_t, tdata.tex_u );
    else
        setTexCoords(0, 0, inTexture.getWidth(), inTexture.getHeight());
}

//--------------------------------------------------------------
void ofPlanePrimitive::setResolution( int columns, int rows ) {
    setResolution( columns,  rows,  0);
}

//--------------------------------------------------------------
void ofPlanePrimitive::setResolution(int resX, int resY, int resZ) {
    of3dModel::setResolution(resX, resY, 0); // no z value //
    _meshes.clear();
    ofMesh mesh = ofGetPlaneMesh( getWidth(), getHeight(), resX, resY );
    addMesh( mesh );
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
//----------------------------------------------------------
ofMesh ofGetIcoSphereMesh(float radius, int iterations) {
    
    const float sqrt5 = sqrt(5.0f);
    const float phi = (1.0f + sqrt5) * 0.5f;
    
    vector<ofVec3f> vertices;
    
    /// Step 1 : Generate icosahedron
    float invnorm = 1/sqrt(phi*phi+1);
    
    vertices.push_back(invnorm*ofVec3f(-1,  phi, 0));//0
    vertices.push_back(invnorm*ofVec3f( 1,  phi, 0));//1
    vertices.push_back(invnorm*ofVec3f(0,   1,  -phi));//2
    vertices.push_back(invnorm*ofVec3f(0,   1,   phi));//3
    vertices.push_back(invnorm*ofVec3f(-phi,0,  -1));//4
    vertices.push_back(invnorm*ofVec3f(-phi,0,   1));//5
    vertices.push_back(invnorm*ofVec3f( phi,0,  -1));//6
    vertices.push_back(invnorm*ofVec3f( phi,0,   1));//7
    vertices.push_back(invnorm*ofVec3f(0,   -1, -phi));//8
    vertices.push_back(invnorm*ofVec3f(0,   -1,  phi));//9
    vertices.push_back(invnorm*ofVec3f(-1,  -phi,0));//10
    vertices.push_back(invnorm*ofVec3f( 1,  -phi,0));//11
    
    int firstFaces[] = {0,1,2,
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
    vector<int> faces(firstFaces, firstFaces + sizeof(firstFaces)/sizeof(*firstFaces));
    int size = 60;
    
    /// Step 2 : tessellate
    for (unsigned short iteration = 0; iteration < iterations; iteration++)
    {
        size*=4;
        std::vector<int> newFaces;
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

























