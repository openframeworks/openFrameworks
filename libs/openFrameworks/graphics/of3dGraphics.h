//
//  of3dGraphics.h
//  openFrameworksLib
//
//  Created by Nick Hardeman on 9/14/12.
//
//

#pragma once

#include "ofConstants.h"
#include "ofColor.h"
#include "ofPoint.h"
#include "ofMatrix4x4.h"
#include "ofRectangle.h"
#include "ofTypes.h"
#include "ofBaseTypes.h"
#include "ofGLRenderer.h"
#include "of3dPrimitives.h"


// Grid Plane - drawn from the center //
void ofSetPlaneResolution( int columns, int rows );
ofVec2f ofGetPlaneResolution();
void ofDrawPlane(float x, float y, float width, float height);
void ofDrawPlane(float x, float y, float z, float width, float height);
void ofDrawPlane(ofPoint& position, float width, float height);
void ofDrawPlane( float width, float height );

// UV Sphere
void ofSetSphereResolution(int res);
int ofGetSphereResolution();
void ofDrawSphere(float x, float y, float radius);
void ofDrawSphere(float x, float y, float z, float radius);
void ofDrawSphere(const ofPoint& position, float radius);
void ofDrawSphere(float radius);

// Deprecated Methods //
OF_DEPRECATED_MSG("Use ofDrawSphere instead.", void ofSphere(float x, float y, float radius) );
OF_DEPRECATED_MSG("Use ofDrawSphere instead.", void ofSphere(float x, float y, float z, float radius) );
OF_DEPRECATED_MSG("Use ofDrawSphere instead.", void ofSphere(const ofPoint& position, float radius) );
OF_DEPRECATED_MSG("Use ofDrawSphere instead.", void ofSphere(float radius) );


// Ico Sphere
void ofSetIcoSphereResolution( int res );
int ofGetIcoSphereResolution();
void ofDrawIcoSphere(float x, float y, float z, float radius);
void ofDrawIcoSphere(float x, float y, float radius);
void ofDrawIcoSphere(const ofPoint& position, float radius);
void ofDrawIcoSphere(float radius);

// Cylinder //
void ofSetCylinderResolution( int radiusSegments, int heightSegments, int capSegments=2 );
ofVec3f ofGetCylinderResolution();
void ofDrawCylinder(float x, float y, float radius, float height);
void ofDrawCylinder(float x, float y, float z, float radius, float height);
void ofDrawCylinder(const ofPoint& position, float radius, float height);
void ofDrawCylinder(float radius, float height);

// Cone
void ofSetConeResolution( int radiusSegments, int heightSegments, int capSegments=2);
ofVec3f ofGetConeResolution();
void ofDrawCone(float x, float y, float z, float radius, float height);
void ofDrawCone(float x, float y, float radius, float height);
void ofDrawCone(const ofPoint& position, float radius, float height);
void ofDrawCone(float radius, float height);

// Deprecated Methods //
OF_DEPRECATED_MSG("Use ofDrawCone instead.", void ofCone(float x, float y, float z, float radius, float height) );
OF_DEPRECATED_MSG("Use ofDrawCone instead.", void ofCone(float x, float y, float radius, float height) );
OF_DEPRECATED_MSG("Use ofDrawCone instead.", void ofCone(const ofPoint& position, float radius, float height) );
OF_DEPRECATED_MSG("Use ofDrawCone instead.", void ofCone(float radius, float height) );

// Box
void ofSetBoxResolution( int res );
void ofSetBoxResolution( int resWidth, int resHeight, int resDepth );
ofVec3f ofGetBoxResolution();
void ofDrawBox( float x, float y, float z, float width, float height, float depth);
void ofDrawBox(float x, float y, float z, float size);
void ofDrawBox(const ofPoint& position, float width, float height, float depth);
void ofDrawBox(const ofPoint& position, float size);
void ofDrawBox(float size);
void ofDrawBox( float width, float height, float depth );

// deprecated methods //
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox( float x, float y, float z, float width, float height, float depth) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox(float x, float y, float z, float size) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox(const ofPoint& position, float width, float height, float depth) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox(const ofPoint& position, float size) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox(float size) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox( float width, float height, float depth ) );









