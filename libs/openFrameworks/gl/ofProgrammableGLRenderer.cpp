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

#ifdef TARGET_OPENGLES
string defaultVertexShader =
		"attribute vec4 position;\
		attribute vec4 color;\
		attribute vec4 normal;\
		attribute vec2 texcoord;\
		\
		uniform mat4 modelViewMatrix;\
		uniform mat4 projectionMatrix;\
		uniform mat4 textureMatrix;\
		uniform mat4 modelViewProjectionMatrix;\
		\
		varying vec4 colorVarying;\
		varying vec2 texCoordVarying;\
        \
		void main(){\
			gl_Position = modelViewProjectionMatrix * position;\
			colorVarying = color;\
			texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;\
		}";


/*string defaultFragmentShader =
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
}";*/

string defaultFragmentShaderTexColor =
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
		    gl_FragColor = texture2D(src_tex_unit0, texCoordVarying)*colorVarying;\
        }";

string defaultFragmentShaderTexNoColor =
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
		    gl_FragColor = texture2D(src_tex_unit0, texCoordVarying)*color;\
        }";

string defaultFragmentShaderNoTexColor =
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
		    gl_FragColor = colorVarying;\
        }";

string defaultFragmentShaderNoTexNoColor =
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
		    gl_FragColor = color;\
        }";

// tig: todo: implement shaders for bitmapstringdraw on GLES2...
// GLSL_ES shader written against spec:
// http://www.khronos.org/registry/gles/specs/2.0/GLSL_ES_Specification_1.0.17.pdf

string bitmapStringVertexShader		= "\n\
#ifdef GL_ES\n\
// define default precision for float, vec, mat.\n\
precision highp float;\n\
#endif\n\
\n\
uniform mat4 projectionMatrix;\n\
uniform mat4 modelViewMatrix;\n\
uniform mat4 textureMatrix;\
uniform mat4 modelViewProjectionMatrix;\n\
\n\
attribute vec4  position;\n\
attribute vec2  texcoord;\n\
\n\
varying vec2 texCoordVarying;\n\
\n\
void main()\n\
{\n\
	texCoordVarying = textureMatrix*texcoord;\n\
	gl_Position = modelViewProjectionMatrix * position;\n\
}";
string bitmapStringFragmentShader	= "\n\
#ifdef GL_ES\n\
// define default precision for float, vec, mat.\n\
precision highp float;\n\
#endif\n\
\n\
uniform sampler2D src_tex_unit0;\n\
uniform vec4 color;\n\
\n\
varying vec2 texCoordVarying;\n\
\n\
void main()\n\
{\n\
	\n\
	vec4 tex = texture2D(src_tex_unit0, texCoordVarying);\n\
	// We will not write anything to the framebuffer if we have a transparent pixel\n\
	// This makes sure we don't mess up our depth buffer.\n\
	if (tex.a < 0.5) discard;\n\
	gl_FragColor = color * tex;\n\
}";

#else

// tig: GLSL #150 shaders written against spec:
// http://www.opengl.org/registry/doc/GLSLangSpec.1.50.09.pdf

string defaultVertexShader =
"#version 150\n\
\n\
uniform mat4 projectionMatrix;\n\
uniform mat4 modelViewMatrix;\n\
uniform mat4 textureMatrix;\
uniform mat4 modelViewProjectionMatrix;\n\
\n\
\n\
in vec4  position;\n\
in vec2  texcoord;\n\
in vec4  color;\n\
in vec3  normal;\n\
\n\
out vec4 colorVarying;\n\
out vec2 texCoordVarying;\n\
out vec4 normalVarying;\n\
\n\
void main()\n\
{\n\
	colorVarying = color;\n\
	texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;\
	gl_Position = modelViewProjectionMatrix * position;\n\
}";


/*string defaultFragmentShader =
"#version 150\n\
\n\
uniform sampler2DRect src_tex_unit0;\n\
uniform float useTexture = 0.0;\n\
uniform float useColors = 0.0;\n\
uniform vec4 color = vec4(1.0);\n\
\n\
in float depth;\n\
in vec4 colorVarying;\n\
in vec2 texCoordVarying;\n\
out vec4 fragColor;\n\
\n\
void main(){\n\
	vec4 c = vec4(1.0);\n\
	if(useColors>0.5){\n\
		c = vec4(1.0,0.0,0.0,1.0);\n\
	}else{\n\
		c = color;\n\
	}\n\
\n\
	if(useTexture>0.5){\n\
		fragColor = texture(src_tex_unit0, texCoordVarying) * c;\n\
	} else {\n\
		fragColor = c;\n\
	}\n\
}";*/

string defaultFragmentShaderTexColor ="\n\
#version 150\n\
\n\
uniform sampler2DRect src_tex_unit0;\n\
uniform float useTexture = 0.0;\n\
uniform float useColors = 0.0;\n\
uniform vec4 color = vec4(1.0);\n\
\n\
in float depth;\n\
in vec4 colorVarying;\n\
in vec2 texCoordVarying;\n\
out vec4 fragColor;\n\
\n\
void main(){\n\
	fragColor = texture(src_tex_unit0, texCoordVarying) * colorVarying;\n\
}";

string defaultFragmentShaderTexNoColor ="\n\
#version 150\n\
\n\
uniform sampler2DRect src_tex_unit0;\n\
uniform float useTexture = 0.0;\n\
uniform float useColors = 0.0;\n\
uniform vec4 color = vec4(1.0);\n\
\n\
in float depth;\n\
in vec4 colorVarying;\n\
in vec2 texCoordVarying;\n\
out vec4 fragColor;\n\
\n\
void main(){\n\
	fragColor = texture(src_tex_unit0, texCoordVarying) * color;\n\
}";

string defaultFragmentShaderTex2DColor ="\n\
#version 150\n\
\n\
uniform sampler2D src_tex_unit0;\n\
uniform float useTexture = 0.0;\n\
uniform float useColors = 0.0;\n\
uniform vec4 color = vec4(1.0);\n\
\n\
in float depth;\n\
in vec4 colorVarying;\n\
in vec2 texCoordVarying;\n\
out vec4 fragColor;\n\
\n\
void main(){\n\
	fragColor = texture(src_tex_unit0, texCoordVarying) * colorVarying;\n\
}";

string defaultFragmentShaderTex2DNoColor ="\n\
#version 150\n\
\n\
uniform sampler2D src_tex_unit0;\n\
uniform float useTexture = 0.0;\n\
uniform float useColors = 0.0;\n\
uniform vec4 color = vec4(1.0);\n\
\n\
in float depth;\n\
in vec4 colorVarying;\n\
in vec2 texCoordVarying;\n\
out vec4 fragColor;\n\
\n\
void main(){\n\
	fragColor = texture(src_tex_unit0, texCoordVarying) * color;\n\
}";

string defaultFragmentShaderNoTexColor ="\n\
#version 150\n\
\n\
uniform sampler2DRect src_tex_unit0;\n\
uniform float useTexture = 0.0;\n\
uniform float useColors = 0.0;\n\
uniform vec4 color = vec4(1.0);\n\
\n\
in float depth;\n\
in vec4 colorVarying;\n\
in vec2 texCoordVarying;\n\
out vec4 fragColor;\n\
\n\
void main(){\n\
	fragColor = colorVarying;\n\
}";

string defaultFragmentShaderNoTexNoColor ="\n\
#version 150\n\
\n\
uniform sampler2DRect src_tex_unit0;\n\
uniform float useTexture = 0.0;\n\
uniform float useColors = 0.0;\n\
uniform vec4 color = vec4(1.0);\n\
\n\
in float depth;\n\
in vec4 colorVarying;\n\
in vec2 texCoordVarying;\n\
out vec4 fragColor;\n\
\n\
void main(){\n\
	fragColor = color;\n\
}";

string bitmapStringVertexShader ="\n\
#version 150\n\
\n\
uniform mat4 projectionMatrix;\n\
uniform mat4 modelViewMatrix;\n\
uniform mat4 textureMatrix;\
uniform mat4 modelViewProjectionMatrix;\n\
\n\
in vec4  position;\n\
in vec4  color;\n\
in vec2  texcoord;\n\
\n\
out vec2 texCoordVarying;\n\
\n\
void main()\n\
{\n\
	texCoordVarying = texcoord;\n\
	gl_Position = modelViewProjectionMatrix * position;\n\
}";

string bitmapStringFragmentShader	= "\n\
#version 150\n\
\n\
uniform sampler2D src_tex_unit0;\n\
uniform vec4 color = vec4(1.0);\n\
in vec2 texCoordVarying;\n\
out vec4 fragColor;\n\
\n\
void main()\n\
{\n\
	\n\
	vec4 tex = texture(src_tex_unit0, texCoordVarying);\n\
	// We will not write anything to the framebuffer if we have a transparent pixel\n\
	// This makes sure we don't mess up our depth buffer.\n\
	if (tex.a < 0.5) discard;\n\
	fragColor = color * tex;\n\
}";
#endif


//----------------------------------------------------------
ofProgrammableGLRenderer::ofProgrammableGLRenderer(string vertexShader, string fragmentShader, bool useShapeColor){
	currentMatrix = &modelViewMatrix;
	bBackgroundAuto = true;

	lineVbo.getVertices().resize(2);
	lineVbo.setMode(OF_PRIMITIVE_LINES);
	triangleVbo.getVertices().resize(3);
	rectVbo.getVertices().resize(4);

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
	externalShaderProvided = false;

	orientationMatrix.makeIdentityMatrix();


	glGetError();
#ifndef TARGET_OPENGLES
	glGenVertexArrays(1, &defaultVAO);
	currentTextureTarget = OF_NO_TEXTURE;
#else
	currentTextureTarget = OF_NO_TEXTURE;
	//glGenVertexArraysOES(1, &defaultVAO);
#endif
	glGetError();
}

ofProgrammableGLRenderer::~ofProgrammableGLRenderer() {

#ifndef TARGET_OPENGLES
	glDeleteVertexArrays(1, &defaultVAO);
#else
	//glDeleteVertexArraysOES(1, &defaultVAO);
#endif

	ofLogNotice() << "Destroyed ofProgrammableGLRenderer";
	
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::startRender() {
	// bind vertex array
#ifndef TARGET_OPENGLES
	glBindVertexArray(defaultVAO);
#else
	//glBindVertexArrayOES(defaultVAO);
#endif
	currentShader.begin();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::finishRender() {
	glUseProgram(0);

	// bind vertex array
#ifndef TARGET_OPENGLES
	glBindVertexArray(0);
#else
	//glBindVertexArrayOES(0);
#endif
	
	int tmpCounter = 0;
	while (!modelViewMatrixStack.empty()){
		modelViewMatrixStack.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ) ofLogWarning() << "Found " << tmpCounter << " unmatched matrices on modelView matrix stack. Check if modelview matrix push() and pop() pair up properly.";
	
	tmpCounter = 0;
	while (!projectionMatrixStack.empty()){
		projectionMatrixStack.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ) ofLogWarning() << "Found " << tmpCounter << " unmatched matrices on projection matrix stack. Check if projection matrix push() and pop() pair up properly.";
}

//----------------------------------------------------------
bool ofProgrammableGLRenderer::setup() {
	bool ret=false;
	externalShaderProvided=false;
	if(vertexFile!="" && fragmentFile==""){
		ofLogError() << "can't use custom vertex shader without fragment, falling back to default";
		vertexFile = "";
	}
	if(vertexFile!=""){
		ofLogNotice("ofProgrammableGLRenderer") << "loading vertex shader from " + vertexFile;
		ret = externalShader.setupShaderFromFile(GL_VERTEX_SHADER,vertexFile);
		externalShaderProvided=true;
	}else{
		ofLogNotice("ofProgrammableGLRenderer") << "loading vertex shader from default source";
		if(fragmentFile!=""){
			externalShader.setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
		}else{
			ret = defaultShaderTexColor.setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
			ret &= defaultShaderTex2DColor.setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
			ret &= defaultShaderNoTexColor.setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
			ret &= defaultShaderTexNoColor.setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
			ret &= defaultShaderTex2DNoColor.setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
			ret &= defaultShaderNoTexNoColor.setupShaderFromSource(GL_VERTEX_SHADER,defaultVertexShader);
		}

	}
	if(ret){
		if(fragmentFile!=""){
			ofLogNotice("ofProgrammableGLRenderer") << "loading fragment shader from " + fragmentFile;
			ret = externalShader.setupShaderFromFile(GL_FRAGMENT_SHADER,fragmentFile);
			externalShaderProvided=true;
		}else{
			ofLogNotice("ofProgrammableGLRenderer") << "loading fragment shader from default source";
			ret = defaultShaderTexColor.setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderTexColor);
			ret &= defaultShaderTex2DColor.setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderTex2DColor);
			ret &= defaultShaderNoTexColor.setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderNoTexColor);
			ret &= defaultShaderTexNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderTexNoColor);
			ret &= defaultShaderTex2DNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderTex2DNoColor);
			ret &= defaultShaderNoTexNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,defaultFragmentShaderNoTexNoColor);
		}
	}
	if(ret){
		if(externalShaderProvided){
			ret = externalShader.linkProgram();
			ofLogVerbose() << "Loaded provided shader";
		}else{
			ret = defaultShaderTexColor.linkProgram();
			ret = defaultShaderTex2DColor.linkProgram();
			ret &= defaultShaderNoTexColor.linkProgram();
			ret &= defaultShaderTexNoColor.linkProgram();
			ret &= defaultShaderTex2DNoColor.linkProgram();
			ret &= defaultShaderNoTexNoColor.linkProgram();
			ofLogVerbose() << "Loaded default shader";
		}
	}
	
	bool bmpShdRet = false;
	
	ofLogNotice("ofProgrammableGLRenderer") << "loading bitmapString vertex shader from default source";
	bmpShdRet = bitmapStringShader.setupShaderFromSource(GL_VERTEX_SHADER, bitmapStringVertexShader);
	if (bmpShdRet) {
		ofLogNotice("ofProgrammableGLRenderer") << "loading bitmapString fragment shader from default source";
		bmpShdRet = bitmapStringShader.setupShaderFromSource(GL_FRAGMENT_SHADER, bitmapStringFragmentShader);
	}
	if (bmpShdRet) {
		bmpShdRet = bitmapStringShader.linkProgram();
		ofLogVerbose() << "Loaded bitmapString shader";
	}

	beginDefaultShader();
	glGetError();
	return ret && bmpShdRet;
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

	// tig: note that there was a lot of code duplication going on here.
	// using ofVbo's draw methods, to keep stuff DRY

	if (meshVbo.getIsAllocated()) {
		meshVbo.clear();
	}

	if (vertexData.getVertices().empty()) return;
	
	meshVbo.setMesh(vertexData, GL_DYNAMIC_DRAW, useColors, useTextures, useNormals);
	
	if (!useColors)		meshVbo.disableColors();
	if (!useTextures)	meshVbo.disableTexCoords();
	if (!useNormals)	meshVbo.disableNormals();
	
	
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
void ofProgrammableGLRenderer::draw(vector<ofPoint> & vertexData, ofPrimitiveMode drawMode){
	if(!vertexData.empty()) {
		if (bSmoothHinted) startSmoothing();

		/*disableTexCoords();
		disableColors();*/

		vertexDataVbo.setVertexData(&vertexData[0], vertexData.size(), GL_DYNAMIC_DRAW);
		vertexDataVbo.draw(ofGetGLPrimitiveMode(drawMode), 0, vertexData.size());

		/*preparePrimitiveDraw(*vertexDataVbo);
		glDrawArrays(ofGetGLPrimitiveMode(drawMode), 0, vertexData.size());
		finishPrimitiveDraw();*/
		if (bSmoothHinted) endSmoothing();
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::draw(ofPolyline & poly){
	if(!poly.getVertices().empty()) {
		// use smoothness, if requested:
		if (bSmoothHinted) startSmoothing();

		/*disableTexCoords();
		disableColors();*/

		vertexDataVbo.setVertexData(&poly.getVertices()[0], poly.size(), GL_DYNAMIC_DRAW);
		vertexDataVbo.draw(poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, poly.size());

		/*preparePrimitiveDraw(vertexDataVbo);
		glDrawArrays(poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, poly.size());
		finishPrimitiveDraw();*/

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
	currentFbo = fbo;
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::pushView() {
	viewportHistory.push(currentViewport);
	ofMatrixMode currentMode = currentMatrixMode;
	matrixMode(OF_MATRIX_PROJECTION);
	pushMatrix();
	matrixMode(OF_MATRIX_MODELVIEW);
	pushMatrix();
	matrixMode(currentMode);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::popView() {
	if( viewportHistory.size() ){
		ofRectangle viewRect = viewportHistory.top();
		viewport(viewRect.x, viewRect.y, viewRect.width, viewRect.height, false);
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
void ofProgrammableGLRenderer::viewport(ofRectangle viewport_){
	viewport(viewport_.x, viewport_.y, viewport_.width, viewport_.height,true);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::viewport(float x, float y, float width, float height, bool invertY) {
	if(!currentFbo && !ofDoesHWOrientation() && (ofGetOrientation()==OF_ORIENTATION_90_LEFT || ofGetOrientation()==OF_ORIENTATION_90_RIGHT)){
		swap(width,height);
		swap(x,y);
	}
	if(width == 0 || height == 0){
		width = ofGetWindowWidth();
		height = ofGetWindowHeight();
	}

	if (!invertY){
		if(currentFbo){
			y = currentFbo->getHeight() - (y + height);
		}else{
			y = ofGetWindowHeight() - (y + height);
		}
	}

	currentViewport.set(x, y, width, height);

	glViewport(x,y,width,height);
}

//----------------------------------------------------------
ofRectangle ofProgrammableGLRenderer::getCurrentViewport(){
	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);

	currentViewport.set(viewport[0], viewport[1], viewport[2], viewport[3]);

	if(!currentFbo && !ofDoesHWOrientation() && (ofGetOrientation()==OF_ORIENTATION_90_LEFT || ofGetOrientation()==OF_ORIENTATION_90_RIGHT)){
		swap(currentViewport.width,currentViewport.height);
		swap(currentViewport.x,currentViewport.y);
	}
	return currentViewport;
}

//----------------------------------------------------------
ofRectangle ofProgrammableGLRenderer::getNativeViewport(){
	ofRectangle orientedViewport = getCurrentViewport();

	if(!ofDoesHWOrientation() && (ofGetOrientation()==OF_ORIENTATION_90_LEFT || ofGetOrientation()==OF_ORIENTATION_90_RIGHT)){
		swap(orientedViewport.width,orientedViewport.height);
		swap(orientedViewport.x,orientedViewport.y);
	}
    return orientedViewport;
}

//----------------------------------------------------------
int ofProgrammableGLRenderer::getViewportWidth(){
	return currentViewport.width;
}

//----------------------------------------------------------
int ofProgrammableGLRenderer::getViewportHeight(){
	return currentViewport.height;
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setCoordHandedness(ofHandednessType handedness) {
	coordHandedness = handedness;
}

//----------------------------------------------------------
ofHandednessType ofProgrammableGLRenderer::getCoordHandedness() {
	return coordHandedness;
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setOrientation(ofOrientation orientation, bool vFlip){
	vFlipped = vFlip;

	if(ofDoesHWOrientation()){
		orientationMatrix.makeIdentityMatrix();
		if(vFlip){
			orientationMatrix.scale(1,-1,1);
		}
	}else{
		switch(orientation) {
			case OF_ORIENTATION_180:
				orientationMatrix.makeRotationMatrix(180,0,0,1);
				if(vFlip)
					orientationMatrix.scale(1,-1,1);
				break;

			case OF_ORIENTATION_90_RIGHT:
				orientationMatrix.makeRotationMatrix(90,0,0,1);
				if(vFlip)
					orientationMatrix.scale(-1,1,1);
				break;

			case OF_ORIENTATION_90_LEFT:
				orientationMatrix.makeRotationMatrix(-90,0,0,1);
				if(vFlip)
					orientationMatrix.scale(-1,1,1);
				break;

			case OF_ORIENTATION_DEFAULT:
			default:
				orientationMatrix.makeIdentityMatrix();
				if(vFlip)
					orientationMatrix.scale(1,-1,1);
				break;
		}
	}

}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setupScreenPerspective(float width, float height, ofOrientation orientation, bool vFlip, float fov, float nearDist, float farDist) {
	ofRectangle currentViewport = getCurrentViewport();
	
	float viewW = currentViewport.width;
	float viewH = currentViewport.height;

	if( orientation == OF_ORIENTATION_UNKNOWN ) orientation = ofGetOrientation();
	setOrientation(orientation,vFlip);

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
void ofProgrammableGLRenderer::setupScreenOrtho(float width, float height, ofOrientation orientation, bool vFlip, float nearDist, float farDist) {
	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();

	ofRectangle currentViewport = getCurrentViewport();

	float viewW = currentViewport.width;
	float viewH = currentViewport.height;

	ofMatrix4x4 ortho;

	ofSetCoordHandedness(OF_RIGHT_HANDED);
	if(vFlip) {
		ortho = ofMatrix4x4::newOrthoMatrix(0, width, height, 0, nearDist, farDist);
		ofSetCoordHandedness(OF_LEFT_HANDED);
	}else{
		ortho = ofMatrix4x4::newOrthoMatrix(0, viewW, 0, viewH, nearDist, farDist);
	}

	setOrientation(orientation,vFlip);

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
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		modelViewMatrixStack.push(modelViewMatrix);
		break;
	case OF_MATRIX_PROJECTION:
		projectionMatrixStack.push(projectionMatrix);
		break;
	case OF_MATRIX_TEXTURE:
		textureMatrixStack.push(textureMatrix);
		break;
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::popMatrix(){
	if (currentMatrixMode == OF_MATRIX_MODELVIEW && !modelViewMatrixStack.empty()){
		modelViewMatrix = modelViewMatrixStack.top();
		modelViewMatrixStack.pop();
	} else if (currentMatrixMode == OF_MATRIX_PROJECTION && !projectionMatrixStack.empty()){
		projectionMatrix = projectionMatrixStack.top();
		projectionMatrixStack.pop();
	} else if (currentMatrixMode == OF_MATRIX_TEXTURE && !textureMatrixStack.empty()){
		textureMatrix = textureMatrixStack.top();
		textureMatrixStack.pop();
	} else {
		ofLogWarning() << "ofxGL3Renderer: Empty matrix stack, cannot pop any further.";
	}
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::translate(const ofVec3f& p){
	translate(p.x, p.y, p.z);
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::translate(float x, float y, float z){
	currentMatrix->glTranslate(x, y, z);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::scale(float xAmnt, float yAmnt, float zAmnt){
	currentMatrix->glScale(xAmnt, yAmnt, zAmnt);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::rotate(float degrees, float vecX, float vecY, float vecZ){
	currentMatrix->glRotate(degrees, vecX, vecY, vecZ);
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
	currentMatrixMode = mode;
	if (currentMatrixMode == OF_MATRIX_MODELVIEW){
		currentMatrix = &modelViewMatrix;
	} else if (currentMatrixMode == OF_MATRIX_PROJECTION){
		currentMatrix = &projectionMatrix;
	} else if (currentMatrixMode == OF_MATRIX_TEXTURE){
		currentMatrix = &textureMatrix;
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::loadIdentityMatrix (void){
	currentMatrix->makeIdentityMatrix();
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::loadMatrix (const ofMatrix4x4 & m){
	currentMatrix->set(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::loadMatrix (const float *m){
	currentMatrix->set(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::multMatrix (const ofMatrix4x4 & m){
	multMatrix(m.getPtr());
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::multMatrix (const float *m){
	currentMatrix->preMult(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::uploadCurrentMatrix(){

	// uploads the current matrix to the current shader.

	if (currentMatrixMode == OF_MATRIX_MODELVIEW){
		currentShader.setUniformMatrix4f("modelViewMatrix", modelViewMatrix);
		currentShader.setUniformMatrix4f("modelViewProjectionMatrix", modelViewMatrix*projectionMatrix*orientationMatrix);
	}
	if (currentMatrixMode == OF_MATRIX_PROJECTION){
		currentShader.setUniformMatrix4f("projectionMatrix", projectionMatrix*orientationMatrix);
		currentShader.setUniformMatrix4f("modelViewProjectionMatrix", modelViewMatrix*projectionMatrix*orientationMatrix);
	}
	if (currentMatrixMode == OF_MATRIX_TEXTURE){
		currentShader.setUniformMatrix4f("textureMatrix", textureMatrix);
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
	currentShader.setUniform4f("color",currentColor.r,currentColor.g,currentColor.b,currentColor.a);
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
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	ofLogVerbose() << "glLineWidth has no effect in openGL3.2+\nUse a geometry shader to generate thick lines.";
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

		case OF_BLENDMODE_ALPHA:{
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}

		case OF_BLENDMODE_ADD:{
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		}

		case OF_BLENDMODE_MULTIPLY:{
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA /* GL_ZERO or GL_ONE_MINUS_SRC_ALPHA */);
			break;
		}

		case OF_BLENDMODE_SCREEN:{
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
			break;
		}

		case OF_BLENDMODE_SUBTRACT:{
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		}


		default:
			break;
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::enablePointSprites(){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::disablePointSprites(){
    // TODO :: needs ES2 code.
}

//----------------------------------------------------------
GLint ofProgrammableGLRenderer::getAttrLocationPosition(){
	return currentShader.getAttributeLocation("position");
}

//----------------------------------------------------------
GLint ofProgrammableGLRenderer::getAttrLocationColor(){
	return currentShader.getAttributeLocation("color");
}

//----------------------------------------------------------
GLint ofProgrammableGLRenderer::getAttrLocationNormal(){
	return currentShader.getAttributeLocation("normal");
}

//----------------------------------------------------------
GLint ofProgrammableGLRenderer::getAttrLocationTexCoord(){
	return currentShader.getAttributeLocation("texcoord");
}

//----------------------------------------------------------
ofShader & ofProgrammableGLRenderer::getCurrentShader(){
	return currentShader;
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::setDefaultShader(ofShader & shader){
	externalShader = shader;
	externalShaderProvided = true;
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::enableVertices(){
	glEnableVertexAttribArray(getAttrLocationPosition());
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::enableTexCoords(){
	texCoordsEnabled = true;
	ofShader prevShader = currentShader;
	if(!usingCustomShader) beginDefaultShader();

	if(prevShader==currentShader){
		GLint loc = getAttrLocationTexCoord();
		if (loc != -1){
			glEnableVertexAttribArray(loc);
		}
		currentShader.setUniform1f("useTexture",1);
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::enableColors(){
	colorsEnabled = true;
	ofShader prevShader = currentShader;
	if(!usingCustomShader) beginDefaultShader();

	if(prevShader==currentShader){
		GLint loc = getAttrLocationColor();
		if (loc != -1){
			glEnableVertexAttribArray(loc);
		}
		currentShader.setUniform1f("useColors",1);
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::enableNormals(){
	normalsEnabled = true;
	GLint loc = getAttrLocationNormal();
	if (loc != -1){
		glEnableVertexAttribArray(getAttrLocationNormal());
	}
}

void ofProgrammableGLRenderer::disableVertices(){
	glDisableVertexAttribArray(getAttrLocationPosition());
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::disableTexCoords(){
	texCoordsEnabled = false;
	ofShader prevShader = currentShader;
	if(!usingCustomShader) beginDefaultShader();

	if(prevShader==currentShader){
		GLint loc = getAttrLocationTexCoord();
		if (loc != -1){
			glDisableVertexAttribArray(loc);
		}
		currentShader.setUniform1f("useTexture",0);
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::disableColors(){
	colorsEnabled = false;
	ofShader prevShader = currentShader;
	if(!usingCustomShader) beginDefaultShader();

	if(prevShader==currentShader){
		GLint loc = getAttrLocationColor();
		if (loc != -1){
			glDisableVertexAttribArray(loc);
		}
		currentShader.setUniform1f("useColors",0);
	}
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::disableNormals(){
	GLint loc = getAttrLocationNormal();
	if (loc != -1){
		glDisableVertexAttribArray(loc);
	}
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
	if(currentShader==shader) return;

	shader.setUniform1f("useTexture",texCoordsEnabled);
	shader.setUniform1f("useColors",colorsEnabled);
	shader.setUniform4f("color",currentColor.r,currentColor.g,currentColor.b,currentColor.a);

	currentShader = shader;
	enableAttributes();
	uploadAllMatrices();
	usingCustomShader = true;
}

void ofProgrammableGLRenderer::uploadAllMatrices(){
	currentShader.setUniformMatrix4f("modelViewMatrix", modelViewMatrix);
	currentShader.setUniformMatrix4f("projectionMatrix", projectionMatrix*orientationMatrix);
	currentShader.setUniformMatrix4f("textureMatrix", textureMatrix);
	currentShader.setUniformMatrix4f("modelViewProjectionMatrix", modelViewMatrix*projectionMatrix*orientationMatrix);
}

void ofProgrammableGLRenderer::disableAtributtes(){

	if (!currentShader.isLoaded()) return;
	
	{
		int loc = getAttrLocationPosition();
		if(loc!=-1){
		glDisableVertexAttribArray(loc);
		}
	}
	if(texCoordsEnabled){
		int loc = getAttrLocationTexCoord();
		if(loc!=-1){
			glDisableVertexAttribArray(loc);
		}
	}
	if(colorsEnabled){
		int loc = getAttrLocationColor();
		if(loc!=-1){
			glDisableVertexAttribArray(loc);
		}
	}
	if(normalsEnabled){
		int loc = getAttrLocationNormal();
		if(loc!=-1){
			glDisableVertexAttribArray(loc);
		}
	}
}

void ofProgrammableGLRenderer::enableAttributes(){
	glEnableVertexAttribArray(getAttrLocationPosition());
	if(texCoordsEnabled){
		int loc = getAttrLocationTexCoord();
		if(loc!=-1){
			glEnableVertexAttribArray(loc);
		}
	}
	if(colorsEnabled){
		int loc = getAttrLocationColor();
		if(loc!=-1){
			glEnableVertexAttribArray(loc);
		}
	}
	if(normalsEnabled){
		int loc = getAttrLocationNormal();
		if(loc!=-1){
			glEnableVertexAttribArray(loc);
		}
	}
}


void ofProgrammableGLRenderer::beginDefaultShader(){
	ofShader nextShader;

	if(externalShaderProvided){
		nextShader = externalShader;
	}else{
		if(colorsEnabled && texCoordsEnabled){
			switch(currentTextureTarget){
			case GL_TEXTURE_RECTANGLE_ARB:
				nextShader = defaultShaderTexColor;
				break;
			case GL_TEXTURE_2D:
				nextShader = defaultShaderTex2DColor;
				break;
			case OF_NO_TEXTURE:
				nextShader = defaultShaderNoTexColor;
				break;
			}
		}else if(colorsEnabled){
			nextShader = defaultShaderNoTexColor;
		}else if(texCoordsEnabled){
			switch(currentTextureTarget){
			case GL_TEXTURE_RECTANGLE_ARB:
				nextShader = defaultShaderTexNoColor;
				break;
			case GL_TEXTURE_2D:
				nextShader = defaultShaderTex2DNoColor;
				break;
			case OF_NO_TEXTURE:
				nextShader = defaultShaderNoTexNoColor;
				break;
			}
		}else{
			nextShader = defaultShaderNoTexNoColor;
		}
	}

	if(currentShader!=nextShader){
		disableAtributtes();
		nextShader.begin();
	}
	usingCustomShader = false;
}

//----------------------------------------------------------
void ofProgrammableGLRenderer::endCustomShader(){
	disableAtributtes();
	beginDefaultShader();
}



inline void ofProgrammableGLRenderer::preparePrimitiveDraw(ofVbo& vbo_){
	glBindBuffer(GL_ARRAY_BUFFER, vbo_.getVertId()); // bind to triangle vertices
	glEnableVertexAttribArray(currentShader.getAttributeLocation("position"));	// activate attribute 'position' in shader
	glVertexAttribPointer(currentShader.getAttributeLocation("position"), 3, GL_FLOAT,GL_FALSE,0,0);
	
}

inline void ofProgrammableGLRenderer::finishPrimitiveDraw(){
	// when drawing a vbo other attrib arrays are bound, but the vbo takes care of unbinding the
	// color, texcoord and index arrays. 
	// glDisableVertexAttribArray(0);		 	// disable vertex attrib array.
	glBindBuffer(GL_ARRAY_BUFFER,0);		// unbind current buffer by binding to zero
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
			//scale(1, -1, 0);
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
			
			ofVec3f dScreen = ofVec3f(x,y,z) * modelViewMatrix * projectionMatrix;
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
			if(!isVFlipped()){
				scale(1,-1,1);
			}
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
	
	bitmapStringShader.begin();

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
	bitmapStringShader.end();


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
