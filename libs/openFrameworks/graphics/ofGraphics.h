#pragma once

#include "ofConstants.h"
#include "ofColor.h"
#include "ofPoint.h"
#include "ofMatrix4x4.h"
#include "ofRectangle.h"
#include "ofTypes.h"
#include "ofBaseTypes.h"

#define  	CIRC_RESOLUTION		    22				// 22 pts for a circle...


void ofSetCurrentRenderer(ofPtr<ofBaseRenderer> renderer,bool setDefaults=false);
void ofSetCurrentRenderer(const string & rendererType,bool setDefaults=false);
ofPtr<ofBaseRenderer> & ofGetCurrentRenderer();

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
void ofViewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip=ofIsVFlipped());

bool ofIsVFlipped();

void ofSetupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0);
void ofSetupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1);

OF_DEPRECATED_MSG("ofSetupScreenPerspective() doesn't accept orientation and vflip parameters anymore, use ofSetOrientation() to specify them",
		void ofSetupScreenPerspective(float width, float height, ofOrientation orientation, bool vFlip = ofIsVFlipped(), float fov = 60, float nearDist = 0, float farDist = 0)
);
OF_DEPRECATED_MSG("ofSetupScreenOrtho() doesn't accept orientation and vflip parameters anymore, use ofSetOrientation() to specify them",
		void ofSetupScreenOrtho(float width, float height, ofOrientation orientation, bool vFlip = ofIsVFlipped(), float nearDist = -1, float farDist = 1)
);

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
///        implement getCurrentMatrix.
ofMatrix4x4 ofGetCurrentOrientationMatrix();
void ofTranslate(float x, float y, float z = 0);
void ofTranslate(const ofPoint & p);
void ofScale(float xAmnt, float yAmnt, float zAmnt = 1);
void ofRotate(float degrees, float vecX, float vecY, float vecZ);
void ofRotateX(float degrees);
void ofRotateY(float degrees);
void ofRotateZ(float degrees);
void ofRotate(float degrees);
void ofLoadIdentityMatrix (void);
void ofLoadMatrix (const ofMatrix4x4 & m);   // Andreas: I've included both a ofMatrix4x4* and a float* version here,
void ofLoadMatrix (const float *m);			// ideally we would always use ofMatrix4x4, but in a lot of temporary
void ofMultMatrix (const ofMatrix4x4 & m);	// ofMatrix4x4 objects when interacting with non-OF code
void ofMultMatrix (const float *m);
void ofSetMatrixMode(ofMatrixMode matrixMode);

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
float * ofBgColorPtr();
ofColor ofGetBackground();
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

void ofRectRounded(const ofRectangle & b, float r);
void ofRectRounded(const ofPoint & p, float w, float h, float r);
void ofRectRounded(float x, float y, float w, float h, float r);
void ofRectRounded(float x, float y, float z, float w, float h, float r);

//----------------------------------------------------------
void ofRectRounded(const ofPoint & p, float w, float h, float topLeftRadius,
                                                        float topRightRadius,
                                                        float bottomRightRadius,
                                                        float bottomLeftRadius);

void ofRectRounded(const ofRectangle & b, float topLeftRadius,
                                          float topRightRadius,
                                          float bottomRightRadius,
                                          float bottomLeftRadius);

void ofRectRounded(float x, float y, float z, float w, float h, float topLeftRadius,
                                                                float topRightRadius,
                                                                float bottomRightRadius,
                                                                float bottomLeftRadius);

void ofCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
void ofCurve(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void ofBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
void ofBezier(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);


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
void ofDrawBitmapString(string textString, const ofPoint & p);
void ofDrawBitmapString(string textString, float x, float y);
void ofDrawBitmapString(string textString, float x, float y, float z);
void ofDrawBitmapStringHighlight(string text, const ofPoint& position, const ofColor& background = ofColor::black, const ofColor& foreground = ofColor::white);
void ofDrawBitmapStringHighlight(string text, int x, int y, const ofColor& background = ofColor::black, const ofColor& foreground = ofColor::white);


// end primitives
//-------------------------------------------
