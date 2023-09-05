//
//  ofShadow.cpp
//  openFrameworksLib
//
//  Created by Nick Hardeman on 10/3/22.
//

#include "ofShadow.h"
#include "of3dUtils.h"
#include "ofGLBaseTypes.h"
#include "ofGLUtils.h"
#include "ofLight.h"
#include "ofGLProgrammableRenderer.h"
#include "ofConstants.h"

#define GLM_FORCE_CTOR_INIT
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"

using std::weak_ptr;
using std::vector;
using std::shared_ptr;


//----------------------------------------
vector<weak_ptr<ofShadow::Data> > & ofShadowsData(){
	static vector<weak_ptr<ofShadow::Data> > * shadowsActive = ofIsGLProgrammableRenderer()?new vector<weak_ptr<ofShadow::Data> >:new vector<weak_ptr<ofShadow::Data> >(8);
	return *shadowsActive;
}

//--------------------------------------------------------------
static std::map< int, ofShadow::GLData > & getGLDatas(){
	static std::map< int,ofShadow::GLData > * idsFB = new std::map< int,ofShadow::GLData >;
	return *idsFB;
}

//--------------------------------------------------------------
static ofShadow::GLData& getGLData( int aLightType ) {
	if( getGLDatas().count(aLightType) < 1 ) {
		getGLDatas()[aLightType] = ofShadow::GLData();
	}
	return getGLDatas()[aLightType];
}

//--------------------------------------------------------------
static void retainFBO(int aLightType){
	if( !getGLData(aLightType).bAllocated ) {
		ofLogVerbose("ofShadow :: retainFBO : for light ") << aLightType << " | " << ofGetFrameNum();
		getGLData(aLightType).bAllocated = true;
		glGenFramebuffers(1, &getGLDatas()[aLightType].fboId);
		glGenTextures(1, &getGLDatas()[aLightType].texId);
	}
}

//--------------------------------------------------------------
static GLuint getFBOId( int aLightType ) {
	retainFBO(aLightType);
	return getGLData(aLightType).fboId;
}
//--------------------------------------------------------------
static GLuint getFBODepthTexId( int aLightType ) {
	retainFBO(aLightType);
	return getGLData(aLightType).texId;
}

//--------------------------------------------------------------
static void releaseFBO(int aLightType){
	if( getGLData(aLightType).bAllocated ){
		ofLogVerbose("ofShadow :: releaseFBO : for light ") << aLightType << " | " << ofGetFrameNum();
		glDeleteTextures(1, &getGLDatas()[aLightType].texId );
		glDeleteFramebuffers(1, &getGLDatas()[aLightType].fboId );
		getGLDatas()[aLightType].bAllocated = false;
		getGLDatas()[aLightType].bFboAllocated = false;
	}
}

//----------------------------------------
int ofShadow::getNumTotalPossibleShadows( int aLightType ) {
	
	GLenum texTarget = getTextureTarget( aLightType );
	if( texTarget == GL_TEXTURE_CUBE_MAP) {
		return 1;
	}
	if( texTarget == GL_TEXTURE_2D ) {
		return 1;
	}
	// a bit of an arbitrary selection here
	return 64;
}

//----------------------------------------
GLenum ofShadow::getTextureTarget( int aLightType ) {
	
//	#if !defined(TARGET_OPENGLES)
	if( aLightType == OF_LIGHT_POINT ) {
		#if !defined(TARGET_OPENGLES)
		if( ofGetGLRenderer() && ofGetGLRenderer()->getGLVersionMajor() < 4 ) {
			// does not support cube map arrays in openGL < 4 glsl shader
			return GL_TEXTURE_CUBE_MAP;
		}
		#else
		return GL_TEXTURE_CUBE_MAP;
		#endif
	}
//	#endif
	
	if( aLightType == OF_LIGHT_POINT ) {
		#ifdef GL_TEXTURE_CUBE_MAP_ARRAY
		#ifdef glTexImage3D
		return GL_TEXTURE_CUBE_MAP_ARRAY;
		#endif
		#endif
		return GL_TEXTURE_CUBE_MAP;
	}
	#ifdef GL_TEXTURE_2D_ARRAY
	#ifdef glTexImage3D
	return GL_TEXTURE_2D_ARRAY;
	#endif
	#endif
	return GL_TEXTURE_2D;
}

//--------------------------------------------------------------
void ofShadow::setDepthMapResolution( int aLightType, int ares ) {
	setDepthMapResolution( aLightType, ares, ares );
}

//--------------------------------------------------------------
void ofShadow::setDepthMapResolution( int aLightType, int awidth, int aheight ) {
	if( aLightType == OF_LIGHT_POINT ) {
		// rendering to a cube, so needs to be the same
		aheight = awidth;
	}
	
	ofLogVerbose("ofShadow :: setDepthMapResolution : incoming: " ) << awidth << " x " << aheight << " current: " << getDepthMapWidth(aLightType) << " x " << getDepthMapHeight(aLightType) << " | " << ofGetFrameNum();
	
	if( awidth != getDepthMapWidth(aLightType) || aheight != getDepthMapHeight(aLightType) ) {
		getGLData(aLightType).width = awidth;
		getGLData(aLightType).height = aheight;
		releaseFBO(aLightType);
	}
}

//--------------------------------------------------------------
int ofShadow::getDepthMapWidth(int aLightType) {
	return getGLData(aLightType).width;
}

//--------------------------------------------------------------
int ofShadow::getDepthMapHeight(int aLightType) {
	return getGLData(aLightType).height;
}

//--------------------------------------------------------------
GLuint ofShadow::getPointTexId() {
	_updateTexDataIds();
	return getFBODepthTexId(OF_LIGHT_POINT);
}

//--------------------------------------------------------------
GLuint ofShadow::getDirectionalTexId() {
	_updateTexDataIds();
	return getFBODepthTexId(OF_LIGHT_DIRECTIONAL);
}

//--------------------------------------------------------------
GLuint ofShadow::getSpotTexId() {
	_updateTexDataIds();
	return getFBODepthTexId(OF_LIGHT_SPOT);
}

//--------------------------------------------------------------
GLuint ofShadow::getAreaTexId() {
	_updateTexDataIds();
	return getFBODepthTexId(OF_LIGHT_AREA);
}

//--------------------------------------------------------------
std::string ofShadow::getShadowTypeAsString( ofShadowType atype ) {
	switch( atype ) {
		case OF_SHADOW_TYPE_HARD:
			return "Hard";
		case OF_SHADOW_TYPE_PCF_LOW:
			return "PCF Low";
		case OF_SHADOW_TYPE_PCF_MED:
			return "PCF Medium";
		case OF_SHADOW_TYPE_PCF_HIGH:
			return "PCF High";
		case OF_SHADOW_TYPE_TOTAL:
			return "Total";
	}
	return "Default";
}

//--------------------------------------------------------------
bool ofShadow::hasActiveShadows() {
	for(size_t i=0;i< ofShadowsData().size();i++){
		std::shared_ptr<ofShadow::Data> shadow = ofShadowsData()[i].lock();
		if(shadow && shadow->isEnabled && shadow->index > -1 ){
			return true;
			break;
		}
	}
	return false;
}

//--------------------------------------------------------------
void ofShadow::enableAllShadows() {
	if( !areShadowsSupported() ) {
		ofLogWarning("ofShadow :: enableAllShadows : only works with programmable renderer.");
		return;
	}
	
	for(size_t i=0;i<ofShadowsData().size();i++){
		std::shared_ptr<ofShadow::Data> shadow = ofShadowsData()[i].lock();
		if(!shadow || shadow->index < 0 ){
			continue;
		}
		shadow->isEnabled = true;
	}
}

//--------------------------------------------------------------
void ofShadow::disableAllShadows() {
	for(size_t i=0;i<ofShadowsData().size();i++){
		std::shared_ptr<ofShadow::Data> shadow = ofShadowsData()[i].lock();
		if(!shadow || shadow->index < 0 ){
			continue;
		}
		shadow->isEnabled = false;
	}
}

//--------------------------------------------------------------
void ofShadow::setAllShadowTypes( ofShadowType atype ) {
	if( !ofIsGLProgrammableRenderer() ) {
		ofLogWarning("ofShadow :: setAllShadowTypes : only works with programmable renderer.");
		return;
	}
	for(size_t i=0;i<ofShadowsData().size();i++){
		std::shared_ptr<ofShadow::Data> shadow = ofShadowsData()[i].lock();
		if(!shadow || shadow->index < 0 ){
			continue;
		}
		shadow->shadowType = atype;
	}
}

//--------------------------------------------------------------
void ofShadow::setAllShadowDepthResolutions(int awidth, int aheight ) {
	if( !ofIsGLProgrammableRenderer() ) {
		ofLogWarning("ofShadow :: setAllShadowDepthResolutions : only works with programmable renderer.");
		return;
	}
	setDepthMapResolution( OF_LIGHT_POINT, awidth, aheight );
	setDepthMapResolution( OF_LIGHT_DIRECTIONAL, awidth, aheight );
	setDepthMapResolution( OF_LIGHT_SPOT, awidth, aheight );
	setDepthMapResolution( OF_LIGHT_AREA, awidth, aheight );
}

//--------------------------------------------------------------
void ofShadow::setAllShadowBias( float bias ) {
	if( !ofIsGLProgrammableRenderer() ) {
		ofLogWarning("ofShadow :: setAllShadowBias : only works with programmable renderer.");
		return;
	}
	for(size_t i=0;i<ofShadowsData().size();i++){
		std::shared_ptr<ofShadow::Data> shadow = ofShadowsData()[i].lock();
		if(!shadow || shadow->index < 0 ){
			continue;
		}
		shadow->bias = bias;
	}
}

//--------------------------------------------------------------
void ofShadow::setAllShadowNormalBias( float normalBias ) {
	if( !ofIsGLProgrammableRenderer() ) {
		ofLogWarning("ofShadow :: setAllShadowNormalBias : only works with programmable renderer.");
		return;
	}
	for(size_t i=0;i<ofShadowsData().size();i++){
		std::shared_ptr<ofShadow::Data> shadow = ofShadowsData()[i].lock();
		if(!shadow || shadow->index < 0 ){
			continue;
		}
		shadow->normalBias = normalBias;
	}
}

//--------------------------------------------------------------
void ofShadow::setAllShadowSampleRadius( float sampleRadius ) {
	if( !ofIsGLProgrammableRenderer() ) {
		ofLogWarning("ofShadow :: enableAllShadows : only works with programmable renderer.");
		return;
	}
	for(size_t i=0;i<ofShadowsData().size();i++){
		std::shared_ptr<ofShadow::Data> shadow = ofShadowsData()[i].lock();
		if(!shadow || shadow->index < 0 ){
			continue;
		}
		shadow->sampleRadius = sampleRadius;
	}
}

//--------------------------------------------------------------
std::string ofShadow::getShaderDefinesAsString() {
	std::string definesString = "";
	if( areShadowsSupported() ) {
		if( ofShadowsData().size() > 0 && ofLightsData().size() > 0) {
			definesString += "#define HAS_SHADOWS 1\n";
		}
		
		if( ofShadow::getTextureTarget( OF_LIGHT_POINT ) != GL_TEXTURE_CUBE_MAP ) {
			definesString += "#define SHADOWS_USE_CUBE_MAP_ARRAY 1\n";
		}
		
		if( ofShadow::getTextureTarget( OF_LIGHT_DIRECTIONAL ) != GL_TEXTURE_2D ) {
			definesString += "#define SHADOWS_USE_TEXTURE_ARRAY 1\n";
		}
	}
	return definesString;
}

//--------------------------------------------------------------
bool ofShadow::areShadowsSupported() {
	#if defined(TARGET_OPENGLES) && !defined(TARGET_EMSCRIPTEN)
	return false;
	#endif
	
	if(!ofIsGLProgrammableRenderer() ) {
		return false;
	}
	return true;
}

//--------------------------------------------------------------
void ofShadow::_updateTexDataIds() {
	std::map<int, int> texIdMap;
	
	for(size_t i=0;i< ofShadowsData().size();i++){
		std::shared_ptr<ofShadow::Data> shadow = ofShadowsData()[i].lock();
		if(!shadow || !shadow->isEnabled || shadow->index < 0 ){
			continue;
		}
		
		if( texIdMap.count((int)shadow->lightType) < 0 ) {
			texIdMap[(int)shadow->lightType] = 0;
		}
		shadow->texIndex = texIdMap[(int)shadow->lightType];
		
		texIdMap[(int)shadow->lightType]++;
	}
}

//--------------------------------------------------------------
ofShadow::ofShadow() {
	data = std::make_shared<ofShadow::Data>();
	data->shadowMatrix = glm::mat4(1.0f);
	setSingleOmniPass(true);
#if defined(TARGET_OPENGLES)
	setSingleOmniPass(false);
#endif
	_checkSetup();
}

//--------------------------------------------------------------
ofShadow::~ofShadow() {
	clear();
}

//--------------------------------------------------------------
void ofShadow::setLightType( int atype ) {
	
	if( (ofLightType)atype != data->lightType ) {
		clear();
	}
	data->lightType = atype;
	data->numDepthPasses = getNumShadowDepthPasses();
	if( getIsEnabled() ) {
		_checkFbos();
		_allocate();
	}
}

//--------------------------------------------------------------
void ofShadow::update( const ofLight& alight ) {
	
	if( !data->isEnabled ) {
		return;
	}
	
	glm::quat rq = alight.getGlobalOrientation();
	glm::vec3 lookAtDir(glm::normalize( rq * glm::vec4(0.f,0.f,-1.f, 1.f)));
	
	setLightType( (ofLightType)alight.getType() );
	
	data->position = alight.getGlobalPosition();
	data->direction = glm::normalize(lookAtDir);
	data->up = rq * glm::vec3(0.0,1.0,0.0);
	data->right = rq * glm::vec3(1.0,0.0,0.0);
	
	unsigned int targetNumMatrices = 1;
	if(data->lightType == OF_LIGHT_POINT) {
		targetNumMatrices = 6;
	}
	
	if( mLookAtMats.size() != targetNumMatrices ) {
		mLookAtMats.clear();
		mLookAtMats.assign(targetNumMatrices, glm::mat4(1.0) );
	}
	if( mViewProjMats.size() != targetNumMatrices ) {
		mViewProjMats.clear();
		mViewProjMats.assign( targetNumMatrices, glm::mat4(1.0) );
	}
	
	if( data->lightType == OF_LIGHT_SPOT || data->lightType == OF_LIGHT_AREA ) {
		if(data->lightType == OF_LIGHT_SPOT) {
			mFov = alight.getSpotlightCutOff() * 2.0f;
		}
		float aspectRatio = (float)getDepthMapWidth() / (float)getDepthMapHeight();
		if( data->lightType == OF_LIGHT_AREA ) {
			mFov = 90;
			aspectRatio = mAreaLightWidth / mAreaLightHeight;
		}
		
		mShadowProjection = glm::perspective(glm::radians(mFov), aspectRatio, getNearClip(), getFarClip());
		mLookAtMats[0] = glm::lookAt( data->position, data->position + data->direction, glm::vec3(0.0, 1.0, 0.0) );
		mViewProjMats[0] = mShadowProjection * mLookAtMats[0];
		data->shadowMatrix = biasMatrix * mViewProjMats[0];
	} else if( data->lightType == OF_LIGHT_DIRECTIONAL ) {
		
		mOrthoScaleX = 1.0;
		if( mDirectionalBoundsWidth > -1 ) {
			mOrthoScaleX = mDirectionalBoundsWidth / (float)getDepthMapWidth();
		}
		
		mOrthoScaleY = 1.0;
		if( mDirectionalBoundsHeight > -1 ) {
			mOrthoScaleY = mDirectionalBoundsHeight / (float)getDepthMapHeight();
		}
		
		float viewWidth = getGLData(data->lightType).width;
		float viewHeight = getGLData(data->lightType).height;//
		mShadowProjection = glm::ortho(
				   - viewWidth/2 * mOrthoScaleX,
				   + viewWidth/2 * mOrthoScaleX,
				   - viewHeight/2 * mOrthoScaleY,
				   + viewHeight/2 * mOrthoScaleY,
									   getNearClip(),
									   getFarClip()
									   );
		mLookAtMats[0] = glm::lookAt( data->position, data->position + data->direction, glm::vec3(0.0, 1.0, 0.0) );
		mViewProjMats[0] = mShadowProjection * mLookAtMats[0];
		data->shadowMatrix = biasMatrix * mViewProjMats[0];
		
	} else if( data->lightType == OF_LIGHT_POINT ) {
		mFov = 90.f;
		// aspect is 1.0 since width and height should be the same
		mShadowProjection = glm::perspective(glm::radians(mFov), 1.0f, getNearClip(), getFarClip() );
		// eye, target and up vector
		// +x, -x, +y, -y, +z and -z direction
		mLookAtMats[0] = glm::lookAt( data->position, data->position + glm::vec3(1,0,0), glm::vec3(0, -1, 0) );
		mLookAtMats[1] = glm::lookAt( data->position, data->position + glm::vec3(-1,0,0), glm::vec3(0, -1, 0) );
		mLookAtMats[2] = glm::lookAt( data->position, data->position + glm::vec3(0,1,0), glm::vec3(0, 0, 1) );
		mLookAtMats[3] = glm::lookAt( data->position, data->position + glm::vec3(0,-1,0), glm::vec3(0, 0, -1) );
		mLookAtMats[4] = glm::lookAt( data->position, data->position + glm::vec3(0,0,1), glm::vec3(0, -1, 0) );
		mLookAtMats[5] = glm::lookAt( data->position, data->position + glm::vec3(0,0,-1), glm::vec3(0, -1, 0) );
		
		for( size_t i = 0; i < 6; i++ ) {
			mViewProjMats[i] = mShadowProjection * mLookAtMats[i];
		}
	}
}

//----------------------------------------------------
bool ofShadow::beginDepth() {
	
	if( !areShadowsSupported() ) {
		ofLogWarning("ofShadow :: beginDepth() : shadows only work with programmable renderer.");
		setEnabled(false);
	}
	
	if( !getIsEnabled() ) {
		return false;
	}
	
	_allocateFbo();
	
	auto glRenderer = ofGetGLRenderer();
	if(!glRenderer){
		return false;
	}
	
	if( data->lightType == OF_LIGHT_POINT ) {
		if( !isSingleOmniPass() ) {
			ofLogWarning("ofShadow :: beginDepth : must call beginDepth(cubeFace) when using point light without single pass.");
			return false;
		}
	}
	
	if( data->texIndex+1 > getNumTotalPossibleShadows(data->lightType) ) {
		ofLogWarning( "ofShadow :: too many shadows detected for light type " ) << data->lightType<<". Total supported for light type: " << getNumTotalPossibleShadows(data->lightType);
	}
	
//
	
	glBindFramebuffer( GL_FRAMEBUFFER, getDepthMapFboId() );
	if( data->lightType == OF_LIGHT_POINT ) {
		// handled in the depthCubeGeom shader
//		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, getDepthMapTexId(), 0, (data->texIndex * 6));
	} else {
		#ifdef glFramebufferTextureLayer
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, getDepthMapTexId(), 0, data->texIndex );
		#else
		
		auto texTarget = getTextureTarget( data->lightType );
		if( texTarget == GL_TEXTURE_2D ) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, getDepthMapTexId(), 0);
		} else {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, data->texIndex, getDepthMapTexId(), 0);
		}
		
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, data->texIndex, getDepthMapTexId(), 0);
		#endif
	}
	
	ofPushView();
	ofViewport(0, 0, getDepthMapWidth(), getDepthMapHeight(), false);
	
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glRenderer->bind(*this);
	
	if( isGlCullingEnabled() ) {
		glEnable(GL_CULL_FACE); // enables face culling
		glFrontFace(mGlFrontFaceWindingOrder);
		glCullFace(GL_FRONT); // tells OpenGL to cull front faces
	}
	return true;
}

//--------------------------------------------------------------
bool ofShadow::endDepth() {
	if( !getIsEnabled() || !areShadowsSupported() ) {
		return false;
	}
	if( isGlCullingEnabled() ) {
		glDisable(GL_CULL_FACE);
	}
	
	auto glRenderer = ofGetGLRenderer();
	if(glRenderer){
		glRenderer->unbind(*this);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ofPopView();
	return true;
}

//--------------------------------------------------------------
bool ofShadow::beginDepth(GLenum aCubeFace) {
	if( !areShadowsSupported() ) {
		ofLogWarning("ofShadow :: beginDepth(cubeFace) : shadows are only available with programmable renderer.");
		setEnabled(false);
	}
	
	if( !getIsEnabled() ) {
		return false;
	}
	
	_allocateFbo();
	
	if( data->lightType != OF_LIGHT_POINT ) {
		ofLogWarning("ofShadow :: beginDepth(cubeFace) called from a light that does not use cube map. Use beginDepth() instead.");
		return false;
	}
	if( isSingleOmniPass() ) {
		ofLogWarning("ofShadow :: beginDepth(cubeFace) called using single pass, should be calling beginDepth().");
		return false;
	}
	
	if( data->texIndex+1 > getNumTotalPossibleShadows(data->lightType) ) {
		ofLogWarning( "ofShadow :: too many shadows detected for light type " ) << data->lightType<<". Total supported for light type: " << getNumTotalPossibleShadows(data->lightType);
	}
	
//	if( data->lightType == OF_LIGHT_POINT ) {
//		glEnable(GL_TEXTURE_CUBE_MAP_ARRAY);
//	}
	
	const auto glRenderer = ofGetGLRenderer();
	if(!glRenderer){
		return false;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, getDepthMapFboId());
	if( getTextureTarget(OF_LIGHT_POINT) != GL_TEXTURE_CUBE_MAP ){
		#ifdef glFramebufferTextureLayer
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, getDepthMapTexId(), 0, (data->texIndex * 6) + aCubeFace);
		#else
		ofLogWarning("ofShadow::beginDepth(GLenum aCubeFace) point light texture target needs to be GL_TEXTURE_CUBE_MAP");
		#endif
	} else {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + aCubeFace, getDepthMapTexId(), 0);
	}
//	glViewport(0, 0, getDepthMapWidth(), getDepthMapHeight() );
	ofPushView();
	ofViewport(0, 0, getDepthMapWidth(), getDepthMapHeight(), false);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glRenderer->bind(*this,aCubeFace);
		
	if( isGlCullingEnabled() ) {
		glEnable(GL_CULL_FACE); // enables face culling
		glFrontFace(mGlFrontFaceWindingOrder);
		glCullFace(GL_FRONT); // tells OpenGL to cull back faces (the sane default setting)
	}
	return true;
}

//--------------------------------------------------------------
bool ofShadow::endDepth(GLenum aCubeFace) {
	return endDepth();
}

//--------------------------------------------------------------
void ofShadow::clear() {
	_checkFbos();
	
	mLookAtMats.clear();
	mViewProjMats.clear();
	
}

//--------------------------------------------------------------
void ofShadow::setEnabled( bool ab ) {
	if( !areShadowsSupported() ) {
		ofLogWarning("ofShadow :: setEnabled : shadows only work with programmable renderer");
		ab = false;
	}
	
	data->isEnabled = ab;
	if( data->isEnabled ) {
		_allocate();
	}
}

//--------------------------------------------------------------
const bool ofShadow::isMultiCubeFacePass() const {
	if( data->lightType == OF_LIGHT_POINT ) {
		return !isSingleOmniPass();
	}
	return false;
}

//--------------------------------------------------------------
const bool ofShadow::isSingleOmniPass() const {
	return mBSinglePass;
}

//--------------------------------------------------------------
const int ofShadow::getNumShadowDepthPasses() const {
	if(isMultiCubeFacePass()) {
		return 6;
	}
	return 1;
}

//--------------------------------------------------------------
void ofShadow::setSingleOmniPass( bool ab ) {
	
	if( ofGetGLRenderer() && ofGetGLRenderer()->getGLVersionMajor() < 4 ) {
		ab = false;
	}
	
#ifdef TARGET_OPENGLES
	// setting to false because the single pass uses a geometry shader
	ab = false;
#endif
	
	if(mBSinglePass != ab ) {
		clear();
	}

	mBSinglePass = ab;
	data->numDepthPasses = getNumShadowDepthPasses();
}

//--------------------------------------------------------------
int ofShadow::getDepthMapWidth() {
	return getDepthMapWidth( data->lightType );
}

//--------------------------------------------------------------
int ofShadow::getDepthMapHeight() {
	return getDepthMapHeight( data->lightType );
}

//--------------------------------------------------------------
void ofShadow::setDirectionalBounds( float aWorldWidth, float aWorldHeight ) {
	mDirectionalBoundsWidth = aWorldWidth;
	mDirectionalBoundsHeight = aWorldHeight;
}

//--------------------------------------------------------------
GLuint ofShadow::getDepthMapFboId() {
	return getFBOId(data->lightType);
}

//--------------------------------------------------------------
GLuint ofShadow::getDepthMapTexId() {
	return getFBODepthTexId(data->lightType);
}

//--------------------------------------------------------------
void ofShadow::drawFrustum() {
	if( data->lightType == OF_LIGHT_POINT ) {
		_drawFrustum( data->up, data->right, data->direction );
		_drawFrustum( data->up, data->right, -data->direction );
		_drawFrustum( data->up, data->direction, data->right );
		_drawFrustum( data->up, data->direction, -data->right );
	} else {
		_drawFrustum( data->up, data->right, data->direction );
	}
}

//--------------------------------------------------------------
void ofShadow::_drawFrustum( const glm::vec3& aup, const glm::vec3& aright, const glm::vec3& afwd ) {
	auto corners = getFrustumCorners(aup, aright, afwd );
	
	ofPushStyle();

	ofSetColor( ofColor::green );
	ofDrawArrow( data->position, data->position+data->up * 100.0, 10.0);
	ofSetColor( ofColor::red );
	ofDrawArrow( data->position, data->position+data->right * 100.0, 10.0);
	ofSetColor( ofColor::blue );
	ofDrawArrow( data->position, data->position+data->direction * 100.0, 10.0);
	
	vector<ofFloatColor> colors;
	
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_LINES);
	
	for( int i = 0; i < 4; i++ ) {
		mesh.addVertex( corners[i] );
		if( i == 3 ) {
			mesh.addVertex(corners[0]);
		} else {
			mesh.addVertex(corners[i+1]);
		}
	}
	
	colors.assign( 8, ofColor(220, 90, 190) );
	mesh.addColors(colors);
	
	for( int i = 0; i < 4; i++ ) {
		mesh.addVertex( corners[i+4] );
		if( i == 3 ) {
			mesh.addVertex(corners[4]);
		} else {
			mesh.addVertex(corners[i+4+1]);
		}
	}
	
	colors.assign( 8, ofColor(220, 220, 90) );
	mesh.addColors(colors);
	
	
	for( int i = 0; i < 4; i++ ) {
		if( data->lightType == OF_LIGHT_SPOT || data->lightType == OF_LIGHT_AREA) {
			mesh.addVertex(data->position);
		} else {
			mesh.addVertex(corners[i+4]);
		}
		mesh.addVertex(corners[i]);
	}
	colors.assign( 8, ofColor(220) );
	mesh.addColors(colors);
	
	if( data->lightType == OF_LIGHT_DIRECTIONAL ) {
		for( int i = 0; i < 4; i++ ) {
			mesh.addVertex(data->position);
			mesh.addVertex(corners[i+4]);
		}
		colors.assign( 8, ofColor(220) );
		mesh.addColors(colors);
	}
	
	
	mesh.draw();
	
	ofPopStyle();
}

//--------------------------------------------------------------
std::vector<glm::vec3> ofShadow::getFrustumCorners( const glm::vec3& aup, const glm::vec3& aright, const glm::vec3& afwd ) {
	
	if( data->lightType == OF_LIGHT_DIRECTIONAL) {
		
		glm::vec3 fc = data->position + afwd * getFarClip();
		glm::vec3 nc = data->position + afwd * getNearClip();
		
		float viewWidth = getGLData(data->lightType).width;
		float viewHeight = getGLData(data->lightType).height;
		if( mDirectionalBoundsWidth > 0 ) {
			viewWidth = mDirectionalBoundsWidth;
		}
		if( mDirectionalBoundsHeight > 0 ) {
			viewHeight = mDirectionalBoundsHeight;
		}
		
		float hw = 0.5f * viewWidth;
		float hh = 0.5f * viewHeight;
		
		glm::vec3 X = glm::normalize(aright);
		glm::vec3 Y = glm::normalize(aup);
		
		std::vector<glm::vec3> corners(8);
		// ftl, ftr, fbl, fbr
		corners[0] = fc + (Y*hh) - (X*hw);
		corners[1] = fc + (Y*hh) + (X*hw);
		corners[2] = fc + (-Y*hh) + (X*hw);
		corners[3] = fc + (-Y*hh) - (X*hw);
		
		corners[4] = nc + (Y*hh) - (X*hw);
		corners[5] = nc + (Y*hh) + (X*hw);
		corners[6] = nc + (-Y*hh) + (X*hw);
		corners[7] = nc + (-Y*hh) - (X*hw);
		
		return corners;
		
	}
		
	glm::vec3 Z = glm::normalize(afwd);
	glm::vec3 X = glm::normalize(aright);
	glm::vec3 Y = glm::normalize(aup);
	
	glm::vec3 p = data->position;
	
	glm::vec3 nc = p + Z * getNearClip();
	glm::vec3 fc = p + Z * getFarClip();
	
	float ratio = (float)getDepthMapWidth() / (float)getDepthMapHeight();
	
	if( data->lightType == OF_LIGHT_AREA ) {
		ratio = mAreaLightWidth / mAreaLightHeight;
	}
	
	float Hnear = 2.f * tan( ofDegToRad( mFov ) / 2.f ) * getNearClip();
	float Wnear = Hnear * ratio;
	
	float Hfar = 2.f * tanf( ofDegToRad( mFov ) / 2.f ) * getFarClip();
	float Wfar = Hfar * ratio;
	
	std::vector<glm::vec3> corners(8);
	
	corners[0] = fc + ( Y * Hfar/2) - ( X * Wfar/2.f); // ftl
	corners[1] = fc + ( Y * Hfar/2) + ( X * Wfar/2); // ftr
	corners[2] = fc - ( Y * Hfar/2) + ( X * Wfar/2); // fbl
	corners[3] = fc - ( Y * Hfar/2) - ( X * Wfar/2); // fbr
	
	corners[4] = nc + ( Y * Hnear/2) - ( X * Wnear/2); // ntl
	corners[5] = nc + ( Y * Hnear/2) + ( X * Wnear/2); // ntr
	corners[6] = nc - ( Y * Hnear/2) + ( X * Wnear/2); // nbl
	corners[7] = nc - ( Y * Hnear/2) - ( X * Wnear/2); // nbr
	
	return corners;
}



//--------------------------------------------------------------
std::string ofShadow::getShadowTypeAsString() {
	return ofShadow::getShadowTypeAsString(data->shadowType);
}

//--------------------------------------------------------------
const ofShader & ofShadow::getDepthShader(ofGLProgrammableRenderer & renderer) const {
	initShaders( renderer );
	if( data->lightType == OF_LIGHT_POINT ) {
		#ifndef TARGET_OPENGLES
		if(isSingleOmniPass()) {
			return shaders[&renderer]->depthCube;
		} else {
			return shaders[&renderer]->depthCubeMultiPass;
		}
		#else
		return shaders[&renderer]->depthCubeMultiPass;
		#endif
	} else if( data->lightType == OF_LIGHT_AREA ) {
		return shaders[&renderer]->depth;
	} else {
		return shaders[&renderer]->depth;
	}
}

//--------------------------------------------------------------
void ofShadow::updateDepth(const ofShader & shader,ofGLProgrammableRenderer & renderer) const {
	shader.setUniform3f("uLightPos", data->position );
	shader.setUniform1f("uNearPlane", data->nearClip );
	shader.setUniform1f("uFarPlane", data->farClip );
	
	if( data->lightType == OF_LIGHT_POINT ) {
		if( isSingleOmniPass() ) {
			for( unsigned int i = 0; i < 6; i++ ) {
				shader.setUniformMatrix4f("light["+ std::to_string(i) +"].viewProjectionMatrix", mViewProjMats[i]);
			}
			shader.setUniform1i("uStartLayer", data->texIndex );
		} else {
			ofLogWarning("Must call beginDepth(cubeFace) to render to point light in multiple passes");
		}
	} else {
		// just in case //
		if(mViewProjMats.size() > 0 ) {
			shader.setUniformMatrix4f("lightsViewProjectionMatrix", mViewProjMats[0]);
		}
	}
}

//--------------------------------------------------------------
void ofShadow::updateDepth(const ofShader & shader,GLenum aCubeFace,ofGLProgrammableRenderer & renderer) const {
//	shader.begin();
	shader.setUniform3f("uLightPos", data->position );
	shader.setUniform1f("uNearPlane", data->nearClip );
	shader.setUniform1f("uFarPlane", data->farClip );
		
	if( aCubeFace < mViewProjMats.size() ) {
		shader.setUniformMatrix4f("lightsViewProjectionMatrix", mViewProjMats[aCubeFace] );
	}
}

//--------------------------------------------------------------
void ofShadow::_checkSetup() {
	if( data->index < 0 ) {
		bool bShadowFound = false;
		// search for the first free block
		for(size_t i=0; i<ofShadowsData().size(); i++) {
			if(ofShadowsData()[i].expired()) {
				data->index = i;
				data->isEnabled = false;
				ofShadowsData()[i] = data;
				bShadowFound = true;
				break;
			}
		}
		if(!bShadowFound && ofIsGLProgrammableRenderer()){
			ofShadowsData().push_back(data);
			data->index = ofShadowsData().size() - 1;
			data->isEnabled = false;
			bShadowFound = true;
		}
	}
	_checkFbos(); // clean up
}

//--------------------------------------------------------------
void ofShadow::_allocate() {
	_checkFbos(); // clean up
	_allocateFbo();
}

//--------------------------------------------------------------
void ofShadow::_allocateFbo() {
	if(getGLData(data->lightType).bFboAllocated) {
		return;
	}
	
	GLenum gl_read_status = GL_FRAMEBUFFER_UNSUPPORTED;
	
	GLenum textureTarget = getTextureTarget(data->lightType);
#if !defined(TARGET_OPENGLES)
	int depthComponent = GL_DEPTH_COMPONENT32F;
	int glType = GL_FLOAT;
#elif defined(TARGET_EMSCRIPTEN)
	int depthComponent = GL_DEPTH_COMPONENT24;
	int glType = GL_UNSIGNED_INT;
#else
	int depthComponent = GL_DEPTH_COMPONENT;
	int glType = GL_UNSIGNED_SHORT;
#endif
	
	glBindTexture(textureTarget, getDepthMapTexId() );
	
	if( data->lightType == OF_LIGHT_POINT ) {
		// Create the cube map depth buffer
		#if !defined(TARGET_OPENGLES)
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		#endif
		
		// OES_depth_texture_cube_map
		
		if( textureTarget == GL_TEXTURE_CUBE_MAP ) {
			for (GLint i = 0 ; i < 6 ; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, depthComponent, getDepthMapWidth(), getDepthMapWidth(), 0, GL_DEPTH_COMPONENT, glType, NULL);
			}
		}
		
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		#ifdef GL_TEXTURE_WRAP_R
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		#endif
		#if defined( GL_TEXTURE_CUBE_MAP_ARRAY ) && defined(glTexImage3D)
		if( textureTarget == GL_TEXTURE_CUBE_MAP_ARRAY ) {
			
			glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, depthComponent, getDepthMapWidth(), getDepthMapWidth(), getGLData(data->lightType).totalShadows * 6, 0, GL_DEPTH_COMPONENT, glType, NULL);
		}
		#endif
	} else {
		if( textureTarget == GL_TEXTURE_2D ) {
			glTexImage2D(GL_TEXTURE_2D, 0, depthComponent, getDepthMapWidth(), getDepthMapHeight(), 0, GL_DEPTH_COMPONENT, glType, NULL);
		}
		
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		//-- This is to allow usage of shadow2DProj function in the shader --//
		if( data->lightType != OF_LIGHT_AREA ) {
			#ifdef GL_TEXTURE_COMPARE_MODE
			glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
			#endif
		}
		//--! This is to allow usage of shadow2DProj function in the shader !--//
		#ifdef GL_TEXTURE_2D_ARRAY
		if( textureTarget == GL_TEXTURE_2D_ARRAY ) {
			#ifdef GL_TEXTURE_WRAP_R
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			#endif
		}
		#endif
		
		#if defined(GL_CLAMP_TO_BORDER) && !defined(TARGET_EMSCRIPTEN)
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glTexParameterfv(textureTarget, GL_TEXTURE_BORDER_COLOR, borderColor);
		#else
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		#endif
		
		#ifdef GL_TEXTURE_2D_ARRAY
		if( textureTarget == GL_TEXTURE_2D_ARRAY ) {
			#ifdef glTexImage3D
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, depthComponent, getDepthMapWidth(), getDepthMapHeight(), getGLData(data->lightType).totalShadows, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			#endif
		}
		#endif
	}
	glBindTexture(textureTarget, 0);
	
	
	// Create the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, getDepthMapFboId() );
	#if defined(TARGET_OPENGLES)
	
	if( textureTarget == GL_TEXTURE_CUBE_MAP ) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, NULL, 0);
		for(int c = 0; c < 6; c++) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X+c, getDepthMapTexId(), 0);
		}
	} else {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureTarget, getDepthMapTexId(), 0);
	}
	#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, getDepthMapTexId(), 0);
	#endif
	
	#ifndef TARGET_OPENGLES
	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);
	
	// Disable reads from the color buffer
	glReadBuffer(GL_NONE);
	#endif
	
	gl_read_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	if (gl_read_status != GL_FRAMEBUFFER_COMPLETE) {
		releaseFBO(data->lightType);
		ofLogError("ofShadow :: _allocateFbo : Frame buffer error, status") << gl_read_status;
	} else {
		getGLData(data->lightType).bFboAllocated = true;
	}
	
	
	ofLogVerbose("xxxxx ofShadow::_allocateFbo xxxxxxxxxxxxxxxxxxxxxxxxx") << std::endl;
}

//--------------------------------------------------------------
void ofShadow::_checkFbos() {
	// count the number of shadows here //
	std::map<int, int> preNumShadows;
	preNumShadows[OF_LIGHT_POINT] = getGLData(OF_LIGHT_POINT).totalShadows;
	preNumShadows[OF_LIGHT_DIRECTIONAL] = getGLData(OF_LIGHT_DIRECTIONAL).totalShadows;
	preNumShadows[OF_LIGHT_SPOT] = getGLData(OF_LIGHT_SPOT).totalShadows;
	preNumShadows[OF_LIGHT_AREA] = getGLData(OF_LIGHT_AREA).totalShadows;
	_updateNumShadows();
	
	if(getGLData(OF_LIGHT_POINT).totalShadows != preNumShadows[OF_LIGHT_POINT]) {
		releaseFBO((int)OF_LIGHT_POINT);
	}
	if(getGLData(OF_LIGHT_DIRECTIONAL).totalShadows != preNumShadows[OF_LIGHT_DIRECTIONAL]) {
		releaseFBO((int)OF_LIGHT_DIRECTIONAL);
	}
	if(getGLData(OF_LIGHT_SPOT).totalShadows != preNumShadows[OF_LIGHT_SPOT]) {
		releaseFBO((int)OF_LIGHT_SPOT);
	}
	if(getGLData(OF_LIGHT_AREA).totalShadows != preNumShadows[OF_LIGHT_AREA]) {
		releaseFBO((int)OF_LIGHT_AREA);
	}
}

//--------------------------------------------------------------
void ofShadow::_updateNumShadows() {
	
	getGLData(OF_LIGHT_POINT).totalShadows = 0;
	getGLData(OF_LIGHT_DIRECTIONAL).totalShadows = 0;
	getGLData(OF_LIGHT_SPOT).totalShadows = 0;
	getGLData(OF_LIGHT_AREA).totalShadows = 0;
	
	for(size_t i=0; i < ofShadowsData().size(); i++){
		if(!ofShadowsData()[i].expired()) {
			auto shadow = ofShadowsData()[i].lock();
			if( shadow ) {
				getGLData(shadow->lightType).totalShadows++;
			}
		}
	}
}

#include "shaders/shadowDepth.vert"
#include "shaders/shadowDepth.frag"
#include "shaders/shadowDepthCubeGeom.glsl"

bool ofShadow::setupShadowDepthShader(ofShader& ashader, const std::string aShaderMain) {
	return setupShadowDepthShader( ashader, data->lightType, aShaderMain );
}

bool ofShadow::setupShadowDepthShader(ofShader& ashader, int aLightType, const std::string aShaderMain) {
	std::string gversion = "#version 150\n";
	#ifdef TARGET_OPENGLES
	gversion = "#version 300 es\nprecision highp float;\n";
	#endif
	std::string tdefines = "#define SINGLE_PASS\n";

	bool bDepthCubeSinglePass = false;

	if( aLightType == OF_LIGHT_POINT ) {
		#ifndef TARGET_OPENGLES
		if(isSingleOmniPass()) {
			//return shaders[&renderer]->depthCube;
			tdefines = "#define CUBE_MAP_SINGLE_PASS\n";
			bDepthCubeSinglePass = true;
		} else {
			//return shaders[&renderer]->depthCubeMultiPass;
			tdefines = "#define CUBE_MAP_MULTI_PASS\n";
		}
		#else
		//return shaders[&renderer]->depthCubeMultiPass;
		tdefines = "#define CUBE_MAP_MULTI_PASS\n";
		#endif
	} else if( aLightType == OF_LIGHT_AREA ) {
		//return shaders[&renderer]->depth;
	} else {
		//return shaders[&renderer]->depth;
	}

	ashader.setupShaderFromSource(GL_VERTEX_SHADER,gversion+tdefines+depthVertexShaderSource+aShaderMain);
	ashader.setupShaderFromSource(GL_FRAGMENT_SHADER,gversion+tdefines+depthFragShaderSource);

	#ifndef TARGET_OPENGLES
	if(bDepthCubeSinglePass) {
		ashader.setupShaderFromSource(GL_GEOMETRY_SHADER_EXT,depthCubeGeometryShaderSource);
	}
	#endif

	ashader.bindDefaults();
	return ashader.linkProgram();
}


void ofShadow::initShaders(ofGLProgrammableRenderer & renderer) const{
	auto rendererShaders = shaders.find(&renderer);
	
	if(rendererShaders == shaders.end() ){
		shaders[&renderer] = std::make_shared<ofShadow::Shaders>();
				
		std::string gversion = "#version 150\n";
		#ifdef TARGET_OPENGLES
		gversion = "#version 300 es\nprecision highp float;\n";
		#endif
		
		std::string vertString = depthVertexShaderSource+depthVertexShader_Main;
		
		shaders[&renderer]->depth.setupShaderFromSource(GL_VERTEX_SHADER,gversion+"#define SINGLE_PASS\n"+vertString);
		shaders[&renderer]->depth.setupShaderFromSource(GL_FRAGMENT_SHADER,gversion+"#define SINGLE_PASS\n"+depthFragShaderSource);
		shaders[&renderer]->depth.bindDefaults();
		shaders[&renderer]->depth.linkProgram();
		
		#ifndef TARGET_OPENGLES
		shaders[&renderer]->depthCube.setupShaderFromSource(GL_VERTEX_SHADER,gversion+"#define CUBE_MAP_SINGLE_PASS\n"+vertString);
		shaders[&renderer]->depthCube.setupShaderFromSource(GL_FRAGMENT_SHADER,gversion+"#define CUBE_MAP_SINGLE_PASS\n"+depthFragShaderSource);
		shaders[&renderer]->depthCube.setupShaderFromSource(GL_GEOMETRY_SHADER_EXT,depthCubeGeometryShaderSource);
		shaders[&renderer]->depthCube.bindDefaults();
		shaders[&renderer]->depthCube.linkProgram();
		#endif
		shaders[&renderer]->depthCubeMultiPass.setupShaderFromSource(GL_VERTEX_SHADER,gversion+"#define CUBE_MAP_MULTI_PASS\n"+vertString);
		shaders[&renderer]->depthCubeMultiPass.setupShaderFromSource(GL_FRAGMENT_SHADER,gversion+"#define CUBE_MAP_MULTI_PASS\n"+depthFragShaderSource);
		shaders[&renderer]->depthCubeMultiPass.bindDefaults();
		shaders[&renderer]->depthCubeMultiPass.linkProgram();
				
	}
	
}


