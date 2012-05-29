#pragma once

#include "ofConstants.h"
#include "ofColor.h"
#include "ofPoint.h"
#include "ofMatrix4x4.h"
#include "ofRectangle.h"
#include "ofTypes.h"
#include "ofBaseTypes.h"
#include "ofGLRenderer.h"

#define  	CIRC_RESOLUTION		    22				// 22 pts for a circle...


void ofSetCurrentRenderer(ofPtr<ofBaseRenderer> renderer);
ofPtr<ofBaseRenderer> & ofGetCurrentRenderer();
ofPtr<ofGLRenderer> ofGetGLRenderer();

//for pdf screenshot
void ofBeginSaveScreenAsPDF(string filename, bool bMultipage = false, bool b3D = false, ofRectangle viewport = ofRectangle(0,0,0,0));
void ofEndSaveScreenAsPDF();


// transformations
// push and pop all matrices and viewport
void ofPushView();
void ofPopView();

// setup matrices and viewport (upto you to push and pop view before and after)
// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
void ofViewport(ofRectangle viewport);
void ofViewport(float x = 0, float y = 0, float width = 0, float height = 0, bool invertY = true);
void ofSetupScreenPerspective(float width = 0, float height = 0, ofOrientation orientation = OF_ORIENTATION_UNKNOWN, bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0);
void ofSetupScreenOrtho(float width = 0, float height = 0, ofOrientation orientation = OF_ORIENTATION_UNKNOWN, bool vFlip = true, float nearDist = -1, float farDist = -1);
ofRectangle ofGetCurrentViewport();
int ofGetViewportWidth();
int ofGetViewportHeight();
int ofOrientationToDegrees(ofOrientation orientation);

void ofSetCoordHandedness(ofHandednessType handedness);
ofHandednessType ofGetCoordHandedness();

//our openGL wrappers
void ofPushMatrix();
void ofPopMatrix();
void ofTranslate(float x, float y, float z = 0);
void ofTranslate(const ofPoint & p);
void ofScale(float xAmnt, float yAmnt, float zAmnt = 1);
void ofRotate(float degrees, float vecX, float vecY, float vecZ);
void ofRotateX(float degrees);
void ofRotateY(float degrees);
void ofRotateZ(float degrees);
void ofRotate(float degrees);

// screen coordinate things / default gl values
void ofSetupGraphicDefaults();
void ofSetupScreen();

// end transformations
// ----------------------------------------------------





// ----------------------------------------------------
// drawing modes

ofRectMode 	ofGetRectMode();				// default is OF_RECTMODE_CORNER

void ofSetCircleResolution(int res);  		// if there 22 is a problem, you can change it here
void ofSetCurveResolution(int res);
void ofSetSphereResolution(int res);


// drawing options
void ofNoFill();
void ofFill();
ofFillFlag ofGetFill();

void ofSetLineWidth(float lineWidth);

// color options
void ofSetColor(int r, int g, int b); // 0-255
void ofSetColor(int r, int g, int b, int a); // 0-255
void ofSetColor(const ofColor & color);
void ofSetColor(const ofColor & color, int _a);
void ofSetColor(int gray); // new set a color as grayscale with one argument
void ofSetHexColor( int hexColor ); // hex, like web 0xFF0033;

// Blending
void ofEnableBlendMode(ofBlendMode blendMode);
void ofDisableBlendMode();

// point sprites
void ofEnablePointSprites();
void ofDisablePointSprites();

// transparency
void ofEnableAlphaBlending();  // this just turns on and off opengl blending, the common mode
void ofDisableAlphaBlending();

// smooth 
void ofEnableSmoothing();
void ofDisableSmoothing();

// drawing style - combines color, fill, blending and smoothing
ofStyle ofGetStyle();
void ofSetStyle(ofStyle style);
void ofPushStyle();
void ofPopStyle();

void ofSetPolyMode(ofPolyWindingMode mode);
void ofSetRectMode(ofRectMode mode);		// set the mode, either to OF_RECTMODE_CORNER or OF_RECTMODE_CENTER

// end drawing modes
//-----------------------------------------------------


//---------------------------------------------------
// background

// bg color
float * ofBgColorPtr();
void ofBackground(int r, int g, int b, int a=255);
void ofBackground(int brightness, int alpha = 255);
void ofBackground(const ofColor & c);
void ofBackgroundHex(int hexColor, int alpha = 255);
void ofBackgroundGradient(const ofColor& start, const ofColor& end, ofGradientMode mode = OF_GRADIENT_CIRCULAR);

void ofSetBackgroundColor(int r, int g, int b, int a=255);
void ofSetBackgroundColor(int brightness, int alpha = 255);
void ofSetBackgroundColor(const ofColor & c);
void ofSetBackgroundColorHex(int hexColor, int alpha = 255);

// user's access to settings (bgAuto, corner mode):
void 	ofSetBackgroundAuto(bool bManual);		// default is true

void ofClear(float r, float g, float b, float a=0);
void ofClear(float brightness, float a=0);
void ofClear(const ofColor & c);
void ofClearAlpha();

// OF's access to settings (bgAuto, origin, corner mode):
bool 	ofbClearBg();

// end background
//---------------------------------------------------







// ----------------------------------------------------
// geometry
void ofTriangle(float x1,float y1,float x2,float y2,float x3, float y3);
void ofTriangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3, float y3,float z3);
void ofTriangle(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3);

void ofCircle(float x, float y, float radius);
void ofCircle(float x, float y, float z, float radius);
void ofCircle(const ofPoint & p, float radius);

void ofEllipse(float x, float y, float width, float height);
void ofEllipse(float x, float y, float z, float width, float height);
void ofEllipse(const ofPoint & p, float width, float height);

void ofLine(float x1,float y1,float x2,float y2);
void ofLine(float x1,float y1,float z1,float x2,float y2,float z2);
void ofLine(const ofPoint & p1, const ofPoint & p2);

void ofRect(float x1,float y1,float w,float h);
void ofRect(const ofRectangle & r);
void ofRect(const ofPoint & p,float w,float h);
void ofRect(float x,float y,float z,float w,float h);

void ofCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
void ofCurve(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void ofBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
void ofBezier(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);


// for polygons
void ofBeginShape();
void ofVertex(float x, float y);
void ofVertex(float x, float y, float z);
void ofVertex(ofPoint & p);
void ofVertexes(const vector <ofPoint> & polyPoints);


void ofCurveVertex(float x, float y);
void ofCurveVertex(ofPoint & p);
void ofCurveVertexes(const vector <ofPoint> & curvePoints);

void ofBezierVertex(float x1, float y1, float x2, float y2, float x3, float y3);
void ofBezierVertex(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3);
void ofBezierVertex(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

void ofEndShape(bool bClose = false);
void ofNextContour(bool bClose = false);  // for multi contour shapes!


//3d
void ofSphere(float x, float y, float z, float radius);
void ofSphere(float x, float y, float radius);
void ofSphere(const ofPoint& position, float radius);
void ofSphere(float radius);

void ofBox(float x, float y, float z, float size);
void ofBox(float x, float y, float size);
void ofBox(const ofPoint& position, float size);
void ofBox(float size);

void ofCone(float x, float y, float z, float radius, float height);
void ofCone(float x, float y, float radius, float height);
void ofCone(const ofPoint& position, float radius, float height);
void ofCone(float radius, float height);

// bitmapped type
void ofSetDrawBitmapMode(ofDrawBitmapMode mode);
void ofDrawBitmapString(string textString, const ofPoint & p);
void ofDrawBitmapString(string textString, float x, float y);
void ofDrawBitmapString(string textString, float x, float y, float z);
void ofDrawBitmapStringHighlight(string text, const ofPoint& position, const ofColor& background = ofColor::black, const ofColor& foreground = ofColor::white);
void ofDrawBitmapStringHighlight(string text, int x, int y, const ofColor& background = ofColor::black, const ofColor& foreground = ofColor::white);


// end primitives
//-------------------------------------------
