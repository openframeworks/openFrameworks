#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofUtils.h"
#include "ofBaseTypes.h"
#include "ofGLRenderer.h"
#include "ofPath.h"
#include "ofRendererCollection.h"
#include "ofGLProgrammableRenderer.h"
#include "ofGLRenderer.h"
#if !defined(TARGET_OF_IOS) && !defined(TARGET_ANDROID)
#include "ofCairoRenderer.h"
#endif


#ifndef TARGET_LINUX_ARM
	#ifdef TARGET_OSX
		#include <OpenGL/glu.h>
	#endif

	#ifdef TARGET_OPENGLES
		#include "glu.h"
	#endif

	#ifdef TARGET_LINUX
		#include <GL/glu.h>
	#endif

	#ifdef TARGET_WIN32
		#include "glu.h"
	#endif
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
#if defined( TARGET_LINUX ) && !defined(TARGET_OPENGLES)
	#include <GL/glut.h>
#endif


//style stuff - new in 006
static ofStyle currentStyle;
static deque <ofStyle> styleHistory;
static deque <ofRectangle> viewportHistory;

static ofPath shape;
static ofPtr<ofBaseRenderer> renderer;
static ofVboMesh gradientMesh;


void ofSetCurrentRenderer(const string & rendererType,bool setDefaults){
	if(rendererType==ofGLProgrammableRenderer::TYPE){
		ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLProgrammableRenderer),setDefaults);
	}else if(rendererType==ofGLRenderer::TYPE){
		ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer),setDefaults);
#if !defined(TARGET_OF_IOS) && !defined(TARGET_ANDROID)
	}else if(rendererType==ofCairoRenderer::TYPE){
		ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofCairoRenderer),setDefaults);
#endif
	}else{
		ofLogError("ofGraphics") << "ofSetCurrentRenderer(): unknown renderer type " << rendererType << ", setting an ofGLRenderer";
		ofLogError("ofGraphics") << "if you want to use a custom renderer, pass an ofPtr to a new instance of it";
		ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer),setDefaults);
	}
}

void ofSetCurrentRenderer(ofPtr<ofBaseRenderer> renderer_,bool setDefaults){
	renderer = renderer_;
	if(renderer->rendersPathPrimitives()){
		shape.setMode(ofPath::COMMANDS);
	}else{
		shape.setMode(ofPath::POLYLINES);
	}

	shape.setUseShapeColor(false);

	if(setDefaults){
		renderer->setupGraphicDefaults();
		ofSetStyle(currentStyle);
		ofBackground(currentStyle.bgColor);
	}
}

ofPtr<ofBaseRenderer> & ofGetCurrentRenderer(){
	return renderer;
}

#if !defined(TARGET_ANDROID) && !defined(TARGET_OF_IOS)

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
#include "ofCairoRenderer.h"
#include "ofGLRenderer.h"

static ofPtr<ofCairoRenderer> cairoScreenshot;
static ofPtr<ofBaseRenderer> storedRenderer;
static ofPtr<ofRendererCollection> rendererCollection;
static bool bScreenShotStarted = false;

//-----------------------------------------------------------------------------------
void ofBeginSaveScreenAsPDF(string filename, bool bMultipage, bool b3D, ofRectangle viewport){
	if( bScreenShotStarted )ofEndSaveScreenAsPDF();
	
	storedRenderer = ofGetCurrentRenderer();
	
	cairoScreenshot = ofPtr<ofCairoRenderer>(new ofCairoRenderer);
	cairoScreenshot->setup(filename, ofCairoRenderer::PDF, bMultipage, b3D, viewport); 		

	rendererCollection = ofPtr<ofRendererCollection>(new ofRendererCollection);
	rendererCollection->renderers.push_back(ofGetGLRenderer());
	rendererCollection->renderers.push_back(cairoScreenshot);
	
	ofSetCurrentRenderer(rendererCollection,true);
	bScreenShotStarted = true;
}

//-----------------------------------------------------------------------------------
void ofEndSaveScreenAsPDF(){
	if( bScreenShotStarted ){

		if( cairoScreenshot ){
			cairoScreenshot->close();
			rendererCollection.reset();
			cairoScreenshot.reset();
		}
		if( storedRenderer ){
			ofSetCurrentRenderer(storedRenderer,true);
			storedRenderer.reset();
		}
		
		bScreenShotStarted = false;
	}
}

#endif



//----------------------------------------------------------
// transformation matrix related functions

//----------------------------------------------------------
void ofPushView(){
	renderer->pushView();
}

//----------------------------------------------------------
void ofPopView(){
	renderer->popView();
}

//----------------------------------------------------------
void ofViewport(ofRectangle viewport){
	renderer->viewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

//----------------------------------------------------------
void ofViewport(float x, float y, float width, float height, bool invertY){
	renderer->viewport(x,y,width,height,invertY);
}

//----------------------------------------------------------
ofRectangle ofGetCurrentViewport(){
	return renderer->getCurrentViewport();
}

//----------------------------------------------------------
ofRectangle ofGetNativeViewport(){
	return renderer->getNativeViewport();
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
int ofOrientationToDegrees(ofOrientation orientation){
	switch(orientation){
	case OF_ORIENTATION_DEFAULT:
		return 0;
	case OF_ORIENTATION_180:
		return 180;
	case OF_ORIENTATION_90_RIGHT:
		return 270;
	case OF_ORIENTATION_90_LEFT:
		return 90;
	default:
		return 0;
	}
}

//----------------------------------------------------------
bool ofIsVFlipped(){
	return renderer->isVFlipped();
}

//----------------------------------------------------------
void ofSetCoordHandedness(ofHandednessType handedness){
	renderer->setCoordHandedness(handedness);
}

//----------------------------------------------------------
ofHandednessType ofGetCoordHandedness(){
	return renderer->getCoordHandedness();
}


static bool setupScreenDeprecated=false;

//----------------------------------------------------------
void ofSetupScreenPerspective(float width, float height, ofOrientation orientation, bool vFlip, float fov, float nearDist, float farDist){
	if(!setupScreenDeprecated){
		ofLogError("ofGraphics") << "ofSetupScreenPerspective() with orientation and vflip is deprecated,";
		ofLogError("ofGraphics") << "set them with ofSetOrientation() before calling ofSetupScreenPerspective()";
		setupScreenDeprecated = true;
	}
	renderer->setupScreenPerspective(width,height,fov,nearDist,farDist);
}

//----------------------------------------------------------
void ofSetupScreenOrtho(float width, float height, ofOrientation orientation, bool vFlip, float nearDist, float farDist){
	if(!setupScreenDeprecated){
		ofLogError("ofGraphics") << "ofSetupScreenOrtho() with orientation and vflip is deprecated,";
		ofLogError("ofGraphics") << "set them with ofSetOrientation() before calling ofSetupScreenPerspective()";
		setupScreenDeprecated = true;
	}
	renderer->setupScreenOrtho(width,height,nearDist,farDist);
}

//----------------------------------------------------------
void ofSetupScreenPerspective(float width, float height, float fov, float nearDist, float farDist){
	renderer->setupScreenPerspective(width,height, fov,nearDist,farDist);
}

//----------------------------------------------------------
void ofSetupScreenOrtho(float width, float height, float nearDist, float farDist){
	renderer->setupScreenOrtho(width,height,nearDist,farDist);
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

//----------------------------------------------------------
void ofLoadIdentityMatrix (void){
	renderer->loadIdentityMatrix();
}

//----------------------------------------------------------
void ofLoadMatrix (const ofMatrix4x4 & m){
	renderer->loadMatrix(m);
}

//----------------------------------------------------------
void ofLoadMatrix (const float *m){
	renderer->loadMatrix(m);
}

//----------------------------------------------------------
void ofMultMatrix (const ofMatrix4x4 & m){
	renderer->multMatrix(m);
}

//----------------------------------------------------------
void ofMultMatrix (const float *m){
	renderer->multMatrix(m);
}

//----------------------------------------------------------
void ofSetMatrixMode(ofMatrixMode matrixMode){
	renderer->matrixMode(matrixMode);
}

// end transformation matrix related functions
//----------------------------------------------------------


//----------------------------------------------------------
// background functions

//----------------------------------------------------------
void ofClear(float r, float g, float b, float a){
	renderer->clear(r,g,b,a);
}

//----------------------------------------------------------
void ofClear(float brightness, float a){
	renderer->clear(brightness, brightness, brightness, a);
}

//----------------------------------------------------------
void ofClear(const ofColor & c){
	renderer->clear(c.r, c.g, c.b, c.a);
}

//----------------------------------------------------------
void ofClearAlpha(){
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
ofColor ofGetBackground(){
	return ofColor(renderer->getBgColor());
}

//----------------------------------------------------------
void ofBackground(int brightness, int alpha){
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

//----------------------------------------------------------
void ofBackgroundGradient(const ofColor& start, const ofColor& end, ofGradientMode mode) {
	float w = ofGetWidth(), h = ofGetHeight();
	gradientMesh.clear();
	gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
#ifdef TARGET_OPENGLES
	if(ofIsGLProgrammableRenderer()) gradientMesh.setUsage(GL_STREAM_DRAW);
#else
	gradientMesh.setUsage(GL_STREAM_DRAW);
#endif
	if(mode == OF_GRADIENT_CIRCULAR) {
		// this could be optimized by building a single mesh once, then copying
		// it and just adding the colors whenever the function is called.
		ofVec2f center(w / 2, h / 2);
		gradientMesh.addVertex(center);
		gradientMesh.addColor(start);
		int n = 32; // circular gradient resolution
		float angleBisector = TWO_PI / (n * 2);
		float smallRadius = ofDist(0, 0, w / 2, h / 2);
		float bigRadius = smallRadius / cos(angleBisector);
		for(int i = 0; i <= n; i++) {
			float theta = i * TWO_PI / n;
			gradientMesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
			gradientMesh.addColor(end);
		}
	} else if(mode == OF_GRADIENT_LINEAR) {
		gradientMesh.addVertex(ofVec2f(0, 0));
		gradientMesh.addVertex(ofVec2f(w, 0));
		gradientMesh.addVertex(ofVec2f(w, h));
		gradientMesh.addVertex(ofVec2f(0, h));
		gradientMesh.addColor(start);
		gradientMesh.addColor(start);
		gradientMesh.addColor(end);
		gradientMesh.addColor(end);
	} else if(mode == OF_GRADIENT_BAR) {
		gradientMesh.addVertex(ofVec2f(w / 2, h / 2));
		gradientMesh.addVertex(ofVec2f(0, h / 2));
		gradientMesh.addVertex(ofVec2f(0, 0));
		gradientMesh.addVertex(ofVec2f(w, 0));
		gradientMesh.addVertex(ofVec2f(w, h / 2));
		gradientMesh.addVertex(ofVec2f(w, h));
		gradientMesh.addVertex(ofVec2f(0, h));
		gradientMesh.addVertex(ofVec2f(0, h / 2));
		gradientMesh.addColor(start);
		gradientMesh.addColor(start);
		gradientMesh.addColor(end);
		gradientMesh.addColor(end);
		gradientMesh.addColor(start);
		gradientMesh.addColor(end);
		gradientMesh.addColor(end);
		gradientMesh.addColor(start);
	}
	GLboolean depthMaskEnabled;
	glGetBooleanv(GL_DEPTH_WRITEMASK,&depthMaskEnabled);
	glDepthMask(GL_FALSE);
	gradientMesh.draw();
	if(depthMaskEnabled){
		glDepthMask(GL_TRUE);
	}
}

//----------------------------------------------------------
void ofSetBackgroundColor(int brightness, int alpha){
	ofSetBackgroundColor(brightness, brightness, brightness, alpha);
}

//----------------------------------------------------------
void ofSetBackgroundColor(const ofColor & c){
	ofSetBackgroundColor ( c.r, c.g, c.b, c.a);
}

//----------------------------------------------------------
void ofSetBackgroundColorHex(int hexColor, int alpha){
	ofSetBackgroundColor ( (hexColor >> 16) & 0xff, (hexColor >> 8) & 0xff, (hexColor >> 0) & 0xff, alpha);
}

//----------------------------------------------------------
void ofSetBackgroundColor(int r, int g, int b, int a){
	currentStyle.bgColor.set(r,g,b,a);
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
    return currentStyle.rectMode;
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
    return currentStyle.bFill ? OF_FILLED : OF_OUTLINE;
}

//----------------------------------------------------------
void ofSetLineWidth(float lineWidth){
	shape.setStrokeWidth(lineWidth);
	renderer->setLineWidth(lineWidth);
	currentStyle.lineWidth = lineWidth;
}

//----------------------------------------------------------
void ofSetDepthTest(bool depthTest){
	renderer->setDepthTest(depthTest);
	//currentStyle.depthTest = depthTest;
}

//----------------------------------------------------------
void ofEnableDepthTest(){
	ofSetDepthTest(true);
}

//----------------------------------------------------------
void ofDisableDepthTest(){
	ofSetDepthTest(false);
}

//----------------------------------------
void ofSetCurveResolution(int res){
	shape.setCurveResolution(res);
	currentStyle.curveResolution = res;
}

//----------------------------------------------------------
void ofSetCircleResolution(int res){
	renderer->setCircleResolution(res);
	currentStyle.circleResolution = res;
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
		ofLogWarning("ofGraphics") << "ofSetColor(): gray value > 255, perhaps you want ofSetHexColor(int hexColor) instead?";
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
void ofEnablePointSprites(){
	renderer->enablePointSprites();
}

//----------------------------------------------------------
void ofDisablePointSprites(){
	renderer->disablePointSprites();
}

//----------------------------------------------------------
void ofDisableBlendMode(){
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
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
void ofEnableAntiAliasing(){
	renderer->enableAntiAliasing();
}

//----------------------------------------
void ofDisableAntiAliasing(){
	renderer->disableAntiAliasing();
}

//----------------------------------------
void ofSetDrawBitmapMode(ofDrawBitmapMode mode){
	currentStyle.drawBitmapMode = mode;
}

//----------------------------------------------------------
void ofSetStyle(ofStyle style){
	//color
	ofSetColor((int)style.color.r, (int)style.color.g, (int)style.color.b, (int)style.color.a);

	//bg color
	ofSetBackgroundColor(style.bgColor);

	//circle resolution - don't worry it only recalculates the display list if the res has changed
	ofSetCircleResolution(style.circleResolution);

	//ofSetSphereResolution(style.sphereResolution);

	ofSetCurveResolution(style.curveResolution);

	//line width - finally!
	ofSetLineWidth(style.lineWidth);
	
	//ofSetDepthTest(style.depthTest); removed since it'll break old projects setting depth test through glEnable

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
	ofEnableBlendMode(style.blendingMode);
	
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
		//ofLogWarning("ofGraphics") "ofPushStyle(): maximum number of style pushes << " OF_MAX_STYLE_HISTORY << " reached, did you forget to pop somewhere?"
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
	ofRect(r.x,r.y,0.0f,r.width, r.height);
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
void ofRectRounded(const ofRectangle & b, float r){
	ofRectRounded(b,r,r,r,r);
}

//----------------------------------------------------------
void ofRectRounded(const ofPoint & p, float w, float h, float r){
	ofRectRounded(p.x, p.y, p.z, w, h, r,r,r,r);
}

//----------------------------------------------------------
void ofRectRounded(float x, float y, float w, float h, float r){
	ofRectRounded(x, y, 0.0f, w, h, r,r,r,r);
}

//----------------------------------------------------------
void ofRectRounded(const ofPoint & p, float w, float h, float topLeftRadius,
                                                        float topRightRadius,
                                                        float bottomRightRadius,
                                                        float bottomLeftRadius){
    ofRectRounded(p.x,p.y,p.z,w,h,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);
}

//----------------------------------------------------------
void ofRectRounded(const ofRectangle & b, float topLeftRadius,
                                          float topRightRadius,
                                          float bottomRightRadius,
                                          float bottomLeftRadius) {

	// if the parameter is an ofRectangle we don't do rectMode
	ofRectRounded(b.x,b.y,0.0f,b.width,b.height,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);
}


//----------------------------------------------------------
void ofRectRounded(float x, float y, float z, float w, float h, float topLeftRadius,
                                                                float topRightRadius,
                                                                float bottomRightRadius,
                                                                float bottomLeftRadius) {
	// respect the current rectmode
	switch (ofGetRectMode()) {
		case OF_RECTMODE_CENTER:
			x -= w / 2.0f;
			y -= h / 2.0f;
			break;
		default:
			break;
	}
	shape.clear();
    shape.rectRounded(x,y,z,w,h,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);
    shape.draw();

}

//----------------------------------------------------------
void ofCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
    shape.setCurveResolution(currentStyle.curveResolution);
    shape.clear();
	shape.curveTo(x0,y0);
	shape.curveTo(x1,y1);
	shape.curveTo(x2,y2);
	shape.curveTo(x3,y3);
	shape.draw();
}

//----------------------------------------------------------
void ofCurve(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
    shape.setCurveResolution(currentStyle.curveResolution);
	shape.clear();
	shape.curveTo(x0,y0,z0);
	shape.curveTo(x1,y1,z1);
	shape.curveTo(x2,y2,z2);
	shape.curveTo(x3,y3,z3);
	shape.draw();
}


//----------------------------------------------------------
void ofBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
    shape.setCurveResolution(currentStyle.curveResolution);
	shape.clear();
	shape.moveTo(x0,y0);
	shape.bezierTo(x1,y1,x2,y2,x3,y3);
	shape.draw();
}

//----------------------------------------------------------
void ofBezier(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
    shape.setCurveResolution(currentStyle.curveResolution);
	shape.clear();
	shape.moveTo(x0,y0,z0);
	shape.bezierTo(x1,y1,z1,x2,y2,z2,x3,y3,z3);
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

//----------------------------------------------------------
void ofVertex(float x, float y, float z){
	shape.lineTo(x,y,z);
}

//---------------------------------------------------
void ofVertex(ofPoint & p){
	shape.lineTo(p);
}

//----------------------------------------------------------
void ofVertices( const vector <ofPoint> & polyPoints ){
	for( int k = 0; k < (int)polyPoints.size(); k++){
		shape.lineTo(polyPoints[k]);
	}
}

//---------------------------------------------------
void ofCurveVertex(float x, float y){
    shape.setCurveResolution(currentStyle.curveResolution);
    shape.curveTo(x,y);
}

//---------------------------------------------------
void ofCurveVertex(float x, float y, float z){
	shape.curveTo(x,y,z);
}

//----------------------------------------------------------
void ofCurveVertices( const vector <ofPoint> & curvePoints){
    shape.setCurveResolution(currentStyle.curveResolution);
	for( int k = 0; k < (int)curvePoints.size(); k++){
		shape.curveTo(curvePoints[k]);
	}
}

//---------------------------------------------------
void ofCurveVertex(ofPoint & p){
    shape.setCurveResolution(currentStyle.curveResolution);
	shape.curveTo(p);
}

//---------------------------------------------------
void ofBezierVertex(float x1, float y1, float x2, float y2, float x3, float y3){
    shape.setCurveResolution(currentStyle.curveResolution);
	shape.bezierTo(x1,y1,x2,y2,x3,y3);
}

void ofBezierVertex(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3){
    shape.setCurveResolution(currentStyle.curveResolution);
	shape.bezierTo(p1, p2, p3);
}

//---------------------------------------------------
void ofBezierVertex(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
    shape.setCurveResolution(currentStyle.curveResolution);
	shape.bezierTo(x1,y1,z1,x2,y2,z2,x3,y3,z3);
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
//--------------------------------------------------
void ofDrawBitmapStringHighlight(string text, const ofPoint& position, const ofColor& background, const ofColor& foreground) {
	ofDrawBitmapStringHighlight(text, position.x, position.y, background, foreground);
}
//--------------------------------------------------
void ofDrawBitmapStringHighlight(string text, int x, int y, const ofColor& background, const ofColor& foreground) {
	vector<string> lines = ofSplitString(text, "\n");
	int maxLineLength = 0;
	for(int i = 0; i < (int)lines.size(); i++) {
		// tabs are not rendered
		const string & line(lines[i]);
		int currentLineLength = 0;
		for(int j = 0; j < (int)line.size(); j++) {
			if (line[j] == '\t') {
				currentLineLength += 8 - (currentLineLength % 8);
			} else {
				currentLineLength++;
			}
		}
		maxLineLength = MAX(maxLineLength, currentLineLength);
	}
	
	int padding = 4;
	int fontSize = 8;
	float leading = 1.7;
	int height = lines.size() * fontSize * leading - 1;
	int width = maxLineLength * fontSize;
	
	ofPushStyle();
	glDepthMask(false);
	ofSetColor(background);
	ofFill();
	ofPushMatrix();
	
	if(currentStyle.drawBitmapMode == OF_BITMAPMODE_MODEL) {
		ofTranslate(x,y,0);
		ofScale(1,-1,0);
		ofTranslate(-(padding), + padding - fontSize - 2,0);
	} else {
		ofTranslate(x-(padding), y-(padding + fontSize + 2), 0);
		
	}
	
	ofRect(0, 0, width + 2 * padding, height + 2 * padding);
	ofPopMatrix();
	ofSetColor(foreground);
	ofNoFill();
	ofDrawBitmapString(text, x, y);
	glDepthMask(true);
	ofPopStyle();
}


// end text
//--------------------------------------------------
