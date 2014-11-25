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
#include "ofLight.h"
#include "ofMaterial.h"


static const string VIEW_MATRIX_UNIFORM="viewMatrix";
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
ofGLProgrammableRenderer::ofGLProgrammableRenderer(const ofAppBaseWindow * window)
:matrixStack(window)
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
	currentMaterial = NULL;
	alphaMaskTextureTarget = OF_NO_TEXTURE;
}

ofGLProgrammableRenderer::~ofGLProgrammableRenderer() {
	
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::startRender() {
	matrixStack.setRenderSurface(*ofGetWindowPtr());
	beginDefaultShader();
	viewport();
    // to do non auto clear on PC for now - we do something like "single" buffering --
    // it's not that pretty but it work for the most part

    #ifdef TARGET_WIN32
    if (getBackgroundAuto() == false){
        glDrawBuffer (GL_FRONT);
    }
    #endif

	if ( getBackgroundAuto() ){// || ofGetFrameNum() < 3){
		clear();
	}
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
void ofGLProgrammableRenderer::draw(const ofMesh & vertexData, bool useColors, bool useTextures, bool useNormals)  const{
	draw(vertexData, OF_MESH_FILL, useColors, useTextures, useNormals); // tig: use default mode if no render mode specified.
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const{
	if (vertexData.getVertices().empty()) return;
	
	
	// tig: note that for GL3+ we use glPolygonMode to draw wireframes or filled meshes, and not the primitive mode.
	// the reason is not purely aesthetic, but more conformant with the behaviour of ofGLRenderer. Whereas
	// gles2.0 doesn't allow for a polygonmode.
	// Also gles2 still supports vertex array syntax for uploading data to attributes and it seems to be faster than
	// vbo's for meshes that are updated frequently so let's use that instead
	
	//if (bSmoothHinted) startSmoothing();

#if defined(TARGET_OPENGLES) && !defined(TARGET_EMSCRIPTEN)
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


	const_cast<ofGLProgrammableRenderer*>(this)->setAttributes(true,useColors,useTextures,useNormals);

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
	
	meshVbo.setMesh(vertexData, GL_STATIC_DRAW, useColors, useTextures, useNormals);

#ifndef TARGET_OPENGLES
	glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
	GLenum drawMode = ofGetGLPrimitiveMode(vertexData.getMode());
#else
	GLenum drawMode;
	switch(renderType){
	case OF_MESH_POINTS:
		drawMode = GL_POINTS;
		break;
	case OF_MESH_WIREFRAME:
		drawMode = GL_LINE_STRIP;
		break;
	case OF_MESH_FILL:
		drawMode = ofGetGLPrimitiveMode(vertexData.getMode());
		break;
	default:
		drawMode = ofGetGLPrimitiveMode(vertexData.getMode());
		break;
	}
#endif
	if(meshVbo.getUsingIndices()) {
		meshVbo.drawElements(drawMode, meshVbo.getNumIndices());
	} else {
		meshVbo.draw(drawMode, 0, meshVbo.getNumVertices());
	}
	
	// tig: note further that we could glGet() and store the current polygon mode, but don't, since that would
	// infer a massive performance hit. instead, we revert the glPolygonMode to mirror the current ofFill state
	// after we're finished drawing, following the principle of least surprise.
	// ideally the glPolygonMode (or the polygon draw mode) should be part of ofStyle so that we can keep track
	// of its state on the client side...

#ifndef TARGET_OPENGLES
	glPolygonMode(GL_FRONT_AND_BACK, (ofGetFill() == OF_OUTLINE) ?  GL_LINE : GL_FILL);
#endif
	
#endif
	
	//if (bSmoothHinted) endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw( const of3dPrimitive& model, ofPolyRenderMode renderType) const {
	if(model.isUsingVbo()){
		model.getMesh().draw(renderType);
	}else{
		draw(model.getMesh(),renderType);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofPolyline & poly) const{
	/*  some things are internally still using ofPolyline
	if(!wrongUseLoggedOnce){
		ofLogWarning("ofGLProgrammableRenderer") << "draw(): drawing an ofMesh or ofPolyline directly is deprecated, use a ofVboMesh or ofPath";
		wrongUseLoggedOnce = true;
	}*/
	if(poly.getVertices().empty()) return;

	// use smoothness, if requested:
	//if (bSmoothHinted) startSmoothing();

#if defined( TARGET_OPENGLES ) && !defined(TARGET_EMSCRIPTEN)

	glEnableVertexAttribArray(ofShader::POSITION_ATTRIBUTE);
	glVertexAttribPointer(ofShader::POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, sizeof(ofVec3f), &poly[0]);

	const_cast<ofGLProgrammableRenderer*>(this)->setAttributes(true,false,false,false);

	GLenum drawMode = poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP;

	glDrawArrays(drawMode, 0, poly.size());

#else

	vertexDataVbo.setVertexData(&poly.getVertices()[0], poly.size(), GL_DYNAMIC_DRAW);
	vertexDataVbo.draw(poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, poly.size());

#endif
	// use smoothness, if requested:
	//if (bSmoothHinted) endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofPath & shape) const{
	ofColor prevColor;
	if(shape.getUseShapeColor()){
		prevColor = ofGetStyle().color;
	}
	ofGLProgrammableRenderer* mut_this = const_cast<ofGLProgrammableRenderer*>(this);
	if(shape.isFilled()){
		const ofMesh & mesh = shape.getTessellation();
		if(shape.getUseShapeColor()){
			mut_this->setColor( shape.getFillColor() * ofGetStyle().color,shape.getFillColor().a/255. * ofGetStyle().color.a);
		}
		draw(mesh,OF_MESH_FILL);
	}
	if(shape.hasOutline()){
		float lineWidth = ofGetStyle().lineWidth;
		if(shape.getUseShapeColor()){
			mut_this->setColor( shape.getStrokeColor() * ofGetStyle().color, shape.getStrokeColor().a/255. * ofGetStyle().color.a);
		}
		mut_this->setLineWidth( shape.getStrokeWidth() );
		const vector<ofPolyline> & outlines = shape.getOutline();
		for(int i=0; i<(int)outlines.size(); i++)
			draw(outlines[i]);
		mut_this->setLineWidth(lineWidth);
	}
	if(shape.getUseShapeColor()){
		mut_this->setColor(prevColor);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	if(image.isUsingTexture()){
		const_cast<ofGLProgrammableRenderer*>(this)->setAttributes(true,false,true,false);
		const ofTexture& tex = image.getTexture();
		if(tex.isAllocated()) {
			tex.bind();
			draw(tex.getMeshForSubsection(x,y,z,w,h,sx,sy,sw,sh),false,true,false);
			tex.unbind();
		} else {
			ofLogWarning("ofGLProgrammableRenderer") << "draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	if(image.isUsingTexture()){
		const_cast<ofGLProgrammableRenderer*>(this)->setAttributes(true,false,true,false);
		const ofTexture& tex = image.getTexture();
		if(tex.isAllocated()) {
			tex.bind();
			draw(tex.getMeshForSubsection(x,y,z,w,h,sx,sy,sw,sh),false,true,false);
			tex.unbind();
		} else {
			ofLogWarning("ofGLProgrammableRenderer") << "draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	if(image.isUsingTexture()){
		const_cast<ofGLProgrammableRenderer*>(this)->setAttributes(true,false,true,false);
		const ofTexture& tex = image.getTexture();
		if(tex.isAllocated()) {
			tex.bind();
			draw(tex.getMeshForSubsection(x,y,z,w,h,sx,sy,sw,sh),false,true,false);
			tex.unbind();
		} else {
			ofLogWarning("ofGLProgrammableRenderer") << "draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const{
	if(!video.isInitialized() || !video.isUsingTexture() || video.getTexturePlanes().empty()){
		return;
	}
	const ofShader * shader = NULL;
	if(!usingCustomShader){
		shader = getVideoShader(video);
		if(shader){
			shader->begin();
			setVideoShaderUniforms(video,*shader);
		}
	}
	const ofTexture& tex = video.getTexture();
	tex.bind();
	draw(tex.getMeshForSubsection(x,y,0,w,h,0,0,w,h),false,true,false);
	tex.unbind();
	if(shader){
		shader->end();
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofBaseVideoDraws & video) const{
	if(!video.isInitialized() || !video.isUsingTexture() || video.getTexturePlanes().empty()){
		return;
	}
	const ofShader * shader = NULL;
	bool binded = false;
	if(!usingCustomShader){
		shader = getVideoShader(video);
		if(shader){
			shader->begin();
			setVideoShaderUniforms(video,*shader);
			binded = true;
		}
	}

	if(!binded){
		video.getTexture().bind();
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofBaseVideoDraws & video) const{
	if(!video.isInitialized() || !video.isUsingTexture() || video.getTexturePlanes().empty()){
		return;
	}
	const ofShader * shader = NULL;
	bool unbinded = false;
	if(!usingCustomShader){
		shader = getVideoShader(video);
		if(shader){
			shader->end();
			unbinded = true;
		}
	}

	if(!unbinded){
		video.getTexture().unbind();
	}

}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setCurrentFBO(const ofFbo * fbo){
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
ofRectangle ofGLProgrammableRenderer::getCurrentViewport() const{
	getNativeViewport();
	return matrixStack.getCurrentViewport();
}

//----------------------------------------------------------
ofRectangle ofGLProgrammableRenderer::getNativeViewport() const{
	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);

	ofGLProgrammableRenderer * mutRenderer = const_cast<ofGLProgrammableRenderer*>(this);
	ofRectangle nativeViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	mutRenderer->matrixStack.nativeViewport(nativeViewport);
    return nativeViewport;
}

//----------------------------------------------------------
int ofGLProgrammableRenderer::getViewportWidth() const{
	return getCurrentViewport().width;
}

//----------------------------------------------------------
int ofGLProgrammableRenderer::getViewportHeight() const{
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
ofHandednessType ofGLProgrammableRenderer::getCoordHandedness() const{
	return matrixStack.getHandedness();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setOrientation(ofOrientation orientation, bool vFlip){
	matrixStack.setOrientation(orientation,vFlip);
	uploadMatrices();

}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setupScreenPerspective(float width, float height, float fov, float nearDist, float farDist) {
	float viewW, viewH;
	if(width<0 || height<0){
		ofRectangle currentViewport = getCurrentViewport();

		viewW = currentViewport.width;
		viewH = currentViewport.height;
	}else{
		viewW = width;
		viewH = height;
	}

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
	loadViewMatrix(lookAt);
	
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setupScreenOrtho(float width, float height, float nearDist, float farDist) {
	float viewW, viewH;
	if(width<0 || height<0){
		ofRectangle currentViewport = getCurrentViewport();

		viewW = currentViewport.width;
		viewH = currentViewport.height;
	}else{
		viewW = width;
		viewH = height;
	}

	ofMatrix4x4 ortho;

	ortho = ofMatrix4x4::newOrthoMatrix(0, viewW, 0, viewH, nearDist, farDist);

	matrixMode(OF_MATRIX_PROJECTION);
	loadMatrix(ortho); // make ortho our new projection matrix.

	matrixMode(OF_MATRIX_MODELVIEW);
	loadViewMatrix(ofMatrix4x4::newIdentityMatrix());
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
	loadMatrix(m.getPtr());
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
void ofGLProgrammableRenderer::loadViewMatrix(const ofMatrix4x4 & m){
	matrixStack.loadViewMatrix(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::multViewMatrix(const ofMatrix4x4 & m){
	matrixStack.multViewMatrix(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
ofMatrix4x4 ofGLProgrammableRenderer::getCurrentViewMatrix() const{
	return matrixStack.getViewMatrix();
}

//----------------------------------------------------------
ofMatrix4x4 ofGLProgrammableRenderer::getCurrentNormalMatrix() const{
	return ofMatrix4x4::getTransposedOf(getCurrentMatrix(OF_MATRIX_MODELVIEW).getInverse());
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::uploadCurrentMatrix(){
	if(!currentShader) return;
	// uploads the current matrix to the current shader.
	switch(matrixStack.getCurrentMatrixMode()){
	case OF_MATRIX_MODELVIEW:
		currentShader->setUniformMatrix4f(VIEW_MATRIX_UNIFORM, matrixStack.getViewMatrix());
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
		default:
			ofLogWarning() << "Invalid getCurrentMatrix query";
			return ofMatrix4x4();
			break;
	}
}

//----------------------------------------------------------
ofMatrix4x4 ofGLProgrammableRenderer::getCurrentOrientationMatrix() const {
	return matrixStack.getOrientationMatrix();
}
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
void ofGLProgrammableRenderer::clear(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
bool ofGLProgrammableRenderer::getBackgroundAuto(){
	return bBackgroundAuto;
}

//----------------------------------------------------------
ofColor ofGLProgrammableRenderer::getBackgroundColor(){
	return bgColor;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setBackgroundColor(const ofColor & c){
	bgColor = c;
	glClearColor(bgColor[0],bgColor[1],bgColor[2], bgColor[3]);
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
	#ifndef TARGET_PROGRAMMABLE_GL
		glEnable(GL_POINT_SPRITE_OES);
	#endif
#else
	glEnable(GL_PROGRAM_POINT_SIZE);
#endif
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disablePointSprites(){
#ifdef TARGET_OPENGLES
	#ifndef TARGET_PROGRAMMABLE_GL
		glEnable(GL_POINT_SPRITE_OES);
	#endif
#else
	glDisable(GL_PROGRAM_POINT_SIZE);
#endif
}


//----------------------------------------------------------
void ofGLProgrammableRenderer::enableAntiAliasing(){
#if !defined(TARGET_PROGRAMMABLE_GL) || !defined(TARGET_OPENGLES)
	glEnable(GL_MULTISAMPLE);
#endif
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disableAntiAliasing(){
#if !defined(TARGET_PROGRAMMABLE_GL) || !defined(TARGET_OPENGLES)
	glDisable(GL_MULTISAMPLE);
#endif
}

//----------------------------------------------------------
const ofShader & ofGLProgrammableRenderer::getCurrentShader() const{
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
	colorsEnabled = color;
	normalsEnabled = normals;

	if(!uniqueShader || currentMaterial){
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
void ofGLProgrammableRenderer::enableTextureTarget(int textureTarget, int textureID, int textureLocation){
	bool wasUsingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);
	currentTextureTarget = textureTarget;

	if(!uniqueShader || currentMaterial){
		beginDefaultShader();
	}

	bool usingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);
	if(wasUsingTexture!=usingTexture){
		if(currentShader) currentShader->setUniform1f(USE_TEXTURE_UNIFORM,usingTexture);
	}

	if((currentTextureTarget!=OF_NO_TEXTURE) && currentShader){
		currentShader->setUniformTexture("src_tex_unit"+ofToString(textureLocation),textureTarget,textureID,textureLocation);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disableTextureTarget(int textureTarget, int textureLocation){
	bool wasUsingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);
	currentTextureTarget = OF_NO_TEXTURE;

	if(!uniqueShader || currentMaterial){
		beginDefaultShader();
	}

	bool usingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);
	if(wasUsingTexture!=usingTexture){
		if(currentShader) currentShader->setUniform1f(USE_TEXTURE_UNIFORM,usingTexture);
	}
	glActiveTexture(GL_TEXTURE0+textureLocation);
	glBindTexture(textureTarget, 0);
	glActiveTexture(GL_TEXTURE0);
}

//----------------------------------------------------------
GLenum ofGLProgrammableRenderer::getCurrentTextureTarget(){
	return currentTextureTarget;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setAlphaMaskTex(ofTexture & tex){
	alphaMaskTextureTarget = tex.getTextureData().textureTarget;
	if(alphaMaskTextureTarget==GL_TEXTURE_2D){
		alphaMask2DShader().begin();
	}else{
		alphaMaskRectShader().begin();
	}
	enableTextureTarget(alphaMaskTextureTarget, tex.getTextureData().textureID, 1);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disableAlphaMask(){
	disableTextureTarget(alphaMaskTextureTarget,1);
	if(alphaMaskTextureTarget==GL_TEXTURE_2D){
		alphaMask2DShader().end();
	}else{
		alphaMaskRectShader().end();
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::beginCustomShader(const ofShader & shader){
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
	if(usingCustomShader && !currentMaterial)	return;

	ofShader * nextShader = NULL;

	if(!uniqueShader){
		if(currentMaterial){
			currentMaterial->beginShader(currentTextureTarget);
		}else if(bitmapStringEnabled){
			nextShader = &bitmapStringShader();

		}else if(colorsEnabled && texCoordsEnabled){
			switch(currentTextureTarget){
	#ifndef TARGET_OPENGLES
			case GL_TEXTURE_RECTANGLE_ARB:
				nextShader = &defaultTexRectColor();
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
				nextShader = &defaultTexRectNoColor();
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
void ofGLProgrammableRenderer::setCurrentMaterial(ofBaseMaterial * material){
	currentMaterial = material;
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
	float fontSize = 8.0f;
	float sx = 0;
	float sy = -fontSize;

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
	setAlphaBitmapText(true);
	ofMesh charMesh = ofBitmapStringGetMesh(textString, 0, 0, mode);
	ofBitmapStringGetTextureRef().bind();
	draw(charMesh,OF_MESH_FILL,false,true,false);
	ofBitmapStringGetTextureRef().unbind();
	setAlphaBitmapText(false);


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
}


#define STRINGIFY(x) #x


// ----------------------------------------------------------------------
// tig: GLSL #150 shaders written against spec:
// http://www.opengl.org/registry/doc/GLSLangSpec.1.50.09.pdf

#ifdef TARGET_OPENGLES
static string vertex_shader_header =
		"precision mediump float;\n"
		"#define IN attribute\n"
		"#define OUT varying\n"
		"#define TEXTURE texture2D\n"
		"#define TARGET_OPENGLES\n";
static string fragment_shader_header =
		"precision mediump float;\n"
		"#define IN varying\n"
		"#define OUT\n"
		"#define TEXTURE texture2D\n"
		"#define FRAG_COLOR gl_FragColor\n"
		"#define TARGET_OPENGLES\n";
#else
static string vertex_shader_header =
		"#version %glsl_version%\n"
		"%extensions%\n"
		"#define IN in\n"
		"#define OUT out\n"
		"#define TEXTURE texture\n";
static string fragment_shader_header =
		"#version %glsl_version%\n"
		"%extensions%\n"
		"#define IN in\n"
		"#define OUT out\n"
		"#define TEXTURE texture\n"
		"#define FRAG_COLOR fragColor\n"
		"out vec4 fragColor;\n";
#endif

static string defaultVertexShader = vertex_shader_header + STRINGIFY(
	uniform mat4 projectionMatrix;
	uniform mat4 modelViewMatrix;
	uniform mat4 textureMatrix;
	uniform mat4 modelViewProjectionMatrix;

	IN vec4  position;
	IN vec2  texcoord;
	IN vec4  color;
	IN vec3  normal;

	OUT vec4 colorVarying;
	OUT vec2 texCoordVarying;
	OUT vec4 normalVarying;

	void main()
	{
		colorVarying = color;
		texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
		gl_Position = modelViewProjectionMatrix * position;
	}
);

// ----------------------------------------------------------------------

static string defaultFragmentShaderTexRectColor = fragment_shader_header + STRINGIFY(

	uniform sampler2DRect src_tex_unit0;
	uniform float usingTexture;
	uniform float usingColors;
	uniform vec4 globalColor;

	IN float depth;
	IN vec4 colorVarying;
	IN vec2 texCoordVarying;


	void main(){
		FRAG_COLOR = TEXTURE(src_tex_unit0, texCoordVarying) * colorVarying;
	}
);

// ----------------------------------------------------------------------

static string defaultFragmentShaderTexRectNoColor = fragment_shader_header + STRINGIFY(

	uniform sampler2DRect src_tex_unit0;
	uniform float usingTexture;
	uniform float usingColors;
	uniform vec4 globalColor;

	IN float depth;
	IN vec4 colorVarying;
	IN vec2 texCoordVarying;

	void main(){
		FRAG_COLOR = TEXTURE(src_tex_unit0, texCoordVarying)* globalColor;
	}
);

// ----------------------------------------------------------------------

static string alphaMaskFragmentShaderTexRectNoColor = fragment_shader_header + STRINGIFY(

	uniform sampler2DRect src_tex_unit0;
	uniform sampler2DRect src_tex_unit1;
	uniform float usingTexture;
	uniform float usingColors;
	uniform vec4 globalColor;

	IN float depth;
	IN vec4 colorVarying;
	IN vec2 texCoordVarying;

	void main(){
		FRAG_COLOR = vec4(TEXTURE(src_tex_unit0, texCoordVarying).rgb,  TEXTURE(src_tex_unit1, texCoordVarying).r)* globalColor;
	}
);

// ----------------------------------------------------------------------

static string alphaMaskFragmentShaderTex2DNoColor = fragment_shader_header + STRINGIFY(

	uniform sampler2D src_tex_unit0;
	uniform sampler2D src_tex_unit1;
	uniform float usingTexture;
	uniform float usingColors;
	uniform vec4 globalColor;

	IN float depth;
	IN vec4 colorVarying;
	IN vec2 texCoordVarying;

	void main(){
		FRAG_COLOR = vec4(TEXTURE(src_tex_unit0, texCoordVarying).rgb,  TEXTURE(src_tex_unit1, texCoordVarying).r)* globalColor;
	}
);

// ----------------------------------------------------------------------

static string defaultFragmentShaderTex2DColor = fragment_shader_header + STRINGIFY(

	uniform sampler2D src_tex_unit0;
	uniform float usingTexture;
	uniform float usingColors;
	uniform vec4 globalColor;

	IN float depth;
	IN vec4 colorVarying;
	IN vec2 texCoordVarying;

	void main(){
		FRAG_COLOR = TEXTURE(src_tex_unit0, texCoordVarying) * colorVarying;
	}
);

// ----------------------------------------------------------------------

static string defaultFragmentShaderTex2DNoColor = fragment_shader_header + STRINGIFY(

	uniform sampler2D src_tex_unit0;
	uniform float usingTexture;
	uniform float usingColors;
	uniform vec4 globalColor;
	
	IN float depth;
	IN vec4 colorVarying;
	IN vec2 texCoordVarying;
	
	void main(){
		FRAG_COLOR = TEXTURE(src_tex_unit0, texCoordVarying) * globalColor;
	}
);

// ----------------------------------------------------------------------

static string defaultFragmentShaderNoTexColor = fragment_shader_header + STRINGIFY (

	uniform float usingTexture;
	uniform float usingColors;
	uniform vec4 globalColor;

	IN float depth;
	IN vec4 colorVarying;
	IN vec2 texCoordVarying;

	void main(){
		FRAG_COLOR = colorVarying;
	}
);

// ----------------------------------------------------------------------

static string defaultFragmentShaderNoTexNoColor = fragment_shader_header + STRINGIFY(

	uniform float usingTexture;
	uniform float usingColors;
	uniform vec4 globalColor;

	IN float depth;
	IN vec4 colorVarying;
	IN vec2 texCoordVarying;

	void main(){
		FRAG_COLOR = globalColor;
	}
);

// ----------------------------------------------------------------------

static string bitmapStringVertexShader = vertex_shader_header + STRINGIFY(

	uniform mat4 projectionMatrix;
	uniform mat4 modelViewMatrix;
	uniform mat4 textureMatrix;
	uniform mat4 modelViewProjectionMatrix;

	IN vec4  position;
	IN vec4  color;
	IN vec2  texcoord;

	OUT vec2 texCoordVarying;

	void main()
	{
		texCoordVarying = texcoord;
		gl_Position = modelViewProjectionMatrix * position;
	}
);

// ----------------------------------------------------------------------

static string bitmapStringFragmentShader = fragment_shader_header + STRINGIFY(

	uniform sampler2D src_tex_unit0;
	uniform vec4 globalColor;

	IN vec2 texCoordVarying;

	void main()
	{
		
		vec4 tex = TEXTURE(src_tex_unit0, texCoordVarying);
		// We will not write anything to the framebuffer if we have a transparent pixel
		// This makes sure we don't mess up our depth buffer.
		if (tex.a < 0.5) discard;
		FRAG_COLOR = globalColor * tex;
	}
);

// ----------------------------------------------------------------------
// changing shaders in raspberry pi is very expensive so we use only one shader there
// in desktop openGL these are not used but we declare it to avoid more ifdefs

static string uniqueVertexShader = vertex_shader_header + STRINGIFY(
        
	uniform mat4 modelViewMatrix;
	uniform mat4 projectionMatrix;
	uniform mat4 textureMatrix;
	uniform mat4 modelViewProjectionMatrix;
	uniform float usingTexture;
	uniform float usingColors;
	uniform vec4 globalColor;

	IN vec4 position;
	IN vec4 color;
	IN vec4 normal;
	IN vec2 texcoord;

	OUT vec4 colorVarying;
	OUT vec2 texCoordVarying;

	void main(){
		gl_Position = modelViewProjectionMatrix * position;
		if(usingTexture>.5) texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
		if(usingColors>.5) colorVarying = color*globalColor;
		else colorVarying = globalColor;
	}
);

// ----------------------------------------------------------------------
static string uniqueFragmentShader = fragment_shader_header + STRINGIFY(
        
	uniform sampler2D src_tex_unit0;
	uniform float usingTexture;
	uniform float bitmapText;

	IN vec4 colorVarying;
	IN vec2 texCoordVarying;

	void main(){
		vec4 tex;
		if(usingTexture>.5){
			tex = TEXTURE(src_tex_unit0, texCoordVarying);
			if(bitmapText>.5 && tex.a < 0.5){
				discard;
			}else{
				FRAG_COLOR = colorVarying*tex;
			}
		}else{
			FRAG_COLOR = colorVarying;
		}
	}
);

// ----------------------------------------------------------------------
// video color space conversion shaders
static string FRAGMENT_SHADER_YUY2 = STRINGIFY(
	uniform SAMPLER src_tex_unit0;\n
	uniform vec4 globalColor;\n

	IN vec4 colorVarying;\n
	IN vec2 texCoordVarying;\n
	uniform float onePixel;\n
	uniform float textureWidth;\n

    const vec3 offset = vec3(-0.0625, -0.5, -0.5);\n
    const vec3 rcoeff = vec3(1.164, 0.000, 1.596);\n
    const vec3 gcoeff = vec3(1.164,-0.391,-0.813);\n
    const vec3 bcoeff = vec3(1.164, 2.018, 0.000);\n


	void main(){\n
        vec3 yuv;\n
	    yuv.x=TEXTURE(src_tex_unit0,texCoordVarying).r;\n
		float x = texCoordVarying.x * textureWidth;\n
		if(mod(x,2.0)>0.5){\n
			yuv.y=TEXTURE(src_tex_unit0,vec2(texCoordVarying.x-onePixel,texCoordVarying.y)).%g;\n
			yuv.z=TEXTURE(src_tex_unit0,texCoordVarying).%g;\n
		}else{\n
			yuv.y=TEXTURE(src_tex_unit0,texCoordVarying).%g;\n
			yuv.z=TEXTURE(src_tex_unit0,vec2(texCoordVarying.x+onePixel,texCoordVarying.y)).%g;\n
		}\n
        yuv += offset;\n
        float r = dot(yuv, rcoeff);\n
        float g = dot(yuv, gcoeff);\n
        float b = dot(yuv, bcoeff);\n
        FRAG_COLOR=vec4(r,g,b,1.0) * globalColor;\n
	}\n
);

// ----------------------------------------------------------------------
static string FRAGMENT_SHADER_NV12_NV21 = STRINGIFY(
	uniform SAMPLER Ytex;\n
	uniform SAMPLER UVtex;\n
	uniform vec4 globalColor;\n
    uniform vec2 tex_scaleUV;\n

	IN vec4 colorVarying;\n
	IN vec2 texCoordVarying;\n

    const vec3 offset = vec3(-0.0625, -0.5, -0.5);\n
    const vec3 rcoeff = vec3(1.164, 0.000, 1.596);\n
    const vec3 gcoeff = vec3(1.164,-0.391,-0.813);\n
    const vec3 bcoeff = vec3(1.164, 2.018, 0.000);\n


	void main(){\n
        vec3 yuv;\n
	    yuv.x=TEXTURE(Ytex,texCoordVarying).r;\n
	    yuv.yz=TEXTURE(UVtex,texCoordVarying * tex_scaleUV).%r%g;\n
        yuv += offset;\n
        float r = dot(yuv, rcoeff);\n
        float g = dot(yuv, gcoeff);\n
        float b = dot(yuv, bcoeff);\n
        FRAG_COLOR=vec4(r,g,b,1.0) * globalColor;\n
	}\n
);

// ----------------------------------------------------------------------
static string FRAGMENT_SHADER_PLANAR_YUV = STRINGIFY(
	uniform SAMPLER Ytex;\n
	uniform SAMPLER Utex;\n
	uniform SAMPLER Vtex;\n
    uniform vec2 tex_scaleY;\n
    uniform vec2 tex_scaleU;\n
    uniform vec2 tex_scaleV;\n
	uniform vec4 globalColor;\n

	IN vec4 colorVarying;\n
	IN vec2 texCoordVarying;\n

    const vec3 offset = vec3(-0.0625, -0.5, -0.5);\n
    const vec3 rcoeff = vec3(1.164, 0.000, 1.596);\n
    const vec3 gcoeff = vec3(1.164,-0.391,-0.813);\n
    const vec3 bcoeff = vec3(1.164, 2.018, 0.000);\n


	void main(){\n
        vec3 yuv;\n
	    yuv.x=TEXTURE(Ytex,texCoordVarying * tex_scaleY).r;\n
	    yuv.y=TEXTURE(Utex,texCoordVarying * tex_scaleU).r;\n
	    yuv.z=TEXTURE(Vtex,texCoordVarying * tex_scaleV).r;\n
        yuv += offset;\n
        float r = dot(yuv, rcoeff);\n
        float g = dot(yuv, gcoeff);\n
        float b = dot(yuv, bcoeff);\n
        FRAG_COLOR=vec4(r,g,b,1.0) * globalColor;\n
	}\n
);


static string shaderSource(const string & src, const string & glslVersion){
	string shaderSrc = src;
	ofStringReplace(shaderSrc,"%glsl_version%",glslVersion);
#ifndef TARGET_OPENGLES
	if(ofGetOpenGLVersionMajor()<4 && ofGetOpenGLVersionMinor()<2){
		ofStringReplace(shaderSrc,"%extensions%","#extension GL_ARB_texture_rectangle : enable");
	}else{
		ofStringReplace(shaderSrc,"%extensions%","");
	}
#endif
	return shaderSrc;
}

static string videoFragmentShaderSource(const ofBaseVideoDraws & video, const string & glslVersion){
	string src;
	switch(video.getPixelFormat()){
		case OF_PIXELS_YUY2:
			src = FRAGMENT_SHADER_YUY2;
			#ifndef TARGET_OPENGLES
				ofStringReplace(src,"%g","g");
			#else
				ofStringReplace(src,"%g","a");
			#endif
			break;
		case OF_PIXELS_NV12:
			src = FRAGMENT_SHADER_NV12_NV21;
			#ifndef TARGET_OPENGLES
				ofStringReplace(src,"%r%g","rg");
			#else
				ofStringReplace(src,"%r%g","ra");
			#endif
			break;
		case OF_PIXELS_NV21:
			src = FRAGMENT_SHADER_NV12_NV21;
			#ifndef TARGET_OPENGLES
				ofStringReplace(src,"%r%g","gr");
			#else
				ofStringReplace(src,"%r%g","ar");
			#endif
			break;
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
			src = FRAGMENT_SHADER_PLANAR_YUV;
			break;
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:
		case OF_PIXELS_RGB565:
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
		case OF_PIXELS_GRAY:
		default:
			break;
	}

	string header = fragment_shader_header;
	GLenum textureTarget = video.getTexture().getTextureData().textureTarget;
	if(textureTarget==GL_TEXTURE_2D){
		header += "#define SAMPLER sampler2D\n";
	}
#ifndef TARGET_OPENGLES
	else if(textureTarget==GL_TEXTURE_RECTANGLE){
		header += "#define SAMPLER sampler2DRect\n";
	}
#endif
	return shaderSource(header + src,glslVersion);
}

void ofGLProgrammableRenderer::setup(const string & glslVersion){
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
	#ifndef TARGET_OPENGLES
		defaultTexRectColor().setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,glslVersion));
		defaultTexRectNoColor().setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,glslVersion));
	#endif
		defaultTex2DColor().setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,glslVersion));
		defaultNoTexColor().setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,glslVersion));
		defaultTex2DNoColor().setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,glslVersion));
		defaultNoTexNoColor().setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,glslVersion));
		alphaMaskRectShader().setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,glslVersion));
		alphaMask2DShader().setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,glslVersion));

	#ifndef TARGET_OPENGLES
		defaultTexRectColor().setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderTexRectColor,glslVersion));
		defaultTex2DColor().setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderTex2DColor,glslVersion));
	#else
		defaultTex2DColor().setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderTex2DColor,glslVersion));
	#endif
		defaultNoTexColor().setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderNoTexColor,glslVersion));
	#ifndef TARGET_OPENGLES
		defaultTexRectNoColor().setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderTexRectNoColor,glslVersion));
		defaultTex2DNoColor().setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderTex2DNoColor,glslVersion));
	#else
		defaultTex2DNoColor().setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderTex2DNoColor,glslVersion));
	#endif
		defaultNoTexNoColor().setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderNoTexNoColor,glslVersion));
		alphaMaskRectShader().setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(alphaMaskFragmentShaderTexRectNoColor,glslVersion));
		alphaMask2DShader().setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(alphaMaskFragmentShaderTex2DNoColor,glslVersion));


		bitmapStringShader().setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(bitmapStringVertexShader,glslVersion));
		bitmapStringShader().setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(bitmapStringFragmentShader,glslVersion));

#ifndef TARGET_OPENGLES
		defaultTexRectColor().bindDefaults();
		defaultTexRectNoColor().bindDefaults();
#endif
		defaultTex2DColor().bindDefaults();
		defaultNoTexColor().bindDefaults();
		defaultTex2DNoColor().bindDefaults();
		defaultNoTexNoColor().bindDefaults();
		alphaMaskRectShader().bindDefaults();
		alphaMask2DShader().bindDefaults();

#ifndef TARGET_OPENGLES
		defaultTexRectColor().linkProgram();
		defaultTexRectNoColor().linkProgram();
#endif
		defaultTex2DColor().linkProgram();
		defaultNoTexColor().linkProgram();
		defaultTex2DNoColor().linkProgram();
		defaultNoTexNoColor().linkProgram();
		alphaMaskRectShader().linkProgram();
		alphaMask2DShader().linkProgram();

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

const ofShader * ofGLProgrammableRenderer::getVideoShader(const ofBaseVideoDraws & video) const{
	const ofShader * shader = NULL;
	GLenum target = video.getTexture().getTextureData().textureTarget;
	switch(video.getPixelFormat()){
		case OF_PIXELS_YUY2:
			if(target==GL_TEXTURE_2D){
				shader = &getShaderPlanarYUY2();
			}else{
				shader = &getShaderPlanarYUY2Rect();
			}
			break;
		case OF_PIXELS_NV12:
			if(target==GL_TEXTURE_2D){
				shader = &getShaderNV12();
			}else{
				shader = &getShaderNV12Rect();
			}
			break;
		case OF_PIXELS_NV21:
			if(target==GL_TEXTURE_2D){
				shader = &getShaderNV21();
			}else{
				shader = &getShaderNV21Rect();
			}
			break;
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
			if(target==GL_TEXTURE_2D){
				shader = &getShaderPlanarYUV();
			}else{
				shader = &getShaderPlanarYUVRect();
			}
			break;
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:
		case OF_PIXELS_RGB565:
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
		case OF_PIXELS_GRAY:
		default:
			break;
	}
	if(shader && !shader->isLoaded()){
		ofShader * mutShader = const_cast<ofShader*>(shader);
		mutShader->setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,ofGetGLSLVersion()));
		mutShader->setupShaderFromSource(GL_FRAGMENT_SHADER,videoFragmentShaderSource(video,ofGetGLSLVersion()));
		mutShader->bindDefaults();
		mutShader->linkProgram();
	}
	return shader;
}

static float getTextureScaleX(const ofBaseVideoDraws & video, int plane){
	if(!video.getTexturePlanes().empty()){
		return video.getTexturePlanes()[plane].getWidth()/video.getWidth();
	}else{
		return 1.0;
	}
}

static float getTextureScaleY(const ofBaseVideoDraws & video, int plane){
	if(!video.getTexturePlanes().empty()){
		return video.getTexturePlanes()[plane].getHeight()/video.getHeight();
	}else{
		return 1.0;
	}
}

void ofGLProgrammableRenderer::setVideoShaderUniforms(const ofBaseVideoDraws & video, const ofShader & shader) const{
	switch(video.getPixelFormat()){
		case OF_PIXELS_YUY2:
#ifndef TARGET_OPENGLES
			if(video.getTexture().getTextureData().textureTarget==GL_TEXTURE_RECTANGLE){
				shader.setUniform1f("onePixel",1.0);
				shader.setUniform1f("textureWidth",1.0);
			}else{
#endif
				shader.setUniform1f("onePixel",1.0/video.getWidth());
				shader.setUniform1f("textureWidth",video.getWidth());
#ifndef TARGET_OPENGLES
			}
#endif
			break;
		case OF_PIXELS_NV12:
		case OF_PIXELS_NV21:
			shader.setUniformTexture("Ytex",video.getTexturePlanes()[0],0);
			shader.setUniformTexture("UVtex",video.getTexturePlanes()[1],1);
#ifndef TARGET_OPENGLES
			if(video.getTexture().getTextureData().textureTarget==GL_TEXTURE_RECTANGLE){
				shader.setUniform2f("tex_scaleUV",getTextureScaleX(video,1),getTextureScaleY(video,1));
			}else{
#endif
				shader.setUniform2f("tex_scaleUV",1.0,1.0);
#ifndef TARGET_OPENGLES
			}
#endif
			break;
		case OF_PIXELS_YV12:
			shader.setUniformTexture("Ytex",video.getTexturePlanes()[0],0);
			shader.setUniformTexture("Utex",video.getTexturePlanes()[2],1);
			shader.setUniformTexture("Vtex",video.getTexturePlanes()[1],2);
#ifndef TARGET_OPENGLES
			if(video.getTexture().getTextureData().textureTarget==GL_TEXTURE_RECTANGLE){
				shader.setUniform2f("tex_scaleY",getTextureScaleX(video,0),getTextureScaleY(video,0));
				shader.setUniform2f("tex_scaleU",getTextureScaleX(video,2),getTextureScaleY(video,2));
				shader.setUniform2f("tex_scaleV",getTextureScaleX(video,1),getTextureScaleY(video,1));
			}else{
#endif
				shader.setUniform2f("tex_scaleY",1.0,1.0);
				shader.setUniform2f("tex_scaleU",1.0,1.0);
				shader.setUniform2f("tex_scaleV",1.0,1.0);
#ifndef TARGET_OPENGLES
			}
#endif
			break;
		case OF_PIXELS_I420:
			shader.setUniformTexture("Ytex",video.getTexturePlanes()[0],0);
			shader.setUniformTexture("Utex",video.getTexturePlanes()[1],1);
			shader.setUniformTexture("Vtex",video.getTexturePlanes()[2],2);
#ifndef TARGET_OPENGLES
			if(video.getTexture().getTextureData().textureTarget==GL_TEXTURE_RECTANGLE){
				shader.setUniform2f("tex_scaleY",getTextureScaleX(video,0),getTextureScaleY(video,0));
				shader.setUniform2f("tex_scaleU",getTextureScaleX(video,1),getTextureScaleY(video,1));
				shader.setUniform2f("tex_scaleV",getTextureScaleX(video,2),getTextureScaleY(video,2));
			}else{
#endif
				shader.setUniform2f("tex_scaleY",1.0,1.0);
				shader.setUniform2f("tex_scaleU",1.0,1.0);
				shader.setUniform2f("tex_scaleV",1.0,1.0);
#ifndef TARGET_OPENGLES
			}
#endif
			break;
		default:
			break;
	}
}

ofShader & ofGLProgrammableRenderer::defaultUniqueShader() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::defaultTexRectColor() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::defaultTexRectNoColor() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::defaultTex2DColor() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::defaultTex2DNoColor() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::defaultNoTexColor() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::defaultNoTexNoColor() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::alphaMaskRectShader() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::alphaMask2DShader() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::bitmapStringShader() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::getShaderPlanarYUY2() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::getShaderNV12() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::getShaderNV21() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::getShaderPlanarYUV() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::getShaderPlanarYUY2Rect() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::getShaderNV12Rect() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::getShaderNV21Rect() const{
	static ofShader * shader = new ofShader;
	return *shader;
}

ofShader & ofGLProgrammableRenderer::getShaderPlanarYUVRect() const{
	static ofShader * shader = new ofShader;
	return *shader;
}
