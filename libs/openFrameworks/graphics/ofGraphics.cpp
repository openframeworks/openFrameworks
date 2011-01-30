#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofBitmapFont.h"
#include "ofUtils.h"
#include "ofBaseTypes.h"
#include "ofGLRenderer.h"
#include "ofPath.h"
#include "ofShapeUtils.h"

#ifdef TARGET_OSX
	#include <OpenGL/glu.h>
#endif

//#ifdef TARGET_OPENGLES
//	#include "glu.h"
//#endif
//
#ifdef TARGET_LINUX
	#include "GL/glu.h"
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
static int		numCirclePts		= 0;
static float	bgColor[4]			= {0,0,0,0};
static bool		bSmoothHinted			= false;
static bool			bUsingArbTex		= true;
static bool			bUsingNormalizedTexCoords = false;
static bool 		bBakgroundAuto		= true;
static ofRectMode	cornerMode			= OF_RECTMODE_CORNER;
static ofPolyWindingMode		polyMode	= OF_POLY_WINDING_ODD;

int				curveResolution = 20;

ofHandednessType coordHandedness;

//style stuff - new in 006
static ofStyle			currentStyle;
static deque <ofStyle> styleHistory;
static deque <ofRectangle> viewportHistory;


void 			setupCircle();


static ofPath path;
static ofShape shape;
static ofVertexData vertexData;
static ofBaseRenderer * renderer = new ofGLRenderer;

void ofSetDefaultRenderer(ofBaseRenderer * renderer_){
	if(renderer) delete renderer;
	renderer = renderer_;
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
	renderer->translate(xAmnt, yAmnt, zAmnt);
}

//----------------------------------------------------------
void ofRotate(float degrees, float vecX, float vecY, float vecZ){
	renderer->rotate(degrees, vecX, vecY, vecZ);
}

//----------------------------------------------------------
void ofRotateX(float degrees){
	renderer->translate(degrees);
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
	glClearColor(r / 255, g / 255, b / 255, a / 255);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
void ofClear(float brightness, float a) {
	ofClear(brightness, brightness, brightness, a);
}

//----------------------------------------------------------
void ofClearAlpha() {
	glColorMask(0, 0, 0, 1);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColorMask(1, 1, 1, 1);
}	

//----------------------------------------------------------
void ofSetBackgroundAuto(bool bAuto){
	bBakgroundAuto = bAuto;
}

//----------------------------------------------------------
bool ofbClearBg(){
	return bBakgroundAuto;
}

//----------------------------------------------------------
float * ofBgColorPtr(){
	return bgColor;
}

//----------------------------------------------------------
void ofBackground(const ofColor & c){
	ofBackground ( c.r, c.g, c.b);
}

//----------------------------------------------------------
void ofBackground(float brightness) {
	ofBackground(brightness);
}

//----------------------------------------------------------
void ofBackground(int hexColor, float _a){
	ofBackground ( (hexColor >> 16) & 0xff, (hexColor >> 8) & 0xff, (hexColor >> 0) & 0xff, _a);
}

//----------------------------------------------------------
void ofBackground(int r, int g, int b, int a){
	bgColor[0] = (float)r / (float)255.0f;
	bgColor[1] = (float)g / (float)255.0f;
	bgColor[2] = (float)b / (float)255.0f;
	bgColor[3] = (float)a / (float)255.0f;
	// if we are in not-auto mode, then clear with a bg call...
	if (ofbClearBg() == false){
		glClearColor(bgColor[0],bgColor[1],bgColor[2], bgColor[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

// end background functions
//----------------------------------------------------------




//---------------------------------------------------------------------------
// drawing modes

static void ofSetCurrentStyleTo(ofPath & path){
	path.setFilled(drawMode == OF_FILLED);
	path.setColor(ofColor(255,255,255,255));
	path.setPolyWindingMode(currentStyle.polyMode);

	if(drawMode == OF_OUTLINE){
		path.setStrokeWidth(currentStyle.lineWidth);
	}
}

static void ofSetCurrentStyleTo(ofShape & shape){
	shape.setFilled(drawMode == OF_FILLED);
	path.setColor(ofColor(255,255,255,255));
	shape.setPolyWindingMode(currentStyle.polyMode);

	if(drawMode == OF_OUTLINE){
		shape.setStrokeWidth(currentStyle.lineWidth);
	}
}

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
	currentStyle.bFill = false;
}

//----------------------------------------------------------
void ofFill(){
	drawMode = OF_FILLED;
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
}

//----------------------------------------
void ofSetSphereResolution(int res) {
	currentStyle.sphereResolution = res;
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
void setupCircle(){
	ofSetCircleResolution(CIRC_RESOLUTION);
}

//----------------------------------------------------------
void ofSetCircleResolution(int res){
	numCirclePts = res;
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
void ofSetColor(int _r, int _g, int _b){
	float r = (float)_r / 255.0f; r = MAX(0,MIN(r,1.0f));
	float g = (float)_g / 255.0f; g = MAX(0,MIN(g,1.0f));
	float b = (float)_b / 255.0f; b = MAX(0,MIN(b,1.0f));

	currentStyle.color.r = r * 255.0f;
	currentStyle.color.g = g * 255.0f;
	currentStyle.color.b = b * 255.0f;
	currentStyle.color.a = 255.0f;

	glColor4f(r,g,b,1);
}


//----------------------------------------------------------
void ofSetColor(int _r, int _g, int _b, int _a){
	float r = (float)_r / 255.0f; r = MAX(0,MIN(r,1.0f));
	float g = (float)_g / 255.0f; g = MAX(0,MIN(g,1.0f));
	float b = (float)_b / 255.0f; b = MAX(0,MIN(b,1.0f));
	float a = (float)_a / 255.0f; a = MAX(0,MIN(a,1.0f));

	currentStyle.color.r = r * 255.0f;
	currentStyle.color.g = g * 255.0f;
	currentStyle.color.b = b * 255.0f;
	currentStyle.color.a = a * 255.0f;

	glColor4f(r,g,b,a);
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
#ifdef TARGET_OF_IPHONE
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
#ifdef TARGET_OF_IPHONE
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
		path=ofTrianglePath(x1,y1,z1,x2,y2,z2,x3,y3,z3);
		ofSetCurrentStyleTo(path);
		renderer->draw(path);
	}else{
		shape=ofTriangleShape(x1,y1,z1,x2,y2,z2,x3,y3,z3);
		ofSetCurrentStyleTo(shape);
		renderer->draw(shape);
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
	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	if(renderer->rendersPathDirectly()){
		path.clear();
		ofSetCurrentStyleTo(path);
		path.arc(x,y,radius,radius,0,360);
		renderer->draw(path);
	}else{
		shape.clear();
		ofSetCurrentStyleTo(shape);
		shape.arc(x,y,radius,radius,0,360,currentStyle.circleResolution);
		renderer->draw(shape);
	}

	// back to normal, if smoothness is on
	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofCircle(float x, float y, float z, float radius){

	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	if(renderer->rendersPathDirectly()){
		path.clear();
		ofSetCurrentStyleTo(path);
		path.arc(x,y,z,radius,radius,0,360);
		renderer->draw(path);
	}else{
		shape.clear();
		ofSetCurrentStyleTo(shape);
		shape.arc(x,y,z,radius,radius,0,360,currentStyle.circleResolution);
		renderer->draw(shape);
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
	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	if(renderer->rendersPathDirectly()){
		path.clear();
		ofSetCurrentStyleTo(path);
		path.arc(x,y,width*0.5,height*0.5,0,360);
		renderer->draw(path);
	}else{
		shape.clear();
		ofSetCurrentStyleTo(shape);
		shape.arc(x,y,width*0.5,height*0.5,0,360,currentStyle.circleResolution);
		renderer->draw(shape);
	}

	// back to normal, if smoothness is on
	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofEllipse(float x, float y, float z, float width, float height){

	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	if(renderer->rendersPathDirectly()){
		path.clear();
		ofSetCurrentStyleTo(path);
		path.arc(x,y,z,width*0.5,height*0.5,0,360);
		renderer->draw(path);
	}else{
		shape.clear();
		ofSetCurrentStyleTo(shape);
		shape.arc(x,y,z,width*0.5,height*0.5,0,360,currentStyle.circleResolution);
		renderer->draw(shape);
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
		path.clear();
		ofSetCurrentStyleTo(path);
		path.lineTo(x1,y1,z1);
		path.lineTo(x2,y2,z2);
		renderer->draw(path);
	}else{
		shape.clear();
		ofSetCurrentStyleTo(shape);
		shape.lineTo(x1,y1,z1);
		shape.lineTo(x2,y2,z2);
		renderer->draw(shape);
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
		path.clear();
		ofSetCurrentStyleTo(path);
		if (cornerMode == OF_RECTMODE_CORNER){
			path.lineTo(x,y,z);
			path.lineTo(x+w,y,z);
			path.lineTo(x+w,y+h,z);
			path.lineTo(x,y+h,z);
		}else{
			path.lineTo(x-w/2.0f,y-h/2.0f,z);
			path.lineTo(x+w/2.0f,y-h/2.0f,z);
			path.lineTo(x+w/2.0f,y+h/2.0f,z);
			path.lineTo(x-w/2.0f,y+h/2.0f,z);
		}
		renderer->draw(path);
	}else{
		shape.clear();
		ofSetCurrentStyleTo(shape);
		if (cornerMode == OF_RECTMODE_CORNER){
			shape.lineTo(x,y,z);
			shape.lineTo(x+w,y,z);
			shape.lineTo(x+w,y+h,z);
			shape.lineTo(x,y+h,z);
		}else{
			shape.lineTo(x-w/2.0f,y-h/2.0f,z);
			shape.lineTo(x+w/2.0f,y-h/2.0f,z);
			shape.lineTo(x+w/2.0f,y+h/2.0f,z);
			shape.lineTo(x-w/2.0f,y+h/2.0f,z);
		}
		renderer->draw(shape);
	}



	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();
}


//----------------------------------------------------------
void ofCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
	if(renderer->rendersPathDirectly()){
		path.clear();
		ofSetCurrentStyleTo(path);
		path.curveTo(x0,y0);
		path.curveTo(x1,y1);
		path.curveTo(x2,y2);
		path.curveTo(x3,y3);
		renderer->draw(path);
	}else{
		shape.clear();
		ofSetCurrentStyleTo(shape);
		shape.curveTo(x0,y0);
		shape.curveTo(x1,y1);
		shape.curveTo(x2,y2);
		shape.curveTo(x3,y3);
		renderer->draw(shape);
	}
}


//----------------------------------------------------------
void ofBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
	if(renderer->rendersPathDirectly()){
		path.clear();
		ofSetCurrentStyleTo(path);
		path.moveTo(x0,y0);
		path.bezierTo(x1,y1,x2,y2,x3,y3);
		renderer->draw(path);
	}else{
		shape.clear();
		ofSetCurrentStyleTo(shape);
		shape.moveTo(x0,y0);
		shape.bezierTo(x1,y1,x2,y2,x3,y3);
		renderer->draw(shape);
	}
}

//----------------------------------------------------------
void ofBeginShape(){

	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	if(renderer->rendersPathDirectly()){
		path.clear();
		ofSetCurrentStyleTo(path);
	}else{
		shape.clear();
		ofSetCurrentStyleTo(shape);
	}
}

//----------------------------------------------------------
void ofVertex(float x, float y){
	if(renderer->rendersPathDirectly()){
		path.lineTo(x,y);
	}else{
		shape.lineTo(x,y);
	}

}

//---------------------------------------------------
void ofVertex(ofPoint & p) {
	if(renderer->rendersPathDirectly()){
		path.lineTo(p);
	}else{
		shape.lineTo(p);
	}
}

//----------------------------------------------------------
void ofVertexes( const vector <ofPoint> & polyPoints ){
	if(renderer->rendersPathDirectly()){
		for( int k = 0; k < (int)polyPoints.size(); k++){
			path.lineTo(polyPoints[k]);
		}
	}else{
		for( int k = 0; k < (int)polyPoints.size(); k++){
			shape.lineTo(polyPoints[k]);
		}
	}
}

//---------------------------------------------------
void ofCurveVertex(float x, float y){
	if(renderer->rendersPathDirectly()){
		path.curveTo(x,y);
	}else{
		shape.curveTo(x,y);
	}
}

//----------------------------------------------------------
void ofCurveVertexes( const vector <ofPoint> & curvePoints){
	if(renderer->rendersPathDirectly()){
		for( int k = 0; k < (int)curvePoints.size(); k++){
			path.curveTo(curvePoints[k]);
		}
	}else{
		for( int k = 0; k < (int)curvePoints.size(); k++){
			shape.curveTo(curvePoints[k]);
		}
	}
}

//---------------------------------------------------
void ofCurveVertex(ofPoint & p) {
	if(renderer->rendersPathDirectly()){
		shape.curveTo(p);
	}
}

//---------------------------------------------------
void ofBezierVertex(float x1, float y1, float x2, float y2, float x3, float y3){
	if(renderer->rendersPathDirectly()){
		path.bezierTo(x1,y1,x2,y2,x3,y3);
	}else{
		shape.bezierTo(x1,y1,x2,y2,x3,y3);
	}
}

//----------------------------------------------------------
void ofNextContour(bool bClose){
	if(renderer->rendersPathDirectly()){
		if (bClose){
			path.close();
		}else{
			path.newSubPath();
		}
	}else{
		if (bClose){
			shape.close();
		}else{
			shape.newSubShape();
		}
	}
}


//----------------------------------------------------------
void ofEndShape(bool bClose){

	// (close -> add the first point to the end)
	// -----------------------------------------------

	if(renderer->rendersPathDirectly()){
		if (bClose){
			path.close();
		}

		renderer->draw(path);
	}else{
		if (bClose){
			shape.close();
		}

		renderer->draw(shape);
	}

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
	
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_VERTEX_ARRAY);
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
		//glVertexPointer(3, GL_FLOAT, 0, vertices);
		vertexData.addVertices(vertices,24);
		
		static ofVec3f normals[] = {
			ofVec3f(+1,0,0), ofVec3f(+1,0,0), ofVec3f(+1,0,0), ofVec3f(+1,0,0),
			ofVec3f(0,+1,0), ofVec3f(0,+1,0), ofVec3f(0,+1,0), ofVec3f(0,+1,0),
			ofVec3f(0,0,+1), ofVec3f(0,0,+1), ofVec3f(0,0,+1), ofVec3f(0,0,+1),
			ofVec3f(-1,0,0), ofVec3f(-1,0,0), ofVec3f(-1,0,0), ofVec3f(-1,0,0),
			ofVec3f(0,-1,0), ofVec3f(0,-1,0), ofVec3f(0,-1,0), ofVec3f(0,-1,0),
			ofVec3f(0,0,-1), ofVec3f(0,0,-1), ofVec3f(0,0,-1), ofVec3f(0,0,-1)
		};
		//glNormalPointer(GL_FLOAT, 0, normals);
		vertexData.addNormals(normals,24);

		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		static ofVec2f tex[] = {
			ofVec2f(1,0), ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1),
			ofVec2f(1,1), ofVec2f(1,0), ofVec2f(0,0), ofVec2f(0,1),
			ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0), ofVec2f(0,0),
			ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0),
			ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0),
			ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0)
		};
		//glTexCoordPointer(2, GL_FLOAT, 0, tex);
		vertexData.addTexCoords(tex,24);
	
		GLuint indices[] = {
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
		//glDrawElements(GL_TRIANGLES, 3 * 6 * 2, GL_UNSIGNED_BYTE, indices);
		vertexData.addIndices(indices,36);
		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		vertexData.setMode(OF_TRIANGLES_MODE);
		renderer->draw(vertexData);
		//vboMesh.setVertexData(&vertexData);
		//vboMesh.drawFaces();
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
		//glVertexPointer(3, GL_FLOAT, 0, vertices);
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
		//glNormalPointer(GL_FLOAT, 0, normals);
		vertexData.addNormals(normals,8);

		static GLuint indices[] = {
			0,1, 1,3, 3,2, 2,0,
			4,5, 5,7, 7,6, 6,4,
			0,4, 5,1, 7,3, 6,2
		};
		//glDrawElements(GL_LINES, 4 * 2 * 3, GL_UNSIGNED_BYTE, indices);
		vertexData.addIndices(indices,24);

		vertexData.setMode(OF_LINES_MODE);
		renderer->draw(vertexData);
		//vboMesh.setVertexData(&vertexData);
		//vboMesh.drawWireframe();
	}
	
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);


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
	GLboolean blend_enabled = glIsEnabled(GL_BLEND);
	GLint blend_src, blend_dst;
	glGetIntegerv( GL_BLEND_SRC, &blend_src );
	glGetIntegerv( GL_BLEND_DST, &blend_dst );
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	int len = (int)textString.length();
	float yOffset = 0;
	float fontSize = 8.0f;
	bool bOrigin = false;
	
	float sx = x;
	float sy = y - fontSize;
	
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

	if( !blend_enabled ){
		glDisable(GL_BLEND);
	}
	glBlendFunc( blend_src, blend_dst );
}


// end text
//--------------------------------------------------
