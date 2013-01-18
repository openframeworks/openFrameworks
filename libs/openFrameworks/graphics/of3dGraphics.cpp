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
static map<of3dPrimitiveType, ofPrimitiveBase*>& getCached3dPrimitives() {
    static map<of3dPrimitiveType, ofPrimitiveBase*>* primitives = new map<of3dPrimitiveType, ofPrimitiveBase*>;
    return *primitives;
}

//----------------------------------------------------------
static ofPrimitiveBase& getCached3dPrimitive( of3dPrimitiveType type ) {
    switch (type) {
        case OF_3D_PRIMITIVE_PLANE:
            if(getCached3dPrimitives().find(type) == getCached3dPrimitives().end()) {
                cout << "of3dGraphics :: getCached3dPrimitive : inserting plane - " << type << endl;
                getCached3dPrimitives().insert( make_pair<of3dPrimitiveType,
                                               ofPrimitiveBase*>(type, new ofPlanePrimitive(1.0f, 1.0f, 6, 4)));
            }
            break;
        case OF_3D_PRIMITIVE_SPHERE:
            if(getCached3dPrimitives().find(type) == getCached3dPrimitives().end()) {
                cout << "of3dGraphics :: getCached3dPrimitive : inserting sphere - " << type << endl;
                getCached3dPrimitives().insert( make_pair<of3dPrimitiveType,
                                               ofPrimitiveBase*>(type, new ofSpherePrimitive(1.0f, 4)));
            }
            break;
        case OF_3D_PRIMITIVE_ICO_SPHERE:
            if(getCached3dPrimitives().find(type) == getCached3dPrimitives().end()) {
                cout << "of3dGraphics :: getCached3dPrimitive : inserting ICO sphere - " << type << endl;
                getCached3dPrimitives().insert( make_pair<of3dPrimitiveType,
                                               ofPrimitiveBase*>(type, new ofIcoSpherePrimitive(1.0f, 2)));
            }
            break;
        case OF_3D_PRIMITIVE_BOX:
            if(getCached3dPrimitives().find(type) == getCached3dPrimitives().end()) {
                cout << "of3dGraphics :: getCached3dPrimitive : inserting box " << type << endl;
                getCached3dPrimitives().insert(make_pair<of3dPrimitiveType,
                                               ofPrimitiveBase*>(type, new  ofBoxPrimitive( 1.f, 1.f, 1.f )  ));
            }
            break;
        case OF_3D_PRIMITIVE_CONE:
            if(getCached3dPrimitives().find(type) == getCached3dPrimitives().end()) {
                cout << "of3dGraphics :: getCached3dPrimitive : inserting cone " << type << endl;
                getCached3dPrimitives().insert(make_pair<of3dPrimitiveType,
                                      ofPrimitiveBase*>(type, new ofConePrimitive( 1.f, 1.f, 9, 3, 2)));
            }
            break;
        case OF_3D_PRIMITIVE_CYLINDER:
            if(getCached3dPrimitives().find(type) == getCached3dPrimitives().end() ) {
                cout << "of3dGraphics :: getCached3dPrimitive : inserting Cylinder - " << endl;
                getCached3dPrimitives().insert( make_pair<of3dPrimitiveType,
                                               ofPrimitiveBase*>(type, new ofCylinderPrimitive(1.f, 1.f, 8, 4, 2, true)));
            }
            break;
        default:
            break;
    }
    return *getCached3dPrimitives().find(type)->second;
}

//----------------------------------------------------------
static void renderCached3dPrimitive( ofPrimitiveBase& model ) {
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
    ofPrimitiveBase& plane = getCached3dPrimitive( OF_3D_PRIMITIVE_PLANE );
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
    ofPrimitiveBase& sphere = getCached3dPrimitive( OF_3D_PRIMITIVE_SPHERE );
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
    ofPrimitiveBase& sphere = getCached3dPrimitive( OF_3D_PRIMITIVE_ICO_SPHERE );
    sphere.setScale( radius );
    renderCached3dPrimitive( sphere );
}


// Cylinder //
void ofSetCylinderResolution( int radiusSegments, int heightSegments, int capSegments ) {
    if(ofGetCylinderResolution() != ofVec3f( radiusSegments, heightSegments, capSegments )) {
        ((ofCylinderPrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_CYLINDER)).setResolution(radiusSegments, heightSegments, capSegments);
    }
}
ofVec3f ofGetCylinderResolution() {
    return ((ofCylinderPrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_CYLINDER)).getResolution();
}
void ofCylinder(float x, float y, float radius, float height) {
    ofCylinder( x, y, 0, radius, height );
}
void ofCylinder(float x, float y, float z, float radius, float height) {
    ofPushMatrix();
    ofTranslate(x, y, z);
    ofCylinder(radius, height);
    ofPopMatrix();
}
void ofCylinder(const ofPoint& position, float radius, float height) {
    ofCylinder( position.x, position.y, position.z, radius, height );
}
void ofCylinder(float radius, float height) {
    ofPrimitiveBase& mesh = getCached3dPrimitive( OF_3D_PRIMITIVE_CYLINDER );
    mesh.setScale( radius, height, radius );
    renderCached3dPrimitive( mesh );
}



// CONE //
//----------------------------------------------------------
void ofSetConeResolution( int radiusSegments, int heightSegments, int capSegments) {
    if(ofGetConeResolution() != ofVec3f( radiusSegments, heightSegments, capSegments )) {
        ((ofConePrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_CONE)).setResolution(radiusSegments, heightSegments, capSegments);
    }
}

//----------------------------------------------------------
ofVec3f ofGetConeResolution() {
    return ((ofConePrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_CONE)).getResolution();
}

//----------------------------------------------------------
void ofCone(float x, float y, float z, float radius, float height) {
    ofPushMatrix();
    ofTranslate(x, y, z);
    ofCone(radius, height);
    ofPopMatrix();
}

//----------------------------------------------------------
void ofCone(float x, float y, float radius, float height) {
    ofCone( x, y, 0, radius, height );
}

//----------------------------------------------------------
void ofCone(const ofPoint& position, float radius, float height) {
    ofCone( position.x, position.y, position.z, radius, height );
}

//----------------------------------------------------------
void ofCone(float radius, float height) {
    ofPrimitiveBase& mesh = getCached3dPrimitive( OF_3D_PRIMITIVE_CONE );
    mesh.setScale( radius, height, radius );
    renderCached3dPrimitive( mesh );
}




// BOX //
//----------------------------------------------------------
void ofSetBoxResolution( int res ) {
    ofSetBoxResolution(res, res, res);
}

//----------------------------------------------------------
void ofSetBoxResolution( int resWidth, int resHeight, int resDepth ) {
    if(ofGetBoxResolution() != ofVec3f( resWidth, resHeight, resDepth )) {
        ((ofBoxPrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_BOX)).setResolution(resWidth, resHeight, resDepth);
    }
}

//----------------------------------------------------------
ofVec3f ofGetBoxResolution() {
    return ((ofBoxPrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_BOX)).getResolution();
}

//----------------------------------------------------------
void ofBox( float x, float y, float z, float width, float height, float depth) {
    ofPushMatrix();
    ofTranslate(x, y, z);
    ofBox( width, height, depth );
    ofPopMatrix();
}

//----------------------------------------------------------
void ofBox(float x, float y, float z, float size) {
    ofBox( x, y, z, size, size, size );
}

//----------------------------------------------------------
void ofBox(const ofPoint& position, float width, float height, float depth) {
    ofBox( position.x, position.y, position.z, width, height, depth );
}

//----------------------------------------------------------
void ofBox(const ofPoint& position, float size) {
    ofBox( position.x, position.y, position.z, size, size, size );
}

//----------------------------------------------------------
void ofBox(float size) {
    ofBox( size, size, size );
}

//----------------------------------------------------------
void ofBox( float width, float height, float depth ) {
    ofPrimitiveBase& mesh = getCached3dPrimitive( OF_3D_PRIMITIVE_BOX );
    mesh.setScale( width, height, depth );
    renderCached3dPrimitive( mesh );
}













