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

string defaultVertexShader =
		"attribute vec4 position;\
		attribute vec4 color;\
		attribute vec4 normal;\
		attribute vec2 texcoord;\
		\
		uniform mat4 modelViewMatrix;\
		uniform mat4 projectionMatrix;\
		\
		varying vec4 colorVarying;\
		varying vec2 texCoordVarying;\
        \
		void main(){\
			gl_Position = projectionMatrix * modelViewMatrix * position;\
			colorVarying = color;\
			texCoordVarying = texcoord;\
		}";


string defaultFragmentShader =
		"#ifdef GL_ES\n\
		// define default precision for float, vec, mat.\n\
		precision highp float;\n\
		#endif\n\
		\
		uniform sampler2D src_tex_unit0;\
		uniform float useTexture;\
		uniform float useColors;\
		uniform vec4 color;\
		\
		varying float depth;\
		varying vec4 colorVarying;\
		varying vec2 texCoordVarying;\
		\
		void main(){\
			vec4 c;\
			if(useColors>0.5){\
				c = colorVarying;\
			}else{\n\
				c = color;\
			}\n\
			if(useTexture>0.5){\
				gl_FragColor = texture2D(src_tex_unit0, texCoordVarying)*c;\
			}else{\n\
				gl_FragColor = c;\
			}\
		}";


//----------------------------------------------------------
ofGLES2Renderer::ofGLES2Renderer(string vertexShader, string fragmentShader, bool useShapeColor){
	bBackgroundAuto = true;

	linePoints.resize(2);
	triPoints.resize(3);
	rectPoints.resize(4);

	currentFbo = NULL;

    rectMode = OF_RECTMODE_CORNER;
    bFilled = OF_FILLED;
    coordHandedness = OF_LEFT_HANDED;
    bSmoothHinted = false;
    currentMatrixMode = OF_MATRIX_MODELVIEW;

    vertexFile = vertexShader;
    fragmentFile = fragmentShader;

    verticesEnabled = true;
    colorsEnabled = false;
    texCoordsEnabled = false;
    normalsEnabled = false;
}

ofGLES2Renderer::~ofGLES2Renderer() {
}

//----------------------------------------------------------
void ofGLES2Renderer::startRender() {
	defaultShader.begin();
}

//----------------------------------------------------------
void ofGLES2Renderer::finishRender() {
	defaultShader.end();
	modelViewStack.empty();
	projectionStack.empty();
}

//----------------------------------------------------------
bool ofGLES2Renderer::setup() {
	bool ret;
	if(vertexFile!=""){
		ofLogNotice() << "GLES2 loading vertex shader from " + vertexFile;
		ret = defaultShader.setupShaderFromFile(GL_VERTEX_SHADER,vertexFile);
	}else{
		ofLogNotice() << "GLES2 loading vertex shader from default source";
		ret = defaultShader.setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
	}
	if(ret){
		if(fragmentFile!=""){
			ofLogNotice() << "GLES2 loading fragment shader from " + fragmentFile;
			ret = defaultShader.setupShaderFromFile(GL_FRAGMENT_SHADER,fragmentFile);
		}else{
			ofLogNotice() << "GLES2 loading fragment shader from default source";
			ret = defaultShader.setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShader);
		}
	}
	if(ret){
		ret = defaultShader.linkProgram();
	}
	currentShader = defaultShader;
	return ret;
}

//----------------------------------------------------------
void ofGLES2Renderer::update(){
    //
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofMesh & vertexData, bool useColors, bool useTextures, bool useNormals){
	if(vertexData.getNumVertices()){
		currentShader.setAttribute3fv("position",&vertexData.getVerticesPointer()->x,sizeof(ofVec3f));
	}
	if(vertexData.getNumNormals() && useNormals){
		currentShader.setAttribute3fv("normal",&vertexData.getNormalsPointer()->x,sizeof(ofVec3f));
	}
	if(vertexData.getNumColors() && useColors){
		currentShader.setAttribute4fv("color",&vertexData.getColorsPointer()->r,sizeof(ofFloatColor));
		enableColors();
	}else{
		disableColors();
	}
	if(vertexData.getNumTexCoords() && useTextures){
		currentShader.setAttribute2fv("texcoord",&vertexData.getTexCoordsPointer()->x,sizeof(ofVec2f));
		enableTexCoords();
	}else{
		disableTexCoords();
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
	if(vertexData.getNumColors()){
		disableColors();
	}
	if(vertexData.getNumNormals()){
		disableNormals();
	}
	if(vertexData.getNumTexCoords()){
		disableTexCoords();
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals){
	if (bSmoothHinted) startSmoothing();
	if(vertexData.getNumVertices()){
		currentShader.setAttribute3fv("position",&vertexData.getVerticesPointer()->x,sizeof(ofVec3f));
	}
	if(vertexData.getNumNormals() && useNormals){
		currentShader.setAttribute3fv("normal",&vertexData.getNormalsPointer()->x,sizeof(ofVec3f));
	}
	if(vertexData.getNumColors() && useColors){
		currentShader.setAttribute4fv("color",&vertexData.getColorsPointer()->r,sizeof(ofFloatColor));
		enableColors();
	}else{
		disableColors();
	}
	if(vertexData.getNumTexCoords() && useTextures){
		currentShader.setAttribute2fv("texcoord",&vertexData.getTexCoordsPointer()->x,sizeof(ofVec2f));
		enableTexCoords();
	}else{
		disableTexCoords();
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
#ifdef TARGET_OPENGLES
		glDrawElements(drawMode, vertexData.getNumIndices(),GL_UNSIGNED_SHORT,vertexData.getIndexPointer());
#else
		glDrawElements(drawMode, vertexData.getNumIndices(),GL_UNSIGNED_INT,vertexData.getIndexPointer());
#endif
	}else{
		glDrawArrays(drawMode, 0, vertexData.getNumVertices());
	}
	if(vertexData.getNumColors()){
		disableColors();
	}
	if(vertexData.getNumNormals()){
		disableNormals();
	}
	if(vertexData.getNumTexCoords()){
		disableTexCoords();
	}
	if (bSmoothHinted) endSmoothing();
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(vector<ofPoint> & vertexData, ofPrimitiveMode drawMode){
	if(!vertexData.empty()) {
		if (bSmoothHinted) startSmoothing();
		currentShader.setAttribute3fv("position",&vertexData[0].x,sizeof(ofVec3f));
		disableTexCoords();
		disableColors();
		glDrawArrays(ofGetGLPrimitiveMode(drawMode), 0, vertexData.size());
		if (bSmoothHinted) endSmoothing();
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofPolyline & poly){
	if(!poly.getVertices().empty()) {
		// use smoothness, if requested:
		if (bSmoothHinted) startSmoothing();

		currentShader.setAttribute3fv("position",&poly.getVertices()[0].x,sizeof(ofVec3f));
		disableTexCoords();
		disableColors();
		glDrawArrays(poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, poly.size());

		// use smoothness, if requested:
		if (bSmoothHinted) endSmoothing();
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofPath & shape){
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
void ofGLES2Renderer::draw(ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
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
void ofGLES2Renderer::draw(ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
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
void ofGLES2Renderer::draw(ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
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
void ofGLES2Renderer::setCurrentFBO(ofFbo * fbo){
	currentFbo = fbo;
}

//----------------------------------------------------------
void ofGLES2Renderer::pushView() {
	viewportHistory.push(currentViewport);
	ofMatrixMode currentMode = currentMatrixMode;
	matrixMode(OF_MATRIX_PROJECTION);
	pushMatrix();
	matrixMode(OF_MATRIX_MODELVIEW);
	pushMatrix();
	matrixMode(currentMode);
}

//----------------------------------------------------------
void ofGLES2Renderer::popView() {
	if( viewportHistory.size() ){
		ofRectangle viewRect = viewportHistory.top();
		viewport(viewRect.x, viewRect.y, viewRect.width, viewRect.height,false);
		viewportHistory.pop();
	}
	ofMatrixMode currentMode = currentMatrixMode;
	matrixMode(OF_MATRIX_PROJECTION);
	popMatrix();
	matrixMode(OF_MATRIX_MODELVIEW);
	popMatrix();
	matrixMode(currentMode);
}

//----------------------------------------------------------
void ofGLES2Renderer::viewport(ofRectangle viewport_){
	viewport(viewport_.x, viewport_.y, viewport_.width, viewport_.height,true);
}

//----------------------------------------------------------
void ofGLES2Renderer::viewport(float x, float y, float width, float height, bool invertY) {
	if(width == 0) width = ofGetWindowWidth();
	if(height == 0) height = ofGetWindowHeight();

	if (invertY){
		if(currentFbo){
			y = currentFbo->getHeight() - (y + height);
		}else{
			y = ofGetWindowHeight() - (y + height);
		}
	}
	currentViewport.set(x, y, width, height);
	glViewport(x, y, width, height);
}

//----------------------------------------------------------
ofRectangle ofGLES2Renderer::getCurrentViewport(){
    return currentViewport;
}

//----------------------------------------------------------
int ofGLES2Renderer::getViewportWidth(){
	return currentViewport.width;
}

//----------------------------------------------------------
int ofGLES2Renderer::getViewportHeight(){
	return currentViewport.height;
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
void ofGLES2Renderer::setOrientationMatrix(float width, float height, ofOrientation orientation, bool vFlip){
	orientationMatrix.makeIdentityMatrix();

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
}

//----------------------------------------------------------
void ofGLES2Renderer::setupScreenPerspective(float width, float height, ofOrientation orientation, bool vFlip, float fov, float nearDist, float farDist) {

	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();

	float viewW = getViewportWidth();
	float viewH = getViewportHeight();

	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) viewW / viewH;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;

	projection.makePerspectiveMatrix(fov, aspect, nearDist, farDist);
	modelView.makeLookAtViewMatrix(ofVec3f(eyeX, eyeY, dist),  ofVec3f(eyeX, eyeY, 0),  ofVec3f(0, 1, 0));
	setOrientationMatrix(width,height,orientation,vFlip);

	modelViewOrientation = modelView;
	modelViewOrientation.preMult(orientationMatrix);
	uploadModelViewMatrix(modelViewOrientation);
	uploadProjectionMatrix(projection);
	matrixMode(OF_MATRIX_MODELVIEW);
}

//----------------------------------------------------------
void ofGLES2Renderer::setupScreenOrtho(float width, float height, ofOrientation orientation, bool vFlip, float nearDist, float farDist) {
	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();

	float viewW = ofGetViewportWidth();
	float viewH = ofGetViewportHeight();


	ofSetCoordHandedness(OF_RIGHT_HANDED);
	if(vFlip) {
		projection = ofMatrix4x4::newOrthoMatrix(0, width, height, 0, nearDist, farDist);
		ofSetCoordHandedness(OF_LEFT_HANDED);
	}else{
		projection = ofMatrix4x4::newOrthoMatrix(0, viewW, 0, viewH, nearDist, farDist);
	}

	setOrientationMatrix(width,height,orientation,vFlip);

	uploadProjectionMatrix(projection);

	matrixMode(OF_MATRIX_MODELVIEW);
	loadIdentityMatrix();
}

//----------------------------------------------------------
//Resets openGL parameters back to OF defaults
void ofGLES2Renderer::setupGraphicDefaults(){
	/*glEnableVertexAttribArray(getAttrLocationPosition());
	glDisableVertexAttribArray(getAttrLocationColor());
	glDisableVertexAttribArray(getAttrLocationNormal());
	glDisableVertexAttribArray(getAttrLocationTexCoord());*/
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
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		modelViewStack.push(modelViewOrientation);
		break;
	case OF_MATRIX_PROJECTION:
		projectionStack.push(projection);
		break;
	case OF_MATRIX_TEXTURE:
		textureStack.push(textureMatrix);
		break;
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::popMatrix(){
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		modelViewOrientation = modelViewStack.top();
		uploadModelViewMatrix(modelViewOrientation);
		modelViewStack.pop();
		break;
	case OF_MATRIX_PROJECTION:
		projection = projectionStack.top();
		uploadProjectionMatrix(projection);
		projectionStack.pop();
		break;
	case OF_MATRIX_TEXTURE:
		textureMatrix = textureStack.top();
		uploadTextureMatrix(textureMatrix);
		textureStack.pop();
		break;
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::translate(const ofPoint& p){
	translate(p.x, p.y, p.z);
}

//----------------------------------------------------------
void ofGLES2Renderer::translate(float x, float y, float z){
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		modelViewOrientation.glTranslate(x,y,z);
		uploadModelViewMatrix(modelViewOrientation);
		break;
	case OF_MATRIX_PROJECTION:
		projection.glTranslate(x,y,z);
		uploadProjectionMatrix(projection);
		break;
	case OF_MATRIX_TEXTURE:
		textureMatrix.glTranslate(x,y,z);
		uploadTextureMatrix(textureMatrix);
		break;
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::scale(float xAmnt, float yAmnt, float zAmnt){
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		modelViewOrientation.glScale(xAmnt,yAmnt,zAmnt);
		uploadModelViewMatrix(modelViewOrientation);
		break;
	case OF_MATRIX_PROJECTION:
		projection.glScale(xAmnt,yAmnt,zAmnt);
		uploadProjectionMatrix(projection);
		break;
	case OF_MATRIX_TEXTURE:
		textureMatrix.glScale(xAmnt,yAmnt,zAmnt);
		uploadTextureMatrix(textureMatrix);
		break;
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::rotate(float degrees, float vecX, float vecY, float vecZ){
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		modelViewOrientation.glRotate(degrees,vecX,vecY,vecZ);
		uploadModelViewMatrix(modelViewOrientation);
		break;
	case OF_MATRIX_PROJECTION:
		projection.glRotate(degrees,vecX,vecY,vecZ);
		uploadProjectionMatrix(projection);
		break;
	case OF_MATRIX_TEXTURE:
		textureMatrix.glRotate(degrees,vecX,vecY,vecZ);
		uploadTextureMatrix(textureMatrix);
		break;
	}
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
void ofGLES2Renderer::matrixMode(ofMatrixMode mode){
	currentMatrixMode = mode;
}

//----------------------------------------------------------
void ofGLES2Renderer::loadIdentityMatrix (void){
	ofMatrix4x4 m;
	m.makeIdentityMatrix();
	loadMatrix(m);
}

//----------------------------------------------------------
void ofGLES2Renderer::loadMatrix (const ofMatrix4x4 & m){
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		modelView = m;
		modelViewOrientation = modelView;
		modelViewOrientation.preMult(orientationMatrix);
		uploadModelViewMatrix(modelViewOrientation);
		break;
	case OF_MATRIX_PROJECTION:
		projection = m;
		uploadProjectionMatrix(projection);
		break;
	case OF_MATRIX_TEXTURE:
		textureMatrix = m;
		uploadTextureMatrix(textureMatrix);
		break;
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::loadMatrix (const float *m){
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		modelView.set(m);
		modelViewOrientation = modelView;
		modelViewOrientation.preMult(orientationMatrix);
		uploadModelViewMatrix(modelViewOrientation);
		break;
	case OF_MATRIX_PROJECTION:
		projection.set(m);
		uploadProjectionMatrix(projection);
		break;
	case OF_MATRIX_TEXTURE:
		textureMatrix.set(m);
		uploadTextureMatrix(textureMatrix);
		break;
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::multMatrix (const ofMatrix4x4 & m){
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		modelViewOrientation.preMult(m);
		uploadModelViewMatrix(modelViewOrientation);
		break;
	case OF_MATRIX_PROJECTION:
		projection.preMult(m);
		uploadProjectionMatrix(projection);
		break;
	case OF_MATRIX_TEXTURE:
		textureMatrix.preMult(m);
		uploadTextureMatrix(textureMatrix);
		break;
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::multMatrix (const float *m){
	ofMatrix4x4 mat(m);
	multMatrix(mat);
}

//----------------------------------------------------------
void ofGLES2Renderer::uploadModelViewMatrix(const ofMatrix4x4 & m){
	currentShader.setUniformMatrix4f("modelViewMatrix",m);
}

//----------------------------------------------------------
void ofGLES2Renderer::uploadProjectionMatrix(const ofMatrix4x4 & m){
	currentShader.setUniformMatrix4f("projectionMatrix",m);
}

void ofGLES2Renderer::uploadTextureMatrix(const ofMatrix4x4 & m){
	currentShader.setUniformMatrix4f("textureMatrix",m);
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
	currentShader.setUniform4f("color",currentColor.r,currentColor.g,currentColor.b,currentColor.a);
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
	switch (blendMode){

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
			ofLog(OF_LOG_WARNING, "OF_BLENDMODE_SUBTRACT not currently supported on OpenGL/ES");
		#endif
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		}


		default:
			break;
	}
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
GLint ofGLES2Renderer::getAttrLocationPosition(){
	return currentShader.getAttributeLocation("position");
}

//----------------------------------------------------------
GLint ofGLES2Renderer::getAttrLocationColor(){
	return currentShader.getAttributeLocation("color");
}

//----------------------------------------------------------
GLint ofGLES2Renderer::getAttrLocationNormal(){
	return currentShader.getAttributeLocation("normal");
}

//----------------------------------------------------------
GLint ofGLES2Renderer::getAttrLocationTexCoord(){
	return currentShader.getAttributeLocation("texcoord");
}

//----------------------------------------------------------
ofShader & ofGLES2Renderer::getCurrentShader(){
	return currentShader;
}

//----------------------------------------------------------
void ofGLES2Renderer::setDefaultShader(ofShader & shader){
	defaultShader = shader;
}

//----------------------------------------------------------
void ofGLES2Renderer::enableTexCoords(){
	currentShader.setUniform1f("useTexture",1);
	texCoordsEnabled = true;
}

//----------------------------------------------------------
void ofGLES2Renderer::enableColors(){
	currentShader.setUniform1f("useColors",1);
	colorsEnabled = true;
}

//----------------------------------------------------------
void ofGLES2Renderer::enableNormals(){
	normalsEnabled = true;
}

//----------------------------------------------------------
void ofGLES2Renderer::disableTexCoords(){
	currentShader.setUniform1f("useTexture",0);
	texCoordsEnabled = false;
}

//----------------------------------------------------------
void ofGLES2Renderer::disableColors(){
	currentShader.setUniform1f("useColors",0);
	colorsEnabled = false;
}

//----------------------------------------------------------
void ofGLES2Renderer::disableNormals(){
	normalsEnabled = false;
}

//----------------------------------------------------------
void ofGLES2Renderer::beginCustomShader(ofShader & shader){
	shader.setUniform1f("useTexture",texCoordsEnabled);
	shader.setUniform1f("useColors",colorsEnabled);
	shader.setUniform4f("color",currentColor.r,currentColor.g,currentColor.b,currentColor.a);
	shader.setUniformMatrix4f("modelViewMatrix",modelViewOrientation);
	shader.setUniformMatrix4f("projectionMatrix",projection);
	shader.setUniformMatrix4f("textureMatrix",textureMatrix);
	currentShader = shader;
}

void ofGLES2Renderer::endCustomShader(){
	beginCustomShader(defaultShader);
}

//----------------------------------------------------------
void ofGLES2Renderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2){
	linePoints[0].set(x1,y1,z1);
	linePoints[1].set(x2,y2,z2);
    
	// use smoothness, if requested:
	if (bSmoothHinted) startSmoothing();

	currentShader.setAttribute3fv("position",&linePoints[0].x,sizeof(ofVec3f));
	disableTexCoords();
	disableColors();
    
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
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	currentShader.setAttribute3fv("position",&rectPoints[0].x,sizeof(ofVec3f));
	disableTexCoords();
	disableColors();

	glDrawArrays((bFilled == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, 4);
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofGLES2Renderer::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	triPoints[0].set(x1,y1,z1);
	triPoints[1].set(x2,y2,z2);
	triPoints[2].set(x3,y3,z3);
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	currentShader.setAttribute3fv("position",&triPoints[0].x,sizeof(ofVec3f));
	disableTexCoords();
	disableColors();
    
	glDrawArrays((bFilled == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, 3);
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofGLES2Renderer::drawCircle(float x, float y, float z,  float radius){
	vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		circlePoints[i].set(radius*circleCache[i].x+x,radius*circleCache[i].y+y,z);
	}
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	currentShader.setAttribute3fv("position",&circlePoints[0].x,sizeof(ofVec3f));
	disableTexCoords();
	disableColors();
    
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
	}
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	currentShader.setAttribute3fv("position",&circlePoints[0].x,sizeof(ofVec3f));
	disableTexCoords();
	disableColors();
    
	glDrawArrays((bFilled == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_STRIP, 0, circlePoints.size());
    
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();
}

//----------------------------------------------------------
void ofGLES2Renderer::drawString(string textString, float x, float y, float z, ofDrawBitmapMode mode){
	// TODO :: needs ES2 code.
}
