//
//  ofCubeMap.cpp
//
//  Created by Nick Hardeman on 10/16/22.
//

#include "ofCubeMap.h"
#include "ofImage.h"
#include "ofConstants.h"
#include "of3dUtils.h"
#include "ofGLBaseTypes.h"
#include "ofGLUtils.h"

#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "ofGLProgrammableRenderer.h"
#include "ofCubeMapShaders.h"

#ifdef TARGET_ANDROID
#include "ofAppAndroidWindow.h"
#endif

#include <map>

#ifndef STRINGIFY
#define STRINGIFY(x) #x
#endif

using std::weak_ptr;
using std::vector;
using std::shared_ptr;

ofVboMesh ofCubeMap::sCubeMesh;
ofShader ofCubeMap::shaderBrdfLUT;
ofTexture ofCubeMap::sBrdfLutTex;


// texture management copied from ofTexture
static std::map<GLuint,int> & getTexturesIndex(){
	static std::map<GLuint,int> * textureReferences = new std::map<GLuint,int>;
	return *textureReferences;
}

static void retain(GLuint id){
	if(id!=0){
		if(getTexturesIndex().find(id)!=getTexturesIndex().end()){
			getTexturesIndex()[id]++;
		}else{
			getTexturesIndex()[id]=1;
		}
	}
}

static void release(GLuint id){
	// try to free up the texture memory so we don't reallocate
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/deletetextures.html
	if (id != 0){
		if(getTexturesIndex().find(id)!=getTexturesIndex().end()){
			getTexturesIndex()[id]--;
			if(getTexturesIndex()[id]==0){
				
#ifdef TARGET_ANDROID
				if (!ofAppAndroidWindow::isSurfaceDestroyed())
#endif
					glDeleteTextures(1, (GLuint *)&id);
				
				getTexturesIndex().erase(id);
			}
		}else{
			ofLogError("ofCubeMap") << "release(): something's wrong here, releasing unknown texture id " << id;
			
#ifdef TARGET_ANDROID
			if (!ofAppAndroidWindow::isSurfaceDestroyed())
#endif
				glDeleteTextures(1, (GLuint *)&id);
		}
	}
}

//----------------------------------------
#ifdef TARGET_ANDROID
// TODO: Hook this up to an event
void ofCubeMap::regenerateAllTextures() {
	for(size_t i=0; i<ofCubeMapsData().size(); i++) {
		if(!ofCubeMapsData()[i].expired()) {
			std::shared_ptr<ofCubeMap::Data> cubeMap = ofCubeMapsData()[i].lock();
			ofCubeMap::clearTextureData(cubeMap);
		}
	}
	sBrdfLutTex.clear();
}
#endif



//----------------------------------------
vector<weak_ptr<ofCubeMap::Data> > & ofCubeMapsData(){
	static vector<weak_ptr<ofCubeMap::Data> > * cubeMapsActive = new vector<weak_ptr<ofCubeMap::Data> >;
	return *cubeMapsActive;
}

//--------------------------------------------------------------
bool ofCubeMap::hasActiveCubeMap() {
	for(size_t i=0;i< ofCubeMapsData().size();i++){
		std::shared_ptr<ofCubeMap::Data> cubeMap = ofCubeMapsData()[i].lock();
		if(cubeMap && cubeMap->isEnabled && cubeMap->index > -1 ){
			return true;
			break;
		}
	}
	return false;
}

//--------------------------------------------------------------
std::shared_ptr<ofCubeMap::Data> ofCubeMap::getActiveData() {
	for(size_t i=0;i< ofCubeMapsData().size();i++){
		std::shared_ptr<ofCubeMap::Data> cubeMap = ofCubeMapsData()[i].lock();
		if(cubeMap && cubeMap->isEnabled && cubeMap->index > -1 ){
			return cubeMap;
		}
	}
	return std::shared_ptr<ofCubeMap::Data>();
}

//--------------------------------------------------------------
void ofCubeMap::clearTextureData(std::shared_ptr<ofCubeMap::Data> adata) {
	if( adata ) {
		if( adata->bPreFilteredMapAllocated ) {
			adata->bPreFilteredMapAllocated=false;
			release(adata->preFilteredMapId);
		}
		
		if( adata->bIrradianceAllocated ) {
			adata->bIrradianceAllocated = false;
			release(adata->irradianceMapId);
		}
		if( adata->bCubeMapAllocated ) {
			adata->bCubeMapAllocated = false;
			release(adata->cubeMapId);
		}
	}
}

//--------------------------------------------------------------
void ofCubeMap::_checkSetup() {
	if( data->index < 0 ) {
		bool bFound = false;
		// search for the first free block
		for(size_t i=0; i<ofCubeMapsData().size(); i++) {
			if(ofCubeMapsData()[i].expired()) {
				data->index = i;
				ofCubeMapsData()[i] = data;
				bFound = true;
				break;
			}
		}
		if(!bFound && ofIsGLProgrammableRenderer()){
			ofCubeMapsData().push_back(data);
			data->index = ofCubeMapsData().size() - 1;
			bFound = true;
		}
	}
	// we should remove empty slots //
}

//----------------------------------------
const ofTexture& ofCubeMap::getBrdfLutTexture() {
	return sBrdfLutTex;
}

//----------------------------------------
int ofCubeMap::getNumMipMaps() {
	return 5;
}


//----------------------------------------
ofCubeMap::ofCubeMap() {
	texFormat = GL_RGB;
	data = std::make_shared<ofCubeMap::Data>();
#ifdef GL_TEXTURE_CUBE_MAP_SEAMLESS
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#endif
	_checkSetup();
	projectionMat = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f );
	
}

//----------------------------------------
ofCubeMap::ofCubeMap(const ofCubeMap & mom) {
	clear();
	
	if(data) {
		data.reset();
	}
	if( mom.data ) {
		data = std::make_shared<ofCubeMap::Data>(*mom.data);
		if( data->bCubeMapAllocated ) {
			retain(data->cubeMapId);
		}
		if( data->bIrradianceAllocated ) {
			retain(data->irradianceMapId);
		}
		if( data->bPreFilteredMapAllocated ) {
			retain(data->preFilteredMapId);
		}
	}
	if( !data ) {
		data = std::make_shared<ofCubeMap::Data>();
	}
	data->index = -1;
	_checkSetup(); // grab a new slot in ofCubeMapsData
	texFormat = mom.texFormat;
	mBFlipY = mom.mBFlipY;
	
	mSourceTex = mom.mSourceTex;
}

//----------------------------------------
ofCubeMap::ofCubeMap(ofCubeMap && mom) {
	clear();
	// taking ownership of the data shared_ptr
	data = mom.data;
	mSourceTex = mom.mSourceTex;
	
	mom.mSourceTex.clear();
	texFormat = mom.texFormat;
	mBFlipY = mom.mBFlipY;
}

//----------------------------------------
ofCubeMap::~ofCubeMap() {
	clear();
}

//--------------------------------------------------------------
ofCubeMap & ofCubeMap::operator=(const ofCubeMap & mom){
	if(&mom==this) return *this;
	clear();
	
	if(data) {
		data.reset();
	}
	if( mom.data ) {
		data = std::make_shared<ofCubeMap::Data>(*mom.data);
		if( data->bCubeMapAllocated ) {
			retain(data->cubeMapId);
		}
		if( data->bIrradianceAllocated ) {
			retain(data->irradianceMapId);
		}
		if( data->bPreFilteredMapAllocated ) {
			retain(data->preFilteredMapId);
		}
	}
	if( !data ) {
		data = std::make_shared<ofCubeMap::Data>();
	}
	data->index = -1;
	_checkSetup(); // grab a new slot in ofCubeMapsData
	texFormat = mom.texFormat;
	mBFlipY = mom.mBFlipY;
	
	mSourceTex = mom.mSourceTex;
	return *this;
}

//--------------------------------------------------------------
ofCubeMap& ofCubeMap::operator=(ofCubeMap && mom) {
	clear();
	
	data = mom.data;
	mSourceTex = mom.mSourceTex;
	
	mom.mSourceTex.clear();
	texFormat = mom.texFormat;
	mBFlipY = mom.mBFlipY;
	return *this;
}

//----------------------------------------
GLenum ofCubeMap::getTextureTarget() {
	return GL_TEXTURE_CUBE_MAP;
}

//----------------------------------------
bool ofCubeMap::load( std::string apath, int aFaceResolution, bool aBFlipY ) {
	if( !ofIsGLProgrammableRenderer() ) {
		ofLogError("ofCubeMap::load") << " cube maps only supported with programmable renderer.";
		return false;
	}
	if( aFaceResolution > 512 ) {
		ofLogWarning("ofCubeMap :: load : a face resolution larger than 512 can cause issues.");
	}
	mSourceTex.clear();
	std::string ext = ofToLower(ofFilePath::getFileExt(apath));
	mBFlipY = aBFlipY;
	bool hdr = (ext == "hdr" || ext == "exr");
	bool bLoadOk = false;
	data->resolution = aFaceResolution;
	data->maxMipLevels = getNumMipMaps();

	#ifdef TARGET_OPENGLES
	if(hdr) {
		ofLogWarning("ofCubeMap::load") << "loading of hdr or exr float textures not supported on GLES. Loading GL_RGB.";
	}
	hdr = false;
	#endif
	
	bool bArbTexEnabled = ofGetUsingArbTex();
	ofDisableArbTex();
	if( hdr ) {
		#ifndef TARGET_OPENGLES
		ofFloatPixels fpix;
		if( ofLoadImage(fpix, apath) ) {
			ofLogNotice("ofCubeMap::load : loaded ") << ext << " image.";
			bLoadOk = true;
			texFormat = GL_RGB32F;
			mSourceTex.loadData(fpix);
		}
		#endif
	} else {
		ofPixels ipix;
		if( ofLoadImage(ipix, apath) ) {
			bLoadOk = true;
			texFormat = GL_RGB;
			mSourceTex.loadData(ipix);
		}
	}
	if( !bLoadOk ) {
		ofLogWarning("ofCubeMap :: failed to load image from ") << apath;
	} else {
		mSourceTex.setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		mSourceTex.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
		
		_createCubeMap();
		if( isHdr() ) {
			_createIrradianceMap();
			_createPrefilteredCubeMap();
		}
	}
	
	if( bArbTexEnabled ) {
		ofEnableArbTex();
	}
	
	return bLoadOk;
}


//----------------------------------------
void ofCubeMap::clear() {
	mSourceTex.clear();
	clearTextureData(data);
}

// TODO: Implement loading image per face
////----------------------------------------
//void ofCubeMap::set( const ofImage& aPosX, const ofImage& aNegX, const ofImage& aPosY, const ofImage& aNegY, const ofImage& aPosZ, const ofImage& aNegZ ) {
//
//	_allocateTextures();
//
//	int size = aPosX.getWidth();
//
//	int texFormat = GL_RGB;
//	GLuint texStorageFormat = GL_UNSIGNED_BYTE;
//
//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, texFormat, size, size, 0, GL_RGB, texStorageFormat, aPosX.getPixels().getData()); // right
//	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, texFormat, size, size, 0, GL_RGB, texStorageFormat, aNegX.getPixels().getData()); // left
//
//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, texFormat, size, size, 0, GL_RGB, texStorageFormat, aPosY.getPixels().getData()); // top
//	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, texFormat, size, size, 0, GL_RGB, texStorageFormat, aNegY.getPixels().getData()); // bottom
//
//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, texFormat, size, size, 0, GL_RGB, texStorageFormat, aPosZ.getPixels().getData()); // back
//	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, texFormat, size, size, 0, GL_RGB, texStorageFormat, aNegZ.getPixels().getData()); // front
//
//	glBindTexture(getTextureTarget(), 0);
//}

////----------------------------------------
//void ofCubeMap::set( const ofFloatImage& aPosX, const ofFloatImage& aNegX, const ofFloatImage& aPosY, const ofFloatImage& aNegY, const ofFloatImage& aPosZ, const ofFloatImage& aNegZ ) {
//	
//	_allocateTextures();
//	
//	int size = aPosX.getWidth();
//	
//	int texFormat = GL_RGB32F;
//	GLuint texStorageFormat = GL_FLOAT;
//	
//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, texFormat, size, size, 0, GL_RGB, texStorageFormat, aPosX.getPixels().getData()); // right
//	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, texFormat, size, size, 0, GL_RGB, texStorageFormat, aNegX.getPixels().getData()); // left
//	
//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, texFormat, size, size, 0, GL_RGB, texStorageFormat, aPosY.getPixels().getData()); // top
//	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, texFormat, size, size, 0, GL_RGB, texStorageFormat, aNegY.getPixels().getData()); // bottom
//	
//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, texFormat, size, size, 0, GL_RGB, texStorageFormat, aPosZ.getPixels().getData()); // back
//	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, texFormat, size, size, 0, GL_RGB, texStorageFormat, aNegZ.getPixels().getData()); // front
//	
//	glBindTexture(getTextureTarget(), 0);
//}

//--------------------------------------------------------------
//void ofCubeMap::draw() {
//	if( !mSourceTex.isAllocated() ) {
//		ofLogWarning("ofCubeMap::draw() : texture not allocated, not drawing");
//		return;
//	}
//	ofSetColor(255);
//	mSourceTex.bind();
//	ofDrawSphere(0, 0, 0, 5000.0);
//	mSourceTex.unbind();
//}

//--------------------------------------------------------------
void ofCubeMap::drawCubeMap() {
	if( !data->bCubeMapAllocated ) {
		ofLogWarning("ofCubeMap::drawCubeMap() : textures not allocated, not drawing");
		return;
	}
	
	_drawCubeStart(data->cubeMapId);
	shaderRender.setUniform1f("uExposure", getExposure() );
	shaderRender.setUniform1f("uMaxMips", 1 );
	_drawCubeEnd();
}

//--------------------------------------------------------------
void ofCubeMap::drawIrradiance() {
	if( !data->bIrradianceAllocated ) {
		ofLogWarning("ofCubeMap::drawIrradiance() : textures not allocated, not drawing");
		return;
	}
	
	_drawCubeStart(data->irradianceMapId);
	shaderRender.setUniform1f("uExposure", getExposure() );
	shaderRender.setUniform1f("uMaxMips", 1 );
	_drawCubeEnd();
}

//--------------------------------------------------------------
void ofCubeMap::drawPrefilteredCube(float aRoughness) {
	if( !data->bPreFilteredMapAllocated ) {
		ofLogWarning("ofCubeMap::drawPrefilteredCube() : textures not allocated, not drawing");
		return;
	}
	
	_drawCubeStart(data->preFilteredMapId);
	shaderRender.setUniform1f("uExposure", getExposure() );
	shaderRender.setUniform1f("uRoughness", aRoughness );
	shaderRender.setUniform1f("uMaxMips", (float)data->maxMipLevels );
	_drawCubeEnd();
}

//--------------------------------------------------------------
void ofCubeMap::_drawCubeStart(GLuint aCubeMapId) {
	_allocateCubeMesh();
	
	if( !shaderRender.isLoaded() ) {
		_loadRenderShader();
	}
	if( shaderRender.isLoaded() ) {
		glDepthFunc(GL_LEQUAL);
		shaderRender.begin();
		shaderRender.setUniformTexture("uCubeMap", getTextureTarget(), aCubeMapId, 0 );
		shaderRender.setUniform1f("uIsHDR", isHdr() ? 1.0f : 0.0f );
	}
}

//--------------------------------------------------------------
void ofCubeMap::_drawCubeEnd() {
	sCubeMesh.draw();
	shaderRender.end();
	glDepthFunc(GL_LESS); // set depth function back to default
}

// TODO: implement drawing into face
////--------------------------------------------------------------
//void ofCubeMap::beginDrawingIntoFace( int aCubeFace ) {
//	if( !data->bCubeMapAllocated ) {
//		data->bCubeMapAllocated = true;
//		glGenTextures(1, &data->cubeMapId );
//		retain(data->cubeMapId);
//		_initEmptyTextures(data->cubeMapId, data->resolution);
//	}
//
//	if( !fbo.isAllocated() ) {
//		_allocateFbo(data->resolution);
//	}
//	fbo.begin();
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + aCubeFace, data->cubeMapId, 0 );
//}
//
////--------------------------------------------------------------
//void ofCubeMap::endDrawingIntoFace( int aCubeFace ) {
//	fbo.end();
//}

//TODO: Implement drawing via 3D camera
////--------------------------------------------------------------
//void ofCubeMap::begin3DDrawingIntoFace( int aCubeFace ) {
//	ofPushView();
//	beginDrawingIntoFace( aCubeFace );
//	// setup the camera for rendering //
//}
//
////--------------------------------------------------------------
//void ofCubeMap::end3DDrawingIntoFace( int aCubeFace ) {
//	endDrawingIntoFace( aCubeFace );
//	ofPopView();
//}

//--------------------------------------------------------------
bool ofCubeMap::hasCubeMap() {
	return data->bCubeMapAllocated;
}

//--------------------------------------------------------------
bool ofCubeMap::hasPrefilteredMap() {
	return data->bPreFilteredMapAllocated;
}

//--------------------------------------------------------------
bool ofCubeMap::hasIrradianceMap() {
	return data->bIrradianceAllocated;
}

//--------------------------------------------------------------
GLuint ofCubeMap::getTextureId() {
	return data->cubeMapId;
}

//--------------------------------------------------------------
bool ofCubeMap::isHdr() {
	#ifndef TARGET_OPENGLES
	return (texFormat == GL_RGB32F);
	#endif
	return false;
}

//--------------------------------------------------------------
void ofCubeMap::setUseBrdfLutTexture( bool ab ) {
	#ifdef TARGET_OPENGLES
	data->useLutTex = false;
	ofLogWarning("ofCubeMap::setUseBrdfLutTexture") << " brdf lut texture not supported on GLES.";
	return;
	#else
	data->useLutTex = ab;
	if(ab && !sBrdfLutTex.isAllocated() ) {
		_createBrdfLUT();
	}
	#endif
}

//--------------------------------------------------------------
void ofCubeMap::_createCubeMap() {
	if( !data->bCubeMapAllocated ) {
		data->bCubeMapAllocated = true;
		glGenTextures(1, &data->cubeMapId );
		retain(data->cubeMapId);
	}
	
	_allocateCubeMesh();
	
	std::vector<glm::mat4> views = _getViewMatrices( glm::vec3(0,0,0) );
	
	if( !shaderEquiRectToCubeMap.isLoaded() ) {
		auto esource = ofCubeMapShaders::equiRectToCubeMap();
		shaderEquiRectToCubeMap.setupShaderFromSource(GL_VERTEX_SHADER, esource.vertShader );
		shaderEquiRectToCubeMap.setupShaderFromSource(GL_FRAGMENT_SHADER, esource.fragShader );
		shaderEquiRectToCubeMap.bindDefaults();
		shaderEquiRectToCubeMap.linkProgram();
	}
	
	glEnable(GL_TEXTURE_CUBE_MAP);
	
	_initEmptyTextures( data->cubeMapId, data->resolution );
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, data->cubeMapId);
	
	//	glBindTexture(GL_TEXTURE_CUBE_MAP, data->preFilteredMapId);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifdef GL_TEXTURE_WRAP_R
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif

	#ifndef TARGET_OPENGLES
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, data->maxMipLevels);
		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	#endif
	
	bool bUseOfFbo = true;
	ofFboSettings fboSettings;
	
	unsigned int captureFBO, captureRBO;
	if( !bUseOfFbo ) {
		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);
		
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, data->resolution, data->resolution );
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	} else {
		fbo.clear();
		
		fboSettings.width = data->resolution;
		fboSettings.height = data->resolution;
		fboSettings.numSamples = 0;
		fboSettings.numColorbuffers = 1;
		fboSettings.useDepth = false;
		fboSettings.textureTarget = GL_TEXTURE_2D;
		fboSettings.internalformat = texFormat;
		fbo.allocate( fboSettings );
	}
	
	
	ofPushView();
	if( bUseOfFbo ) {
		fbo.begin();
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		ofViewport(0, 0, data->resolution, data->resolution, false);
	}
	ofSetColor( 255 );

	shaderEquiRectToCubeMap.begin();
	//	glActiveTexture(GL_TEXTURE0);
	//	setUniformTexture(const string & name, int textureTarget, GLint textureID, int textureLocation)
	shaderEquiRectToCubeMap.setUniformTexture("uEquirectangularTex", mSourceTex, 0 );
	//		shaderEquiRectToCubeMap.setUniformTexture("uEquirectangularTex", GL_TEXTURE_2D, mSourceTex.getTextureData().textureID, 0 );
	shaderEquiRectToCubeMap.setUniformMatrix4f("uProjection", projectionMat );
	shaderEquiRectToCubeMap.setUniform1f("uFlipY", mBFlipY ? 1.0 : 0.0 );
	
//	fbo.activateAllDrawBuffers();
	for (unsigned int i = 0; i < 6; i++) {
//		fbo.setActiveDrawBuffer(i);
		shaderEquiRectToCubeMap.setUniformMatrix4f("uView", views[i]);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, data->preFilteredMapId, mip);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, data->cubeMapId, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ofClear(0, 0, 0);
		sCubeMesh.draw();
	}
	shaderEquiRectToCubeMap.end();
	if( bUseOfFbo ) {
		fbo.end();
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	ofPopView();
//	fbo.clear();
	
//	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, data->cubeMapId);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, 0);
	if( !bUseOfFbo ) {
		glDeleteRenderbuffers(1, &captureRBO);
		glDeleteFramebuffers(1, &captureFBO);
	}
	
}

//--------------------------------------------------------------
void ofCubeMap::_configureCubeTextures(GLuint aCubeMapId) {
	
	GLenum textureTarget = getTextureTarget();
	glBindTexture(textureTarget, aCubeMapId );
	
#ifdef GL_TEXTURE_CUBE_MAP_SEAMLESS
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#endif
	
	glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifdef GL_TEXTURE_WRAP_R
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif
	glBindTexture(getTextureTarget(), 0);
}

//--------------------------------------------------------------
void ofCubeMap::_initEmptyTextures(GLuint aCubeMapId, int aSize) {
	
	GLenum textureTarget = getTextureTarget();
	glBindTexture(textureTarget, aCubeMapId );
	GLuint texStorageFormat = getTexStorageFormat();
	
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, texFormat, aSize, aSize, 0, GL_RGB, texStorageFormat, nullptr );
	}
	
	glBindTexture(getTextureTarget(), 0);
}

//--------------------------------------------------------------
void ofCubeMap::_createIrradianceMap() {
	if(data->bIrradianceAllocated) {
		return;
	}
	if( !data->bIrradianceAllocated ) {
		data->bIrradianceAllocated = true;
		glGenTextures(1, &data->irradianceMapId );
		retain(data->irradianceMapId);
	}
	_configureCubeTextures(data->irradianceMapId);
	_initEmptyTextures(data->irradianceMapId, data->irradianceRes );
	
	std::vector<glm::mat4> views = _getViewMatrices( glm::vec3(0,0,0) );
	
	if( !shaderIrradianceMap.isLoaded() ) {
		auto isource = ofCubeMapShaders::irriadianceCubeMap();
		shaderIrradianceMap.setupShaderFromSource(GL_VERTEX_SHADER, isource.vertShader );
		shaderIrradianceMap.setupShaderFromSource(GL_FRAGMENT_SHADER, isource.fragShader );
		shaderIrradianceMap.bindDefaults();
		shaderIrradianceMap.linkProgram();
	}
	
		
	
	fbo.clear();
	ofFboSettings fboSettings;
	fboSettings.width = data->irradianceRes;
	fboSettings.height = data->irradianceRes;
	fboSettings.numSamples = 0;
	fboSettings.numColorbuffers = 6;
	fboSettings.useDepth = true;
	fboSettings.textureTarget = GL_TEXTURE_2D;
	fboSettings.internalformat = texFormat;
	fbo.allocate( fboSettings );
	
	_allocateCubeMesh();
	
	ofPushView();
	fbo.begin();
	ofSetColor( 255 );
	shaderIrradianceMap.begin();
	//	setUniformTexture(const string & name, int textureTarget, GLint textureID, int textureLocation)
	shaderIrradianceMap.setUniformTexture("environmentMap", getTextureTarget(), data->cubeMapId, 1 );
	shaderIrradianceMap.setUniformMatrix4f("uProjection", projectionMat );
	//shaderIrradianceMap.setUniform1f("uFlipY", mBFlipY ? 1.0 : 0.0 );
	
	for( size_t i = 0; i < 6; i++ ) {
		shaderIrradianceMap.setUniformMatrix4f("uView", views[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, data->irradianceMapId, 0);
		//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ofClear(0, 0, 0);
		sCubeMesh.draw();
	}
	
	shaderIrradianceMap.end();
	fbo.end();
	
	ofPopView();
	
	fbo.clear();
}

//--------------------------------------------------------------
void ofCubeMap::_createPrefilteredCubeMap() {
	if(data->bPreFilteredMapAllocated) {
		return;
	}
	
	_allocateCubeMesh();
	data->bPreFilteredMapAllocated = true;
	glGenTextures(1, &data->preFilteredMapId );
	retain(data->preFilteredMapId);
	
	//_initEmptyTextures( data->preFilteredMapId, data->preFilterRes );
	glBindTexture(GL_TEXTURE_CUBE_MAP, data->preFilteredMapId);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	#ifdef GL_TEXTURE_WRAP_R
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	#endif

	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	#ifndef TARGET_OPENGLES
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, data->maxMipLevels - 1);
	#endif
	// generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
	// glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	GLuint texStorageFormat = getTexStorageFormat();

	for (int mip = 0; mip < data->maxMipLevels; mip++) {
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth  = data->preFilterRes * std::pow(0.5, mip);
		unsigned int mipHeight = data->preFilterRes * std::pow(0.5, mip);
		for (unsigned int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip, texFormat, mipWidth, mipHeight, 0, GL_RGB, texStorageFormat, nullptr );
		}
	}
	#ifdef TARGET_OPENGLES
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	#endif
	
	if( !shaderPreFilterMap.isLoaded() ) {
		auto psource = ofCubeMapShaders::prefilter();
		shaderPreFilterMap.setupShaderFromSource(GL_VERTEX_SHADER, psource.vertShader );
		shaderPreFilterMap.setupShaderFromSource(GL_FRAGMENT_SHADER, psource.fragShader );
		shaderPreFilterMap.bindDefaults();
		shaderPreFilterMap.linkProgram();
	}
	
	std::vector<glm::mat4> views = _getViewMatrices( glm::vec3(0,0,0) );
	
	
	fbo.clear();
	ofFboSettings fboSettings;
	fboSettings.numSamples = 0;
	fboSettings.numColorbuffers = 6;
	fboSettings.useDepth = false;
	fboSettings.textureTarget = GL_TEXTURE_2D;
	fboSettings.internalformat = texFormat;

	for (int mip = 0; mip < data->maxMipLevels; mip++) {
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth  = data->preFilterRes * std::pow(0.5, mip);
		unsigned int mipHeight = data->preFilterRes * std::pow(0.5, mip);

		fbo.clear();
		fboSettings.width = mipWidth;
		fboSettings.height = mipHeight;
		fbo.allocate(fboSettings);
		
		fbo.begin();
		shaderPreFilterMap.begin();
		shaderPreFilterMap.setUniformTexture("environmentMap", getTextureTarget(), data->cubeMapId, 0 );
		shaderPreFilterMap.setUniformMatrix4f("uProjection", projectionMat );
		
		float roughness = (float)mip / (float)(data->maxMipLevels - 1);
		shaderPreFilterMap.setUniform1f("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i) {
			shaderPreFilterMap.setUniformMatrix4f( "uView", views[i] );
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, data->preFilteredMapId, mip);
			ofClear(0, 0, 0);
			sCubeMesh.draw();
		}
		shaderPreFilterMap.end();
		fbo.end();
	}
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

//--------------------------------------------------------------
void ofCubeMap::_createBrdfLUT() {
	#ifndef TARGET_OPENGLES
	int lutWidth = 512;
	int lutHeight = 512;
	
	ofFbo lutFbo;
	
	if( !shaderBrdfLUT.isLoaded() ) {
		auto bsource = ofCubeMapShaders::brdfLUT();
		shaderBrdfLUT.setupShaderFromSource(GL_VERTEX_SHADER, bsource.vertShader );
		shaderBrdfLUT.setupShaderFromSource(GL_FRAGMENT_SHADER, bsource.fragShader );
		shaderBrdfLUT.bindDefaults();
		shaderBrdfLUT.linkProgram();
	}
	
	ofMesh quadMesh;
	quadMesh.setMode(OF_PRIMITIVE_TRIANGLES);
	quadMesh.addVertices({
		glm::vec3(0,0,0),
		glm::vec3(lutWidth, 0.0, 0.0),
		glm::vec3(lutWidth, lutHeight, 0.0),
		glm::vec3(0, lutHeight, 0.0f)
		
	});
	
	quadMesh.addTexCoords( {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f)
	});
	
	quadMesh.addIndices({
		0, 1, 3,
		1, 2, 3
	});
	quadMesh.disableColors();
	quadMesh.disableNormals();
	lutFbo.allocate(lutWidth, lutHeight, GL_RG32F );
	
	ofSetColor(255);
	ofPushView();
	lutFbo.begin();
	ofClear(0, 0, 0);
	shaderBrdfLUT.begin();
	quadMesh.draw();
	ofSetColor(255);
	shaderBrdfLUT.end();
	lutFbo.end();
	ofPopView();
	
	lutFbo.updateTexture(0);
	sBrdfLutTex = lutFbo.getTexture(0);
	lutFbo.clear();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	#else
	ofLogWarning("ofCubeMap::_createBrdfLUT") << " brdf lut texture not supported on GLES";
	#endif
}

//--------------------------------------------------------------
void ofCubeMap::_allocateFbo(int asize) {
	if( fbo.isAllocated() ) {
		return;
	}
	
	ofFboSettings fboSettings;
	fboSettings.width = asize;
	fboSettings.height = asize;
	fboSettings.numSamples = 0;
	fboSettings.numColorbuffers = 6;
	fboSettings.useDepth = true;
	fboSettings.textureTarget = GL_TEXTURE_2D;
	fboSettings.internalformat = texFormat;
	fbo.allocate( fboSettings );
}

//--------------------------------------------------------------
void ofCubeMap::_allocateCubeMesh() {
	if( sCubeMesh.getNumVertices() > 0 ) {
		return;
	}
	sCubeMesh = ofMesh::box( 1, 1, 1, 4, 4, 4);
	sCubeMesh.disableColors();
	sCubeMesh.disableTextures();
	sCubeMesh.disableNormals();
	sCubeMesh.getTexCoords().clear();
}

//--------------------------------------------------------------
std::vector<glm::mat4> ofCubeMap::_getViewMatrices(const glm::vec3& apos ) {
	// eye, target and up vector
	// +x, -x, +y, -y, +z and -z direction
	vector<glm::mat4> views = {
		glm::lookAt( apos, apos+glm::vec3(1,0,0), glm::vec3(0, -1, 0) ),
		glm::lookAt( apos, apos+glm::vec3(-1,0,0), glm::vec3(0, -1, 0) ),
		glm::lookAt( apos, apos+glm::vec3(0,1,0), glm::vec3(0, 0, 1) ),
		glm::lookAt( apos, apos+glm::vec3(0,-1,0), glm::vec3(0, 0, -1) ),
		glm::lookAt( apos, apos+glm::vec3(0,0,1), glm::vec3(0, -1, 0) ),
		glm::lookAt( apos, apos+glm::vec3(0,0,-1), glm::vec3(0, -1, 0) )
	};
	return views;
}

//--------------------------------------------------------------
GLuint ofCubeMap::getTexStorageFormat() {
	#ifndef TARGET_OPENGLES
	if( texFormat == GL_RGB32F ) {
		return GL_FLOAT;
	}
	#endif
	return GL_UNSIGNED_BYTE;
}

//--------------------------------------------------------------
bool ofCubeMap::_loadRenderShader() {
	shaderRender.unload();
	
	auto rsource = ofCubeMapShaders::renderShader();
	shaderRender.setupShaderFromSource(GL_VERTEX_SHADER, rsource.vertShader );
	shaderRender.setupShaderFromSource(GL_FRAGMENT_SHADER, rsource.fragShader );
	shaderRender.bindDefaults();
	return shaderRender.linkProgram();
}
