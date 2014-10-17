#include "ofGLRenderer.h"
#include "ofMesh.h"
#include "ofPath.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofMesh.h"
#include "of3dPrimitives.h"
#include "ofBitmapFont.h"
#include "ofGLUtils.h"
#include "ofImage.h"
#include "ofFbo.h"
#include "ofLight.h"
#include "ofMaterial.h"

const string ofGLRenderer::TYPE="GL";

//----------------------------------------------------------
ofGLRenderer::ofGLRenderer(bool useShapeColor)
:matrixStack(*ofGetWindowPtr()){
	bBackgroundAuto = true;

	linePoints.resize(2);
	rectPoints.resize(4);
	triPoints.resize(3);
	fillFlag = OF_FILLED;
	bSmoothHinted = false;
	normalsEnabled = false;
	lightingEnabled = true;
	rectMode = OF_RECTMODE_CORNER;
	alphaMaskTextureTarget = GL_TEXTURE_2D;
}

//----------------------------------------------------------
void ofGLRenderer::update(){
    matrixStack.setRenderSurface(*ofGetWindowPtr());
}

//----------------------------------------------------------
void ofGLRenderer::draw(const ofMesh & vertexData, bool useColors, bool useTextures, bool useNormals) const{
	if(vertexData.getNumVertices()){
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &vertexData.getVerticesPointer()->x);
	}
	if(vertexData.getNumNormals() && useNormals){
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(ofVec3f), &vertexData.getNormalsPointer()->x);
	}
	if(vertexData.getNumColors() && useColors){
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT, sizeof(ofFloatColor), &vertexData.getColorsPointer()->r);
	}

	if(vertexData.getNumTexCoords() && useTextures){
		set<int>::iterator textureLocation = textureLocationsEnabled.begin();
		for(;textureLocation!=textureLocationsEnabled.end();textureLocation++){
			glActiveTexture(GL_TEXTURE0+*textureLocation);
			glClientActiveTexture(GL_TEXTURE0+*textureLocation);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, sizeof(ofVec2f), &vertexData.getTexCoordsPointer()->x);
		}
		glActiveTexture(GL_TEXTURE0);
		glClientActiveTexture(GL_TEXTURE0);
	}

	if(vertexData.getNumIndices()){
#ifdef TARGET_OPENGLES
		glDrawElements(ofGetGLPrimitiveMode(vertexData.getMode()), vertexData.getNumIndices(),GL_UNSIGNED_SHORT,vertexData.getIndexPointer());
#else
		glDrawElements(ofGetGLPrimitiveMode(vertexData.getMode()), vertexData.getNumIndices(),GL_UNSIGNED_INT,vertexData.getIndexPointer());
#endif
	}else{
		glDrawArrays(ofGetGLPrimitiveMode(vertexData.getMode()), 0, vertexData.getNumVertices());
	}

	if(vertexData.getNumColors() && useColors){
		glDisableClientState(GL_COLOR_ARRAY);
	}
	if(vertexData.getNumNormals() && useNormals){
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	if(vertexData.getNumTexCoords() && useTextures){
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

//----------------------------------------------------------
void ofGLRenderer::draw(const ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const{
		if (bSmoothHinted) const_cast<ofGLRenderer*>(this)->startSmoothing();
#ifndef TARGET_OPENGLES
		glPushAttrib(GL_POLYGON_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
		draw(vertexData,useColors,useTextures,useNormals);
		glPopAttrib(); //TODO: GLES doesnt support polygon mode, add renderType to gl renderer?
#else
		if(vertexData.getNumVertices()){
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), vertexData.getVerticesPointer());
		}
		if(vertexData.getNumNormals() && useNormals){
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, vertexData.getNormalsPointer());
		}
		if(vertexData.getNumColors() && useColors){
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4,GL_FLOAT, sizeof(ofFloatColor), vertexData.getColorsPointer());
		}

		if(vertexData.getNumTexCoords() && useTextures){
			set<int>::iterator textureLocation = textureLocationsEnabled.begin();
			for(;textureLocation!=textureLocationsEnabled.end();textureLocation++){
				glActiveTexture(GL_TEXTURE0+*textureLocation);
				glClientActiveTexture(GL_TEXTURE0+*textureLocation);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, sizeof(ofVec2f), &vertexData.getTexCoordsPointer()->x);
			}
			glActiveTexture(GL_TEXTURE0);
			glClientActiveTexture(GL_TEXTURE0);
		}

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
		if(vertexData.getNumColors() && useColors){
			glDisableClientState(GL_COLOR_ARRAY);
		}
		if(vertexData.getNumNormals() && useNormals){
			glDisableClientState(GL_NORMAL_ARRAY);
		}
		if(vertexData.getNumTexCoords() && useTextures){
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
#endif
		if (bSmoothHinted) const_cast<ofGLRenderer*>(this)->endSmoothing();
}

//----------------------------------------------------------
void ofGLRenderer::draw( const of3dPrimitive& model, ofPolyRenderMode renderType)  const{
	if(model.isUsingVbo()){
		model.getMesh().draw(renderType);
	}else{
		draw(model.getMesh(),renderType);
	}
}

//----------------------------------------------------------
void ofGLRenderer::draw(const ofPolyline & poly) const{
	if(!poly.getVertices().empty()) {
		// use smoothness, if requested:
		if (bSmoothHinted) const_cast<ofGLRenderer*>(this)->startSmoothing();

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &poly.getVertices()[0].x);
		glDrawArrays(poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, poly.size());

		// use smoothness, if requested:
		if (bSmoothHinted) const_cast<ofGLRenderer*>(this)->endSmoothing();
	}
}

//----------------------------------------------------------
void ofGLRenderer::draw(const ofPath & shape) const{
	ofColor prevColor;
	if(shape.getUseShapeColor()){
		prevColor = ofGetStyle().color;
	}
	ofGLRenderer * mut_this = const_cast<ofGLRenderer*>(this);
	if(shape.isFilled()){
		const ofMesh & mesh = shape.getTessellation();
		if(shape.getUseShapeColor()){
			mut_this->setColor( shape.getFillColor(),shape.getFillColor().a);
		}
		draw(mesh,OF_MESH_FILL);
	}
	if(shape.hasOutline()){
		float lineWidth = ofGetStyle().lineWidth;
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
void ofGLRenderer::draw(const ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	if(image.isUsingTexture()){
		const ofTexture& tex = image.getTextureReference();
		if(tex.bAllocated()) {
			tex.bind();
			draw(tex.getMeshForSubsection(x,y,z,w,h,sx,sy,sw,sh),false,true,false);
			tex.unbind();
		} else {
			ofLogWarning("ofGLRenderer") << "drawing an unallocated texture";
		}
	}
}

//----------------------------------------------------------
void ofGLRenderer::draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	if(image.isUsingTexture()){
		const ofTexture& tex = image.getTextureReference();
		if(tex.bAllocated()) {
			tex.bind();
			draw(tex.getMeshForSubsection(x,y,z,w,h,sx,sy,sw,sh),false,true,false);
			tex.unbind();
		} else {
			ofLogWarning("ofGLRenderer") << "draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLRenderer::draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	if(image.isUsingTexture()){
		const ofTexture& tex = image.getTextureReference();
		if(tex.bAllocated()) {
			tex.bind();
			draw(tex.getMeshForSubsection(x,y,z,w,h,sx,sy,sw,sh),false,true,false);
			tex.unbind();
		} else {
			ofLogWarning("ofGLRenderer") << "draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLRenderer::draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const{
	if(video.isInitialized() && video.isUsingTexture()){
		const ofTexture& tex = video.getTextureReference();
		tex.bind();
		draw(tex.getMeshForSubsection(x,y,0,w,h,0,0,w,h),false,true,false);
		tex.unbind();
	}
}

//----------------------------------------------------------
void ofGLRenderer::bind(const ofBaseVideoDraws & video) const{
	if(video.isInitialized() && video.isUsingTexture()){
		video.getTextureReference().bind();
	}
}

//----------------------------------------------------------
void ofGLRenderer::unbind(const ofBaseVideoDraws & video) const{
	if(video.isInitialized() && video.isUsingTexture()){
		video.getTextureReference().unbind();
	}
}

//----------------------------------------------------------
void ofGLRenderer::setCurrentFBO(const ofFbo * fbo){
	if(fbo!=NULL){
		ofMatrix4x4 m;
		glGetFloatv(GL_PROJECTION_MATRIX,m.getPtr());
		m =  m*matrixStack.getOrientationMatrixInverse();
		ofMatrixMode currentMode = matrixStack.getCurrentMatrixMode();
		matrixStack.matrixMode(OF_MATRIX_PROJECTION);
		matrixStack.loadMatrix(m.getPtr());
		matrixStack.setRenderSurface(*fbo);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(matrixStack.getProjectionMatrix().getPtr());
		matrixMode(currentMode);
	}else{
		matrixStack.setRenderSurface(*ofGetWindowPtr());
	}
}

//----------------------------------------------------------
void ofGLRenderer::pushView() {
	getCurrentViewport();

	ofMatrix4x4 m;
	ofMatrixMode matrixMode = matrixStack.getCurrentMatrixMode();
	glGetFloatv(GL_PROJECTION_MATRIX,m.getPtr());
	matrixStack.matrixMode(OF_MATRIX_PROJECTION);
	matrixStack.loadMatrix(m.getPtr());
	glGetFloatv(GL_MODELVIEW_MATRIX,m.getPtr());
	matrixStack.matrixMode(OF_MATRIX_MODELVIEW);
	matrixStack.loadMatrix(m.getPtr());

	matrixStack.matrixMode(matrixMode);

	matrixStack.pushView();
}


//----------------------------------------------------------
void ofGLRenderer::popView() {
	matrixStack.popView();

	ofMatrix4x4 m;
	ofMatrixMode currentMode = matrixStack.getCurrentMatrixMode();

	matrixMode(OF_MATRIX_PROJECTION);
	loadMatrix(matrixStack.getProjectionMatrix());

	matrixMode(OF_MATRIX_MODELVIEW);
	loadMatrix(matrixStack.getModelViewMatrix());

	matrixMode(currentMode);

	viewport(matrixStack.getCurrentViewport());
}


//----------------------------------------------------------
void ofGLRenderer::viewport(ofRectangle viewport_){
	viewport(viewport_.x, viewport_.y, viewport_.width, viewport_.height, isVFlipped());
}

//----------------------------------------------------------
void ofGLRenderer::viewport(float x, float y, float width, float height, bool vflip) {
	matrixStack.viewport(x,y,width,height,vflip);
	ofRectangle nativeViewport = matrixStack.getNativeViewport();
	glViewport(nativeViewport.x,nativeViewport.y,nativeViewport.width,nativeViewport.height);
}

//----------------------------------------------------------
ofRectangle ofGLRenderer::getCurrentViewport() const{
	getNativeViewport();
	return matrixStack.getCurrentViewport();
}

//----------------------------------------------------------
ofRectangle ofGLRenderer::getNativeViewport() const{
	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);

	ofGLRenderer * mutRenderer = const_cast<ofGLRenderer*>(this);
	ofRectangle nativeViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	mutRenderer->matrixStack.nativeViewport(nativeViewport);
    return nativeViewport;
}

//----------------------------------------------------------
int ofGLRenderer::getViewportWidth() const{
	return getCurrentViewport().width;
}

//----------------------------------------------------------
int ofGLRenderer::getViewportHeight() const{
	return getCurrentViewport().height;
}

//----------------------------------------------------------
void ofGLRenderer::setCoordHandedness(ofHandednessType handedness) {

}

//----------------------------------------------------------
ofHandednessType ofGLRenderer::getCoordHandedness() const{
	return matrixStack.getHandedness();
}

//----------------------------------------------------------
void ofGLRenderer::setOrientation(ofOrientation orientation, bool vFlip){
	matrixStack.setOrientation(orientation,vFlip);
}

//----------------------------------------------------------
bool ofGLRenderer::isVFlipped() const{
	return matrixStack.isVFlipped();
}

//----------------------------------------------------------
bool ofGLRenderer::texturesNeedVFlip() const{
	return matrixStack.customMatrixNeedsFlip();
}

//----------------------------------------------------------
void ofGLRenderer::setupScreenPerspective(float width, float height, float fov, float nearDist, float farDist) {
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
void ofGLRenderer::setupScreenOrtho(float width, float height, float nearDist, float farDist) {
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
void ofGLRenderer::setupGraphicDefaults(){

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//----------------------------------------------------------
void ofGLRenderer::setupScreen(){
	setupScreenPerspective();	// assume defaults
}

//----------------------------------------------------------
void ofGLRenderer::setCircleResolution(int res){
	if((int)circlePolyline.size()!=res+1){
		circlePolyline.clear();
		circlePolyline.arc(0,0,0,1,1,0,360,res);
		circlePoints.resize(circlePolyline.size());
	}
}

//our openGL wrappers
//----------------------------------------------------------
void ofGLRenderer::pushMatrix(){
	glPushMatrix();
}

//----------------------------------------------------------
void ofGLRenderer::popMatrix(){
	glPopMatrix();
}

//----------------------------------------------------------
void ofGLRenderer::translate(const ofPoint& p){
	glTranslatef(p.x, p.y, p.z);
}

//----------------------------------------------------------
void ofGLRenderer::translate(float x, float y, float z){
	glTranslatef(x, y, z);
}

//----------------------------------------------------------
void ofGLRenderer::scale(float xAmnt, float yAmnt, float zAmnt){
	glScalef(xAmnt, yAmnt, zAmnt);
}

//----------------------------------------------------------
void ofGLRenderer::rotate(float degrees, float vecX, float vecY, float vecZ){
	glRotatef(degrees, vecX, vecY, vecZ);
}

//----------------------------------------------------------
void ofGLRenderer::rotateX(float degrees){
	glRotatef(degrees, 1, 0, 0);
}

//----------------------------------------------------------
void ofGLRenderer::rotateY(float degrees){
	glRotatef(degrees, 0, 1, 0);
}

//----------------------------------------------------------
void ofGLRenderer::rotateZ(float degrees){
	glRotatef(degrees, 0, 0, 1);
}

//same as ofRotateZ
//----------------------------------------------------------
void ofGLRenderer::rotate(float degrees){
	glRotatef(degrees, 0, 0, 1);
}

//----------------------------------------------------------
void ofGLRenderer::matrixMode(ofMatrixMode mode){
	glMatrixMode(GL_MODELVIEW+mode);
	matrixStack.matrixMode(mode);
}

//----------------------------------------------------------
void ofGLRenderer::loadIdentityMatrix (void){
	loadMatrix(ofMatrix4x4::newIdentityMatrix());
}

//----------------------------------------------------------
void ofGLRenderer::loadMatrix (const ofMatrix4x4 & m){
	loadMatrix( m.getPtr() );
}

//----------------------------------------------------------
void ofGLRenderer::loadMatrix (const float *m){
	if(matrixStack.getCurrentMatrixMode()==OF_MATRIX_PROJECTION){
		matrixStack.loadMatrix(m);
		glLoadMatrixf(matrixStack.getProjectionMatrix().getPtr());
	}else{
		glLoadMatrixf(m);
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
ofMatrix4x4 ofGLRenderer::getCurrentMatrix(ofMatrixMode matrixMode_) const {
	ofMatrix4x4 mat;
	switch (matrixMode_) {
		case OF_MATRIX_MODELVIEW:
			glGetFloatv(GL_MODELVIEW_MATRIX, mat.getPtr());
			break;
		case OF_MATRIX_PROJECTION:
			glGetFloatv(GL_PROJECTION_MATRIX, mat.getPtr());
			break;
		case OF_MATRIX_TEXTURE:
			glGetFloatv(GL_TEXTURE_MATRIX, mat.getPtr());
			break;
		default:
			ofLogWarning() << "Invalid getCurrentMatrix query";
			mat = ofMatrix4x4();
			break;
	}
	return mat;
}

//----------------------------------------------------------
ofMatrix4x4 ofGLRenderer::getCurrentOrientationMatrix() const {
	return matrixStack.getOrientationMatrix();
}

//----------------------------------------------------------
void ofGLRenderer::multMatrix (const ofMatrix4x4 & m){
	multMatrix( m.getPtr() );
}

//----------------------------------------------------------
void ofGLRenderer::multMatrix (const float *m){
	if(matrixStack.getCurrentMatrixMode()==OF_MATRIX_PROJECTION){
		ofMatrix4x4 current;
		glGetFloatv(GL_PROJECTION_MATRIX,current.getPtr());
		if(matrixStack.customMatrixNeedsFlip()){
			current.scale(1,-1,1);
		}
		matrixStack.loadMatrix(current.getPtr());
		matrixStack.multMatrix(m);
		glLoadMatrixf(matrixStack.getProjectionMatrix().getPtr());
	}else{
		glMultMatrixf(m);
	}
}

//----------------------------------------------------------
void ofGLRenderer::loadViewMatrix(const ofMatrix4x4 & m){
	int matrixMode;
	glGetIntegerv(GL_MATRIX_MODE,&matrixMode);
	matrixStack.loadViewMatrix(m);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m.getPtr());
	glMatrixMode(matrixMode);

	if(lightingEnabled){
		for(size_t i=0;i<ofLightsData().size();i++){
			shared_ptr<ofLight::Data> lightData = ofLightsData()[i].lock();
			if(lightData && lightData->isEnabled){
				glLightfv(GL_LIGHT0 + lightData->glIndex, GL_POSITION, &lightData->position.x);
			}
		}
	}
}

//----------------------------------------------------------
void ofGLRenderer::multViewMatrix(const ofMatrix4x4 & m){
	ofLogError() << "mutlViewMatrix not implemented on fixed GL renderer";
}

//----------------------------------------------------------
ofMatrix4x4 ofGLRenderer::getCurrentViewMatrix() const{
	return matrixStack.getViewMatrix();
}

//----------------------------------------------------------
ofMatrix4x4 ofGLRenderer::getCurrentNormalMatrix() const{
	return ofMatrix4x4::getTransposedOf(getCurrentMatrix(OF_MATRIX_MODELVIEW).getInverse());
}

//----------------------------------------------------------
void ofGLRenderer::setColor(const ofColor & color){
	setColor(color.r,color.g,color.b,color.a);
}

//----------------------------------------------------------
void ofGLRenderer::setColor(const ofColor & color, int _a){
	setColor(color.r,color.g,color.b,_a);
}

//----------------------------------------------------------
void ofGLRenderer::setColor(int _r, int _g, int _b){
	glColor4f(_r/255.f,_g/255.f,_b/255.f,1.f);
}


//----------------------------------------------------------
void ofGLRenderer::setColor(int _r, int _g, int _b, int _a){
	glColor4f(_r/255.f,_g/255.f,_b/255.f,_a/255.f);
}

//----------------------------------------------------------
void ofGLRenderer::setColor(int gray){
	setColor(gray, gray, gray);
}

//----------------------------------------------------------
void ofGLRenderer::setHexColor(int hexColor){
	int r = (hexColor >> 16) & 0xff;
	int g = (hexColor >> 8) & 0xff;
	int b = (hexColor >> 0) & 0xff;
	setColor(r,g,b);
}

//----------------------------------------------------------
void ofGLRenderer::clear(float r, float g, float b, float a) {
	glClearColor(r / 255., g / 255., b / 255., a / 255.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
void ofGLRenderer::clear(float brightness, float a) {
	clear(brightness, brightness, brightness, a);
}

//----------------------------------------------------------
void ofGLRenderer::clearAlpha() {
	glColorMask(0, 0, 0, 1);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColorMask(1, 1, 1, 1);
}

//----------------------------------------------------------
void ofGLRenderer::setBackgroundAuto(bool bAuto){
	bBackgroundAuto = bAuto;
}

//----------------------------------------------------------
bool ofGLRenderer::bClearBg(){
	return bBackgroundAuto;
}

//----------------------------------------------------------
ofFloatColor & ofGLRenderer::getBgColor(){
	return bgColor;
}

//----------------------------------------------------------
void ofGLRenderer::background(const ofColor & c){
	bgColor = c;
	glClearColor(bgColor[0],bgColor[1],bgColor[2], bgColor[3]);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
void ofGLRenderer::background(float brightness) {
	background(brightness);
}

//----------------------------------------------------------
void ofGLRenderer::background(int hexColor, float _a){
	background ( (hexColor >> 16) & 0xff, (hexColor >> 8) & 0xff, (hexColor >> 0) & 0xff, _a);
}

//----------------------------------------------------------
void ofGLRenderer::background(int r, int g, int b, int a){
	background(ofColor(r,g,b,a));
}

//----------------------------------------------------------
void ofGLRenderer::setFillMode(ofFillFlag fill){
	fillFlag = fill;
}

//----------------------------------------------------------
ofFillFlag ofGLRenderer::getFillMode(){
	return fillFlag;
}

//----------------------------------------------------------
void ofGLRenderer::setRectMode(ofRectMode mode){
	rectMode = mode;
}

//----------------------------------------------------------
ofRectMode ofGLRenderer::getRectMode(){
	return rectMode;
}

//----------------------------------------------------------
void ofGLRenderer::setLineWidth(float lineWidth){
	glLineWidth(lineWidth);
}

//----------------------------------------------------------
void ofGLRenderer::setDepthTest(bool depthTest){
	if(depthTest) {
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_DEPTH_TEST);
	}
}

//----------------------------------------------------------
void ofGLRenderer::setLineSmoothing(bool smooth){
	bSmoothHinted = smooth;
}


//----------------------------------------------------------
void ofGLRenderer::startSmoothing(){
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
void ofGLRenderer::endSmoothing(){
	#ifndef TARGET_OPENGLES
		glPopAttrib();
	#endif
}

//----------------------------------------------------------
void ofGLRenderer::setBlendMode(ofBlendMode blendMode){
	switch (blendMode){
		case OF_BLENDMODE_DISABLED:
			glDisable(GL_BLEND);
			break;

		case OF_BLENDMODE_ALPHA:{
			glEnable(GL_BLEND);
			#ifndef TARGET_OPENGLES
				glBlendEquation(GL_FUNC_ADD);
			#endif
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}

		case OF_BLENDMODE_ADD:{
			glEnable(GL_BLEND);
			#ifndef TARGET_OPENGLES
				glBlendEquation(GL_FUNC_ADD);
			#endif
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		}

		case OF_BLENDMODE_MULTIPLY:{
			glEnable(GL_BLEND);
			#ifndef TARGET_OPENGLES
				glBlendEquation(GL_FUNC_ADD);
			#endif
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA /* GL_ZERO or GL_ONE_MINUS_SRC_ALPHA */);
			break;
		}

		case OF_BLENDMODE_SCREEN:{
			glEnable(GL_BLEND);
			#ifndef TARGET_OPENGLES
				glBlendEquation(GL_FUNC_ADD);
			#endif
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
			break;
		}

		case OF_BLENDMODE_SUBTRACT:{
			glEnable(GL_BLEND);
		#ifndef TARGET_OPENGLES
			glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		#else
			ofLogWarning("ofGLRenderer") << "OF_BLENDMODE_SUBTRACT not currently supported on OpenGL ES";
		#endif
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		}

		default:
			break;
	}
}

//----------------------------------------------------------
void ofGLRenderer::enablePointSprites(){

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
void ofGLRenderer::disablePointSprites(){

#ifdef TARGET_OPENGLES
	glDisable(GL_POINT_SPRITE_OES);
#else
	glDisable(GL_POINT_SPRITE);
#endif
}

//----------------------------------------------------------
void ofGLRenderer::enableAntiAliasing(){
	glEnable(GL_MULTISAMPLE);
}

//----------------------------------------------------------
void ofGLRenderer::disableAntiAliasing(){
	glDisable(GL_MULTISAMPLE);
}

//----------------------------------------------------------
void ofGLRenderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2){
	linePoints[0].set(x1,y1,z1);
	linePoints[1].set(x2,y2,z2);

	// use smoothness, if requested:
	if (bSmoothHinted) startSmoothing();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &linePoints[0].x);
	glDrawArrays(GL_LINES, 0, 2);

	// use smoothness, if requested:
	if (bSmoothHinted) endSmoothing();

}

//----------------------------------------------------------
void ofGLRenderer::drawRectangle(float x, float y, float z,float w, float h){

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

	// use smoothness, if requested:
	if (bSmoothHinted && fillFlag == OF_OUTLINE) startSmoothing();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &rectPoints[0].x);
	glDrawArrays((fillFlag == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, 4);

	// use smoothness, if requested:
	if (bSmoothHinted && fillFlag == OF_OUTLINE) endSmoothing();

}

//----------------------------------------------------------
void ofGLRenderer::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	triPoints[0].set(x1,y1,z1);
	triPoints[1].set(x2,y2,z2);
	triPoints[2].set(x3,y3,z3);

	// use smoothness, if requested:
	if (bSmoothHinted && fillFlag == OF_OUTLINE) startSmoothing();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &triPoints[0].x);
	glDrawArrays((fillFlag == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, 3);

	// use smoothness, if requested:
	if (bSmoothHinted && fillFlag == OF_OUTLINE) endSmoothing();

}

//----------------------------------------------------------
void ofGLRenderer::drawCircle(float x, float y, float z,  float radius){
	vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		circlePoints[i].set(radius*circleCache[i].x+x,radius*circleCache[i].y+y,z);
	}

	// use smoothness, if requested:
	if (bSmoothHinted && fillFlag == OF_OUTLINE) startSmoothing();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &circlePoints[0].x);
	glDrawArrays((fillFlag == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_STRIP, 0, circlePoints.size());

	// use smoothness, if requested:
	if (bSmoothHinted && fillFlag == OF_OUTLINE) endSmoothing();

}

//----------------------------------------------------------
void ofGLRenderer::drawEllipse(float x, float y, float z, float width, float height){
	float radiusX = width*0.5;
	float radiusY = height*0.5;
	vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		circlePoints[i].set(radiusX*circlePolyline[i].x+x,radiusY*circlePolyline[i].y+y,z);
	}

	// use smoothness, if requested:
	if (bSmoothHinted && fillFlag == OF_OUTLINE) startSmoothing();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &circlePoints[0].x);
	glDrawArrays((fillFlag == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_STRIP, 0, circlePoints.size());

	// use smoothness, if requested:
	if (bSmoothHinted && fillFlag == OF_OUTLINE) endSmoothing();

}

//----------------------------------------------------------
void ofGLRenderer::drawString(string textString, float x, float y, float z, ofDrawBitmapMode mode){


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

			ofMatrix4x4 modelview, projection;
			glGetFloatv(GL_MODELVIEW_MATRIX, modelview.getPtr());
			glGetFloatv(GL_PROJECTION_MATRIX, projection.getPtr());

			ofVec3f dScreen = ofVec3f(x,y,z) * modelview * projection * matrixStack.getOrientationMatrixInverse();
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
	// remember the current blend mode so that we can restore it at the end of this method.
	GLint blend_src, blend_dst;
	glGetIntegerv( GL_BLEND_SRC, &blend_src );
	glGetIntegerv( GL_BLEND_DST, &blend_dst );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef TARGET_OPENGLES
	// this temporarily enables alpha testing,
	// which discards pixels unless their alpha is 1.0f
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
#endif

	ofMesh charMesh = ofBitmapStringGetMesh(textString,0,0,mode);
	ofBitmapStringGetTextureRef().bind();
	draw(charMesh,OF_MESH_FILL,false,true,false);
	ofBitmapStringGetTextureRef().unbind();

#ifndef TARGET_OPENGLES
	glPopAttrib();
#endif
	// restore blendmode
	glBlendFunc(blend_src, blend_dst);

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

}

//----------------------------------------------------------
void ofGLRenderer::enableTextureTarget(int textureTarget, int textureID, int textureLocation){
	glActiveTexture(GL_TEXTURE0+textureLocation);
	glClientActiveTexture(GL_TEXTURE0+textureLocation);
	glEnable(textureTarget);
	glBindTexture( textureTarget, (GLuint)textureID);
	textureLocationsEnabled.insert(textureLocation);
}

//----------------------------------------------------------
void ofGLRenderer::disableTextureTarget(int textureTarget, int textureLocation){
	glActiveTexture(GL_TEXTURE0+textureLocation);
	//glClientActiveTexture(GL_TEXTURE0+textureLocation);
	glBindTexture( textureTarget, 0);
	glDisable(textureTarget);
	glActiveTexture(GL_TEXTURE0);
	//glClientActiveTexture(GL_TEXTURE0);
	textureLocationsEnabled.erase(textureLocation);
}

void ofGLRenderer::setAlphaMaskTex(ofTexture & tex){
	enableTextureTarget(tex.getTextureData().textureTarget, tex.getTextureData().textureID, 1);
	alphaMaskTextureTarget = tex.getTextureData().textureTarget;
}

void ofGLRenderer::disableAlphaMask(){
	disableTextureTarget(alphaMaskTextureTarget,1);
}

//----------------------------------------------------------
void ofGLRenderer::enableLighting(){
	glEnable(GL_LIGHTING);
#ifndef TARGET_OPENGLES  //TODO: fix this
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
#endif
	glEnable(GL_COLOR_MATERIAL);

	// FIXME: we do this so the 3d ofDraw* functions work with lighting
	// but if someone enables it between ofEnableLighting it'll be disabled
	// on ofDisableLighting. by now it seems the best option to not loose
	// performance when drawing lots of primitives
	normalsEnabled = glIsEnabled( GL_NORMALIZE );
	glEnable(GL_NORMALIZE);
	lightingEnabled = true;
}

//----------------------------------------------------------
void ofGLRenderer::disableLighting(){
	glDisable(GL_LIGHTING);
	if(!normalsEnabled){
		glDisable(GL_NORMALIZE);
	}
	lightingEnabled = false;
}

//----------------------------------------------------------
void ofGLRenderer::enableSeparateSpecularLight(){
#ifndef TARGET_OPENGLES
	glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
#endif
}

//----------------------------------------------------------
void ofGLRenderer::disableSeparateSpecularLight(){
#ifndef TARGET_OPENGLES
	glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL,GL_SINGLE_COLOR);
#endif
}

//----------------------------------------------------------
bool ofGLRenderer::getLightingEnabled(){
	return glIsEnabled(GL_LIGHTING);
}

//----------------------------------------------------------
void ofGLRenderer::setSmoothLighting(bool b){
	if (b) glShadeModel(GL_SMOOTH);
	else glShadeModel(GL_FLAT);
}

//----------------------------------------------------------
void ofGLRenderer::setGlobalAmbientColor(const ofColor& c){
	GLfloat cc[] = {c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, cc);
}

//----------------------------------------------------------
void ofGLRenderer::enableLight(int lightIndex){
	enableLighting();
	glEnable(GL_LIGHT0 + lightIndex);
}

//----------------------------------------------------------
void ofGLRenderer::disableLight(int lightIndex){
	if(lightIndex!=-1) {
		glDisable(GL_LIGHT0 + lightIndex);
	}
}

//----------------------------------------------------------
void ofGLRenderer::setLightSpotlightCutOff(int lightIndex, float spotCutOff){
	glLightf(GL_LIGHT0 + lightIndex, GL_SPOT_CUTOFF, spotCutOff );
}

//----------------------------------------------------------
void ofGLRenderer::setLightSpotConcentration(int lightIndex, float exponent){
	glLightf(GL_LIGHT0 + lightIndex, GL_SPOT_EXPONENT, exponent);
}

//----------------------------------------------------------
void ofGLRenderer::setLightAttenuation(int lightIndex, float constant, float linear, float quadratic ){
    if(lightIndex==-1) return;
	glLightf(GL_LIGHT0 + lightIndex, GL_CONSTANT_ATTENUATION, constant);
	glLightf(GL_LIGHT0 + lightIndex, GL_LINEAR_ATTENUATION, linear);
	glLightf(GL_LIGHT0 + lightIndex, GL_QUADRATIC_ATTENUATION, quadratic);
}

//----------------------------------------------------------
void ofGLRenderer::setLightAmbientColor(int lightIndex, const ofFloatColor& c){
    if(lightIndex==-1) return;
	glLightfv(GL_LIGHT0 + lightIndex, GL_AMBIENT, &c.r);
}

//----------------------------------------------------------
void ofGLRenderer::setLightDiffuseColor(int lightIndex, const ofFloatColor& c){
    if(lightIndex==-1) return;
	glLightfv(GL_LIGHT0 + lightIndex, GL_DIFFUSE, &c.r);
}

//----------------------------------------------------------
void ofGLRenderer::setLightSpecularColor(int lightIndex, const ofFloatColor& c){
    if(lightIndex==-1) return;
	glLightfv(GL_LIGHT0 + lightIndex, GL_SPECULAR, &c.r);
}

//----------------------------------------------------------
void ofGLRenderer::setLightPosition(int lightIndex, const ofVec4f & position){
	if(lightIndex==-1) return;
	int matrixMode;
	glGetIntegerv(GL_MATRIX_MODE,&matrixMode);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(matrixStack.getViewMatrix().getPtr());
	glLightfv(GL_LIGHT0 + lightIndex, GL_POSITION, &position.x);
	glPopMatrix();
	glMatrixMode(matrixMode);
}

//----------------------------------------------------------
void ofGLRenderer::setLightSpotDirection(int lightIndex, const ofVec4f & direction){
	if(lightIndex==-1) return;
	glLightfv(GL_LIGHT0 + lightIndex, GL_SPOT_DIRECTION, &direction.x);
}
