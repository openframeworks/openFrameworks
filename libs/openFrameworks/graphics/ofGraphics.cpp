#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofUtils.h"
#include "ofBaseTypes.h"
#include "ofPath.h"
#include "ofRendererCollection.h"
#if !defined(TARGET_OF_IOS) && !defined(TARGET_ANDROID) && !defined(TARGET_EMSCRIPTEN)
#include "ofCairoRenderer.h"
#endif


#ifndef TARGET_WIN32
    #define CALLBACK
#endif


//style stuff - new in 006
static ofVboMesh gradientMesh;

void ofSetCurrentRenderer(shared_ptr<ofBaseRenderer> renderer,bool setDefaults){
	if(setDefaults){
		ofStyle style = ofGetCurrentRenderer()->getStyle();
		renderer->setupGraphicDefaults();
		renderer->setStyle(style);
	}
	ofGetCurrentRenderer() = renderer;
}

#if !defined(TARGET_ANDROID) && !defined(TARGET_OF_IOS)

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
#include "ofCairoRenderer.h"
#include "ofGLRenderer.h"

static shared_ptr<ofCairoRenderer> cairoScreenshot;
static shared_ptr<ofBaseRenderer> storedRenderer;
static shared_ptr<ofRendererCollection> rendererCollection;
static bool bScreenShotStarted = false;


static void ofEndSaveScreen(){
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

static void ofBeginSaveScreen(string filename, ofCairoRenderer::Type type, bool bMultipage, bool b3D, ofRectangle viewport){
	if( bScreenShotStarted ) ofEndSaveScreen();
	
	storedRenderer = ofGetCurrentRenderer();
	
	cairoScreenshot = shared_ptr<ofCairoRenderer>(new ofCairoRenderer);
	cairoScreenshot->setup(filename, type, bMultipage, b3D, viewport);

	rendererCollection = shared_ptr<ofRendererCollection>(new ofRendererCollection);
	rendererCollection->renderers.push_back(storedRenderer);
	rendererCollection->renderers.push_back(cairoScreenshot);
	
	ofSetCurrentRenderer(rendererCollection, true);
	cairoScreenshot->background(cairoScreenshot->getStyle().bgColor);
	bScreenShotStarted = true;
}

//-----------------------------------------------------------------------------------
void ofBeginSaveScreenAsPDF(string filename, bool bMultipage, bool b3D, ofRectangle viewport){
	ofBeginSaveScreen(filename, ofCairoRenderer::PDF, bMultipage, b3D, viewport);
}

//-----------------------------------------------------------------------------------
void ofEndSaveScreenAsPDF(){
	ofEndSaveScreen();
}

//-----------------------------------------------------------------------------------
void ofBeginSaveScreenAsSVG(string filename, bool bMultipage, bool b3D, ofRectangle viewport){
	ofBeginSaveScreen(filename, ofCairoRenderer::SVG, bMultipage, b3D, viewport);
}

//-----------------------------------------------------------------------------------
void ofEndSaveScreenAsSVG(){
	ofEndSaveScreen();
}

#endif



//----------------------------------------------------------
// transformation matrix related functions

//----------------------------------------------------------
void ofPushView(){
	ofGetCurrentRenderer()->pushView();
}

//----------------------------------------------------------
void ofPopView(){
	ofGetCurrentRenderer()->popView();
}

//----------------------------------------------------------
void ofViewport(ofRectangle viewport){
	ofGetCurrentRenderer()->viewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

//----------------------------------------------------------
void ofViewport(float x, float y, float width, float height, bool invertY){
	ofGetCurrentRenderer()->viewport(x,y,width,height,invertY);
}

//----------------------------------------------------------
ofRectangle ofGetCurrentViewport(){
	return ofGetCurrentRenderer()->getCurrentViewport();
}

//----------------------------------------------------------
ofRectangle ofGetNativeViewport(){
	return ofGetCurrentRenderer()->getNativeViewport();
}

//----------------------------------------------------------
int ofGetViewportWidth(){
	return ofGetCurrentRenderer()->getViewportWidth();
}

//----------------------------------------------------------
int ofGetViewportHeight(){
	return ofGetCurrentRenderer()->getViewportHeight();
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
	return ofGetCurrentRenderer()->isVFlipped();
}

//----------------------------------------------------------
void ofSetCoordHandedness(ofHandednessType handedness){
	ofGetCurrentRenderer()->setCoordHandedness(handedness);
}

//----------------------------------------------------------
ofHandednessType ofGetCoordHandedness(){
	return ofGetCurrentRenderer()->getCoordHandedness();
}

//----------------------------------------------------------
void ofSetupScreenPerspective(float width, float height, float fov, float nearDist, float farDist){
	ofGetCurrentRenderer()->setupScreenPerspective(width,height, fov,nearDist,farDist);
}

//----------------------------------------------------------
void ofSetupScreenOrtho(float width, float height, float nearDist, float farDist){
	ofGetCurrentRenderer()->setupScreenOrtho(width,height,nearDist,farDist);
}

//----------------------------------------------------------
//Resets openGL parameters back to OF defaults
void ofSetupGraphicDefaults(){
	ofGetCurrentRenderer()->setupGraphicDefaults();
}

//----------------------------------------------------------
void ofSetupScreen(){
	ofGetCurrentRenderer()->setupScreen();	// assume defaults
}



//our openGL wrappers
//----------------------------------------------------------
void ofPushMatrix(){
	ofGetCurrentRenderer()->pushMatrix();
}

//----------------------------------------------------------
void ofPopMatrix(){
	ofGetCurrentRenderer()->popMatrix();
}

//----------------------------------------------------------
/** @brief	Queries the current OpenGL matrix state
 *  @detail Returns the specified matrix as held by the renderer's current matrix stack.
 *
 *			You can query one of the following:
 *
 *			[OF_MATRIX_MODELVIEW | OF_MATRIX_PROJECTION | OF_MATRIX_TEXTURE]
 *
 *			Each query will return the state of the matrix
 *			as it was uploaded to the shader currently bound.
 *
 *	@param	matrixMode_  Which matrix mode to query
 */
ofMatrix4x4 ofGetCurrentMatrix(ofMatrixMode matrixMode_){
	return ofGetCurrentRenderer()->getCurrentMatrix(matrixMode_);
}

//----------------------------------------------------------
ofMatrix4x4 ofGetCurrentOrientationMatrix(){
	return ofGetCurrentRenderer()->getCurrentOrientationMatrix();
}

//----------------------------------------------------------
ofMatrix4x4 ofGetCurrentNormalMatrix(){
	return ofGetCurrentRenderer()->getCurrentNormalMatrix();
}

//----------------------------------------------------------
void ofTranslate(const ofPoint& p){
	ofGetCurrentRenderer()->translate(p);
}


//----------------------------------------------------------
void ofTranslate(float x, float y, float z){
	ofGetCurrentRenderer()->translate(x, y, z);
}

//----------------------------------------------------------
void ofScale(float xAmnt, float yAmnt, float zAmnt){
	ofGetCurrentRenderer()->scale(xAmnt, yAmnt, zAmnt);
}

//----------------------------------------------------------
void ofRotate(float degrees, float vecX, float vecY, float vecZ){
	ofGetCurrentRenderer()->rotate(degrees, vecX, vecY, vecZ);
}

//----------------------------------------------------------
void ofRotateX(float degrees){
	ofGetCurrentRenderer()->rotateX(degrees);
}

//----------------------------------------------------------
void ofRotateY(float degrees){
	ofGetCurrentRenderer()->rotateY(degrees);
}

//----------------------------------------------------------
void ofRotateZ(float degrees){
	ofGetCurrentRenderer()->rotateZ(degrees);
}

//same as ofRotateZ
//----------------------------------------------------------
void ofRotate(float degrees){
	ofGetCurrentRenderer()->rotate(degrees);
}

//----------------------------------------------------------
void ofLoadIdentityMatrix (void){
	ofGetCurrentRenderer()->loadIdentityMatrix();
}

//----------------------------------------------------------
void ofLoadMatrix (const ofMatrix4x4 & m){
	ofGetCurrentRenderer()->loadMatrix(m);
}

//----------------------------------------------------------
void ofLoadMatrix (const float *m){
	ofGetCurrentRenderer()->loadMatrix(m);
}

//----------------------------------------------------------
void ofMultMatrix (const ofMatrix4x4 & m){
	ofGetCurrentRenderer()->multMatrix(m);
}

//----------------------------------------------------------
void ofMultMatrix (const float *m){
	ofGetCurrentRenderer()->multMatrix(m);
}

//----------------------------------------------------------
void ofSetMatrixMode(ofMatrixMode matrixMode){
	ofGetCurrentRenderer()->matrixMode(matrixMode);
}

void ofLoadViewMatrix(const ofMatrix4x4 & m){
	ofGetCurrentRenderer()->loadViewMatrix(m);
}

void ofMultViewMatrix(const ofMatrix4x4 & m){
	ofGetCurrentRenderer()->multViewMatrix(m);
}

ofMatrix4x4 ofGetCurrentViewMatrix(){
	return ofGetCurrentRenderer()->getCurrentViewMatrix();
}

// end transformation matrix related functions
//----------------------------------------------------------


//----------------------------------------------------------
// background functions

//----------------------------------------------------------
void ofClear(float r, float g, float b, float a){
	ofGetCurrentRenderer()->clear(r,g,b,a);
}

//----------------------------------------------------------
void ofClear(float brightness, float a){
	ofGetCurrentRenderer()->clear(brightness, brightness, brightness, a);
}

//----------------------------------------------------------
void ofClear(const ofColor & c){
	ofGetCurrentRenderer()->clear(c.r, c.g, c.b, c.a);
}

//----------------------------------------------------------
void ofClearAlpha(){
	ofGetCurrentRenderer()->clearAlpha();
}	

//----------------------------------------------------------
void ofSetBackgroundAuto(bool bAuto){
	ofGetCurrentRenderer()->setBackgroundAuto(bAuto);
}

bool ofGetBackgroundAuto(){
	return ofGetCurrentRenderer()->getBackgroundAuto();
}

//----------------------------------------------------------
bool ofbClearBg(){
	return ofGetBackgroundAuto();
}

//----------------------------------------------------------
ofColor ofGetBackground(){
	return ofGetCurrentRenderer()->getBackgroundColor();
}

//----------------------------------------------------------
ofColor ofGetBackgroundColor(){
	return ofGetCurrentRenderer()->getBackgroundColor();
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
	ofGetCurrentRenderer()->background(r,g,b,a);
}

//----------------------------------------------------------
void ofBackgroundGradient(const ofColor& start, const ofColor& end, ofGradientMode mode) {
	float w = ofGetWidth(), h = ofGetHeight();
	gradientMesh.clear();
	gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
#ifndef TARGET_EMSCRIPTEN
	#ifdef TARGET_OPENGLES
		if(ofIsGLProgrammableRenderer()) gradientMesh.setUsage(GL_STREAM_DRAW);
	#else
		gradientMesh.setUsage(GL_STREAM_DRAW);
	#endif
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
void ofSetBackgroundColorHex(int hexColor, int alpha){
	ofSetBackgroundColor ( (hexColor >> 16) & 0xff, (hexColor >> 8) & 0xff, (hexColor >> 0) & 0xff, alpha);
}

//----------------------------------------------------------
void ofSetBackgroundColor(int r, int g, int b, int a){
	ofSetBackgroundColor (ofColor(r,g,b,a));
}

//----------------------------------------------------------
void ofSetBackgroundColor(const ofColor & c){
	ofGetCurrentRenderer()->setBackgroundColor(c);
}

// end background functions
//----------------------------------------------------------




//---------------------------------------------------------------------------
// drawing modes

//----------------------------------------------------------
void  ofSetRectMode(ofRectMode mode){
	ofGetCurrentRenderer()->setRectMode(mode);
}

//----------------------------------------------------------
ofRectMode ofGetRectMode(){
    return ofGetCurrentRenderer()->getRectMode();
}

//----------------------------------------------------------
void ofNoFill(){
	ofGetCurrentRenderer()->setFillMode(OF_OUTLINE);
}

//----------------------------------------------------------
void ofFill(){
	ofGetCurrentRenderer()->setFillMode(OF_FILLED);
}

// Returns OF_FILLED or OF_OUTLINE
//----------------------------------------------------------
ofFillFlag ofGetFill(){
    return ofGetCurrentRenderer()->getFillMode();
}

//----------------------------------------------------------
void ofSetLineWidth(float lineWidth){
	ofGetCurrentRenderer()->setLineWidth(lineWidth);
}

//----------------------------------------------------------
void ofSetDepthTest(bool depthTest){
	ofGetCurrentRenderer()->setDepthTest(depthTest);
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
	ofGetCurrentRenderer()->setCurveResolution(res);
}

//----------------------------------------------------------
void ofSetCircleResolution(int res){
	ofGetCurrentRenderer()->setCircleResolution(res);
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
	ofSetColor(r,g,b,255);
}


//----------------------------------------------------------
void ofSetColor(int r, int g, int b, int a){
	ofGetCurrentRenderer()->setColor(r,g,b,a);
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
	ofGetCurrentRenderer()->setBlendMode(blendMode);
}

//----------------------------------------------------------
void ofEnablePointSprites(){
	if(ofGetCurrentRenderer()->getType()=="GL" || ofGetCurrentRenderer()->getType()=="ProgrammableGL"){
		static_cast<ofBaseGLRenderer*>(ofGetCurrentRenderer().get())->enablePointSprites();
	}
}

//----------------------------------------------------------
void ofDisablePointSprites(){
	if(ofGetCurrentRenderer()->getType()=="GL" || ofGetCurrentRenderer()->getType()=="ProgrammableGL"){
		static_cast<ofBaseGLRenderer*>(ofGetCurrentRenderer().get())->disablePointSprites();
	}
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
	ofGetCurrentRenderer()->setLineSmoothing(true);
}

//----------------------------------------------------------
void ofDisableSmoothing(){
	ofGetCurrentRenderer()->setLineSmoothing(false);
}

//----------------------------------------------------------
void ofSetPolyMode(ofPolyWindingMode mode){
	ofGetCurrentRenderer()->setPolyMode(mode);
}

//----------------------------------------
void ofEnableAntiAliasing(){
	ofGetCurrentRenderer()->enableAntiAliasing();
}

//----------------------------------------
void ofDisableAntiAliasing(){
	ofGetCurrentRenderer()->disableAntiAliasing();
}

//----------------------------------------
void ofSetDrawBitmapMode(ofDrawBitmapMode mode){
	ofGetCurrentRenderer()->setBitmapTextMode(mode);
}

//----------------------------------------------------------
void ofSetStyle(ofStyle style){
	ofGetCurrentRenderer()->setStyle(style);
}

//----------------------------------------------------------
ofStyle ofGetStyle(){
	return ofGetCurrentRenderer()->getStyle();
}

//----------------------------------------------------------
void ofPushStyle(){
	ofGetCurrentRenderer()->pushStyle();
}

//----------------------------------------------------------
void ofPopStyle(){
	ofGetCurrentRenderer()->popStyle();
}



// end drawing modes
//---------------------------------------------------------------------------




//----------------------------------------------------------
// primitives

//----------------------------------------------------------
void ofDrawTriangle(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3){
	ofDrawTriangle(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z);
}

//----------------------------------------------------------
void ofDrawTriangle(float x1,float y1,float x2,float y2,float x3, float y3){
	ofDrawTriangle(x1, y1, 0.0f, x2, y2, 0.0f, x3, y3, 0.0f);
}

//----------------------------------------------------------
void ofDrawTriangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3, float y3,float z3){
	ofGetCurrentRenderer()->drawTriangle(x1,y1,z1,x2,y2,z2,x3,y3,z3);
}

//----------------------------------------------------------
void ofDrawCircle(const ofPoint & p, float radius){
	ofDrawCircle(p.x, p.y, p.z, radius);
}

//----------------------------------------------------------
void ofDrawCircle(float x, float y, float radius){
	ofDrawCircle(x,y,0,radius);
}

//----------------------------------------------------------
void ofDrawCircle(float x, float y, float z, float radius){
	ofGetCurrentRenderer()->drawCircle(x,y,z,radius);
}

//----------------------------------------------------------
void ofDrawEllipse(const ofPoint & p, float width, float height){
	ofDrawEllipse(p.x, p.y, p.z, width, height);
}

//----------------------------------------------------------
void ofDrawEllipse(float x, float y, float width, float height){
	ofDrawEllipse(x,y,0,width,height);
}

//----------------------------------------------------------
void ofDrawEllipse(float x, float y, float z, float width, float height){
	ofGetCurrentRenderer()->drawEllipse(x,y,z,width,height);
}

//----------------------------------------------------------
void ofDrawLine(const ofPoint & p1, const ofPoint & p2){
	ofDrawLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

//----------------------------------------------------------
void ofDrawLine(float x1,float y1,float x2,float y2){
	ofDrawLine(x1, y1, 0.0f, x2, y2, 0.0f);
}

//----------------------------------------------------------
void ofDrawLine(float x1,float y1,float z1,float x2,float y2,float z2){
	ofGetCurrentRenderer()->drawLine(x1,y1,z1,x2,y2,z2);
}

//----------------------------------------------------------
void ofDrawRectangle(const ofRectangle & r){
	ofDrawRectangle(r.x,r.y,0.0f,r.width, r.height);
}

//----------------------------------------------------------
void ofDrawRectangle(const ofPoint & p,float w,float h){
	ofDrawRectangle(p.x, p.y, p.z, w, h);
}

//----------------------------------------------------------
void ofDrawRectangle(float x,float y,float w,float h){
	ofDrawRectangle(x, y, 0.0f, w, h);
}

//----------------------------------------------------------
void ofDrawRectangle(float x,float y,float z,float w,float h){
	ofGetCurrentRenderer()->drawRectangle(x,y,z,w,h);
}

//----------------------------------------------------------
void ofDrawRectRounded(const ofRectangle & b, float r){
	ofDrawRectRounded(b,r,r,r,r);
}

//----------------------------------------------------------
void ofDrawRectRounded(const ofPoint & p, float w, float h, float r){
	ofDrawRectRounded(p.x, p.y, p.z, w, h, r,r,r,r);
}

//----------------------------------------------------------
void ofDrawRectRounded(float x, float y, float w, float h, float r){
	ofDrawRectRounded(x,y,0.0f,w,h,r,r,r,r);
}

//----------------------------------------------------------
void ofDrawRectRounded(float x, float y, float z, float w, float h, float r){
	ofDrawRectRounded(x,y,z,w,h,r,r,r,r);
}

//----------------------------------------------------------
void ofDrawRectRounded(const ofPoint & p, float w, float h, float topLeftRadius,
                                                        float topRightRadius,
                                                        float bottomRightRadius,
                                                        float bottomLeftRadius){
	ofDrawRectRounded(p.x,p.y,p.z,w,h,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);
}

//----------------------------------------------------------
void ofDrawRectRounded(const ofRectangle & b, float topLeftRadius,
                                          float topRightRadius,
                                          float bottomRightRadius,
                                          float bottomLeftRadius) {

	// if the parameter is an ofRectangle we don't do rectMode
	ofDrawRectRounded(b.x,b.y,0.0f,b.width,b.height,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);
}


//----------------------------------------------------------
void ofDrawRectRounded(float x, float y, float z, float w, float h, float topLeftRadius,
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
	ofGetCurrentRenderer()->getPath().clear();
	ofGetCurrentRenderer()->getPath().rectRounded(x,y,z,w,h,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);
    ofGetCurrentRenderer()->draw(ofGetCurrentRenderer()->getPath());//.draw();

}

//----------------------------------------------------------
void ofDrawCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
	ofGetCurrentRenderer()->getPath().clear();
	ofGetCurrentRenderer()->getPath().curveTo(x0,y0);
	ofGetCurrentRenderer()->getPath().curveTo(x1,y1);
	ofGetCurrentRenderer()->getPath().curveTo(x2,y2);
	ofGetCurrentRenderer()->getPath().curveTo(x3,y3);
    ofGetCurrentRenderer()->draw(ofGetCurrentRenderer()->getPath());//.draw();
}

//----------------------------------------------------------
void ofDrawCurve(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	ofGetCurrentRenderer()->getPath().clear();
	ofGetCurrentRenderer()->getPath().curveTo(x0,y0,z0);
	ofGetCurrentRenderer()->getPath().curveTo(x1,y1,z1);
	ofGetCurrentRenderer()->getPath().curveTo(x2,y2,z2);
	ofGetCurrentRenderer()->getPath().curveTo(x3,y3,z3);
    ofGetCurrentRenderer()->draw(ofGetCurrentRenderer()->getPath());//.draw();
}


//----------------------------------------------------------
void ofDrawBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
	ofGetCurrentRenderer()->getPath().clear();
	ofGetCurrentRenderer()->getPath().moveTo(x0,y0);
	ofGetCurrentRenderer()->getPath().bezierTo(x1,y1,x2,y2,x3,y3);
    ofGetCurrentRenderer()->draw(ofGetCurrentRenderer()->getPath());//.draw();
}

//----------------------------------------------------------
void ofDrawBezier(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	ofGetCurrentRenderer()->getPath().clear();
	ofGetCurrentRenderer()->getPath().moveTo(x0,y0,z0);
	ofGetCurrentRenderer()->getPath().bezierTo(x1,y1,z1,x2,y2,z2,x3,y3,z3);
    ofGetCurrentRenderer()->draw(ofGetCurrentRenderer()->getPath());//.draw();
}

//----------------------------------------------------------
void ofTriangle(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3){
	ofDrawTriangle(p1,p2,p3);
}

//----------------------------------------------------------
void ofTriangle(float x1,float y1,float x2,float y2,float x3, float y3){
	ofDrawTriangle(x1, y1, x2, y2, x3, y3);
}

//----------------------------------------------------------
void ofTriangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3, float y3,float z3){
	ofDrawTriangle(x1,y1,z1,x2,y2,z2,x3,y3,z3);
}

//----------------------------------------------------------
void ofCircle(const ofPoint & p, float radius){
	ofDrawCircle(p, radius);
}

//----------------------------------------------------------
void ofCircle(float x, float y, float radius){
	ofDrawCircle(x,y,radius);
}

//----------------------------------------------------------
void ofCircle(float x, float y, float z, float radius){
	ofDrawCircle(x,y,z,radius);
}

//----------------------------------------------------------
void ofEllipse(const ofPoint & p, float width, float height){
	ofDrawEllipse(p, width, height);
}

//----------------------------------------------------------
void ofEllipse(float x, float y, float width, float height){
	ofDrawEllipse(x,y,width,height);
}

//----------------------------------------------------------
void ofEllipse(float x, float y, float z, float width, float height){
	ofDrawEllipse(x,y,z,width,height);
}

//----------------------------------------------------------
void ofLine(const ofPoint & p1, const ofPoint & p2){
	ofDrawLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

//----------------------------------------------------------
void ofLine(float x1,float y1,float x2,float y2){
	ofDrawLine(x1, y1, x2, y2);
}

//----------------------------------------------------------
void ofLine(float x1,float y1,float z1,float x2,float y2,float z2){
	ofDrawLine(x1,y1,z1,x2,y2,z2);
}

//----------------------------------------------------------
void ofRect(const ofRectangle & r){
	ofDrawRectangle(r.x,r.y,r.width, r.height);
}

//----------------------------------------------------------
void ofRect(const ofPoint & p,float w,float h){
	ofDrawRectangle(p, w, h);
}

//----------------------------------------------------------
void ofRect(float x,float y,float w,float h){
	ofDrawRectangle(x, y, w, h);
}

//----------------------------------------------------------
void ofRect(float x,float y,float z,float w,float h){
	ofDrawRectangle(x,y,z,w,h);
}

//----------------------------------------------------------
void ofRectRounded(const ofRectangle & b, float r){
	ofDrawRectRounded(b,r);
}

//----------------------------------------------------------
void ofRectRounded(const ofPoint & p, float w, float h, float r){
	ofDrawRectRounded(p, w, h, r);
}

//----------------------------------------------------------
void ofRectRounded(float x, float y, float w, float h, float r){
	ofDrawRectRounded(x, y, w, h, r);
}

//----------------------------------------------------------
void ofRectRounded(float x, float y, float z, float w, float h, float r){
	ofDrawRectRounded(x,y,z,w,h,r,r,r,r);
}

//----------------------------------------------------------
void ofRectRounded(const ofPoint & p, float w, float h, float topLeftRadius,
                                                        float topRightRadius,
                                                        float bottomRightRadius,
                                                        float bottomLeftRadius){
	ofDrawRectRounded(p,w,h,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);
}

//----------------------------------------------------------
void ofRectRounded(const ofRectangle & b, float topLeftRadius,
                                          float topRightRadius,
                                          float bottomRightRadius,
                                          float bottomLeftRadius) {

	// if the parameter is an ofRectangle we don't do rectMode
	ofDrawRectRounded(b,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);
}


//----------------------------------------------------------
void ofRectRounded(float x, float y, float z, float w, float h, float topLeftRadius,
                                                                float topRightRadius,
                                                                float bottomRightRadius,
                                                                float bottomLeftRadius) {

	ofDrawRectRounded(x,y,z,w,h,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);

}

//----------------------------------------------------------
void ofCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
    ofDrawCurve(x0, y0, x1, y1, x2, y2, x3, y3);
}

//----------------------------------------------------------
void ofCurve(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	ofDrawCurve(x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3);
}


//----------------------------------------------------------
void ofBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
    ofDrawBezier(x0,y0,x1,y1,x2,y2,x3,y3);
}

//----------------------------------------------------------
void ofBezier(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
    ofDrawBezier(x0,y0,z0,x1,y1,z1,x2,y2,z2,x3,y3,z3);
}

//----------------------------------------------------------
void ofBeginShape(){
	ofGetCurrentRenderer()->getPath().clear();
}

//----------------------------------------------------------
void ofVertex(float x, float y){
	ofGetCurrentRenderer()->getPath().lineTo(x,y);
}

//----------------------------------------------------------
void ofVertex(float x, float y, float z){
	ofGetCurrentRenderer()->getPath().lineTo(x,y,z);
}

//---------------------------------------------------
void ofVertex(ofPoint & p){
	ofGetCurrentRenderer()->getPath().lineTo(p);
}

//----------------------------------------------------------
void ofVertices( const vector <ofPoint> & polyPoints ){
	for( int k = 0; k < (int)polyPoints.size(); k++){
		ofGetCurrentRenderer()->getPath().lineTo(polyPoints[k]);
	}
}

//---------------------------------------------------
void ofCurveVertex(float x, float y){
	ofGetCurrentRenderer()->getPath().curveTo(x,y);
}

//---------------------------------------------------
void ofCurveVertex(float x, float y, float z){
	ofGetCurrentRenderer()->getPath().curveTo(x,y,z);
}

//----------------------------------------------------------
void ofCurveVertices( const vector <ofPoint> & curvePoints){
	for( int k = 0; k < (int)curvePoints.size(); k++){
		ofGetCurrentRenderer()->getPath().curveTo(curvePoints[k]);
	}
}

//---------------------------------------------------
void ofCurveVertex(ofPoint & p){
	ofGetCurrentRenderer()->getPath().curveTo(p);
}

//---------------------------------------------------
void ofBezierVertex(float x1, float y1, float x2, float y2, float x3, float y3){
	ofGetCurrentRenderer()->getPath().bezierTo(x1,y1,x2,y2,x3,y3);
}

void ofBezierVertex(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3){
	ofGetCurrentRenderer()->getPath().bezierTo(p1, p2, p3);
}

//---------------------------------------------------
void ofBezierVertex(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	ofGetCurrentRenderer()->getPath().bezierTo(x1,y1,z1,x2,y2,z2,x3,y3,z3);
}

//----------------------------------------------------------
void ofNextContour(bool bClose){
	if (bClose){
		ofGetCurrentRenderer()->getPath().close();
	}
	ofGetCurrentRenderer()->getPath().newSubPath();
}


//----------------------------------------------------------
void ofEndShape(bool bClose){

	// (close -> add the first point to the end)
	// -----------------------------------------------

	if (bClose){
		ofGetCurrentRenderer()->getPath().close();
	}

    ofGetCurrentRenderer()->draw(ofGetCurrentRenderer()->getPath());//.draw();

}

//--------------------------------------------------
// text
//--------------------------------------------------
template<>
void ofDrawBitmapString(const string & textString, float x, float y, float z){
	ofGetCurrentRenderer()->drawString(textString,x,y,z);
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
	
	if(ofGetStyle().drawBitmapMode == OF_BITMAPMODE_MODEL) {
		ofTranslate(x,y,0);
		ofScale(1,-1,0);
		ofTranslate(-(padding), + padding - fontSize - 2,0);
	} else {
		ofTranslate(x-(padding), y-(padding + fontSize + 2), 0);
		
	}
	
	ofDrawRectangle(0, 0, width + 2 * padding, height + 2 * padding);
	ofPopMatrix();
	ofSetColor(foreground);
	ofNoFill();
	ofDrawBitmapString(text, x, y);
	glDepthMask(true);
	ofPopStyle();
}


// end text
//--------------------------------------------------
