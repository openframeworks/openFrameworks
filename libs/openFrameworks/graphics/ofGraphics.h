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
OF_INTERNAL_DEPRECATED(void ofPushView());
OF_INTERNAL_DEPRECATED(void ofPopView());

// setup matrices and viewport (upto you to push and pop view before and after)
// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
OF_INTERNAL_DEPRECATED(bool ofIsVFlipped());
OF_INTERNAL_DEPRECATED(void ofViewport(ofRectangle viewport));
OF_INTERNAL_DEPRECATED(void ofViewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip=ofIsVFlipped()));


OF_INTERNAL_DEPRECATED(void ofSetupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0));
OF_INTERNAL_DEPRECATED(void ofSetupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1));

OF_DEPRECATED_MSG("ofSetupScreenPerspective() doesn't accept orientation and vflip parameters anymore, use ofSetOrientation() to specify them",
		void ofSetupScreenPerspective(float width, float height, ofOrientation orientation, bool vFlip = ofIsVFlipped(), float fov = 60, float nearDist = 0, float farDist = 0)
);
OF_DEPRECATED_MSG("ofSetupScreenOrtho() doesn't accept orientation and vflip parameters anymore, use ofSetOrientation() to specify them",
		void ofSetupScreenOrtho(float width, float height, ofOrientation orientation, bool vFlip = ofIsVFlipped(), float nearDist = -1, float farDist = 1)
);

OF_INTERNAL_DEPRECATED(ofRectangle ofGetCurrentViewport());
OF_INTERNAL_DEPRECATED(ofRectangle ofGetNativeViewport());
OF_INTERNAL_DEPRECATED(int ofGetViewportWidth());
OF_INTERNAL_DEPRECATED(int ofGetViewportHeight());
OF_INTERNAL_DEPRECATED(int ofOrientationToDegrees(ofOrientation orientation));

OF_INTERNAL_DEPRECATED(void ofSetCoordHandedness(ofHandednessType handedness));
OF_INTERNAL_DEPRECATED(ofHandednessType ofGetCoordHandedness());

//our openGL wrappers
OF_INTERNAL_DEPRECATED(void ofPushMatrix());
OF_INTERNAL_DEPRECATED(void ofPopMatrix());
OF_INTERNAL_DEPRECATED(ofMatrix4x4 ofGetCurrentMatrix(ofMatrixMode matrixMode_));

/// \brief Query the current (oF internal) Orientation Matrix state.
/// \note  The matrix returned is the matrix openFrameworks uses internally
///        to calculate the (final, oriented) projection matrix as it is
///        passed on to the GPU.
///
///        Currently, only GL Programmable Renderer and GL Renderer
///        implement ofGetCurrentOrientationMatrix.
OF_INTERNAL_DEPRECATED(ofMatrix4x4 ofGetCurrentOrientationMatrix());

/// \brief Query the current (oF internal) Normal Matrix state.
/// \note  The matrix returned is the transposed of the inverse of the
///        view matrix
///
///        Currently, only GL Programmable Renderer and GL Renderer
///        implement ofGetCurrentNormalMatrix.
OF_INTERNAL_DEPRECATED(ofMatrix4x4 ofGetCurrentNormalMatrix());

OF_INTERNAL_DEPRECATED(void ofTranslate(float x, float y, float z = 0));
OF_INTERNAL_DEPRECATED(void ofTranslate(const ofPoint & p));
OF_INTERNAL_DEPRECATED(void ofScale(float xAmnt, float yAmnt, float zAmnt = 1));
OF_INTERNAL_DEPRECATED(void ofRotate(float degrees, float vecX, float vecY, float vecZ));
OF_INTERNAL_DEPRECATED(void ofRotateX(float degrees));
OF_INTERNAL_DEPRECATED(void ofRotateY(float degrees));
OF_INTERNAL_DEPRECATED(void ofRotateZ(float degrees));
OF_INTERNAL_DEPRECATED(void ofRotate(float degrees));
OF_INTERNAL_DEPRECATED(void ofLoadIdentityMatrix (void));
OF_INTERNAL_DEPRECATED(void ofLoadMatrix (const ofMatrix4x4 & m));
OF_INTERNAL_DEPRECATED(void ofLoadMatrix (const float *m));
OF_INTERNAL_DEPRECATED(void ofMultMatrix (const ofMatrix4x4 & m));
OF_INTERNAL_DEPRECATED(void ofMultMatrix (const float *m));
OF_INTERNAL_DEPRECATED(void ofSetMatrixMode(ofMatrixMode matrixMode));
OF_INTERNAL_DEPRECATED(void ofLoadViewMatrix(const ofMatrix4x4 & m));
OF_INTERNAL_DEPRECATED(void ofMultViewMatrix(const ofMatrix4x4 & m));
OF_INTERNAL_DEPRECATED(ofMatrix4x4 ofGetCurrentViewMatrix());

// screen coordinate things / default gl values
OF_INTERNAL_DEPRECATED(void ofSetupGraphicDefaults());
OF_INTERNAL_DEPRECATED(void ofSetupScreen());

// end transformations
// ----------------------------------------------------





// ----------------------------------------------------
// drawing modes

OF_INTERNAL_DEPRECATED(ofRectMode 	ofGetRectMode());				// default is OF_RECTMODE_CORNER

OF_INTERNAL_DEPRECATED(void ofSetCircleResolution(int res));  		// if there 22 is a problem, you can change it here
OF_INTERNAL_DEPRECATED(void ofSetCurveResolution(int res));


// drawing options
OF_INTERNAL_DEPRECATED(void ofNoFill());
OF_INTERNAL_DEPRECATED(void ofFill());
OF_INTERNAL_DEPRECATED(ofFillFlag ofGetFill());

OF_INTERNAL_DEPRECATED(void ofSetLineWidth(float lineWidth));
OF_INTERNAL_DEPRECATED(void ofSetDepthTest(bool depthTest));
OF_INTERNAL_DEPRECATED(void ofEnableDepthTest());
OF_INTERNAL_DEPRECATED(void ofDisableDepthTest());

// color options
OF_INTERNAL_DEPRECATED(void ofSetColor(int r, int g, int b)); // 0-255
OF_INTERNAL_DEPRECATED(void ofSetColor(int r, int g, int b, int a)); // 0-255
OF_INTERNAL_DEPRECATED(void ofSetColor(const ofColor & color));
OF_INTERNAL_DEPRECATED(void ofSetColor(const ofColor & color, int _a));
OF_INTERNAL_DEPRECATED(void ofSetColor(int gray)); // new set a color as grayscale with one argument
OF_INTERNAL_DEPRECATED(void ofSetHexColor( int hexColor )); // hex, like web 0xFF0033;

// Blending
OF_INTERNAL_DEPRECATED(void ofEnableBlendMode(ofBlendMode blendMode));
OF_INTERNAL_DEPRECATED(void ofDisableBlendMode());

// point sprites
OF_INTERNAL_DEPRECATED(void ofEnablePointSprites());
OF_INTERNAL_DEPRECATED(void ofDisablePointSprites());

// transparency
OF_INTERNAL_DEPRECATED(void ofEnableAlphaBlending());  // this just turns on and off opengl blending, the common mode
OF_INTERNAL_DEPRECATED(void ofDisableAlphaBlending());

// smooth 
OF_INTERNAL_DEPRECATED(void ofEnableSmoothing());
OF_INTERNAL_DEPRECATED(void ofDisableSmoothing());

// antialiasing
OF_INTERNAL_DEPRECATED(void ofEnableAntiAliasing());
OF_INTERNAL_DEPRECATED(void ofDisableAntiAliasing());

// drawing style - combines color, fill, blending and smoothing
OF_INTERNAL_DEPRECATED(ofStyle ofGetStyle());
OF_INTERNAL_DEPRECATED(void ofSetStyle(ofStyle style));
OF_INTERNAL_DEPRECATED(void ofPushStyle());
OF_INTERNAL_DEPRECATED(void ofPopStyle());

OF_INTERNAL_DEPRECATED(void ofSetPolyMode(ofPolyWindingMode mode));
OF_INTERNAL_DEPRECATED(void ofSetRectMode(ofRectMode mode));		// set the mode, either to OF_RECTMODE_CORNER or OF_RECTMODE_CENTER

// end drawing modes
//-----------------------------------------------------


//---------------------------------------------------
// background

// bg color
OF_DEPRECATED_MSG("Use ofGetBackgroundColor instead",ofColor ofGetBackground());
OF_INTERNAL_DEPRECATED(ofColor ofGetBackgroundColor());
OF_INTERNAL_DEPRECATED(void ofBackground(int r, int g, int b, int a=255));
OF_INTERNAL_DEPRECATED(void ofBackground(int brightness, int alpha = 255));
OF_INTERNAL_DEPRECATED(void ofBackground(const ofColor & c));
OF_INTERNAL_DEPRECATED(void ofBackgroundHex(int hexColor, int alpha = 255));
OF_INTERNAL_DEPRECATED(void ofBackgroundGradient(const ofColor& start, const ofColor& end, ofGradientMode mode = OF_GRADIENT_CIRCULAR));

OF_INTERNAL_DEPRECATED(void ofSetBackgroundColor(int r, int g, int b, int a=255));
OF_INTERNAL_DEPRECATED(void ofSetBackgroundColor(int brightness, int alpha = 255));
OF_INTERNAL_DEPRECATED(void ofSetBackgroundColor(const ofColor & c));
OF_INTERNAL_DEPRECATED(void ofSetBackgroundColorHex(int hexColor, int alpha = 255));

// user's access to settings (bgAuto, corner mode):
OF_INTERNAL_DEPRECATED(void ofSetBackgroundAuto(bool bManual));		// default is true
OF_INTERNAL_DEPRECATED(bool ofGetBackgroundAuto());

OF_INTERNAL_DEPRECATED(void ofClear(float r, float g, float b, float a=0));
OF_INTERNAL_DEPRECATED(void ofClear(float brightness, float a=0));
OF_INTERNAL_DEPRECATED(void ofClear(const ofColor & c));
OF_INTERNAL_DEPRECATED(void ofClearAlpha());

// OF's access to settings (bgAuto, origin, corner mode):
OF_DEPRECATED_MSG("Use ofGetBackgroundAuto instead",bool ofbClearBg());

// end background
//---------------------------------------------------







// ----------------------------------------------------
// geometry
OF_INTERNAL_DEPRECATED(void ofDrawTriangle(float x1,float y1,float x2,float y2,float x3, float y3));
OF_INTERNAL_DEPRECATED(void ofDrawTriangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3, float y3,float z3));
OF_INTERNAL_DEPRECATED(void ofDrawTriangle(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3));

OF_INTERNAL_DEPRECATED(void ofDrawCircle(float x, float y, float radius));
OF_INTERNAL_DEPRECATED(void ofDrawCircle(float x, float y, float z, float radius));
OF_INTERNAL_DEPRECATED(void ofDrawCircle(const ofPoint & p, float radius));

OF_INTERNAL_DEPRECATED(void ofDrawEllipse(float x, float y, float width, float height));
OF_INTERNAL_DEPRECATED(void ofDrawEllipse(float x, float y, float z, float width, float height));
OF_INTERNAL_DEPRECATED(void ofDrawEllipse(const ofPoint & p, float width, float height));

OF_INTERNAL_DEPRECATED(void ofDrawLine(float x1,float y1,float x2,float y2));
OF_INTERNAL_DEPRECATED(void ofDrawLine(float x1,float y1,float z1,float x2,float y2,float z2));
OF_INTERNAL_DEPRECATED(void ofDrawLine(const ofPoint & p1, const ofPoint & p2));

OF_INTERNAL_DEPRECATED(void ofDrawRectangle(float x1,float y1,float w,float h));
OF_INTERNAL_DEPRECATED(void ofDrawRectangle(const ofRectangle & r));
OF_INTERNAL_DEPRECATED(void ofDrawRectangle(const ofPoint & p,float w,float h));
OF_INTERNAL_DEPRECATED(void ofDrawRectangle(float x,float y,float z,float w,float h));

OF_INTERNAL_DEPRECATED(void ofDrawRectRounded(const ofRectangle & b, float r));
OF_INTERNAL_DEPRECATED(void ofDrawRectRounded(const ofPoint & p, float w, float h, float r));
OF_INTERNAL_DEPRECATED(void ofDrawRectRounded(float x, float y, float w, float h, float r));
OF_INTERNAL_DEPRECATED(void ofDrawRectRounded(float x, float y, float z, float w, float h, float r));

//----------------------------------------------------------
OF_INTERNAL_DEPRECATED(void ofDrawRectRounded(const ofPoint & p, float w, float h, float topLeftRadius,
                                                        float topRightRadius,
                                                        float bottomRightRadius,
                                                        float bottomLeftRadius));

OF_INTERNAL_DEPRECATED(void ofDrawRectRounded(const ofRectangle & b, float topLeftRadius,
                                          float topRightRadius,
                                          float bottomRightRadius,
                                          float bottomLeftRadius));

OF_INTERNAL_DEPRECATED(void ofDrawRectRounded(float x, float y, float z, float w, float h, float topLeftRadius,
                                                                float topRightRadius,
                                                                float bottomRightRadius,
                                                                float bottomLeftRadius));

OF_INTERNAL_DEPRECATED(void ofDrawCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3));
OF_INTERNAL_DEPRECATED(void ofDrawCurve(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3));
OF_INTERNAL_DEPRECATED(void ofDrawBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3));
OF_INTERNAL_DEPRECATED(void ofDrawBezier(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3));

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
OF_INTERNAL_DEPRECATED(void ofBeginShape());
OF_INTERNAL_DEPRECATED(void ofVertex(float x, float y));
OF_INTERNAL_DEPRECATED(void ofVertex(float x, float y, float z));
OF_INTERNAL_DEPRECATED(void ofVertex(ofPoint & p));
OF_INTERNAL_DEPRECATED(void ofVertices(const vector <ofPoint> & polyPoints));

OF_INTERNAL_DEPRECATED(void ofCurveVertex(float x, float y));
OF_INTERNAL_DEPRECATED(void ofCurveVertex(float x, float y, float z));
OF_INTERNAL_DEPRECATED(void ofCurveVertex(ofPoint & p));
OF_INTERNAL_DEPRECATED(void ofCurveVertices(const vector <ofPoint> & curvePoints));

OF_INTERNAL_DEPRECATED(void ofBezierVertex(float x1, float y1, float x2, float y2, float x3, float y3));
OF_INTERNAL_DEPRECATED(void ofBezierVertex(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3));
OF_INTERNAL_DEPRECATED(void ofBezierVertex(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3));

OF_INTERNAL_DEPRECATED(void ofEndShape(bool bClose = false));
OF_INTERNAL_DEPRECATED(void ofNextContour(bool bClose = false));  // for multi contour shapes!


// bitmapped type
OF_INTERNAL_DEPRECATED(void ofSetDrawBitmapMode(ofDrawBitmapMode mode));
template<typename T>
OF_INTERNAL_DEPRECATED(void ofDrawBitmapString(const T & textString, const ofPoint & p));
template<typename T>
OF_INTERNAL_DEPRECATED(void ofDrawBitmapString(const T & textString, float x, float y));
template<typename T>
OF_INTERNAL_DEPRECATED(void ofDrawBitmapString(const T & textString, float x, float y, float z));
template<>
OF_INTERNAL_DEPRECATED(void ofDrawBitmapString(const string & textString, float x, float y, float z));
OF_INTERNAL_DEPRECATED(void ofDrawBitmapStringHighlight(string text, const ofPoint& position, const ofColor& background = ofColor::black, const ofColor& foreground = ofColor::white));
OF_INTERNAL_DEPRECATED(void ofDrawBitmapStringHighlight(string text, int x, int y, const ofColor& background = ofColor::black, const ofColor& foreground = ofColor::white));


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
