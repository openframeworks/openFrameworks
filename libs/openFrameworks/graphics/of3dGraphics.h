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
void ofPlane(float x, float y, float width, float height);
void ofPlane(float x, float y, float z, float width, float height);
void ofPlane(ofPoint& position, float width, float height);
void ofPlane( float width, float height );

// UV Sphere
void ofSetSphereResolution(int res);
int ofGetSphereResolution();
void ofSphere(float x, float y, float radius);
void ofSphere(float x, float y, float z, float radius);
void ofSphere(const ofPoint& position, float radius);
void ofSphere(float radius);

// Ico Sphere
void ofSetIcoSphereResolution( int res );
int ofGetIcoSphereResolution();
void ofIcoSphere(float x, float y, float z, float radius);
void ofIcoSphere(float x, float y, float radius);
void ofIcoSphere(const ofPoint& position, float radius);
void ofIcoSphere(float radius);

// Cylinder //
void ofSetCylinderResolution( int radiusSegments, int heightSegments, int capSegments=2 );
ofVec3f ofGetCylinderResolution();
void ofCylinder(float x, float y, float radius, float height);
void ofCylinder(float x, float y, float z, float radius, float height);
void ofCylinder(const ofPoint& position, float radius, float height);
void ofCylinder(float radius, float height);


void ofBox(float x, float y, float z, float size);
void ofBox(float x, float y, float size);
void ofBox(const ofPoint& position, float size);
void ofBox(float size);

void ofCone(float x, float y, float z, float radius, float height);
void ofCone(float x, float y, float radius, float height);
void ofCone(const ofPoint& position, float radius, float height);
void ofCone(float radius, float height);
