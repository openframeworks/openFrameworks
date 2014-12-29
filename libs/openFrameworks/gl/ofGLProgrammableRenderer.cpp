#include "ofGLProgrammableRenderer.h"
#include "ofMesh.h"
#include "ofPath.h"
#include "ofMesh.h"
#include "ofBitmapFont.h"
#include "ofGLUtils.h"
#include "ofImage.h"
#include "ofFbo.h"
#include "ofVbo.h"
#include "of3dPrimitives.h"
#include "ofLight.h"
#include "ofMaterial.h"
#include "ofCamera.h"
#include "ofTrueTypeFont.h"
#include "ofNode.h"


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
static bool programmableRendererCreated = false;

bool ofIsGLProgrammableRenderer(){
	return programmableRendererCreated;
}


//----------------------------------------------------------
ofGLProgrammableRenderer::ofGLProgrammableRenderer(const ofAppBaseWindow * _window)
:matrixStack(_window)
,graphics3d(this)
{
	programmableRendererCreated = true;
	bBackgroundAuto = true;

	lineMesh.getVertices().resize(2);
	lineMesh.setMode(OF_PRIMITIVE_LINES);
	triangleMesh.getVertices().resize(3);
	rectMesh.getVertices().resize(4);

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

	major = 3;
	minor = 2;
	window = _window;
}

ofGLProgrammableRenderer::~ofGLProgrammableRenderer() {
	
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::startRender() {
	matrixStack.setRenderSurface(*window);
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
		background(currentStyle.bgColor);
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
	

#ifndef TARGET_OPENGLES
	meshVbo.setMesh(vertexData, GL_STREAM_DRAW, useColors, useTextures, useNormals);
	glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
	GLenum drawMode = ofGetGLPrimitiveMode(vertexData.getMode());
#else
	meshVbo.setMesh(vertexData, GL_STATIC_DRAW, useColors, useTextures, useNormals);
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
		drawElements(meshVbo,drawMode, meshVbo.getNumIndices());
	} else {
		draw(meshVbo, drawMode, 0, vertexData.getNumVertices());
	}
	
	// tig: note further that we could glGet() and store the current polygon mode, but don't, since that would
	// infer a massive performance hit. instead, we revert the glPolygonMode to mirror the current ofFill state
	// after we're finished drawing, following the principle of least surprise.
	// ideally the glPolygonMode (or the polygon draw mode) should be part of ofStyle so that we can keep track
	// of its state on the client side...

#ifndef TARGET_OPENGLES
	glPolygonMode(GL_FRONT_AND_BACK, currentStyle.bFill ?  GL_FILL : GL_LINE);
#endif
	
#endif
	
	//if (bSmoothHinted) endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofVboMesh & mesh, ofPolyRenderMode renderType) const{
	drawInstanced(mesh,renderType,1);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawInstanced(const ofVboMesh & mesh, ofPolyRenderMode renderType, int primCount) const{
	if(mesh.getNumVertices()==0) return;
	GLuint mode = ofGetGLPrimitiveMode(mesh.getMode());
#ifndef TARGET_OPENGLES
	glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
	if(mesh.getNumIndices() && renderType!=OF_MESH_POINTS){
		if (primCount <= 1) {
			drawElements(mesh.getVbo(),mode,mesh.getNumIndices());
		} else {
			drawElementsInstanced(mesh.getVbo(),mode,mesh.getNumIndices(),primCount);
		}
	}else{
		if (primCount <= 1) {
			draw(mesh.getVbo(),mode,0,mesh.getNumVertices());
		} else {
			drawInstanced(mesh.getVbo(),mode,0,mesh.getNumVertices(),primCount);
		}
	}

	// tig: note further that we could glGet() and store the current polygon mode, but don't, since that would
	// infer a massive performance hit. instead, we revert the glPolygonMode to mirror the current ofFill state
	// after we're finished drawing, following the principle of least surprise.
	// ideally the glPolygonMode (or the polygon draw mode) should be part of ofStyle so that we can keep track
	// of its state on the client side...

	//glPolygonMode(GL_FRONT_AND_BACK, currentStyle.bFill ?  GL_LINE : GL_FILL);
#else
	if(renderType == OF_MESH_POINTS){
		draw(mesh.getVbo(),GL_POINTS,0,mesh.getNumVertices());
	}else if(renderType == OF_MESH_WIREFRAME){
		if(mesh.getNumIndices()){
			drawElements(mesh.getVbo(),GL_LINES,mesh.getNumIndices());
		}else{
			draw(mesh.getVbo(),GL_LINES,0,mesh.getNumVertices());
		}
	}else{
		if(mesh.getNumIndices()){
			drawElements(mesh.getVbo(),mode,mesh.getNumIndices());
		}else{
			draw(mesh.getVbo(),mode,0,mesh.getNumVertices());
		}
	}
#endif
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw( const of3dPrimitive& model, ofPolyRenderMode renderType) const {
	const_cast<ofGLProgrammableRenderer*>(this)->pushMatrix();
	const_cast<ofGLProgrammableRenderer*>(this)->multMatrix(model.getGlobalTransformMatrix());
	if(model.isUsingVbo()){
		draw(static_cast<const ofVboMesh&>(model.getMesh()),renderType);
	}else{
		draw(model.getMesh(),renderType);
	}
	const_cast<ofGLProgrammableRenderer*>(this)->popMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofNode& node) const{
	const_cast<ofGLProgrammableRenderer*>(this)->pushMatrix();
	const_cast<ofGLProgrammableRenderer*>(this)->multMatrix(node.getGlobalTransformMatrix());
	node.customDraw(this);
	const_cast<ofGLProgrammableRenderer*>(this)->popMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofPolyline & poly) const{
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

	meshVbo.setVertexData(&poly.getVertices()[0], poly.size(), GL_DYNAMIC_DRAW);
	meshVbo.draw(poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, poly.size());

#endif
	// use smoothness, if requested:
	//if (bSmoothHinted) endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofPath & shape) const{
	ofColor prevColor;
	if(shape.getUseShapeColor()){
		prevColor = currentStyle.color;
	}
	ofGLProgrammableRenderer * mut_this = const_cast<ofGLProgrammableRenderer*>(this);
	if(shape.isFilled()){
		const ofMesh & mesh = shape.getTessellation();
		if(shape.getUseShapeColor()){
			mut_this->setColor( shape.getFillColor(),shape.getFillColor().a);
		}
		draw(mesh,OF_MESH_FILL);
	}
	if(shape.hasOutline()){
		float lineWidth = currentStyle.lineWidth;
		if(shape.getUseShapeColor()){
			mut_this->setColor( shape.getStrokeColor(), shape.getStrokeColor().a);
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
			const_cast<ofGLProgrammableRenderer*>(this)->bind(tex,0);
			draw(tex.getMeshForSubsection(x,y,z,w,h,sx,sy,sw,sh,isVFlipped(),currentStyle.rectMode),false,true,false);
			const_cast<ofGLProgrammableRenderer*>(this)->unbind(tex,0);
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
			const_cast<ofGLProgrammableRenderer*>(this)->bind(tex,0);
			draw(tex.getMeshForSubsection(x,y,z,w,h,sx,sy,sw,sh,isVFlipped(),currentStyle.rectMode),false,true,false);
			const_cast<ofGLProgrammableRenderer*>(this)->unbind(tex,0);
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
			const_cast<ofGLProgrammableRenderer*>(this)->bind(tex,0);
			draw(tex.getMeshForSubsection(x,y,z,w,h,sx,sy,sw,sh,isVFlipped(),currentStyle.rectMode),false,true,false);
			const_cast<ofGLProgrammableRenderer*>(this)->unbind(tex,0);
		} else {
			ofLogWarning("ofGLProgrammableRenderer") << "draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofTexture & tex, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	const_cast<ofGLProgrammableRenderer*>(this)->setAttributes(true,false,true,false);
	if(tex.isAllocated()) {
		const_cast<ofGLProgrammableRenderer*>(this)->bind(tex,0);
		draw(tex.getMeshForSubsection(x,y,z,w,h,sx,sy,sw,sh,isVFlipped(),currentStyle.rectMode),false,true,false);
		const_cast<ofGLProgrammableRenderer*>(this)->unbind(tex,0);
	} else {
		ofLogWarning("ofGLProgrammableRenderer") << "draw(): texture is not allocated";
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const{
	if(!video.isInitialized() || !video.isUsingTexture() || video.getTexturePlanes().empty()){
		return;
	}
	const_cast<ofGLProgrammableRenderer*>(this)->bind(video);
	draw(video.getTexture().getMeshForSubsection(x,y,0,w,h,0,0,w,h,isVFlipped(),currentStyle.rectMode),false,true,false);
	const_cast<ofGLProgrammableRenderer*>(this)->unbind(video);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofVbo & vbo, GLuint drawMode, int first, int total) const{
	if(vbo.getUsingVerts()) {
		vbo.bind();
		const_cast<ofGLProgrammableRenderer*>(this)->setAttributes(vbo.getUsingVerts(),vbo.getUsingColors(),vbo.getUsingTexCoords(),vbo.getUsingNormals());
		glDrawArrays(drawMode, first, total);
		vbo.unbind();
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawElements(const ofVbo & vbo, GLuint drawMode, int amt) const{
	if(vbo.getUsingVerts()) {
		vbo.bind();
		const_cast<ofGLProgrammableRenderer*>(this)->setAttributes(vbo.getUsingVerts(),vbo.getUsingColors(),vbo.getUsingTexCoords(),vbo.getUsingNormals());
#ifdef TARGET_OPENGLES
        glDrawElements(drawMode, amt, GL_UNSIGNED_SHORT, NULL);
#else
        glDrawElements(drawMode, amt, GL_UNSIGNED_INT, NULL);
#endif
		vbo.unbind();
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawInstanced(const ofVbo & vbo, GLuint drawMode, int first, int total, int primCount) const{
	if(vbo.getUsingVerts()) {
		vbo.bind();
		const_cast<ofGLProgrammableRenderer*>(this)->setAttributes(vbo.getUsingVerts(),vbo.getUsingColors(),vbo.getUsingTexCoords(),vbo.getUsingNormals());
#ifdef TARGET_OPENGLES
		// todo: activate instancing once OPENGL ES supports instancing, starting with version 3.0
		// unfortunately there is currently no easy way within oF to query the current OpenGL version.
		// https://www.khronos.org/opengles/sdk/docs/man3/xhtml/glDrawElementsInstanced.xml
		ofLogWarning("ofVbo") << "drawInstanced(): hardware instancing is not supported on OpenGL ES < 3.0";
		// glDrawArraysInstanced(drawMode, first, total, primCount);
#else
		glDrawArraysInstanced(drawMode, first, total, primCount);
#endif
		vbo.unbind();
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawElementsInstanced(const ofVbo & vbo, GLuint drawMode, int amt, int primCount) const{
	if(vbo.getUsingVerts()) {
		vbo.bind();
		const_cast<ofGLProgrammableRenderer*>(this)->setAttributes(vbo.getUsingVerts(),vbo.getUsingColors(),vbo.getUsingTexCoords(),vbo.getUsingNormals());
#ifdef TARGET_OPENGLES
        // todo: activate instancing once OPENGL ES supports instancing, starting with version 3.0
        // unfortunately there is currently no easy way within oF to query the current OpenGL version.
        // https://www.khronos.org/opengles/sdk/docs/man3/xhtml/glDrawElementsInstanced.xml
        ofLogWarning("ofVbo") << "drawElementsInstanced(): hardware instancing is not supported on OpenGL ES < 3.0";
        // glDrawElementsInstanced(drawMode, amt, GL_UNSIGNED_SHORT, NULL, primCount);
#else
        glDrawElementsInstanced(drawMode, amt, GL_UNSIGNED_INT, NULL, primCount);
#endif
		vbo.unbind();
	}
}

//----------------------------------------------------------
ofPath & ofGLProgrammableRenderer::getPath(){
	return path;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofBaseVideoDraws & video){
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
void ofGLProgrammableRenderer::unbind(const ofBaseVideoDraws & video){
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
	viewport(viewport_.x,viewport_.y,viewport_.width,viewport_.height,isVFlipped());
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::viewport(float x, float y, float width, float height, bool vflip) {
	matrixStack.viewport(x,y,width,height,vflip);
	ofRectangle nativeViewport = matrixStack.getNativeViewport();
	glViewport(nativeViewport.x,nativeViewport.y,nativeViewport.width,nativeViewport.height);
}

//----------------------------------------------------------
ofRectangle ofGLProgrammableRenderer::getCurrentViewport() const{
	return matrixStack.getCurrentViewport();
}

//----------------------------------------------------------
ofRectangle ofGLProgrammableRenderer::getNativeViewport() const{
    return matrixStack.getNativeViewport();
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
	setStyle(ofStyle());
	path.setMode(ofPath::POLYLINES);
	path.setUseShapeColor(false);
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
		path.setCircleResolution(res);
	}
}

void ofGLProgrammableRenderer::setPolyMode(ofPolyWindingMode mode){
	currentStyle.polyMode = mode;
	path.setPolyWindingMode(mode);
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
		if(currentMaterial){
			currentMaterial->uploadMatrices(*currentShader,*this);
		}
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
	if(newColor!=currentStyle.color){
		currentStyle.color = newColor;
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
void ofGLProgrammableRenderer::setBitmapTextMode(ofDrawBitmapMode mode){
	currentStyle.drawBitmapMode = mode;
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
	return currentStyle.bgColor;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setBackgroundColor(const ofColor & c){
	currentStyle.bgColor = c;
	glClearColor(currentStyle.bgColor[0]/255., currentStyle.bgColor[1]/255., currentStyle.bgColor[2]/255., currentStyle.bgColor[3]/255.);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::background(const ofColor & c){
	setBackgroundColor(c);
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
	currentStyle.bFill = (fill==OF_FILLED);
	if(currentStyle.bFill){
		path.setFilled(true);
		path.setStrokeWidth(0);
	}else{
		path.setFilled(false);
		path.setStrokeWidth(currentStyle.lineWidth);
	}
}

//----------------------------------------------------------
ofFillFlag ofGLProgrammableRenderer::getFillMode(){
	if(currentStyle.bFill){
		return OF_FILLED;
	}else{
		return OF_OUTLINE;
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setRectMode(ofRectMode mode){
	currentStyle.rectMode = mode;
}

//----------------------------------------------------------
ofRectMode ofGLProgrammableRenderer::getRectMode(){
	return currentStyle.rectMode;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setLineWidth(float lineWidth){
	// tig: glLinewidth is 'kind of' deprecated.
	// http://www.opengl.org/registry/doc/glspec32.core.20090803.pdf
	// p.330: "LineWidth values greater than 1.0 will generate an
	// INVALID_VALUE error".
	// use geometry shaders to draw lines of varying thickness...

	currentStyle.lineWidth = lineWidth;
	if(!currentStyle.bFill){
		path.setStrokeWidth(lineWidth);
	}
	glLineWidth(lineWidth);
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
	currentStyle.smoothing = smooth;
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

ofStyle ofGLProgrammableRenderer::getStyle() const{
	return currentStyle;
}

void ofGLProgrammableRenderer::pushStyle(){
	styleHistory.push_back(currentStyle);
	//if we are over the max number of styles we have set, then delete the oldest styles.
	if( styleHistory.size() > OF_MAX_STYLE_HISTORY ){
		styleHistory.pop_front();
		//should we warn here?
		ofLogWarning("ofGraphics") << "ofPushStyle(): maximum number of style pushes << " << OF_MAX_STYLE_HISTORY << " reached, did you forget to pop somewhere?";
	}
}

void ofGLProgrammableRenderer::popStyle(){
	if( styleHistory.size() ){
		setStyle(styleHistory.front());
		styleHistory.pop_back();
	}
}

void ofGLProgrammableRenderer::setStyle(const ofStyle & style){

	//color
	setColor((int)style.color.r, (int)style.color.g, (int)style.color.b, (int)style.color.a);

	//bg color
	setBackgroundColor(style.bgColor);

	//circle resolution - don't worry it only recalculates the display list if the res has changed
	setCircleResolution(style.circleResolution);

	setCurveResolution(style.curveResolution);

	//line width - finally!
	setLineWidth(style.lineWidth);

	//ofSetDepthTest(style.depthTest); removed since it'll break old projects setting depth test through glEnable

	//rect mode: corner/center
	setRectMode(style.rectMode);

	//poly mode: winding type
	setPolyMode(style.polyMode);

	//fill
	if(style.bFill ){
		setFillMode(OF_FILLED);
	}else{
		setFillMode(OF_OUTLINE);
	}

	//smoothing
	/*if(style.smoothing ){
		enableSmoothing();
	}else{
		disableSmoothing();
	}*/

	//blending
	setBlendMode(style.blendingMode);

	currentStyle = style;
}

void ofGLProgrammableRenderer::setCurveResolution(int resolution){
	currentStyle.curveResolution = resolution;
	path.setCurveResolution(resolution);
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
void ofGLProgrammableRenderer::enableTextureTarget(const ofTexture & tex, int textureLocation){
	bool wasUsingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);
	currentTextureTarget = tex.texData.textureTarget;

	if(!uniqueShader || currentMaterial){
		beginDefaultShader();
	}

	bool usingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);
	if(wasUsingTexture!=usingTexture){
		if(currentShader) currentShader->setUniform1f(USE_TEXTURE_UNIFORM,usingTexture);
	}

	if((currentTextureTarget!=OF_NO_TEXTURE) && currentShader){
		currentShader->setUniformTexture("src_tex_unit"+ofToString(textureLocation),tex.texData.textureTarget,tex.texData.textureID,textureLocation);
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
void ofGLProgrammableRenderer::setAlphaMaskTex(const ofTexture & tex){
	alphaMaskTextureTarget = tex.getTextureData().textureTarget;
	if(alphaMaskTextureTarget==GL_TEXTURE_2D){
		alphaMask2DShader.begin();
	}else{
		alphaMaskRectShader.begin();
	}
	enableTextureTarget(tex, 1);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disableAlphaMask(){
	disableTextureTarget(alphaMaskTextureTarget,1);
	if(alphaMaskTextureTarget==GL_TEXTURE_2D){
		alphaMask2DShader.end();
	}else{
		alphaMaskRectShader.end();
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofShader & shader){
	if(currentShader && *currentShader==shader){
		return;
	}
	glUseProgram(shader.getProgram());

	currentShader = &shader;
	uploadMatrices();
	setDefaultUniforms();
	if(!settingDefaultShader){
		usingCustomShader = true;
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofShader & shader){
	glUseProgram(0);
	usingCustomShader = false;
	beginDefaultShader();
}


//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofFbo & fbo, bool setupPerspective){
	matrixStack.pushView();
	matrixStack.setRenderSurface(fbo);
	viewport();
	if(setupPerspective){
		setupScreenPerspective();
	}else{
		uploadMatrices();
	}
	fbo.bind();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofFbo & fbo){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	matrixStack.setRenderSurface(*window);
	uploadMatrices();
	matrixStack.popView();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofBaseMaterial & material){
	currentMaterial = &material;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofBaseMaterial & material){
	currentMaterial = NULL;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofTexture & texture, int location){
	//we could check if it has been allocated - but we don't do that in draw()
	if(texture.getAlphaMask()){
		setAlphaMaskTex(*texture.getAlphaMask());
	}
	enableTextureTarget(texture,location);


	if(ofGetUsingNormalizedTexCoords()) {
		matrixMode(OF_MATRIX_TEXTURE);
		pushMatrix();
		ofMatrix4x4 m;

#ifndef TARGET_OPENGLES
		if(texture.texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB)
			m.makeScaleMatrix(texture.texData.width, texture.texData.height, 1.0f);
		else
#endif
			m.makeScaleMatrix(texture.texData.width / texture.texData.tex_w, texture.texData.height / texture.texData.tex_h, 1.0f);

		loadMatrix(m);
		matrixMode(OF_MATRIX_MODELVIEW);
	}
	if(texture.isUsingTextureMatrix()){
		matrixMode(OF_MATRIX_TEXTURE);
		if(!ofGetUsingNormalizedTexCoords()) pushMatrix();
		multMatrix(texture.getTextureMatrix());
		matrixMode(OF_MATRIX_MODELVIEW);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofTexture & texture, int location){
	disableTextureTarget(texture.texData.textureTarget,location);
	if(texture.getAlphaMask()){
		disableAlphaMask();
	}

	if(texture.isUsingTextureMatrix() || ofGetUsingNormalizedTexCoords()) {
		matrixMode(OF_MATRIX_TEXTURE);
		popMatrix();
		matrixMode(OF_MATRIX_MODELVIEW);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofCamera & camera, const ofRectangle & _viewport){
	pushView();
	viewport(_viewport);
	setOrientation(matrixStack.getOrientation(),camera.isVFlipped());
	matrixMode(OF_MATRIX_PROJECTION);
	loadMatrix(camera.getProjectionMatrix(_viewport).getPtr());
	matrixMode(OF_MATRIX_MODELVIEW);
	loadViewMatrix(camera.getModelViewMatrix());
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofCamera & camera){
	popView();
}


//----------------------------------------------------------
void ofGLProgrammableRenderer::uploadMatrices(){
	if(!currentShader) return;
	currentShader->setUniformMatrix4f(MODELVIEW_MATRIX_UNIFORM, matrixStack.getModelViewMatrix());
	currentShader->setUniformMatrix4f(PROJECTION_MATRIX_UNIFORM, matrixStack.getProjectionMatrix());
	currentShader->setUniformMatrix4f(TEXTURE_MATRIX_UNIFORM, matrixStack.getTextureMatrix());
	currentShader->setUniformMatrix4f(MODELVIEW_PROJECTION_MATRIX_UNIFORM, matrixStack.getModelViewProjectionMatrix());
	if(currentMaterial){
		currentMaterial->uploadMatrices(*currentShader,*this);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setDefaultUniforms(){
	if(!currentShader) return;
	currentShader->setUniform4f(COLOR_UNIFORM, currentStyle.color.r/255.,currentStyle.color.g/255.,currentStyle.color.b/255.,currentStyle.color.a/255.);
	bool usingTexture = texCoordsEnabled & (currentTextureTarget!=OF_NO_TEXTURE);
	currentShader->setUniform1f(USE_TEXTURE_UNIFORM,usingTexture);
	currentShader->setUniform1f(USE_COLORS_UNIFORM,colorsEnabled);
	if(currentMaterial){
		currentMaterial->updateMaterial(*currentShader,*this);
		currentMaterial->updateLights(*currentShader,*this);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::beginDefaultShader(){
	if(usingCustomShader && !currentMaterial)	return;

	const ofShader * nextShader = NULL;

	if(!uniqueShader || currentMaterial){
		if(currentMaterial){
			nextShader = &currentMaterial->getShader(currentTextureTarget,*this);
		}else if(bitmapStringEnabled){
			nextShader = &bitmapStringShader;

		}else if(colorsEnabled && texCoordsEnabled){
			switch(currentTextureTarget){
	#ifndef TARGET_OPENGLES
			case GL_TEXTURE_RECTANGLE_ARB:
				nextShader = &defaultTexRectColor;
				break;
	#endif
			case GL_TEXTURE_2D:
				nextShader = &defaultTex2DColor;
				break;
			case OF_NO_TEXTURE:
				nextShader = &defaultNoTexColor;
				break;
			}

		}else if(colorsEnabled){
			nextShader = &defaultNoTexColor;

		}else if(texCoordsEnabled){
			switch(currentTextureTarget){
	#ifndef TARGET_OPENGLES
			case GL_TEXTURE_RECTANGLE_ARB:
				nextShader = &defaultTexRectNoColor;
				break;
	#endif
			case GL_TEXTURE_2D:
				nextShader = &defaultTex2DNoColor;
				break;
			case OF_NO_TEXTURE:
				nextShader = &defaultNoTexNoColor;
				break;
			}

		}else{
			nextShader = &defaultNoTexNoColor;
		}

	}else{
		nextShader = &defaultUniqueShader;
	}

	if(nextShader){
		if(!currentShader || *currentShader!=*nextShader){
			settingDefaultShader = true;
			bind(*nextShader);
			settingDefaultShader = false;
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const{
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer*>(this);
	lineMesh.getVertices()[0].set(x1,y1,z1);
	lineMesh.getVertices()[1].set(x2,y2,z2);
    
	// use smoothness, if requested:
	if (currentStyle.smoothing) mutThis->startSmoothing();
    
	draw(lineMesh,OF_MESH_FILL,false,false,false);
    
	// use smoothness, if requested:
	if (currentStyle.smoothing) mutThis->endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawRectangle(float x, float y, float z, float w, float h) const{
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer*>(this);
	if (currentStyle.rectMode == OF_RECTMODE_CORNER){
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
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->startSmoothing();

	rectMesh.setMode(currentStyle.bFill ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_LOOP);
	draw(rectMesh,OF_MESH_FILL,false,false,false);
    
	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const{
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer*>(this);
	triangleMesh.getVertices()[0].set(x1,y1,z1);
	triangleMesh.getVertices()[1].set(x2,y2,z2);
	triangleMesh.getVertices()[2].set(x3,y3,z3);
    
	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->startSmoothing();

	triangleMesh.setMode(currentStyle.bFill ? OF_PRIMITIVE_TRIANGLE_STRIP : OF_PRIMITIVE_LINE_LOOP);
	draw(triangleMesh,OF_MESH_FILL,false,false,false);
    
	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawCircle(float x, float y, float z,  float radius) const{
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer*>(this);
	const vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		circleMesh.getVertices()[i].set(radius*circleCache[i].x+x,radius*circleCache[i].y+y,z);
	}
    
	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->startSmoothing();

	circleMesh.setMode(currentStyle.bFill ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_STRIP);
	draw(circleMesh,OF_MESH_FILL,false,false,false);
	
	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawEllipse(float x, float y, float z, float width, float height) const{
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer*>(this);
	float radiusX = width*0.5;
	float radiusY = height*0.5;
	const vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		circleMesh.getVertices()[i].set(radiusX*circlePolyline[i].x+x,radiusY*circlePolyline[i].y+y,z);
	}
    
	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->startSmoothing();

	circleMesh.setMode(currentStyle.bFill ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_STRIP);
	draw(circleMesh,OF_MESH_FILL,false,false,false);
    
	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawString(string textString, float x, float y, float z) const{
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer*>(this);
	float sx = 0;
	float sy = 0;

	///////////////////////////
	// APPLY TRANSFORM / VIEW
	///////////////////////////
	//

	bool hasModelView = false;
	bool hasProjection = false;
	bool hasViewport = false;

	ofRectangle rViewport;
	ofMatrix4x4 modelView;

	switch (currentStyle.drawBitmapMode) {

		case OF_BITMAPMODE_SIMPLE:

			sx += x;
			sy += y;
			break;

		case OF_BITMAPMODE_SCREEN:

			hasViewport = true;
			mutThis->pushView();

			rViewport = matrixStack.getFullSurfaceViewport();
			mutThis->viewport(rViewport);

			mutThis->matrixMode(OF_MATRIX_PROJECTION);
			mutThis->loadIdentityMatrix();
			mutThis->matrixMode(OF_MATRIX_MODELVIEW);

			modelView.makeTranslationMatrix(-1,-1,0);
			modelView.glScale(2/rViewport.width, 2/rViewport.height, 1);
			modelView.glTranslate(x,y, 0);
			mutThis->loadMatrix(modelView);
			break;

		case OF_BITMAPMODE_VIEWPORT:

			rViewport = getCurrentViewport();

			hasProjection = true;
			mutThis->matrixMode(OF_MATRIX_PROJECTION);
			mutThis->pushMatrix();
			mutThis->loadIdentityMatrix();

			hasModelView = true;
			mutThis->matrixMode(OF_MATRIX_MODELVIEW);
			mutThis->pushMatrix();

			modelView.makeTranslationMatrix(-1,-1,0);
			modelView.glScale(2/rViewport.width, 2/rViewport.height, 1);
			modelView.glTranslate(x,y, 0);
			mutThis->loadMatrix(modelView);
			break;

		case OF_BITMAPMODE_MODEL:

			hasModelView = true;
			mutThis->matrixMode(OF_MATRIX_MODELVIEW);
			mutThis->pushMatrix();

			mutThis->translate(x, y, z);
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
			mutThis->matrixMode(OF_MATRIX_PROJECTION);
			mutThis->pushMatrix();
			mutThis->loadIdentityMatrix();

			hasModelView = true;
			mutThis->matrixMode(OF_MATRIX_MODELVIEW);
			mutThis->pushMatrix();

			modelView.makeTranslationMatrix(-1,-1,0);
			modelView.glScale(2/rViewport.width, 2/rViewport.height, 1);
			modelView.glTranslate(dScreen.x, dScreen.y, 0);
			mutThis->loadMatrix(modelView);
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
	mutThis->setAlphaBitmapText(true);
	ofMesh charMesh = bitmapFont.getMesh(textString, sx, sy, currentStyle.drawBitmapMode, isVFlipped());
	mutThis->bind(bitmapFont.getTexture(),0);
	draw(charMesh,OF_MESH_FILL,false,true,false);
	mutThis->unbind(bitmapFont.getTexture(),0);
	mutThis->setAlphaBitmapText(false);


	if (hasViewport){
		mutThis->popView();
	}else{
		if (hasModelView){
			mutThis->popMatrix();
		}

		if (hasProjection)
		{
			mutThis->matrixMode(OF_MATRIX_PROJECTION);
			mutThis->popMatrix();
			mutThis->matrixMode(OF_MATRIX_MODELVIEW);
		}
	}
}


//----------------------------------------------------------
void ofGLProgrammableRenderer::drawString(const ofTrueTypeFont & font, string text, float x, float y) const{
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer*>(this);
	ofBlendMode blendMode = currentStyle.blendingMode;

	mutThis->setBlendMode(OF_BLENDMODE_ALPHA);

	mutThis->bind(font.getFontTexture(),0);
	draw(font.getStringMesh(text,x,y,isVFlipped()),OF_MESH_FILL);
	mutThis->unbind(font.getFontTexture(),0);

	mutThis->setBlendMode(blendMode);
}

#define STRINGIFY(x) #x


// ----------------------------------------------------------------------
// tig: GLSL #150 shaders written against spec:
// http://www.opengl.org/registry/doc/GLSLangSpec.1.50.09.pdf

#ifdef TARGET_OPENGLES
static const string vertex_shader_header =
		"precision mediump float;\n"
		"#define IN attribute\n"
		"#define OUT varying\n"
		"#define TEXTURE texture2D\n"
		"#define TARGET_OPENGLES\n";
static const string fragment_shader_header =
		"precision mediump float;\n"
		"#define IN varying\n"
		"#define OUT\n"
		"#define TEXTURE texture2D\n"
		"#define FRAG_COLOR gl_FragColor\n"
		"#define TARGET_OPENGLES\n";
#else
static const string vertex_shader_header =
		"#version %glsl_version%\n"
		"%extensions%\n"
		"#define IN in\n"
		"#define OUT out\n"
		"#define TEXTURE texture\n";
static const string fragment_shader_header =
		"#version %glsl_version%\n"
		"%extensions%\n"
		"#define IN in\n"
		"#define OUT out\n"
		"#define TEXTURE texture\n"
		"#define FRAG_COLOR fragColor\n"
		"out vec4 fragColor;\n";
#endif

static const string defaultVertexShader = vertex_shader_header + STRINGIFY(
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

static const string defaultFragmentShaderTexRectColor = fragment_shader_header + STRINGIFY(

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

static const string defaultFragmentShaderTexRectNoColor = fragment_shader_header + STRINGIFY(

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

static const string alphaMaskFragmentShaderTexRectNoColor = fragment_shader_header + STRINGIFY(

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

static const string alphaMaskFragmentShaderTex2DNoColor = fragment_shader_header + STRINGIFY(

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

static const string defaultFragmentShaderTex2DColor = fragment_shader_header + STRINGIFY(

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

static const string defaultFragmentShaderTex2DNoColor = fragment_shader_header + STRINGIFY(

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

static const string defaultFragmentShaderNoTexColor = fragment_shader_header + STRINGIFY (

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

static const string defaultFragmentShaderNoTexNoColor = fragment_shader_header + STRINGIFY(

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

static const string bitmapStringVertexShader = vertex_shader_header + STRINGIFY(

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

static const string bitmapStringFragmentShader = fragment_shader_header + STRINGIFY(

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

static const string uniqueVertexShader = vertex_shader_header + STRINGIFY(
        
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
static const string uniqueFragmentShader = fragment_shader_header + STRINGIFY(
        
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
static const string FRAGMENT_SHADER_YUY2 = STRINGIFY(
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
static const string FRAGMENT_SHADER_NV12_NV21 = STRINGIFY(
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
static const string FRAGMENT_SHADER_PLANAR_YUV = STRINGIFY(
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

static string defaultShaderHeader(string header, GLenum textureTarget, int major, int minor){
	ofStringReplace(header,"%glsl_version%",ofGLSLVersionFromGL(major,minor));
#ifndef TARGET_OPENGLES
	if(major<4 && minor<2){
		ofStringReplace(header,"%extensions%","#extension GL_ARB_texture_rectangle : enable");
	}else{
		ofStringReplace(header,"%extensions%","");
	}
#endif
	if(textureTarget==GL_TEXTURE_2D){
		header += "#define SAMPLER sampler2D\n";
	}else{
		header += "#define SAMPLER sampler2DRect\n";
	}
	return header;
}


static string shaderSource(const string & src, int major, int minor){
	string shaderSrc = src;
	ofStringReplace(shaderSrc,"%glsl_version%",ofGLSLVersionFromGL(major,minor));
#ifndef TARGET_OPENGLES
	if(major<4 && minor<2){
		ofStringReplace(shaderSrc,"%extensions%","#extension GL_ARB_texture_rectangle : enable");
	}else{
		ofStringReplace(shaderSrc,"%extensions%","");
	}
#endif
	return shaderSrc;
}

static string videoFragmentShaderSource(const ofBaseVideoDraws & video, int major, int minor){
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
	return shaderSource(header + src, major, minor);
}

string ofGLProgrammableRenderer::defaultVertexShaderHeader(GLenum textureTarget){
	return defaultShaderHeader(vertex_shader_header,textureTarget,major,minor);
}

string ofGLProgrammableRenderer::defaultFragmentShaderHeader(GLenum textureTarget){
	return defaultShaderHeader(fragment_shader_header,textureTarget,major,minor);
}

void ofGLProgrammableRenderer::setup(int _major, int _minor){
	glGetError();

	major = _major;
	minor = _minor;
#ifdef TARGET_RASPBERRY_PI
	uniqueShader = true;
#else
	uniqueShader = false;
#endif

	if(uniqueShader){
		defaultUniqueShader.setupShaderFromSource(GL_VERTEX_SHADER,uniqueVertexShader);
		defaultUniqueShader.setupShaderFromSource(GL_FRAGMENT_SHADER,uniqueFragmentShader);
		defaultUniqueShader.bindDefaults();
		defaultUniqueShader.linkProgram();
		beginDefaultShader();
	}else{
	#ifndef TARGET_OPENGLES
		defaultTexRectColor.setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,major, minor));
		defaultTexRectNoColor.setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,major, minor));
		alphaMaskRectShader.setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,major, minor));
	#endif
		defaultTex2DColor.setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,major, minor));
		defaultNoTexColor.setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,major, minor));
		defaultTex2DNoColor.setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,major, minor));
		defaultNoTexNoColor.setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,major, minor));
		alphaMask2DShader.setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,major, minor));

	#ifndef TARGET_OPENGLES
		defaultTexRectColor.setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderTexRectColor,major, minor));
		defaultTexRectNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderTexRectNoColor,major, minor));
		alphaMaskRectShader.setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(alphaMaskFragmentShaderTexRectNoColor,major, minor));
	#endif
		defaultTex2DColor.setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderTex2DColor,major, minor));
		defaultNoTexColor.setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderNoTexColor,major, minor));
		defaultTex2DNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderTex2DNoColor,major, minor));
		defaultNoTexNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(defaultFragmentShaderNoTexNoColor,major, minor));
		alphaMask2DShader.setupShaderFromSource(GL_FRAGMENT_SHADER,shaderSource(alphaMaskFragmentShaderTex2DNoColor,major, minor));


		bitmapStringShader.setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(bitmapStringVertexShader,major, minor));
		bitmapStringShader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(bitmapStringFragmentShader,major, minor));

#ifndef TARGET_OPENGLES
		defaultTexRectColor.bindDefaults();
		defaultTexRectNoColor.bindDefaults();
		alphaMaskRectShader.bindDefaults();
#endif
		defaultTex2DColor.bindDefaults();
		defaultNoTexColor.bindDefaults();
		defaultTex2DNoColor.bindDefaults();
		defaultNoTexNoColor.bindDefaults();
		alphaMask2DShader.bindDefaults();

#ifndef TARGET_OPENGLES
		defaultTexRectColor.linkProgram();
		defaultTexRectNoColor.linkProgram();
		alphaMaskRectShader.linkProgram();
#endif
		defaultTex2DColor.linkProgram();
		defaultNoTexColor.linkProgram();
		defaultTex2DNoColor.linkProgram();
		defaultNoTexNoColor.linkProgram();
		alphaMask2DShader.linkProgram();

		bitmapStringShader.bindDefaults();
		bitmapStringShader.linkProgram();
	}

	setupGraphicDefaults();
	viewport();
	setupScreenPerspective();
}

const ofShader * ofGLProgrammableRenderer::getVideoShader(const ofBaseVideoDraws & video) const{
	const ofShader * shader = NULL;
	GLenum target = video.getTexture().getTextureData().textureTarget;
	switch(video.getPixelFormat()){
		case OF_PIXELS_YUY2:
			if(target==GL_TEXTURE_2D){
				shader = &shaderPlanarYUY2;
			}else{
				shader = &shaderPlanarYUY2Rect;
			}
			break;
		case OF_PIXELS_NV12:
			if(target==GL_TEXTURE_2D){
				shader = &shaderNV12;
			}else{
				shader = &shaderNV12Rect;
			}
			break;
		case OF_PIXELS_NV21:
			if(target==GL_TEXTURE_2D){
				shader = &shaderNV21;
			}else{
				shader = &shaderNV21Rect;
			}
			break;
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
			if(target==GL_TEXTURE_2D){
				shader = &shaderPlanarYUV;
			}else{
				shader = &shaderPlanarYUVRect;
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
		mutShader->setupShaderFromSource(GL_VERTEX_SHADER,shaderSource(defaultVertexShader,major,minor));
		mutShader->setupShaderFromSource(GL_FRAGMENT_SHADER,videoFragmentShaderSource(video,major,minor));
		mutShader->bindDefaults();
		mutShader->linkProgram();
	}
	return shader;
}

#ifndef TARGET_OPENGLES
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
#endif

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

int ofGLProgrammableRenderer::getGLVersionMajor(){
	return major;
}

int ofGLProgrammableRenderer::getGLVersionMinor(){
	return minor;
}


void ofGLProgrammableRenderer::saveFullViewport(ofPixels & pixels){
	ofRectangle v = getCurrentViewport();
	saveScreen(v.x,v.y,v.width,v.height,pixels);
}

void ofGLProgrammableRenderer::saveScreen(int x, int y, int w, int h, ofPixels & pixels){

    int sh = getViewportHeight();


	#ifndef TARGET_OPENGLES
	ofBufferObject buffer;
	pixels.allocate(w, h, OF_PIXELS_RGB);
	buffer.allocate(pixels.size(),GL_STATIC_READ);
	if(isVFlipped()){
		y = sh - y;
		y -= h; // top, bottom issues
	}

	buffer.bind(GL_PIXEL_PACK_BUFFER);
	glReadPixels(x, y, w, h, ofGetGlFormat(pixels), GL_UNSIGNED_BYTE, 0); // read the memory....
	buffer.unbind(GL_PIXEL_PACK_BUFFER);
	unsigned char * p = buffer.map<unsigned char>(GL_READ_ONLY);
	ofPixels src;
	src.setFromExternalPixels(p,w,h,OF_PIXELS_RGB);
	src.mirrorTo(pixels,true,false);
	buffer.unmap();


	#else

	int sw = getViewportWidth();
	int numPixels   = w*h;
	if( numPixels == 0 ){
		ofLogError("ofImage") << "grabScreen(): unable to grab screen, image width and/or height are 0: " << w << "x" << h;
		return;
	}
	pixels.allocate(w, h, OF_PIXELS_RGBA);

	switch(matrixStack.getOrientation()){
	case OF_ORIENTATION_DEFAULT:

		if(isVFlipped()){
			y = sh - y;   // screen is flipped vertically.
			y -= h;
		}

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.getData());
		pixels.mirror(true,false);
		break;
	case OF_ORIENTATION_180:

		if(isVFlipped()){
			x = sw - x;   // screen is flipped horizontally.
			x -= w;
		}

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.getData());
		pixels.mirror(false,true);
		break;
	case OF_ORIENTATION_90_RIGHT:
		swap(w,h);
		swap(x,y);
		if(!isVFlipped()){
			x = sw - x;   // screen is flipped horizontally.
			x -= w;

			y = sh - y;   // screen is flipped vertically.
			y -= h;
		}

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.getData());
		pixels.mirror(true,true);
		break;
	case OF_ORIENTATION_90_LEFT:
		swap(w, h);
		swap(x, y);
		if(isVFlipped()){
			x = sw - x;   // screen is flipped horizontally.
			x -= w;

			y = sh - y;   // screen is flipped vertically.
			y -= h;
		}

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.getData());
		pixels.mirror(true,true);
		break;
	}

	#endif
}

const of3dGraphics & ofGLProgrammableRenderer::get3dGraphics() const{
	return graphics3d;
}

of3dGraphics & ofGLProgrammableRenderer::get3dGraphics(){
	return graphics3d;
}
