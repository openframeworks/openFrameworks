#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofBitmapFont.h"
#include "ofUtils.h"
#include "ofBaseTypes.h"
#include "ofGLRenderer.h"
#include "ofShape.h"
#include "ofShapeUtils.h"

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


#include <deque>

//----------------------------------------------------------
// static
static float	drawMode			= OF_FILLED;
static bool		bSmoothHinted			= false;
static bool			bUsingArbTex		= true;
static bool			bUsingNormalizedTexCoords = false;
static ofRectMode	cornerMode			= OF_RECTMODE_CORNER;
static ofPolyWindingMode		polyMode	= OF_POLY_WINDING_ODD;

int				curveResolution = 20;

ofHandednessType coordHandedness;

//style stuff - new in 006
static ofStyle			currentStyle;
static deque <ofStyle> styleHistory;
static deque <ofRectangle> viewportHistory;

//memory for drawing basic shapes ultrafast
static vector<ofPoint> linePoints;
static vector<ofPoint> rectPoints;
static vector<ofPoint> triPoints;
static vector<ofPoint> circlePoints;
static ofPolyline circlePolyline;


void 			setupCircle();

static ofShape shape;
static ofPrimitive vertexData;
static ofBaseRenderer * renderer = NULL;

void ofSetDefaultRenderer(ofBaseRenderer * renderer_){
	if(renderer) delete renderer;
	renderer = renderer_;
	renderer->setupGraphicDefaults();
	if(renderer->rendersPathDirectly()){
		shape.setMode(ofShape::PATHS);
	}else{
		shape.setMode(ofShape::POLYLINES);
	}
	shape.setFilled(currentStyle.bFill);
	shape.setStrokeWidth(currentStyle.bFill?0:currentStyle.lineWidth);
	shape.setPolyWindingMode(currentStyle.polyMode);
	linePoints.resize(2);
	rectPoints.resize(4);
	triPoints.resize(3);
	setupCircle();
}

ofBaseRenderer * ofGetDefaultRenderer(){
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
void ofSetupScreenPerspective(float width, float height, int orientation, bool vFlip, float fov, float nearDist, float farDist) {
	renderer->setupScreenPerspective(width,height, orientation, vFlip,fov,nearDist,farDist);
}

//----------------------------------------
void ofSetDrawBitmapMode(ofDrawBitmapMode mode) {
	currentStyle.drawBitmapMode = mode;
}

//----------------------------------------------------------
void ofSetupScreenOrtho(float width, float height, bool vFlip, float nearDist, float farDist) {
	renderer->setupScreenOrtho(width,height,vFlip,nearDist,farDist);
}

//----------------------------------------------------------
//Resets openGL parameters back to OF defaults
void ofSetupGraphicDefaults(){
	renderer->setupGraphicDefaults();
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
	return renderer->getBgColor().v;
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
	renderer->background(r,g,b,a);
}

// end background functions
//----------------------------------------------------------




//---------------------------------------------------------------------------
// drawing modes

//----------------------------------------------------------
void  ofSetRectMode(ofRectMode mode){
	if (mode == OF_RECTMODE_CORNER) 		cornerMode = OF_RECTMODE_CORNER;
	else if (mode == OF_RECTMODE_CENTER) 	cornerMode = OF_RECTMODE_CENTER;

	currentStyle.rectMode = cornerMode;
}

//----------------------------------------------------------
ofRectMode ofGetRectMode(){
	return 	cornerMode;
}

//----------------------------------------------------------
void ofNoFill(){
	drawMode = OF_OUTLINE;
	shape.setFilled(false);
	shape.setStrokeWidth(currentStyle.lineWidth);
	currentStyle.bFill = false;
}

//----------------------------------------------------------
void ofFill(){
	drawMode = OF_FILLED;
	shape.setFilled(true);
	shape.setStrokeWidth(0);
	currentStyle.bFill = true;
}

// Returns OF_FILLED or OF_OUTLINE
//----------------------------------------------------------
int ofGetFill(){
	return drawMode;
}

//----------------------------------------------------------
void ofSetLineWidth(float lineWidth){
	glLineWidth(lineWidth);
	currentStyle.lineWidth = lineWidth;
}

//----------------------------------------
void ofSetCurveResolution(int res){
	curveResolution = res;
	shape.setCurveResolution(res);
}

//----------------------------------------
void ofSetSphereResolution(int res) {
	currentStyle.sphereResolution = res;
}

//----------------------------------------------------------
void setupCircle(){
	ofSetCircleResolution(CIRC_RESOLUTION);
}

//----------------------------------------------------------
void ofSetCircleResolution(int res){
	if(circlePolyline.size()!=res+1){
		circlePolyline.clear();
		circlePolyline.arc(0,0,0,1,1,0,360,res);
		circlePoints.resize(circlePolyline.size());
	}
}

//----------------------------------------------------------
void startSmoothing();
void startSmoothing(){
	#ifndef TARGET_OPENGLES
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
	#endif

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);

	//why do we need this?
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


//----------------------------------------------------------
void endSmoothing();
void endSmoothing(){
	#ifndef TARGET_OPENGLES
		glPopAttrib();
	#endif
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
    switch (blendMode){

        case OF_BLENDMODE_ALPHA:{
            glEnable(GL_BLEND);
			#ifndef TARGET_OPENGLES
				glBlendEquation(GL_FUNC_ADD);
			#endif
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            currentStyle.blending = 1;
            currentStyle.blendSrc = GL_SRC_ALPHA;
            currentStyle.blendDst = GL_ONE_MINUS_SRC_ALPHA;
			#ifndef TARGET_OPENGLES
				currentStyle.blendEquation = GL_FUNC_ADD;
			#endif
            break;
        }

        case OF_BLENDMODE_ADD:{
            glEnable(GL_BLEND);
			#ifndef TARGET_OPENGLES
				glBlendEquation(GL_FUNC_ADD);
			#endif
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            currentStyle.blending = 1;
            currentStyle.blendSrc = GL_SRC_ALPHA;
            currentStyle.blendDst = GL_ONE;
			#ifndef TARGET_OPENGLES
				currentStyle.blendEquation = GL_FUNC_ADD;
			#endif
			break;
        }

        case OF_BLENDMODE_MULTIPLY:{
            glEnable(GL_BLEND);
			#ifndef TARGET_OPENGLES
				glBlendEquation(GL_FUNC_ADD);
			#endif
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA /* GL_ZERO or GL_ONE_MINUS_SRC_ALPHA */);
            currentStyle.blending = 1;
            currentStyle.blendSrc = GL_DST_COLOR;
            currentStyle.blendDst = GL_ONE_MINUS_SRC_ALPHA;
			#ifndef TARGET_OPENGLES
				currentStyle.blendEquation = GL_FUNC_ADD;
			#endif
			break;
        }

        case OF_BLENDMODE_SCREEN:{
            glEnable(GL_BLEND);
			#ifndef TARGET_OPENGLES
				glBlendEquation(GL_FUNC_ADD);
			#endif
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
            currentStyle.blending = 1;
            currentStyle.blendSrc = GL_ONE_MINUS_DST_COLOR;
            currentStyle.blendDst = GL_ONE;
			#ifndef TARGET_OPENGLES
				currentStyle.blendEquation = GL_FUNC_ADD;
			#endif
			break;
        }

        case OF_BLENDMODE_SUBTRACT:{
            glEnable(GL_BLEND);
		#ifndef TARGET_OPENGLES
            glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		#else
			ofLog(OF_LOG_WARNING, "OF_BLENDMODE_SUBTRACT not currently supported on iPhone");
		#endif
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            currentStyle.blending = 1;
            currentStyle.blendSrc = GL_SRC_ALPHA;
            currentStyle.blendDst = GL_ONE;
			#ifndef TARGET_OPENGLES
				currentStyle.blendEquation = GL_FUNC_ADD;
			#endif
			break;
        }


        default:
            break;
    }
}

//----------------------------------------------------------
void ofEnablePointSprites() {
#ifdef TARGET_OPENGLES
	glEnable(GL_POINT_SPRITE_OES);
	glTexEnvi(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE);
	// does look like this needs to be enabled in ES because
	// it is always eneabled...
	//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	
#else
	glEnable(GL_POINT_SPRITE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);	
#endif
}

//----------------------------------------------------------
void ofDisablePointSprites() {
#ifdef TARGET_OPENGLES
	glDisable(GL_POINT_SPRITE_OES);
#else
	glDisable(GL_POINT_SPRITE);
#endif
}

//----------------------------------------------------------
void ofDisableBlendMode()
{
    glDisable(GL_BLEND);
	currentStyle.blending = 0;
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
	bSmoothHinted = true;
	currentStyle.smoothing = 1;
}

//----------------------------------------------------------
void ofDisableSmoothing(){
	bSmoothHinted = false;
	currentStyle.smoothing = 0;
}

//----------------------------------------------------------
void ofSetStyle(ofStyle style){
	//color
	ofSetColor((int)style.color.r, (int)style.color.g, (int)style.color.b, (int)style.color.a);

	//circle resolution - don't worry it only recalculates the display list if the res has changed
	ofSetCircleResolution(style.circleResolution);

	ofSetSphereResolution(style.sphereResolution);

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
	if(style.blending ){
		ofEnableAlphaBlending();
	}else{
		ofDisableAlphaBlending();
	}
	
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


//----------------------------------------------------------
void ofSetPolyMode(ofPolyWindingMode mode){
	polyMode = mode;
	shape.setPolyWindingMode(mode);
	currentStyle.polyMode = polyMode;
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

	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	if(renderer->rendersPathDirectly()){
		shape.clear();
		shape.moveTo(x1,y1,z1);
		shape.lineTo(x2,y2,z2);
		shape.lineTo(x3,y3,z3);
		shape.draw();
	}else{
		triPoints[0].set(x1,y1,z1);
		triPoints[1].set(x2,y2,z2);
		triPoints[2].set(x3,y3,z3);
		renderer->draw(triPoints,drawMode==OF_FILLED?OF_TRIANGLES_MODE:OF_LINE_LOOP_MODE);
	}

	// back to normal, if smoothness is on
	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();
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

	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	if(renderer->rendersPathDirectly()){
		shape.clear();
		shape.arc(x,y,z,radius,radius,0,360);
		shape.draw();
	}else{
		vector<ofPoint> & circleCache = circlePolyline.getVertices();
		for(int i=0;i<circleCache.size();i++){
			circlePoints[i].set(radius*circleCache[i].x+x,radius*circleCache[i].y+y,z);
		}
		renderer->draw(circlePoints, (drawMode == OF_FILLED) ? OF_TRIANGLE_FAN_MODE : OF_LINE_LOOP_MODE);
	}
	// back to normal, if smoothness is on
	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();

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

	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();
	float radiusX = width*0.5;
	float radiusY = height*0.5;
	if(renderer->rendersPathDirectly()){
		shape.clear();
		shape.arc(x,y,z,radiusX,radiusY,0,360);
		shape.draw();
	}else{
		vector<ofPoint> & circleCache = circlePolyline.getVertices();
		for(int i=0;i<circleCache.size();i++){
			circlePoints[i].set(radiusX*circlePolyline[i].x+x,radiusY*circlePolyline[i].y+y,z);
		}
		renderer->draw(circlePoints, (drawMode == OF_FILLED) ? OF_TRIANGLE_FAN_MODE : OF_LINE_LOOP_MODE);
	}

	// back to normal, if smoothness is on
	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();
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

	// use smoothness, if requested:
	if (bSmoothHinted) startSmoothing();

	if(renderer->rendersPathDirectly()){
		shape.clear();
		shape.moveTo(x1,y1,z1);
		shape.lineTo(x2,y2,z2);
		shape.draw();
	}else{
		linePoints[0].set(x1,y1,z1);
		linePoints[1].set(x2,y2,z2);
		renderer->draw(linePoints,OF_LINES_MODE);
	}

	// back to normal, if smoothness is on
	if (bSmoothHinted) endSmoothing();
	
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

	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	if(renderer->rendersPathDirectly()){
		shape.clear();
		if (cornerMode == OF_RECTMODE_CORNER){
			shape.moveTo(x,y,z);
			shape.lineTo(x+w,y,z);
			shape.lineTo(x+w,y+h,z);
			shape.lineTo(x,y+h,z);
		}else{
			shape.moveTo(x-w/2.0f,y-h/2.0f,z);
			shape.lineTo(x+w/2.0f,y-h/2.0f,z);
			shape.lineTo(x+w/2.0f,y+h/2.0f,z);
			shape.lineTo(x-w/2.0f,y+h/2.0f,z);
		}
	}else{
		if (cornerMode == OF_RECTMODE_CORNER){
			rectPoints[0].set(x,y,z);
			rectPoints[1].set(x+w,y,z);
			rectPoints[2].set(x+w,y+h,z);
			rectPoints[3].set(x,y+h,z);
		}else{
			rectPoints[0].set(x-w/2.0f,y-h/2.0f,z);
			rectPoints[1].set(x+w/2.0f,y-h/2.0f,z);
			rectPoints[2].set(x+w/2.0f,y+h/2.0f,z);
			rectPoints[3].set(x-w/2.0f,y+h/2.0f,z);
		}
	}
	renderer->draw(rectPoints,(drawMode == OF_FILLED) ? OF_TRIANGLE_FAN_MODE : OF_LINE_LOOP_MODE);

	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();
}


//----------------------------------------------------------
void ofCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
	shape.clear();
	shape.curveTo(x0,y0,curveResolution);
	shape.curveTo(x1,y1,curveResolution);
	shape.curveTo(x2,y2,curveResolution);
	shape.curveTo(x3,y3,curveResolution);
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

	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

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

   	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();

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
	
	// this is copied from the ofTrueTypeFont
	//GLboolean blend_enabled = glIsEnabled(GL_BLEND); //TODO: this is not used?
	GLint blend_src, blend_dst;
	glGetIntegerv( GL_BLEND_SRC, &blend_src );
	glGetIntegerv( GL_BLEND_DST, &blend_dst );
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	int len = (int)textString.length();
	//float yOffset = 0;
	float fontSize = 8.0f;
	bool bOrigin = false;
	
	float sx = 0;
	float sy = -fontSize;


	///////////////////////////
	// APPLY TRANSFORM / VIEW
	///////////////////////////
	//

	bool hasModelView = false;
	bool hasProjection = false;
	bool hasViewport = false;

	ofRectangle rViewport;

	switch (currentStyle.drawBitmapMode) {

		case OF_BITMAPMODE_SIMPLE:

			sx += x;
			sy += y;
			break;

		case OF_BITMAPMODE_SCREEN:

			hasViewport = true;
			ofPushView();

			rViewport = ofGetWindowRect();
			ofViewport(rViewport);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glTranslatef(-1, 1, 0);
			glScalef(2/rViewport.width, -2/rViewport.height, 1);

			ofTranslate(x, y, 0);
			break;

		case OF_BITMAPMODE_VIEWPORT:

			rViewport = ofGetCurrentViewport();

			hasProjection = true;
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();

			hasModelView = true;
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glTranslatef(-1, 1, 0);
			glScalef(2/rViewport.width, -2/rViewport.height, 1);

			ofTranslate(x, y, 0);
			break;

		case OF_BITMAPMODE_MODEL:

			hasModelView = true;
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

			ofTranslate(x, y, z);
			ofScale(1, -1, 0);
			break;

		case OF_BITMAPMODE_MODEL_BILLBOARD:
			//our aim here is to draw to screen
			//at the viewport position related
			//to the world position x,y,z
			
			// ***************
			// this will not compile for opengl ES
			// ***************
#ifndef TARGET_OPENGLES
			//gluProject method
			GLdouble modelview[16], projection[16];
			GLint view[4];
			double dScreenX, dScreenY, dScreenZ;
			glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
			glGetDoublev(GL_PROJECTION_MATRIX, projection);
			glGetIntegerv(GL_VIEWPORT, view);
			view[0] = 0; view[1] = 0; //we're already drawing within viewport
			gluProject(x, y, z, modelview, projection, view, &dScreenX, &dScreenY, &dScreenZ);

			rViewport = ofGetCurrentViewport();

			hasProjection = true;
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();

			hasModelView = true;
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glTranslatef(-1, -1, 0);
			glScalef(2/rViewport.width, 2/rViewport.height, 1);
			
			glTranslatef(dScreenX, dScreenY, 0);
			glScalef(1, -1, 1);
#endif
			break;

		default:
			break;
	}
	//
	///////////////////////////

	
	// (c) enable texture once before we start drawing each char (no point turning it on and off constantly)
	//We do this because its way faster
	ofDrawBitmapCharacterStart();
	
	for(int c = 0; c < len; c++){
		if(textString[c] == '\n'){
			
			sy += bOrigin ? -1 : 1 * (fontSize*1.7);
			sx = x;
			
			//glRasterPos2f(x,y + (int)yOffset);
		} else if (textString[c] >= 32){
			// < 32 = control characters - don't draw
			// solves a bug with control characters
			// getting drawn when they ought to not be
			ofDrawBitmapCharacter(textString[c], (int)sx, (int)sy);
			
			sx += fontSize;
		}
	}
	//We do this because its way faster
	ofDrawBitmapCharacterEnd();


	if (hasModelView)
		glPopMatrix();

	if (hasProjection)
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	if (hasViewport)
		ofPopView();}


// end text
//--------------------------------------------------
