#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofUtils.h"
#include "ofBaseTypes.h"
#include "ofGLRenderer.h"
#include "ofPath.h"

#ifdef TARGET_OSX
	#include <OpenGL/glu.h>
#endif

//#ifdef TARGET_OPENGLES
//	#include "glu.h"
//#endif
//
#ifdef TARGET_LINUX
	#include <GL/glu.h>
#endif

#ifdef TARGET_WIN32
	#include "glu.h"
#endif

#ifndef TARGET_WIN32
    #define CALLBACK
#endif

#ifdef TARGET_WIN32
	#define GLUT_BUILDING_LIB
	#include "glut.h"
#endif
#ifdef TARGET_OSX
	#include <GLUT/glut.h>
#endif
#ifdef TARGET_LINUX
	#include <GL/glut.h>
#endif


//----------------------------------------------------------
// static
static bool		bUsingArbTex		= true;
static bool		bUsingNormalizedTexCoords = false;

//style stuff - new in 006
static ofStyle currentStyle;
static deque <ofStyle> styleHistory;
static deque <ofRectangle> viewportHistory;

static ofPath shape;
static ofMesh vertexData;
static ofPtr<ofBaseRenderer> renderer;

void ofSetDefaultRenderer(ofPtr<ofBaseRenderer> renderer_){
	renderer = renderer_;
	renderer->setupGraphicDefaults();

	if(renderer->rendersPathPrimitives()){
		shape.setMode(ofPath::PATHS);
	}else{
		shape.setMode(ofPath::POLYLINES);
	}

	shape.setUseShapeColor(false);

	ofSetStyle(currentStyle);
}

ofPtr<ofBaseRenderer> & ofGetDefaultRenderer(){
	return renderer;
}


// opengl specifics

bool ofGetUsingNormalizedTexCoords() {
	return bUsingNormalizedTexCoords;
}

void ofEnableNormalizedTexCoords() {
	bUsingNormalizedTexCoords = true;
}

void ofDisableNormalizedTexCoords() {
	bUsingNormalizedTexCoords = false;
}



//***** add global functions to override texture settings
//----------------------------------------------------------
static bool bUseCustomTextureWrap = false;

//----------------------------------------------------------
void ofSetTextureWrap(GLfloat wrapS, GLfloat wrapT) {
	bUseCustomTextureWrap = true;
	GLenum textureTarget = GL_TEXTURE_2D;
#ifndef TARGET_OPENGLES
	if (ofGetUsingArbTex() && GL_ARB_texture_rectangle){
		textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	};
#endif
	glTexParameterf(textureTarget, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameterf(textureTarget, GL_TEXTURE_WRAP_T, wrapT);
}

//----------------------------------------------------------
bool ofGetUsingCustomTextureWrap() {
	return bUseCustomTextureWrap;
}

//----------------------------------------------------------
void ofRestoreTextureWrap() {
	bUseCustomTextureWrap = false;
}



static bool bUseCustomMinMagFilters = false;
//----------------------------------------------------------
void ofSetMinMagFilters(GLfloat minFilter, GLfloat maxFilter) {
	bUseCustomMinMagFilters = true;
	GLenum textureTarget = GL_TEXTURE_2D;
#ifndef TARGET_OPENGLES
	if (ofGetUsingArbTex() && GL_ARB_texture_rectangle){
		textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	};
#endif
	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, maxFilter);
}

//----------------------------------------------------------
bool ofGetUsingCustomMinMagFilters() {
	return bUseCustomMinMagFilters;
}

//----------------------------------------------------------
void ofRestoreMinMagFilters() {
	bUseCustomMinMagFilters = false;
}

//***** global functions to override texture settings


//----------------------------------------------------------
bool ofGetUsingArbTex(){
	return bUsingArbTex;
}

//----------------------------------------------------------
void ofEnableArbTex(){
	bUsingArbTex = true;
}

//----------------------------------------------------------
void ofDisableArbTex(){
	bUsingArbTex = false;
}

//end opengl specifics









//----------------------------------------------------------
// transformation matrix related functions

//----------------------------------------------------------
void ofPushView() {
	renderer->pushView();
}

//----------------------------------------------------------
void ofPopView() {
	renderer->popView();
}

//----------------------------------------------------------
void ofViewport(ofRectangle viewport){
	renderer->viewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

//----------------------------------------------------------
void ofViewport(float x, float y, float width, float height, bool invertY) {
	renderer->viewport(x,y,width,height,invertY);
}

//----------------------------------------------------------
ofRectangle ofGetCurrentViewport(){
	return renderer->getCurrentViewport();
}

//----------------------------------------------------------
int ofGetViewportWidth(){
	return renderer->getViewportWidth();
}

//----------------------------------------------------------
int ofGetViewportHeight(){
	return renderer->getViewportHeight();
}

//----------------------------------------------------------
void ofSetCoordHandedness(ofHandednessType handedness) {
	renderer->setCoordHandedness(handedness);
}

//----------------------------------------------------------
ofHandednessType ofGetCoordHandedness() {
	return renderer->getCoordHandedness();
}

//----------------------------------------------------------
void ofSetupScreenPerspective(float width, float height, ofOrientation orientation, bool vFlip, float fov, float nearDist, float farDist) {
	renderer->setupScreenPerspective(width,height, orientation, vFlip,fov,nearDist,farDist);
}

//----------------------------------------------------------
void ofSetupScreenOrtho(float width, float height, ofOrientation orientation, bool vFlip, float nearDist, float farDist) {
	renderer->setupScreenOrtho(width,height,orientation,vFlip,nearDist,farDist);
}

//----------------------------------------------------------
//Resets openGL parameters back to OF defaults
void ofSetupGraphicDefaults(){
	renderer->setupGraphicDefaults();
	ofSetStyle(ofStyle());
}

//----------------------------------------------------------
void ofSetupScreen(){
	renderer->setupScreen();	// assume defaults
}



//our openGL wrappers
//----------------------------------------------------------
void ofPushMatrix(){
	renderer->pushMatrix();
}

//----------------------------------------------------------
void ofPopMatrix(){
	renderer->popMatrix();
}

//----------------------------------------------------------
void ofTranslate(const ofPoint& p){
	renderer->translate(p);
}


//----------------------------------------------------------
void ofTranslate(float x, float y, float z){
	renderer->translate(x, y, z);
}

//----------------------------------------------------------
void ofScale(float xAmnt, float yAmnt, float zAmnt){
	renderer->scale(xAmnt, yAmnt, zAmnt);
}

//----------------------------------------------------------
void ofRotate(float degrees, float vecX, float vecY, float vecZ){
	renderer->rotate(degrees, vecX, vecY, vecZ);
}

//----------------------------------------------------------
void ofRotateX(float degrees){
	renderer->rotateX(degrees);
}

//----------------------------------------------------------
void ofRotateY(float degrees){
	renderer->rotateY(degrees);
}

//----------------------------------------------------------
void ofRotateZ(float degrees){
	renderer->rotateZ(degrees);
}

//same as ofRotateZ
//----------------------------------------------------------
void ofRotate(float degrees){
	renderer->rotate(degrees);
}

// end transformation matrix related functions
//----------------------------------------------------------


//----------------------------------------------------------
// background functions

//----------------------------------------------------------
void ofClear(float r, float g, float b, float a) {
	renderer->clear(r,g,b,a);
}

//----------------------------------------------------------
void ofClear(float brightness, float a) {
	renderer->clear(brightness, brightness, brightness, a);
}

//----------------------------------------------------------
void ofClearAlpha() {
	renderer->clearAlpha();
}	

//----------------------------------------------------------
void ofSetBackgroundAuto(bool bAuto){
	renderer->setBackgroundAuto(bAuto);
}

//----------------------------------------------------------
bool ofbClearBg(){
	return renderer->bClearBg();
}

//----------------------------------------------------------
float * ofBgColorPtr(){
	return &renderer->getBgColor().r;
}

//----------------------------------------------------------
void ofBackground(int brightness, int alpha) {
	ofBackground(brightness, brightness, brightness, alpha);
}

//----------------------------------------------------------
void ofBackground(const ofColor & c){
	ofBackground ( c.r, c.g, c.b, c.a);
}

//----------------------------------------------------------
void ofBackgroundHex(int hexColor, int alpha){
	ofBackground ( (hexColor >> 16) & 0xff, (hexColor >> 8) & 0xff, (hexColor >> 0) & 0xff, alpha);
}

//----------------------------------------------------------
void ofBackground(int r, int g, int b, int a){
	currentStyle.bgColor.set(r,g,b,a);
	renderer->background(r,g,b,a);
}

// end background functions
//----------------------------------------------------------




//---------------------------------------------------------------------------
// drawing modes

//----------------------------------------------------------
void  ofSetRectMode(ofRectMode mode){
	renderer->setRectMode(mode);
	currentStyle.rectMode = mode;
}

//----------------------------------------------------------
ofRectMode ofGetRectMode(){
	return 	renderer->getRectMode();
}

//----------------------------------------------------------
void ofNoFill(){
	shape.setFilled(false);
	shape.setStrokeWidth(currentStyle.lineWidth);
	renderer->setFillMode(OF_OUTLINE);
	currentStyle.bFill = false;
}

//----------------------------------------------------------
void ofFill(){
	shape.setFilled(true);
	shape.setStrokeWidth(0);
	renderer->setFillMode(OF_FILLED);
	currentStyle.bFill = true;
}

// Returns OF_FILLED or OF_OUTLINE
//----------------------------------------------------------
ofFillFlag ofGetFill(){
	return renderer->getFillMode();
}

//----------------------------------------------------------
void ofSetLineWidth(float lineWidth){
	shape.setStrokeWidth(lineWidth);
	renderer->setLineWidth(lineWidth);
	currentStyle.lineWidth = lineWidth;
}

//----------------------------------------
void ofSetCurveResolution(int res){
	shape.setCurveResolution(res);
	currentStyle.curveResolution = res;
}

//----------------------------------------
void ofSetSphereResolution(int res) {
	currentStyle.sphereResolution = res;
}

//----------------------------------------------------------
void ofSetCircleResolution(int res){
	currentStyle.circleResolution = res;
	renderer->setCircleResolution(res);
}

//----------------------------------------------------------
void ofSetColor(const ofColor & color){
	ofSetColor(color.r,color.g,color.b,color.a);
}

//----------------------------------------------------------
void ofSetColor(const ofColor & color, int _a){
	ofSetColor(color.r,color.g,color.b,_a);
}

//----------------------------------------------------------
void ofSetColor(int r, int g, int b){

	currentStyle.color.r = r;
	currentStyle.color.g = g;
	currentStyle.color.b = b;
	currentStyle.color.a = 255.0f;

	renderer->setColor(r,g,b,255);
}


//----------------------------------------------------------
void ofSetColor(int r, int g, int b, int a){

	currentStyle.color.r = r;
	currentStyle.color.g = g;
	currentStyle.color.b = b;
	currentStyle.color.a = a;

	renderer->setColor(r,g,b,a);
}

//----------------------------------------------------------
void ofSetColor(int gray){
	if( gray > 255 ){
		ofLog(OF_LOG_WARNING, "ofSetColor(int hexColor) - has changed to ofSetColor(int gray) - perhaps you want ofSetHexColor instead?\n" );
	}
	ofSetColor(gray, gray, gray);
}

//----------------------------------------------------------
void ofSetHexColor(int hexColor){
	int r = (hexColor >> 16) & 0xff;
	int g = (hexColor >> 8) & 0xff;
	int b = (hexColor >> 0) & 0xff;
	ofSetColor(r,g,b);
}


//----------------------------------------------------------

void ofEnableBlendMode(ofBlendMode blendMode){
	currentStyle.blendingMode = blendMode;
	renderer->setBlendMode(blendMode);
}

//----------------------------------------------------------
void ofEnablePointSprites() {
	renderer->enablePointSprites();
}

//----------------------------------------------------------
void ofDisablePointSprites() {
	renderer->disablePointSprites();
}

//----------------------------------------------------------
void ofDisableBlendMode()
{
    glDisable(GL_BLEND);
	currentStyle.blendingMode = OF_BLENDMODE_DISABLED;
}

//----------------------------------------------------------
void ofEnableAlphaBlending(){
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//----------------------------------------------------------
void ofDisableAlphaBlending(){
    ofDisableBlendMode();
}

//----------------------------------------------------------
void ofEnableSmoothing(){
	// please see:
	// http://www.opengl.org/resources/faq/technical/rasterization.htm
	renderer->setLineSmoothing(true);
	currentStyle.smoothing = 1;
}

//----------------------------------------------------------
void ofDisableSmoothing(){
	renderer->setLineSmoothing(false);
	currentStyle.smoothing = 0;
}

//----------------------------------------------------------
void ofSetPolyMode(ofPolyWindingMode mode){
	shape.setPolyWindingMode(mode);
	currentStyle.polyMode = mode;
}

//----------------------------------------
void ofSetDrawBitmapMode(ofDrawBitmapMode mode) {
	currentStyle.drawBitmapMode = mode;
}

//----------------------------------------------------------
void ofSetStyle(ofStyle style){
	//color
	ofSetColor((int)style.color.r, (int)style.color.g, (int)style.color.b, (int)style.color.a);

	//bg color
	ofBackground(style.bgColor);

	//circle resolution - don't worry it only recalculates the display list if the res has changed
	ofSetCircleResolution(style.circleResolution);

	ofSetSphereResolution(style.sphereResolution);

	ofSetCurveResolution(style.curveResolution);

	//line width - finally!
	ofSetLineWidth(style.lineWidth);

	//rect mode: corner/center
	ofSetRectMode(style.rectMode);

	//poly mode: winding type
	ofSetPolyMode(style.polyMode);

	//fill
	if(style.bFill ){
		ofFill();
	}else{
		ofNoFill();
	}

	//smoothing
	if(style.smoothing ){
		ofEnableSmoothing();
	}else{
		ofDisableSmoothing();
	}

	//blending
	ofEnableBlendMode(currentStyle.blendingMode);
	
	//bitmap draw mode
	ofSetDrawBitmapMode(style.drawBitmapMode);
}

//----------------------------------------------------------
ofStyle ofGetStyle(){
	return currentStyle;
}

//----------------------------------------------------------
void ofPushStyle(){
	styleHistory.push_front(currentStyle);

	//if we are over the max number of styles we have set, then delete the oldest styles.
	if( styleHistory.size() > OF_MAX_STYLE_HISTORY ){
		styleHistory.pop_back();
		//should we warn here?
		//ofLog(OF_LOG_WARNING, "ofPushStyle - warning: you have used ofPushStyle more than %i times without calling ofPopStyle - check your code!", OF_MAX_STYLE_HISTORY);
	}
}

//----------------------------------------------------------
void ofPopStyle(){
	if( styleHistory.size() ){
		ofSetStyle(styleHistory.front());
		styleHistory.pop_front();
	}
}



// end drawing modes
//---------------------------------------------------------------------------




//----------------------------------------------------------
// primitives

//----------------------------------------------------------
void ofTriangle(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3){
	ofTriangle(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z);
}

//----------------------------------------------------------
void ofTriangle(float x1,float y1,float x2,float y2,float x3, float y3){
	ofTriangle(x1, y1, 0.0f, x2, y2, 0.0f, x3, y3, 0.0f);
}

//----------------------------------------------------------
void ofTriangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3, float y3,float z3){
	renderer->drawTriangle(x1,y1,z1,x2,y2,z2,x3,y3,z3);
}

//----------------------------------------------------------
void ofCircle(const ofPoint & p, float radius){
	ofCircle(p.x, p.y, p.z, radius);
}

//----------------------------------------------------------
void ofCircle(float x, float y, float radius){
	ofCircle(x,y,0,radius);
}

//----------------------------------------------------------
void ofCircle(float x, float y, float z, float radius){
	renderer->drawCircle(x,y,z,radius);
}

//----------------------------------------------------------
void ofEllipse(const ofPoint & p, float width, float height){
	ofEllipse(p.x, p.y, p.z, width, height);
}

//----------------------------------------------------------
void ofEllipse(float x, float y, float width, float height){
	ofEllipse(x,y,0,width,height);
}

//----------------------------------------------------------
void ofEllipse(float x, float y, float z, float width, float height){
	renderer->drawEllipse(x,y,z,width,height);
}

//----------------------------------------------------------
void ofLine(const ofPoint & p1, const ofPoint & p2){
	ofLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

//----------------------------------------------------------
void ofLine(float x1,float y1,float x2,float y2){
	ofLine(x1, y1, 0.0f, x2, y2, 0.0f);
}

//----------------------------------------------------------
void ofLine(float x1,float y1,float z1,float x2,float y2,float z2){
	renderer->drawLine(x1,y1,z1,x2,y2,z2);
}

//----------------------------------------------------------
void ofRect(const ofRectangle & r){
	ofRect(r.x, r.y, 0.0f, r.width, r.height);
}

//----------------------------------------------------------
void ofRect(const ofPoint & p,float w,float h){
	ofRect(p.x, p.y, p.z, w, h);
}

//----------------------------------------------------------
void ofRect(float x,float y,float w,float h){
	ofRect(x, y, 0.0f, w, h);
}

//----------------------------------------------------------
void ofRect(float x,float y,float z,float w,float h){
	renderer->drawRectangle(x,y,z,w,h);
}


//----------------------------------------------------------
void ofCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
	shape.clear();
	shape.curveTo(x0,y0);
	shape.curveTo(x1,y1);
	shape.curveTo(x2,y2);
	shape.curveTo(x3,y3);
	shape.draw();
}


//----------------------------------------------------------
void ofBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
	shape.clear();
	shape.moveTo(x0,y0);
	shape.bezierTo(x1,y1,x2,y2,x3,y3);
	shape.draw();
}

//----------------------------------------------------------
void ofBeginShape(){
	shape.clear();
}

//----------------------------------------------------------
void ofVertex(float x, float y){
	shape.lineTo(x,y);

}

//---------------------------------------------------
void ofVertex(ofPoint & p) {
	shape.lineTo(p);
}

//----------------------------------------------------------
void ofVertexes( const vector <ofPoint> & polyPoints ){
	for( int k = 0; k < (int)polyPoints.size(); k++){
		shape.lineTo(polyPoints[k]);
	}
}

//---------------------------------------------------
void ofCurveVertex(float x, float y){
	shape.curveTo(x,y);
}

//----------------------------------------------------------
void ofCurveVertexes( const vector <ofPoint> & curvePoints){
	for( int k = 0; k < (int)curvePoints.size(); k++){
		shape.curveTo(curvePoints[k]);
	}
}

//---------------------------------------------------
void ofCurveVertex(ofPoint & p) {
	shape.curveTo(p);
}

//---------------------------------------------------
void ofBezierVertex(float x1, float y1, float x2, float y2, float x3, float y3){
	shape.bezierTo(x1,y1,x2,y2,x3,y3);
}

//----------------------------------------------------------
void ofNextContour(bool bClose){
	if (bClose){
		shape.close();
	}
	shape.newSubPath();
}


//----------------------------------------------------------
void ofEndShape(bool bClose){

	// (close -> add the first point to the end)
	// -----------------------------------------------

	if (bClose){
		shape.close();
	}

	shape.draw();

}

//--------------------------------------------------
// 3d primitives

//----------------------------------------
void ofSphere(float x, float y, float z, float radius) {
	ofSphere(ofPoint(x, y, z), radius);
}

//----------------------------------------
void ofSphere(float x, float y, float radius) {
	ofSphere(x, y, 0, radius);
}

//----------------------------------------
void ofSphere(const ofPoint& position, float radius) {
	ofPushMatrix();
	ofTranslate(position);
	ofSphere(radius);
	ofPopMatrix();
}

//----------------------------------------
void ofSphere(float radius) {
	// TODO: add an implementation using ofMesh
#ifndef TARGET_OPENGLES
	// this needs to be swapped out with non-glut code
	// for good references see cinder's implementation from paul bourke:
	// https://github.com/cinder/Cinder/blob/master/src/cinder/gl/gl.cpp
	// void drawSphere( const Vec3f &center, float radius, int segments )
	// and processing's implementation of icospheres:
	// https://code.google.com/p/processing/source/browse/trunk/processing/core/src/processing/core/PGraphics.java?r=7543
	// public void sphere(float r)
	
	ofPushMatrix();
	ofRotateX(90);
	if(ofGetStyle().bFill) {
		glutSolidSphere(radius, 2 * currentStyle.sphereResolution, currentStyle.sphereResolution);
	} else {
		glutWireSphere(radius, 2 * currentStyle.sphereResolution, currentStyle.sphereResolution);
	}
	ofPopMatrix();
#endif
}

//----------------------------------------
void ofBox(float x, float y, float z, float size) {
	ofBox(ofPoint(x, y, z), size);
}

//----------------------------------------
void ofBox(float x, float y, float size) {
	ofBox(x, y, 0, size);
}

//----------------------------------------
void ofBox(const ofPoint& position, float size) {
	ofPushMatrix();
	ofTranslate(position);
	ofBox(size);
	ofPopMatrix();
}

//----------------------------------------
void ofBox(float size) {
	ofPushMatrix();
	if(ofGetCoordHandedness() == OF_LEFT_HANDED) {
		ofScale(1, 1, -1);
	}

	float h = size * .5;
	
	vertexData.clear();
	if(ofGetStyle().bFill) {
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
		vertexData.setMode(OF_TRIANGLES_MODE);
		renderer->draw(vertexData);
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

		vertexData.setMode(OF_LINES_MODE);
		renderer->draw(vertexData);
	}


	ofPopMatrix();
}

// end 3d primitives
//--------------------------------------------------


//--------------------------------------------------
// text
//--------------------------------------------------
void ofDrawBitmapString(string textString, const ofPoint & p){
	ofDrawBitmapString(textString, p.x, p.y, p.z);
}
//--------------------------------------------------
void ofDrawBitmapString(string textString, float x, float y){
	ofDrawBitmapString(textString, x, y, 0.0f);
}
//--------------------------------------------------
void ofDrawBitmapString(string textString, float x, float y, float z){
	renderer->drawString(textString,x,y,z,currentStyle.drawBitmapMode);
}


// end text
//--------------------------------------------------
