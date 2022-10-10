//
//  ofShadow.cpp
//  emptyExample
//
//  Created by Nick Hardeman on 10/3/22.
//

#include "ofShadow.h"
#include "ofConstants.h"
#include "of3dUtils.h"
#include "ofGLBaseTypes.h"
#include "ofGLUtils.h"

#include "ofLight.h"

#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "ofGLProgrammableRenderer.h"

using std::weak_ptr;
using std::vector;
using std::shared_ptr;


//----------------------------------------
vector<weak_ptr<ofShadow::Data> > & ofShadowsData(){
	static vector<weak_ptr<ofShadow::Data> > * shadowsActive = ofIsGLProgrammableRenderer()?new vector<weak_ptr<ofShadow::Data> >:new vector<weak_ptr<ofShadow::Data> >(8);
	return *shadowsActive;
}

//--------------------------------------------------------------
bool ofHasActiveShadows() {
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
//		_checkFbos();
//		_allocateFbo();
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

//----------------------------------------
bool ofSetShadowShaderData( const ofShader& ashader, int aStartTexLocation ) {
	ashader.setUniformTexture("uShadowCubeMap", GL_TEXTURE_CUBE_MAP_ARRAY, ofShadow::getPointTexId(), aStartTexLocation );
	ashader.setUniformTexture("uShadowMapDirectional", GL_TEXTURE_2D_ARRAY, ofShadow::getDirectionalTexId(), aStartTexLocation+1 );
	ashader.setUniformTexture("uShadowMapSpot", GL_TEXTURE_2D_ARRAY, ofShadow::getSpotTexId(), aStartTexLocation+2 );
	
	for(size_t i=0;i< MIN(ofLightsData().size(),ofShadowsData().size());i++){
		std::string idx = ofToString(i,0);
		std::shared_ptr<ofShadow::Data> shadow = ofShadowsData()[i].lock();
		std::string shadowAddress = "shadows["+idx+"]";
		if(!shadow || !shadow->isEnabled || shadow->index < 0 ){
			ashader.setUniform1f(shadowAddress+".enabled", 0 );
			continue;
		}
		ashader.setUniform1f(shadowAddress+".enabled", 1 );
		if( shadow->lightType != OF_LIGHT_POINT ) {
			ashader.setUniformMatrix4f(shadowAddress+".shadowMatrix", shadow->shadowMatrix );
		}
		
		ashader.setUniform1f(shadowAddress+".near", shadow->near );
		ashader.setUniform1f(shadowAddress+".far", shadow->far );
		ashader.setUniform1f(shadowAddress+".normalBias", shadow->normalBias );
		ashader.setUniform1f(shadowAddress+".bias", shadow->bias );
		if( shadow->lightType != OF_LIGHT_POINT ) {
			ashader.setUniform1f(shadowAddress+".sampleRadius", shadow->sampleRadius/(float)getGLData(shadow->lightType).width );
		} else {
			ashader.setUniform1f(shadowAddress+".sampleRadius", shadow->sampleRadius );
		}
		
		ashader.setUniform3f(shadowAddress+".lightWorldPos", shadow->position );
		
		ashader.setUniform4f(shadowAddress+".color", shadow->color );
		
		ashader.setUniform3f(shadowAddress+".lightUp", shadow->up );
		ashader.setUniform3f(shadowAddress+".lightRight", shadow->right );
		ashader.setUniform1f(shadowAddress+".shadowType", (float)shadow->shadowType );
		ashader.setUniform1i(shadowAddress+".texIndex", shadow->texIndex );
//		std::cout << "ofSetShadowShaderData :: shadow index: " << shadow->texIndex << std::endl;
	}
	return true;
}

//--------------------------------------------------------------
ofShadow::ofShadow() {
	data = std::make_shared<ofShadow::Data>();
	data->color = ofFloatColor(0,0,0,0.5);
	data->shadowMatrix = glm::mat4(1.0f);
	setSingleOmniPass(true);
	_checkSetup();
}

//--------------------------------------------------------------
ofShadow::~ofShadow() {
	clear();
}

//--------------------------------------------------------------
void ofShadow::setLightType( int atype ) {
	
	if( (ofLightType)atype != data->lightType ) {
		ofLogNotice() << "ofShadow::setLightType : " << atype;
		clear();
	}
	data->lightType = atype;
	data->numDepthPasses = getNumShadowDepthPasses();
	_checkFbos();
	_allocate();
}

//--------------------------------------------------------------
//void ofShadow::update(const glm::vec3& aposition, const glm::vec3& adirection, const glm::vec3& aup, const glm::vec3& aright ) {
void ofShadow::update( const ofLight& alight ) {
	
	if( !data->isEnabled ) {
		return;
	}
	
	glm::quat rq = alight.getGlobalOrientation();
	glm::vec3 lookAtDir(glm::normalize( rq * glm::vec4(0.f,0.f,-1.f, 1.f)));
	
	setLightType( (ofLightType)alight.getType() );
	
	data->position = alight.getGlobalPosition();
	data->direction = glm::normalize(lookAtDir);
	if( data->lightType == OF_LIGHT_DIRECTIONAL ) {
		data->direction *= -1.;
	}
	data->up = alight.getUpDir();
	data->right = alight.getSideDir();
	
	int targetNumMatrices = 1;
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
	
	if( data->lightType == OF_LIGHT_SPOT ) {
		mFov = alight.getSpotlightCutOff() * 2.0f;
		mShadowProjection = glm::perspective(glm::radians(mFov), 1.0f, getNearClip(), getFarClip());
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
		// aspect is 1.0 since width and height are the same
//		mShadowProjection = glm::perspective(ofDegToRad(90.0f), 1.0f, mNearClip, mFarClip);
		mShadowProjection = glm::perspective(glm::radians(mFov), 1.0f, getNearClip(), getFarClip() );
		// eye, target and up vector
		// +x, -x, +y, -y, +z and -z direction
		mLookAtMats[0] = glm::lookAt( data->position, data->position + glm::vec3(1,0,0), glm::vec3(0, -1, 0) );
		mLookAtMats[1] = glm::lookAt( data->position, data->position + glm::vec3(-1,0,0), glm::vec3(0, -1, 0) );
		mLookAtMats[2] = glm::lookAt( data->position, data->position + glm::vec3(0,1,0), glm::vec3(0, 0, 1) );
		mLookAtMats[3] = glm::lookAt( data->position, data->position + glm::vec3(0,-1,0), glm::vec3(0, 0, -1) );
		mLookAtMats[4] = glm::lookAt( data->position, data->position + glm::vec3(0,0,1), glm::vec3(0, -1, 0) );
		mLookAtMats[5] = glm::lookAt( data->position, data->position + glm::vec3(0,0,-1), glm::vec3(0, -1, 0) );
		
		for( uint i = 0; i < 6; i++ ) {
			mViewProjMats[i] = mShadowProjection * mLookAtMats[i];
		}
	}
	
//	if( ofGetKeyPressed('r') || !mShader ) {
//		if( mShader ) {
//			mShader.reset();
//		}
//		_allocateShader();
//	}
}

//----------------------------------------------------
bool ofShadow::beginDepth() {
	
	_allocateFbo();
	
	if( !getIsEnabled() ) {
		return false;
	}
	
	if( !ofIsGLProgrammableRenderer() ) {
		ofLogWarning("ofShadow :: beginDepth() : shadows are only available with programmable renderer.");
		return false;
	}
	
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
	
	glBindFramebuffer( GL_FRAMEBUFFER, getDepthMapFboId() );
	if( data->lightType == OF_LIGHT_POINT ) {
		// handled in the depthCubeGeom shader
//		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, getDepthMapTexId(), 0, (data->texIndex * 6));
	} else {
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, getDepthMapTexId(), 0, data->texIndex );
	}
	
	ofPushView();
	ofViewport(0, 0, getDepthMapWidth(), getDepthMapHeight(), false);
//		ofViewport(ofRectangle(0,0,mDepthMapSize,mDepthMapSize));
//	float ratioW = 1600.0 / 900.0;
//	glViewport( 0, 0, getDepthMapWidth(), getDepthMapHeight() );
	//	ofClear(0);
	//	glDrawBuffer(GL_NONE);
	glClear(GL_DEPTH_BUFFER_BIT);
	
//	if( !mShader ) {
//		_allocateShader();
//	}
	
	glRenderer->bind(*this);
	
//	depthShader.begin();
//	depthShader.setUniform3f("uLightPos", data->position );
//	depthShader.setUniform1f("uNearPlane", getNearClip() );
//	depthShader.setUniform1f("uFarPlane", getFarClip() );
//
//	if( data->lightType == OF_LIGHT_POINT ) {
//
////		ofLogNotice() << "Setting point light data " << endl;
//		if( isSingleOmniPass() ) {
//			for( unsigned int i = 0; i < 6; i++ ) {
//				depthShader.setUniformMatrix4f("light["+ std::to_string(i) +"].viewProjectionMatirx", mViewProjMats[i]);
//			}
//		} else {
//			ofLogWarning("Must call beginDepth(cubeFace) to render to point light in multiple passes");
//		}
//	} else {
////		mShader->setUniform1i("uWriteDistance", 0 );
//		// just in case //
//		if(mViewProjMats.size() > 0 ) {
//			depthShader.setUniformMatrix4f("lightsViewProjectionMatrix", mViewProjMats[0]);
//		}
//	}
	
	if( isGlCullingEnabled() ) {
		glEnable(GL_CULL_FACE); // enables face culling
		glFrontFace(mGlFrontFaceWindingOrder);
		glCullFace(GL_FRONT); // tells OpenGL to cull back faces (the sane default setting)
	}
	return true;
}

//--------------------------------------------------------------
bool ofShadow::endDepth() {
	if( !getIsEnabled() || !ofIsGLProgrammableRenderer() ) {
		return false;
	}
	if( isGlCullingEnabled() ) {
		glDisable(GL_CULL_FACE);
	}
	
	if( data->lightType == OF_LIGHT_POINT ) {
//		glDisable(GL_TEXTURE_CUBE_MAP_ARRAY);
	} else {
//		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
//		glDisable(GL_TEXTURE_2D_ARRAY);
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
	_allocateFbo();
	
	if( !getIsEnabled() ) {
		return false;
	}
	if( data->lightType != OF_LIGHT_POINT ) {
		ofLogWarning("ofShadow :: beginDepth(cubeFace) called from a light that does not use cube map. Use beginDepth() instead.");
		return false;
	}
	if( isSingleOmniPass() ) {
		ofLogWarning("ofShadow :: beginDepth(cubeFace) called using single pass, should be calling beginDepth().");
		return false;
	}
	if( !ofIsGLProgrammableRenderer() ) {
		ofLogWarning("ofShadow :: beginDepth(cubeFace) : shadows are only available with programmable renderer.");
		return false;
	}
	
//	if( data->lightType == OF_LIGHT_POINT ) {
//		glEnable(GL_TEXTURE_CUBE_MAP_ARRAY);
//	}
	
	const auto glRenderer = ofGetGLRenderer();
	if(!glRenderer){
		return false;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, getDepthMapFboId());
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + aCubeFace, getDepthMapTexId(), 0);
	glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, getDepthMapTexId(), 0, (data->texIndex * 6) + aCubeFace);
	glViewport(0, 0, getDepthMapWidth(), getDepthMapHeight() );
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glRenderer->bind(*this,aCubeFace);
	
//	depthShader.begin();
//	depthShader.setUniform3f("uLightPos", data->position );
//	depthShader.setUniform1f("uNearPlane", getNearClip() );
//	depthShader.setUniform1f("uFarPlane", getFarClip() );
//
//	if( aCubeFace < mViewProjMats.size() ) {
//		depthShader.setUniformMatrix4f("lightsViewProjectionMatrix", mViewProjMats[aCubeFace] );
//	}
	
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
	if( !ofIsGLProgrammableRenderer() ) {
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
#ifdef TARGET_OPENGLES
	// setting to false because the single pass uses a geometry shader
	ab = false;
#endif
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
		if( data->lightType == OF_LIGHT_SPOT ) {
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
	
	if( data->lightType == OF_LIGHT_DIRECTIONAL ) {
		
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
		
	glm::vec3 Z = glm::normalize(-afwd);
	glm::vec3 X = glm::normalize(aright);
	glm::vec3 Y = glm::normalize(aup);
	
	glm::vec3 p = data->position;
	
	glm::vec3 nc = p - Z * getNearClip();
	glm::vec3 fc = p - Z * getFarClip();
	
	float ratio = (float)getDepthMapWidth() / (float)getDepthMapHeight();
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
std::string ofShadow::ofGetShadowTypeAsString( ofShadowType atype ) {
	int type = (int)atype;
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
std::string ofShadow::getShadowTypeAsString() {
	return ofGetShadowTypeAsString(data->shadowType);
}

//--------------------------------------------------------------
void ofShadow::setColor( const ofFloatColor& acolor ) {
	data->color = acolor;
}

//--------------------------------------------------------------
const ofShader & ofShadow::getDepthShader(ofGLProgrammableRenderer & renderer) const {
	initShaders( renderer );
	if( data->lightType == OF_LIGHT_POINT ) {
		if(isSingleOmniPass()) {
			return shaders[&renderer]->depthCube;
		} else {
			return shaders[&renderer]->depthCubeMultiPass;
		}
	} else {
		return shaders[&renderer]->depth;
	}
}

//--------------------------------------------------------------
void ofShadow::updateDepth(const ofShader & shader,ofGLProgrammableRenderer & renderer) const {
	shader.setUniform3f("uLightPos", data->position );
	shader.setUniform1f("uNearPlane", data->near );
	shader.setUniform1f("uFarPlane", data->far );
	
	if( data->lightType == OF_LIGHT_POINT ) {
		if( isSingleOmniPass() ) {
			for( unsigned int i = 0; i < 6; i++ ) {
				shader.setUniformMatrix4f("light["+ std::to_string(i) +"].viewProjectionMatirx", mViewProjMats[i]);
			}
//			std::cout << "ofShadow point light single omni pass: " << data->texIndex << std::endl;
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
	shader.begin();
	shader.setUniform3f("uLightPos", data->position );
	shader.setUniform1f("uNearPlane", data->near );
	shader.setUniform1f("uFarPlane", data->far );
	
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
//		ofLogNotice("ofShadow :: _allocateFbo : already allocated: ") << data->lightType << " | " << ofGetFrameNum();
		return;
	}
	
	GLenum Status = GL_FRAMEBUFFER_UNSUPPORTED;
	
	int textureTarget = GL_TEXTURE_2D_ARRAY;
	if( data->lightType == OF_LIGHT_POINT ) {
		textureTarget = GL_TEXTURE_CUBE_MAP_ARRAY;
	}
	
	ofLogNotice("Allocating depth map for light type:  ") << data->lightType << " | " << ofGetFrameNum();// << " depth tex id: " << getDepthMapTexId();
	
	
//	glGenTextures(1, &mDepthMapTexId);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glBindTexture(textureTarget, getDepthMapTexId() );
	
	if( data->lightType == OF_LIGHT_POINT ) {
		// Create the cube map depth buffer
//		glGenTextures(1, &mDepthMapTexId);
//		glBindTexture(GL_TEXTURE_CUBE_MAP, mDepthMapTexId);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		
		if( textureTarget == GL_TEXTURE_CUBE_MAP ) {
			for (GLint i = 0 ; i < 6 ; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32F, getDepthMapWidth(), getDepthMapWidth(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			}
		}
		
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		if( textureTarget == GL_TEXTURE_CUBE_MAP_ARRAY ) {
			glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT32F, getDepthMapWidth(), getDepthMapWidth(), getGLData(data->lightType).totalShadows * 6, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		
//		// Create the fbo
//		glGenFramebuffers(1, &mFboDepthId);
//		glBindFramebuffer(GL_FRAMEBUFFER, mFboDepthId);
//
//		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthMapTexId, 0);
//
//		// Disable writes to the color buffer
//		glDrawBuffer(GL_NONE);
//
//		// Disable reads from the color buffer
//		glReadBuffer(GL_NONE);
//
//		Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	} else {
//		ofLogNotice("Allocating light depth map");
//		glGenTextures(1, &mDepthMapTexId);
//		glBindTexture(GL_TEXTURE_2D, mDepthMapTexId);
		if( textureTarget == GL_TEXTURE_2D ) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, getDepthMapWidth(), getDepthMapHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//-- This is to allow usage of shadow2DProj function in the shader --//
		glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
		//--! This is to allow usage of shadow2DProj function in the shader !--//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if( textureTarget == GL_TEXTURE_2D_ARRAY ) {
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(textureTarget, GL_TEXTURE_BORDER_COLOR, borderColor);
		
		if( textureTarget == GL_TEXTURE_2D_ARRAY ) {
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, getDepthMapWidth(), getDepthMapHeight(), getGLData(data->lightType).totalShadows, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		}
		
//		glGenFramebuffers(1, &mFboDepthId);
//		glBindFramebuffer(GL_FRAMEBUFFER, mFboDepthId);
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMapTexId, 0);
//		glDrawBuffer(GL_NONE);
//		glReadBuffer(GL_NONE);
//
//		Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	glBindTexture(textureTarget, 0);
	
	
	// Create the fbo
//	glGenFramebuffers(1, &mFboDepthId);
	glBindFramebuffer(GL_FRAMEBUFFER, getDepthMapFboId() );
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, getDepthMapTexId(), 0);
	
	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);
	
	// Disable reads from the color buffer
	glReadBuffer(GL_NONE);
	
	Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		releaseFBO(data->lightType);
		ofLogError("ofShadow :: _allocateFbo : Frame buffer error, status") << Status;
	} else {
		getGLData(data->lightType).bFboAllocated = true;
	}
	
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
//void ofShadow::_allocateShader() {
//	if( mShader ) {
//		return;
//	}
//	mShader = std::make_shared<ofShader>();
//	bool bok = false;
//	if( isSingleOmniPass() && data->lightType == OF_LIGHT_POINT ) {
//		bok = mShader->load("depthSinglePass.vert", "depthSinglePass.frag", "depthSinglePassGeom.glsl");
//	} else {
//		bok = mShader->load("depth");
//	}
//	ofLogNotice("ofShadow :: _setupShader() bok: ") << bok << " | " << ofGetFrameNum();
//
//}

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

//#include "shaders/shadowDepthCube.vert"
//#include "shaders/shadowDepthCube.frag"
#include "shaders/shadowDepthCubeGeom.glsl"

//#include "shaders/shadowDepthCubeMulti.vert"
//#include "shaders/shadowDepthCubeMulti.frag"


void ofShadow::initShaders(ofGLProgrammableRenderer & renderer) const{
	auto rendererShaders = shaders.find(&renderer);
	
//	std::string shaderVertSource = "#version 330\n";
//	std::string shaderFragSource = "#version 330\n";
	if(rendererShaders == shaders.end() ){
		shaders[&renderer] = std::make_shared<ofShadow::Shaders>();
		
//		shaderVertSource += "#define SINGLE_PASS\n";
//		shaderVertSource += depthVertexShaderSource;
		
//		shaderFragSource += "#define SINGLE_PASS\n";
//		shaderFragSource += depthFragShaderSource;
		
		shaders[&renderer]->depth.setupShaderFromSource(GL_VERTEX_SHADER,"#version 330\n#define SINGLE_PASS\n"+depthVertexShaderSource);
		shaders[&renderer]->depth.setupShaderFromSource(GL_FRAGMENT_SHADER,"#version 330\n#define SINGLE_PASS\n"+depthFragShaderSource);
		shaders[&renderer]->depth.bindDefaults();
		shaders[&renderer]->depth.linkProgram();
		
#ifndef TARGET_OPENGLES
		shaders[&renderer]->depthCube.setupShaderFromSource(GL_VERTEX_SHADER,"#version 330\n#define CUBE_MAP_SINGLE_PASS\n"+depthVertexShaderSource);
		shaders[&renderer]->depthCube.setupShaderFromSource(GL_FRAGMENT_SHADER,"#version 330\n#define CUBE_MAP_SINGLE_PASS\n"+depthFragShaderSource);
//		setupShaderSources( shaders[&renderer]->depthCube, "#define CUBE_MAP_SINGLE_PASS\n" );
		shaders[&renderer]->depthCube.setupShaderFromSource(GL_GEOMETRY_SHADER_EXT,depthCubeGeometryShaderSource);
		shaders[&renderer]->depthCube.bindDefaults();
		shaders[&renderer]->depthCube.linkProgram();
#endif
		shaders[&renderer]->depthCubeMultiPass.setupShaderFromSource(GL_VERTEX_SHADER,"#version 330\n#define CUBE_MAP_MULTI_PASS\n"+depthVertexShaderSource);
		shaders[&renderer]->depthCubeMultiPass.setupShaderFromSource(GL_FRAGMENT_SHADER,"#version 330\n#define CUBE_MAP_MULTI_PASS\n"+depthFragShaderSource);
//		setupShaderSources( shaders[&renderer]->depthCubeMultiPass, "#define CUBE_MAP_MULTI_PASS\n" );
		shaders[&renderer]->depthCubeMultiPass.bindDefaults();
		shaders[&renderer]->depthCubeMultiPass.linkProgram();
		
	}
	
}


