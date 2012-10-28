#include "ofGLES2Renderer.h"
#include "ofMesh.h"
#include "ofPath.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofMesh.h"
#include "ofBitmapFont.h"
#include "ofGLUtils.h"
#include "ofImage.h"
#include "ofFbo.h"

#ifdef TARGET_OF_IPHONE
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif

#ifdef TARGET_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

enum {
    UNIFORM_MODELVIEW_PROJECTION_MATRIX,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

enum {
    ATTRIB_VERTEX,
    ATTRIB_COLOR,
    NUM_ATTRIBUTES
};

//----------------------------------------------------------
ofGLES2Renderer::ofGLES2Renderer(bool useShapeColor){
	bBackgroundAuto = true;
    
	linePoints.resize(2);
    lineColors.resize(2);
	triPoints.resize(3);
    triColors.resize(3);
	rectPoints.resize(4);
    rectColors.resize(4);
    
	currentFbo = NULL;
    
    program = 0;
    vertShader = 0;
    fragShader = 0;
}

ofGLES2Renderer::~ofGLES2Renderer() {
    destroyShaders(vertShader, fragShader, program);
}

//----------------------------------------------------------
void ofGLES2Renderer::startRender() {
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IPHONE)
    if(program) {
        glUseProgram(program);
    }
#endif
}

void ofGLES2Renderer::finishRender() {
    //
}

//---------------------------------------------------------- SHADERS.
//  The below code should be handled by ofShader.
//  but currently it does not support OpenGL ES.
//  Once it does, all this shader code should be removed.
//----------------------------------------------------------

bool ofGLES2Renderer::loadShaders() {
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IPHONE)
    string vertShaderPathname;
    string fragShaderPathname;
    
	// create shader program
	program = glCreateProgram();
	
	// create and compile vertex shader
    vertShaderPathname = ofToDataPath("shaders/Shader.vsh");
	if (!compileShader(&vertShader, GL_VERTEX_SHADER, 1, vertShaderPathname)) {
		destroyShaders(vertShader, fragShader, program);
		return false;
	}
	
	// create and compile fragment shader
	fragShaderPathname = ofToDataPath("shaders/Shader.fsh");
	if (!compileShader(&fragShader, GL_FRAGMENT_SHADER, 1, fragShaderPathname)) {
		destroyShaders(vertShader, fragShader, program);
		return false;
	}
	
	// attach vertex shader to program
	glAttachShader(program, vertShader);
	
	// attach fragment shader to program
	glAttachShader(program, fragShader);
	
	// bind attribute locations
	// this needs to be done prior to linking
	glBindAttribLocation(program, ATTRIB_VERTEX, "position");
	glBindAttribLocation(program, ATTRIB_COLOR, "color");
	
	// link program
	if (!linkProgram(program)) {
		destroyShaders(vertShader, fragShader, program);
		return false;
	}
	
	// get uniform locations
	uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX] = glGetUniformLocation(program, "modelViewProjectionMatrix");
	
	// release vertex and fragment shaders
	if (vertShader) {
		glDeleteShader(vertShader);
		vertShader = 0;
	}
	if (fragShader) {
		glDeleteShader(fragShader);
		fragShader = 0;
	}
	
	return true;
#endif
    return false;
}

GLint ofGLES2Renderer::compileShader(GLuint *shader, GLenum type, GLsizei count, string fileName) {
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IPHONE)    
	GLint status;
	const GLchar * sources;
	
    ofFile file;
    file.open(fileName, ofFile::ReadWrite, false);
    ofBuffer buff = file.readToBuffer();
    
	// get source code
	sources = buff.getText().c_str();
	if(!sources) {
        ofLog(OF_LOG_ERROR, "Failed to load vertex shader");
		return 0;
	}
	
    *shader = glCreateShader(type);				// create shader
    glShaderSource(*shader, 1, &sources, NULL);	// set source code in the shader
    glCompileShader(*shader);					// compile shader
	
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        ofLog(OF_LOG_ERROR, "Failed to compile shader:");
		for(int i = 0; i < count; i++) {
            ofLog(OF_LOG_ERROR, &sources[i]);
        }
	}
	
	return status;
#endif
    return 0;
}

GLint ofGLES2Renderer::linkProgram(GLuint prog) {
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IPHONE)    
	GLint status;
	
	glLinkProgram(prog);
	
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        ofLog(OF_LOG_ERROR, "Failed to link program");
    }
	
	return status;
#endif
    return 0;
}

GLint ofGLES2Renderer::validateProgram(GLuint prog) {
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IPHONE)
	GLint logLength, status;
	
	glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if(logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if(status == GL_FALSE) {
        ofLog(OF_LOG_ERROR, "Failed to validate program");
    }
	
	return status;
#endif
    return 0;
}

void ofGLES2Renderer::destroyShaders(GLuint vertShader, GLuint fragShader, GLuint prog) {
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IPHONE)
	if (vertShader) {
		glDeleteShader(vertShader);
		vertShader = 0;
	}
	if (fragShader) {
		glDeleteShader(fragShader);
		fragShader = 0;
	}
	if (prog) {
		glDeleteProgram(prog);
		prog = 0;
	}
#endif
}

//----------------------------------------------------------
void ofGLES2Renderer::update(){
    //
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofMesh & vertexData, bool useColors, bool useTextures, bool useNormals){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(vector<ofPoint> & vertexData, ofPrimitiveMode drawMode){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofPolyline & poly){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofPath & shape){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::setCurrentFBO(ofFbo * fbo){
	currentFbo = fbo;
}

//----------------------------------------------------------
void ofGLES2Renderer::pushView() {
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::popView() {
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::viewport(ofRectangle viewport_){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::viewport(float x, float y, float width, float height, bool invertY) {
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
ofRectangle ofGLES2Renderer::getCurrentViewport(){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
int ofGLES2Renderer::getViewportWidth(){
    // TODO :: needs ES2 code.
	return ofGetScreenWidth();
}

//----------------------------------------------------------
int ofGLES2Renderer::getViewportHeight(){
    // TODO :: needs ES2 code.
	return ofGetScreenHeight();
}

//----------------------------------------------------------
void ofGLES2Renderer::setCoordHandedness(ofHandednessType handedness) {
	coordHandedness = handedness;
}

//----------------------------------------------------------
ofHandednessType ofGLES2Renderer::getCoordHandedness() {
	return coordHandedness;
}

//----------------------------------------------------------
void ofGLES2Renderer::setupScreenPerspective(float width, float height, ofOrientation orientation, bool vFlip, float fov, float nearDist, float farDist) {
    
	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();
    
	float viewW = ofGetViewportWidth();
	float viewH = ofGetViewportHeight();
    
	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) viewW / viewH;
    
	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;
    
	ofMatrix4x4 projectionMatrix;
	projectionMatrix.makePerspectiveMatrix(fov, aspect, nearDist, farDist);
    
	ofMatrix4x4 modelViewMatrix;
	modelViewMatrix.makeLookAtViewMatrix(ofVec3f(eyeX, eyeY, dist),  ofVec3f(eyeX, eyeY, 0),  ofVec3f(0, 1, 0));
    
    ofMatrix4x4 orientationMatrix;
    
	//note - theo checked this on iPhone and Desktop for both vFlip = false and true
	if(ofDoesHWOrientation()){
		if(vFlip){
            orientationMatrix.glScale(1, -1, 1);
            orientationMatrix.glTranslate(0, -height, 0);
		}
	}else{
		if( orientation == OF_ORIENTATION_UNKNOWN ) orientation = ofGetOrientation();
		switch(orientation) {
			case OF_ORIENTATION_180:
                orientationMatrix.glRotate(-180, 0, 0, 1);
				if(vFlip){
                    orientationMatrix.glScale(1, -1, 1);
                    orientationMatrix.glTranslate(-width, 0, 0);
				}else{
                    orientationMatrix.glTranslate(-width, -height, 0);
				}
                
				break;
                
			case OF_ORIENTATION_90_RIGHT:
                orientationMatrix.glRotate(-90, 0, 0, 1);
				if(vFlip){
                    orientationMatrix.glScale(-1, 1, 1);
				}else{
                    orientationMatrix.glScale(-1, -1, 1);
                    orientationMatrix.glTranslate(0, -height, 0);
				}
				break;
                
			case OF_ORIENTATION_90_LEFT:
                orientationMatrix.glRotate(90, 0, 0, 1);
				if(vFlip){
                    orientationMatrix.glScale(-1, 1, 1);
                    orientationMatrix.glTranslate(-width, -height, 0);
				}else{
                    orientationMatrix.glScale(-1, -1, 1);
                    orientationMatrix.glTranslate(-width, 0, 0);
				}
				break;
                
			case OF_ORIENTATION_DEFAULT:
			default:
				if(vFlip){
                    orientationMatrix.glScale(1, -1, 1);
                    orientationMatrix.glTranslate(0, -height, 0);
				}
				break;
		}
	}
    
    ofMatrix4x4 modelViewProjectionMatrix;
    modelViewProjectionMatrix = projectionMatrix;
    modelViewProjectionMatrix.preMult(modelViewMatrix);
    modelViewProjectionMatrix.preMult(orientationMatrix);
    
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IPHONE)
    glUniformMatrix4fv(0, 1, GL_FALSE, modelViewProjectionMatrix.getPtr());
#endif
}

//----------------------------------------------------------
void ofGLES2Renderer::setupScreenOrtho(float width, float height, ofOrientation orientation, bool vFlip, float nearDist, float farDist) {
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
//Resets openGL parameters back to OF defaults
void ofGLES2Renderer::setupGraphicDefaults(){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::setupScreen(){
	setupScreenPerspective();	// assume defaults
}

//----------------------------------------------------------
void ofGLES2Renderer::setCircleResolution(int res){
	if((int)circlePolyline.size()!=res+1){
		circlePolyline.clear();
		circlePolyline.arc(0,0,0,1,1,0,360,res);
		circlePoints.resize(circlePolyline.size());
        circleColors.resize(circlePolyline.size());
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::setSphereResolution(int res) {
	if(sphereMesh.getNumVertices() == 0 || res != ofGetStyle().sphereResolution) {
		int n = res * 2;
		float ndiv2=(float)n/2;
        
		/*
		 Original code by Paul Bourke
		 A more efficient contribution by Federico Dosil (below)
		 Draw a point for zero radius spheres
		 Use CCW facet ordering
		 http://paulbourke.net/texture_colour/texturemap/
		 */
		
		float theta2 = TWO_PI;
		float phi1 = -HALF_PI;
		float phi2 = HALF_PI;
		float r = 1.f; // normalize the verts //
        
		sphereMesh.clear();
        sphereMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
		int i, j;
        float theta1 = 0.f;
		float jdivn,j1divn,idivn,dosdivn,unodivn=1/(float)n,t1,t2,t3,cost1,cost2,cte1,cte3;
		cte3 = (theta2-theta1)/n;
		cte1 = (phi2-phi1)/ndiv2;
		dosdivn = 2*unodivn;
		ofVec3f e,p,e2,p2;
        
		if (n < 0){
			n = -n;
			ndiv2 = -ndiv2;
		}
		if (n < 4) {n = 4; ndiv2=(float)n/2;}
        if(r <= 0) r = 1;
		
		t2=phi1;
		cost2=cos(phi1);
		j1divn=0;
        
        ofVec3f vert, normal;
        ofVec2f tcoord;
		
		for (j=0;j<ndiv2;j++) {
			t1 = t2;
			t2 += cte1;
			t3 = theta1 - cte3;
			cost1 = cost2;
			cost2 = cos(t2);
			e.y = sin(t1);
			e2.y = sin(t2);
			p.y = r * e.y;
			p2.y = r * e2.y;
			
			idivn=0;
			jdivn=j1divn;
			j1divn+=dosdivn;
			for (i=0;i<=n;i++) {
				t3 += cte3;
				e.x = cost1 * cos(t3);
				e.z = cost1 * sin(t3);
				p.x = r * e.x;
				p.z = r * e.z;
				
				normal.set( e.x, e.y, e.z );
				tcoord.set( idivn, jdivn );
				vert.set( p.x, p.y, p.z );
				
				sphereMesh.addNormal(normal);
				sphereMesh.addTexCoord(tcoord);
				sphereMesh.addVertex(vert);
				
				e2.x = cost2 * cos(t3);
				e2.z = cost2 * sin(t3);
				p2.x = r * e2.x;
				p2.z = r * e2.z;
				
				normal.set(e2.x, e2.y, e2.z);
				tcoord.set(idivn, j1divn);
				vert.set(p2.x, p2.y, p2.z);
				
				sphereMesh.addNormal(normal);
				sphereMesh.addTexCoord(tcoord);
				sphereMesh.addVertex(vert);
				
				idivn += unodivn;
				
			}
		}
	}
}

//our openGL wrappers
//----------------------------------------------------------
void ofGLES2Renderer::pushMatrix(){
	transformStack.push(ofMatrix4x4());
}

//----------------------------------------------------------
void ofGLES2Renderer::popMatrix(){
	transformStack.pop();
}

//----------------------------------------------------------
void ofGLES2Renderer::translate(const ofPoint& p){
	translate(p.x, p.y, p.z);
}

//----------------------------------------------------------
void ofGLES2Renderer::translate(float x, float y, float z){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::scale(float xAmnt, float yAmnt, float zAmnt){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::rotate(float degrees, float vecX, float vecY, float vecZ){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::rotateX(float degrees){
	rotate(degrees, 1, 0, 0);
}

//----------------------------------------------------------
void ofGLES2Renderer::rotateY(float degrees){
	rotate(degrees, 0, 1, 0);
}

//----------------------------------------------------------
void ofGLES2Renderer::rotateZ(float degrees){
	rotate(degrees, 0, 0, 1);
}

//same as ofRotateZ
//----------------------------------------------------------
void ofGLES2Renderer::rotate(float degrees){
	rotateZ(degrees);
}

//----------------------------------------------------------
void ofGLES2Renderer::loadIdentityMatrix (void){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::loadMatrix (const ofMatrix4x4 & m){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::loadMatrix (const float *m){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::multMatrix (const ofMatrix4x4 & m){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::multMatrix (const float *m){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::setColor(const ofColor & color){
	setColor(color.r,color.g,color.b,color.a);
}

//----------------------------------------------------------
void ofGLES2Renderer::setColor(const ofColor & color, int _a){
	setColor(color.r,color.g,color.b,_a);
}

//----------------------------------------------------------
void ofGLES2Renderer::setColor(int _r, int _g, int _b){
	setColor(_r, _g, _b, 255);
}

//----------------------------------------------------------
void ofGLES2Renderer::setColor(int _r, int _g, int _b, int _a){
	currentColor.set(_r/255.f,_g/255.f,_b/255.f,_a/255.f);
}

//----------------------------------------------------------
void ofGLES2Renderer::setColor(int gray){
	setColor(gray, gray, gray);
}

//----------------------------------------------------------
void ofGLES2Renderer::setHexColor(int hexColor){
	int r = (hexColor >> 16) & 0xff;
	int g = (hexColor >> 8) & 0xff;
	int b = (hexColor >> 0) & 0xff;
	setColor(r,g,b);
}

//----------------------------------------------------------
void ofGLES2Renderer::clear(float r, float g, float b, float a) {
	glClearColor(r / 255., g / 255., b / 255., a / 255.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
void ofGLES2Renderer::clear(float brightness, float a) {
	clear(brightness, brightness, brightness, a);
}

//----------------------------------------------------------
void ofGLES2Renderer::clearAlpha() {
	glColorMask(0, 0, 0, 1);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColorMask(1, 1, 1, 1);
}

//----------------------------------------------------------
void ofGLES2Renderer::setBackgroundAuto(bool bAuto){
	bBackgroundAuto = bAuto;
}

//----------------------------------------------------------
bool ofGLES2Renderer::bClearBg(){
	return bBackgroundAuto;
}

//----------------------------------------------------------
ofFloatColor & ofGLES2Renderer::getBgColor(){
	return bgColor;
}

//----------------------------------------------------------
void ofGLES2Renderer::background(const ofColor & c){
	bgColor = c;
	glClearColor(bgColor[0],bgColor[1],bgColor[2], bgColor[3]);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
void ofGLES2Renderer::background(float brightness) {
	background(brightness);
}

//----------------------------------------------------------
void ofGLES2Renderer::background(int hexColor, float _a){
	background ( (hexColor >> 16) & 0xff, (hexColor >> 8) & 0xff, (hexColor >> 0) & 0xff, _a);
}

//----------------------------------------------------------
void ofGLES2Renderer::background(int r, int g, int b, int a){
	background(ofColor(r,g,b,a));
}

//----------------------------------------------------------
void ofGLES2Renderer::setFillMode(ofFillFlag fill){
	bFilled = fill;
}

//----------------------------------------------------------
ofFillFlag ofGLES2Renderer::getFillMode(){
	return bFilled;
}

//----------------------------------------------------------
void ofGLES2Renderer::setRectMode(ofRectMode mode){
	rectMode = mode;
}

//----------------------------------------------------------
ofRectMode ofGLES2Renderer::getRectMode(){
	return rectMode;
}

//----------------------------------------------------------
void ofGLES2Renderer::setLineWidth(float lineWidth){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::setLineSmoothing(bool smooth){
	bSmoothHinted = smooth;
}

//----------------------------------------------------------
void ofGLES2Renderer::startSmoothing(){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::endSmoothing(){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::setBlendMode(ofBlendMode blendMode){
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::enablePointSprites(){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::disablePointSprites(){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2){
	linePoints[0].set(x1,y1,z1);
	linePoints[1].set(x2,y2,z2);
    
    lineColors[0].set(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    lineColors[1].set(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    
	// use smoothness, if requested:
	if (bSmoothHinted) startSmoothing();
    
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(ofVec3f), &linePoints[0].x);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, GL_TRUE, sizeof(ofFloatColor), &lineColors[0].r);
    glEnableVertexAttribArray(ATTRIB_COLOR);
    
	glDrawArrays(GL_LINES, 0, 2);
    
	// use smoothness, if requested:
	if (bSmoothHinted) endSmoothing();
}

//----------------------------------------------------------
void ofGLES2Renderer::drawRectangle(float x, float y, float z, float w, float h) {
	if (rectMode == OF_RECTMODE_CORNER){
		rectPoints[0].set(x,y,z);
		rectPoints[1].set(x+w, y, z);
		rectPoints[2].set(x+w, y+h, z);
		rectPoints[3].set(x, y+h, z);
	}else{
		rectPoints[0].set(x-w/2.0f, y-h/2.0f, z);
		rectPoints[1].set(x+w/2.0f, y-h/2.0f, z);
		rectPoints[2].set(x+w/2.0f, y+h/2.0f, z);
		rectPoints[3].set(x-w/2.0f, y+h/2.0f, z);
	}
    
    rectColors[0].set(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    rectColors[1].set(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    rectColors[2].set(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    rectColors[3].set(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();
    
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(ofVec3f), &rectPoints[0].x);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, GL_TRUE, sizeof(ofFloatColor), &rectColors[0].r);
    glEnableVertexAttribArray(ATTRIB_COLOR);

	glDrawArrays((bFilled == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, 4);
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofGLES2Renderer::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	triPoints[0].set(x1,y1,z1);
	triPoints[1].set(x2,y2,z2);
	triPoints[2].set(x3,y3,z3);
    
    triColors[0].set(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    triColors[1].set(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    triColors[2].set(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(ofVec3f), &triPoints[0].x);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, GL_TRUE, sizeof(ofFloatColor), &triColors[0].r);
    glEnableVertexAttribArray(ATTRIB_COLOR);
    
	glDrawArrays((bFilled == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, 3);
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofGLES2Renderer::drawCircle(float x, float y, float z,  float radius){
	vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		circlePoints[i].set(radius*circleCache[i].x+x,radius*circleCache[i].y+y,z);
        circleColors[i].set(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
	}
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();
    
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(ofVec3f), &circlePoints[0].x);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, GL_TRUE, sizeof(ofFloatColor), &circleColors[0].r);
    glEnableVertexAttribArray(ATTRIB_COLOR);
    
	glDrawArrays((bFilled == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_STRIP, 0, circlePoints.size());
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofGLES2Renderer::drawSphere(float x, float y, float z, float radius) {
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLES2Renderer::drawEllipse(float x, float y, float z, float width, float height){
	float radiusX = width*0.5;
	float radiusY = height*0.5;
	vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		circlePoints[i].set(radiusX*circlePolyline[i].x+x,radiusY*circlePolyline[i].y+y,z);
        circleColors[i].set(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
	}
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();
    
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(ofVec3f), &circlePoints[0].x);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, GL_TRUE, sizeof(ofFloatColor), &circleColors[0].r);
    glEnableVertexAttribArray(ATTRIB_COLOR);
    
	glDrawArrays((bFilled == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_STRIP, 0, circlePoints.size());
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofGLES2Renderer::drawString(string textString, float x, float y, float z, ofDrawBitmapMode mode){
	// TODO :: needs ES2 code.
}
