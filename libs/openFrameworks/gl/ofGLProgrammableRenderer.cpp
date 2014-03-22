#include "ofGLProgrammableRenderer.h"
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
static const string MODELVIEW_PROJECTION_MATRIX_UNIFORM="modelViewProjectionMatrix";
static const string TEXTURE_MATRIX_UNIFORM="textureMatrix";
static const string COLOR_UNIFORM="globalColor";

static const string USE_TEXTURE_UNIFORM="usingTexture";
static const string USE_COLORS_UNIFORM="usingColors";
static const string BITMAP_STRING_UNIFORM="bitmapText";


const string ofGLProgrammableRenderer::TYPE="ProgrammableGL";


//----------------------------------------------------------
ofGLProgrammableRenderer::ofGLProgrammableRenderer(bool useShapeColor)
:matrixStack(*ofGetWindowPtr())
{
	bBackgroundAuto = true;

	lineMesh.getVertices().resize(2);
	lineMesh.setMode(OF_PRIMITIVE_LINES);
	triangleMesh.getVertices().resize(3);
	rectMesh.getVertices().resize(4);


    rectMode = OF_RECTMODE_CORNER;
    bFilled = OF_FILLED;
    bSmoothHinted = false;

	bitmapStringEnabled = false;
    verticesEnabled = true;
    colorsEnabled = false;
    texCoordsEnabled = false;
    normalsEnabled = false;
	settingDefaultShader = false;
	usingCustomShader = false;

	wrongUseLoggedOnce = false;

	uniqueShader = false;

	currentShader = NULL;

	currentTextureTarget = OF_NO_TEXTURE;
}

ofGLProgrammableRenderer::~ofGLProgrammableRenderer() {
	
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::startRender() {
	matrixStack.setRenderSurface(*ofGetWindowPtr());
	beginDefaultShader();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::finishRender() {
	if (!uniqueShader) {
		glUseProgram(0);
		if(!usingCustomShader) currentShader = NULL;
	}
	
	matrixStack.clearStacks();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::update(){
    //
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(ofMesh & vertexData, bool useColors, bool useTextures, bool useNormals){
	draw(vertexData, OF_MESH_FILL, useColors, useTextures, useNormals); // tig: use default mode if no render mode specified.
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals){
	if (vertexData.getVertices().empty()) return;
	
	
	// tig: note that for GL3+ we use glPolygonMode to draw wireframes or filled meshes, and not the primitive mode.
	// the reason is not purely aesthetic, but more conformant with the behaviour of ofGLRenderer. Whereas
	// gles2.0 doesn't allow for a polygonmode.
	// Also gles2 still supports vertex array syntax for uploading data to attributes and it seems to be faster than
	// vbo's for meshes that are updated frequently so let's use that instead
	
	if (bSmoothHinted) startSmoothing();

#ifdef TARGET_OPENGLES
	glEnableVertexAttribArray(ofShader::POSITION_ATTRIBUTE);
	glVertexAttribPointer(ofShader::POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, sizeof(ofVec3f), vertexData.getVerticesPointer());
	
	useNormals &= (vertexData.getNumNormals()>0);
	if(useNormals){
		glEnableVertexAttribArray(ofShader::NORMAL_ATTRIBUTE);
		glVertexAttribPointer(ofShader::NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, sizeof(ofVec3f), vertexData.getNormalsPointer());
	}else{
		glDisableVertexAttribArray(ofShader::NORMAL_ATTRIBUTE);
	}
	
	useColors &= (vertexData.getNumColors()>0);
	if(useColors){
		glEnableVertexAttribArray(ofShader::COLOR_ATTRIBUTE);
		glVertexAttribPointer(ofShader::COLOR_ATTRIBUTE, 4,GL_FLOAT, GL_FALSE, sizeof(ofFloatColor), vertexData.getColorsPointer());
	}else{
		glDisableVertexAttribArray(ofShader::COLOR_ATTRIBUTE);
	}

	useTextures &= (vertexData.getNumTexCoords()>0);
	if(useTextures){
		glEnableVertexAttribArray(ofShader::TEXCOORD_ATTRIBUTE);
		glVertexAttribPointer(ofShader::TEXCOORD_ATTRIBUTE,2, GL_FLOAT, GL_FALSE, sizeof(ofVec2f), vertexData.getTexCoordsPointer());
	}else{
		glDisableVertexAttribArray(ofShader::TEXCOORD_ATTRIBUTE);
	}


	setAttributes(true,useColors,useTextures,useNormals);

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
		drawMode = ofGetGLPrimitiveMode(vertexData.getMode());
		break;
	}

	if(vertexData.getNumIndices()){
		glDrawElements(drawMode, vertexData.getNumIndices(),GL_UNSIGNED_SHORT,vertexData.getIndexPointer());
	}else{
		glDrawArrays(drawMode, 0, vertexData.getNumVertices());
	}
#else

	// OpenGL
	/*  some things are internally still using ofMesh
	if(!wrongUseLoggedOnce){
		ofLogWarning("ofGLProgrammableRenderer") << "draw(): drawing an ofMesh or ofPolyline directly is deprecated, use a ofVboMesh";
		wrongUseLoggedOnce = true;
	}*/
	
	meshVbo.setMesh(vertexData, GL_DYNAMIC_DRAW, useColors, useTextures, useNormals);

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
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw( of3dPrimitive& model, ofPolyRenderMode renderType) {
	model.getMesh().draw(renderType);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(ofPolyline & poly){
	/*  some things are internally still using ofPolyline
	if(!wrongUseLoggedOnce){
		ofLogWarning("ofGLProgrammableRenderer") << "draw(): drawing an ofMesh or ofPolyline directly is deprecated, use a ofVboMesh or ofPath";
		wrongUseLoggedOnce = true;
	}*/
	if(poly.getVertices().empty()) return;

	// use smoothness, if requested:
	if (bSmoothHinted) startSmoothing();

#ifdef TARGET_OPENGLES

	glEnableVertexAttribArray(ofShader::POSITION_ATTRIBUTE);
	glVertexAttribPointer(ofShader::POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, sizeof(ofVec3f), &poly[0]);

	setAttributes(true,false,false,false);

	GLenum drawMode = poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP;

	glDrawArrays(drawMode, 0, poly.size());

#else

	vertexDataVbo.setVertexData(&poly.getVertices()[0], poly.size(), GL_DYNAMIC_DRAW);
	vertexDataVbo.draw(poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, poly.size());

#endif
	// use smoothness, if requested:
	if (bSmoothHinted) endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(ofPath & shape){
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
void ofGLProgrammableRenderer::draw(ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	if(image.isUsingTexture()){
		setAttributes(true,false,true,false);
		ofTexture& tex = image.getTextureReference();
		if(tex.bAllocated()) {
			tex.drawSubsection(x,y,z,w,h,sx,sy,sw,sh);
		} else {
			ofLogWarning("ofGLProgrammableRenderer") << "draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	if(image.isUsingTexture()){
		setAttributes(true,false,true,false);
		ofTexture& tex = image.getTextureReference();
		if(tex.bAllocated()) {
			tex.drawSubsection(x,y,z,w,h,sx,sy,sw,sh);
		} else {
			ofLogWarning("ofGLProgrammableRenderer") << "draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	if(image.isUsingTexture()){
		setAttributes(true,false,true,false);
		ofTexture& tex = image.getTextureReference();
		if(tex.bAllocated()) {
			tex.drawSubsection(x,y,z,w,h,sx,sy,sw,sh);
		} else {
			ofLogWarning("ofGLProgrammableRenderer") << "draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setCurrentFBO(ofFbo * fbo){
	if(fbo!=NULL){
		matrixStack.setRenderSurface(*fbo);
		uploadMatrices();
	}else{
		matrixStack.setRenderSurface(*ofGetWindowPtr());
		uploadMatrices();
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::pushView() {
	matrixStack.pushView();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::popView() {
	matrixStack.popView();
	uploadMatrices();
	viewport(matrixStack.getCurrentViewport());
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::viewport(ofRectangle viewport_){
	viewport(viewport_.x,viewport_.y,viewport_.width,viewport_.height,ofIsVFlipped());
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::viewport(float x, float y, float width, float height, bool vflip) {
	matrixStack.viewport(x,y,width,height,vflip);
	ofRectangle nativeViewport = matrixStack.getNativeViewport();
	glViewport(nativeViewport.x,nativeViewport.y,nativeViewport.width,nativeViewport.height);
}

//----------------------------------------------------------
ofRectangle ofGLProgrammableRenderer::getCurrentViewport(){
	getNativeViewport();
	return matrixStack.getCurrentViewport();
}

//----------------------------------------------------------
ofRectangle ofGLProgrammableRenderer::getNativeViewport(){
	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);

	ofRectangle nativeViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	matrixStack.nativeViewport(nativeViewport);
    return nativeViewport;
}

//----------------------------------------------------------
int ofGLProgrammableRenderer::getViewportWidth(){
	return getCurrentViewport().width;
}

//----------------------------------------------------------
int ofGLProgrammableRenderer::getViewportHeight(){
	return getCurrentViewport().height;
}

//----------------------------------------------------------
bool ofGLProgrammableRenderer::isVFlipped() const{
	return matrixStack.isVFlipped();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setCoordHandedness(ofHandednessType handedness) {
	;
}

//----------------------------------------------------------
ofHandednessType ofGLProgrammableRenderer::getCoordHandedness() {
	return matrixStack.getHandedness();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setOrientation(ofOrientation orientation, bool vFlip){
	matrixStack.setOrientation(orientation,vFlip);
	uploadMatrices();

}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setupScreenPerspective(float width, float height, float fov, float nearDist, float farDist) {
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
void ofGLProgrammableRenderer::setupScreenOrtho(float width, float height, float nearDist, float farDist) {
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
void ofGLProgrammableRenderer::setupGraphicDefaults(){
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setupScreen(){
	beginDefaultShader();
	setupScreenPerspective();	// assume defaults
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setCircleResolution(int res){
	if((int)circlePolyline.size()!=res+1){
		circlePolyline.clear();
		circlePolyline.arc(0,0,0,1,1,0,360,res);
		circleMesh.getVertices() = circlePolyline.getVertices();
	}
}

//our openGL wrappers
//----------------------------------------------------------
void ofGLProgrammableRenderer::pushMatrix(){
	matrixStack.pushMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::popMatrix(){
	matrixStack.popMatrix();
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::translate(const ofVec3f& p){
	translate(p.x, p.y, p.z);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::translate(float x, float y, float z){
	matrixStack.translate(x,y,z);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::scale(float xAmnt, float yAmnt, float zAmnt){
	matrixStack.scale(xAmnt, yAmnt, zAmnt);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::rotate(float degrees, float vecX, float vecY, float vecZ){
	matrixStack.rotate(degrees, vecX, vecY, vecZ);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::rotateX(float degrees){
	rotate(degrees, 1, 0, 0);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::rotateY(float degrees){
	rotate(degrees, 0, 1, 0);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::rotateZ(float degrees){
	rotate(degrees, 0, 0, 1);
}

//same as ofRotateZ
//----------------------------------------------------------
void ofGLProgrammableRenderer::rotate(float degrees){
	rotateZ(degrees);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::matrixMode(ofMatrixMode mode){
	matrixStack.matrixMode(mode);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::loadIdentityMatrix (void){
	matrixStack.loadIdentityMatrix();
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::loadMatrix (const ofMatrix4x4 & m){
	matrixStack.loadMatrix(m.getPtr());
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::loadMatrix (const float *m){
	matrixStack.loadMatrix(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::multMatrix (const ofMatrix4x4 & m){
	multMatrix(m.getPtr());
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::multMatrix (const float *m){
	matrixStack.multMatrix(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::uploadCurrentMatrix(){
	if(!currentShader) return;
	// uploads the current matrix to the current shader.
	switch(matrixStack.getCurrentMatrixMode()){
	case OF_MATRIX_MODELVIEW:
		currentShader->setUniformMatrix4f(MODELVIEW_MATRIX_UNIFORM, matrixStack.getModelViewMatrix());
		currentShader->setUniformMatrix4f(MODELVIEW_PROJECTION_MATRIX_UNIFORM, matrixStack.getModelViewProjectionMatrix());
		break;
	case OF_MATRIX_PROJECTION:
		currentShader->setUniformMatrix4f(PROJECTION_MATRIX_UNIFORM, matrixStack.getProjectionMatrix());
		currentShader->setUniformMatrix4f(MODELVIEW_PROJECTION_MATRIX_UNIFORM, matrixStack.getModelViewProjectionMatrix());
		break;
	case OF_MATRIX_TEXTURE:
		currentShader->setUniformMatrix4f(TEXTURE_MATRIX_UNIFORM, matrixStack.getTextureMatrix());
		break;
	}

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
ofMatrix4x4 ofGLProgrammableRenderer::getCurrentMatrix(ofMatrixMode matrixMode_) const {
	switch (matrixMode_) {
		case OF_MATRIX_MODELVIEW:
			return matrixStack.getModelViewMatrix();
			break;
		case OF_MATRIX_PROJECTION:
			return matrixStack.getProjectionMatrix();
			break;
		case OF_MATRIX_TEXTURE:
			return matrixStack.getTextureMatrix();
			break;
	}
}

//----------------------------------------------------------

//----------------------------------------------------------
void ofGLProgrammableRenderer::setColor(const ofColor & color){
	setColor(color.r,color.g,color.b,color.a);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setColor(const ofColor & color, int _a){
	setColor(color.r,color.g,color.b,_a);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setColor(int _r, int _g, int _b){
	setColor(_r, _g, _b, 255);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setColor(int _r, int _g, int _b, int _a){
	ofColor newColor(_r,_g,_b,_a);
	if(newColor!=currentColor){
		currentColor = newColor;
		if(currentShader){
			currentShader->setUniform4f(COLOR_UNIFORM,_r/255.,_g/255.,_b/255.,_a/255.);
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setColor(int gray){
	setColor(gray, gray, gray);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setHexColor(int hexColor){
	int r = (hexColor >> 16) & 0xff;
	int g = (hexColor >> 8) & 0xff;
	int b = (hexColor >> 0) & 0xff;
	setColor(r,g,b);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::clear(float r, float g, float b, float a) {
	glClearColor(r / 255., g / 255., b / 255., a / 255.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::clear(float brightness, float a) {
	clear(brightness, brightness, brightness, a);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::clearAlpha() {
	glColorMask(0, 0, 0, 1);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColorMask(1, 1, 1, 1);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setBackgroundAuto(bool bAuto){
	bBackgroundAuto = bAuto;
}

//----------------------------------------------------------
bool ofGLProgrammableRenderer::bClearBg(){
	return bBackgroundAuto;
}

//----------------------------------------------------------
ofFloatColor & ofGLProgrammableRenderer::getBgColor(){
	return bgColor;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::background(const ofColor & c){
	bgColor = c;
	glClearColor(bgColor[0],bgColor[1],bgColor[2], bgColor[3]);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::background(float brightness) {
	background(brightness);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::background(int hexColor, float _a){
	background ( (hexColor >> 16) & 0xff, (hexColor >> 8) & 0xff, (hexColor >> 0) & 0xff, _a);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::background(int r, int g, int b, int a){
	background(ofColor(r,g,b,a));
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setFillMode(ofFillFlag fill){
	bFilled = fill;
}

//----------------------------------------------------------
ofFillFlag ofGLProgrammableRenderer::getFillMode(){
	return bFilled;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setRectMode(ofRectMode mode){
	rectMode = mode;
}

//----------------------------------------------------------
ofRectMode ofGLProgrammableRenderer::getRectMode(){
	return rectMode;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setLineWidth(float lineWidth){
	// tig: glLinewidth is 'kind of' deprecated.
	// http://www.opengl.org/registry/doc/glspec32.core.20090803.pdf
	// p.330: "LineWidth values greater than 1.0 will generate an
	// INVALID_VALUE error".
	// use geometry shaders to draw lines of varying thickness...
	
#ifndef TARGET_OPENGLES
	//ofLogVerbose("ofGLProgrammableRenderer") << "setLineWidth(): has no effect in OpenGL 3.2+;
	//<< "use a geometry shader to generate thick lines";
#else
	glLineWidth(lineWidth);
#endif
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setDepthTest(bool depthTest) {
	if(depthTest) {
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_DEPTH_TEST);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setLineSmoothing(bool smooth){
	bSmoothHinted = smooth;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::startSmoothing(){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::endSmoothing(){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setBlendMode(ofBlendMode blendMode){
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
void ofGLProgrammableRenderer::enablePointSprites(){
#ifdef TARGET_OPENGLES
        glEnable(GL_POINT_SPRITE_OES);
#else
	glEnable(GL_PROGRAM_POINT_SIZE);
#endif
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disablePointSprites(){
#ifdef TARGET_OPENGLES
        glEnable(GL_POINT_SPRITE_OES);
#else
	glDisable(GL_PROGRAM_POINT_SIZE);
#endif
}


//----------------------------------------------------------
void ofGLProgrammableRenderer::enableAntiAliasing(){
	glEnable(GL_MULTISAMPLE);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disableAntiAliasing(){
	glDisable(GL_MULTISAMPLE);
}

//----------------------------------------------------------
ofShader & ofGLProgrammableRenderer::getCurrentShader(){
	return *currentShader;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setAlphaBitmapText(bool bitmapText){
	bool wasBitmapStringEnabled = bitmapStringEnabled;
	bitmapStringEnabled = bitmapText;

	if(wasBitmapStringEnabled!=bitmapText){
		if(currentShader) currentShader->setUniform1f(BITMAP_STRING_UNIFORM,bitmapText);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setAttributes(bool vertices, bool color, bool tex, bool normals){
	bool wasColorsEnabled = colorsEnabled;
	bool wasUsingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);

	texCoordsEnabled = tex;
	colorsEnabled=color;
	normalsEnabled = normals;

	if(!uniqueShader){
		beginDefaultShader();
	}

	bool usingTexture = tex & (currentTextureTarget!=OF_NO_TEXTURE);
	if(wasUsingTexture!=usingTexture){
		if(currentShader) currentShader->setUniform1f(USE_TEXTURE_UNIFORM,usingTexture);
	}
	if(wasColorsEnabled!=color){
		if(currentShader) currentShader->setUniform1f(USE_COLORS_UNIFORM,color);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::enableTextureTarget(int textureTarget){
	bool wasUsingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);
	currentTextureTarget = textureTarget;

	if(!uniqueShader){
		beginDefaultShader();
	}

	bool usingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);
	if(wasUsingTexture!=usingTexture){
		if(currentShader) currentShader->setUniform1f(USE_TEXTURE_UNIFORM,usingTexture);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disableTextureTarget(int textureTarget){
	bool wasUsingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);
	currentTextureTarget = OF_NO_TEXTURE;

	if(!uniqueShader){
		beginDefaultShader();
	}

	bool usingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);
	if(wasUsingTexture!=usingTexture){
		if(currentShader) currentShader->setUniform1f(USE_TEXTURE_UNIFORM,usingTexture);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::beginCustomShader(ofShader & shader){
	if(currentShader && *currentShader==shader){
		return;
	}

	currentShader = &shader;
	uploadMatrices();
	setDefaultUniforms();
	if(!settingDefaultShader){
		usingCustomShader = true;
	}
}

void ofGLProgrammableRenderer::uploadMatrices(){
	if(!currentShader) return;
	currentShader->setUniformMatrix4f(MODELVIEW_MATRIX_UNIFORM, matrixStack.getModelViewMatrix());
	currentShader->setUniformMatrix4f(PROJECTION_MATRIX_UNIFORM, matrixStack.getProjectionMatrix());
	currentShader->setUniformMatrix4f(TEXTURE_MATRIX_UNIFORM, matrixStack.getTextureMatrix());
	currentShader->setUniformMatrix4f(MODELVIEW_PROJECTION_MATRIX_UNIFORM, matrixStack.getModelViewProjectionMatrix());
}

void ofGLProgrammableRenderer::setDefaultUniforms(){
	if(!currentShader) return;
	currentShader->setUniform4f(COLOR_UNIFORM, currentColor.r/255.,currentColor.g/255.,currentColor.b/255.,currentColor.a/255.);
	bool usingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);
	currentShader->setUniform1f(USE_TEXTURE_UNIFORM,usingTexture);
	currentShader->setUniform1f(USE_COLORS_UNIFORM,colorsEnabled);
}

void ofGLProgrammableRenderer::beginDefaultShader(){
	if(usingCustomShader)	return;

	ofShader * nextShader = NULL;

	if(!uniqueShader){
		if(bitmapStringEnabled){
			nextShader = &bitmapStringShader();

		}else if(colorsEnabled && texCoordsEnabled){
			switch(currentTextureTarget){
	#ifndef TARGET_OPENGLES
			case GL_TEXTURE_RECTANGLE_ARB:
				nextShader = &defaultTexColor();
				break;
	#endif
			case GL_TEXTURE_2D:
				nextShader = &defaultTex2DColor();
				break;
			case OF_NO_TEXTURE:
				nextShader = &defaultNoTexColor();
				break;
			}

		}else if(colorsEnabled){
			nextShader = &defaultNoTexColor();

		}else if(texCoordsEnabled){
			switch(currentTextureTarget){
	#ifndef TARGET_OPENGLES
			case GL_TEXTURE_RECTANGLE_ARB:
				nextShader = &defaultTexNoColor();
				break;
	#endif
			case GL_TEXTURE_2D:
				nextShader = &defaultTex2DNoColor();
				break;
			case OF_NO_TEXTURE:
				nextShader = &defaultNoTexNoColor();
				break;
			}

		}else{
			nextShader = &defaultNoTexNoColor();
		}

	}else{
		nextShader = &defaultUniqueShader();
	}

	if(nextShader && (!currentShader || *currentShader!=*nextShader)){
		settingDefaultShader = true;
		nextShader->begin();
		settingDefaultShader = false;
	}else{
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::endCustomShader(){
	usingCustomShader = false;
	beginDefaultShader();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2){
	lineMesh.getVertices()[0].set(x1,y1,z1);
	lineMesh.getVertices()[1].set(x2,y2,z2);
    
	// use smoothness, if requested:
	if (bSmoothHinted) startSmoothing();
    
	lineMesh.draw();
    
	// use smoothness, if requested:
	if (bSmoothHinted) endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawRectangle(float x, float y, float z, float w, float h) {
	if (rectMode == OF_RECTMODE_CORNER){
		rectMesh.getVertices()[0].set(x,y,z);
		rectMesh.getVertices()[1].set(x+w, y, z);
		rectMesh.getVertices()[2].set(x+w, y+h, z);
		rectMesh.getVertices()[3].set(x, y+h, z);
	}else{
		rectMesh.getVertices()[0].set(x-w/2.0f, y-h/2.0f, z);
		rectMesh.getVertices()[1].set(x+w/2.0f, y-h/2.0f, z);
		rectMesh.getVertices()[2].set(x+w/2.0f, y+h/2.0f, z);
		rectMesh.getVertices()[3].set(x-w/2.0f, y+h/2.0f, z);
	}
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	rectMesh.setMode((bFilled == OF_FILLED) ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_LOOP);
	rectMesh.draw();
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	triangleMesh.getVertices()[0].set(x1,y1,z1);
	triangleMesh.getVertices()[1].set(x2,y2,z2);
	triangleMesh.getVertices()[2].set(x3,y3,z3);
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	triangleMesh.setMode((bFilled == OF_FILLED) ? OF_PRIMITIVE_TRIANGLE_STRIP : OF_PRIMITIVE_LINE_LOOP);
	triangleMesh.draw();
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawCircle(float x, float y, float z,  float radius){
	vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		circleMesh.getVertices()[i].set(radius*circleCache[i].x+x,radius*circleCache[i].y+y,z);
	}
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	circleMesh.setMode((bFilled == OF_FILLED) ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_STRIP);
	circleMesh.draw();
	
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawEllipse(float x, float y, float z, float width, float height){
	float radiusX = width*0.5;
	float radiusY = height*0.5;
	vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		circleMesh.getVertices()[i].set(radiusX*circlePolyline[i].x+x,radiusY*circlePolyline[i].y+y,z);
	}
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	circleMesh.setMode((bFilled == OF_FILLED) ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_STRIP);
	circleMesh.draw();
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawString(string textString, float x, float y, float z, ofDrawBitmapMode mode){

	// remember the current blend mode so that we can restore it at the end of this method.
	ofBlendMode previousBlendMode = ofGetStyle().blendingMode;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	int len = (int)textString.length();
	float fontSize = 8.0f;
	float lineHeight = fontSize*1.7f;
	int newLineDirection = 1.0f;

	float sx = 0;
	float sy = -fontSize;

	if(!ofIsVFlipped()){
		newLineDirection  = -1;
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(textString,"\n");
		//y = lines*lineHeight;
	}

	if(!ofIsVFlipped()){
		newLineDirection  = -1;
	}

	///////////////////////////
	// APPLY TRANSFORM / VIEW
	///////////////////////////
	//

	bool hasModelView = false;
	bool hasProjection = false;
	bool hasViewport = false;

	static ofRectangle rViewport;
	static ofMatrix4x4 modelView;

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

			modelView.makeTranslationMatrix(-1,-1,0);
			modelView.glScale(2/rViewport.width, 2/rViewport.height, 1);
			modelView.glTranslate(x,y, 0);
			loadMatrix(modelView);
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

			modelView.makeTranslationMatrix(-1,-1,0);
			modelView.glScale(2/rViewport.width, 2/rViewport.height, 1);
			modelView.glTranslate(x,y, 0);
			loadMatrix(modelView);
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

			modelView.makeTranslationMatrix(-1,-1,0);
			modelView.glScale(2/rViewport.width, 2/rViewport.height, 1);
			modelView.glTranslate(dScreen.x, dScreen.y, 0);
			loadMatrix(modelView);
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

	// (c) enable texture once before we start drawing each char (no point turning it on and off constantly)
	//We do this because its way faster
	ofDrawBitmapCharacterStart(textString.size());

	int column = 0;

	for(int c = 0; c < len; c++){
		if(textString[c] == '\n'){

			sy += lineHeight*newLineDirection;
			if(mode == OF_BITMAPMODE_SIMPLE) {
				sx = x;
			} else {
				sx = 0;
			}

			column = 0;
		} else if (textString[c] == '\t'){
			//move the cursor to the position of the next tab
			//8 is the default tab spacing in osx terminal and windows	 command line
			int out = column + 8 - (column % 8);
			sx += fontSize * (out-column);
			column = out;
		} else if (textString[c] >= 32){
			// < 32 = control characters - don't draw
			// solves a bug with control characters
			// getting drawn when they ought to not be
			ofDrawBitmapCharacter(textString[c], (int)sx, (int)sy);

			sx += fontSize;
			column++;
		}
	}
	//We do this because its way faster
	ofDrawBitmapCharacterEnd();



	if (hasViewport){
		popView();
	}else{
		if (hasModelView){
			popMatrix();
		}

		if (hasProjection)
		{
			matrixMode(OF_MATRIX_PROJECTION);
			popMatrix();
			matrixMode(OF_MATRIX_MODELVIEW);
		}
	}

	// restore blendmode
	ofEnableBlendMode(previousBlendMode);
}


#define STRINGIFY(x) #x

#ifdef TARGET_OPENGLES
static string defaultVertexShader = STRINGIFY(
		precision lowp float;
		attribute vec4 position;
		attribute vec4 color;
		attribute vec4 normal;
		attribute vec2 texcoord;

		uniform mat4 modelViewMatrix;
		uniform mat4 projectionMatrix;
		uniform mat4 textureMatrix;
		uniform mat4 modelViewProjectionMatrix;

		varying vec4 colorVarying;
		varying vec2 texCoordVarying;

		void main(){
			gl_Position = modelViewProjectionMatrix * position;
			colorVarying = color;
			texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
		}
);

static string defaultFragmentShaderTexColor = STRINGIFY(
		precision lowp float;

		uniform sampler2D src_tex_unit0;
		uniform float usingTexture;
		uniform float usingColors;
		uniform vec4 globalColor;

		varying float depth;
		varying vec4 colorVarying;
		varying vec2 texCoordVarying;

		void main(){
		    gl_FragColor = texture2D(src_tex_unit0, texCoordVarying)*colorVarying;
        }
);

static string defaultFragmentShaderTexNoColor =  STRINGIFY(
		precision lowp float;

		uniform sampler2D src_tex_unit0;
		uniform float usingTexture;
		uniform float usingColors;
		uniform vec4 globalColor;

		varying float depth;
		varying vec4 colorVarying;
		varying vec2 texCoordVarying;

		void main(){
		    gl_FragColor = texture2D(src_tex_unit0, texCoordVarying)*globalColor;
        }
);

static string defaultFragmentShaderNoTexColor = STRINGIFY(
		precision lowp float;

		uniform sampler2D src_tex_unit0;
		uniform float usingTexture;
		uniform float usingColors;
		uniform vec4 globalColor;

		varying float depth;
		varying vec4 colorVarying;
		varying vec2 texCoordVarying;

		void main(){
		    gl_FragColor = colorVarying;
        }
);

static string defaultFragmentShaderNoTexNoColor  =  STRINGIFY(
		precision lowp float;

		uniform sampler2D src_tex_unit0;
		uniform float usingTexture;
		uniform float usingColors;
		uniform vec4 globalColor;

		varying float depth;
		varying vec4 colorVarying;
		varying vec2 texCoordVarying;

		void main(){
		    gl_FragColor = globalColor;
        }
);

static string bitmapStringVertexShader =  STRINGIFY(
		precision lowp float;

		uniform mat4 projectionMatrix;
		uniform mat4 modelViewMatrix;
		uniform mat4 textureMatrix;
		uniform mat4 modelViewProjectionMatrix;

		attribute vec4  position;
		attribute vec2  texcoord;

		varying vec2 texCoordVarying;

		void main(){
			texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
			gl_Position = modelViewProjectionMatrix * position;
		}
);

static string bitmapStringFragmentShader =  STRINGIFY(
		precision lowp float;

		uniform sampler2D src_tex_unit0;
		uniform vec4 globalColor;

		varying vec2 texCoordVarying;

		void main(){
			vec4 tex = texture2D(src_tex_unit0, texCoordVarying);
			// We will not write anything to the framebuffer if we have a transparent pixel
			// This makes sure we don't mess up our depth buffer.
			if (tex.a < 0.5) discard;
			gl_FragColor = globalColor * tex;
		}
);



// changing shaders in raspberry pi is very expensive so we use only one shader there
// in every other platform avoiding conditionals inside the shader is much faster

static string uniqueVertexShader = STRINGIFY(
		precision lowp float;

		attribute vec4 position;
		attribute vec4 color;
		attribute vec4 normal;
		attribute vec2 texcoord;

		uniform mat4 modelViewMatrix;
		uniform mat4 projectionMatrix;
		uniform mat4 textureMatrix;
		uniform mat4 modelViewProjectionMatrix;

		varying vec4 colorVarying;
		varying vec2 texCoordVarying;

		uniform float usingTexture;
		uniform float usingColors;

		uniform vec4 globalColor;

		void main(){
			gl_Position = modelViewProjectionMatrix * position;
		    if(usingTexture>.5) texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
		    if(usingColors>.5) colorVarying = color*globalColor;
			else colorVarying = globalColor;
		}
);

static string uniqueFragmentShader = STRINGIFY(
		precision lowp float;

		uniform sampler2D src_tex_unit0;

		uniform float usingTexture;
		uniform float bitmapText;

		varying vec4 colorVarying;
		varying vec2 texCoordVarying;

		void main(){
		    vec4 tex;
		    if(usingTexture>.5){
		        tex = texture2D(src_tex_unit0, texCoordVarying);
				if(bitmapText>.5 && tex.a < 0.5){
					discard;
				}else{
		            gl_FragColor = colorVarying*tex;
                }
		    }else{
		        gl_FragColor = colorVarying;
            }
        }
);

#else

// ----------------------------------------------------------------------
// tig: GLSL #150 shaders written against spec:
// http://www.opengl.org/registry/doc/GLSLangSpec.1.50.09.pdf

static string defaultVertexShader = "#version 150\n" STRINGIFY(

	uniform mat4 projectionMatrix;
	uniform mat4 modelViewMatrix;
	uniform mat4 textureMatrix;
	uniform mat4 modelViewProjectionMatrix;

	in vec4  position;
	in vec2  texcoord;
	in vec4  color;
	in vec3  normal;
	 
	out vec4 colorVarying;
	out vec2 texCoordVarying;
	out vec4 normalVarying;

	void main()
	{
		colorVarying = color;
		texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
		gl_Position = modelViewProjectionMatrix * position;
	}
);

// ----------------------------------------------------------------------

static string defaultFragmentShaderTexColor = "#version 150\n" STRINGIFY(

	uniform sampler2DRect src_tex_unit0;
	uniform float usingTexture = 0.0;
	uniform float usingColors = 0.0;
	uniform vec4 globalColor = vec4(1.0);

	in float depth;
	in vec4 colorVarying;
	in vec2 texCoordVarying;
																		 
	out vec4 fragColor;

	void main(){
		fragColor = texture(src_tex_unit0, texCoordVarying) * colorVarying;
	}
);

// ----------------------------------------------------------------------

static string defaultFragmentShaderTexNoColor = "#version 150\n" STRINGIFY(

	uniform sampler2DRect src_tex_unit0;
	uniform float usingTexture = 0.0;
	uniform float usingColors = 0.0;
	uniform vec4 globalColor = vec4(1.0);

	in float depth;
	in vec4 colorVarying;
	in vec2 texCoordVarying;
																		   
	out vec4 fragColor;

	void main(){
		fragColor = texture(src_tex_unit0, texCoordVarying) * globalColor;
	}
);

// ----------------------------------------------------------------------

static string defaultFragmentShaderTex2DColor = "#version 150\n" STRINGIFY(

	uniform sampler2D src_tex_unit0;
	uniform float usingTexture = 0.0;
	uniform float usingColors = 0.0;
	uniform vec4 globalColor = vec4(1.0);

	in float depth;
	in vec4 colorVarying;
	in vec2 texCoordVarying;
																		   
	out vec4 fragColor;

	void main(){
		fragColor = texture(src_tex_unit0, texCoordVarying) * colorVarying;
	}
);

// ----------------------------------------------------------------------

static string defaultFragmentShaderTex2DNoColor = "#version 150\n" STRINGIFY(

	uniform sampler2D src_tex_unit0;
	uniform float usingTexture = 0.0;
	uniform float usingColors = 0.0;
	uniform vec4 globalColor = vec4(1.0);
	
	in float depth;
	in vec4 colorVarying;
	in vec2 texCoordVarying;
																			 
	out vec4 fragColor;
	
	void main(){
		fragColor = texture(src_tex_unit0, texCoordVarying) * globalColor;
	}
);

// ----------------------------------------------------------------------

static string defaultFragmentShaderNoTexColor = "#version 150\n" STRINGIFY (

	uniform sampler2DRect src_tex_unit0;
	uniform float usingTexture = 0.0;
	uniform float usingColors = 0.0;
	uniform vec4 globalColor = vec4(1.0);

	in float depth;
	in vec4 colorVarying;
	in vec2 texCoordVarying;
																			
	out vec4 fragColor;

	void main(){
		fragColor = colorVarying;
	}
);

// ----------------------------------------------------------------------

static string defaultFragmentShaderNoTexNoColor = "#version 150\n" STRINGIFY(

	uniform sampler2DRect src_tex_unit0;
	uniform float usingTexture = 0.0;
	uniform float usingColors = 0.0;
	uniform vec4 globalColor = vec4(1.0);

	in float depth;
	in vec4 colorVarying;
	in vec2 texCoordVarying;
																			 
	out vec4 fragColor;

	void main(){
		fragColor = globalColor;
	}
);

// ----------------------------------------------------------------------

static string bitmapStringVertexShader = "#version 150\n" STRINGIFY(

	uniform mat4 projectionMatrix;
	uniform mat4 modelViewMatrix;
	uniform mat4 textureMatrix;
	uniform mat4 modelViewProjectionMatrix;

	in vec4  position;
	in vec4  color;
	in vec2  texcoord;

	out vec2 texCoordVarying;

	void main()
	{
		texCoordVarying = texcoord;
		gl_Position = modelViewProjectionMatrix * position;
	}
);

// ----------------------------------------------------------------------

static string bitmapStringFragmentShader = "#version 150\n" STRINGIFY(

	uniform sampler2D src_tex_unit0;
	uniform vec4 globalColor = vec4(1.0);

	in vec2 texCoordVarying;
																	  
	out vec4 fragColor;

	void main()
	{
		
		vec4 tex = texture(src_tex_unit0, texCoordVarying);
		// We will not write anything to the framebuffer if we have a transparent pixel
		// This makes sure we don't mess up our depth buffer.
		if (tex.a < 0.5) discard;
		fragColor = globalColor * tex;
	}
);

// ----------------------------------------------------------------------
// changing shaders in raspberry pi is very expensive so we use only one shader there
// in desktop openGL these are not used but we declare it to avoid more ifdefs

static string uniqueVertexShader = "#version 150\n" STRINGIFY(
        
		uniform mat4 modelViewMatrix;
		uniform mat4 projectionMatrix;
		uniform mat4 textureMatrix;
		uniform mat4 modelViewProjectionMatrix;
		uniform float usingTexture;
		uniform float usingColors;
		uniform vec4 globalColor;

		in vec4 position;
		in vec4 color;
		in vec4 normal;
		in vec2 texcoord;
        
		out vec4 colorVarying;
		out vec2 texCoordVarying;
        
		void main(){
			gl_Position = modelViewProjectionMatrix * position;
		    if(usingTexture>.5) texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
		    if(usingColors>.5) colorVarying = color*globalColor;
			else colorVarying = globalColor;
		}
);

// ----------------------------------------------------------------------

static string uniqueFragmentShader = "#version 150\n" STRINGIFY(
        
		uniform sampler2D src_tex_unit0;
		uniform float usingTexture;
		uniform float bitmapText;
        
		in vec4 colorVarying;
		in vec2 texCoordVarying;
        
        out vec4 fragColor;
																
		void main(){
		    vec4 tex;
		    if(usingTexture>.5){
		        tex = texture(src_tex_unit0, texCoordVarying);
				if(bitmapText>.5 && tex.a < 0.5){
					discard;
				}else{
		            fragColor = colorVarying*tex;
                }
		    }else{
		        fragColor = colorVarying;
            }
        }
);


#endif

void ofGLProgrammableRenderer::setup(){
	glGetError();

#ifdef TARGET_RASPBERRY_PI
	uniqueShader = true;
#else
	uniqueShader = false;
#endif

	matrixStack.setRenderSurface(*ofGetWindowPtr());

	if(uniqueShader){
		defaultUniqueShader().setupShaderFromSource(GL_VERTEX_SHADER,uniqueVertexShader);
		defaultUniqueShader().setupShaderFromSource(GL_FRAGMENT_SHADER,uniqueFragmentShader);
		defaultUniqueShader().bindDefaults();
		defaultUniqueShader().linkProgram();
		beginDefaultShader();
	}else{
		defaultTexColor().setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
		defaultTex2DColor().setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
		defaultNoTexColor().setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
		defaultTexNoColor().setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
		defaultTex2DNoColor().setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
		defaultNoTexNoColor().setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);

	#ifndef TARGET_OPENGLES
		defaultTexColor().setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderTexColor);
		defaultTex2DColor().setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderTex2DColor);
	#else
		defaultTexColor().setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderTexColor);
		defaultTex2DColor().setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderTexColor);
	#endif
		defaultNoTexColor().setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderNoTexColor);
	#ifndef TARGET_OPENGLES
		defaultTexNoColor().setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderTexNoColor);
		defaultTex2DNoColor().setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderTex2DNoColor);
	#else
		defaultTexNoColor().setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderTexNoColor);
		defaultTex2DNoColor().setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderTexNoColor);
	#endif
		defaultNoTexNoColor().setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderNoTexNoColor);


		bitmapStringShader().setupShaderFromSource(GL_VERTEX_SHADER, bitmapStringVertexShader);
		bitmapStringShader().setupShaderFromSource(GL_FRAGMENT_SHADER, bitmapStringFragmentShader);

		defaultTexColor().bindDefaults();
		defaultTex2DColor().bindDefaults();
		defaultNoTexColor().bindDefaults();
		defaultTexNoColor().bindDefaults();
		defaultTex2DNoColor().bindDefaults();
		defaultNoTexNoColor().bindDefaults();

		defaultTexColor().linkProgram();
		defaultTex2DColor().linkProgram();
		defaultNoTexColor().linkProgram();
		defaultTexNoColor().linkProgram();
		defaultTex2DNoColor().linkProgram();
		defaultNoTexNoColor().linkProgram();

		bitmapStringShader().bindDefaults();
		bitmapStringShader().linkProgram();
	}

#ifndef TARGET_OPENGLES
	circleMesh.setUsage(GL_STREAM_DRAW);
	triangleMesh.setUsage(GL_STREAM_DRAW);
	rectMesh.setUsage(GL_STREAM_DRAW);
	lineMesh.setUsage(GL_STREAM_DRAW);
#endif
}


ofShader & ofGLProgrammableRenderer::defaultUniqueShader(){
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::defaultTexColor(){
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::defaultTexNoColor(){
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::defaultTex2DColor(){
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::defaultTex2DNoColor(){
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::defaultNoTexColor(){
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::defaultNoTexNoColor(){
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::bitmapStringShader(){
	static ofShader * shader = new ofShader;
	return *shader;
}
