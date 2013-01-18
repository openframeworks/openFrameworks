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
//int ofPrimitiveBase::getNumTexcoords() {
//    return (int)_texCoords.size();
//}

//----------------------------------------------------------
ofVec4f* ofPrimitiveBase::getTexCoordsPtr() {
//    if( _texCoords.find(texCoordIndex) == _texCoords.end() ) {
//        ofLog(OF_LOG_ERROR, "ofPrimitiveBase :: getTexCoordPtr()") << " : texCoordIndex "<<texCoordIndex<<" out of bounds, returning NULL";
//        return NULL;
//    }
    return& _texCoords;
}

//----------------------------------------------------------
ofVec4f& ofPrimitiveBase::getTexCoords() {
//    if( _texCoords.find(texCoordIndex) == _texCoords.end() ) {
//        _texCoords[texCoordIndex] = ofVec4f(0,0,1,1);
//    }
    //if(_texCoords.x == _texCoords.y == _texCoords.z == _texCoords.w == 0) {
    //    _texCoords = ofVec4f(0,0,1,1);
    //}
    return _texCoords;
}

//----------------------------------------------------------
//ofVec4f ofPrimitiveBase::getTexCoords() {
//    return _texCoords;
//}

//----------------------------------------------------------
vector<ofIndexType> ofPrimitiveBase::getIndicies( int startIndex, int endIndex ) {
    vector<ofIndexType> indicies;
    indicies.assign( getMesh().getIndices().begin()+startIndex, getMesh().getIndices().begin()+endIndex );
    return indicies;
}

//----------------------------------------------------------
void ofPrimitiveBase::setColorForIndicies( int startIndex, int endIndex, ofColor color ) {
    if(!getMesh().hasColors()) {
        // no colors for verticies, so we must set them here //
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
    
    vector<ofVec3f> verticies;
    verticies.assign( getMesh().getVertices().begin()+startVertIndex, getMesh().getVertices().begin()+endVertIndex );
    mesh.addVertices( verticies );
    
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

//----------------------------------------------------------
ofVec3f ofPrimitiveBase::getFaceNormal(of3dTriangle& tri) {
    ofVec3f U, V, n;
    
    U = (tri.points[1]-tri.points[0]);
    V = (tri.points[2]-tri.points[0]);
    
    n = U.crossed(V);
    n.normalize();
    
    return n;
}

//----------------------------------------------------------
vector<of3dTriangle> ofPrimitiveBase::getUniqueTriangles() {
    
    vector<of3dTriangle> triangles;
    
    vector<ofVec3f>& verts       = getMesh().getVertices();
    vector<ofVec3f>& normals     = getMesh().getNormals();
    vector<ofVec2f>& tcoords     = getMesh().getTexCoords();
    vector<ofFloatColor>&colors  = getMesh().getColors();
    vector<ofIndexType>& indices = getMesh().getIndices();
    // if we are doing triangles, we have to use a vert and normal for each triangle
    // that way we can calculate face normals and use getFace();
    
    int index;
    
    bool bHasColors     = getMesh().hasColors();
    bool bHasNormals    = getMesh().hasNormals();
    bool bHasTexcoords  = getMesh().hasTexCoords();
    
    //cout << "getUniqueTriangles :: indicies.size = "<<indices.size() << endl;

    if(getMesh().getMode() == OF_PRIMITIVE_TRIANGLES) {
        for(int j = 0; j < indices.size(); j += 3) {
            of3dTriangle tri;
            for(int k = 0; k < 3; k++) {
                index = indices[j+k];
                tri.points[k].set(verts[index].x, verts[index].y, verts[index].z);//      = verts[index];
                tri.faceNormal = getFaceNormal( tri );
                if(bHasNormals)
                    tri.normals[k]      = normals[index];
                if(bHasTexcoords)
                    tri.texcoords[k]    = tcoords[index];
                if(bHasColors)
                    tri.colors[k]       = colors[index];
                tri.setHasColors(bHasColors);
                tri.setHasNormals(bHasNormals);
                tri.setHasTexcoords(bHasTexcoords);
            }
            triangles.push_back( tri );
        }
        
    } else {
        ofLog( OF_LOG_WARNING, "ofPrimitiveBase :: splitVertices only works with mode == OF_PRIMITIVE_TRIANGLES" );
    }
    
    return triangles;
    
}

//----------------------------------------------------------
vector<ofVec3f> ofPrimitiveBase::getFaceNormals( bool perVetex ) {
    // default for ofPrimitiveBase is vertex normals //
    vector<ofVec3f> faceNormals;
        
    if(getMesh().hasVertices()) {
        vector<ofVec3f>& verts          = getMesh().getVertices();
        vector<ofIndexType>& indicies   = getMesh().getIndices();
        vector<ofVec3f> normals         = getMesh().getNormals();
        
        if(verts.size() > 3 && indicies.size() > 3) {
            
            ofVec3f v1, v2, v3, n; ofVec3f U, V;
            for(int i = 0; i < indicies.size(); i+=3) {
                v1 = verts[indicies[i+0]];
                v2 = verts[indicies[i+1]];
                v3 = verts[indicies[i+2]];
                
                U = (v2-v1);
                V = (v3-v1);
                
                n = U.crossed(V);
                n.normalize();
                
                faceNormals.push_back(n);
                if(perVetex) {
                    faceNormals.push_back(n);
                    faceNormals.push_back(n);
                }
            }
            return faceNormals;
        }
        
    }
    
    return faceNormals;
}

//----------------------------------------------------------
void ofPrimitiveBase::setFromTriangles( vector<of3dTriangle>& tris, bool bUseFaceNormal ) {
    vector<of3dTriangle>::iterator it;
    
    vector<ofVec3f> verts;
    vector<ofVec2f> tcoords;
    vector<ofFloatColor> colors;
    vector<ofVec3f> normals;
    
    for(it = tris.begin(); it != tris.end(); it++) {
        for(int k = 0; k < 3; k++) {
            verts.push_back( it->points[k] );
            if(it->hasTexcoords())
                tcoords.push_back( it->texcoords[k] );
            if(it->hasColors())
                colors.push_back( it->colors[k] );
            if(it->hasNormals() || bUseFaceNormal) {
                if(bUseFaceNormal)
                    normals.push_back(it->faceNormal);
                else
                    normals.push_back( it->normals[k] );
            }
        }
    }
    getMesh().clearIndices();
    getMesh().clearVertices();
    getMesh().addVertices(verts);
    
    getMesh().setupIndicesAuto();
    
    if(normals.size() > 0) {
        getMesh().clearNormals();
        getMesh().addNormals(normals);
    }
    if(colors.size() > 0) {
        getMesh().clearColors();
        getMesh().addColors( colors );
    }
    if( tcoords.size() > 0 ) {
        getMesh().clearTexCoords();
        getMesh().addTexCoords( tcoords );
    }
}

//----------------------------------------------------------
void ofPrimitiveBase::smoothNormals( float angle ) {
    
    if(getMesh().getMode() == OF_PRIMITIVE_TRIANGLES) {
        vector<of3dTriangle> triangles = getUniqueTriangles();
        vector<ofVec3f> verts;
        for(int i = 0; i < triangles.size(); i++) {
            for(int j = 0; j < 3; j++) {
                verts.push_back(triangles[i].points[j]);
            }
        }
        
        map<int, int> removeIds;
        
        float epsilon = .01f;
        for(int i = 0; i < verts.size()-1; i++) {
            for(int j = i+1; j < verts.size(); j++) {
                if(i != j) {
                    ofVec3f& v1 = verts[i];
                    ofVec3f& v2 = verts[j];
                    if( v1.distance(v2) <= epsilon ) {
                        // average the location //
                        verts[i] = (v1+v2)/2.f;
                        verts[j] = verts[i];
                        removeIds[j] = 1;
                    }
                }
            }
        }
        
        cout << "verts.size() = " << verts.size() << " removeIds.size() = " << removeIds.size() << endl;
        
        for(map<int, int>::iterator it = removeIds.begin(); it != removeIds.end(); ++it ) {
            //verts.erase( verts.begin() + it->first );
        }
        
        //for(int i = removeIds.size()-1; i >= 0; i--) {
        //    verts.erase( verts.begin() + removeIds[i] );
        //}
        
        cout << "verts.size() = " << verts.size() << endl;
        
        //return;
        
        
        // string of vertex in 3d space to triangle index //
        map<string, vector<int> > vertHash;
        //map<string, int > vertHash;
        
        cout << "ofPrimitiveBase :: smoothNormals num verts = " << verts.size() << " tris size = " << triangles.size() << endl;
        
        string xStr, yStr, zStr;
        
        for(int i = 0; i < verts.size(); i++ ) {
            xStr = "x"+ofToString(verts[i].x==-0?0:verts[i].x);
            yStr = "y"+ofToString(verts[i].y==-0?0:verts[i].y);
            zStr = "z"+ofToString(verts[i].z==-0?0:verts[i].z);
            string vstring = xStr+yStr+zStr;
            if(vertHash.find(vstring) == vertHash.end()) {
                for(int j = 0; j < triangles.size(); j++) {
                    for(int k = 0; k < 3; k++) {
                        if(verts[i].x == triangles[j].points[k].x) {
                            if(verts[i].y == triangles[j].points[k].y) {
                                if(verts[i].z == triangles[j].points[k].z) {
                                    vertHash[vstring].push_back( j );
                                }
                            }
                        }
                    }
                }
            }
        }
        
        for( map<string, vector<int> >::iterator it = vertHash.begin(); it != vertHash.end(); ++it) {
        //for( map<string, int >::iterator it = vertHash.begin(); it != vertHash.end(); ++it) {
            cout << "ofPrimitiveBase :: SmoothNormals : "<<it->first<<"  num = " << it->second.size() << endl;
        }
        
        
        ofVec3f normal;
        float angleCos = cos(angle * DEG_TO_RAD );
        float numNormals=0;
        ofVec3f f1, f2;
        
        for(int j = 0; j < triangles.size(); j++) {
            for(int k = 0; k < 3; k++) {
                xStr = "x"+ofToString(triangles[j].points[k].x==-0?0:triangles[j].points[k].x);
                yStr = "y"+ofToString(triangles[j].points[k].y==-0?0:triangles[j].points[k].y);
                zStr = "z"+ofToString(triangles[j].points[k].z==-0?0:triangles[j].points[k].z);
                 
                string vstring = xStr+yStr+zStr;
                numNormals=0;
                normal.set(0,0,0);
                if(vertHash.find(vstring) != vertHash.end()) {
                    for(int i = 0; i < vertHash[vstring].size(); i++) {
                        f1 = triangles[j].faceNormal;
                        f2 = triangles[vertHash[vstring][i]].faceNormal;
                        if(f1.dot(f2) >= angleCos ) {
                            normal += f2;
                            numNormals+=1.f;
                        }
                    }
                    //normal /= (float)vertHash[vstring].size();
                    normal /= numNormals;
                    
                    triangles[j].normals[k] = normal;
                    triangles[j].setHasNormals( true );
                }
            }
        }
        
        //cout << "SmoothNormals - Setting from triangles " << endl;
        setFromTriangles( triangles );
        
    }
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

// apply to a specific mesh //
//----------------------------------------------------------
//void ofPrimitiveBase::setTexCoords( float u1, float v1, float u2, float v2 ) {
//    
//    ofVec4f prevTcoord = getTexCoord( meshindex );
//    
//    for(int j = 0; j < _meshes[meshindex].getNumTexCoords(); j++ ) {
//        ofVec2f tcoord = _meshes[meshindex].getTexCoord(j);
//        tcoord.x = ofMap(tcoord.x, prevTcoord.x, prevTcoord.z, u1, u2);
//        tcoord.y = ofMap(tcoord.y, prevTcoord.y, prevTcoord.w, v1, v2);
//        
//        _meshes[meshindex].setTexCoord(j, tcoord);
//    }
//    
//    _texCoords[meshindex].set(u1, v1, u2, v2);
//    
//}

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



//----------------------------------------------------------
void ofPrimitiveBase::mergeDuplicateVerticies() {
    
    vector<ofVec3f> verts          = getMesh().getVertices();
    vector<ofIndexType> indicies   = getMesh().getIndices();
    
    //get indexes to share single point - TODO: try j < i
    for(int i = 0; i < indicies.size(); i++) {
        for(int j = 0; j < indicies.size(); j++ ) {
            if(i==j) continue;
            
            ofIndexType i1  = indicies[i];
            ofIndexType i2  = indicies[j];
            ofVec3f v1      = verts[ i1 ];
            ofVec3f v2      = verts[ i2 ];
            
            if( v1 == v2 && i1 != i2) {
                indicies[j] = i1;
                break;
            }
        }
    }
    
    //indicies array now has list of unique points we need
    //but we need to delete the old points we're not using and that means the index values will change
    //so we are going to create a new list of points and new indexes - we will use a map to map old index values to the new ones
    vector <ofPoint> newPoints;
    vector <ofIndexType> newIndexes;
    map <int, bool> ptCreated;
    map <int, int> oldIndexNewIndex;
    
    vector<ofFloatColor> newColors;
    vector<ofFloatColor>& colors    = getMesh().getColors();
    vector<ofVec2f> newTCoords;
    vector<ofVec2f>& tcoords        = getMesh().getTexCoords();
    vector<ofVec3f> newNormals;
    vector<ofVec3f>& normals        = getMesh().getNormals();
    
    for(int i = 0; i < indicies.size(); i++){
        ptCreated[i] = false;
    }
    
    for(int i = 0; i < indicies.size(); i++){
        ofIndexType index = indicies[i];
        ofPoint p = verts[ index ];
        
        if( ptCreated[index] == false ){
            oldIndexNewIndex[index] = newPoints.size();
            newPoints.push_back( p );
            if(getMesh().hasColors()) {
                newColors.push_back(colors[index]);
            }
            if(getMesh().hasTexCoords()) {
                newTCoords.push_back(tcoords[index]);
            }
            if(getMesh().hasNormals()) {
                newNormals.push_back(normals[index]);
            }
            
            ptCreated[index] = true;
        }
        
        //cout << "["<<i<<"]: old " << index << " --> " << oldIndexNewIndex[index] << endl;
        newIndexes.push_back( oldIndexNewIndex[index] );
    }
    
    verts.clear();
    verts = newPoints;
    
    indicies.clear();
    indicies = newIndexes;
    
    getMesh().clearIndices();
    getMesh().addIndices(indicies);
    getMesh().clearVertices();
    getMesh().addVertices( verts );
    
    if(getMesh().hasColors()) {
        getMesh().clearColors();
        getMesh().addColors( newColors );
    }
    
    if(getMesh().hasTexCoords()) {
        getMesh().clearTexCoords();
        getMesh().addTexCoords( newTCoords );
    }
    
    if(getMesh().hasNormals()) {
        getMesh().clearNormals();
        getMesh().addNormals( newNormals );
    }
    
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
    _mesh = ofGetPlaneMesh( getWidth(), getHeight(), getResolution().x, getResolution().y, mode );
    
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






//----------------------------------------------------------
ofMesh ofGetSphereMesh(float radius, int res, ofPrimitiveMode mode ) {

    ofMesh mesh;
    
    float doubleRes = res*2.f;
    float polarInc = PI/(res); // ringAngle
    float azimInc = TWO_PI/(doubleRes); // segAngle //
    
    if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
        mode = OF_PRIMITIVE_TRIANGLE_STRIP;
    }
    mesh.setMode(mode);
    
    ofVec3f vert;
    ofVec2f tcoord;
    
    for(float i = 0; i < res+1; i++) {
        
        float tr = sin( PI-i * polarInc );
        float ny = cos( PI-i * polarInc );
        
        tcoord.y = i / res;
        
        for(float j = 0; j <= doubleRes; j++) {
            
            float nx = tr * sin(j * azimInc);
            float nz = tr * cos(j * azimInc);
            
            tcoord.x = j / (doubleRes);
            
            vert.set(nx, ny, nz);
            mesh.addNormal(vert);
            vert *= radius;
            mesh.addVertex(vert);
            mesh.addTexCoord(tcoord);
        }
    }
    
    int nr = doubleRes+1;
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        
        int index1, index2, index3;
        
        for(float iy = 0; iy < res; iy++) {
            for(float ix = 0; ix < doubleRes; ix++) {
                
                // first tri //
                if(iy > 0) {
                    index1 = (iy+0) * (nr) + (ix+0);
                    index2 = (iy+0) * (nr) + (ix+1);
                    index3 = (iy+1) * (nr) + (ix+0);
                    
                    mesh.addIndex(index1);
                    mesh.addIndex(index2);
                    mesh.addIndex(index3);
                }
                
                if(iy < res-1 ) {
                    // second tri //
                    index1 = (iy+0) * (nr) + (ix+1);
                    index2 = (iy+1) * (nr) + (ix+1);
                    index3 = (iy+1) * (nr) + (ix+0);
                    
                    mesh.addIndex(index1);
                    mesh.addIndex(index2);
                    mesh.addIndex(index3);
                    
                }
            }
        }
        
    } else {
        for(int y = 0; y < res; y++) {
            for(int x = 0; x <= doubleRes; x++) {
                mesh.addIndex( (y)*nr + x );
                mesh.addIndex( (y+1)*nr + x );
            }
        }
    }
    
     
    return mesh;
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
    _mesh = ofGetSphereMesh( getRadius(), getResolution().x, mode );
    
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



/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural
 
 For the latest info, see http://code.google.com/p/ogre-procedural/
 
 Copyright (c) 2010 Michael Broutin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
// http://code.google.com/p/ogre-procedural/source/browse/library/src/ProceduralIcoSphereGenerator.cpp

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
    
    sphere.addIndices( faces );
    sphere.addNormals( vertices );
    sphere.addTexCoords( texCoords );
    
    for(int i = 0; i < vertices.size(); i++ ) {
        vertices[i] *= radius;
    }
    sphere.addVertices( vertices );
    
    return  sphere;
}
/*
-----------------------------------------------------------------------------
// END OGRE
-----------------------------------------------------------------------------
*/


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
    _mesh = ofGetIcoSphereMesh( getRadius(), getResolution().x );
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
    
    float angleIncRadius = -1 * (TWO_PI/((float)radiusSegments-1.f));
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
                    if(y > 0) {
                        // first triangle //
                        mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                        mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                        mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                    }
                    
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
                    
                    if(y < capSegs -1 && capSegs > 2) {
                        // second triangle //
                        mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                        mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset);
                        mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                    }
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
    
    
    getMesh().clear();
    _mesh = ofGetCylinderMesh( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, getCapped(), mode );
    
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
    setColorForIndicies( _strides[0][0], _strides[0][0]+_strides[0][1], color );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setCylinderColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
    }
    setColorForIndicies( _strides[1][0], _strides[1][0]+_strides[1][1], color );
}

//--------------------------------------------------------------
void ofCylinderPrimitive::setBottomCapColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
    }
    setColorForIndicies( _strides[2][0], _strides[2][0]+_strides[2][1], color );
}

//--------------------------------------------------------------
vector<ofIndexType> ofCylinderPrimitive::getTopCapIndicies() {
    return ofPrimitiveBase::getIndicies( _strides[0][0], _strides[0][0] + _strides[0][1] );
}

//--------------------------------------------------------------
ofMesh ofCylinderPrimitive::getTopCapMesh() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
        return ofMesh();
    }
    return getMeshForIndexes( _strides[0][0], _strides[0][0]+_strides[0][1],
                             _verticies[0][0], _verticies[0][0]+_verticies[0][1] );
}

//--------------------------------------------------------------
vector<ofIndexType> ofCylinderPrimitive::getCylinderIndicies() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
    }
    return ofPrimitiveBase::getIndicies( _strides[1][0], _strides[1][0] + _strides[1][1] );
}

//--------------------------------------------------------------
ofMesh ofCylinderPrimitive::getCylinderMesh() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
        return ofMesh();
    }
    return getMeshForIndexes( _strides[1][0], _strides[1][0]+_strides[1][1],
                             _verticies[1][0], _verticies[1][0]+_verticies[1][1] );
}

//--------------------------------------------------------------
vector<ofIndexType> ofCylinderPrimitive::getBottomCapIndicies() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
    }
    return ofPrimitiveBase::getIndicies( _strides[2][0], _strides[2][0] + _strides[2][1] );
}

//--------------------------------------------------------------
ofMesh ofCylinderPrimitive::getBottomCapMesh() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofCylinderPrimitive : must be in triangle strip mode" << endl;
        return ofMesh();
    }
    return getMeshForIndexes( _strides[2][0], _strides[2][0]+_strides[2][1],
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

    
    float angleIncRadius    = -1.f * ((TWO_PI/((float)radiusSegments-1.f)));
    float heightInc         = height/((float)heightSegments-1);
    float halfH             = height*.5f;
    
    float newRad;
    ofVec3f vert;
    ofVec3f normal;
    ofVec2f tcoord;
    ofVec3f up(0,1,0);
    
        
    int vertOffset = 0;
    
    float maxTexY = heightSegments-1.f + capSegs-1.f;
    
    float crossAngle = ofVec3f(radius, 1, 0).angle( ofVec3f(0,1,0) );
    ofVec3f startVec(0, -halfH-1.f, 0);
    
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
            
            if(iy == 0) {
                newRad = 1.f;
                vert.x = cos((float)ix*angleIncRadius) * newRad;
                vert.y = heightInc*((float)iy) - halfH;
                vert.z = sin((float)ix*angleIncRadius) * newRad;
            }
            
            ofVec3f diff = vert-startVec;
            ofVec3f crossed = up.crossed(vert);
            normal = crossed.normalized();
            normal = crossed.getPerpendicular(diff);
            
            normal.normalize();
            
            mesh.addNormal( normal );
            //}
            
        }
    }
    
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        for(int y = 0; y < heightSegments-1; y++) {
            for(int x = 0; x < radiusSegments-1; x++) {
                if(y > 0){
                    // first triangle //
                    mesh.addIndex( (y)*radiusSegments + x );
                    mesh.addIndex( (y)*radiusSegments + x+1 );
                    mesh.addIndex( (y+1)*radiusSegments + x );
                }
                
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
                //if(y > 0) {
                // first triangle //
                mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                //}
                
                if(y < capSegs-1) {
                    // second triangle //
                    mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                }
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
    _verticies[0][0] = 0;
    _verticies[0][1] = getResolution().x * getResolution().y;
    
    _strides[ 1 ][0] = _strides[ 0 ][0] + _strides[ 0 ][1];
    _strides[ 1 ][1] = (resX)*(resZ) * indexStep;
    _verticies[1][0] = _verticies[0][0] + _verticies[0][1];
    _verticies[1][1] = getResolution().x * getResolution().z;
    
    getMesh().clear();
    _mesh = ofGetConeMesh( getRadius(), getHeight(), getResolution().x, getResolution().y, getResolution().z, mode );
    
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
    setColorForIndicies( _strides[0][0], _strides[0][0]+_strides[0][1], color );
}

//--------------------------------------------------------------
void ofConePrimitive::setCapColor( ofColor color ) {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofConePrimitive : must be in triangle strip mode" << endl;
    }
    setColorForIndicies( _strides[1][0], _strides[1][0]+_strides[1][1], color );
}

//--------------------------------------------------------------
vector<ofIndexType> ofConePrimitive::getConeIndicies() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofConePrimitive : must be in triangle strip mode" << endl;
    }
    return ofPrimitiveBase::getIndicies(_strides[0][0], _strides[0][0]+_strides[0][1]);
}

//--------------------------------------------------------------
ofMesh ofConePrimitive::getConeMesh() {
    int startIndex  = _strides[0][0];
    int endIndex    = startIndex + _strides[0][1];
    
    int startVertIndex  = _verticies[0][0];
    int endVertIndex    = startVertIndex + _verticies[0][1];
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofConePrimitive : must be in triangle strip mode" << endl;
        return ofMesh();
    }
    return getMeshForIndexes( startIndex, endIndex, startVertIndex, endVertIndex );
}

//--------------------------------------------------------------
vector<ofIndexType> ofConePrimitive::getCapIndicies() {
    if(getMesh().getMode() != OF_PRIMITIVE_TRIANGLE_STRIP) {
        ofLog(OF_LOG_WARNING) << "ofConePrimitive : must be in triangle strip mode" << endl;
    }
    return ofPrimitiveBase::getIndicies( _strides[1][0], _strides[1][0] + _strides[1][1] );
}

//--------------------------------------------------------------
ofMesh ofConePrimitive::getCapMesh() {
    int startIndex  = _strides[1][0];
    int endIndex    = startIndex + _strides[1][1];
    
    int startVertIndex  = _verticies[1][0];
    int endVertIndex    = startVertIndex + _verticies[1][1];
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
    
    _mesh.clear();
    _mesh = ofGetBoxMesh( getWidth(), getHeight(), getDepth(), getResolution().x, getResolution().y, getResolution().z );
    
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
    setColorForIndicies( _strides[sideIndex][0], _strides[sideIndex][0]+_strides[sideIndex][1], color );
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

















