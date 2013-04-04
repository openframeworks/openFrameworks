#include "ofProgrammableGLRenderer.h"
#include "ofMesh.h"
#include "ofPath.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofMesh.h"
#include "ofBitmapFont.h"
#include "ofGLUtils.h"
#include "ofImage.h"
#include "ofFbo.h"
#include "ofVbo.h"
#include "of3dPrimitives.h"

static const int OF_NO_TEXTURE=-1;

static const string MODELVIEW_MATRIX_UNIFORM="modelViewMatrix";
static const string PROJECTION_MATRIX_UNIFORM="projectionMatrix";
static const string ORIENTATION_MATRIX_UNIFORM="orientationMatrix";
static const string MODELVIEW_PROJECTION_MATRIX_UNIFORM="modelViewProjectionMatrix";
static const string TEXTURE_MATRIX_UNIFORM="textureMatrix";
static const string COLOR_UNIFORM="color";

static const string USE_TEXTURECOORDS_UNIFORM="useTexture";
static const string USE_COLORS_UNIFORM="useColors";


const string ofProgrammableGLRenderer::TYPE="ProgrammableGL";


//----------------------------------------------------------
ofProgrammableGLRenderer::ofProgrammableGLRenderer(string vertexShader, string fragmentShader, bool useShapeColor)
:matrixStack(*ofGetWindowPtr())
{
	bBackgroundAuto = true;

	lineVbo.getVertices().resize(2);
	lineVbo.setMode(OF_PRIMITIVE_LINES);
	triangleVbo.getVertices().resize(3);
	rectVbo.getVertices().resize(4);


    rectMode = OF_RECTMODE_CORNER;
    bFilled = OF_FILLED;
    bSmoothHinted = false;

    verticesEnabled = true;
    colorsEnabled = false;
    texCoordsEnabled = false;
    normalsEnabled = false;
	settingDefaultShader = false;

	wrongUseLoggedOnce = false;

	currentShader = NULL;

	currentTextureTarget = OF_NO_TEXTURE;
}

ofProgrammableGLRenderer::~ofProgrammableGLRenderer() {
	
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::startRender() {
	beginDefaultShader();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::finishRender() {
	glUseProgram(0);
	if(!usingCustomShader) currentShader = NULL;
	
	matrixStack.clearStacks();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::update(){
    //
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::draw(ofMesh & vertexData, bool useColors, bool useTextures, bool useNormals){
	draw(vertexData, OF_MESH_FILL, useColors, useTextures, useNormals); // tig: use default mode if no render mode specified.
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::draw(ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals){
	if(!wrongUseLoggedOnce){
		ofLogWarning() << "drawing ofMesh or ofPolyline directly is deprecated, use an ofVboMesh or ofPath";
		wrongUseLoggedOnce = true;
	}
	// tig: note that there was a lot of code duplication going on here.
	// using ofVbo's draw methods, to keep stuff DRY

	if (vertexData.getVertices().empty()) return;
	
	meshVbo.setMesh(vertexData, GL_DYNAMIC_DRAW, useColors, useTextures, useNormals);
	
	
	// tig: note that for GL3+ we use glPolygonMode to draw wireframes or filled meshes, and not the primitive mode.
	// the reason is not purely aesthetic, but more conformant with the behaviour of ofGLRenderer. Whereas
	// gles2.0 doesn't allow for a polygonmode.

	
#ifdef TARGET_OPENGLES
	
	// GLES 
	
	GLenum drawMode;
	switch(renderType){
		case OF_MESH_POINTS:
			drawMode = GL_POINTS;
			break;
		case OF_MESH_WIREFRAME:
			drawMode = GL_LINES;
			break;
		case OF_MESH_FILL:
			drawMode = ofGetGLPrimitiveMode(vertexData.getMode());
			break;
		default:
			// use the current fill mode to tell.
			drawMode = ofGetGLPrimitiveMode(vertexData.getMode());
			break;
	}
	if(vertexData.hasIndices()){
		meshVbo.drawElements(drawMode, meshVbo.getNumIndices());
	} else {
		meshVbo.draw(drawMode, 0, meshVbo.getNumVertices());
	}
#else

	// OpenGL
	
	glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
	if(meshVbo.getUsingIndices()) {
		meshVbo.drawElements(ofGetGLPrimitiveMode(vertexData.getMode()), meshVbo.getNumIndices());
	} else {
		meshVbo.draw(ofGetGLPrimitiveMode(vertexData.getMode()), 0, meshVbo.getNumVertices());
	}
	
	// tig: note further that we could glGet() and store the current polygon mode, but don't, since that would
	// infer a massive performance hit. instead, we revert the glPolygonMode to mirror the current ofFill state
	// after we're finished drawing, following the principle of least surprise.
	// ideally the glPolygonMode (or the polygon draw mode) should be part of ofStyle so that we can keep track
	// of its state on the client side...
	
	glPolygonMode(GL_FRONT_AND_BACK, (ofGetFill() == OF_OUTLINE) ?  GL_LINE : GL_FILL);
	
#endif
	
	if (bSmoothHinted) endSmoothing();
	
	//finishPrimitiveDraw();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::draw( of3dPrimitive& model, ofPolyRenderMode renderType) {
	model.getMesh().draw(renderType);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::draw(ofPolyline & poly){
	if(!wrongUseLoggedOnce){
		ofLogWarning() << "drawing ofMesh or ofPolyline directly is deprecated, use an ofVboMesh or ofPath";
		wrongUseLoggedOnce = true;
	}
	if(!poly.getVertices().empty()) {
		// use smoothness, if requested:
		if (bSmoothHinted) startSmoothing();

		vertexDataVbo.setVertexData(&poly.getVertices()[0], poly.size(), GL_DYNAMIC_DRAW);
		vertexDataVbo.draw(poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, poly.size());

		// use smoothness, if requested:
		if (bSmoothHinted) endSmoothing();
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::draw(ofPath & shape){
	ofColor prevColor;
	if(shape.getUseShapeColor()){
		prevColor = ofGetStyle().color;
	}
	if(shape.isFilled()){
		ofMesh & mesh = shape.getTessellation();
		if(shape.getUseShapeColor()){
			setColor( shape.getFillColor() * ofGetStyle().color,shape.getFillColor().a/255. * ofGetStyle().color.a);
		}
		draw(mesh);
	}
	if(shape.hasOutline()){
		float lineWidth = ofGetStyle().lineWidth;
		if(shape.getUseShapeColor()){
			setColor( shape.getStrokeColor() * ofGetStyle().color, shape.getStrokeColor().a/255. * ofGetStyle().color.a);
		}
		setLineWidth( shape.getStrokeWidth() );
		vector<ofPolyline> & outlines = shape.getOutline();
		for(int i=0; i<(int)outlines.size(); i++)
			draw(outlines[i]);
		setLineWidth(lineWidth);
	}
	if(shape.getUseShapeColor()){
		setColor(prevColor);
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::draw(ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	if(image.isUsingTexture()){
		enableTexCoords();
		disableColors();
		ofTexture& tex = image.getTextureReference();
		if(tex.bAllocated()) {
			tex.drawSubsection(x,y,z,w,h,sx,sy,sw,sh);
		} else {
			ofLogWarning() << "ofGLRenderer::draw(): texture is not allocated";
		}
		disableTexCoords();
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::draw(ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	if(image.isUsingTexture()){
		enableTexCoords();
		disableColors();
		ofTexture& tex = image.getTextureReference();
		if(tex.bAllocated()) {
			tex.drawSubsection(x,y,z,w,h,sx,sy,sw,sh);
		} else {
			ofLogWarning() << "ofGLRenderer::draw(): texture is not allocated";
		}
		disableTexCoords();
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::draw(ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	if(image.isUsingTexture()){
		enableTexCoords();
		disableColors();
		ofTexture& tex = image.getTextureReference();
		if(tex.bAllocated()) {
			tex.drawSubsection(x,y,z,w,h,sx,sy,sw,sh);
		} else {
			ofLogWarning() << "ofGLRenderer::draw(): texture is not allocated";
		}
		disableTexCoords();
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setCurrentFBO(ofFbo * fbo){
	if(fbo!=NULL){
		matrixStack.setRenderSurface(*fbo);
	}else{
		matrixStack.setRenderSurface(*ofGetWindowPtr());
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::pushView() {
	matrixStack.pushView();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::popView() {
	matrixStack.popView();
	uploadAllMatrices();
	viewport(matrixStack.getCurrentViewport());
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::viewport(ofRectangle viewport_){
	viewport(viewport_.x,viewport_.y,viewport_.width,viewport_.height,ofIsVFlipped());
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::viewport(float x, float y, float width, float height, bool vflip) {
	matrixStack.viewport(x,y,width,height,vflip);
	ofRectangle nativeViewport = matrixStack.getNativeViewport();
	glViewport(nativeViewport.x,nativeViewport.y,nativeViewport.width,nativeViewport.height);
}

//----------------------------------------------------------
ofRectangle ofProgrammableGLRenderer::getCurrentViewport(){
	getNativeViewport();
	return matrixStack.getCurrentViewport();
}

//----------------------------------------------------------
ofRectangle ofProgrammableGLRenderer::getNativeViewport(){
	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);

	ofRectangle nativeViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	matrixStack.nativeViewport(nativeViewport);
    return nativeViewport;
}

//----------------------------------------------------------
int ofProgrammableGLRenderer::getViewportWidth(){
	return getCurrentViewport().width;
}

//----------------------------------------------------------
int ofProgrammableGLRenderer::getViewportHeight(){
	return getCurrentViewport().height;
}

//----------------------------------------------------------
bool ofProgrammableGLRenderer::isVFlipped() const{
	return matrixStack.isVFlipped();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setCoordHandedness(ofHandednessType handedness) {
	;
}

//----------------------------------------------------------
ofHandednessType ofProgrammableGLRenderer::getCoordHandedness() {
	return matrixStack.getHandedness();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setOrientation(ofOrientation orientation, bool vFlip){
	matrixStack.setOrientation(orientation,vFlip);
	uploadAllMatrices();

}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setupScreenPerspective(float width, float height, float fov, float nearDist, float farDist) {
	ofRectangle currentViewport = getCurrentViewport();
	
	float viewW = currentViewport.width;
	float viewH = currentViewport.height;

	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) viewW / viewH;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;


	matrixMode(OF_MATRIX_PROJECTION);
	ofMatrix4x4 persp;
	persp.makePerspectiveMatrix(fov, aspect, nearDist, farDist);
	loadMatrix( persp );

	matrixMode(OF_MATRIX_MODELVIEW);
	ofMatrix4x4 lookAt;
	lookAt.makeLookAtViewMatrix( ofVec3f(eyeX, eyeY, dist),  ofVec3f(eyeX, eyeY, 0),  ofVec3f(0, 1, 0) );
	loadMatrix(lookAt);
	
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setupScreenOrtho(float width, float height, float nearDist, float farDist) {
	ofRectangle currentViewport = getCurrentViewport();

	float viewW = currentViewport.width;
	float viewH = currentViewport.height;

	ofMatrix4x4 ortho;

	ortho = ofMatrix4x4::newOrthoMatrix(0, viewW, 0, viewH, nearDist, farDist);

	matrixMode(OF_MATRIX_PROJECTION);
	loadMatrix(ortho); // make ortho our new projection matrix.

	matrixMode(OF_MATRIX_MODELVIEW);
	loadIdentityMatrix();
}

//----------------------------------------------------------
//Resets openGL parameters back to OF defaults
void ofProgrammableGLRenderer::setupGraphicDefaults(){
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setupScreen(){
	beginDefaultShader();
	setupScreenPerspective();	// assume defaults
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setCircleResolution(int res){
	if((int)circlePolyline.size()!=res+1){
		circlePolyline.clear();
		circlePolyline.arc(0,0,0,1,1,0,360,res);
		circleVbo.getVertices() = circlePolyline.getVertices();
	}
}

//our openGL wrappers
//----------------------------------------------------------
void ofProgrammableGLRenderer::pushMatrix(){
	matrixStack.pushMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::popMatrix(){
	matrixStack.popMatrix();
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::translate(const ofVec3f& p){
	translate(p.x, p.y, p.z);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::translate(float x, float y, float z){
	matrixStack.translate(x,y,z);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::scale(float xAmnt, float yAmnt, float zAmnt){
	matrixStack.scale(xAmnt, yAmnt, zAmnt);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::rotate(float degrees, float vecX, float vecY, float vecZ){
	matrixStack.rotate(degrees, vecX, vecY, vecZ);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::rotateX(float degrees){
	rotate(degrees, 1, 0, 0);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::rotateY(float degrees){
	rotate(degrees, 0, 1, 0);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::rotateZ(float degrees){
	rotate(degrees, 0, 0, 1);
}

//same as ofRotateZ
//----------------------------------------------------------
void ofProgrammableGLRenderer::rotate(float degrees){
	rotateZ(degrees);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::matrixMode(ofMatrixMode mode){
	matrixStack.matrixMode(mode);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::loadIdentityMatrix (void){
	matrixStack.loadIdentityMatrix();
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::loadMatrix (const ofMatrix4x4 & m){
	matrixStack.loadMatrix(m.getPtr());
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::loadMatrix (const float *m){
	matrixStack.loadMatrix(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::multMatrix (const ofMatrix4x4 & m){
	multMatrix(m.getPtr());
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::multMatrix (const float *m){
	matrixStack.multMatrix(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::uploadCurrentMatrix(){
	if(!currentShader) return;
	// uploads the current matrix to the current shader.
	switch(matrixStack.getCurrentMatrixMode()){
	case OF_MATRIX_MODELVIEW:
		currentShader->setUniformMatrix4f(MODELVIEW_MATRIX_UNIFORM, matrixStack.getModelViewMatrix());
		currentShader->setUniformMatrix4f(MODELVIEW_PROJECTION_MATRIX_UNIFORM, matrixStack.getModelViewPorjectionMatrix());
		break;
	case OF_MATRIX_PROJECTION:
		currentShader->setUniformMatrix4f(PROJECTION_MATRIX_UNIFORM, matrixStack.getProjectionMatrix());
		currentShader->setUniformMatrix4f(MODELVIEW_PROJECTION_MATRIX_UNIFORM, matrixStack.getModelViewPorjectionMatrix());
		break;
	case OF_MATRIX_TEXTURE:
		currentShader->setUniformMatrix4f(TEXTURE_MATRIX_UNIFORM, matrixStack.getTextureMatrix());
		break;
	}

}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setColor(const ofColor & color){
	setColor(color.r,color.g,color.b,color.a);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setColor(const ofColor & color, int _a){
	setColor(color.r,color.g,color.b,_a);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setColor(int _r, int _g, int _b){
	setColor(_r, _g, _b, 255);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setColor(int _r, int _g, int _b, int _a){
	currentColor.set(_r/255.f,_g/255.f,_b/255.f,_a/255.f);
	if(currentShader) currentShader->setUniform4f(COLOR_UNIFORM,currentColor.r,currentColor.g,currentColor.b,currentColor.a);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setColor(int gray){
	setColor(gray, gray, gray);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setHexColor(int hexColor){
	int r = (hexColor >> 16) & 0xff;
	int g = (hexColor >> 8) & 0xff;
	int b = (hexColor >> 0) & 0xff;
	setColor(r,g,b);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::clear(float r, float g, float b, float a) {
	glClearColor(r / 255., g / 255., b / 255., a / 255.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::clear(float brightness, float a) {
	clear(brightness, brightness, brightness, a);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::clearAlpha() {
	glColorMask(0, 0, 0, 1);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColorMask(1, 1, 1, 1);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setBackgroundAuto(bool bAuto){
	bBackgroundAuto = bAuto;
}

//----------------------------------------------------------
bool ofProgrammableGLRenderer::bClearBg(){
	return bBackgroundAuto;
}

//----------------------------------------------------------
ofFloatColor & ofProgrammableGLRenderer::getBgColor(){
	return bgColor;
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::background(const ofColor & c){
	bgColor = c;
	glClearColor(bgColor[0],bgColor[1],bgColor[2], bgColor[3]);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::background(float brightness) {
	background(brightness);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::background(int hexColor, float _a){
	background ( (hexColor >> 16) & 0xff, (hexColor >> 8) & 0xff, (hexColor >> 0) & 0xff, _a);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::background(int r, int g, int b, int a){
	background(ofColor(r,g,b,a));
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setFillMode(ofFillFlag fill){
	bFilled = fill;
}

//----------------------------------------------------------
ofFillFlag ofProgrammableGLRenderer::getFillMode(){
	return bFilled;
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setRectMode(ofRectMode mode){
	rectMode = mode;
}

//----------------------------------------------------------
ofRectMode ofProgrammableGLRenderer::getRectMode(){
	return rectMode;
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setLineWidth(float lineWidth){
	// tig: glLinewidth is 'kind of' deprecated.
	// http://www.opengl.org/registry/doc/glspec32.core.20090803.pdf
	// p.330: "LineWidth values greater than 1.0 will generate an
	// INVALID_VALUE error".
	// use geometry shaders to draw lines of varying thickness...
	
#ifndef TARGET_OPENGLES
	//ofLogVerbose() << "glLineWidth has no effect in openGL3.2+\nUse a geometry shader to generate thick lines.";
#else
	glLineWidth(lineWidth);
#endif
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setLineSmoothing(bool smooth){
	bSmoothHinted = smooth;
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::startSmoothing(){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::endSmoothing(){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setBlendMode(ofBlendMode blendMode){
	switch (blendMode){
		case OF_BLENDMODE_DISABLED:
			glDisable(GL_BLEND);
			break;

		case OF_BLENDMODE_ALPHA:
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;

		case OF_BLENDMODE_ADD:
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;

		case OF_BLENDMODE_MULTIPLY:
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA /* GL_ZERO or GL_ONE_MINUS_SRC_ALPHA */);
			break;

		case OF_BLENDMODE_SCREEN:
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
			break;

		case OF_BLENDMODE_SUBTRACT:
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;

		default:
			break;
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::enablePointSprites(){
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::disablePointSprites(){
}

//----------------------------------------------------------
ofShader & ofProgrammableGLRenderer::getCurrentShader(){
	return *currentShader;
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::enableVertices(){
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::enableTexCoords(){
	texCoordsEnabled = true;
	beginDefaultShader();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::enableColors(){
	colorsEnabled = true;
	beginDefaultShader();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::enableNormals(){
	normalsEnabled = true;
}

void ofProgrammableGLRenderer::disableVertices(){
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::disableTexCoords(){
	texCoordsEnabled = false;
	beginDefaultShader();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::disableColors(){
	colorsEnabled = false;
	beginDefaultShader();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::disableNormals(){
	normalsEnabled = false;
}

void ofProgrammableGLRenderer::enableTextureTarget(int textureTarget){
	currentTextureTarget = textureTarget;
	beginDefaultShader();
}

void ofProgrammableGLRenderer::disableTextureTarget(int textureTarget){
	currentTextureTarget = OF_NO_TEXTURE;
	beginDefaultShader();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::beginCustomShader(ofShader & shader){
	if(currentShader && *currentShader==shader){
		return;
	}

	shader.setUniform1f(USE_TEXTURECOORDS_UNIFORM,texCoordsEnabled);
	shader.setUniform1f(USE_COLORS_UNIFORM,colorsEnabled);
	shader.setUniform4f(COLOR_UNIFORM,currentColor.r,currentColor.g,currentColor.b,currentColor.a);

	currentShader = &shader;
	uploadAllMatrices();
	if(!settingDefaultShader) usingCustomShader = true;
}

void ofProgrammableGLRenderer::uploadAllMatrices(){
	if(!currentShader) return;
	currentShader->setUniformMatrix4f(MODELVIEW_MATRIX_UNIFORM, matrixStack.getModelViewMatrix());
	currentShader->setUniformMatrix4f(PROJECTION_MATRIX_UNIFORM, matrixStack.getProjectionMatrix());
	currentShader->setUniformMatrix4f(TEXTURE_MATRIX_UNIFORM, matrixStack.getTextureMatrix());
	currentShader->setUniformMatrix4f(MODELVIEW_PROJECTION_MATRIX_UNIFORM, matrixStack.getModelViewPorjectionMatrix());
}

void ofProgrammableGLRenderer::beginDefaultShader(){
	if(usingCustomShader) return;
	ofShader * nextShader = NULL;

	if(colorsEnabled && texCoordsEnabled){
		switch(currentTextureTarget){
		case GL_TEXTURE_RECTANGLE_ARB:
			nextShader = &ofShader::defaultTexColor();
			break;
		case GL_TEXTURE_2D:
			nextShader = &ofShader::defaultTex2DColor();
			break;
		case OF_NO_TEXTURE:
			nextShader = &ofShader::defaultNoTexColor();
			break;
		}
	}else if(colorsEnabled){
		nextShader = &ofShader::defaultNoTexColor();
	}else if(texCoordsEnabled){
		switch(currentTextureTarget){
		case GL_TEXTURE_RECTANGLE_ARB:
			nextShader = &ofShader::defaultTexNoColor();
			break;
		case GL_TEXTURE_2D:
			nextShader = &ofShader::defaultTex2DNoColor();
			break;
		case OF_NO_TEXTURE:
			nextShader = &ofShader::defaultNoTexNoColor();
			break;
		}
	}else{
		nextShader = &ofShader::defaultNoTexNoColor();
	}

	if(nextShader && (!currentShader || *currentShader!=*nextShader)){
		settingDefaultShader = true;
		nextShader->begin();
		settingDefaultShader = false;
	}else{
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::endCustomShader(){
	usingCustomShader = false;
	beginDefaultShader();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2){
	lineVbo.getVertices()[0].set(x1,y1,z1);
	lineVbo.getVertices()[1].set(x2,y2,z2);
    
	// use smoothness, if requested:
	if (bSmoothHinted) startSmoothing();
    
	lineVbo.draw();
    
	// use smoothness, if requested:
	if (bSmoothHinted) endSmoothing();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::drawRectangle(float x, float y, float z, float w, float h) {
	if (rectMode == OF_RECTMODE_CORNER){
		rectVbo.getVertices()[0].set(x,y,z);
		rectVbo.getVertices()[1].set(x+w, y, z);
		rectVbo.getVertices()[2].set(x+w, y+h, z);
		rectVbo.getVertices()[3].set(x, y+h, z);
	}else{
		rectVbo.getVertices()[0].set(x-w/2.0f, y-h/2.0f, z);
		rectVbo.getVertices()[1].set(x+w/2.0f, y-h/2.0f, z);
		rectVbo.getVertices()[2].set(x+w/2.0f, y+h/2.0f, z);
		rectVbo.getVertices()[3].set(x-w/2.0f, y+h/2.0f, z);
	}
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	rectVbo.setMode((bFilled == OF_FILLED) ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_LOOP);
	rectVbo.draw();
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	triangleVbo.getVertices()[0].set(x1,y1,z1);
	triangleVbo.getVertices()[1].set(x2,y2,z2);
	triangleVbo.getVertices()[2].set(x3,y3,z3);
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	triangleVbo.setMode((bFilled == OF_FILLED) ? OF_PRIMITIVE_TRIANGLE_STRIP : OF_PRIMITIVE_LINE_LOOP);
	triangleVbo.draw();
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::drawCircle(float x, float y, float z,  float radius){
	vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		circleVbo.getVertices()[i].set(radius*circleCache[i].x+x,radius*circleCache[i].y+y,z);
	}
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	circleVbo.setMode((bFilled == OF_FILLED) ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_STRIP);
	circleVbo.draw();
	
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::drawEllipse(float x, float y, float z, float width, float height){
	float radiusX = width*0.5;
	float radiusY = height*0.5;
	vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		circleVbo.getVertices()[i].set(radiusX*circlePolyline[i].x+x,radiusY*circlePolyline[i].y+y,z);
	}
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	circleVbo.setMode((bFilled == OF_FILLED) ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_STRIP);
	circleVbo.draw();
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::drawString(string textString, float x, float y, float z, ofDrawBitmapMode mode){

	// remember the current blend mode so that we can restore it at the end of this method.
	ofBlendMode previousBlendMode = ofGetStyle().blendingMode;

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

	switch (mode) {

		case OF_BITMAPMODE_SIMPLE:

			sx += x;
			sy += y;
			break;

		case OF_BITMAPMODE_SCREEN:

			hasViewport = true;
			pushView();

			rViewport = ofGetWindowRect();
			viewport(rViewport);

			matrixMode(OF_MATRIX_PROJECTION);
			loadIdentityMatrix();
			matrixMode(OF_MATRIX_MODELVIEW);
			loadIdentityMatrix();

			translate(-1, 1, 0);
			scale(2/rViewport.width, -2/rViewport.height, 1);

			translate(x, y, 0);
			break;

		case OF_BITMAPMODE_VIEWPORT:

			rViewport = getCurrentViewport();

			hasProjection = true;
			matrixMode(OF_MATRIX_PROJECTION);
			pushMatrix();
			loadIdentityMatrix();

			hasModelView = true;
			matrixMode(OF_MATRIX_MODELVIEW);
			pushMatrix();
			loadIdentityMatrix();

			translate(-1, 1, 0);
			scale(2/rViewport.width, -2/rViewport.height, 1);

			translate(x, y, 0);
			break;

		case OF_BITMAPMODE_MODEL:

			hasModelView = true;
			matrixMode(OF_MATRIX_MODELVIEW);
			pushMatrix();

			translate(x, y, z);
			break;

		case OF_BITMAPMODE_MODEL_BILLBOARD:
		{
			//our aim here is to draw to screen
			//at the viewport position related
			//to the world position x,y,z

			// tig: we want to get the signed normalised screen coordinates (-1,+1) of our point (x,y,z)
			// that's projection * modelview * point in GLSL multiplication order
			// then doing the good old (v + 1.0) / 2. to get unsigned normalized screen (0,1) coordinates.
			// we then multiply x by width and y by height to get window coordinates.

			// previous implementations used gluProject, which made it incompatible with GLES (and the future)
			// https://developer.apple.com/library/mac/#documentation/Darwin/Reference/ManPages/man3/gluProject.3.html
			//
			// this could probably be backported to the GL2 Renderer =)
			
			rViewport = getCurrentViewport();
			
			ofVec3f dScreen = ofVec3f(x,y,z) * matrixStack.getModelViewMatrix() * matrixStack.getProjectionMatrixNoOrientation();
			dScreen += ofVec3f(1.0) ;
			dScreen *= 0.5;
			
			dScreen.x += rViewport.x;
			dScreen.x *= rViewport.width;

			dScreen.y += rViewport.y;
			dScreen.y *= rViewport.height;
			
			if (dScreen.z >= 1) return;


			hasProjection = true;
			matrixMode(OF_MATRIX_PROJECTION);
			pushMatrix();
			loadIdentityMatrix();

			hasModelView = true;
			matrixMode(OF_MATRIX_MODELVIEW);
			pushMatrix();
			loadIdentityMatrix();

			translate(-1, -1, 0);

			scale(2/rViewport.width, 2/rViewport.height, 1);

			translate(dScreen.x, dScreen.y, 0);
		}
			break;

		default:
			break;
	}
	//
	///////////////////////////

	// tig: we switch over to our built-in bitmapstring shader
	// to render text. This gives us more flexibility & control
	// and does not mess/interfere with client side shaders.
	
	ofShader::bitmapStringShader().begin();

	// (c) enable texture once before we start drawing each char (no point turning it on and off constantly)
	//We do this because its way faster
	ofDrawBitmapCharacterStart(textString.size());

	for(int c = 0; c < len; c++){
		if(textString[c] == '\n'){

			sy += bOrigin ? -1 : 1 * (fontSize*1.7);
			if(mode == OF_BITMAPMODE_SIMPLE) {
				sx = x;
			} else {
				sx = 0;
			}

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
	ofShader::bitmapStringShader().end();


	if (hasModelView)
		popMatrix();

	if (hasProjection)
	{
		matrixMode(OF_MATRIX_PROJECTION);
		popMatrix();
		matrixMode(OF_MATRIX_MODELVIEW);
	}

	if (hasViewport)
		popView();

	// restore blendmode
	ofEnableBlendMode(previousBlendMode);
}
