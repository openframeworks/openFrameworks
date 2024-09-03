#include "of3dPrimitives.h"
#include "ofBitmapFont.h"
#include "ofCamera.h"
#include "ofFbo.h"
#include "ofGLProgrammableRenderer.h"
#include "ofGLUtils.h"
#include "ofImage.h"
#include "ofLight.h"
#include "ofMaterialBaseTypes.h"
#include "ofMesh.h"
#include "ofNode.h"
#include "ofPath.h"
#include "ofTrueTypeFont.h"
#include "ofVboMesh.h"
#include "ofVideoBaseTypes.h"

using std::string;
using std::swap;
using std::vector;

static const string MODEL_MATRIX_UNIFORM = "modelMatrix";
static const string VIEW_MATRIX_UNIFORM = "viewMatrix";
static const string MODELVIEW_MATRIX_UNIFORM = "modelViewMatrix";
static const string PROJECTION_MATRIX_UNIFORM = "projectionMatrix";
static const string MODELVIEW_PROJECTION_MATRIX_UNIFORM = "modelViewProjectionMatrix";
static const string TEXTURE_MATRIX_UNIFORM = "textureMatrix";
static const string COLOR_UNIFORM = "globalColor";

static const string USE_TEXTURE_UNIFORM = "usingTexture";
static const string USE_COLORS_UNIFORM = "usingColors";
static const string BITMAP_STRING_UNIFORM = "bitmapText";

const string ofGLProgrammableRenderer::TYPE = "ProgrammableGL";
static bool programmableRendererCreated = false;

bool ofIsGLProgrammableRenderer() {
	return programmableRendererCreated;
}

//----------------------------------------------------------
ofGLProgrammableRenderer::ofGLProgrammableRenderer(const ofAppBaseWindow * _window)
	: matrixStack(_window)
	, graphics3d(this) {
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
	usingVideoShader = false;
	usingCustomShader = false;

	wrongUseLoggedOnce = false;

	uniqueShader = false;

	currentShader = nullptr;

	currentTextureTarget = OF_NO_TEXTURE;
	currentMaterial = nullptr;
	alphaMaskTextureTarget = OF_NO_TEXTURE;

	currentShadow = nullptr;
	bIsShadowDepthPass = false;

	major = 3;
	minor = 2;
	window = _window;

	currentFramebufferId = 0;
	defaultFramebufferId = 0;
	path.setMode(ofPath::POLYLINES);
	path.setUseShapeColor(false);
	currentEyePos = { 0.f, 0.f, 0.f };
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::startRender() {
	currentFramebufferId = defaultFramebufferId;
	framebufferIdStack.push_back(defaultFramebufferId);
	matrixStack.setRenderSurface(*window);
	beginDefaultShader();
	viewport();
	// to do non auto clear on PC for now - we do something like "single" buffering --
	// it's not that pretty but it work for the most part

#ifdef TARGET_WIN32
	if (getBackgroundAuto() == false) {
		glDrawBuffer(GL_FRONT);
	}
#endif

	if (getBackgroundAuto()) { // || ofGetFrameNum() < 3){
		background(currentStyle.bgColor);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::finishRender() {
	if (!uniqueShader) {
		glUseProgram(0);
		if (!usingCustomShader) currentShader = nullptr;
	}
	matrixStack.clearStacks();
	framebufferIdStack.clear();
	mDrawMode = GL_TRIANGLES;
	mUniformsTex.clear();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const {
	if (vertexData.getVertices().empty()) return;

		// tig: note that for GL3+ we use glPolygonMode to draw wireframes or filled meshes, and not the primitive mode.
		// the reason is not purely aesthetic, but more conformant with the behaviour of ofGLRenderer. Whereas
		// gles2.0 doesn't allow for a polygonmode.
		// Also gles2 still supports vertex array syntax for uploading data to attributes and it seems to be faster than
		// vbo's for meshes that are updated frequently so let's use that instead

		//if (bSmoothHinted) startSmoothing();

#if defined(TARGET_OPENGLES) && !defined(TARGET_EMSCRIPTEN)
	glEnableVertexAttribArray(ofShader::POSITION_ATTRIBUTE);
	glVertexAttribPointer(ofShader::POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, sizeof(typename ofMesh::VertexType), vertexData.getVerticesPointer());

	useNormals &= (vertexData.getNumNormals() > 0);
	if (useNormals) {
		glEnableVertexAttribArray(ofShader::NORMAL_ATTRIBUTE);
		glVertexAttribPointer(ofShader::NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, sizeof(typename ofMesh::NormalType), vertexData.getNormalsPointer());
	} else {
		glDisableVertexAttribArray(ofShader::NORMAL_ATTRIBUTE);
	}

	useColors &= (vertexData.getNumColors() > 0);
	if (useColors) {
		glEnableVertexAttribArray(ofShader::COLOR_ATTRIBUTE);
		glVertexAttribPointer(ofShader::COLOR_ATTRIBUTE, 4, GL_FLOAT, GL_FALSE, sizeof(ofFloatColor), vertexData.getColorsPointer());
	} else {
		glDisableVertexAttribArray(ofShader::COLOR_ATTRIBUTE);
	}

	useTextures &= (vertexData.getNumTexCoords() > 0);
	if (useTextures) {
		glEnableVertexAttribArray(ofShader::TEXCOORD_ATTRIBUTE);
		glVertexAttribPointer(ofShader::TEXCOORD_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(typename ofMesh::TexCoordType), vertexData.getTexCoordsPointer());
	} else {
		glDisableVertexAttribArray(ofShader::TEXCOORD_ATTRIBUTE);
	}

	// const_cast<ofGLProgrammableRenderer *>(this)->setAttributes(true, useColors, useTextures, useNormals);

	GLenum drawMode;
	switch (renderType) {
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
	const_cast<ofGLProgrammableRenderer *>(this)->setAttributes(true, useColors, useTextures, useNormals, drawMode);

	if (vertexData.getNumIndices()) {
		glDrawElements(drawMode, vertexData.getNumIndices(), GL_UNSIGNED_SHORT, vertexData.getIndexPointer());
	} else {
		glDrawArrays(drawMode, 0, vertexData.getNumVertices());
	}
#else
	
	ofVbo* vboToRender = nullptr;
	bool tGoingToRenderLines = false;
#ifndef TARGET_OPENGLES
//	meshVbo.setMesh(vertexData, GL_STREAM_DRAW, useColors, useTextures, useNormals);
	glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
	GLenum drawMode = ofGetGLPrimitiveMode(vertexData.getMode());
	// if the render type is different than the primitive mode
	// ie. mesh mode is triangles but we called mesh.drawVertices() which uses GL_POINT for the render type
	// however, we need GL_POINTS for rendering point sprites
	if (pointSpritesEnabled && !usingCustomShader && !uniqueShader) {
		if (renderType == OF_MESH_POINTS) {
			drawMode = GL_POINTS;
		}
	}
	
	bool bConfigureForLinesShader = areLinesShadersEnabled() && (drawMode == GL_LINES || drawMode == GL_LINE_STRIP || drawMode == GL_LINE_LOOP);
	if( usingCustomShader || usingCustomShader || currentMaterial) {
		bConfigureForLinesShader = false;
	}
	
	if( bConfigureForLinesShader ) {
		mDrawMode = drawMode;
		tGoingToRenderLines = true;
		ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer *>(this);
		if( drawMode == GL_LINES ) {
			mutThis->configureLinesBundleFromMesh( mutThis->mLinesBundleMap[GL_LINES], drawMode, vertexData);
			vboToRender = &mutThis->mLinesBundleMap[GL_LINES].vbo;
		} else {
			mutThis->configureLinesBundleFromMesh( mutThis->mLinesBundleMap[GL_LINE_STRIP], drawMode, vertexData);
			vboToRender = &mutThis->mLinesBundleMap[GL_LINE_STRIP].vbo;
		}
	} else {
		meshVbo.setMesh(vertexData, GL_STREAM_DRAW, useColors, useTextures, useNormals);
		vboToRender = &meshVbo;
	}

#else
//	meshVbo.setMesh(vertexData, GL_STATIC_DRAW, useColors, useTextures, useNormals);
	GLenum drawMode;
	switch (renderType) {
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
	
	bool bConfigureForLinesShader = areLinesShadersEnabled() && (drawMode == GL_LINES || drawMode == GL_LINE_STRIP || drawMode == GL_LINE_LOOP);
	if( usingCustomShader || usingCustomShader || currentMaterial) {
		bConfigureForLinesShader = false;
	}
	
	if(bConfigureForLinesShader) {
		mDrawMode = drawMode;
		tGoingToRenderLines = true;
		ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer *>(this);
		if( drawMode == GL_LINES ) {
			mutThis->configureLinesBundleFromMesh( mutThis->mLinesBundleMap[GL_LINES], drawMode, vertexData);
			vboToRender = &mutThis->mLinesBundleMap[GL_LINES].vbo;
		} else {
			mutThis->configureLinesBundleFromMesh( mutThis->mLinesBundleMap[GL_LINE_STRIP], drawMode, vertexData);
			vboToRender = &mutThis->mLinesBundleMap[GL_LINE_STRIP].vbo;
		}
	} else {
		meshVbo.setMesh(vertexData, GL_STATIC_DRAW, useColors, useTextures, useNormals);
		vboToRender = &meshVbo;
	}
	
	#endif
	
	if( vboToRender != nullptr ) {
		if( tGoingToRenderLines ) {
			// Setting a bool here so that the setAttributes function does not try to switch the shaders because
			// we are going to draw the mesh as triangles and we need the lines shader
			// we are rendering lines, and the meshes we constructed are made of triangles
			mBRenderingLines = true;
			if( renderType == OF_MESH_FILL ) {
				drawMode = GL_TRIANGLES;
			}
		}
		
		if (vboToRender->getUsingIndices()) {
			drawElements(*vboToRender, drawMode, vboToRender->getNumIndices());
		} else {
			draw(*vboToRender, drawMode, 0, vboToRender->getNumVertices());
		}
		
		if( tGoingToRenderLines ) {
			mBRenderingLines = false;
		}
	}

	// tig: note further that we could glGet() and store the current polygon mode, but don't, since that would
	// infer a massive performance hit. instead, we revert the glPolygonMode to mirror the current ofFill state
	// after we're finished drawing, following the principle of least surprise.
	// ideally the glPolygonMode (or the polygon draw mode) should be part of ofStyle so that we can keep track
	// of its state on the client side...

	#ifndef TARGET_OPENGLES
	glPolygonMode(GL_FRONT_AND_BACK, currentStyle.bFill ? GL_FILL : GL_LINE);
	#endif

#endif

	//if (bSmoothHinted) endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofVboMesh & mesh, ofPolyRenderMode renderType) const {
	drawInstanced(mesh, renderType, 1);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawInstanced(const ofVboMesh & mesh, ofPolyRenderMode renderType, int primCount) const {
	if (mesh.getNumVertices() == 0) return;
	GLuint mode = ofGetGLPrimitiveMode(mesh.getMode());
	// nh: if the render type is different than the primitive mode
	// ie. mesh mode is triangles but we called mesh.drawVertices() which uses GL_POINT for the render type
	// however, we need GL_POINTS for rendering point sprites
	if (pointSpritesEnabled) {
		if (renderType == OF_MESH_POINTS) {
			mode = GL_POINTS;
		}
	}
#if !defined( TARGET_OPENGLES ) || defined(TARGET_EMSCRIPTEN)
	#if !defined(TARGET_EMSCRIPTEN)
	glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
	#else
	// nh: glPolygonMode is not supported via emscripten,
	// we can not render wire frames with vbos.
	// this is not the best solution, but does provide some information
	// and does not render as solid when wireframe mode is requested.
	if (renderType == OF_MESH_WIREFRAME) {
		if (mesh.getNumIndices()) {
			drawElements(mesh.getVbo(), GL_LINES, mesh.getNumIndices());
		} else {
			draw(mesh.getVbo(), GL_LINES, 0, mesh.getNumVertices());
		}
	} else {
	#endif
		if (mesh.getNumIndices() && renderType != OF_MESH_POINTS) {
			if (primCount <= 1) {
				drawElements(mesh.getVbo(), mode, mesh.getNumIndices());
			} else {
				drawElementsInstanced(mesh.getVbo(), mode, mesh.getNumIndices(), primCount);
			}
		} else {
			if (primCount <= 1) {
				draw(mesh.getVbo(), mode, 0, mesh.getNumVertices());
			} else {
				drawInstanced(mesh.getVbo(), mode, 0, mesh.getNumVertices(), primCount);
			}
		}
	#if defined(TARGET_EMSCRIPTEN)
	} // close the if for checking for wireframe
	#endif
	
	// tig: note further that we could glGet() and store the current polygon mode, but don't, since that would
	// infer a massive performance hit. instead, we revert the glPolygonMode to mirror the current ofFill state
	// after we're finished drawing, following the principle of least surprise.
	// ideally the glPolygonMode (or the polygon draw mode) should be part of ofStyle so that we can keep track
	// of its state on the client side...
	#if !defined(TARGET_EMSCRIPTEN)
		glPolygonMode(GL_FRONT_AND_BACK, currentStyle.bFill ? GL_FILL : GL_LINE);
	#endif
#else
	if (renderType == OF_MESH_POINTS) {
		draw(mesh.getVbo(), GL_POINTS, 0, mesh.getNumVertices());
	} else if (renderType == OF_MESH_WIREFRAME) {
		if (mesh.getNumIndices()) {
			drawElements(mesh.getVbo(), GL_LINES, mesh.getNumIndices());
		} else {
			draw(mesh.getVbo(), GL_LINES, 0, mesh.getNumVertices());
		}
	} else {
		if (mesh.getNumIndices()) {
			drawElements(mesh.getVbo(), mode, mesh.getNumIndices());
		} else {
			draw(mesh.getVbo(), mode, 0, mesh.getNumVertices());
		}
	}
#endif
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const of3dPrimitive & model, ofPolyRenderMode renderType) const {
	const_cast<ofGLProgrammableRenderer *>(this)->pushMatrix();
	const_cast<ofGLProgrammableRenderer *>(this)->multMatrix(model.getGlobalTransformMatrix());
	if (model.isUsingVbo()) {
		draw(static_cast<const ofVboMesh &>(model.getMesh()), renderType);
	} else {
		draw(model.getMesh(), renderType);
	}
	const_cast<ofGLProgrammableRenderer *>(this)->popMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofNode & node) const {
	const_cast<ofGLProgrammableRenderer *>(this)->pushMatrix();
	const_cast<ofGLProgrammableRenderer *>(this)->multMatrix(node.getGlobalTransformMatrix());
	node.customDraw(this);
	const_cast<ofGLProgrammableRenderer *>(this)->popMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofPolyline & poly) const {
	if (poly.getVertices().empty()) return;

	// use smoothness, if requested:
	//if (bSmoothHinted) startSmoothing();

#if defined(TARGET_OPENGLES) && !defined(TARGET_EMSCRIPTEN)

	glEnableVertexAttribArray(ofShader::POSITION_ATTRIBUTE);
	glVertexAttribPointer(ofShader::POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, sizeof(typename ofPolyline::VertexType), &poly[0]);

	// const_cast<ofGLProgrammableRenderer *>(this)->setAttributes(true, false, false, false);

	GLenum drawMode = poly.isClosed() ? GL_LINE_LOOP : GL_LINE_STRIP;
	const_cast<ofGLProgrammableRenderer *>(this)->setAttributes(true, false, false, false, drawMode);

	glDrawArrays(drawMode, 0, poly.size());

#else
	
//	polylineMesh.clear();
//	polylineMesh.addVertices(poly.getVertices());
	
	
	polylineMesh.getVertices() = poly.getVertices();
	// check if it is closed and the last point is the same as the first
	if( poly.isClosed() ) {
		if(polylineMesh.getNumVertices() > 1 && polylineMesh.getVertices().front() == polylineMesh.getVertices().back() ) {
			polylineMesh.getVertices().pop_back();
		}
	}
	
	if( currentTextureTarget != OF_NO_TEXTURE ) {
		// TODO: Should we be able to set tex coords on polylines somehow??
		polylineMesh.getTexCoords().resize( polylineMesh.getNumVertices(), glm::vec2(0.f, 0.f));
		auto& tcs = polylineMesh.getTexCoords();
		int numtxs = (int)polylineMesh.getNumVertices();
		float numTxsF = (float)numtxs - 1.f;
		if( numTxsF > 0.0f ) {
			for( int ix = 0; ix < numtxs; ix++ ) {
				tcs[ix].x = (float)ix / numTxsF;
			}
		}
		polylineMesh.enableTextures();
	} else {
		polylineMesh.disableTextures();
	}
	
	polylineMesh.setMode( poly.isClosed() ? OF_PRIMITIVE_LINE_LOOP : OF_PRIMITIVE_LINE_STRIP );
//	draw(const ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const;
	draw(polylineMesh, OF_MESH_FILL, false, false, false);
	
	
//	meshVbo.setVertexData(&poly.getVertices()[0], poly.size(), GL_DYNAMIC_DRAW);
//	meshVbo.draw(poly.isClosed() ? GL_LINE_LOOP : GL_LINE_STRIP, 0, poly.size());
//	meshPolylineVbo.setVertexData(&poly.getVertices()[0], poly.size(), GL_DYNAMIC_DRAW);
//	meshPolylineVbo.draw(poly.isClosed() ? GL_LINE_LOOP : GL_LINE_STRIP, 0, poly.size());

#endif
	// use smoothness, if requested:
	//if (bSmoothHinted) endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofPath & shape) const {
	ofFloatColor prevColor;
	if (shape.getUseShapeColor()) {
		prevColor = currentStyle.color;
	}
	ofGLProgrammableRenderer * mut_this = const_cast<ofGLProgrammableRenderer *>(this);
	if (shape.isFilled()) {
		const ofMesh & mesh = shape.getTessellation();
		if (shape.getUseShapeColor()) {
			mut_this->setColor(shape.getFillColor(), shape.getFillColor().a);
		}
		draw(mesh, OF_MESH_FILL);
	}
	if (shape.hasOutline()) {
		float lineWidth = currentStyle.lineWidth;
		if (shape.getUseShapeColor()) {
			mut_this->setColor(shape.getStrokeColor(), shape.getStrokeColor().a);
		}
		mut_this->setLineWidth(shape.getStrokeWidth());
		const vector<ofPolyline> & outlines = shape.getOutline();
		for (int i = 0; i < (int)outlines.size(); i++)
			draw(outlines[i]);
		mut_this->setLineWidth(lineWidth);
	}
	if (shape.getUseShapeColor()) {
		mut_this->setColor(prevColor);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const {
	if (image.isUsingTexture()) {
		const_cast<ofGLProgrammableRenderer *>(this)->setAttributes(true, false, true, false, GL_TRIANGLES);
		const ofTexture & tex = image.getTexture();
		if (tex.isAllocated()) {
			const_cast<ofGLProgrammableRenderer *>(this)->bind(tex, 0);
			draw(tex.getMeshForSubsection(x, y, z, w, h, sx, sy, sw, sh, isVFlipped(), currentStyle.rectMode), OF_MESH_FILL, false, true, false);
			const_cast<ofGLProgrammableRenderer *>(this)->unbind(tex, 0);
		} else {
			ofLogWarning("ofGLProgrammableRenderer") << "draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const {
	if (image.isUsingTexture()) {
		const_cast<ofGLProgrammableRenderer *>(this)->setAttributes(true, false, true, false, GL_TRIANGLES);
		const ofTexture & tex = image.getTexture();
		if (tex.isAllocated()) {
			const_cast<ofGLProgrammableRenderer *>(this)->bind(tex, 0);
			draw(tex.getMeshForSubsection(x, y, z, w, h, sx, sy, sw, sh, isVFlipped(), currentStyle.rectMode), OF_MESH_FILL, false, true, false);
			const_cast<ofGLProgrammableRenderer *>(this)->unbind(tex, 0);
		} else {
			ofLogWarning("ofGLProgrammableRenderer") << "draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const {
	if (image.isUsingTexture()) {
		const_cast<ofGLProgrammableRenderer *>(this)->setAttributes(true, false, true, false, GL_TRIANGLES);
		const ofTexture & tex = image.getTexture();
		if (tex.isAllocated()) {
			const_cast<ofGLProgrammableRenderer *>(this)->bind(tex, 0);
			draw(tex.getMeshForSubsection(x, y, z, w, h, sx, sy, sw, sh, isVFlipped(), currentStyle.rectMode), OF_MESH_FILL, false, true, false);
			const_cast<ofGLProgrammableRenderer *>(this)->unbind(tex, 0);
		} else {
			ofLogWarning("ofGLProgrammableRenderer") << "draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofTexture & tex, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const {
	const_cast<ofGLProgrammableRenderer *>(this)->setAttributes(true, false, true, false, GL_TRIANGLES);
	if (tex.isAllocated()) {
		const_cast<ofGLProgrammableRenderer *>(this)->bind(tex, 0);
		draw(tex.getMeshForSubsection(x, y, z, w, h, sx, sy, sw, sh, isVFlipped(), currentStyle.rectMode), OF_MESH_FILL, false, true, false);
		const_cast<ofGLProgrammableRenderer *>(this)->unbind(tex, 0);
	} else {
		ofLogWarning("ofGLProgrammableRenderer") << "draw(): texture is not allocated";
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const {
	if (!video.isInitialized() || !video.isUsingTexture() || video.getTexturePlanes().empty()) {
		return;
	}
	const_cast<ofGLProgrammableRenderer *>(this)->bind(video);
	draw(video.getTexture().getMeshForSubsection(x, y, 0, w, h, 0, 0, video.getWidth(), video.getHeight(), isVFlipped(), currentStyle.rectMode), OF_MESH_FILL, false, true, false);
	const_cast<ofGLProgrammableRenderer *>(this)->unbind(video);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::draw(const ofVbo & vbo, GLuint drawMode, int first, int total) const {
	if (vbo.getUsingVerts()) {	
		vbo.bind();
		const_cast<ofGLProgrammableRenderer *>(this)->setAttributes(vbo.getUsingVerts(), vbo.getUsingColors(), vbo.getUsingTexCoords(), vbo.getUsingNormals(), drawMode);
		glDrawArrays(drawMode, first, total);
		vbo.unbind();
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawElements(const ofVbo & vbo, GLuint drawMode, int amt, int offsetelements) const {
	if (vbo.getUsingVerts()) {
		vbo.bind();
		const_cast<ofGLProgrammableRenderer *>(this)->setAttributes(vbo.getUsingVerts(), vbo.getUsingColors(), vbo.getUsingTexCoords(), vbo.getUsingNormals(), drawMode);
#ifdef TARGET_OPENGLES
		glDrawElements(drawMode, amt, GL_UNSIGNED_SHORT, (void *)(sizeof(ofIndexType) * offsetelements));
#else
		glDrawElements(drawMode, amt, GL_UNSIGNED_INT, (void *)(sizeof(ofIndexType) * offsetelements));
#endif
		vbo.unbind();
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawInstanced(const ofVbo & vbo, GLuint drawMode, int first, int total, int primCount) const {
	if (vbo.getUsingVerts()) {
		vbo.bind();
		const_cast<ofGLProgrammableRenderer *>(this)->setAttributes(vbo.getUsingVerts(), vbo.getUsingColors(), vbo.getUsingTexCoords(), vbo.getUsingNormals(), drawMode);
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
void ofGLProgrammableRenderer::drawElementsInstanced(const ofVbo & vbo, GLuint drawMode, int amt, int primCount) const {
	if (vbo.getUsingVerts()) {
		vbo.bind();
		const_cast<ofGLProgrammableRenderer *>(this)->setAttributes(vbo.getUsingVerts(), vbo.getUsingColors(), vbo.getUsingTexCoords(), vbo.getUsingNormals(), drawMode);
#if defined(TARGET_OPENGLES) && !defined(TARGET_EMSCRIPTEN) // TODO: Check against OPENGL_ES Version
		// todo: activate instancing once OPENGL ES supports instancing, starting with version 3.0
		// unfortunately there is currently no easy way within oF to query the current OpenGL version.
		// https://www.khronos.org/opengles/sdk/docs/man3/xhtml/glDrawElementsInstanced.xml
		ofLogWarning("ofVbo") << "drawElementsInstanced(): hardware instancing is not supported on OpenGL ES < 3.0";
		// glDrawElementsInstanced(drawMode, amt, GL_UNSIGNED_SHORT, nullptr, primCount);
#else
		#if defined(TARGET_OPENGLES)
		glDrawElementsInstanced(drawMode, amt, GL_UNSIGNED_SHORT, nullptr, primCount);
		#else
		glDrawElementsInstanced(drawMode, amt, GL_UNSIGNED_INT, nullptr, primCount);
		#endif
#endif
		vbo.unbind();
	}
}

//----------------------------------------------------------
ofPath & ofGLProgrammableRenderer::getPath() {
	return path;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofBaseVideoDraws & video) {
	if (!video.isInitialized() || !video.isUsingTexture() || video.getTexturePlanes().empty()) {
		return;
	}
	const ofShader * shader = nullptr;
	if (!usingCustomShader) {
		shader = getVideoShader(video);
		if (shader) {
			bind(*shader);
			setVideoShaderUniforms(video, *shader);
			usingVideoShader = true;
		}
	}

	if (!usingVideoShader) {
		bind(video.getTexture(), 0);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofBaseVideoDraws & video) {
	if (!video.isInitialized() || !video.isUsingTexture() || video.getTexturePlanes().empty()) {
		return;
	}
	if (usingVideoShader) {
		unbind(*currentShader);
	} else {
		unbind(video.getTexture(), 0);
	}
	usingVideoShader = false;
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
void ofGLProgrammableRenderer::viewport(ofRectangle viewport_) {
	viewport(viewport_.x, viewport_.y, viewport_.width, viewport_.height, isVFlipped());
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::viewport(float x, float y, float width, float height, bool vflip) {
	matrixStack.viewport(x, y, width, height, vflip);
	ofRectangle nativeViewport = matrixStack.getNativeViewport();
	glViewport(nativeViewport.x, nativeViewport.y, nativeViewport.width, nativeViewport.height);
}

//----------------------------------------------------------
ofRectangle ofGLProgrammableRenderer::getCurrentViewport() const {
	return matrixStack.getCurrentViewport();
}

//----------------------------------------------------------
ofRectangle ofGLProgrammableRenderer::getNativeViewport() const {
	return matrixStack.getNativeViewport();
}

//----------------------------------------------------------
int ofGLProgrammableRenderer::getViewportWidth() const {
	return getCurrentViewport().width;
}

//----------------------------------------------------------
int ofGLProgrammableRenderer::getViewportHeight() const {
	return getCurrentViewport().height;
}

//----------------------------------------------------------
bool ofGLProgrammableRenderer::isVFlipped() const {
	return matrixStack.isVFlipped();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setCoordHandedness(ofHandednessType handedness) {
	;
}

//----------------------------------------------------------
ofHandednessType ofGLProgrammableRenderer::getCoordHandedness() const {
	return matrixStack.getHandedness();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setOrientation(ofOrientation orientation, bool vFlip) {
	matrixStack.setOrientation(orientation, vFlip);
	uploadMatrices();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setupScreenPerspective(float width, float height, float fov, float nearDist, float farDist) {
	float viewW, viewH;
	if (width < 0 || height < 0) {
		ofRectangle currentViewport = getCurrentViewport();

		viewW = currentViewport.width;
		viewH = currentViewport.height;
	} else {
		viewW = width;
		viewH = height;
	}

	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
	float halfFov = glm::pi<float>() * fov / 360.0f;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float)viewW / viewH;

	if (nearDist == 0) nearDist = dist / 10.0f;
	if (farDist == 0) farDist = dist * 10.0f;

	matrixMode(OF_MATRIX_PROJECTION);
	auto persp = glm::perspective(glm::radians(fov), aspect, nearDist, farDist);
	loadMatrix(persp);

	matrixMode(OF_MATRIX_MODELVIEW);
	auto lookAt = glm::lookAt(glm::vec3 { eyeX, eyeY, dist }, glm::vec3 { eyeX, eyeY, 0.f }, glm::vec3 { 0.f, 1.f, 0.f });
	loadViewMatrix(lookAt);

	currentEyePos = glm::vec3 { eyeX, eyeY, dist };
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setupScreenOrtho(float width, float height, float nearDist, float farDist) {
	float viewW, viewH;
	if (width < 0 || height < 0) {
		ofRectangle currentViewport = getCurrentViewport();

		viewW = currentViewport.width;
		viewH = currentViewport.height;
	} else {
		viewW = width;
		viewH = height;
	}

	auto ortho = glm::ortho(0.f, viewW, 0.f, viewH, nearDist, farDist);

	matrixMode(OF_MATRIX_PROJECTION);
	loadMatrix(ortho); // make ortho our new projection matrix.

	matrixMode(OF_MATRIX_MODELVIEW);
	loadViewMatrix(glm::mat4(1.0));

	currentEyePos = glm::vec3 { viewW / 2.0f, viewH / 2.0f, nearDist };
}

//----------------------------------------------------------
//Resets openGL parameters back to OF defaults
void ofGLProgrammableRenderer::setupGraphicDefaults() {
	setStyle(ofStyle());
	path.setMode(ofPath::POLYLINES);
	path.setUseShapeColor(false);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setupScreen() {
	beginDefaultShader();
	setupScreenPerspective(); // assume defaults
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setCircleResolution(int res) {
	if ((int)circlePolyline.size() != res + 1) {
		circlePolyline.clear();
		circlePolyline.arc(0, 0, 0, 1, 1, 0, 360, res);
		circleMesh.getVertices() = circlePolyline.getVertices();
		path.setCircleResolution(res);
		
		// for the outline polyline, we need a closed loop
		// so we make another line that is closed, excluding the same start and end points
		circleOutlinePolyline.clear();
		for( int i = 0; i < res; i++ ) {
			float ta = ((float)i / (float)res) * glm::two_pi<float>();
			circleOutlinePolyline.addVertex(glm::vec3(cosf(ta),sinf(ta), 0.0f));
		}
//		circleOutlinePolyline.setClosed(true);
		
		circleOutlineMesh.getVertices() = circleOutlinePolyline.getVertices();
		
		
	}
	currentStyle.circleResolution = res;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setPolyMode(ofPolyWindingMode mode) {
	currentStyle.polyMode = mode;
	path.setPolyWindingMode(mode);
}

//our openGL wrappers
//----------------------------------------------------------
void ofGLProgrammableRenderer::pushMatrix() {
	matrixStack.pushMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::popMatrix() {
	matrixStack.popMatrix();
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::translate(const glm::vec3 & p) {
	translate(p.x, p.y, p.z);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::translate(float x, float y, float z) {
	matrixStack.translate(x, y, z);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::scale(float xAmnt, float yAmnt, float zAmnt) {
	matrixStack.scale(xAmnt, yAmnt, zAmnt);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::rotateRad(float radians, float vecX, float vecY, float vecZ) {
	matrixStack.rotateRad(radians, vecX, vecY, vecZ);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::rotateXRad(float radians) {
	rotateRad(radians, 1, 0, 0);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::rotateYRad(float radians) {
	rotateRad(radians, 0, 1, 0);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::rotateZRad(float radians) {
	rotateRad(radians, 0, 0, 1);
}

//same as ofRotateZ
//----------------------------------------------------------
void ofGLProgrammableRenderer::rotateRad(float radians) {
	rotateZRad(radians);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::matrixMode(ofMatrixMode mode) {
	matrixStack.matrixMode(mode);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::loadIdentityMatrix(void) {
	matrixStack.loadIdentityMatrix();
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::loadMatrix(const glm::mat4 & m) {
	matrixStack.loadMatrix(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::loadMatrix(const float * m) {
	loadMatrix(glm::make_mat4(m));
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::multMatrix(const glm::mat4 & m) {
	matrixStack.multMatrix(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::multMatrix(const float * m) {
	multMatrix(glm::make_mat4(m));
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::loadViewMatrix(const glm::mat4 & m) {
	matrixStack.loadViewMatrix(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::multViewMatrix(const glm::mat4 & m) {
	matrixStack.multViewMatrix(m);
	uploadCurrentMatrix();
}

//----------------------------------------------------------
glm::mat4 ofGLProgrammableRenderer::getCurrentViewMatrix() const {
	return matrixStack.getViewMatrix();
}

//----------------------------------------------------------
glm::mat4 ofGLProgrammableRenderer::getCurrentNormalMatrix() const {
	return glm::transpose(glm::inverse(getCurrentMatrix(OF_MATRIX_MODELVIEW)));
}

//----------------------------------------------------------
glm::mat4 ofGLProgrammableRenderer::getCurrentModelMatrix() const {
	return matrixStack.getModelMatrix();
}

//----------------------------------------------------------
glm::vec3 ofGLProgrammableRenderer::getCurrentEyePosition() const {
	return currentEyePos;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::uploadCurrentMatrix() {
	if (!currentShader) return;
	// uploads the current matrix to the current shader.
	switch (matrixStack.getCurrentMatrixMode()) {
	case OF_MATRIX_MODELVIEW:
		currentShader->setUniformMatrix4f(MODEL_MATRIX_UNIFORM, matrixStack.getModelMatrix());
		currentShader->setUniformMatrix4f(VIEW_MATRIX_UNIFORM, matrixStack.getViewMatrix());
		currentShader->setUniformMatrix4f(MODELVIEW_MATRIX_UNIFORM, matrixStack.getModelViewMatrix());
		currentShader->setUniformMatrix4f(MODELVIEW_PROJECTION_MATRIX_UNIFORM, matrixStack.getModelViewProjectionMatrix());
		if (currentMaterial) {
			currentMaterial->uploadMatrices(*currentShader, *this);
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

//----------------------------------------------------------
glm::mat4 ofGLProgrammableRenderer::getCurrentMatrix(ofMatrixMode matrixMode_) const {
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
		return glm::mat4(1.0);
		break;
	}
}

//----------------------------------------------------------
glm::mat4 ofGLProgrammableRenderer::getCurrentOrientationMatrix() const {
	return matrixStack.getOrientationMatrix();
}
//----------------------------------------------------------
void ofGLProgrammableRenderer::setColor(const ofFloatColor & color) {
	setColor(color.r, color.g, color.b, color.a);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setColor(const ofFloatColor & color, float _a) {
	setColor(color.r, color.g, color.b, _a);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setColor(float _r, float _g, float _b) {
	setColor(_r, _g, _b, 1.f);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setColor(float _r, float _g, float _b, float _a) {
	ofFloatColor newColor(_r, _g, _b, _a);
	if (newColor != currentStyle.color) {
		currentStyle.color = newColor;
		if (currentShader) {
			currentShader->setUniform4f(COLOR_UNIFORM, _r, _g, _b, _a);
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setColor(float gray) {
	setColor(gray, gray, gray);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setHexColor(int hexColor) {
	int r = (hexColor >> 16) & 0xff;
	int g = (hexColor >> 8) & 0xff;
	int b = (hexColor >> 0) & 0xff;
	setColor((float)r / 255.f, (float)g / 255.f, (float)b / 255.f);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setBitmapTextMode(ofDrawBitmapMode mode) {
	currentStyle.drawBitmapMode = mode;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
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
void ofGLProgrammableRenderer::setBackgroundAuto(bool bAuto) {
	bBackgroundAuto = bAuto;
}

//----------------------------------------------------------
bool ofGLProgrammableRenderer::getBackgroundAuto() {
	return bBackgroundAuto;
}

//----------------------------------------------------------
ofFloatColor ofGLProgrammableRenderer::getBackgroundColor() {
	return currentStyle.bgColor;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setBackgroundColor(const ofFloatColor & c) {
	currentStyle.bgColor = c;
	glClearColor(currentStyle.bgColor[0], currentStyle.bgColor[1], currentStyle.bgColor[2], currentStyle.bgColor[3]);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::background(const ofFloatColor & c) {
	setBackgroundColor(c);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::background(float brightness) {
	background(ofFloatColor(brightness));
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::background(int hexColor, int _a) {
	int r = (hexColor >> 16) & 0xff;
	int g = (hexColor >> 8) & 0xff;
	int b = (hexColor >> 0) & 0xff;
	background((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, _a / 255.f);
	//	background ( (hexColor >> 16) & 0xff, (hexColor >> 8) & 0xff, (hexColor >> 0) & 0xff, _a);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::background(float r, float g, float b, float a) {
	background(ofFloatColor(r, g, b, a));
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setFillMode(ofFillFlag fill) {
	currentStyle.bFill = (fill == OF_FILLED);
	if (currentStyle.bFill) {
		path.setFilled(true);
		path.setStrokeWidth(0);
#ifndef TARGET_OPENGLES
		// GLES does not support glPolygonMode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
	} else {
		path.setFilled(false);
		path.setStrokeWidth(currentStyle.lineWidth);
#ifndef TARGET_OPENGLES
		// GLES does not support glPolygonMode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
	}
}

//----------------------------------------------------------
ofFillFlag ofGLProgrammableRenderer::getFillMode() {
	if (currentStyle.bFill) {
		return OF_FILLED;
	} else {
		return OF_OUTLINE;
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setRectMode(ofRectMode mode) {
	currentStyle.rectMode = mode;
}

//----------------------------------------------------------
ofRectMode ofGLProgrammableRenderer::getRectMode() {
	return currentStyle.rectMode;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setLineWidth(float lineWidth) {
	// tig: glLinewidth is 'kind of' deprecated.
	// http://www.opengl.org/registry/doc/glspec32.core.20090803.pdf
	// p.330: "LineWidth values greater than 1.0 will generate an
	// INVALID_VALUE error".
	// use geometry shaders to draw lines of varying thickness...

	currentStyle.lineWidth = lineWidth;
	if (!currentStyle.bFill) {
		path.setStrokeWidth(lineWidth);
	}
	//glLineWidth(lineWidth);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setPointSize(float pointSize) {
	// will use a shader to render the points at different sizes
	currentStyle.pointSize = pointSize;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setDepthTest(bool depthTest) {
	if (depthTest) {
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_DEPTH_TEST);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setLineSmoothing(bool smooth) {
	currentStyle.smoothing = smooth;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::startSmoothing() {
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::endSmoothing() {
	// TODO :: needs ES2 code.
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setBlendMode(ofBlendMode blendMode) {
	switch (blendMode) {
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

	case OF_BLENDMODE_MAX:
		glEnable(GL_BLEND);
#ifdef TARGET_OPENGLES
		ofLogWarning("ofGLProgrammableRenderer") << "OF_BLENDMODE_MAX not currently supported on OpenGL ES";
#else
		glBlendEquation(GL_MAX);
#endif

		break;

	case OF_BLENDMODE_MIN:
		glEnable(GL_BLEND);
#ifdef TARGET_OPENGLES
		ofLogWarning("ofGLProgrammableRenderer") << "OF_BLENDMODE_MIN not currently supported on OpenGL ES";
#else
		glBlendEquation(GL_MIN);
#endif
		break;

	default:
		break;
	}
	currentStyle.blendingMode = blendMode;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::enablePointSprites() {
	pointSpritesEnabled = true;
#ifdef TARGET_OPENGLES
#ifndef TARGET_PROGRAMMABLE_GL
	glEnable(GL_POINT_SPRITE_OES);
#endif
#else
		glEnable(GL_PROGRAM_POINT_SIZE);
#endif
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disablePointSprites() {
#ifdef TARGET_OPENGLES
	#ifndef TARGET_PROGRAMMABLE_GL
	glEnable(GL_POINT_SPRITE_OES);
	#endif
#else
	glDisable(GL_PROGRAM_POINT_SIZE);
#endif
	pointSpritesEnabled = false;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::enableAntiAliasing() {
#if !defined(TARGET_PROGRAMMABLE_GL) || !defined(TARGET_OPENGLES)
	glEnable(GL_MULTISAMPLE);
#endif
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disableAntiAliasing() {
#if !defined(TARGET_PROGRAMMABLE_GL) || !defined(TARGET_OPENGLES)
	glDisable(GL_MULTISAMPLE);
#endif
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::enableLineSizeAttenuation() {
	mBLineSizeAttenutation = true;
}
//----------------------------------------------------------
void ofGLProgrammableRenderer::disableLineSizeAttenuation() {
	mBLineSizeAttenutation = false;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::enableLinesShaders() {
	mBEnableLinesShaders = true;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disableLinesShaders() {
	mBEnableLinesShaders = false;
}

//----------------------------------------------------------
bool ofGLProgrammableRenderer::areLinesShadersEnabled() const {
	return mBEnableLinesShaders;
}

//----------------------------------------------------------
const ofShader & ofGLProgrammableRenderer::getCurrentShader() const {
	return *currentShader;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setAlphaBitmapText(bool bitmapText) {
	bool wasBitmapStringEnabled = bitmapStringEnabled;
	bitmapStringEnabled = bitmapText;

	if (wasBitmapStringEnabled != bitmapText) {
		if (currentShader) currentShader->setUniform1f(BITMAP_STRING_UNIFORM, bitmapText);
	}
}

//----------------------------------------------------------
ofStyle ofGLProgrammableRenderer::getStyle() const {
	return currentStyle;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::pushStyle() {
	styleHistory.push_back(currentStyle);
	//if we are over the max number of styles we have set, then delete the oldest styles.
	if (styleHistory.size() > OF_MAX_STYLE_HISTORY) {
		styleHistory.pop_front();
		//should we warn here?
		ofLogWarning("ofGraphics") << "ofPushStyle(): maximum number of style pushes << " << OF_MAX_STYLE_HISTORY << " reached, did you forget to pop somewhere?";
	}
}

void ofGLProgrammableRenderer::popStyle() {
	if (styleHistory.size()) {
		setStyle(styleHistory.back());
		styleHistory.pop_back();
	}
}

void ofGLProgrammableRenderer::setStyle(const ofStyle & style) {

	//color
	setColor(style.color.r, style.color.g, style.color.b, style.color.a);

	//bg color
	setBackgroundColor(style.bgColor);

	//circle resolution - don't worry it only recalculates the display list if the res has changed
	setCircleResolution(style.circleResolution);

	setCurveResolution(style.curveResolution);

	//line width - finally!
	setLineWidth(style.lineWidth);
	
	setPointSize(style.pointSize);

	//ofSetDepthTest(style.depthTest); removed since it'll break old projects setting depth test through glEnable

	//rect mode: corner/center
	setRectMode(style.rectMode);

	//poly mode: winding type
	setPolyMode(style.polyMode);

	//fill
	if (style.bFill) {
		setFillMode(OF_FILLED);
	} else {
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

void ofGLProgrammableRenderer::setCurveResolution(int resolution) {
	currentStyle.curveResolution = resolution;
	path.setCurveResolution(resolution);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setAttributes(bool vertices, bool color, bool tex, bool normals, GLuint drawMode) {
	bool wasColorsEnabled = colorsEnabled;
	bool wasUsingTexture = texCoordsEnabled & (currentTextureTarget != OF_NO_TEXTURE);
	GLuint prevDrawMode = mDrawMode;

	texCoordsEnabled = tex;
	colorsEnabled = color;
	normalsEnabled = normals;
		
	// nh: we set a variable (mBRenderingLines) before calling set attributes() to disable
	// the setting of the draw mode. When rendering lines, the requested mode may be GL_LINES
	// but we use a shader with GL_TRIANGLES to create a mesh to render the lines of varying widths.
	if( !mBRenderingLines ) {
		mDrawMode = drawMode;
	}
	
	if( mDrawMode == GL_LINE_LOOP ) {
		// this uses the same line resources as GL_LINE_STRIP
		mDrawMode = GL_LINE_STRIP;
	}
	// if we are not using the lines shader, set the draw mode back to triangles
	// mDrawMode is used to switch the shaders in the beginDefaultShader function
	if( !areLinesShadersEnabled() ) {
		mDrawMode = GL_TRIANGLES;
	}
	
	// prevent a shader switch if we don't need it.
	if( mDrawMode != GL_TRIANGLES && mDrawMode != GL_POINTS && mDrawMode != GL_LINES && mDrawMode != GL_LINE_STRIP ) {
		mDrawMode = GL_TRIANGLES;
	}

	if (!uniqueShader || currentMaterial) {
		beginDefaultShader();
	}

	bool usingTexture = tex & (currentTextureTarget != OF_NO_TEXTURE);
	if (wasUsingTexture != usingTexture) {
		if (currentShader) currentShader->setUniform1f(USE_TEXTURE_UNIFORM, usingTexture);
	}
	if (wasColorsEnabled != color) {
		if (currentShader) currentShader->setUniform1f(USE_COLORS_UNIFORM, color);
	}

	// if we switch the draw mode, lets set the textures 
	if( prevDrawMode != mDrawMode ) {
		if (currentTextureTarget != OF_NO_TEXTURE && currentShader ) {
			// set all of the texture uniforms
			// setUniformTexture(const string & name, int textureTarget, GLint textureID, int textureLocation) const {
			for (auto &texUniform : mUniformsTex ) {
				currentShader->setUniformTexture(texUniform.uniformName, texUniform.texData, texUniform.textureLocation);
			}

			if (pointSpritesEnabled && mDrawMode == GL_POINTS && !usingCustomShader && !uniqueShader && !currentMaterial && mUniformsTex.size() > 0) {
				auto &texUniform = mUniformsTex[0];
				#if !defined(TARGET_OPENGLES)
				if (currentTextureTarget == GL_TEXTURE_RECTANGLE_ARB) {
					// set the size of the texture, since gl_PointCoord is normalized
					currentShader->setUniform2f("src_tex_unit0_dims", texUniform.texData.width, texUniform.texData.height);
				}
				#endif
				if (currentTextureTarget == GL_TEXTURE_2D) {
					currentShader->setUniform2f("src_tex_unit0_dims", texUniform.texData.width, texUniform.texData.height);
				}
			}
		}
	}
	
	if (!usingCustomShader && currentShader && !uniqueShader && !currentMaterial) {
		if (mDrawMode == GL_POINTS && pointSpritesEnabled) {
			currentShader->setUniform1f("pointSize", currentStyle.pointSize );
			currentShader->setUniform1f("smoothing", currentStyle.smoothing ? 1.0f : 0.0f );
		} else if( mDrawMode == GL_LINES || mDrawMode == GL_LINE_STRIP || mDrawMode == GL_LINE_LOOP || mBRenderingLines ) {
			auto viewRect = getCurrentViewport();
			glm::vec4 viewRect4(viewRect.x, viewRect.y, viewRect.getWidth(), viewRect.getHeight() );
			currentShader->setUniform4f("viewRect", viewRect4);
			currentShader->setUniform1f("uLineWidth", currentStyle.lineWidth);
			currentShader->setUniform1f("uUsePerspective", mBLineSizeAttenutation ? 1.0f : 0.0f);
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::enableTextureTarget(const ofTexture & tex, int textureLocation) {
	bool bUseTexture = texCoordsEnabled;
	if (mDrawMode == GL_POINTS) {
		// if we are drawing points, we don't need tex coords
		if (currentTextureTarget != OF_NO_TEXTURE) {
			bUseTexture = true;
		}
	}
	bool wasUsingTexture = bUseTexture & (currentTextureTarget != OF_NO_TEXTURE);
	// bool wasUsingTexture = texCoordsEnabled & (currentTextureTarget != OF_NO_TEXTURE);
	currentTextureTarget = tex.texData.textureTarget;

	TextureUniform texUniform;
	texUniform.texData = tex.getTextureData();
	texUniform.uniformName = "src_tex_unit" + ofToString(textureLocation);
	texUniform.textureLocation = textureLocation;
	mUniformsTex.push_back(texUniform);

	if (!uniqueShader || currentMaterial) {
		beginDefaultShader();
	}

	bool usingTexture = bUseTexture & (currentTextureTarget != OF_NO_TEXTURE);
	if (wasUsingTexture != usingTexture) {
		if (currentShader) currentShader->setUniform1f(USE_TEXTURE_UNIFORM, usingTexture);
	}

	if ((currentTextureTarget != OF_NO_TEXTURE) && currentShader) {
		currentShader->setUniformTexture("src_tex_unit" + ofToString(textureLocation), tex, textureLocation);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disableTextureTarget(int textureTarget, int textureLocation) {
	bool wasUsingTexture = texCoordsEnabled & (currentTextureTarget != OF_NO_TEXTURE);
	currentTextureTarget = OF_NO_TEXTURE;
	mUniformsTex.clear();

	if (!uniqueShader || currentMaterial) {
		beginDefaultShader();
	}

	bool usingTexture = texCoordsEnabled & (currentTextureTarget != OF_NO_TEXTURE);
	if (wasUsingTexture != usingTexture) {
		if (currentShader) currentShader->setUniform1f(USE_TEXTURE_UNIFORM, usingTexture);
	}
	glActiveTexture(GL_TEXTURE0 + textureLocation);
	glBindTexture(textureTarget, 0);
	glActiveTexture(GL_TEXTURE0);
}

//----------------------------------------------------------
GLenum ofGLProgrammableRenderer::getCurrentTextureTarget() {
	return currentTextureTarget;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setAlphaMaskTex(const ofTexture & tex) {
	alphaMaskTextureTarget = tex.getTextureData().textureTarget;
	if (alphaMaskTextureTarget == GL_TEXTURE_2D) {
		alphaMask2DShader.begin();
	} else {
		alphaMaskRectShader.begin();
	}
	enableTextureTarget(tex, 1);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disableAlphaMask() {
	disableTextureTarget(alphaMaskTextureTarget, 1);
	if (alphaMaskTextureTarget == GL_TEXTURE_2D) {
		alphaMask2DShader.end();
	} else {
		alphaMaskRectShader.end();
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofShader & shader) {
	if (currentShader && *currentShader == shader) {
		return;
	}

	if (bIsShadowDepthPass) {
		// if we are not the shadow shader, lets unbind it
		if (!settingDefaultShader && currentShadow && currentShader) {
			// lets assume it's bound?
			if (!bCustomShadowShader) {
				glUseProgram(0);
			}
			// we are assuming that since it's a custom depth shader, it all will be taken care of ...
			bCustomShadowShader = true;
		}
	}

	glUseProgram(shader.getProgram());

	currentShader = &shader;
	uploadMatrices();
	setDefaultUniforms();
	if (!settingDefaultShader) {
		usingCustomShader = true;
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofShader & shader) {
	glUseProgram(0);
	usingCustomShader = false;
	bCustomShadowShader = false;
	beginDefaultShader();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::begin(const ofFbo & fbo, ofFboMode mode) {
	pushView();
	pushStyle();
	if (mode & OF_FBOMODE_MATRIXFLIP) {
		matrixStack.setRenderSurface(fbo);
	} else {
		matrixStack.setRenderSurfaceNoMatrixFlip(fbo);
	}
	viewport();
	if (mode & OF_FBOMODE_PERSPECTIVE) {
		setupScreenPerspective();
	} else {
		uploadMatrices();
	}
	bind(fbo);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::end(const ofFbo & fbo) {
	unbind(fbo);
	matrixStack.setRenderSurface(*window);
	uploadMatrices();
	popStyle();
	popView();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofFbo & fbo) {
	if (currentFramebufferId == fbo.getId()) {
		ofLogWarning() << "Framebuffer with id: " << fbo.getId() << " cannot be bound onto itself. \n"
					   << "Most probably you forgot to end() the current framebuffer before calling begin() again or you forgot to allocate() before calling begin().";
		return;
	}
	// this method could just as well have been placed in ofBaseGLRenderer
	// and shared over both programmable and fixed function renderer.
	// I'm keeping it here, so that if we want to do more fancyful
	// named framebuffers with GL 4.5+, we can have
	// different implementations.
	framebufferIdStack.push_back(currentFramebufferId);
	currentFramebufferId = fbo.getId();
	glBindFramebuffer(GL_FRAMEBUFFER, currentFramebufferId);
}

#ifndef TARGET_OPENGLES
//----------------------------------------------------------
void ofGLProgrammableRenderer::bindForBlitting(const ofFbo & fboSrc, ofFbo & fboDst, int attachmentPoint) {
	if (currentFramebufferId == fboSrc.getId()) {
		ofLogWarning() << "Framebuffer with id: " << fboSrc.getId() << " cannot be bound onto itself. \n"
					   << "Most probably you forgot to end() the current framebuffer before calling getTexture().";
		return;
	}
	// this method could just as well have been placed in ofBaseGLRenderer
	// and shared over both programmable and fixed function renderer.
	// I'm keeping it here, so that if we want to do more fancyful
	// named framebuffers with GL 4.5+, we can have
	// different implementations.
	framebufferIdStack.push_back(currentFramebufferId);
	currentFramebufferId = fboSrc.getId();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, currentFramebufferId);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentPoint);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboDst.getIdDrawBuffer());
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachmentPoint);
}
#endif

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofFbo & fbo) {
	if (framebufferIdStack.empty()) {
		ofLogError() << "unbalanced fbo bind/unbind binding default framebuffer";
		currentFramebufferId = defaultFramebufferId;
	} else {
		currentFramebufferId = framebufferIdStack.back();
		framebufferIdStack.pop_back();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, currentFramebufferId);
	fbo.flagDirty();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofBaseMaterial & material) {
	if (bIsShadowDepthPass && !material.hasDepthShader()) {
		//		currentMaterial = nullptr;
		// we are the shadow depth pass right now, we don't need
		// textures or lighting, etc.
		return;
	}
	currentMaterial = &material;
	// FIXME: this invalidates the previous shader to avoid that
	// when binding 2 materials one after another, the second won't
	// get the right parameters.
	currentMaterial->bind(*this);
	currentShader = nullptr;
	beginDefaultShader();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofShadow & shadow) {
	currentShadow = &shadow;
	bIsShadowDepthPass = true;
	beginDefaultShader();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofShadow & shadow, GLenum aCubeFace) {
	shadowCubeFace = aCubeFace;
	bind(shadow);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofBaseMaterial &) {
	if (currentMaterial) {
		currentMaterial->unbind(*this);
	}
	currentMaterial = nullptr;
	if (bIsShadowDepthPass) {
		// we are the shadow depth pass right now, we don't need
		// textures or lighting, etc.
		return;
	}
	beginDefaultShader();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofShadow & shadow) {
	currentShadow = nullptr;
	bIsShadowDepthPass = false;
	bCustomShadowShader = false;
	beginDefaultShader();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofShadow & shadow, GLenum aCubeFace) {
	unbind(shadow);
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::enableLighting() {
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disableLighting() {
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::enableLight(int) {
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::disableLight(int) {
}

//----------------------------------------------------------
bool ofGLProgrammableRenderer::getLightingEnabled() {
	return true;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofTexture & texture, int location) {
	//we could check if it has been allocated - but we don't do that in draw()
	if (texture.getAlphaMask()) {
		setAlphaMaskTex(*texture.getAlphaMask());
	}
	enableTextureTarget(texture, location);

	if (ofGetUsingNormalizedTexCoords()) {
		matrixMode(OF_MATRIX_TEXTURE);
		pushMatrix();
		glm::mat4 m = glm::mat4(1.0);

#ifndef TARGET_OPENGLES
		if (texture.texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB)
			m = glm::scale(m, glm::vec3(texture.texData.width, texture.texData.height, 1.0f));
		else
#endif
			m = glm::scale(m, glm::vec3(texture.texData.width / texture.texData.tex_w, texture.texData.height / texture.texData.tex_h, 1.0f));

		loadMatrix(m);
		matrixMode(OF_MATRIX_MODELVIEW);
	}
	if (texture.isUsingTextureMatrix()) {
		matrixMode(OF_MATRIX_TEXTURE);
		if (!ofGetUsingNormalizedTexCoords()) pushMatrix();
		multMatrix(texture.getTextureMatrix());
		matrixMode(OF_MATRIX_MODELVIEW);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofTexture & texture, int location) {
	disableTextureTarget(texture.texData.textureTarget, location);
	if (texture.getAlphaMask()) {
		disableAlphaMask();
	}

	if (texture.isUsingTextureMatrix() || ofGetUsingNormalizedTexCoords()) {
		matrixMode(OF_MATRIX_TEXTURE);
		popMatrix();
		matrixMode(OF_MATRIX_MODELVIEW);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::bind(const ofCamera & camera, const ofRectangle & _viewport) {
	pushView();
	viewport(_viewport);
	setOrientation(matrixStack.getOrientation(), camera.isVFlipped());
	matrixMode(OF_MATRIX_PROJECTION);
	loadMatrix(camera.getProjectionMatrix(_viewport));
	matrixMode(OF_MATRIX_MODELVIEW);
	loadViewMatrix(camera.getModelViewMatrix());
	currentEyePos = camera.getPosition();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::unbind(const ofCamera & camera) {
	popView();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::uploadMatrices() {
	if (!currentShader) return;
	currentShader->setUniformMatrix4f(MODEL_MATRIX_UNIFORM, matrixStack.getModelMatrix());
	currentShader->setUniformMatrix4f(VIEW_MATRIX_UNIFORM, matrixStack.getViewMatrix());
	currentShader->setUniformMatrix4f(MODELVIEW_MATRIX_UNIFORM, matrixStack.getModelViewMatrix());
	currentShader->setUniformMatrix4f(PROJECTION_MATRIX_UNIFORM, matrixStack.getProjectionMatrix());
	currentShader->setUniformMatrix4f(TEXTURE_MATRIX_UNIFORM, matrixStack.getTextureMatrix());
	currentShader->setUniformMatrix4f(MODELVIEW_PROJECTION_MATRIX_UNIFORM, matrixStack.getModelViewProjectionMatrix());
	if (currentMaterial) {
		currentMaterial->uploadMatrices(*currentShader, *this);
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::setDefaultUniforms() {
	if (!currentShader) return;
	currentShader->setUniform4f(COLOR_UNIFORM, currentStyle.color.r, currentStyle.color.g, currentStyle.color.b, currentStyle.color.a);
	bool usingTexture = texCoordsEnabled & (currentTextureTarget != OF_NO_TEXTURE);
	currentShader->setUniform1f(USE_TEXTURE_UNIFORM, usingTexture);
	currentShader->setUniform1f(USE_COLORS_UNIFORM, colorsEnabled);
	if (currentMaterial) {
		currentMaterial->updateMaterial(*currentShader, *this);
		currentMaterial->updateLights(*currentShader, *this);
		currentMaterial->updateShadows(*currentShader, *this);
		currentMaterial->updateEnvironmentMaps(*currentShader, *this);
	}
	if (currentShadow) {
		if (currentShadow->isMultiCubeFacePass()) {
			currentShadow->updateDepth(*currentShader, shadowCubeFace, *this);
		} else {
			currentShadow->updateDepth(*currentShader, *this);
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::beginDefaultShader() {
	if (usingCustomShader && !currentMaterial && !currentShadow) return;
	if (currentShadow && bCustomShadowShader) return;

	const ofShader * nextShader = nullptr;

	bool bUseTexture = texCoordsEnabled;
	if( mDrawMode == GL_POINTS ){
		// if we are drawing points, we don't need tex coords and would like to use the texture
		if (currentTextureTarget != OF_NO_TEXTURE ) {
			bUseTexture = true;
		}
	}

	if (!uniqueShader || currentMaterial || currentShadow)
	{
		if (currentShadow) {
			if (currentMaterial && currentMaterial->hasDepthShader()) {
				nextShader = &currentMaterial->getShadowDepthShader(*currentShadow, *this);
			} else {
				nextShader = &currentShadow->getDepthShader(*this);
			}
		} else if (currentMaterial) {
			nextShader = &currentMaterial->getShader(currentTextureTarget, colorsEnabled, *this);
		} else if (bitmapStringEnabled) {
			nextShader = &bitmapStringShader;

		} else if (colorsEnabled && bUseTexture) {
			auto &shaderCollection = getShaderCollectionForMode(mDrawMode);
			
			switch (currentTextureTarget)
			{
#ifndef TARGET_OPENGLES
			case GL_TEXTURE_RECTANGLE_ARB:
				nextShader = &shaderCollection->texRectColor;
				// nextShader = &defaultTexRectColor;
				break;
#endif
			case GL_TEXTURE_2D:
				nextShader = &shaderCollection->tex2DColor;
				// nextShader = &defaultTex2DColor;
				break;
			case OF_NO_TEXTURE:
				nextShader = &shaderCollection->noTexColor;
				// nextShader = &defaultNoTexColor;
				break;
#ifdef TARGET_ANDROID
			case GL_TEXTURE_EXTERNAL_OES:
				nextShader = &defaultOESTexColor;
				break;
#endif
			}
		} else if (colorsEnabled) {
			nextShader = &getShaderCollectionForMode(mDrawMode)->noTexColor;
			// nextShader = &defaultNoTexColor;
		} else if (bUseTexture) {
			auto &shaderCollection = getShaderCollectionForMode(mDrawMode);
			switch (currentTextureTarget) {
#ifndef TARGET_OPENGLES
			case GL_TEXTURE_RECTANGLE_ARB:
				nextShader = &shaderCollection->texRectNoColor;
				// nextShader = &defaultTexRectNoColor;
				break;
#endif
			case GL_TEXTURE_2D:
				nextShader = &shaderCollection->tex2DNoColor;
				// nextShader = &defaultTex2DNoColor;
				break;
			case OF_NO_TEXTURE:
				nextShader = &shaderCollection->noTexNoColor;
				// nextShader = &defaultNoTexNoColor;
				break;
#ifdef TARGET_ANDROID
			case GL_TEXTURE_EXTERNAL_OES:
				nextShader = &defaultOESTexNoColor;
				break;
#endif
			}
		} else {
			nextShader = &getShaderCollectionForMode(mDrawMode)->noTexNoColor;
			// nextShader = &defaultNoTexNoColor;
		}
	} else {
		nextShader = &defaultUniqueShader;
	}

	if (nextShader) {
		if (!currentShader || *currentShader != *nextShader) {
			settingDefaultShader = true;
			bind(*nextShader);
			settingDefaultShader = false;
		}
	}
}

//----------------------------------------------------------
std::shared_ptr<ofGLProgrammableRenderer::ShaderCollection>& ofGLProgrammableRenderer::getShaderCollectionForMode(GLuint drawMode) {
	if( drawMode == GL_POINTS ) {
		return mDefaultShadersMap[GL_POINTS];
	} else if( drawMode == GL_LINES ) {
		return mDefaultShadersMap[GL_LINES];
	} else if( drawMode == GL_LINE_STRIP || drawMode == GL_LINE_LOOP ) {
		return mDefaultShadersMap[GL_LINE_STRIP];
	}
	return mDefaultShadersMap[GL_TRIANGLES];
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const {
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer *>(this);
	lineMesh.getVertices()[0] = { x1, y1, z1 };
	lineMesh.getVertices()[1] = { x2, y2, z2 };

	// use smoothness, if requested:
	if (currentStyle.smoothing) mutThis->startSmoothing();

	draw(lineMesh, OF_MESH_FILL, false, false, false);

	// use smoothness, if requested:
	if (currentStyle.smoothing) mutThis->endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawRectangle(float x, float y, float z, float w, float h) const {
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer *>(this);
	if (currentStyle.rectMode == OF_RECTMODE_CORNER) {
		rectMesh.getVertices()[0] = { x, y, z };
		rectMesh.getVertices()[1] = { x + w, y, z };
		rectMesh.getVertices()[2] = { x + w, y + h, z };
		rectMesh.getVertices()[3] = { x, y + h, z };
	} else {
		rectMesh.getVertices()[0] = { x - w / 2.0f, y - h / 2.0f, z };
		rectMesh.getVertices()[1] = { x + w / 2.0f, y - h / 2.0f, z };
		rectMesh.getVertices()[2] = { x + w / 2.0f, y + h / 2.0f, z };
		rectMesh.getVertices()[3] = { x - w / 2.0f, y + h / 2.0f, z };
	}

	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->startSmoothing();

	rectMesh.setMode(currentStyle.bFill ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_LOOP);
	draw(rectMesh, OF_MESH_FILL, false, false, false);

	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const {
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer *>(this);
	triangleMesh.getVertices()[0] = { x1, y1, z1 };
	triangleMesh.getVertices()[1] = { x2, y2, z2 };
	triangleMesh.getVertices()[2] = { x3, y3, z3 };

	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->startSmoothing();

	triangleMesh.setMode(currentStyle.bFill ? OF_PRIMITIVE_TRIANGLE_STRIP : OF_PRIMITIVE_LINE_LOOP);
	draw(triangleMesh, OF_MESH_FILL, false, false, false);

	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawCircle(float x, float y, float z, float radius) const {
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer *>(this);
	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->startSmoothing();
		
	if( !currentStyle.bFill ) {
		// nh: We use the circleOutlineMesh to render a closed polyline
		// since the lines mesh creation and shader depends on it.
		const auto & circleCache = circleOutlinePolyline.getVertices();
		for (int i = 0; i < (int)circleCache.size(); i++) {
			circleOutlineMesh.getVertices()[i] = { radius * circleCache[i].x + x, radius * circleCache[i].y + y, z };
		}
		circleOutlineMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
		draw(circleOutlineMesh, OF_MESH_FILL, false, false, false);
	} else {
		const auto & circleCache = circlePolyline.getVertices();
		for (int i = 0; i < (int)circleCache.size(); i++) {
			circleMesh.getVertices()[i] = { radius * circleCache[i].x + x, radius * circleCache[i].y + y, z };
		}
		circleMesh.setMode(currentStyle.bFill ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_STRIP);
		draw(circleMesh, OF_MESH_FILL, false, false, false);
	}
	
//	circleMesh.setMode(currentStyle.bFill ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_STRIP);
//	draw(circleMesh, OF_MESH_FILL, false, false, false);

	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawEllipse(float x, float y, float z, float width, float height) const {
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer *>(this);
	float radiusX = width * 0.5;
	float radiusY = height * 0.5;
//	const auto & circleCache = circlePolyline.getVertices();
//	for (int i = 0; i < (int)circleCache.size(); i++) {
//		circleMesh.getVertices()[i] = { radiusX * circlePolyline[i].x + x, radiusY * circlePolyline[i].y + y, z };
//	}

	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->startSmoothing();
	
	if( !currentStyle.bFill ) {
		// nh: We use the circleOutlineMesh to render a closed polyline
		// since the lines mesh creation and shader depends on it.
		const auto & circleCache = circleOutlinePolyline.getVertices();
		for (int i = 0; i < (int)circleCache.size(); i++) {
			circleOutlineMesh.getVertices()[i] = { radiusX * circleCache[i].x + x, radiusY * circleCache[i].y + y, z };
		}
		circleOutlineMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
		draw(circleOutlineMesh, OF_MESH_FILL, false, false, false);
	} else {
		const auto & circleCache = circlePolyline.getVertices();
		for (int i = 0; i < (int)circleCache.size(); i++) {
			circleMesh.getVertices()[i] = { radiusX * circleCache[i].x + x, radiusY * circleCache[i].y + y, z };
		}
		circleMesh.setMode(currentStyle.bFill ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_STRIP);
		draw(circleMesh, OF_MESH_FILL, false, false, false);
	}

//	circleMesh.setMode(currentStyle.bFill ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_STRIP);
//	draw(circleMesh, OF_MESH_FILL, false, false, false);

	// use smoothness, if requested:
	if (currentStyle.smoothing && !currentStyle.bFill) mutThis->endSmoothing();
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawString(string textString, float x, float y, float z) const {
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer *>(this);
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
	glm::mat4 modelView = glm::mat4(1.0);

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

		modelView = glm::translate(modelView, glm::vec3(-1, -1, 0));
		modelView = glm::scale(modelView, glm::vec3(2 / rViewport.width, 2 / rViewport.height, 1));
		modelView = glm::translate(modelView, glm::vec3(x, y, 0));
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

		modelView = glm::translate(modelView, glm::vec3(-1, -1, 0));
		modelView = glm::scale(modelView, glm::vec3(2 / rViewport.width, 2 / rViewport.height, 1));
		modelView = glm::translate(modelView, glm::vec3(x, y, 0));
		mutThis->loadMatrix(modelView);
		break;

	case OF_BITMAPMODE_MODEL:

		hasModelView = true;
		mutThis->matrixMode(OF_MATRIX_MODELVIEW);
		mutThis->pushMatrix();

		mutThis->translate(x, y, z);
		break;

	case OF_BITMAPMODE_MODEL_BILLBOARD: {
		//our aim here is to draw to screen
		//at the viewport position related
		//to the world position x,y,z

		// tig: we want to get the signed normalised screen coordinates (-1,+1) of our point (x,y,z)
		// that's projection * modelview * point in GLSL multiplication order
		// then doing the good old (v + 1.0) / 2. to get unsigned normalized screen (0,1) coordinates.
		// we then multiply x by width and y by height to get window coordinates.

		rViewport = getCurrentViewport();

		glm::mat4 mat = matrixStack.getProjectionMatrixNoOrientation() * matrixStack.getModelViewMatrix();
		glm::vec4 dScreen4 = mat * glm::vec4(x, y, z, 1.0);
		glm::vec3 dScreen = glm::vec3(dScreen4) / dScreen4.w;
		dScreen += glm::vec3(1.0);
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

		modelView = glm::translate(modelView, glm::vec3(-1, -1, 0));
		modelView = glm::scale(modelView, glm::vec3(2 / rViewport.width, 2 / rViewport.height, 1));
		modelView = glm::translate(modelView, glm::vec3(dScreen.x, dScreen.y, 0));
		mutThis->loadMatrix(modelView);
	} break;

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
	mutThis->bind(bitmapFont.getTexture(), 0);
	draw(charMesh, OF_MESH_FILL, false, true, false);
	mutThis->unbind(bitmapFont.getTexture(), 0);
	mutThis->setAlphaBitmapText(false);

	if (hasViewport) {
		mutThis->popView();
	} else {
		if (hasModelView) {
			mutThis->popMatrix();
		}

		if (hasProjection) {
			mutThis->matrixMode(OF_MATRIX_PROJECTION);
			mutThis->popMatrix();
			mutThis->matrixMode(OF_MATRIX_MODELVIEW);
		}
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::drawString(const ofTrueTypeFont & font, string text, float x, float y) const {
	ofGLProgrammableRenderer * mutThis = const_cast<ofGLProgrammableRenderer *>(this);
	ofBlendMode blendMode = currentStyle.blendingMode;

	mutThis->setBlendMode(OF_BLENDMODE_ALPHA);

	mutThis->bind(font.getFontTexture(), 0);
	draw(font.getStringMesh(text, x, y, isVFlipped()), OF_MESH_FILL);
	mutThis->unbind(font.getFontTexture(), 0);

	mutThis->setBlendMode(blendMode);
}

// clang-format off
#define STRINGIFY(x) #x


// ----------------------------------------------------------------------
// tig: GLSL #150 shaders written against spec:
// http://www.opengl.org/registry/doc/GLSLangSpec.1.50.09.pdf

#ifdef TARGET_OPENGLES
static const string vertex_shader_header =
		"%extensions%\n"
		"precision highp float;\n"
		"#define IN attribute\n"
		"#define OUT varying\n"
		"#define TEXTURE texture2D\n"
		"#define TARGET_OPENGLES\n";
static const string fragment_shader_header =
		"%extensions%\n"
		"precision highp float;\n"
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
static const std::string defaultPointsVertexShader = vertex_shader_header +
STRINGIFY(
		  uniform mat4 projectionMatrix;
		  uniform mat4 modelViewMatrix;
		  uniform mat4 textureMatrix;
		  uniform mat4 modelViewProjectionMatrix;
		  
		  uniform float pointSize;
		  
		  IN vec4  position;
		  IN vec2  texcoord;
		  IN vec4  color;
		  IN vec3  normal;
		  
		  OUT vec4 colorVarying;
		  
		  void main()
		  {
			  gl_PointSize = pointSize;
			  colorVarying = color;
			  gl_Position = modelViewProjectionMatrix * position;
		  }
);

// ----------------------------------------------------------------------
static const std::string defaultLinesVertexShader = vertex_shader_header +
STRINGIFY(
		  uniform mat4 projectionMatrix;\n
		  uniform mat4 modelViewMatrix;\n
		  uniform mat4 textureMatrix;\n
		  uniform mat4 modelViewProjectionMatrix;\n
		  
		  uniform vec4 viewRect;\n
		  uniform float uLineWidth;\n
		  uniform float uUsePerspective;\n
		  
		  IN vec4 position;
		  IN vec2 texcoord;
		  IN vec4 color;
		  IN vec3 normal;
		  IN vec4 nextVertex;
		  
		  OUT vec4 colorVarying;
		  OUT vec2 texCoordVarying;
		  
		  void main() {
			  colorVarying = color;
			  float pushDir = nextVertex.w;
			  texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y, 0.0,1.0)).xy;
			  
			  // clip space
			  vec4 cClipPos = modelViewProjectionMatrix * vec4(position.xyz, 1.0);
			  vec4 nClipPos = modelViewProjectionMatrix * vec4(nextVertex.xyz, 1.0);
			  
			  vec2 cNdcPos = (cClipPos.xy / cClipPos.w);
			  vec2 nNdcPos = (nClipPos.xy / nClipPos.w);
			  /*
			  * convert to screen space based on
			  * https://stackoverflow.com/questions/3484260/opengl-line-width
			   */
			  cNdcPos = (cNdcPos + 1.0) * (0.5 * viewRect.zw);
			  nNdcPos = (nNdcPos + 1.0) * (0.5 * viewRect.zw);
			  
			  float thickness = uLineWidth * 0.5;
			 
			  
			  vec2 dir = normalize(cNdcPos - nNdcPos);
			  dir = vec2(-dir.y, dir.x);
			  dir = dir * thickness * pushDir;
			  
			  vec4 posScreen = cClipPos;
			  posScreen.xy = cNdcPos;
			  posScreen.xy += dir;
			  posScreen.xy = posScreen.xy / viewRect.zw * 2.0 - 1.0;
			  posScreen.xy *= posScreen.w;
			  
			  // perspective
			  float vaspect = viewRect.w / viewRect.z;
			  dir.x *= vaspect;
			  vec4 posPersp = cClipPos;
			  posPersp.xy += dir;
			  
			  gl_Position = mix( posScreen, posPersp, uUsePerspective );
		  }
);

// ----------------------------------------------------------------------
static const std::string defaultLineStripVertexShader = vertex_shader_header +
STRINGIFY(
		  uniform mat4 projectionMatrix;\n
		  uniform mat4 modelViewMatrix;\n
		  uniform mat4 textureMatrix;
		  uniform mat4 modelViewProjectionMatrix;
		  
		  uniform vec4 viewRect;
		  uniform float uLineWidth;
		  uniform float uUsePerspective;
		  
		  IN vec4 position;
		  IN vec2 texcoord;
		  IN vec4 color;
		  IN vec3 normal;
		  IN vec4 prevVertex;
		  IN vec4 nextVertex;
		  
		  OUT vec4 colorVarying;
		  OUT vec2 texCoordVarying;
		  
		  float mapClamp( float value, float inMin, float inMax, float outMin, float outMax ) {
			  float outVal = ( (value - inMin ) / ( inMax - inMin ) * ( outMax - outMin ) ) + outMin;
			  if(outMax < outMin){
				  if( outVal < outMax ) outVal = outMax;
				  else if( outVal > outMin )outVal = outMin;
			  }else{
				  if( outVal > outMax ) outVal = outMax;
				  else if( outVal < outMin ) outVal = outMin;
			  }
			  return outVal;
		  }
		  
		  void main() {
			  colorVarying = color;
			  texCoordVarying = (textureMatrix*vec4(texcoord.x, texcoord.y, 0.0,1.0)).xy;
			  
			  float thickness = uLineWidth;// * 0.5;
			  float pushDir = nextVertex.w;
			  float pushDirY = -prevVertex.w;
			  
			  // 	// clip space
			  vec4 pClipPos = modelViewProjectionMatrix * vec4(prevVertex.xyz, 1.0);
			  vec4 cClipPos = modelViewProjectionMatrix * vec4(position.xyz, 1.0);
			  vec4 nClipPos = modelViewProjectionMatrix * vec4(nextVertex.xyz, 1.0);
			  
			  vec2 pNdcPos = (pClipPos.xy / pClipPos.w);
			  vec2 cNdcPos = (cClipPos.xy / cClipPos.w);
			  vec2 nNdcPos = (nClipPos.xy / nClipPos.w);
			  
			  // // convert to screen space
			  pNdcPos = (pNdcPos + 1.0) * (0.5 * viewRect.zw);
			  cNdcPos = (cNdcPos + 1.0) * (0.5 * viewRect.zw);
			  nNdcPos = (nNdcPos + 1.0) * (0.5 * viewRect.zw);
			  
			  vec2 dir = vec2(1.0, 0.0);
			  if( position.xyz == nextVertex.xyz ) {
				  // this is the last point ( on a non-closed line )
				  dir = normalize( cNdcPos - pNdcPos );
				  dir = vec2(-dir.y, dir.x);
				  dir = dir * pushDir;
			  } else if( position.xyz == prevVertex.xyz ) {
				  // this is the first point ( on a non-closed line )
				  dir = normalize( nNdcPos - cNdcPos );
				  dir = vec2(-dir.y, dir.x);
				  dir = dir * pushDir;
			  } else {
				  vec2 pdir = normalize( cNdcPos-pNdcPos );
				  vec2 ndir = normalize( nNdcPos-cNdcPos );
				  vec2 tangent = normalize(pdir+ndir);
				  
				  // Miter code based on
				  // https://blog.scottlogic.com/2019/11/18/drawing-lines-with-webgl.html
				  // by Matt Stobbs
				  
				  vec2 miter = vec2(-tangent.y, tangent.x);
				  vec2 normalA = vec2(-pdir.y, pdir.x);
				  dir = miter;
				  float miterLength = 1.0 / clamp(abs(dot(miter, normalA)), 0.01, 2.0);
				  thickness = miterLength * thickness;
				  
				  vec2 point = normalize(pdir-ndir);
				  float dmp = dot(miter, point);
				  
//				  float miterTooLongMix = clamp(step( 2.0, miterLength) * sign(pushDir * dmp), 0.0, 1.0);
				  float miterTooLongMix = clamp(step( 2.0, miterLength), 0.0, 1.0);
				  float pstr = mapClamp( miterLength, 2.0, 3.0, 0.0, 1.0 );
				  float thicknessL = mix( miterLength * uLineWidth, uLineWidth, pstr );
				  
				  dir = mix( dir * pushDir, (pushDir * pushDirY) * normalA, pstr * miterTooLongMix );
				  thickness = mix( clamp(miterLength, 0.0, 3.0) * uLineWidth, thicknessL, miterTooLongMix );
			  }
			  			  
			  dir = dir * (thickness * 0.5 );
			  			  
			  // we calculate both the screen pos and perspective to avoid an if statement
			  vec4 posScreen = cClipPos;
			  posScreen.xy = cNdcPos;
			  posScreen.xy += dir;// * 0.5;
			  posScreen.xy = posScreen.xy / viewRect.zw * 2.0 - 1.0;
			  posScreen.xy *= posScreen.w;
			  
			  // perspective
			  vec4 posPersp = cClipPos;
			  float vaspect = viewRect.w / viewRect.z;
			  dir.x *= vaspect;
			  posPersp.xy += (dir);
			  
			  gl_Position = mix( posScreen, posPersp, uUsePerspective );
		  }
		  );



// ----------------------------------------------------------------------
static const string defaultFragmentShaderPoints =
R"(
#if defined(OF_USING_TEXTURE_2D)
		  uniform sampler2D src_tex_unit0;
#endif
#if defined(OF_USING_TEXTURE_RECT)
		  uniform sampler2DRect src_tex_unit0;
#endif
#if defined(OF_USING_TEXTURE_2D) || defined(OF_USING_TEXTURE_RECT)
		uniform vec2 src_tex_unit0_dims;
	#if !defined(OF_USING_TEXTURE)
		#define OF_USING_TEXTURE 1
	#endif
#endif
		uniform float smoothing;

		  uniform vec4 globalColor;
		  uniform float usingTexture;
			uniform float usingColors;
		  
			IN vec4 colorVarying;
		  void main(){

			vec2 centerPt = gl_PointCoord - vec2(0.5, 0.5);
			float dist_sq = dot( centerPt, centerPt );

			vec2 st = gl_PointCoord;
			
			#if defined(OF_USING_TEXTURE_RECT)
				st.x = gl_PointCoord.x * src_tex_unit0_dims.x;
				st.y = gl_PointCoord.y * src_tex_unit0_dims.y;

				// unfortunately textureSize is not supported for sampler2DRect on older versions of GLSL
				// ivec2 texRectSize = textureSize(src_tex_unit0, 0);
				// st = vec2(gl_PointCoord.x * float(texRectSize.x), gl_PointCoord.y * float(texRectSize.y) );
			#endif

#if defined(OF_USING_TEXTURE) && defined(OF_USING_VERTEX_COLORS)
			  FRAG_COLOR = TEXTURE(src_tex_unit0, st) * colorVarying;
#elif defined(OF_USING_TEXTURE)
			  FRAG_COLOR = TEXTURE(src_tex_unit0, st) * globalColor;
#elif defined(OF_USING_VERTEX_COLORS)
			  FRAG_COLOR = colorVarying;
#else
			  FRAG_COLOR = globalColor;
#endif
			// fwidth is not availabe on some opengl es
	#ifndef TARGET_OPENGLES
			//smoothing with fwidth based on
			// https://rubendv.be/posts/fwidth/
			float delta = fwidth( dist_sq );
//			//float delta = abs(dFdx(dist_sq)) + abs(dFdy(dist_sq));
			FRAG_COLOR.a *= clamp( (1.0-smoothing) + 1.0-(smoothstep( 0.25-delta, 0.25, dist_sq )), 0.0, 1.0);
	#endif
		  }
)";

// ----------------------------------------------------------------------
static const string defaultFragmentShaderLines =
R"(
#if defined(OF_USING_TEXTURE_2D)
	uniform sampler2D src_tex_unit0;
#endif
#if defined(OF_USING_TEXTURE_RECT)
	uniform sampler2DRect src_tex_unit0;
#endif
#if defined(OF_USING_TEXTURE_2D) || defined(OF_USING_TEXTURE_RECT)
 #if !defined(OF_USING_TEXTURE)
  #define OF_USING_TEXTURE 1
 #endif
#endif

	uniform vec4 globalColor;
	uniform float usingTexture;
   uniform float usingColors;

	IN vec2 texCoordVarying;
   IN vec4 colorVarying;

	void main(){
		#if defined(OF_USING_TEXTURE) && defined(OF_USING_VERTEX_COLORS)
			 FRAG_COLOR = TEXTURE(src_tex_unit0, texCoordVarying) * colorVarying;
		#elif defined(OF_USING_TEXTURE)
			 FRAG_COLOR = TEXTURE(src_tex_unit0, texCoordVarying) * globalColor;
		#elif defined(OF_USING_VERTEX_COLORS)
			 FRAG_COLOR = colorVarying;
		#else
			 FRAG_COLOR = globalColor;
		#endif
	}

)";

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

static const string defaultFragmentShaderOESTexNoColor = fragment_shader_header + STRINGIFY(
	
	uniform samplerExternalOES src_tex_unit0;
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

static const string defaultFragmentShaderOESTexColor = fragment_shader_header + STRINGIFY(
																							
	uniform samplerExternalOES src_tex_unit0;
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
		if(usingColors>.5) colorVarying = color;
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

// clang-format on

static string defaultShaderHeader(string header, GLenum textureTarget, int major, int minor) {
	ofStringReplace(header, "%glsl_version%", ofGLSLVersionFromGL(major, minor));
#ifndef TARGET_OPENGLES
	if (major < 4 && minor < 2) {
		ofStringReplace(header, "%extensions%", "#extension GL_ARB_texture_rectangle : enable");
	} else {
		ofStringReplace(header, "%extensions%", "");
	}
#else
	ofStringReplace(header, "%extensions%", "#extension GL_OES_standard_derivatives : enable");
#endif
	if (textureTarget == GL_TEXTURE_2D) {
		header += "#define SAMPLER sampler2D\n";
	} else {
		header += "#define SAMPLER sampler2DRect\n";
	}
	return header;
}

static string shaderSource(const string & src, int major, int minor) {
	string shaderSrc = src;
	ofStringReplace(shaderSrc, "%glsl_version%", ofGLSLVersionFromGL(major, minor));
#ifndef TARGET_OPENGLES
	if (major < 4 && minor < 2) {
		ofStringReplace(shaderSrc, "%extensions%", "#extension GL_ARB_texture_rectangle : enable");
	} else {
		ofStringReplace(shaderSrc, "%extensions%", "");
	}
#else
	ofStringReplace(shaderSrc, "%extensions%", "");
#endif
	return shaderSrc;
}

#ifdef TARGET_ANDROID
static string shaderOESSource(const string & src, int major, int minor) {
	string shaderSrc = src;
	ofStringReplace(shaderSrc, "%glsl_version%", ofGLSLVersionFromGL(major, minor));
	ofStringReplace(shaderSrc, "%extensions%", "#extension GL_OES_EGL_image_external : require");
	return shaderSrc;
}
#endif

static string videoFragmentShaderSource(const ofBaseVideoDraws & video, int major, int minor) {
	string src;
	switch (video.getPixelFormat()) {
	case OF_PIXELS_YUY2:
		src = FRAGMENT_SHADER_YUY2;
#ifndef TARGET_OPENGLES
		ofStringReplace(src, "%g", "g");
#else
		ofStringReplace(src, "%g", "a");
#endif
		break;
	case OF_PIXELS_NV12:
		src = FRAGMENT_SHADER_NV12_NV21;
#ifndef TARGET_OPENGLES
		ofStringReplace(src, "%r%g", "rg");
#else
		ofStringReplace(src, "%r%g", "ra");
#endif
		break;
	case OF_PIXELS_NV21:
		src = FRAGMENT_SHADER_NV12_NV21;
#ifndef TARGET_OPENGLES
		ofStringReplace(src, "%r%g", "gr");
#else
		ofStringReplace(src, "%r%g", "ar");
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
	if (textureTarget == GL_TEXTURE_2D) {
		header += "#define SAMPLER sampler2D\n";
	}
#ifndef TARGET_OPENGLES
	else if (textureTarget == GL_TEXTURE_RECTANGLE) {
		header += "#define SAMPLER sampler2DRect\n";
	}
#endif
	return shaderSource(header + src, major, minor);
}

string ofGLProgrammableRenderer::defaultVertexShaderHeader(GLenum textureTarget) {
	return defaultShaderHeader(vertex_shader_header, textureTarget, major, minor);
}

string ofGLProgrammableRenderer::defaultFragmentShaderHeader(GLenum textureTarget) {
	return defaultShaderHeader(fragment_shader_header, textureTarget, major, minor);
}

// ----------------------------------------------------------------------
void ofGLProgrammableRenderer::ShaderCollection::bindAttribute( GLuint location, const std::string & name ) {
#if !defined( TARGET_OPENGLES )
	texRectColor.bindAttribute( location, name);
	texRectNoColor.bindAttribute( location, name);
#endif
	tex2DColor.bindAttribute( location, name);
	noTexColor.bindAttribute( location, name);
	tex2DNoColor.bindAttribute( location, name);
	noTexNoColor.bindAttribute( location, name);
}

// ----------------------------------------------------------------------
void ofGLProgrammableRenderer::ShaderCollection::bindDefaults() {
#if !defined( TARGET_OPENGLES )
	texRectColor.bindDefaults();
	texRectNoColor.bindDefaults();
#endif
	tex2DColor.bindDefaults();
	noTexColor.bindDefaults();
	tex2DNoColor.bindDefaults();
	noTexNoColor.bindDefaults();
};

// ----------------------------------------------------------------------
void ofGLProgrammableRenderer::ShaderCollection::linkPrograms() {
#if !defined( TARGET_OPENGLES )
	texRectColor.linkProgram();
	texRectNoColor.linkProgram();
#endif
	tex2DColor.linkProgram();
	noTexColor.linkProgram();
	tex2DNoColor.linkProgram();
	noTexNoColor.linkProgram();
};

// ----------------------------------------------------------------------
void ofGLProgrammableRenderer::ShaderCollection::setupAllVertexShaders(const std::string& aShaderSrc) {
#ifndef TARGET_OPENGLES
	texRectColor.setupShaderFromSource(GL_VERTEX_SHADER, aShaderSrc);
	texRectNoColor.setupShaderFromSource(GL_VERTEX_SHADER, aShaderSrc);
#endif
	tex2DColor.setupShaderFromSource(GL_VERTEX_SHADER, aShaderSrc);
	tex2DNoColor.setupShaderFromSource(GL_VERTEX_SHADER, aShaderSrc);
	noTexColor.setupShaderFromSource(GL_VERTEX_SHADER, aShaderSrc);
	noTexNoColor.setupShaderFromSource(GL_VERTEX_SHADER, aShaderSrc);
};

// ----------------------------------------------------------------------
void ofGLProgrammableRenderer::LinesBundle::setMeshDataToVbo() {
	if( mesh.getNumVertices() > 0 ) {
		// void setMesh(const ofMesh & mesh, int usage, bool useColors, bool useTextures, bool useNormals);
		vbo.setMesh( mesh, GL_DYNAMIC_DRAW, mesh.hasColors() && mesh.usingColors(), mesh.hasTexCoords() && mesh.usingTextures(), mesh.hasNormals() && mesh.usingNormals() );
		
		if( lineMeshPrevVerts.size() > 0 ) {
			vbo.setAttributeData(vertAttribPrev, &lineMeshPrevVerts[0].x, 4, mesh.getNumVertices(), GL_DYNAMIC_DRAW);
		}
		if( lineMeshNextVerts.size() > 0 ) {
			vbo.setAttributeData(vertAttribNext, &lineMeshNextVerts[0].x, 4, mesh.getNumVertices(), GL_DYNAMIC_DRAW);
		}
	}
}

// ----------------------------------------------------------------------
void ofGLProgrammableRenderer::setup(int _major, int _minor) {
	glGetError();
#ifdef TARGET_OPENGLES
	// OpenGL ES might have set a default frame buffer for
	// MSAA rendering to the window, bypassing ofFbo, so we
	// can't trust ofFbo to have correctly tracked the bind
	// state. Therefore, we are forced to use the slower glGet() method
	// to be sure to get the correct default framebuffer.
	GLint currentFrameBuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFrameBuffer);
	defaultFramebufferId = currentFrameBuffer;
	currentFramebufferId = defaultFramebufferId;
#endif

	major = _major;
	minor = _minor;
#ifdef TARGET_RASPBERRY_PI
	uniqueShader = true;
#else
	uniqueShader = false;
#endif
	mDefaultShadersMap.clear();
	
	mLinesBundleMap.clear();
	if( mLinesBundleMap.count(GL_LINES) < 1 ) {
		mLinesBundleMap[GL_LINES] = LinesBundle();
		mLinesBundleMap[GL_LINES].mesh.setMode(OF_PRIMITIVE_TRIANGLES);
		// we will only be using the next vertex attribute for GL_LINES
		mLinesBundleMap[GL_LINES].vertAttribNext = 4;//ofShader::TEXCOORD_ATTRIBUTE+1;
	}
	if (mLinesBundleMap.count(GL_LINE_STRIP) < 1) {
		mLinesBundleMap[GL_LINE_STRIP] = LinesBundle();
		mLinesBundleMap[GL_LINE_STRIP].mesh.setMode(OF_PRIMITIVE_TRIANGLES);
		mLinesBundleMap[GL_LINE_STRIP].vertAttribPrev = 4;//ofShader::TEXCOORD_ATTRIBUTE+1;
		mLinesBundleMap[GL_LINE_STRIP].vertAttribNext = 5;//ofShader::TEXCOORD_ATTRIBUTE+2;
	}

	if (uniqueShader) {
		defaultUniqueShader.setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(uniqueVertexShader, major, minor));
		defaultUniqueShader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(uniqueFragmentShader, major, minor));
		defaultUniqueShader.bindDefaults();
		defaultUniqueShader.linkProgram();
		beginDefaultShader();
	} else {
		
		mDefaultShadersMap[GL_TRIANGLES] = std::make_shared<ShaderCollection>();
		mDefaultShadersMap[GL_POINTS] = std::make_shared<ShaderCollection>();
		mDefaultShadersMap[GL_LINES] = std::make_shared<ShaderCollection>();
		mDefaultShadersMap[GL_LINE_STRIP] = std::make_shared<ShaderCollection>();
		
		
		mDefaultShadersMap[GL_TRIANGLES]->setupAllVertexShaders(shaderSource(defaultVertexShader, major, minor));
		
#ifndef TARGET_OPENGLES
//		defaultTexRectColor.setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(defaultVertexShader, major, minor));
//		defaultTexRectNoColor.setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(defaultVertexShader, major, minor));
		alphaMaskRectShader.setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(defaultVertexShader, major, minor));
#endif
//		defaultTex2DColor.setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(defaultVertexShader, major, minor));
//		defaultNoTexColor.setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(defaultVertexShader, major, minor));
//		defaultTex2DNoColor.setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(defaultVertexShader, major, minor));
//		defaultNoTexNoColor.setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(defaultVertexShader, major, minor));
		alphaMask2DShader.setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(defaultVertexShader, major, minor));

#ifndef TARGET_OPENGLES
		mDefaultShadersMap[GL_TRIANGLES]->texRectColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(defaultFragmentShaderTexRectColor, major, minor));
		mDefaultShadersMap[GL_TRIANGLES]->texRectNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(defaultFragmentShaderTexRectNoColor, major, minor));
//		defaultTexRectColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(defaultFragmentShaderTexRectColor, major, minor));
//		defaultTexRectNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(defaultFragmentShaderTexRectNoColor, major, minor));
		alphaMaskRectShader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(alphaMaskFragmentShaderTexRectNoColor, major, minor));
#endif
		mDefaultShadersMap[GL_TRIANGLES]->tex2DColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(defaultFragmentShaderTex2DColor, major, minor));
		mDefaultShadersMap[GL_TRIANGLES]->noTexColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(defaultFragmentShaderNoTexColor, major, minor));
		mDefaultShadersMap[GL_TRIANGLES]->tex2DNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(defaultFragmentShaderTex2DNoColor, major, minor));
		mDefaultShadersMap[GL_TRIANGLES]->noTexNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(defaultFragmentShaderNoTexNoColor, major, minor));
//		defaultTex2DColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(defaultFragmentShaderTex2DColor, major, minor));
//		defaultNoTexColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(defaultFragmentShaderNoTexColor, major, minor));
//		defaultTex2DNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(defaultFragmentShaderTex2DNoColor, major, minor));
//		defaultNoTexNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(defaultFragmentShaderNoTexNoColor, major, minor));
		alphaMask2DShader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(alphaMaskFragmentShaderTex2DNoColor, major, minor));

		bitmapStringShader.setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(bitmapStringVertexShader, major, minor));
		bitmapStringShader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource(bitmapStringFragmentShader, major, minor));

#ifndef TARGET_OPENGLES
//		defaultTexRectColor.bindDefaults();
//		defaultTexRectNoColor.bindDefaults();
		alphaMaskRectShader.bindDefaults();
#endif
//		defaultTex2DColor.bindDefaults();
//		defaultNoTexColor.bindDefaults();
//		defaultTex2DNoColor.bindDefaults();
//		defaultNoTexNoColor.bindDefaults();
		alphaMask2DShader.bindDefaults();
		
		mDefaultShadersMap[GL_TRIANGLES]->bindDefaults();
		mDefaultShadersMap[GL_TRIANGLES]->linkPrograms();

#ifndef TARGET_OPENGLES
//		defaultTexRectColor.linkProgram();
//		defaultTexRectNoColor.linkProgram();
		alphaMaskRectShader.linkProgram();
#endif
//		defaultTex2DColor.linkProgram();
//		defaultNoTexColor.linkProgram();
//		defaultTex2DNoColor.linkProgram();
//		defaultNoTexNoColor.linkProgram();
		alphaMask2DShader.linkProgram();

		bitmapStringShader.bindDefaults();
		bitmapStringShader.linkProgram();

#ifdef TARGET_ANDROID
		defaultOESTexNoColor.setupShaderFromSource(GL_VERTEX_SHADER, shaderOESSource(defaultVertexShader, major, minor));
		defaultOESTexColor.setupShaderFromSource(GL_VERTEX_SHADER, shaderOESSource(defaultVertexShader, major, minor));
		defaultOESTexColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderOESSource(defaultFragmentShaderOESTexColor, major, minor));
		defaultOESTexNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderOESSource(defaultFragmentShaderOESTexNoColor, major, minor));

		defaultOESTexColor.bindDefaults();
		defaultOESTexNoColor.bindDefaults();

		defaultOESTexColor.linkProgram();
		defaultOESTexNoColor.linkProgram();
#endif
		
		
		// now lets start setting up the points shaders
		mDefaultShadersMap[GL_POINTS]->setupAllVertexShaders(shaderSource(defaultPointsVertexShader, major, minor));
		
		// ok, now lets setup the lines shaders //
		mDefaultShadersMap[GL_LINES]->setupAllVertexShaders(shaderSource(defaultLinesVertexShader, major, minor));
		
		// ok, now lets setup the line strip shaders //
		mDefaultShadersMap[GL_LINE_STRIP]->setupAllVertexShaders(shaderSource(defaultLineStripVertexShader, major, minor));
		
		// defaultFragmentShaderLines
		
		string alt_frag_header = fragment_shader_header;
		string defines = "#define OF_USING_TEXTURE_RECT 1\n#define OF_USING_VERTEX_COLORS 1\n";
		std::string header_w_defines = alt_frag_header + defines;
#ifndef TARGET_OPENGLES
		mDefaultShadersMap[GL_POINTS]->texRectColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																		  shaderSource(header_w_defines + defaultFragmentShaderPoints, major, minor));
		mDefaultShadersMap[GL_LINES]->texRectColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																		  shaderSource(header_w_defines + defaultFragmentShaderLines, major, minor));
		mDefaultShadersMap[GL_LINE_STRIP]->texRectColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																		  shaderSource(header_w_defines + defaultFragmentShaderLines, major, minor));
		
		defines = "#define OF_USING_TEXTURE_RECT 1\n";
		header_w_defines = alt_frag_header + defines;
		mDefaultShadersMap[GL_POINTS]->texRectNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																		  shaderSource(header_w_defines + defaultFragmentShaderPoints, major, minor));
		mDefaultShadersMap[GL_LINES]->texRectNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																			shaderSource(header_w_defines + defaultFragmentShaderLines, major, minor));
		mDefaultShadersMap[GL_LINE_STRIP]->texRectNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																		   shaderSource(header_w_defines + defaultFragmentShaderLines, major, minor));
#endif
		
		defines = "#define OF_USING_TEXTURE_2D 1\n#define OF_USING_VERTEX_COLORS 1\n";
		header_w_defines = alt_frag_header + defines;
		mDefaultShadersMap[GL_POINTS]->tex2DColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
													  shaderSource(header_w_defines + defaultFragmentShaderPoints, major, minor));
		mDefaultShadersMap[GL_LINES]->tex2DColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																		shaderSource(header_w_defines + defaultFragmentShaderLines, major, minor));
		mDefaultShadersMap[GL_LINE_STRIP]->tex2DColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																	   shaderSource(header_w_defines + defaultFragmentShaderLines, major, minor));
		
		defines = "#define OF_USING_TEXTURE_2D 1\n";
		header_w_defines = alt_frag_header + defines;
		mDefaultShadersMap[GL_POINTS]->tex2DNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
														shaderSource(header_w_defines + defaultFragmentShaderPoints, major, minor));
		mDefaultShadersMap[GL_LINES]->tex2DNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																		  shaderSource(header_w_defines + defaultFragmentShaderLines, major, minor));
		mDefaultShadersMap[GL_LINE_STRIP]->tex2DNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																		 shaderSource(header_w_defines + defaultFragmentShaderLines, major, minor));

		defines = "#define OF_USING_VERTEX_COLORS 1\n";
		header_w_defines = alt_frag_header + defines;
		mDefaultShadersMap[GL_POINTS]->noTexColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
													  shaderSource(header_w_defines + defaultFragmentShaderPoints, major, minor));
		mDefaultShadersMap[GL_LINES]->noTexColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																		shaderSource(header_w_defines + defaultFragmentShaderLines, major, minor));
		mDefaultShadersMap[GL_LINE_STRIP]->noTexColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																	   shaderSource(header_w_defines + defaultFragmentShaderLines, major, minor));

		defines = "";
		header_w_defines = alt_frag_header + defines;
		mDefaultShadersMap[GL_POINTS]->noTexNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
														shaderSource(header_w_defines + defaultFragmentShaderPoints, major, minor));
		mDefaultShadersMap[GL_LINES]->noTexNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																		  shaderSource(header_w_defines + defaultFragmentShaderLines, major, minor));
		mDefaultShadersMap[GL_LINE_STRIP]->noTexNoColor.setupShaderFromSource(GL_FRAGMENT_SHADER,
																		  shaderSource(header_w_defines + defaultFragmentShaderLines, major, minor));
		
		mDefaultShadersMap[GL_POINTS]->bindDefaults();
		mDefaultShadersMap[GL_POINTS]->linkPrograms();
		
		
		mDefaultShadersMap[GL_LINES]->bindDefaults();
		// now lets bind the vertex attribute for the next vertex
		mDefaultShadersMap[GL_LINES]->bindAttribute( mLinesBundleMap[GL_LINES].vertAttribNext, "nextVertex" );
		mDefaultShadersMap[GL_LINES]->linkPrograms();
		
		
		mDefaultShadersMap[GL_LINE_STRIP]->bindDefaults();
		// now lets bind the vertex attribute for the previous and next vertex
		mDefaultShadersMap[GL_LINE_STRIP]->bindAttribute( mLinesBundleMap[GL_LINE_STRIP].vertAttribPrev, "prevVertex" );
		mDefaultShadersMap[GL_LINE_STRIP]->bindAttribute( mLinesBundleMap[GL_LINE_STRIP].vertAttribNext, "nextVertex" );
		mDefaultShadersMap[GL_LINE_STRIP]->linkPrograms();
		
//		defaultPointsTex2DColor.bindDefaults();
//		defaultPointsTex2DNoColor.bindDefaults();
//		defaultPointsNoTexColor.bindDefaults();
//		defaultPointsNoTexNoColor.bindDefaults();
//		
//		defaultPointsTex2DColor.linkProgram();
//		defaultPointsTex2DNoColor.linkProgram();
//		defaultPointsNoTexColor.linkProgram();
//		defaultPointsNoTexNoColor.linkProgram();
	}

	setupGraphicDefaults();
	viewport();
	setupScreenPerspective();
}

const ofShader * ofGLProgrammableRenderer::getVideoShader(const ofBaseVideoDraws & video) const {
	const ofShader * shader = nullptr;
	GLenum target = video.getTexture().getTextureData().textureTarget;
	switch (video.getPixelFormat()) {
	case OF_PIXELS_YUY2:
		if (target == GL_TEXTURE_2D) {
			shader = &shaderPlanarYUY2;
		} else {
			shader = &shaderPlanarYUY2Rect;
		}
		break;
	case OF_PIXELS_NV12:
		if (target == GL_TEXTURE_2D) {
			shader = &shaderNV12;
		} else {
			shader = &shaderNV12Rect;
		}
		break;
	case OF_PIXELS_NV21:
		if (target == GL_TEXTURE_2D) {
			shader = &shaderNV21;
		} else {
			shader = &shaderNV21Rect;
		}
		break;
	case OF_PIXELS_YV12:
	case OF_PIXELS_I420:
		if (target == GL_TEXTURE_2D) {
			shader = &shaderPlanarYUV;
		} else {
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
	if (shader && !shader->isLoaded()) {
		ofShader * mutShader = const_cast<ofShader *>(shader);
		mutShader->setupShaderFromSource(GL_VERTEX_SHADER, shaderSource(defaultVertexShader, major, minor));
		mutShader->setupShaderFromSource(GL_FRAGMENT_SHADER, videoFragmentShaderSource(video, major, minor));
		mutShader->bindDefaults();
		mutShader->linkProgram();
	}
	return shader;
}

#ifndef TARGET_OPENGLES
static float getTextureScaleX(const ofBaseVideoDraws & video, int plane) {
	if (!video.getTexturePlanes().empty()) {
		return video.getTexturePlanes()[plane].getWidth() / video.getWidth();
	} else {
		return 1.0;
	}
}

static float getTextureScaleY(const ofBaseVideoDraws & video, int plane) {
	if (!video.getTexturePlanes().empty()) {
		return video.getTexturePlanes()[plane].getHeight() / video.getHeight();
	} else {
		return 1.0;
	}
}
#endif

void ofGLProgrammableRenderer::setVideoShaderUniforms(const ofBaseVideoDraws & video, const ofShader & shader) const {
	switch (video.getPixelFormat()) {
	case OF_PIXELS_YUY2:
#ifndef TARGET_OPENGLES
		if (video.getTexture().getTextureData().textureTarget == GL_TEXTURE_RECTANGLE) {
			shader.setUniform1f("onePixel", 1.0);
			shader.setUniform1f("textureWidth", 1.0);
		} else {
#endif
			shader.setUniform1f("onePixel", 1.0 / video.getWidth());
			shader.setUniform1f("textureWidth", video.getWidth());
#ifndef TARGET_OPENGLES
		}
#endif
		shader.setUniformTexture("src_tex_unit0", video.getTexturePlanes()[0], 0);
		break;
	case OF_PIXELS_NV12:
	case OF_PIXELS_NV21:
		shader.setUniformTexture("Ytex", video.getTexturePlanes()[0], 0);
		shader.setUniformTexture("UVtex", video.getTexturePlanes()[1], 1);
#ifndef TARGET_OPENGLES
		if (video.getTexture().getTextureData().textureTarget == GL_TEXTURE_RECTANGLE) {
			shader.setUniform2f("tex_scaleUV", getTextureScaleX(video, 1), getTextureScaleY(video, 1));
		} else {
#endif
			shader.setUniform2f("tex_scaleUV", 1.0, 1.0);
#ifndef TARGET_OPENGLES
		}
#endif
		break;
	case OF_PIXELS_YV12:
		shader.setUniformTexture("Ytex", video.getTexturePlanes()[0], 0);
		shader.setUniformTexture("Utex", video.getTexturePlanes()[2], 1);
		shader.setUniformTexture("Vtex", video.getTexturePlanes()[1], 2);
#ifndef TARGET_OPENGLES
		if (video.getTexture().getTextureData().textureTarget == GL_TEXTURE_RECTANGLE) {
			shader.setUniform2f("tex_scaleY", getTextureScaleX(video, 0), getTextureScaleY(video, 0));
			shader.setUniform2f("tex_scaleU", getTextureScaleX(video, 2), getTextureScaleY(video, 2));
			shader.setUniform2f("tex_scaleV", getTextureScaleX(video, 1), getTextureScaleY(video, 1));
		} else {
#endif
			shader.setUniform2f("tex_scaleY", 1.0, 1.0);
			shader.setUniform2f("tex_scaleU", 1.0, 1.0);
			shader.setUniform2f("tex_scaleV", 1.0, 1.0);
#ifndef TARGET_OPENGLES
		}
#endif
		break;
	case OF_PIXELS_I420:
		shader.setUniformTexture("Ytex", video.getTexturePlanes()[0], 0);
		shader.setUniformTexture("Utex", video.getTexturePlanes()[1], 1);
		shader.setUniformTexture("Vtex", video.getTexturePlanes()[2], 2);
#ifndef TARGET_OPENGLES
		if (video.getTexture().getTextureData().textureTarget == GL_TEXTURE_RECTANGLE) {
			shader.setUniform2f("tex_scaleY", getTextureScaleX(video, 0), getTextureScaleY(video, 0));
			shader.setUniform2f("tex_scaleU", getTextureScaleX(video, 1), getTextureScaleY(video, 1));
			shader.setUniform2f("tex_scaleV", getTextureScaleX(video, 2), getTextureScaleY(video, 2));
		} else {
#endif
			shader.setUniform2f("tex_scaleY", 1.0, 1.0);
			shader.setUniform2f("tex_scaleU", 1.0, 1.0);
			shader.setUniform2f("tex_scaleV", 1.0, 1.0);
#ifndef TARGET_OPENGLES
		}
#endif
		break;
	default:
		break;
	}
}

int ofGLProgrammableRenderer::getGLVersionMajor() {
	return major;
}

int ofGLProgrammableRenderer::getGLVersionMinor() {
	return minor;
}

void ofGLProgrammableRenderer::saveFullViewport(ofPixels & pixels) {
	ofRectangle v = getCurrentViewport();
	saveScreen(v.x, v.y, v.width, v.height, pixels);
}

void ofGLProgrammableRenderer::saveScreen(int x, int y, int w, int h, ofPixels & pixels) {
	int sh = getViewportHeight();

#ifndef TARGET_OPENGLES
	if (isVFlipped()) {
		y = sh - y;
		y -= h; // top, bottom issues
	}
	auto pixelFormat = OF_PIXELS_BGRA;
	pixels.allocate(w, h, pixelFormat);
	auto glFormat = ofGetGLFormat(pixels);

	ofBufferObject buffer;
	buffer.allocate(pixels.size(), GL_STATIC_READ);

	buffer.bind(GL_PIXEL_PACK_BUFFER);
	glReadPixels(x, y, w, h, glFormat, GL_UNSIGNED_BYTE, 0); // read the memory....
	buffer.unbind(GL_PIXEL_PACK_BUFFER);

	if (unsigned char * p = buffer.map<unsigned char>(GL_READ_ONLY)) {
		ofPixels src;
		src.setFromExternalPixels(p, w, h, pixelFormat);
		src.mirrorTo(pixels, true, false);
		buffer.unmap();
	} else {
		ofLogError("ofGLProgrammableRenderer") << "Error saving screen";
	}

#else

	int sw = getViewportWidth();
	int numPixels = w * h;
	if (numPixels == 0) {
		ofLogError("ofImage") << "grabScreen(): unable to grab screen, image width and/or height are 0: " << w << "x" << h;
		return;
	}
	pixels.allocate(w, h, OF_PIXELS_RGBA);

	switch (matrixStack.getOrientation()) {
	case OF_ORIENTATION_UNKNOWN:
	case OF_ORIENTATION_DEFAULT:

		if (isVFlipped()) {
			y = sh - y; // screen is flipped vertically.
			y -= h;
		}

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.getData());
		pixels.mirror(true, false);
		break;
	case OF_ORIENTATION_180:

		if (isVFlipped()) {
			x = sw - x; // screen is flipped horizontally.
			x -= w;
		}

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.getData());
		pixels.mirror(false, true);
		break;
	case OF_ORIENTATION_90_RIGHT:
		swap(w, h);
		swap(x, y);
		if (!isVFlipped()) {
			x = sw - x; // screen is flipped horizontally.
			x -= w;

			y = sh - y; // screen is flipped vertically.
			y -= h;
		}

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.getData());
		pixels.mirror(true, true);
		break;
	case OF_ORIENTATION_90_LEFT:
		swap(w, h);
		swap(x, y);
		if (isVFlipped()) {
			x = sw - x; // screen is flipped horizontally.
			x -= w;

			y = sh - y; // screen is flipped vertically.
			y -= h;
		}

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.getData());
		pixels.mirror(true, true);
		break;
	}

#endif
}

//----------------------------------------------------------
const of3dGraphics & ofGLProgrammableRenderer::get3dGraphics() const {
	return graphics3d;
}

//----------------------------------------------------------
of3dGraphics & ofGLProgrammableRenderer::get3dGraphics() {
	return graphics3d;
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::configureMeshToMatchWithNewVertsAndIndices(const ofMesh& aSrcMesh, ofMesh& aDstMesh, std::size_t aTargetNumVertices, std::size_t aTargetNumIndices) {
	bool bUseColors     = aSrcMesh.hasColors() && aSrcMesh.usingColors() && (aSrcMesh.getNumColors() == aSrcMesh.getNumVertices());
	bool bUseNormals    = aSrcMesh.hasNormals() && aSrcMesh.usingNormals() && (aSrcMesh.getNumNormals() == aSrcMesh.getNumVertices());
	bool bUseTexCoords  = aSrcMesh.hasTexCoords() && aSrcMesh.usingTextures() && (aSrcMesh.getNumTexCoords() == aSrcMesh.getNumVertices());
	
	if (aDstMesh.getNumVertices() != aTargetNumVertices) {
		aDstMesh.getVertices().assign(aTargetNumVertices, glm::vec3(0.f, 0.f, 0.f));
	}
	
	if( aDstMesh.getNumIndices() != aTargetNumIndices ) {
		aDstMesh.getIndices().assign(aTargetNumIndices, 0);
	}
	aDstMesh.enableIndices();
	
	if (bUseColors) {
		if (aDstMesh.getNumColors() != aTargetNumVertices) {
			aDstMesh.getColors().assign(aTargetNumVertices, ofFloatColor(1.f));
		}
		aDstMesh.enableColors();
	} else {
		aDstMesh.disableColors();
	}
	if (bUseNormals) {
		if (aDstMesh.getNumNormals() != aTargetNumVertices) {
			aDstMesh.getNormals().assign(aTargetNumVertices, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		aDstMesh.enableNormals();
	} else {
		aDstMesh.disableNormals();
	}
	if (bUseTexCoords) {
		if (aDstMesh.getNumTexCoords() != aTargetNumVertices) {
			aDstMesh.getTexCoords().assign(aTargetNumVertices, glm::vec2(0.0f, 0.0f));
		}
		aDstMesh.enableTextures();
	} else {
		aDstMesh.disableTextures();
	}
}

//----------------------------------------------------------
void ofGLProgrammableRenderer::configureLinesBundleFromMesh(LinesBundle& aLinesBundle, GLuint drawMode, const ofMesh& amesh) {
	auto& polyMesh = aLinesBundle.mesh;
	auto& nextVerts = aLinesBundle.lineMeshNextVerts;
	auto& prevVerts = aLinesBundle.lineMeshPrevVerts;
	
	bool bClosed = (drawMode == GL_LINE_LOOP) && amesh.getNumVertices() > 2;
	bool srcHasIndices = amesh.getNumIndices() > 0 && amesh.usingIndices();
	std::size_t srcNumVs = srcHasIndices ? amesh.getNumIndices() : amesh.getNumVertices();
	
	std::size_t targetNumPs = (bClosed ? srcNumVs + 1 : srcNumVs);
	std::size_t numIndicesA = (targetNumPs - 1) * 6 + (targetNumPs) * 6;
	
	std::size_t numVertsPer = 4;
	
	if( drawMode == GL_LINES ) {
		numVertsPer = 2;
		std::size_t targetNumLines = srcNumVs / 2;
		numIndicesA = targetNumLines * 6;
	}
	std::size_t targetNumVs = targetNumPs * numVertsPer;
	
	
	configureMeshToMatchWithNewVertsAndIndices(amesh, polyMesh, targetNumVs, numIndicesA );
	
	if( drawMode != GL_LINES ) {
		if( prevVerts.size() != targetNumVs ) {
			prevVerts.assign(targetNumVs, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		}
	}
	if (nextVerts.size() != targetNumVs ) {
		nextVerts.assign(targetNumVs, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	}
	
	bool bUseColors = amesh.hasColors() && amesh.usingColors() && (amesh.getNumColors() == amesh.getNumVertices());
	bool bUseNormals = amesh.hasNormals() && amesh.usingNormals() && (amesh.getNumNormals() == amesh.getNumVertices());
	bool bUseTexCoords = amesh.hasTexCoords() && amesh.usingTextures() && (amesh.getNumTexCoords() == amesh.getNumVertices());
	
	
	const auto *srcVerts = amesh.getVerticesPointer();
	const auto *srcIndices = amesh.getIndexPointer();
	const auto *srcColors = amesh.getColorsPointer();
	const auto *srcNormals = amesh.getNormalsPointer();
	const auto *srcTexCoords = amesh.getTexCoordsPointer();
	std::size_t numPs = srcHasIndices ? amesh.getNumIndices() : amesh.getNumVertices();
	
	auto &pmVerts = polyMesh.getVertices();
	auto &pmIndices = polyMesh.getIndices();
	auto *pmColors = polyMesh.getColorsPointer();
	auto *pmNormals = polyMesh.getNormalsPointer();
	auto *pmTexCoords = polyMesh.getTexCoordsPointer();
	
	float texU = 1.0;
	if( currentTextureTarget != OF_NO_TEXTURE ) {
		if( mUniformsTex.size() > 0 ) {
			texU = mUniformsTex[0].texData.tex_u;
		}
	}
	
	
	glm::vec3 EPSILON_VEC3 = glm::vec3(0.00005f);
	
	
	if( drawMode == GL_LINES ) {
		std::size_t cindex1 = 0;
		std::size_t cindex2 = 0;
		std::size_t lineIndex = 0;
		
		std::size_t pmIndex = 0;
		std::size_t kindex = 0;
		std::size_t newIndex = 0;
		
		std::size_t k = 0;
		
		for (size_t i = 0; i < targetNumPs; i += 2) {
			cindex1 = (i % numPs);
			cindex2 = (i + 1) % numPs;
			
			lineIndex = i / 2;
			
			if (srcHasIndices) {
				// get the verts from the indices
				cindex1 = srcIndices[cindex1];
				cindex2 = srcIndices[cindex2];
			}
			
			pmIndex = i * 2;
			
			// duplicate the vertices //
			for(k = 0; k < 4; k++ ) {
				kindex = cindex2;
				if (k < 2) {
					kindex = cindex1;
				}
				pmVerts[pmIndex + k] = srcVerts[kindex];
				
				if (bUseColors) {
					pmColors[pmIndex + k] = srcColors[kindex];
				}
				if (bUseNormals) {
					pmNormals[pmIndex + k] = srcNormals[kindex];
				}
				if (bUseTexCoords) {
//					pmTexCoords[pmIndex + k] = srcTexCoords[kindex];
					if( k == 1 || k == 2 ) {
						// move the tex y coord down
						pmTexCoords[pmIndex + k] = glm::vec2(srcTexCoords[kindex].x, texU);
					} else {
						pmTexCoords[pmIndex + k] = srcTexCoords[kindex];
					}
				}
			}
			
			nextVerts[pmIndex + 0] = glm::vec4(srcVerts[cindex2], -1.0f);
			nextVerts[pmIndex + 1] = glm::vec4(srcVerts[cindex2], 1.0f);
			
			nextVerts[pmIndex + 2] = glm::vec4(srcVerts[cindex1], 1.0f);
			nextVerts[pmIndex + 3] = glm::vec4(srcVerts[cindex1], -1.0f);
			
			newIndex = lineIndex * 6;
			pmIndices[newIndex + 0] = pmIndex + 0;
			pmIndices[newIndex + 1] = pmIndex + 2;
			pmIndices[newIndex + 2] = pmIndex + 1;
			
			pmIndices[newIndex + 3] = pmIndex + 1;
			pmIndices[newIndex + 4] = pmIndex + 2;
			pmIndices[newIndex + 5] = pmIndex + 3;
		}
		
		aLinesBundle.setMeshDataToVbo();
	} else {
		std::size_t nindex, pindex, cindex;
		glm::vec4 nvert;
		glm::vec4 pvert;
		std::size_t pmIndex = 0;
		std::size_t kindex = 0;
		std::size_t k = 0;
		std::size_t newIndex = 0;
		std::size_t nextIndex = 0;
		
		// if we have vertices lying on top of each other,
		// lets create some way to store valid ones
		// might not be the best approach, but easier / faster than removing the vertices from the src mesh
		glm::vec3 cachedPrevVert = {0.f, 0.f, 0.f};
		glm::vec3 cachedNextVert = {0.f, 0.f, 0.f};
		
		for (size_t i = 0; i < targetNumPs; i++) {
			cindex = (i % numPs);
			nindex = (i + 1) % numPs;
			pindex = (i - 1) % numPs;
			
			if (i == 0) {
				if (bClosed) {
					cindex = 0;
					pindex = numPs - 1;
					nindex = 1;
				} else {
					pindex = 0;
				}
			} else if (i >= numPs - 1) {
				if (!bClosed) {
					nindex = numPs - 1;
				}
			}
			
			if (srcHasIndices) {
				// get the verts from the indices
				cindex = srcIndices[cindex];
				nindex = srcIndices[nindex];
				pindex = srcIndices[pindex];
			}
			
			if( i == 0 ) {
				// just in case, lets init the cached verts to use
				cachedPrevVert = srcVerts[cindex];
				cachedNextVert = srcVerts[cindex];
			}
			
			// duplicate the vertices //
			pmIndex = i * numVertsPer;
			
			pvert = glm::vec4(srcVerts[pindex], 1.0f);
			nvert = glm::vec4(srcVerts[nindex], 1.0f);
			
			if( glm::all(glm::lessThan(glm::abs(srcVerts[cindex]-srcVerts[pindex]), EPSILON_VEC3 ))) {
				if( i != 0 || (i == 0 && bClosed )) {
					// loop through and find a good next vert //
					for( int pi = i-2; pi >= 0; pi-- ) {
						pindex = (pi) % numPs;
						if (srcHasIndices) {
							pindex = srcIndices[pindex];
						}
						if( !glm::all(glm::lessThan(glm::abs(srcVerts[cindex]-srcVerts[pindex]), EPSILON_VEC3 ) )) {
							pvert = glm::vec4(srcVerts[pindex], 1.f);
							break;
						}
					}
				} else {
					pvert = glm::vec4(cachedPrevVert, 1.0f);
				}
			} else {
				cachedPrevVert = srcVerts[pindex];
			}
			
			
			
			if(glm::all(glm::lessThan(glm::abs(srcVerts[cindex]-srcVerts[nindex]), EPSILON_VEC3 )) ) {
//				nvert = glm::vec4(cachedNextVert, 1.f);
				if( i != targetNumPs - 1 || (i == targetNumPs - 1 && bClosed) ) {
					// loop through and find a good next vert //
					for( int ni = i+2; ni < targetNumPs; ni++ ) {
						nindex = (ni) % numPs;
						if (srcHasIndices) {
							nindex = srcIndices[nindex];
						}
						if( !glm::all(glm::lessThan(glm::abs(srcVerts[cindex]-srcVerts[nindex]), EPSILON_VEC3 ) )) {
							nvert = glm::vec4(srcVerts[nindex], 1.f);
							break;
						}
					}
				} else {
					nvert = glm::vec4(cachedNextVert, 1.f);
				}
			} else {
				cachedNextVert = srcVerts[nindex];
			}
			
			for(k = 0; k < numVertsPer; k++) {
				pmVerts[pmIndex + k] = srcVerts[cindex];
				nextVerts[pmIndex + k] = nvert;
				prevVerts[pmIndex + k] = pvert;
				if (k % 2 == 0 || k == 0) {
					nextVerts[pmIndex + k].w = -1.0f;
				} else {
					nextVerts[pmIndex + k].w = 1.0f;
				}
				kindex = cindex;
				prevVerts[pmIndex + k].w = 1.0;
				if (k < 2) {
					prevVerts[pmIndex + k].w = -1.0;
				}
				
				if (bUseColors) {
					pmColors[pmIndex + k] = srcColors[kindex];
				}
				if (bUseNormals) {
					pmNormals[pmIndex + k] = srcNormals[kindex];
				}
				if (bUseTexCoords) {
					if (k % 2 == 0 || k == 0) {
						// should be in relation to the top of the image
						pmTexCoords[pmIndex + k] = srcTexCoords[kindex];
					} else {
						// lets go down to the bottom of the texture 
						pmTexCoords[pmIndex + k] = glm::vec2(srcTexCoords[kindex].x, texU);
					}
//					pmTexCoords[pmIndex + k] = srcTexCoords[kindex];
				}
			}
			
			newIndex = i * 2 * 6;
			pmIndices[newIndex + 0] = pmIndex + 0;
			pmIndices[newIndex + 1] = pmIndex + 2;
			pmIndices[newIndex + 2] = pmIndex + 1;
			
			pmIndices[newIndex + 3] = pmIndex + 1;
			pmIndices[newIndex + 4] = pmIndex + 2;
			pmIndices[newIndex + 5] = pmIndex + 3;
			
			nextIndex = (i+1);
			if( bClosed && i > targetNumPs -1 ) {
				nextIndex = 0;
			}
			
			if( nextIndex < targetNumPs) {
				std::size_t nextPmIndex = (nextIndex) * numVertsPer;
				pmIndices[newIndex + 6] = pmIndex + 2;
				pmIndices[newIndex + 7] = nextPmIndex + 0;
				pmIndices[newIndex + 8] = pmIndex + 3;
				
				pmIndices[newIndex + 9] = pmIndex + 3;
				pmIndices[newIndex + 10] = nextPmIndex + 0;
				pmIndices[newIndex + 11] = nextPmIndex + 1;
			}
		}
		
		aLinesBundle.setMeshDataToVbo();
	}
}
