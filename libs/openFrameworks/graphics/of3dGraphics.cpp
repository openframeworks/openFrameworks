//
//  of3dGraphics.cpp
//  openFrameworksLib
//
//  Created by Nick Hardeman on 9/14/12.
//
//

#include "of3dGraphics.h"
#include "ofGraphics.h"
#include "ofVboMesh.h"
#include <map>


enum of3dPrimitiveType {
	OF_3D_PRIMITIVE_PLANE,
    OF_3D_PRIMITIVE_SPHERE,
    OF_3D_PRIMITIVE_ICO_SPHERE,
	OF_3D_PRIMITIVE_BOX,
	OF_3D_PRIMITIVE_CONE,
    OF_3D_PRIMITIVE_CYLINDER,
    OF_3D_PRIMITIVE_BOX_WIREFRAME
};


//----------------------------------------------------------
static of3dPrimitive& getCached3dPrimitive( of3dPrimitiveType type ) {
    switch (type) {
        case OF_3D_PRIMITIVE_PLANE:{
        	static ofPlanePrimitive * plane = new ofPlanePrimitive(1.0f, 1.0f, 6, 4);
        	return *plane;
        }break;
        case OF_3D_PRIMITIVE_SPHERE:{
        	static ofSpherePrimitive * sphere = new ofSpherePrimitive(1.0f, 20);
        	return *sphere;
        }break;
        case OF_3D_PRIMITIVE_ICO_SPHERE:{
        	static ofIcoSpherePrimitive * icosphere = new ofIcoSpherePrimitive(1.0f, 2);
        	return *icosphere;
        }break;
        case OF_3D_PRIMITIVE_BOX:{
        	static ofBoxPrimitive * box = new  ofBoxPrimitive( 1.f, 1.f, 1.f );
        	return *box;
        }break;
        case OF_3D_PRIMITIVE_CONE:{
        	static ofConePrimitive * cone = new ofConePrimitive( 1.f, 1.f, 9, 3, 2 );
        	return *cone;
        }break;
        case OF_3D_PRIMITIVE_CYLINDER:{
        	static ofCylinderPrimitive * cylinder = new ofCylinderPrimitive(1.f, 1.f, 8, 4, 2, true);
        	return *cylinder;
        }break;
            // special case for rendering with lines, so the triangles are not visible //
        case OF_3D_PRIMITIVE_BOX_WIREFRAME: {
            static ofBoxPrimitive * boxWireframe = new ofBoxPrimitive( 1.f, 1.f, 1.f );
            ofMesh* boxWireframeMesh = boxWireframe->getMeshPtr();
            boxWireframeMesh->clear();
            boxWireframeMesh->setMode( OF_PRIMITIVE_LINES );
            
            boxWireframeMesh->addVertex(ofVec3f(-.5, -.5, -.5));
            boxWireframeMesh->addVertex(ofVec3f(.5, -.5, -.5));
            boxWireframeMesh->addVertex(ofVec3f(.5, .5, -.5));
            boxWireframeMesh->addVertex(ofVec3f(-.5, .5, -.5));
            
            boxWireframeMesh->addVertex(ofVec3f(-.5, -.5, .5));
            boxWireframeMesh->addVertex(ofVec3f(.5, -.5, .5));
            boxWireframeMesh->addVertex(ofVec3f(.5, .5, .5));
            boxWireframeMesh->addVertex(ofVec3f(-.5, .5, .5));
            
            // front face
            boxWireframeMesh->addIndex(0);
            boxWireframeMesh->addIndex(1);
            
            boxWireframeMesh->addIndex(1);
            boxWireframeMesh->addIndex(2);
            
            boxWireframeMesh->addIndex(2);
            boxWireframeMesh->addIndex(3);
            
            boxWireframeMesh->addIndex(3);
            boxWireframeMesh->addIndex(0);
            
            // back face
            boxWireframeMesh->addIndex(4);
            boxWireframeMesh->addIndex(5);
            
            boxWireframeMesh->addIndex(5);
            boxWireframeMesh->addIndex(6);
            
            boxWireframeMesh->addIndex(6);
            boxWireframeMesh->addIndex(7);
            
            boxWireframeMesh->addIndex(7);
            boxWireframeMesh->addIndex(4);
            
            
            boxWireframeMesh->addIndex(0);
            boxWireframeMesh->addIndex(4);
            
            boxWireframeMesh->addIndex(1);
            boxWireframeMesh->addIndex(5);
            
            boxWireframeMesh->addIndex(2);
            boxWireframeMesh->addIndex(6);
            
            boxWireframeMesh->addIndex(3);
            boxWireframeMesh->addIndex(7);
            
            
            return * boxWireframe;
        }break;
        default:{
        	static of3dPrimitive * primitive = new of3dPrimitive;
        	return * primitive;
        }break;
    }
}

//----------------------------------------------------------
static void renderCached3dPrimitive( ofMesh& model ) {
    if(ofGetFill() == OF_FILLED) {
    	model.draw(OF_MESH_FILL);
    } else {
        model.draw(OF_MESH_WIREFRAME);
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
    return plane.getResolution();
}

//----------------------------------------------------------
void ofDrawPlane(float x, float y, float width, float height) {
    ofDrawPlane( x, y, 0, width, height);
}

//----------------------------------------------------------
void ofDrawPlane(float x, float y, float z, float width, float height) {
	static ofMatrix4x4 m;
	m.makeScaleMatrix(width,height,1);
	m.translate(x,y,z);
    ofMesh & plane = getCached3dPrimitive( OF_3D_PRIMITIVE_PLANE ).getMesh();
    ofPushMatrix();
    ofMultMatrix(m);
    renderCached3dPrimitive( plane );
    ofPopMatrix();
}

//----------------------------------------------------------
void ofDrawPlane(ofPoint& position, float width, float height) {
    ofDrawPlane(position.x,position.y,position.z,width, height);
}

//----------------------------------------------------------
void ofDrawPlane( float width, float height ) {
	static ofMatrix4x4 m;
	m.makeScaleMatrix(width,height,1);
    ofMesh & plane = getCached3dPrimitive( OF_3D_PRIMITIVE_PLANE ).getMesh();
    ofPushMatrix();
    ofMultMatrix(m);
    renderCached3dPrimitive( plane );
    ofPopMatrix();
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
    return ((ofSpherePrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_SPHERE)).getResolution();
}

//----------------------------------------------------------
void ofDrawSphere(float x, float y, float z, float radius) {
	static ofMatrix4x4 m;
	m.makeScaleMatrix(radius,radius,radius);
	m.translate(x,y,z);
	ofMesh& sphere = getCached3dPrimitive( OF_3D_PRIMITIVE_SPHERE ).getMesh();
    ofPushMatrix();
    ofMultMatrix(m);
    renderCached3dPrimitive( sphere );
    ofPopMatrix();
}

//----------------------------------------------------------
void ofDrawSphere(float x, float y, float radius) {
    ofDrawSphere(x, y, 0, radius);
}

//----------------------------------------------------------
void ofDrawSphere(const ofPoint& position, float radius) {
    ofDrawSphere(position.x,position.y,position.z,radius);
}

//----------------------------------------------------------
void ofDrawSphere(float radius) {
	static ofMatrix4x4 m;
	m.makeScaleMatrix(radius,radius,radius);
	ofMesh& sphere = getCached3dPrimitive( OF_3D_PRIMITIVE_SPHERE ).getMesh();
    ofPushMatrix();
    ofMultMatrix(m);
    renderCached3dPrimitive( sphere );
    ofPopMatrix();
}

// Deprecated methods - for compatability with previous versions of OF //
//----------------------------------------------------------
void ofSphere(float x, float y, float z, float radius) {
    ofDrawSphere(x, y, z, radius);
}

//----------------------------------------------------------
void ofSphere(float x, float y, float radius) {
    ofDrawSphere(x, y, 0, radius);
}

//----------------------------------------------------------
void ofSphere(const ofPoint& position, float radius) {
    ofDrawSphere(position.x,position.y,position.z,radius);
}

//----------------------------------------------------------
void ofSphere(float radius) {
    ofDrawSphere(radius);
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
    return ((ofIcoSpherePrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_ICO_SPHERE)).getResolution();
}

//----------------------------------------------------------
void ofDrawIcoSphere(float x, float y, float z, float radius) {
	static ofMatrix4x4 m;
	m.makeScaleMatrix(radius,radius,radius);
	m.translate(x,y,z);
	ofMesh& sphere = getCached3dPrimitive( OF_3D_PRIMITIVE_ICO_SPHERE ).getMesh();
    ofPushMatrix();
    ofMultMatrix(m);
    renderCached3dPrimitive( sphere );
    ofPopMatrix();
}

//----------------------------------------------------------
void ofDrawIcoSphere(float x, float y, float radius) {
    ofDrawIcoSphere(x, y, 0, radius);
}

//----------------------------------------------------------
void ofDrawIcoSphere(const ofPoint& position, float radius) {
    ofDrawIcoSphere(position.x,position.y,position.z,radius);
}

//----------------------------------------------------------
void ofDrawIcoSphere(float radius) {
	static ofMatrix4x4 m;
	m.makeScaleMatrix(radius,radius,radius);
	ofMesh& sphere = getCached3dPrimitive( OF_3D_PRIMITIVE_ICO_SPHERE ).getMesh();
    ofPushMatrix();
    ofMultMatrix(m);
    renderCached3dPrimitive( sphere );
    ofPopMatrix();
}


// Cylinder //
//----------------------------------------------------------
void ofSetCylinderResolution( int radiusSegments, int heightSegments, int capSegments ) {
    if(ofGetCylinderResolution() != ofVec3f( radiusSegments, heightSegments, capSegments )) {
        ((ofCylinderPrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_CYLINDER)).setResolution(radiusSegments, heightSegments, capSegments);
    }
}

//----------------------------------------------------------
ofVec3f ofGetCylinderResolution() {
    return ((ofCylinderPrimitive&)getCached3dPrimitive(OF_3D_PRIMITIVE_CYLINDER)).getResolution();
}

//----------------------------------------------------------
void ofDrawCylinder(float x, float y, float radius, float height) {
    ofDrawCylinder( x, y, 0, radius, height );
}

//----------------------------------------------------------
void ofDrawCylinder(float x, float y, float z, float radius, float height) {
	static ofMatrix4x4 m;
	m.makeScaleMatrix(radius,height,radius);
	m.translate(x,y,z);
	ofMesh& mesh = getCached3dPrimitive( OF_3D_PRIMITIVE_CYLINDER ).getMesh();
    ofPushMatrix();
    ofMultMatrix(m);
    renderCached3dPrimitive( mesh );
    ofPopMatrix();
}

//----------------------------------------------------------
void ofDrawCylinder(const ofPoint& position, float radius, float height) {
    ofDrawCylinder( position.x, position.y, position.z, radius, height );
}

//----------------------------------------------------------
void ofDrawCylinder(float radius, float height) {
	static ofMatrix4x4 m;
	m.makeScaleMatrix(radius,height,radius);
	ofMesh& mesh = getCached3dPrimitive( OF_3D_PRIMITIVE_CYLINDER ).getMesh();
    ofPushMatrix();
    ofMultMatrix(m);
    renderCached3dPrimitive( mesh );
    ofPopMatrix();
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
void ofDrawCone(float x, float y, float z, float radius, float height) {
	static ofMatrix4x4 m;
	m.makeScaleMatrix(radius,height,radius);
	m.translate(x,y,z);
	ofMesh& mesh = getCached3dPrimitive( OF_3D_PRIMITIVE_CONE ).getMesh();
    ofPushMatrix();
    ofMultMatrix(m);
    renderCached3dPrimitive( mesh );
    ofPopMatrix();
}

//----------------------------------------------------------
void ofDrawCone(float x, float y, float radius, float height) {
	ofDrawCone( x, y, 0, radius, height );
}

//----------------------------------------------------------
void ofDrawCone(const ofPoint& position, float radius, float height) {
	ofDrawCone( position.x, position.y, position.z, radius, height );
}

//----------------------------------------------------------
void ofDrawCone(float radius, float height) {
	static ofMatrix4x4 m;
	m.makeScaleMatrix(radius,height,radius);
	ofMesh& mesh = getCached3dPrimitive( OF_3D_PRIMITIVE_CONE ).getMesh();
    ofPushMatrix();
    ofMultMatrix(m);
    renderCached3dPrimitive( mesh );
    ofPopMatrix();
}

//----------------------------------------------------------
void ofCone(float x, float y, float z, float radius, float height) {
    ofDrawCone(x, y, z, radius, height);
}

//----------------------------------------------------------
void ofCone(float x, float y, float radius, float height) {
    ofDrawCone( x, y, 0, radius, height );
}

//----------------------------------------------------------
void ofCone(const ofPoint& position, float radius, float height) {
    ofDrawCone( position.x, position.y, position.z, radius, height );
}

//----------------------------------------------------------
void ofCone(float radius, float height) {
    ofDrawCone(radius, height );
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
void ofDrawBox( float x, float y, float z, float width, float height, float depth) {
	static ofMatrix4x4 m;
	m.makeScaleMatrix(width,height,depth);
	m.translate(x,y,z);
	
    ofPushMatrix();
    ofMultMatrix(m);
    if(ofGetFill() == OF_FILLED) {
    	ofMesh& mesh = getCached3dPrimitive( OF_3D_PRIMITIVE_BOX ).getMesh();
        renderCached3dPrimitive( mesh );
    } else {
        ofMesh& mesh = getCached3dPrimitive( OF_3D_PRIMITIVE_BOX_WIREFRAME ).getMesh();
        renderCached3dPrimitive( mesh );
    }
    ofPopMatrix();
}

//----------------------------------------------------------
void ofDrawBox(float x, float y, float z, float size) {
	ofDrawBox( x, y, z, size, size, size );
}

//----------------------------------------------------------
void ofDrawBox(const ofPoint& position, float width, float height, float depth) {
	ofDrawBox( position.x, position.y, position.z, width, height, depth );
}

//----------------------------------------------------------
void ofDrawBox(const ofPoint& position, float size) {
	ofDrawBox( position.x, position.y, position.z, size, size, size );
}

//----------------------------------------------------------
void ofDrawBox(float size) {
	ofDrawBox( size, size, size );
}

//----------------------------------------------------------
void ofDrawBox( float width, float height, float depth ) {
	static ofMatrix4x4 m;
	m.makeScaleMatrix(width,height,depth);
    ofPushMatrix();
    ofMultMatrix(m);
    if(ofGetFill() == OF_FILLED) {
    	ofMesh& mesh = getCached3dPrimitive( OF_3D_PRIMITIVE_BOX ).getMesh();
        renderCached3dPrimitive( mesh );
    } else {
        ofMesh& mesh = getCached3dPrimitive( OF_3D_PRIMITIVE_BOX_WIREFRAME ).getMesh();
        renderCached3dPrimitive( mesh );
    }
    ofPopMatrix();
}

// deprecated methods //
//----------------------------------------------------------
void ofBox( float x, float y, float z, float width, float height, float depth) {
    ofDrawBox(x,y,z,width,height,depth);
}

//----------------------------------------------------------
void ofBox(float x, float y, float z, float size) {
    ofDrawBox( x, y, z, size, size, size );
}

//----------------------------------------------------------
void ofBox(const ofPoint& position, float width, float height, float depth) {
    ofDrawBox( position.x, position.y, position.z, width, height, depth );
}

//----------------------------------------------------------
void ofBox(const ofPoint& position, float size) {
    ofDrawBox( position.x, position.y, position.z, size, size, size );
}

//----------------------------------------------------------
void ofBox(float size) {
    ofDrawBox( size, size, size );
}

//----------------------------------------------------------
void ofBox( float width, float height, float depth ) {
    ofDrawBox(width,height,depth);
}













