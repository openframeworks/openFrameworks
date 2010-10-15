#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofBitmapFont.h"

#ifdef TARGET_OSX
	#include <OpenGL/glu.h>
#endif

#ifdef TARGET_OPENGLES
	#include "glu.h"
#endif

#ifdef TARGET_LINUX
	#include "GL/glu.h"
#endif

#ifdef TARGET_WIN32
	#include "glu.h"
#endif

#ifndef TARGET_WIN32
    #define CALLBACK
#endif

#include <vector>

//----------------------------------------------------------
// static
static float	drawMode			= OF_FILLED;
static bool 	bSetupCircle		= false;
static int		numCirclePts		= 0;
float 			bgColor[4]			= {0,0,0,0};
void 			setupCircle();
bool 			bSmoothHinted		= false;
bool			bUsingArbTex		= true;
bool			bUsingNormalizedTexCoords = false;
bool 			bBakgroundAuto		= true;
int 			cornerMode			= OF_RECTMODE_CORNER;
int 			polyMode			= OF_POLY_WINDING_ODD;

int				curveResolution = 20;

//style stuff - new in 006
ofStyle			currentStyle;
vector <ofStyle> styleHistory;

static float circlePts[OF_MAX_CIRCLE_PTS*2];
static float circlePtsScaled[OF_MAX_CIRCLE_PTS*2];
static float trianglePoints[6];
static float linePoints[4];
static float rectPoints[8];

//----------------------------------------------------------
void  ofSetRectMode(int mode){
	if (mode == OF_RECTMODE_CORNER) 		cornerMode = OF_RECTMODE_CORNER;
	else if (mode == OF_RECTMODE_CENTER) 	cornerMode = OF_RECTMODE_CENTER;

	currentStyle.rectMode = cornerMode;
}

//----------------------------------------------------------
int ofGetRectMode(){
	return 	cornerMode;
}

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
	if (ofGetUsingArbTex() && GLEE_ARB_texture_rectangle){
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
	if (ofGetUsingArbTex() && GLEE_ARB_texture_rectangle){
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
void ofBackground(int r, int g, int b){
	bgColor[0] = (float)r / (float)255.0f;
	bgColor[1] = (float)g / (float)255.0f;
	bgColor[2] = (float)b / (float)255.0f;
	bgColor[3] = 1.0f;
	// if we are in not-auto mode, then clear with a bg call...
	if (ofbClearBg() == false){
		glClearColor(bgColor[0],bgColor[1],bgColor[2], bgColor[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
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

//----------------------------------------------------------
void ofSetLineWidth(float lineWidth){
	glLineWidth(lineWidth);
	currentStyle.lineWidth = lineWidth;
}

void ofSetCurveResolution(int res){
	curveResolution = res;
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
	res = MIN( MAX(1, res), OF_MAX_CIRCLE_PTS);

	if (res > 1 && res != numCirclePts){
		numCirclePts = res;
		currentStyle.circleResolution = numCirclePts;

		float angle = 0.0f;
		float angleAdder = M_TWO_PI / (float)res;
		int k = 0;
		for (int i = 0; i < numCirclePts; i++){
			circlePts[k] = cos(angle);
			circlePts[k+1] = sin(angle);
			angle += angleAdder;
			k+=2;
		}
		bSetupCircle = true;
	}
}


//----------------------------------------------------------
void ofTriangle(float x1,float y1,float x2,float y2,float x3, float y3){

	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	// draw:
	trianglePoints[0] = x1;
	trianglePoints[1] = y1;
	trianglePoints[2] = x2;
	trianglePoints[3] = y2;
	trianglePoints[4] = x3;
	trianglePoints[5] = y3;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &trianglePoints[0]);
	glDrawArrays((drawMode == OF_FILLED) ? GL_TRIANGLES : GL_LINE_LOOP, 0, 3);

	// back to normal, if smoothness is on
	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofCircle(float x,float y, float radius){

	if (!bSetupCircle) setupCircle();

	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	int k = 0;
	for(int i = 0; i < numCirclePts; i++){
		circlePtsScaled[k]   = x + circlePts[k] * radius;
		circlePtsScaled[k+1] = y + circlePts[k+1] * radius;
		k+=2;
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &circlePtsScaled[0]);
	glDrawArrays( (drawMode == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, numCirclePts);

	// back to normal, if smoothness is on
	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();

}

//----------------------------------------------------------
void ofEllipse(float x, float y, float width, float height){

	if (!bSetupCircle) setupCircle();

	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	int k = 0;
	for(int i = 0; i < numCirclePts; i++){
		circlePtsScaled[k]   = x + circlePts[k] * width  * 0.5;
		circlePtsScaled[k+1] = y + circlePts[k+1] * height * 0.5;
		k+=2;
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &circlePtsScaled[0]);
	glDrawArrays( (drawMode == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, numCirclePts);

	// back to normal, if smoothness is on
	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofLine(float x1,float y1,float x2,float y2){

	// use smoothness, if requested:
	if (bSmoothHinted) startSmoothing();

	linePoints[0] = x1;
	linePoints[1] = y1;
	linePoints[2] = x2;
	linePoints[3] = y2;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &linePoints[0]);
	glDrawArrays(GL_LINES, 0, 2);

	// back to normal, if smoothness is on
	if (bSmoothHinted) endSmoothing();

}

//----------------------------------------------------------
void ofRect(float x,float y,float w,float h){

	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	if (cornerMode == OF_RECTMODE_CORNER){
		rectPoints[0] = x;
		rectPoints[1] = y;

		rectPoints[2] = x+w;
		rectPoints[3] = y;

		rectPoints[4] = x+w;
		rectPoints[5] = y+h;

		rectPoints[6] = x;
		rectPoints[7] = y+h;
	}else{
		rectPoints[0] = x-w/2;
		rectPoints[1] = y-h/2;

		rectPoints[2] = x+w/2;
		rectPoints[3] = y-h/2;

		rectPoints[4] = x+w/2;
		rectPoints[5] = y+h/2;

		rectPoints[6] = x-w/2;
		rectPoints[7] = y+h/2;
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &rectPoints[0]);
	glDrawArrays((drawMode == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, 4);


	// use smoothness, if requested:
	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();
}


//----------------------------------------------------------
void ofCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){

	int resolution = curveResolution;

	float t,t2,t3;
	float x,y;

	ofBeginShape();

	for (int i = 0; i < resolution; i++){

		t 	=  (float)i / (float)(resolution-1);
		t2 	= t * t;
		t3 	= t2 * t;

		x = 0.5f * ( ( 2.0f * x1 ) +
		( -x0 + x2 ) * t +
		( 2.0f * x0 - 5.0f * x1 + 4 * x2 - x3 ) * t2 +
		( -x0 + 3.0f * x1 - 3.0f * x2 + x3 ) * t3 );

		y = 0.5f * ( ( 2.0f * y1 ) +
		( -y0 + y2 ) * t +
		( 2.0f * y0 - 5.0f * y1 + 4 * y2 - y3 ) * t2 +
		( -y0 + 3.0f * y1 - 3.0f * y2 + y3 ) * t3 );

		ofVertex(x,y);
	}

	ofEndShape();
}


//----------------------------------------------------------
void ofBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){

	float   ax, bx, cx;
    float   ay, by, cy;
    float   t, t2, t3;
    float   x, y;

    // polynomial coefficients
    cx = 3.0f * (x1 - x0);
    bx = 3.0f * (x2 - x1) - cx;
    ax = x3 - x0 - cx - bx;

    cy = 3.0f * (y1 - y0);
    by = 3.0f * (y2 - y1) - cy;
    ay = y3 - y0 - cy - by;


    int resolution = curveResolution;

    ofBeginShape();
    for (int i = 0; i < resolution; i++){
    	t 	=  (float)i / (float)(resolution-1);
    	t2 = t * t;
    	t3 = t2 * t;
		x = (ax * t3) + (bx * t2) + (cx * t) + x0;
    	y = (ay * t3) + (by * t2) + (cy * t) + y0;
    	ofVertex(x,y);
    }
    ofEndShape();
}

//----------------------------------------------------------
void ofSetColor(int _r, int _g, int _b){
	float r = (float)_r / 255.0f; r = MAX(0,MIN(r,1.0f));
	float g = (float)_g / 255.0f; g = MAX(0,MIN(g,1.0f));
	float b = (float)_b / 255.0f; b = MAX(0,MIN(b,1.0f));

	currentStyle.color.r = r * 255.0;
	currentStyle.color.g = g * 255.0;
	currentStyle.color.b = b * 255.0;
	currentStyle.color.a = 255;

	glColor4f(r,g,b,1);
}


//----------------------------------------------------------
void ofSetColor(int _r, int _g, int _b, int _a){
	float r = (float)_r / 255.0f; r = MAX(0,MIN(r,1.0f));
	float g = (float)_g / 255.0f; g = MAX(0,MIN(g,1.0f));
	float b = (float)_b / 255.0f; b = MAX(0,MIN(b,1.0f));
	float a = (float)_a / 255.0f; a = MAX(0,MIN(a,1.0f));

	currentStyle.color.r = r * 255.0;
	currentStyle.color.g = g * 255.0;
	currentStyle.color.b = b * 255.0;
	currentStyle.color.a = a * 255.0;

	glColor4f(r,g,b,a);
}

//----------------------------------------------------------
void ofSetColor(int hexColor){
	int r = (hexColor >> 16) & 0xff;
	int g = (hexColor >> 8) & 0xff;
	int b = (hexColor >> 0) & 0xff;
	ofSetColor(r,g,b);
}

//----------------------------------------------------------
void ofEnableAlphaBlending(){
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	currentStyle.blending = 1;
}

//----------------------------------------------------------
void ofDisableAlphaBlending(){
	glDisable(GL_BLEND);
	currentStyle.blending = 0;
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
	styleHistory.insert(styleHistory.begin(), currentStyle);

	//if we are over the max number of styles we have set, then delete the oldest styles.
	if( styleHistory.size() > OF_MAX_STYLE_HISTORY ){
		styleHistory.erase(styleHistory.begin() + OF_MAX_STYLE_HISTORY, styleHistory.end());
		//should we warn here?
		//ofLog(OF_LOG_WARNING, "ofPushStyle - warning: you have used ofPushStyle more than %i times without calling ofPopStyle - check your code!", OF_MAX_STYLE_HISTORY);
	}
}

//----------------------------------------------------------
void ofPopStyle(){
	if( styleHistory.size() ){
		ofSetStyle(styleHistory[0]);
		styleHistory.erase(styleHistory.begin(), styleHistory.begin()+1);
	}
}


//our openGL wrappers
//----------------------------------------------------------
void ofPushMatrix(){
	glPushMatrix();
}

//----------------------------------------------------------
void ofPopMatrix(){
	glPopMatrix();
}

//----------------------------------------------------------
void ofTranslate(float x, float y, float z){
	glTranslatef(x, y, z);
}

//----------------------------------------------------------
void ofScale(float xAmnt, float yAmnt, float zAmnt){
	glScalef(xAmnt, yAmnt, zAmnt);
}

//----------------------------------------------------------
void ofRotate(float degrees, float vecX, float vecY, float vecZ){
	glRotatef(degrees, vecX, vecY, vecZ);
}

//----------------------------------------------------------
void ofRotateX(float degrees){
	glRotatef(degrees, 1, 0, 0);
}

//----------------------------------------------------------
void ofRotateY(float degrees){
	glRotatef(degrees, 0, 1, 0);
}

//----------------------------------------------------------
void ofRotateZ(float degrees){
	glRotatef(degrees, 0, 0, 1);
}

//same as ofRotateZ
//----------------------------------------------------------
void ofRotate(float degrees){
	glRotatef(degrees, 0, 0, 1);
}


//--------------------------------------------------
void ofDrawBitmapString(string textString, float x, float y){

#ifndef TARGET_OPENGLES	// temp for now, until is ported from existing iphone implementations

    glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );
    glPixelStorei( GL_UNPACK_SWAP_BYTES,  GL_FALSE );
    glPixelStorei( GL_UNPACK_LSB_FIRST,   GL_FALSE );
    glPixelStorei( GL_UNPACK_ROW_LENGTH,  0        );
    glPixelStorei( GL_UNPACK_SKIP_ROWS,   0        );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0        );
    glPixelStorei( GL_UNPACK_ALIGNMENT,   1        );

	int len = (int)textString.length();
	float yOffset = 0;
	float fontSize = 8.0f;
	glRasterPos2f(x,y);
	bool bOrigin = false;
	for(int c = 0; c < len; c++)
	{
		if(textString[c] == '\n')
		{

			yOffset += bOrigin ? -1 : 1 * (fontSize*1.7);
			glRasterPos2f(x,y + (int)yOffset);
		} else if (textString[c] >= 32){
			// < 32 = control characters - don't draw
			// solves a bug with control characters
			// getting drawn when they ought to not be
			ofDrawBitmapCharacter(textString[c]);
			//ofDrawBitmapCharacter(textString[c], x + (c * 8), y);
		}
	}

	glPopClientAttrib( );
#else 
	
	// this is copied from the ofTrueTypeFont
	GLboolean blend_enabled = glIsEnabled(GL_BLEND);
	GLint blend_src, blend_dst;
	glGetIntegerv( GL_BLEND_SRC, &blend_src );
	glGetIntegerv( GL_BLEND_DST, &blend_dst );
	
    	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// (c) enable texture once before we start drawing each char (no point turning it on and off constantly)
	
	
	int len = (int)textString.length();
	float yOffset = 0;
	float fontSize = 8.0f;
	bool bOrigin = false;
	
	float sx = x;
	float sy = y - fontSize;
	
	for(int c = 0; c < len; c++)
	{
		if(textString[c] == '\n')
		{
			
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
	
	if( !blend_enabled )
		glDisable(GL_BLEND);
	glBlendFunc( blend_src, blend_dst );
	
	
#endif
}


//----------------------------------------------------------
//Resets openGL parameters back to OF defaults
void ofSetupGraphicDefaults(){

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	ofDisableSmoothing();
	ofEnableAlphaBlending();
	ofBackground(200, 200, 200);
	ofSetColor(255, 255, 255, 255);
	
}

//----------------------------------------------------------
void ofSetupScreen(){
	int w, h;

	w = ofGetWidth();
	h = ofGetHeight();

	float halfFov, theTan, screenFov, aspect;
	screenFov 		= 60.0f;

	float eyeX 		= (float)w / 2.0;
	float eyeY 		= (float)h / 2.0;
	halfFov 		= PI * screenFov / 360.0;
	theTan 			= tanf(halfFov);
	float dist 		= eyeY / theTan;
	float nearDist 	= dist / 10.0;	// near / far clip plane
	float farDist 	= dist * 10.0;
	aspect 			= (float)w/(float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(screenFov, aspect, nearDist, farDist);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);

	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
  	glTranslatef(0, -h, 0);       // shift origin up to upper-left corner.
}


//-------------- polygons ----------------------------------
//
// to do polygons, we need tesselation
// to do tesselation, we need glu and callbacks....
// ------------------------------------
// one of the callbacks creates new vertices (on intersections, etc),
// and there is a potential for memory leaks
// if we don't clean up properly
// ------------------------------------
// also the easiest system, using beginShape
// vertex(), endShape, will also use dynamically
// allocated memory
// ------------------------------------
// so, therefore, we will be using a static vector here
// for two things:
//
// a) collecting vertices
// b) new vertices on combine callback
//
// important note!
//
// this assumes single threaded polygon creation
// you can have big problems if creating polygons in
// multiple threads... please be careful
//
// (but also be aware that alot of opengl code
// is single threaded anyway, so you will have problems
// with many things opengl related across threads)
//
// ------------------------------------
// (note: this implementation is based on code from ftgl)
// ------------------------------------

//---------------------------- for combine callback:
std::vector <double*> newVectrices;
std::vector <float> tessVertices;

//---------------------------- store all the polygon vertices:
std::vector <double*> polyVertices;
//---------------------------- and for curve vertexes, since we need 4 to make a curve
std::vector <double*> curveVertices;

static int currentStartVertex = 0;

// what is the starting vertex of the shape we are drawing
// this allows multi contour polygons;

static GLUtesselator * tobj = NULL;
//static bool tessInited = false;
//static GLdouble point[3];
static GLint shapeType;

void CALLBACK tessError(GLenum);
void CALLBACK tessVertex( void* data);
void CALLBACK tessCombine( GLdouble coords[3], void* vertex_data[4], GLfloat weight[4], void** outData);
void clearTessVertices();
void clearCurveVertices();

//----------------------------------------------------------
void CALLBACK tessError(GLenum errCode){
	const GLubyte* estring;
	estring = gluErrorString( errCode);
	ofLog(OF_LOG_ERROR, "tessError: %s", estring);
}


//----------------------------------------------------------
void CALLBACK tessBegin(GLint type){
	shapeType = type;
	tessVertices.clear();
}

//----------------------------------------------------------
void CALLBACK tessEnd(){
	//we draw as 3d not 2d: change 3s bellow to 2 and comment the 3rd push_back in tessVertex to do 2D

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &tessVertices[0]);
	glDrawArrays(shapeType, 0, tessVertices.size()/3);
	tessVertices.clear();
}


//----------------------------------------------------------
void CALLBACK tessVertex( void* data){

	tessVertices.push_back( ( (double *)data)[0] );
	tessVertices.push_back( ( (double *)data)[1] );
	tessVertices.push_back( ( (double *)data)[2] );	//No need for z for now?
}


//----------------------------------------------------------
void CALLBACK tessCombine( GLdouble coords[3], void* vertex_data[4], GLfloat weight[4], void** outData){
    double* vertex = new double[3];
    newVectrices.push_back(vertex);
    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];
    *outData = vertex;
}

//----------------------------------------------------------
void clearTessVertices(){
	// -------------------------------------------------
    // ---------------- delete newly created vertices !
     for(vector<double*>::iterator itr=polyVertices.begin();
        itr!=polyVertices.end();
        ++itr){
        delete [] (*itr);
    }
    polyVertices.clear();

    // combine callback also makes new vertices, let's clear them:
    for(vector<double*>::iterator itr=newVectrices.begin();
        itr!=newVectrices.end();
        ++itr){
        delete [] (*itr);
    }
    newVectrices.clear();
    // -------------------------------------------------

    clearCurveVertices();
    currentStartVertex = 0;
}

//----------------------------------------------------------
void clearCurveVertices(){
	// combine callback also makes new vertices, let's clear them:
    for(vector<double*>::iterator itr=curveVertices.begin();
        itr!=curveVertices.end();
        ++itr){
        delete [] (*itr);
    }
    curveVertices.clear();
}

//----------------------------------------------------------
void ofSetPolyMode(int mode){
	switch (mode){
		case OF_POLY_WINDING_ODD:
			polyMode = OF_POLY_WINDING_ODD;
			break;
		case OF_POLY_WINDING_NONZERO:
			polyMode = OF_POLY_WINDING_NONZERO;
			break;
		case OF_POLY_WINDING_POSITIVE:
			polyMode = OF_POLY_WINDING_POSITIVE;
			break;
		case OF_POLY_WINDING_NEGATIVE:
			polyMode = OF_POLY_WINDING_NEGATIVE;
			break;
		case OF_POLY_WINDING_ABS_GEQ_TWO:
			polyMode = OF_POLY_WINDING_ABS_GEQ_TWO;
			break;
		default:
			ofLog(OF_LOG_ERROR," error in ofSetPolyMode");

	}

	currentStyle.polyMode = polyMode;
}

//----------------------------------------------------------
void ofBeginShape(){

	if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

	// just clear the vertices, just to make sure that
	// someone didn't do something improper, like :
	// a) ofBeginShape()
	// b) ofVertex(), ofVertex(), ofVertex() ....
	// c) ofBeginShape()
	// etc...

	clearTessVertices();


	// now get the tesselator object up and ready:

	tobj = gluNewTess();


	// --------------------------------------------------------
	// note: 	you could write your own begin and end callbacks
	// 			if you wanted to...
	// 			for example, to count triangles or know which
	// 			type of object tess is giving back, etc...
	// --------------------------------------------------------

	#if defined( TARGET_OSX)
		#ifndef MAC_OS_X_VERSION_10_5
			#define OF_NEED_GLU_FIX
		#endif
	#endif

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// MAC - XCODE USERS PLEASE NOTE - some machines will not be able to compile the code below
	// if this happens uncomment the "OF_NEED_GLU_FIX" line below and it
	// should compile also please post to the forums with the error message, you OS X version,
	// Xcode verison and the CPU type - PPC or Intel. Thanks!
	// (note: this is known problem based on different version of glu.h, we are working on a fix)
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//#define OF_NEED_GLU_FIX

	#ifdef OF_NEED_GLU_FIX
		#define OF_GLU_CALLBACK_HACK (void(CALLBACK*)(...)
	#else
		#define OF_GLU_CALLBACK_HACK (void(CALLBACK*)()
	#endif

	gluTessCallback( tobj, GLU_TESS_BEGIN, OF_GLU_CALLBACK_HACK)&tessBegin);
	gluTessCallback( tobj, GLU_TESS_VERTEX, OF_GLU_CALLBACK_HACK)&tessVertex);
	gluTessCallback( tobj, GLU_TESS_COMBINE, OF_GLU_CALLBACK_HACK)&tessCombine);
	gluTessCallback( tobj, GLU_TESS_END, OF_GLU_CALLBACK_HACK)&tessEnd);
	gluTessCallback( tobj, GLU_TESS_ERROR, OF_GLU_CALLBACK_HACK)&tessError);

	gluTessProperty( tobj, GLU_TESS_WINDING_RULE, polyMode);
	if (drawMode == OF_OUTLINE){
		gluTessProperty( tobj, GLU_TESS_BOUNDARY_ONLY, true);
	} else {
		gluTessProperty( tobj, GLU_TESS_BOUNDARY_ONLY, false);
	}
	gluTessProperty( tobj, GLU_TESS_TOLERANCE, 0);

	/* ------------------------------------------
	for 2d, this next call (normal) likely helps speed up ....
	quote : The computation of the normal represents about 10% of
	the computation time. For example, if all polygons lie in
	the x-y plane, you can provide the normal by using the
	-------------------------------------------  */

	gluTessNormal(tobj, 0.0, 0.0, 1.0);
	gluTessBeginPolygon( tobj, NULL);

}

//----------------------------------------------------------
void ofVertex(float x, float y){
 	double* point = new double[3];
 	point[0] = x;
	point[1] = y;
	point[2] = 0;
 	polyVertices.push_back(point);


 	clearCurveVertices();	// we drop any "curve calls"
 							// once a vertex call has been made
 							// ie,
 							// you can't mix
 							// ofCurveVertex();
 							// ofCurveVertex();
 							// ofVertex();
 							// etc...
 							// and you need 4 calls
 							// to curve to see something...

}


//---------------------------------------------------
void ofCurveVertex(float x, float y){

	double* point = new double[3];
 	point[0] = x;
	point[1] = y;
	point[2] = 0;
 	curveVertices.push_back(point);

 	if (curveVertices.size() >= 4){

 		int startPos = (int)curveVertices.size() - 4;

 		float x0 = curveVertices[startPos + 0][0];
	   	float y0 = curveVertices[startPos + 0][1];
 		float x1 = curveVertices[startPos + 1][0];
	   	float y1 = curveVertices[startPos + 1][1];
 		float x2 = curveVertices[startPos + 2][0];
	   	float y2 = curveVertices[startPos + 2][1];
 		float x3 = curveVertices[startPos + 3][0];
	   	float y3 = curveVertices[startPos + 3][1];

 		int resolution = curveResolution;

		float t,t2,t3;
		float x,y;

		for (int i = 0; i < resolution; i++){

			t 	=  (float)i / (float)(resolution-1);
			t2 	= t * t;
			t3 	= t2 * t;

			x = 0.5f * ( ( 2.0f * x1 ) +
			( -x0 + x2 ) * t +
			( 2.0f * x0 - 5.0f * x1 + 4 * x2 - x3 ) * t2 +
			( -x0 + 3.0f * x1 - 3.0f * x2 + x3 ) * t3 );

			y = 0.5f * ( ( 2.0f * y1 ) +
			( -y0 + y2 ) * t +
			( 2.0f * y0 - 5.0f * y1 + 4 * y2 - y3 ) * t2 +
			( -y0 + 3.0f * y1 - 3.0f * y2 + y3 ) * t3 );

			double* newPoint = new double[3];
			newPoint[0] = x;
			newPoint[1] = y;
			newPoint[2] = 0;
			polyVertices.push_back(newPoint);
		}
 	}

}

void ofBezierVertex(float x1, float y1, float x2, float y2, float x3, float y3){


	clearCurveVertices();	// we drop any stored "curve calls"


	// if, and only if poly vertices has points, we can make a bezier
	// from the last point

	// the resolultion with which we computer this bezier
	// is arbitrary, can we possibly make it dynamic?

	if (polyVertices.size() > 0){

		float x0 = polyVertices[polyVertices.size()-1][0];
		float y0 = polyVertices[polyVertices.size()-1][1];

		float   ax, bx, cx;
		float   ay, by, cy;
		float   t, t2, t3;
		float   x, y;

		// polynomial coefficients
		cx = 3.0f * (x1 - x0);
		bx = 3.0f * (x2 - x1) - cx;
		ax = x3 - x0 - cx - bx;

		cy = 3.0f * (y1 - y0);
		by = 3.0f * (y2 - y1) - cy;
		ay = y3 - y0 - cy - by;

		// arbitrary ! can we fix??
		int resolution = curveResolution;

		for (int i = 0; i < resolution; i++){
			t 	=  (float)i / (float)(resolution-1);
			t2 = t * t;
			t3 = t2 * t;
			x = (ax * t3) + (bx * t2) + (cx * t) + x0;
			y = (ay * t3) + (by * t2) + (cy * t) + y0;
			ofVertex(x,y);
		}


	}


}

//----------------------------------------------------------
void ofNextContour(bool bClose){

	if ((bClose == true)){
		//---------------------------
		if ((int)polyVertices.size() > currentStartVertex){

			double* point = new double[3];
	 		point[0] = polyVertices[currentStartVertex][0];
			point[1] = polyVertices[currentStartVertex][1];
			point[2] = 0;
	 		polyVertices.push_back(point);
 		}
	}

	if ((polyMode == OF_POLY_WINDING_ODD) && (drawMode == OF_OUTLINE)){
		// let's just draw via another method, like glLineLoop
		// much, much faster, and *no* tess / computation necessary

		int numToDraw = polyVertices.size()-currentStartVertex;
		if( numToDraw > 0){

			// GLfloat points[numToDraw * 2];	// zach, we can't do this on VS 2008
			GLfloat * points = new GLfloat[numToDraw * 2];
			int k = 0;

			for (int i=currentStartVertex; i< (int)polyVertices.size(); i++) {
				points[k] = polyVertices[i][0];
				points[k+1] = polyVertices[i][1];
				k+=2;
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, &points[0]);
			glDrawArrays(GL_LINE_STRIP, 0, numToDraw);

			delete [] points;
		}

	} else {

		if ( tobj != NULL){
	      gluTessBeginContour( tobj);
			for (int i=currentStartVertex; i<(int)polyVertices.size(); i++) {
	   			gluTessVertex( tobj, polyVertices[i],polyVertices[i]);
			}
			gluTessEndContour( tobj);
		}
   	}

   	currentStartVertex = (int)polyVertices.size();

}


//----------------------------------------------------------
void ofEndShape(bool bClose){

	// (close -> add the first point to the end)
	// -----------------------------------------------

	if ((bClose == true)){
		//---------------------------
		if ((int)polyVertices.size() > currentStartVertex){

			double* point = new double[3];
	 		point[0] = polyVertices[currentStartVertex][0];
			point[1] = polyVertices[currentStartVertex][1];
			point[2] = 0;
	 		polyVertices.push_back(point);

 		}
	}
	//------------------------------------------------



	if ((polyMode == OF_POLY_WINDING_ODD) && (drawMode == OF_OUTLINE)){

		// let's just draw via another method, like glLineLoop
		// much, much faster, and *no* tess / computation necessary

		int numToDraw = polyVertices.size()-currentStartVertex;
		if( numToDraw > 0){

			// GLfloat points[numToDraw * 2]; // zach, needed for VS 2008

			GLfloat * points = new GLfloat[numToDraw * 2];

			int k = 0;

			for (int i=currentStartVertex; i< (int)polyVertices.size(); i++) {
				points[k] = polyVertices[i][0];
				points[k+1] = polyVertices[i][1];

				k+=2;
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, &points[0]);
			glDrawArrays(GL_LINE_STRIP, 0, numToDraw);

			delete [] points;
		}


	} else {

		if ( tobj != NULL){
	    	gluTessBeginContour( tobj);
			for (int i=currentStartVertex; i<(int)polyVertices.size(); i++) {
	   			gluTessVertex( tobj, polyVertices[i],polyVertices[i]);
			}

			gluTessEndContour( tobj);

		}
   	}

	if ( tobj != NULL){
		// no matter what we did / do, we need to delete the tesselator object
		gluTessEndPolygon( tobj);
		gluDeleteTess( tobj);
		tobj = NULL;
	}

   	// now clear the vertices on the dynamically allocated data
   	clearTessVertices();

   	if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();

}


