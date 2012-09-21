//
//  of3dGraphics.cpp
//  openFrameworksLib
//
//  Created by Nick Hardeman on 9/14/12.
//
//

#include "of3dGraphics.h"
#include "ofGraphics.h"
#include "of3dPrimitives.h"
#include <map>

//----------------------------------------------------------
//static map<of3dPrimitiveType, of3dModel>& getCached3dPrimitives() {
    //static map<of3dPrimitiveType, of3dModel>* primitives = new map<of3dPrimitiveType, of3dModel>;
static map<of3dPrimitiveType, of3dModel> primitives;// = new map<of3dPrimitiveType, of3dModel>;
    //return *primitives;
//}

//----------------------------------------------------------
static of3dModel& getCached3dPrimitive( of3dPrimitiveType type ) {
    switch (type) {
        case OF_3D_PRIMITIVE_PLANE:
            if(primitives.find(type) == primitives.end()) {
                cout << "of3dGraphics :: getCached3dPrimitive : inserting plane - " << type << endl;
                primitives[type] = ofPlanePrimitive();
                ((ofPlanePrimitive&)primitives[type]).set(1.0f, 1.0f, 6, 4);
            }
            break;
        case OF_3D_PRIMITIVE_SPHERE:
            if(primitives.find(type) == primitives.end()) {
                cout << "of3dGraphics :: getCached3dPrimitive : inserting sphere - " << type << endl;
                primitives[type] = ofSpherePrimitive( 1.0f, 4 );
                ((ofSpherePrimitive&)primitives[type]).setRadius( 1.0f );
            }
            break;
        case OF_3D_PRIMITIVE_ICO_SPHERE:
            if(primitives.find(type) == primitives.end()) {
                cout << "of3dGraphics :: getCached3dPrimitive : inserting ICO sphere - " << type << endl;
                primitives[type] = ofIcoSpherePrimitive(1.0f, 2);
                ((ofIcoSpherePrimitive&)primitives[type]).setRadius( 1.0f );
            }
            break;
        case OF_3D_PRIMITIVE_BOX:
            
            break;
        case OF_3D_PRIMITIVE_CONE:
            
            break;
        case OF_3D_PRIMITIVE_CYLINDER:
            
            break;
        default:
            break;
    }
    return primitives.find(type)->second;
}

//----------------------------------------------------------
static void renderCached3dPrimitive( of3dModel& model ) {
    if(ofGetFill() == OF_OUTLINE) {
        model.draw(OF_MESH_WIREFRAME);
    } else {
        model.draw(OF_MESH_FILL);
    }
}

// Plane //
//----------------------------------------------------------
void ofSetPlaneResolution( int columns, int rows ) {
    ((ofPlanePrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_PLANE)).setResolution(columns, rows);
}

// returns columns as x value of vector and rows as y value
//----------------------------------------------------------
ofVec2f ofGetPlaneResolution() {
    ofPlanePrimitive& plane = ((ofPlanePrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_PLANE));
    return ofVec2f(plane.getResolution().x, plane.getResolution().y);
}

//----------------------------------------------------------
void ofPlane(float x, float y, float width, float height) {
    ofPlane( x, y, 0, width, height);
}

//----------------------------------------------------------
void ofPlane(float x, float y, float z, float width, float height) {
    ofPushMatrix();
    ofTranslate(x, y, z);
    ofPlane(width, height);
    ofPopMatrix();
}

//----------------------------------------------------------
void ofPlane(ofPoint& position, float width, float height) {
    ofPlane(position.x,position.y,position.z,width, height);
}

//----------------------------------------------------------
void ofPlane( float width, float height ) {
    of3dModel& plane = getCached3dPrimitive( OF_3D_PRIMITIVE_PLANE );
    plane.setScale( width, height, 1.0 );
    renderCached3dPrimitive( plane );
}


// UV SPHERE //
//----------------------------------------------------------
void ofSetSphereResolution(int res) {
    if(ofGetSphereResolution() != res) {
        ((ofSpherePrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_SPHERE)).setResolution(res);
    }
}

//---------------------------------------------------------
int ofGetSphereResolution() {
    return ((ofSpherePrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_SPHERE)).getResolution().x;
}

//----------------------------------------------------------
void ofSphere(float x, float y, float z, float radius) {
    ofPushMatrix();
    ofTranslate(x, y, z);
    ofSphere(radius);
    ofPopMatrix();
}

//----------------------------------------------------------
void ofSphere(float x, float y, float radius) {
    ofSphere(x, y, 0, radius);
}

//----------------------------------------------------------
void ofSphere(const ofPoint& position, float radius) {
    ofSphere(position.x,position.y,position.z,radius);
}

//----------------------------------------------------------
void ofSphere(float radius) {
    of3dModel& sphere = getCached3dPrimitive( OF_3D_PRIMITIVE_SPHERE );
    sphere.setScale( radius, radius, radius );
    renderCached3dPrimitive( sphere );
}


// ICO SPHERE //
//----------------------------------------------------------
void ofSetIcoSphereResolution( int res ) {
    if(ofGetIcoSphereResolution() != res) {
        ((ofIcoSpherePrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_ICO_SPHERE)).setResolution(res);
    }
}

//----------------------------------------------------------
int ofGetIcoSphereResolution() {
    return ((ofIcoSpherePrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_ICO_SPHERE)).getResolution().x;
}

//----------------------------------------------------------
void ofIcoSphere(float x, float y, float z, float radius) {
    ofPushMatrix();
    ofTranslate(x, y, z);
    ofIcoSphere(radius);
    ofPopMatrix();
}

//----------------------------------------------------------
void ofIcoSphere(float x, float y, float radius) {
    ofIcoSphere(x, y, 0, radius);
}

//----------------------------------------------------------
void ofIcoSphere(const ofPoint& position, float radius) {
    ofIcoSphere(position.x,position.y,position.z,radius);
}

//----------------------------------------------------------
void ofIcoSphere(float radius) {
    of3dModel& sphere = getCached3dPrimitive( OF_3D_PRIMITIVE_ICO_SPHERE );
    sphere.setScale( radius );
    renderCached3dPrimitive( sphere );
}



// CONE //
//----------------------------------------------------------
void ofCone(float x, float y, float z, float radius, float height) {
    
}

//----------------------------------------------------------
void ofCone(float x, float y, float radius, float height) {
    
}

//----------------------------------------------------------
void ofCone(const ofPoint& position, float radius, float height) {
    
}

//----------------------------------------------------------
void ofCone(float radius, float height) {
    
}



/*
 //--------------------------------------------------
 // 3d primitives
 
 //----------------------------------------
 void ofSphere(float x, float y, float z, float radius){
 ofPushMatrix();
 ofTranslate(x, y, z);
 ofSphere(radius);
 ofPopMatrix();
 }
 
 //----------------------------------------
 void ofSphere(float x, float y, float radius){
 ofSphere(x, y, 0, radius);
 }
 
 //----------------------------------------
 void ofSphere(const ofPoint& position, float radius){
 ofSphere(position.x,position.y,position.z,radius);
 }
 
 //----------------------------------------
 void ofSphere(float radius){
 renderer->drawSphere(0,0,0,radius);
 }
 
 //----------------------------------------
 void ofBox(float x, float y, float z, float size){
 ofBox(ofPoint(x, y, z), size);
 }
 
 //----------------------------------------
 void ofBox(float x, float y, float size){
 ofBox(x, y, 0, size);
 }
 
 //----------------------------------------
 void ofBox(const ofPoint& position, float size){
 ofPushMatrix();
 ofTranslate(position);
 ofBox(size);
 ofPopMatrix();
 }
 
 //----------------------------------------
 void ofBox(float size){
 ofPushMatrix();
 if(ofGetCoordHandedness() == OF_LEFT_HANDED){
 ofScale(1, 1, -1);
 }
 
 float h = size * .5;
 
 vertexData.clear();
 if(ofGetStyle().bFill){
 ofVec3f vertices[] = {
 ofVec3f(+h,-h,+h), ofVec3f(+h,-h,-h), ofVec3f(+h,+h,-h), ofVec3f(+h,+h,+h),
 ofVec3f(+h,+h,+h), ofVec3f(+h,+h,-h), ofVec3f(-h,+h,-h), ofVec3f(-h,+h,+h),
 ofVec3f(+h,+h,+h), ofVec3f(-h,+h,+h), ofVec3f(-h,-h,+h), ofVec3f(+h,-h,+h),
 ofVec3f(-h,-h,+h), ofVec3f(-h,+h,+h), ofVec3f(-h,+h,-h), ofVec3f(-h,-h,-h),
 ofVec3f(-h,-h,+h), ofVec3f(-h,-h,-h), ofVec3f(+h,-h,-h), ofVec3f(+h,-h,+h),
 ofVec3f(-h,-h,-h), ofVec3f(-h,+h,-h), ofVec3f(+h,+h,-h), ofVec3f(+h,-h,-h)
 };
 vertexData.addVertices(vertices,24);
 
 static ofVec3f normals[] = {
 ofVec3f(+1,0,0), ofVec3f(+1,0,0), ofVec3f(+1,0,0), ofVec3f(+1,0,0),
 ofVec3f(0,+1,0), ofVec3f(0,+1,0), ofVec3f(0,+1,0), ofVec3f(0,+1,0),
 ofVec3f(0,0,+1), ofVec3f(0,0,+1), ofVec3f(0,0,+1), ofVec3f(0,0,+1),
 ofVec3f(-1,0,0), ofVec3f(-1,0,0), ofVec3f(-1,0,0), ofVec3f(-1,0,0),
 ofVec3f(0,-1,0), ofVec3f(0,-1,0), ofVec3f(0,-1,0), ofVec3f(0,-1,0),
 ofVec3f(0,0,-1), ofVec3f(0,0,-1), ofVec3f(0,0,-1), ofVec3f(0,0,-1)
 };
 vertexData.addNormals(normals,24);
 
 static ofVec2f tex[] = {
 ofVec2f(1,0), ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1),
 ofVec2f(1,1), ofVec2f(1,0), ofVec2f(0,0), ofVec2f(0,1),
 ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0), ofVec2f(0,0),
 ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0),
 ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0),
 ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0)
 };
 vertexData.addTexCoords(tex,24);
 
 static ofIndexType indices[] = {
 0,1,2, // right top left
 0,2,3, // right bottom right
 4,5,6, // bottom top right
 4,6,7, // bottom bottom left
 8,9,10, // back bottom right
 8,10,11, // back top left
 12,13,14, // left bottom right
 12,14,15, // left top left
 16,17,18, // ... etc
 16,18,19,
 20,21,22,
 20,22,23
 };
 vertexData.addIndices(indices,36);
 vertexData.setMode(OF_PRIMITIVE_TRIANGLES);
 renderer->draw(vertexData,vertexData.usingColors(),vertexData.usingTextures(),vertexData.usingNormals());
 } else {
 ofVec3f vertices[] = {
 ofVec3f(+h,+h,+h),
 ofVec3f(+h,+h,-h),
 ofVec3f(+h,-h,+h),
 ofVec3f(+h,-h,-h),
 ofVec3f(-h,+h,+h),
 ofVec3f(-h,+h,-h),
 ofVec3f(-h,-h,+h),
 ofVec3f(-h,-h,-h)
 };
 vertexData.addVertices(vertices,8);
 
 static float n = sqrtf(3);
 static ofVec3f normals[] = {
 ofVec3f(+n,+n,+n),
 ofVec3f(+n,+n,-n),
 ofVec3f(+n,-n,+n),
 ofVec3f(+n,-n,-n),
 ofVec3f(-n,+n,+n),
 ofVec3f(-n,+n,-n),
 ofVec3f(-n,-n,+n),
 ofVec3f(-n,-n,-n)
 };
 vertexData.addNormals(normals,8);
 
 static ofIndexType indices[] = {
 0,1, 1,3, 3,2, 2,0,
 4,5, 5,7, 7,6, 6,4,
 0,4, 5,1, 7,3, 6,2
 };
 vertexData.addIndices(indices,24);
 
 vertexData.setMode(OF_PRIMITIVE_LINES);
 renderer->draw(vertexData, vertexData.usingColors(),vertexData.usingTextures(),vertexData.usingNormals());
 }
 
 
 ofPopMatrix();
 }
 
 //----------------------------------------
 void ofCone(float x, float y, float z, float radius, float height) {
 ofCone(ofPoint(x, y, z), radius, height);
 }
 
 //----------------------------------------
 void ofCone(float x, float y, float radius, float height) {
 ofCone(x, y, 0, radius, height);
 }
 
 //----------------------------------------
 void ofCone(const ofPoint& position, float radius, float height) {
 ofPushMatrix();
 ofTranslate(position);
 ofCone(radius, height);
 ofPopMatrix();
 }
 
 //----------------------------------------
 void ofCone(float radius, float height) {
 // TODO: add an implementation using ofMesh
 #ifndef TARGET_OPENGLES
 // this needs to be swapped out with non-glut code
 // see ofSphere above
 
 if(ofGetStyle().bFill) {
 glutSolidCone(radius, height, currentStyle.circleResolution, 1);
 } else {
 glutWireCone(radius, height, currentStyle.circleResolution, 1);
 }
 #endif
 }
 */

// end 3d primitives
//--------------------------------------------------












