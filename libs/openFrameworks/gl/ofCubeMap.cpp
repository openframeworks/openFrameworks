//
//  ofCubeMap.cpp
//
//  Created by Nick Hardeman on 10/16/22.
//

#include "ofShader.h"
#include "ofCubeMap.h"
#include "ofImage.h"
#include "of3dUtils.h"
#include "ofGLBaseTypes.h"
#include "ofGLUtils.h"
#include "ofGLProgrammableRenderer.h"
#include "ofCubeMapShaders.h"
#include "ofFbo.h"
#include "ofConstants.h"

#define GLM_FORCE_CTOR_INIT
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include <map>

#ifdef TARGET_ANDROID
#include "ofAppAndroidWindow.h"
#endif

using std::weak_ptr;
using std::vector;
using std::shared_ptr;

ofVboMesh ofCubeMap::sCubeMesh;
ofShader ofCubeMap::shaderBrdfLUT;
ofTexture ofCubeMap::sBrdfLutTex;

void ofCubeMap::setExposure(float aExposure) {
	data->exposure=ofClamp(aExposure, 0.0f, 1.0f);
}

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
ofCubeMap::ofCubeMap() {
	data = std::make_shared<ofCubeMap::Data>();
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
	if( mom.data ) {
		data->settings = mom.data->settings;
	}
	_checkSetup(); // grab a new slot in ofCubeMapsData
	texFormat = mom.texFormat;
}

//----------------------------------------
ofCubeMap::ofCubeMap(ofCubeMap && mom) {
	clear();
	// taking ownership of the data shared_ptr
	data = mom.data;
	texFormat = mom.texFormat;
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
	if( mom.data ) {
		data->settings = mom.data->settings;
	}
	data->index = -1;
	_checkSetup(); // grab a new slot in ofCubeMapsData
	texFormat = mom.texFormat;
	
	return *this;
}

//--------------------------------------------------------------
ofCubeMap& ofCubeMap::operator=(ofCubeMap && mom) {
	clear();
	data = mom.data;
	texFormat = mom.texFormat;
	return *this;
}

//----------------------------------------
GLenum ofCubeMap::getTextureTarget() {
	return GL_TEXTURE_CUBE_MAP;
}

//----------------------------------------
bool isPowerOfTwo(int x) {
	/* First x in the below expression is for the case when
	 * x is 0 */
	return x && (!(x & (x - 1)));
}

//----------------------------------------
bool ofCubeMap::load( const of::filesystem::path & apath, int aFaceResolution, bool aBFlipY ) {
	return load(apath, aFaceResolution, aBFlipY, 32, 128 );
}

//----------------------------------------
bool ofCubeMap::load( const of::filesystem::path & apath, int aFaceResolution, bool aBFlipY, int aIrradianceRes, int aPreFilterRes ) {
	
	ofCubeMapSettings settings;
	settings.flipVertically = aBFlipY;
	settings.filePath = apath;
	
	settings.resolution = aFaceResolution;
	settings.irradianceRes = aIrradianceRes;
	settings.preFilterRes = aPreFilterRes;
	
	return load(settings);
}

//----------------------------------------
bool ofCubeMap::load( ofCubeMapSettings aSettings ) {
	if( !ofIsGLProgrammableRenderer() ) {
		ofLogError("ofCubeMap::load") << " cube maps only supported with programmable renderer.";
		return false;
	}
//	if( aSettings.resolution > 512 ) {
//		ofLogWarning("ofCubeMap :: load : a face resolution larger than 512 can cause issues.");
//	}
	
	if( aSettings.filePath.empty() ) {
		ofLogError("ofCubeMap :: load : must set file path");
		return false;
	}
	
	clear();
	
	std::string ext = ofToLower(aSettings.filePath.extension().string());
	bool hdr = (ext == ".hdr" || ext == ".exr");
	
#if defined(TARGET_OPENGLES) && !defined(TARGET_EMSCRIPTEN)
	if( hdr ) {
		ofLogError("ofCubeMap :: load : hdr and exr not supported on OPENGL_ES");
		return false;
	}
#endif
	
	bool bLoadOk = false;
	data->settings = aSettings;
	
	ofTexture srcTex;
	
	bool bArbTexEnabled = ofGetUsingArbTex();
	ofDisableArbTex();
	if( hdr ) {
		ofFloatPixels fpix;
		if( ofLoadImage(fpix, data->settings.filePath) ) {
			ofLogNotice("ofCubeMap::load : loaded ") << ext << " image.";
			bLoadOk = true;
#if defined(TARGET_EMSCRIPTEN)
			// GL_RGB32F GL_RGB16F &&  is not supported in Emscripten opengl es, so we need to set to GL_RGBA16F or GL_RGBA32F
			// ofFloatPixels uses 32F and is supported via Emscripten, so we switch to that in the _load functions.
			texFormat = GL_RGBA16F;
			// just in case, we need to make sure that it's 4 channels
			if( fpix.getNumChannels() != 4 ) {
				fpix.setImageType( OF_IMAGE_COLOR_ALPHA );
			}
#elif !defined(TARGET_OPENGLES)
			if( fpix.getNumChannels() != 3 ) {
				fpix.setImageType( OF_IMAGE_COLOR );
			}
			texFormat = GL_RGB32F;
#endif
			srcTex.loadData(fpix);
		}
	} else {
		ofPixels ipix;
		if( ofLoadImage(ipix, data->settings.filePath) ) {
			bLoadOk = true;
			texFormat = GL_RGB;
			srcTex.loadData(ipix);
		}
	}
	if( !bLoadOk ) {
		ofLogWarning("ofCubeMap :: failed to load image from ") << data->settings.filePath;
	} else {
		srcTex.setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		srcTex.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
#if !defined(TARGET_OPENGLES) && defined(GL_TEXTURE_CUBE_MAP_SEAMLESS)
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#endif
		
		if( !isPowerOfTwo(data->settings.resolution) ) {
			ofLogNotice("ofCubeMap :: load : changing resolution " ) << data->settings.resolution << " to next power of 2: " << ofNextPow2(data->settings.resolution);
			data->settings.resolution = ofNextPow2(data->settings.resolution);
		}
		if( !isPowerOfTwo(data->settings.irradianceRes) ) {
			ofLogNotice("ofCubeMap :: load : changing resolution " ) << data->settings.irradianceRes << " to next power of 2: " << ofNextPow2(data->settings.irradianceRes);
			data->settings.irradianceRes = ofNextPow2(data->settings.irradianceRes);
		}
		if( !isPowerOfTwo(data->settings.preFilterRes) ) {
			ofLogNotice("ofCubeMap :: load : changing resolution " ) << data->settings.preFilterRes << " to next power of 2: " << ofNextPow2(data->settings.preFilterRes);
			data->settings.preFilterRes = ofNextPow2(data->settings.preFilterRes);
		}
		
		_createCubeMap(srcTex);
		if( isHdr() ) {
			
			int srcCubeFSize = std::max(256, data->settings.preFilterRes);
			GLuint cubeFid = _createFloatCubeMap(srcTex, srcCubeFSize );
			
			// figure out the number of mip maps //
			data->maxMipLevels = log2(data->settings.preFilterRes) + 1;
			
			auto encFolder = data->settings.cacheDirectory;
			if( !encFolder.empty() ) {
				if( !ofDirectory::doesDirectoryExist( data->settings.cacheDirectory )) {
					#if !defined(TARGET_OPENGLES)
					if(!ofDirectory::createDirectory( data->settings.cacheDirectory )) {
						ofLogWarning("ofCubeMap :: load : unable to create directory: ") << data->settings.cacheDirectory;
					}
					#endif
				} else {
					
				}
				encFolder = data->settings.cacheDirectory;
			}
			of::filesystem::path baseName = data->settings.filePath.stem(); // equivalent to getBaseName
			of::filesystem::path cacheIrrName { baseName };
			cacheIrrName += ("_irr_"+ofToString(data->settings.irradianceRes,0)+".exr");
			of::filesystem::path cachePrefilterName { baseName };
			cachePrefilterName += ("_pre_"+ofToString(data->settings.preFilterRes,0)+".exr");
			
			bool bHasCachedIrr = false;
			bool bHasCachedPre = false;
			if( data->settings.useCache && !data->settings.overwriteCache ) {
				bHasCachedIrr = _loadIrradianceMap(encFolder / cacheIrrName);
				ofLogVerbose("ofCubeMap :: _loadIrradianceMap: ") << bHasCachedIrr;
				bHasCachedPre = _loadPrefilterMap(encFolder / cachePrefilterName);
				ofLogVerbose("ofCubeMap :: _loadPrefilterMap: ") << bHasCachedPre;
			}
			
			bool bMakeCache = data->settings.useCache;
			#if defined(TARGET_EMSCRIPTEN)
			// not supporting making caches on Emscripten.
			bMakeCache = false;
			#endif
			
			if( !bHasCachedIrr ) {
				ofLogVerbose("ofCubeMap :: going to create irradiance map");
				_createIrradianceMap(cubeFid,bMakeCache, encFolder / cacheIrrName);
			}
			
			if( !bHasCachedPre ) {
				ofLogVerbose("ofCubeMap :: going to create pre filtered cube map");
				_createPrefilteredCubeMap(cubeFid, srcCubeFSize,bMakeCache,encFolder / cachePrefilterName );
			}
			
			glDeleteTextures(1, &cubeFid );
		}
	}
	
	if( bArbTexEnabled ) {
		ofEnableArbTex();
	}
	
	return bLoadOk;
}


//----------------------------------------
void ofCubeMap::clear() {
	clearTextureData(data);
}

//--------------------------------------------------------------
void ofCubeMap::draw() {
	drawCubeMap();
}

//--------------------------------------------------------------
void ofCubeMap::drawCubeMap() {
	if( !data->bCubeMapAllocated ) {
		ofLogWarning("ofCubeMap::drawCubeMap() : textures not allocated, not drawing");
		return;
	}
	
	_drawCubeStart(data->cubeMapId);
	shaderRender.setUniform1f("uExposure", getExposure() );
	shaderRender.setUniform1f("uRoughness", 0.0f );
	shaderRender.setUniform1f("uMaxMips", 1.0f );
	shaderRender.setUniform1f("uIsHDR", 0.0f );
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
	shaderRender.setUniform1f("uRoughness", 0.0f );
	shaderRender.setUniform1f("uMaxMips", 1.0f );
	shaderRender.setUniform1f("uIsHDR", 1.0f );
	_drawCubeEnd();
}

//--------------------------------------------------------------
void ofCubeMap::drawPrefilteredCube(float aRoughness) {
	if( !data->bPreFilteredMapAllocated ) {
		ofLogWarning("ofCubeMap::drawPrefilteredCube() : textures not allocated, not drawing");
		return;
	}
	
	_drawCubeStart(data->preFilteredMapId);
	shaderRender.setUniform1f("uIsHDR", 1.0f );
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
		shaderRender.setUniformTexture("uCubeMap", getTextureTarget(), aCubeMapId, 0 );	}
}

//--------------------------------------------------------------
void ofCubeMap::_drawCubeEnd() {
	sCubeMesh.draw();
	shaderRender.end();
	glDepthFunc(GL_LESS); // set depth function back to default
}

//--------------------------------------------------------------
bool ofCubeMap::hasCubeMap() {
	if( !data ) return false;
	return data->bCubeMapAllocated;
}

//--------------------------------------------------------------
bool ofCubeMap::hasPrefilteredMap() {
	if( !data ) return false;
	return data->bPreFilteredMapAllocated;
}

//--------------------------------------------------------------
bool ofCubeMap::hasIrradianceMap() {
	if( !data ) return false;
	return data->bIrradianceAllocated;
}

//--------------------------------------------------------------
GLuint ofCubeMap::getTextureId() {
	if( !data ) return 0;
	return data->cubeMapId;
}

//--------------------------------------------------------------
bool ofCubeMap::isHdr() {
#if defined(TARGET_OPENGLES) && !defined(TARGET_EMSCRIPTEN)
	return false;
#else
	#ifdef GL_RGBA32F_EXT
	if( texFormat == GL_RGBA32F_EXT ) {
		return true;
	}
	#endif
	#ifdef GL_RGB32F_EXT
	if( texFormat == GL_RGB32F_EXT ) {
		return true;
	}
	#endif
	#ifdef GL_RGBA16F
	if( texFormat == GL_RGBA16F ) {
		return true;
	}
	#endif
	return (texFormat == GL_RGBA32F || texFormat == GL_RGB32F);
#endif
}

//--------------------------------------------------------------
void ofCubeMap::setUseBrdfLutTexture( bool ab ) {
	#ifdef TARGET_OPENGLES
	data->settings.useLutTex = false;
	ofLogWarning("ofCubeMap::setUseBrdfLutTexture") << " brdf lut texture not supported on GLES.";
	return;
	#else
	data->settings.useLutTex = ab;
	if(ab && !sBrdfLutTex.isAllocated() ) {
		_createBrdfLUT();
	}
	#endif
}

//--------------------------------------------------------------
void ofCubeMap::_createCubeMap(ofTexture& aSrcTex) {
	
	if( !data->bCubeMapAllocated ) {
		data->bCubeMapAllocated = true;
		glGenTextures(1, &data->cubeMapId );
		retain(data->cubeMapId);
	}
		
	GLuint internalFormat = ofGetGLInternalFormatFromPixelFormat(OF_PIXELS_RGB);
	#ifdef TARGET_OPENGLES
	internalFormat = ofGetGLInternalFormatFromPixelFormat(OF_PIXELS_RGBA);
	#endif
	
	GLuint texStorageFormat = GL_UNSIGNED_BYTE;
	GLuint gFormat = GL_RGB;
	#ifdef TARGET_OPENGLES
	gFormat = GL_RGBA;
	#endif
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, data->cubeMapId);
	
	for (GLint i = 0 ; i < 6 ; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, data->settings.resolution, data->settings.resolution, 0, gFormat, texStorageFormat, NULL);
	}
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	_configureCubeTextures( data->cubeMapId, true );
	
	
	_equiRectToCubeMap( data->cubeMapId, aSrcTex, data->settings.resolution, true );
	
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, data->cubeMapId);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
}

//--------------------------------------------------------------
void ofCubeMap::_configureCubeTextures(GLuint aCubeMapId, bool abLinearMipLinear) {
	
	GLenum textureTarget = getTextureTarget();
	glBindTexture(textureTarget, aCubeMapId );
	
	if(abLinearMipLinear) {
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
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
	GLuint gFormat = getGlTypeFromInternalFormat();
	
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, texFormat, aSize, aSize, 0, gFormat, texStorageFormat, nullptr );
	}
	
	glBindTexture(getTextureTarget(), 0);
}

//--------------------------------------------------------------
void ofCubeMap::_initEmptyTextures(GLuint aCubeMapId, GLuint aInternalFormat, int aSize, int aNumMipMaps ) {
	GLenum textureTarget = getTextureTarget();
	glBindTexture(textureTarget, aCubeMapId );
	GLuint texStorageFormat = getTexStorageFormat(aInternalFormat);
	GLuint gFormat = getGlTypeFromInternalFormat(aInternalFormat);
	
	for (int mip = 0; mip < data->maxMipLevels; mip++) {
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth  = static_cast<unsigned int>(data->settings.preFilterRes * std::pow(0.5, mip));
		if(mipWidth < 1 ) {
			mipWidth = 1;
		}
		for (unsigned int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip, aInternalFormat, mipWidth, mipWidth, 0, gFormat, texStorageFormat, nullptr );
		}
	}
	
	glBindTexture(getTextureTarget(), 0);
}

//--------------------------------------------------------------
GLuint ofCubeMap::_createFloatCubeMap(ofTexture& aSrcTex, int aSrcRes) {
	GLuint cubeTexF;
	glGenTextures(1, &cubeTexF );
			
	GLuint texStorageFormat = getTexStorageFormat();
	GLuint gFormat = getGlTypeFromInternalFormat();
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexF );
	
	for (GLint i = 0 ; i < 6 ; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, texFormat, aSrcRes, aSrcRes, 0, gFormat, texStorageFormat, NULL);
	}
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	_configureCubeTextures( cubeTexF, true );
	_equiRectToCubeMap( cubeTexF, aSrcTex, aSrcRes, false );
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexF);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	return cubeTexF;
}

//--------------------------------------------------------------
void ofCubeMap::_equiRectToCubeMap( GLuint& aCubeTexId, ofTexture& aSrcTex, int aSrcRes, bool aBConvertToNonFloat ) {
	bool bShaderLoaded = _loadEquiRectToCubeMapShader();
	if( !bShaderLoaded ) {
		ofLogError("ofCubeMap::_equiRectToCubeMap : error loading shader");
		return;
	}
	
	_allocateCubeMesh();
	std::vector<glm::mat4> views = _getViewMatrices( glm::vec3(0,0,0) );
	
	unsigned int captureFBO;
	glGenFramebuffers(1, &captureFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO );
	
	ofSetColor( 255 );
		
	ofPushView();
	ofViewport(0, 0, aSrcRes, aSrcRes, false);
	
	shaderEquiRectToCubeMap.begin();
	shaderEquiRectToCubeMap.setUniformTexture("uEquirectangularTex", aSrcTex, 0 );
	shaderEquiRectToCubeMap.setUniformMatrix4f("uProjection", projectionMat );
	shaderEquiRectToCubeMap.setUniform1f("uFlipY", data->settings.flipVertically ? 1.0f : 0.0f );
	shaderEquiRectToCubeMap.setUniform1f("uConvertToNonFloat", aBConvertToNonFloat ? 1.0f : 0.0f );
	
	for (unsigned int i = 0; i < 6; i++) {
		shaderEquiRectToCubeMap.setUniformMatrix4f("uView", views[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, aCubeTexId, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		sCubeMesh.draw();
	}
	shaderEquiRectToCubeMap.end();
	ofPopView();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &captureFBO);
}

//--------------------------------------------------------------
void ofCubeMap::_createIrradianceMap(GLuint aSrcCubeFid, bool aBMakeCache, const of::filesystem::path & aCachePath) {
	if(data->bIrradianceAllocated) {
		return;
	}
	if( !data->bIrradianceAllocated ) {
		data->bIrradianceAllocated = true;
		glGenTextures(1, &data->irradianceMapId );
		retain(data->irradianceMapId);
	}
	
	_allocateCubeMesh();
	
	std::vector<glm::mat4> views = _getViewMatrices( glm::vec3(0,0,0) );
	
	if( !shaderIrradianceMap.isLoaded() ) {
		auto isource = ofCubeMapShaders::irriadianceCubeMap();
		shaderIrradianceMap.setupShaderFromSource(GL_VERTEX_SHADER, isource.vertShader );
		shaderIrradianceMap.setupShaderFromSource(GL_FRAGMENT_SHADER, isource.fragShader );
		shaderIrradianceMap.bindDefaults();
		shaderIrradianceMap.linkProgram();
	}
	
	if( aBMakeCache ) {
				
		ofLogNotice("ofCubeMap :: _createIrradianceMap : making cache");
		
		ofFbo tfbo;
		//	fbo.clear();
		ofFboSettings fboSettings;
		fboSettings.width = data->settings.irradianceRes;
		fboSettings.height = data->settings.irradianceRes;
		fboSettings.numSamples = 0;
		//	fboSettings.numColorbuffers = 6;
		fboSettings.useDepth = false;
//		fboSettings.textureTarget = GL_TEXTURE_2D;
		fboSettings.internalformat = texFormat;
		tfbo.allocate(fboSettings);
				
		ofSetColor( 255 );
		
		vector<ofFloatPixels> fpixels;
		fpixels.assign(6, ofFloatPixels());
		bool bAllPixelsCreated = true;
		
		for( unsigned int i = 0; i < 6; i++ ) {
			tfbo.begin();
			ofClear(0, 0, 0);
			shaderIrradianceMap.begin();
			shaderIrradianceMap.setUniformTexture("environmentMap", getTextureTarget(), aSrcCubeFid, 0 );
			shaderIrradianceMap.setUniformMatrix4f("uProjection", projectionMat );
			shaderIrradianceMap.setUniformMatrix4f("uView", views[i]);
			sCubeMesh.draw();
			shaderIrradianceMap.end();
			
			tfbo.end();
			tfbo.updateTexture(0);
			tfbo.readToPixels(fpixels[i]);
			if( fpixels[i].getWidth() < 1 || fpixels[i].getHeight() < 1 ) {
				bAllPixelsCreated = false;
			}
		}
		
		if(bAllPixelsCreated) {
			GLenum textureTarget = getTextureTarget();
			glBindTexture(textureTarget, data->irradianceMapId );
			GLuint texStorageFormat = getTexStorageFormat();
			GLuint gFormat = getGlTypeFromInternalFormat();
			// we need to create a single image //
			for (unsigned int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, texFormat, fpixels[i].getWidth(), fpixels[i].getHeight(), 0, gFormat, texStorageFormat, fpixels[i].getData() );
			}
			glBindTexture(textureTarget, 0 );
			
			_configureCubeTextures(data->irradianceMapId, false);
			
			// ok, now lets make a single fbo
			int fullWidth = data->settings.irradianceRes * 3;
			fboSettings.width = fullWidth;
			fboSettings.height = data->settings.irradianceRes * 2;
			
			int texSize = data->settings.irradianceRes;
			
			tfbo.clear();
			tfbo.allocate( fboSettings );
			ofSetColor(255);
			tfbo.begin(); {
				ofClear(0);
				ofTexture ftex;
				for (unsigned int j = 0; j < 6; j++) {
					ftex.loadData( fpixels[j] );
					ftex.draw((j % 3) * texSize, floor(j / 3) * texSize, texSize, texSize);
				}
			} tfbo.end();
			
			ofFloatPixels fpix;
			tfbo.updateTexture(0);
			tfbo.readToPixels(fpix);
			if( fpix.getNumChannels() != 3 ) {
				fpix.setNumChannels(3);
			}
			if(!ofSaveImage(fpix, aCachePath)) {
				ofLogError("ofCubeMap :: _createIrradianceMap : ") << aCachePath;
			}
		}
		
		
	} else {
		
		_initEmptyTextures(data->irradianceMapId, data->settings.irradianceRes );
		_configureCubeTextures(data->irradianceMapId, false);
		
		unsigned int captureFBO;

		glGenFramebuffers(1, &captureFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO );

		ofPushView();
		ofViewport(0, 0, data->settings.irradianceRes, data->settings.irradianceRes, false);
		
		ofSetColor( 255 );
		shaderIrradianceMap.begin();
		shaderIrradianceMap.setUniformTexture("environmentMap", getTextureTarget(), aSrcCubeFid, 0 );
		shaderIrradianceMap.setUniformMatrix4f("uProjection", projectionMat );

		for( unsigned int i = 0; i < 6; i++ ) {
			shaderIrradianceMap.setUniformMatrix4f("uView", views[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, data->irradianceMapId, 0);
			ofClear(0, 0, 0, 255);
			sCubeMesh.draw();
		}

		shaderIrradianceMap.end();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &captureFBO);
		ofPopView();
	}
	
}

//--------------------------------------------------------------
bool ofCubeMap::_loadIrradianceMap(const of::filesystem::path & aCachePath) {
	
	if(data->bIrradianceAllocated) {
		return false;
	}
		
	ofLogVerbose("ofCubeMap :: _loadIrradianceMap : does file exist: ") << ofFile::doesFileExist(aCachePath);
	if( !ofFile::doesFileExist(aCachePath) ) {
		return false;
	}
	
	ofFloatPixels fullPix;
	if( !ofLoadImage( fullPix, aCachePath )) {
		ofLogError("ofCubeMap :: _loadIrradianceMap : unable to load from ") << aCachePath;
		return false;
	}
	
	if( !data->bIrradianceAllocated ) {
		data->bIrradianceAllocated = true;
		glGenTextures(1, &data->irradianceMapId );
		retain(data->irradianceMapId);
	}
	
//	_configureCubeTextures(data->irradianceMapId, false);
	
	int texSize = fullPix.getWidth() / 3;
	
	ofFloatPixels fpix;
	size_t numChannels = getNumPixelChannels();
	GLenum textureTarget = getTextureTarget();
	
	ofLogVerbose() << "ofCubeMap :: _loadIrradianceMap : num channels: " << numChannels;
	
	glBindTexture(textureTarget, data->irradianceMapId );
	
	GLuint loadTexFormat = texFormat;
#if defined(TARGET_EMSCRIPTEN)
	loadTexFormat = GL_RGBA32F;
#endif
	
	GLuint texStorageFormat = getTexStorageFormat(loadTexFormat);
	GLuint gFormat = getGlTypeFromInternalFormat(loadTexFormat);
	
	for(unsigned int j = 0; j < 6; j++ ) {
		//cropTo(ofPixels_<PixelType> &toPix, size_t x, size_t y, size_t _width, size_t _height)
		fullPix.cropTo( fpix, (j % 3) * texSize, floor(j / 3) * texSize, texSize, texSize );
		if( fpix.getNumChannels() != numChannels ) {
			fpix.setNumChannels(numChannels);
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, loadTexFormat, fpix.getWidth(), fpix.getHeight(), 0, gFormat, texStorageFormat, fpix.getData() );
	}
	glBindTexture(textureTarget, 0 );
	
	_configureCubeTextures(data->irradianceMapId, false);
	
	return true;
}

//--------------------------------------------------------------
void ofCubeMap::_createPrefilteredCubeMap(GLuint aSrcCubeFid, int aSrcRes, bool aBMakeCache, const of::filesystem::path & aCachePath) {
	if(data->bPreFilteredMapAllocated) {
		return;
	}
	
	_allocateCubeMesh();
	data->bPreFilteredMapAllocated = true;
	glGenTextures(1, &data->preFilteredMapId );
	retain(data->preFilteredMapId);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, data->preFilteredMapId);

	GLuint texStorageFormat = getTexStorageFormat();
	GLuint gFormat = getGlTypeFromInternalFormat();
	
	// generate all of the textures and mip maps at once ...
	//glTexStorage2D(GL_TEXTURE_CUBE_MAP, data->maxMipLevels, texFormat, data->settings.preFilterRes, data->settings.preFilterRes);
	// create all of the textures with mip maps //
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	_initEmptyTextures( data->preFilteredMapId, texFormat, data->settings.preFilterRes, data->maxMipLevels );
	
	_configureCubeTextures(data->preFilteredMapId, true);
	
	if( !shaderPreFilterMap.isLoaded() ) {
		auto psource = ofCubeMapShaders::prefilter();
		shaderPreFilterMap.setupShaderFromSource(GL_VERTEX_SHADER, psource.vertShader );
		shaderPreFilterMap.setupShaderFromSource(GL_FRAGMENT_SHADER, psource.fragShader );
		shaderPreFilterMap.bindDefaults();
		shaderPreFilterMap.linkProgram();
	}
	
	std::vector<glm::mat4> views = _getViewMatrices( glm::vec3(0,0,0) );
	
	if( aBMakeCache ) {
		ofLogVerbose("ofCubeMap :: _createPrefilteredCubeMap : making cache");
		ofFboSettings fboSettings;
		ofFbo cacheFbo;
		fboSettings.width = data->settings.preFilterRes * 3;
		fboSettings.height = fboSettings.width;
		fboSettings.numSamples = 0;
		fboSettings.useDepth = false;
		fboSettings.internalformat = texFormat;
		
		cacheFbo.allocate(fboSettings);
		cacheFbo.begin(); {
			ofClear(255, 0, 0);
		} cacheFbo.end();
		
		ofFbo tfbo;
		
		vector<ofFloatPixels> fpixels;
		fpixels.assign(6, ofFloatPixels() );
		// bool bAllPixelsCreated = true;
		
		int shiftX = 0;
		int shiftY = 0;
		
		for (int mip = 0; mip < data->maxMipLevels; mip++) {
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth  = static_cast<unsigned int>(data->settings.preFilterRes * std::pow(0.5, mip));
			if(mipWidth < 1 ) {
				mipWidth = 1;
			}
			fboSettings.width = mipWidth;
			fboSettings.height = fboSettings.width;
			tfbo.clear();
			tfbo.allocate(fboSettings);
			
			float roughness = (float)mip / (float)(data->maxMipLevels - 1);
			
			if( mip > 0 ) {
				shiftY = data->settings.preFilterRes * 2;
			}
			
			for (unsigned int i = 0; i < 6; ++i) {
				tfbo.begin();
				ofClear(0);
				shaderPreFilterMap.begin();
				shaderPreFilterMap.setUniformTexture("environmentMap", getTextureTarget(), aSrcCubeFid, 0 );
				shaderPreFilterMap.setUniformMatrix4f("uProjection", projectionMat );
				shaderPreFilterMap.setUniform1f("resolution", (float)aSrcRes );
				shaderPreFilterMap.setUniform1f("uroughness", roughness);
				shaderPreFilterMap.setUniformMatrix4f( "uView", views[i] );
				sCubeMesh.draw();
				shaderPreFilterMap.end();
				tfbo.end();
				
				tfbo.readToPixels(fpixels[i]);
				if( fpixels[i].getWidth() < 1 || fpixels[i].getHeight() < 1 ) {
					// bAllPixelsCreated = false;
				} else {
					cacheFbo.begin();
					tfbo.getTexture().draw( (i%3) * mipWidth + shiftX, floor(i/3) * mipWidth + shiftY, mipWidth, mipWidth );
					cacheFbo.end();
				}
			}
			
			if( mip > 0 ) {
				shiftX += mipWidth * 3;
			}
			
			glBindTexture(GL_TEXTURE_CUBE_MAP, data->preFilteredMapId);
			for (unsigned int i = 0; i < 6; ++i) {
				if( fpixels[i].getWidth() > 0 && fpixels[i].getHeight() > 0 ) {
//					// must use glTexSubImage with glTexStorage2D
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip, 0, 0, mipWidth, mipWidth, gFormat, texStorageFormat,fpixels[i].getData());

				}
			}
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
		
		ofFloatPixels cachePix;
		cacheFbo.readToPixels(cachePix);
		if( cachePix.getWidth() > 0 ) {
			if( !ofSaveImage(cachePix, aCachePath) ) {
				ofLogError("ofCubeMap :: _createPrefilteredCubeMap: ") << aCachePath;
			}
		}
		
	} else {
		
		unsigned int captureFBO;
		glGenFramebuffers(1, &captureFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO );
		
		for (int mip = 0; mip < data->maxMipLevels; mip++) {
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth  = static_cast<unsigned int>(data->settings.preFilterRes * std::pow(0.5, mip));
			ofPushView();
			ofViewport(0, 0, mipWidth, mipWidth, false);
			shaderPreFilterMap.begin();
			shaderPreFilterMap.setUniformTexture("environmentMap", getTextureTarget(), aSrcCubeFid, 0 );
			shaderPreFilterMap.setUniformMatrix4f("uProjection", projectionMat );
			shaderPreFilterMap.setUniform1f("resolution", (float)aSrcRes );
			
			float roughness = (float)mip / (float)(data->maxMipLevels - 1);
			shaderPreFilterMap.setUniform1f("uroughness", roughness);
			for (unsigned int i = 0; i < 6; ++i) {
				shaderPreFilterMap.setUniformMatrix4f( "uView", views[i] );
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, data->preFilteredMapId, mip);
				ofClear(0, 0, 0);
				sCubeMesh.draw();
			}
			shaderPreFilterMap.end();
			ofPopView();
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &captureFBO);
	}
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

//--------------------------------------------------------------
bool ofCubeMap::_loadPrefilterMap( const of::filesystem::path & aCachePath ) {
	if(data->bPreFilteredMapAllocated) {
		return false;
	}
	
	ofLogVerbose("ofCubeMap :: _loadPrefilterMap : does file exist: ") << ofFile::doesFileExist(aCachePath);
	if( !ofFile::doesFileExist(aCachePath) ) {
		return false;
	}
	
	ofFloatPixels fullPix;
	if( !ofLoadImage( fullPix, aCachePath )) {
		ofLogError("ofCubeMap :: _loadPrefilterMap : unable to load from ") << aCachePath;
		return false;
	}
	
	_allocateCubeMesh();
	data->bPreFilteredMapAllocated = true;
	glGenTextures(1, &data->preFilteredMapId );
	retain(data->preFilteredMapId);
	
	
	GLuint loadTexFormat = texFormat;
#if defined(TARGET_EMSCRIPTEN)
	loadTexFormat = GL_RGBA32F;
#endif
	
	GLuint texStorageFormat = getTexStorageFormat(loadTexFormat);
	GLuint gFormat = getGlTypeFromInternalFormat(loadTexFormat);
	
	_initEmptyTextures( data->preFilteredMapId, loadTexFormat, data->settings.preFilterRes, data->maxMipLevels );
	
	//glBindTexture(GL_TEXTURE_CUBE_MAP, data->preFilteredMapId);
	//glTexStorage2D(GL_TEXTURE_CUBE_MAP, data->maxMipLevels, loadTexFormat, data->settings.preFilterRes, data->settings.preFilterRes);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	_configureCubeTextures(data->preFilteredMapId, true);
		
	ofFloatPixels fpix;
	size_t numChannels = getNumPixelChannels();
	
	float shiftX = 0.0f;
	float shiftY = 0.0f;
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, data->preFilteredMapId );
	for (int mip = 0; mip < data->maxMipLevels; mip++) {
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth  = static_cast<unsigned int>(data->settings.preFilterRes * std::pow(0.5, mip));
		if(mipWidth < 1 ) {mipWidth = 1;}
		
		if( mip > 0 ) {
			shiftY = data->settings.preFilterRes * 2;
		}
		
		for (unsigned int i = 0; i < 6; ++i) {
			fullPix.cropTo( fpix, (i % 3) * mipWidth + shiftX, floor(i / 3) * mipWidth + shiftY, mipWidth, mipWidth );
			if( fpix.getNumChannels() != numChannels ) {
				fpix.setNumChannels(numChannels);
			}
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip, 0, 0, mipWidth, mipWidth, gFormat, texStorageFormat,fpix.getData());
		}
		
		if( mip > 0 ) {
			shiftX += mipWidth * 3;
		}
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;
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
	return getTexStorageFormat(texFormat);
}

//--------------------------------------------------------------
GLuint ofCubeMap::getTexStorageFormat(GLuint aInternalFormat) {
	#if !defined(TARGET_OPENGLES) || defined(TARGET_EMSCRIPTEN)
	if( aInternalFormat == GL_RGB32F ) {
		return GL_FLOAT;
	} else if( aInternalFormat == GL_RGBA32F ) {
		return GL_FLOAT;
	} else if( aInternalFormat == GL_RGBA16F ) {
		return GL_HALF_FLOAT;
	}
	#ifdef GL_RGBA32F_EXT
	if( aInternalFormat == GL_RGBA32F_EXT ) {
		return GL_FLOAT;
	}
	#endif
	#ifdef GL_RGB32F_EXT
	if( aInternalFormat == GL_RGB32F_EXT ) {
		return GL_FLOAT;
	}
	#endif
	#endif
	return GL_UNSIGNED_BYTE;
}

//--------------------------------------------------------------
GLuint ofCubeMap::getGlTypeFromInternalFormat() {
	return getGlTypeFromInternalFormat(texFormat);
}

//--------------------------------------------------------------
GLuint ofCubeMap::getGlTypeFromInternalFormat(GLuint aInternalFormat) {
	#if !defined(TARGET_OPENGLES) || defined(TARGET_EMSCRIPTEN)
		if( aInternalFormat == GL_RGBA32F ) {
			return GL_RGBA;
		}
		if( aInternalFormat == GL_RGBA16F ) {
			return GL_RGBA;
		}
	
		#ifdef GL_RGBA32F_EXT
		if( aInternalFormat == GL_RGBA32F_EXT ) {
			return GL_RGBA;
		}
		#endif
		#ifdef GL_RGB32F_EXT
		if( aInternalFormat == GL_RGB32F_EXT ) {
			return GL_RGB;
		}
		#endif
	#endif
	return GL_RGB;
}

//--------------------------------------------------------------
int ofCubeMap::getNumPixelChannels() {
	GLuint glType = getGlTypeFromInternalFormat();
	if( glType == GL_RGBA ) {
		return 4;
	}
	return 3;
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

//--------------------------------------------------------------
bool ofCubeMap::_loadEquiRectToCubeMapShader() {
	if( !shaderEquiRectToCubeMap.isLoaded() ) {
		auto esource = ofCubeMapShaders::equiRectToCubeMap();
		shaderEquiRectToCubeMap.setupShaderFromSource(GL_VERTEX_SHADER, esource.vertShader );
		shaderEquiRectToCubeMap.setupShaderFromSource(GL_FRAGMENT_SHADER, esource.fragShader );
		shaderEquiRectToCubeMap.bindDefaults();
		if(shaderEquiRectToCubeMap.linkProgram()) {
			return true;
		} else {
			shaderEquiRectToCubeMap.unload();
			ofLogNotice("ofCubeMap::_loadEquiRectToCubeMapShader : unable to create shaderEquiRectToCubeMap shader ");
			return false;
		}
	}
	return true;
}
