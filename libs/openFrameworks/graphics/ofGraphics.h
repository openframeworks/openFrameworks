#pragma once

#include "ofConstants.h"
#include "ofColor.h"
#include "ofPoint.h"
#include "ofMatrix4x4.h"
#include "ofRectangle.h"
#include "ofTypes.h"
#define  	CIRC_RESOLUTION		    22				// 22 pts for a circle...

//for pdf screenshot
void ofBeginSaveScreenAsPDF(string filename, bool bMultipage = false, bool b3D = false, ofRectangle viewport = ofRectangle(0,0,0,0));
void ofEndSaveScreenAsPDF();
void ofBeginSaveScreenAsSVG(string filename, bool bMultipage = false, bool b3D = false, ofRectangle viewport = ofRectangle(0,0,0,0));
void ofEndSaveScreenAsSVG();


// transformations
// push and pop all matrices and viewport
void ofPushView();
void ofPopView();

// setup matrices and viewport (upto you to push and pop view before and after)
// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
bool ofIsVFlipped();
void ofViewport(ofRectangle viewport);
void ofViewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip=ofIsVFlipped());


void ofSetupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0);
void ofSetupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1);

ofRectangle ofGetCurrentViewport();
ofRectangle ofGetNativeViewport();
int ofGetViewportWidth();
int ofGetViewportHeight();
int ofOrientationToDegrees(ofOrientation orientation);

void ofSetCoordHandedness(ofHandednessType handedness);
ofHandednessType ofGetCoordHandedness();

//our openGL wrappers
void ofPushMatrix();
void ofPopMatrix();
ofMatrix4x4 ofGetCurrentMatrix(ofMatrixMode matrixMode_);

/// \brief Query the current (oF internal) Orientation Matrix state.
/// \note  The matrix returned is the matrix openFrameworks uses internally
///        to calculate the (final, oriented) projection matrix as it is
///        passed on to the GPU.
///
///        Currently, only GL Programmable Renderer and GL Renderer
///        implement ofGetCurrentOrientationMatrix.
ofMatrix4x4 ofGetCurrentOrientationMatrix();

/// \brief Query the current (oF internal) Normal Matrix state.
/// \note  The matrix returned is the transposed of the inverse of the
///        view matrix
///
///        Currently, only GL Programmable Renderer and GL Renderer
///        implement ofGetCurrentNormalMatrix.
ofMatrix4x4 ofGetCurrentNormalMatrix();

void ofTranslate(float x, float y, float z = 0);
void ofTranslate(const ofPoint & p);
void ofScale(float xAmnt, float yAmnt, float zAmnt = 1);
void ofRotate(float degrees, float vecX, float vecY, float vecZ);
void ofRotateX(float degrees);
void ofRotateY(float degrees);
void ofRotateZ(float degrees);
void ofRotate(float degrees);
void ofLoadIdentityMatrix (void);
void ofLoadMatrix (const ofMatrix4x4 & m);
void ofLoadMatrix (const float *m);
void ofMultMatrix (const ofMatrix4x4 & m);
void ofMultMatrix (const float *m);
void ofSetMatrixMode(ofMatrixMode matrixMode);
void ofLoadViewMatrix(const ofMatrix4x4 & m);
void ofMultViewMatrix(const ofMatrix4x4 & m);
ofMatrix4x4 ofGetCurrentViewMatrix();

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


// drawing options
void ofNoFill();
void ofFill();
ofFillFlag ofGetFill();

void ofSetLineWidth(float lineWidth);
void ofSetDepthTest(bool depthTest);
void ofEnableDepthTest();
void ofDisableDepthTest();

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

// antialiasing
void ofEnableAntiAliasing();
void ofDisableAntiAliasing();

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
OF_DEPRECATED_MSG("Use ofGetBackgroundColor instead",ofColor ofGetBackground());
ofColor ofGetBackgroundColor();
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
void ofSetBackgroundAuto(bool bManual);		// default is true
bool ofGetBackgroundAuto();

void ofClear(float r, float g, float b, float a=0);
void ofClear(float brightness, float a=0);
void ofClear(const ofColor & c);
void ofClearAlpha();

// OF's access to settings (bgAuto, origin, corner mode):
OF_DEPRECATED_MSG("Use ofGetBackgroundAuto instead",bool ofbClearBg());

// end background
//---------------------------------------------------







// ----------------------------------------------------
// geometry
void ofDrawTriangle(float x1,float y1,float x2,float y2,float x3, float y3);
void ofDrawTriangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3, float y3,float z3);
void ofDrawTriangle(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3);

void ofDrawCircle(float x, float y, float radius);
void ofDrawCircle(float x, float y, float z, float radius);
void ofDrawCircle(const ofPoint & p, float radius);

void ofDrawEllipse(float x, float y, float width, float height);
void ofDrawEllipse(float x, float y, float z, float width, float height);
void ofDrawEllipse(const ofPoint & p, float width, float height);

void ofDrawLine(float x1,float y1,float x2,float y2);
void ofDrawLine(float x1,float y1,float z1,float x2,float y2,float z2);
void ofDrawLine(const ofPoint & p1, const ofPoint & p2);

void ofDrawRectangle(float x1,float y1,float w,float h);
void ofDrawRectangle(const ofRectangle & r);
void ofDrawRectangle(const ofPoint & p,float w,float h);
void ofDrawRectangle(float x,float y,float z,float w,float h);

void ofDrawRectRounded(const ofRectangle & b, float r);
void ofDrawRectRounded(const ofPoint & p, float w, float h, float r);
void ofDrawRectRounded(float x, float y, float w, float h, float r);
void ofDrawRectRounded(float x, float y, float z, float w, float h, float r);

//----------------------------------------------------------
void ofDrawRectRounded(const ofPoint & p, float w, float h, float topLeftRadius,
                                                        float topRightRadius,
                                                        float bottomRightRadius,
                                                        float bottomLeftRadius);

void ofDrawRectRounded(const ofRectangle & b, float topLeftRadius,
                                          float topRightRadius,
                                          float bottomRightRadius,
                                          float bottomLeftRadius);

void ofDrawRectRounded(float x, float y, float z, float w, float h, float topLeftRadius,
                                                                float topRightRadius,
                                                                float bottomRightRadius,
                                                                float bottomLeftRadius);

void ofDrawCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
void ofDrawCurve(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void ofDrawBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
void ofDrawBezier(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

OF_DEPRECATED_MSG("Use ofDrawTriangle instead",void ofTriangle(float x1,float y1,float x2,float y2,float x3, float y3));
OF_DEPRECATED_MSG("Use ofDrawTriangle instead",void ofTriangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3, float y3,float z3));
OF_DEPRECATED_MSG("Use ofDrawTriangle instead",void ofTriangle(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3));

OF_DEPRECATED_MSG("Use ofDrawCircle instead",void ofCircle(float x, float y, float radius));
OF_DEPRECATED_MSG("Use ofDrawCircle instead",void ofCircle(float x, float y, float z, float radius));
OF_DEPRECATED_MSG("Use ofDrawCircle instead",void ofCircle(const ofPoint & p, float radius));

OF_DEPRECATED_MSG("Use ofDrawEllipse instead",void ofEllipse(float x, float y, float width, float height));
OF_DEPRECATED_MSG("Use ofDrawEllipse instead",void ofEllipse(float x, float y, float z, float width, float height));
OF_DEPRECATED_MSG("Use ofDrawEllipse instead",void ofEllipse(const ofPoint & p, float width, float height));

OF_DEPRECATED_MSG("Use ofDrawLine instead",void ofLine(float x1,float y1,float x2,float y2));
OF_DEPRECATED_MSG("Use ofDrawLine instead",void ofLine(float x1,float y1,float z1,float x2,float y2,float z2));
OF_DEPRECATED_MSG("Use ofDrawLine instead",void ofLine(const ofPoint & p1, const ofPoint & p2));

OF_DEPRECATED_MSG("Use ofDrawRectangle instead",void ofRect(float x1,float y1,float w,float h));
OF_DEPRECATED_MSG("Use ofDrawRectangle instead",void ofRect(const ofRectangle & r));
OF_DEPRECATED_MSG("Use ofDrawRectangle instead",void ofRect(const ofPoint & p,float w,float h));
OF_DEPRECATED_MSG("Use ofDrawRectangle instead",void ofRect(float x,float y,float z,float w,float h));

OF_DEPRECATED_MSG("Use ofDrawRectRounded instead",void ofRectRounded(const ofRectangle & b, float r));
OF_DEPRECATED_MSG("Use ofDrawRectRounded instead",void ofRectRounded(const ofPoint & p, float w, float h, float r));
OF_DEPRECATED_MSG("Use ofDrawRectRounded instead",void ofRectRounded(float x, float y, float w, float h, float r));
OF_DEPRECATED_MSG("Use ofDrawRectRounded instead",void ofRectRounded(float x, float y, float z, float w, float h, float r));

//----------------------------------------------------------
OF_DEPRECATED_MSG("Use ofDrawRectRounded instead",void ofRectRounded(const ofPoint & p, float w, float h, float topLeftRadius,
                                                        float topRightRadius,
                                                        float bottomRightRadius,
                                                        float bottomLeftRadius));

OF_DEPRECATED_MSG("Use ofDrawRectRounded instead",void ofRectRounded(const ofRectangle & b, float topLeftRadius,
                                          float topRightRadius,
                                          float bottomRightRadius,
                                          float bottomLeftRadius));

OF_DEPRECATED_MSG("Use ofDrawRectRounded instead",void ofRectRounded(float x, float y, float z, float w, float h, float topLeftRadius,
                                                                float topRightRadius,
                                                                float bottomRightRadius,
                                                                float bottomLeftRadius));

OF_DEPRECATED_MSG("Use ofDrawCurve instead",void ofCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3));
OF_DEPRECATED_MSG("Use ofDrawCurve instead",void ofCurve(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3));
OF_DEPRECATED_MSG("Use ofDrawBezier instead",void ofBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3));
OF_DEPRECATED_MSG("Use ofDrawBezier instead",void ofBezier(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3));



// for polygons
void ofBeginShape();
void ofVertex(float x, float y);
void ofVertex(float x, float y, float z);
void ofVertex(ofPoint & p);
void ofVertices(const vector <ofPoint> & polyPoints);

void ofCurveVertex(float x, float y);
void ofCurveVertex(float x, float y, float z);
void ofCurveVertex(ofPoint & p);
void ofCurveVertices(const vector <ofPoint> & curvePoints);

void ofBezierVertex(float x1, float y1, float x2, float y2, float x3, float y3);
void ofBezierVertex(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3);
void ofBezierVertex(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

void ofEndShape(bool bClose = false);
void ofNextContour(bool bClose = false);  // for multi contour shapes!


// bitmapped type
void ofSetDrawBitmapMode(ofDrawBitmapMode mode);
template<typename T>
void ofDrawBitmapString(const T & textString, const ofPoint & p);
template<typename T>
void ofDrawBitmapString(const T & textString, float x, float y);
template<typename T>
void ofDrawBitmapString(const T & textString, float x, float y, float z);
template<>
void ofDrawBitmapString(const string & textString, float x, float y, float z);
void ofDrawBitmapStringHighlight(string text, const ofPoint& position, const ofColor& background = ofColor::black, const ofColor& foreground = ofColor::white);
void ofDrawBitmapStringHighlight(string text, int x, int y, const ofColor& background = ofColor::black, const ofColor& foreground = ofColor::white);


// end primitives
//-------------------------------------------



//--------------------------------------------------
template<typename T>
void ofDrawBitmapString(const T & textString, const ofPoint & p){
	ofDrawBitmapString(textString, p.x, p.y, p.z);
}

//--------------------------------------------------
template<typename T>
void ofDrawBitmapString(const T & textString, float x, float y){
	ofDrawBitmapString(textString, x, y, 0.0f);
}

//--------------------------------------------------
template<typename T>
void ofDrawBitmapString(const T & textString, float x, float y, float z){
	ofDrawBitmapString(ofToString(textString),x,y,z);
}
