#include "ofConstants.h"
#include "ofFbo.h"
#include "ofAppRunner.h"
#include "ofUtils.h"
#include "ofGraphics.h"
#include "ofGLRenderer.h"
#include <map>

#ifdef TARGET_OPENGLES
#include <dlfcn.h>
#endif
#ifdef TARGET_ANDROID
#include "ofxAndroidUtils.h"
#endif

using namespace std;

/*

 See
 http://www.gandogames.com/2010/07/tutorial-using-anti-aliasing-msaa-in-the-iphone/
 and
 http://stackoverflow.com/questions/3340189/how-do-you-activate-multisampling-in-opengl-es-on-the-iphone
 for multisampling on iphone

 */

#ifdef TARGET_OPENGLES
	bool ofFbo::bglFunctionsInitialized=false;
	
	typedef void (* glGenFramebuffersType) (GLsizei n, GLuint* framebuffers);
	glGenFramebuffersType glGenFramebuffersFunc;
	#define glGenFramebuffers								glGenFramebuffersFunc

	typedef void (* glDeleteFramebuffersType) (GLsizei n, const GLuint* framebuffers);
	glDeleteFramebuffersType glDeleteFramebuffersFunc;
	#define	glDeleteFramebuffers							glDeleteFramebuffersFunc

	typedef void (* glDeleteRenderbuffersType) (GLsizei n, const GLuint* renderbuffers);
	glDeleteRenderbuffersType glDeleteRenderbuffersFunc;
	#define	glDeleteRenderbuffers							glDeleteRenderbuffersFunc

	typedef void (* glBindFramebufferType) (GLenum target, GLuint framebuffer);
	glBindFramebufferType glBindFramebufferFunc;
	#define	glBindFramebuffer								glBindFramebufferFunc

	typedef void (* glBindRenderbufferType) (GLenum target, GLuint renderbuffer);
	glBindRenderbufferType glBindRenderbufferFunc;
	#define	glBindRenderbuffer								glBindRenderbufferFunc

	typedef void (* glRenderbufferStorageType) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	glRenderbufferStorageType glRenderbufferStorageFunc;
	#define glRenderbufferStorage							glRenderbufferStorageFunc

	typedef void (* glFramebufferRenderbufferType) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	glFramebufferRenderbufferType glFramebufferRenderbufferFunc;
	#define glFramebufferRenderbuffer						glFramebufferRenderbufferFunc

	typedef void (* glRenderbufferStorageMultisampleType) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
	glRenderbufferStorageMultisampleType glRenderbufferStorageMultisampleFunc;
	#define glRenderbufferStorageMultisample				glRenderbufferStorageMultisampleFunc

	typedef void (* glFramebufferTexture2DType) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	glFramebufferTexture2DType glFramebufferTexture2DFunc;
	#define glFramebufferTexture2D							glFramebufferTexture2DFunc

	typedef GLenum (* glCheckFramebufferStatusType)  (GLenum target);
	glCheckFramebufferStatusType glCheckFramebufferStatusFunc;
	#define glCheckFramebufferStatus						glCheckFramebufferStatusFunc
#endif



//-------------------------------------------------------------------------------------
ofFboSettings::ofFboSettings(std::shared_ptr<ofBaseGLRenderer> renderer) {
	width					= 0;
	height					= 0;
	numColorbuffers			= 1;
	useDepth				= false;
	useStencil				= false;
	depthStencilAsTexture	= false;
#ifndef TARGET_OPENGLES
	textureTarget			= ofGetUsingArbTex() ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
#else
	textureTarget			= GL_TEXTURE_2D;
#endif
	internalformat			= GL_RGBA;
	depthStencilInternalFormat = GL_DEPTH_COMPONENT24;
	wrapModeHorizontal		= GL_CLAMP_TO_EDGE;
	wrapModeVertical		= GL_CLAMP_TO_EDGE;
	minFilter				= GL_LINEAR;
	maxFilter				= GL_LINEAR;
	numSamples				= 0;
	this->renderer = renderer;
}

//--------------------------------------------------------------
bool ofFboSettings::operator!=(const ofFboSettings & other){
	if(width != other.width){
		ofLogError() << "settings width differs from source";
		return true;
	}
	if(height != other.height){
		ofLogError() << "settings height differs from source";
		return true;
	}
	if(numColorbuffers != other.numColorbuffers){
		ofLogError() << "settings numColorbuffers differs from source";
		return true;
	}
	if(colorFormats != other.colorFormats){
		ofLogError() << "settings colorFormats differs from source";
		return true;
	}
	if(useDepth != other.useDepth){
		ofLogError() << "settings useDepth differs from source";
		return true;
	}
	if(useStencil != other.useStencil){
		ofLogError() << "settings useStencil differs from source";
		return true;
	}
	if(depthStencilAsTexture != other.depthStencilAsTexture){
		ofLogError() << "settings depthStencilAsTexture differs from source";
		return true;
	}
	if(textureTarget != other.textureTarget){
		ofLogError() << "settings textureTarget differs from source";
		return true;
	}
	if(internalformat != other.internalformat){
		ofLogError() << "settings internalformat differs from source";
		return true;
	}
	if(depthStencilInternalFormat != other.depthStencilInternalFormat){
		ofLogError() << "settings depthStencilInternalFormat differs from source";
		return true;
	}
	if(wrapModeHorizontal != other.wrapModeHorizontal){
		ofLogError() << "settings wrapModeHorizontal differs from source";
		return true;
	}
	if(wrapModeVertical != other.wrapModeVertical){
		ofLogError() << "settings wrapModeVertical differs from source";
		return true;
	}
	if(minFilter != other.minFilter){
		ofLogError() << "settings minFilter differs from source";
		return true;
	}
	if(maxFilter != other.maxFilter){
		ofLogError() << "settings maxFilter differs from source";
		return false;
	}
	if(numSamples != other.numSamples){
		ofLogError() << "settings numSamples differs from source";
		return true;
	}
	if(renderer.lock() != other.renderer.lock()){
		ofLogError() << "settings renderers are different";
		return true;
	}
	return false;
}

//--------------------------------------------------------------
static map<GLuint,int> & getIdsFB(){
	static map<GLuint,int> * idsFB = new map<GLuint,int>;
	return *idsFB;
}

//--------------------------------------------------------------
static void retainFB(GLuint id){
	if(id==0) return;
	if(getIdsFB().find(id)!=getIdsFB().end()){
		getIdsFB()[id]++;
	}else{
		getIdsFB()[id]=1;
	}
}

//--------------------------------------------------------------
static void releaseFB(GLuint id){
	if(getIdsFB().find(id)!=getIdsFB().end()){
		getIdsFB()[id]--;
		if(getIdsFB()[id]==0){
			glDeleteFramebuffers(1, &id);
		}
	}else{
		ofLogWarning("ofFbo") << "releaseFB(): something's wrong here, releasing unknown frame buffer id " << id;
		glDeleteFramebuffers(1, &id);
	}
}

//--------------------------------------------------------------
static map<GLuint,int> & getIdsRB(){
	static map<GLuint,int> * idsRB = new map<GLuint,int>;
	return *idsRB;
}

//--------------------------------------------------------------
static void retainRB(GLuint id){
	if(id==0) return;
	if(getIdsRB().find(id)!=getIdsRB().end()){
		getIdsRB()[id]++;
	}else{
		getIdsRB()[id]=1;
	}
}

//--------------------------------------------------------------
static void releaseRB(GLuint id){
	if(getIdsRB().find(id)!=getIdsRB().end()){
		getIdsRB()[id]--;
		if(getIdsRB()[id]==0){
			glDeleteRenderbuffers(1, &id);
		}
	}else{
		ofLogWarning("ofFbo") << "releaseRB(): something's wrong here, releasing unknown render buffer id " << id;
		glDeleteRenderbuffers(1, &id);
	}
}

//-------------------------------------------------------------------------------------
int	ofFbo::_maxColorAttachments = -1;
int	ofFbo::_maxDrawBuffers = -1;
int	ofFbo::_maxSamples = -1;


//--------------------------------------------------------------
ofFbo::ofFbo():
fbo(0),
fboTextures(0),
depthBuffer(0),
stencilBuffer(0),
dirty(false),
defaultTextureIndex(0),
bIsAllocated(false)
{
#ifdef TARGET_OPENGLES
	if(!bglFunctionsInitialized){
		if(ofIsGLProgrammableRenderer()){
			glGenFramebuffers = (glGenFramebuffersType)dlsym(RTLD_DEFAULT, "glGenFramebuffers");
			glDeleteFramebuffers =  (glDeleteFramebuffersType)dlsym(RTLD_DEFAULT, "glDeleteFramebuffers");
			glDeleteRenderbuffers =  (glDeleteRenderbuffersType)dlsym(RTLD_DEFAULT, "glDeleteRenderbuffers");
			glBindFramebuffer =  (glBindFramebufferType)dlsym(RTLD_DEFAULT, "glBindFramebuffer");
			glBindRenderbuffer = (glBindRenderbufferType)dlsym(RTLD_DEFAULT, "glBindRenderbuffer");
			glRenderbufferStorage = (glRenderbufferStorageType)dlsym(RTLD_DEFAULT, "glRenderbufferStorage");
			glFramebufferRenderbuffer = (glFramebufferRenderbufferType)dlsym(RTLD_DEFAULT, "glFramebufferRenderbuffer");
			glRenderbufferStorageMultisample = (glRenderbufferStorageMultisampleType)dlsym(RTLD_DEFAULT, "glRenderbufferStorageMultisample");
			glFramebufferTexture2D = (glFramebufferTexture2DType)dlsym(RTLD_DEFAULT, "glFramebufferTexture2D");
			glCheckFramebufferStatus = (glCheckFramebufferStatusType)dlsym(RTLD_DEFAULT, "glCheckFramebufferStatus");
		}else{
			glGenFramebuffers = (glGenFramebuffersType)dlsym(RTLD_DEFAULT, "glGenFramebuffersOES");
			glDeleteFramebuffers = (glDeleteFramebuffersType)dlsym(RTLD_DEFAULT, "glDeleteFramebuffersOES");
			glDeleteRenderbuffers = (glDeleteRenderbuffersType)dlsym(RTLD_DEFAULT, "glDeleteRenderbuffersOES");
			glBindFramebuffer = (glBindFramebufferType)dlsym(RTLD_DEFAULT, "glBindFramebufferOES");
			glBindRenderbuffer = (glBindRenderbufferType)dlsym(RTLD_DEFAULT, "glBindRenderbufferOES");
			glRenderbufferStorage = (glRenderbufferStorageType)dlsym(RTLD_DEFAULT, "glRenderbufferStorageOES");
			glFramebufferRenderbuffer = (glFramebufferRenderbufferType)dlsym(RTLD_DEFAULT, "glFramebufferRenderbufferOES");
			glRenderbufferStorageMultisample = (glRenderbufferStorageMultisampleType)dlsym(RTLD_DEFAULT, "glRenderbufferStorageMultisampleOES");
			glFramebufferTexture2D = (glFramebufferTexture2DType)dlsym(RTLD_DEFAULT, "glFramebufferTexture2DOES");
			glCheckFramebufferStatus = (glCheckFramebufferStatusType)dlsym(RTLD_DEFAULT, "glCheckFramebufferStatusOES");
		}
	}
#endif
}

//--------------------------------------------------------------
ofFbo::ofFbo(const ofFbo & mom){
	settings = mom.settings;
	bIsAllocated = mom.bIsAllocated;

	fbo = mom.fbo;
	retainFB(fbo);
	fboTextures = mom.fboTextures;
	if(settings.numSamples){
		retainFB(fboTextures);
	}
	if(mom.settings.depthStencilAsTexture){
		depthBufferTex = mom.depthBufferTex;
	}else{
		depthBuffer = mom.depthBuffer;
		retainRB(depthBuffer);
	}
	stencilBuffer = mom.stencilBuffer;
	retainRB(stencilBuffer);

	colorBuffers = mom.colorBuffers;
	for(int i=0;i<(int)colorBuffers.size();i++){
		retainRB(colorBuffers[i]);
	}
	textures = mom.textures;
	dirty = mom.dirty;
	defaultTextureIndex = mom.defaultTextureIndex;
    activeDrawBuffers = mom.activeDrawBuffers;
    if(fbo!=0){
    #ifdef TARGET_ANDROID
        ofAddListener(ofxAndroidEvents().reloadGL,this,&ofFbo::reloadFbo);
    #endif
    }
}

//--------------------------------------------------------------
ofFbo & ofFbo::operator=(const ofFbo & mom){
	if(&mom==this) return *this;
	clear();
	settings = mom.settings;
	bIsAllocated = mom.bIsAllocated;

	fbo = mom.fbo;
	retainFB(fbo);
	fboTextures = mom.fboTextures;
	if(settings.numSamples){
		retainFB(fboTextures);
    }
    if(mom.settings.depthStencilAsTexture){
        depthBufferTex = mom.depthBufferTex;
    }else{
        depthBuffer = mom.depthBuffer;
		retainRB(depthBuffer);
	}
	stencilBuffer = mom.stencilBuffer;
	retainRB(stencilBuffer);

	colorBuffers = mom.colorBuffers;
	for(int i=0;i<(int)colorBuffers.size();i++){
		retainRB(colorBuffers[i]);
	}
	textures = mom.textures;
	dirty = mom.dirty;
	defaultTextureIndex = mom.defaultTextureIndex;
    activeDrawBuffers = mom.activeDrawBuffers;
    if(fbo!=0){
    #ifdef TARGET_ANDROID
        ofAddListener(ofxAndroidEvents().reloadGL,this,&ofFbo::reloadFbo);
    #endif
    }
	return *this;
}

ofFbo::ofFbo(ofFbo && mom)
:settings(std::move(mom.settings))
,fbo(mom.fbo)
,fboTextures(mom.fboTextures)
,depthBuffer(mom.depthBuffer)
,stencilBuffer(mom.stencilBuffer)
,colorBuffers(std::move(mom.colorBuffers))
,textures(std::move(mom.textures))
,depthBufferTex(std::move(mom.depthBufferTex))
,activeDrawBuffers(std::move(mom.activeDrawBuffers))
,dirty(std::move(mom.dirty))
,defaultTextureIndex(std::move(mom.defaultTextureIndex))
,bIsAllocated(std::move(mom.bIsAllocated)){
    if(fbo!=0){
    #ifdef TARGET_ANDROID
        ofAddListener(ofxAndroidEvents().reloadGL,this,&ofFbo::reloadFbo);
    #endif
    }
    mom.fbo = 0;
    mom.depthBuffer = 0;
    mom.fboTextures = 0;
    mom.stencilBuffer = 0;
}

ofFbo & ofFbo::operator=(ofFbo && mom){
    if(&mom==this) return *this;
    clear();
    settings = std::move(mom.settings);
    bIsAllocated = std::move(mom.bIsAllocated);

    fbo = mom.fbo;
    fboTextures = mom.fboTextures;
    if(mom.settings.depthStencilAsTexture){
        depthBufferTex = std::move(mom.depthBufferTex);
    }else{
        depthBuffer = mom.depthBuffer;
    }
    stencilBuffer = std::move(mom.stencilBuffer);

    colorBuffers = std::move(mom.colorBuffers);
    textures = std::move(mom.textures);
    dirty = std::move(mom.dirty);
    defaultTextureIndex = std::move(mom.defaultTextureIndex);

    if(fbo!=0){
    #ifdef TARGET_ANDROID
        ofAddListener(ofxAndroidEvents().reloadGL,this,&ofFbo::reloadFbo);
    #endif
    }
    mom.fbo = 0;
    mom.depthBuffer = 0;
    mom.fboTextures = 0;
    mom.stencilBuffer = 0;
    return *this;
}

//--------------------------------------------------------------
ofFbo::~ofFbo(){
	clear();
}

//--------------------------------------------------------------
int	ofFbo::maxColorAttachments() {
	if(_maxColorAttachments<0) checkGLSupport();
	return _maxColorAttachments;
}

//--------------------------------------------------------------
int	ofFbo::maxDrawBuffers() {
	if(_maxDrawBuffers<0) checkGLSupport();
	return _maxDrawBuffers;
}

//--------------------------------------------------------------
int	ofFbo::maxSamples() {
	if(_maxSamples<0) checkGLSupport();
	return _maxSamples;
}


//--------------------------------------------------------------
void ofFbo::clear() {
	if(fbo){
		releaseFB(fbo);
		fbo=0;
	}
	if(depthBuffer){
		releaseRB(depthBuffer);
		depthBuffer = 0;
	}
	if(depthBufferTex.isAllocated()){
		depthBufferTex.clear();
	}
	if(stencilBuffer){
		releaseRB(stencilBuffer);
		stencilBuffer = 0;
	}
	if(settings.numSamples && fboTextures){
		releaseFB(fboTextures);
		fboTextures = 0;
	}
	textures.clear();
	for (int i=0; i < (int)colorBuffers.size(); i++) {
		releaseRB(colorBuffers[i]);
	}
	colorBuffers.clear();
	activeDrawBuffers.clear();
	bIsAllocated = false;
#ifdef TARGET_ANDROID
	ofRemoveListener(ofxAndroidEvents().reloadGL,this,&ofFbo::reloadFbo);
#endif
}


#ifndef TARGET_OPENGLES
//--------------------------------------------------------------
void ofFbo::clearColorBuffer(const ofFloatColor & color){
	glClearBufferfv(GL_COLOR, 0, &color.r);
}

//--------------------------------------------------------------
void ofFbo::clearColorBuffer(size_t buffer_idx, const ofFloatColor & color){
	glClearBufferfv(GL_COLOR, buffer_idx, &color.r);
}

//--------------------------------------------------------------
void ofFbo::clearDepthBuffer(float value){
	glClearBufferfv(GL_DEPTH, 0, &value);
}

//--------------------------------------------------------------
void ofFbo::clearStencilBuffer(int value){
	glClearBufferiv(GL_STENCIL, 0, &value);
}

//--------------------------------------------------------------
void ofFbo::clearDepthStencilBuffer(float depth, int stencil){
	glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil);
}
#endif

//--------------------------------------------------------------
void ofFbo::destroy() {
	clear();
}

//--------------------------------------------------------------
bool ofFbo::checkGLSupport() {
#ifndef TARGET_OPENGLES
	
	if (!ofIsGLProgrammableRenderer()){
		if(ofGLCheckExtension("GL_EXT_framebuffer_object")){
			ofLogVerbose("ofFbo") << "GL frame buffer object supported";
		}else{
			ofLogError("ofFbo") << "GL frame buffer object not supported by this graphics card";
			return false;
		}
	}

	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &_maxColorAttachments);
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &_maxDrawBuffers);
	glGetIntegerv(GL_MAX_SAMPLES, &_maxSamples);

	ofLogVerbose("ofFbo") << "checkGLSupport(): "
                          << "maxColorAttachments: " << _maxColorAttachments << ", "
                          << "maxDrawBuffers: " << _maxDrawBuffers << ", "
                          << "maxSamples: " << _maxSamples;
#else

	if(ofIsGLProgrammableRenderer() || ofGLCheckExtension("GL_OES_framebuffer_object")){
		ofLogVerbose("ofFbo") << "GL frame buffer object supported";
	}else{
		ofLogError("ofFbo") << "GL frame buffer object not supported by this graphics card";
		return false;
	}
#endif

	return true;
}


//--------------------------------------------------------------
void ofFbo::allocate(int width, int height, int internalformat, int numSamples) {

	settings.width			= width;
	settings.height			= height;
	settings.internalformat	= internalformat;
	settings.numSamples		= numSamples;
    
#ifdef TARGET_OPENGLES
	settings.useDepth		= false;
	settings.useStencil		= false;
	//we do this as the fbo and the settings object it contains could be created before the user had the chance to disable or enable arb rect.
    settings.textureTarget	= GL_TEXTURE_2D;
#else
	settings.useDepth		= true;
	settings.useStencil		= true;
	//we do this as the fbo and the settings object it contains could be created before the user had the chance to disable or enable arb rect. 	
    settings.textureTarget	= ofGetUsingArbTex() ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;    
#endif 
    
	allocate(settings);
}

//--------------------------------------------------------------
void ofFbo::allocate(ofFboSettings _settings) {
	if(!checkGLSupport()) return;

	clear();
	auto renderer = _settings.renderer.lock();
	if(renderer){
		settings.renderer = renderer;
	}else{
		settings.renderer = ofGetGLRenderer();
	}

	// check that passed values are correct
	if(_settings.width <= 0 || _settings.height <= 0){
		ofLogError("ofFbo") << "width and height have to be more than 0";
	}
	if(_settings.numSamples > maxSamples() && maxSamples() > -1) {
		ofLogWarning("ofFbo") << "allocate(): clamping numSamples " << _settings.numSamples << " to maxSamples " << maxSamples() << " for frame buffer object" << fbo;
		_settings.numSamples = maxSamples();
	}

	if(_settings.depthStencilAsTexture && _settings.numSamples){
		ofLogWarning("ofFbo") << "allocate(): multisampling not supported with depthStencilAsTexture, setting 0 samples for frame buffer object " << fbo;
		_settings.numSamples = 0;
	}

	//currently depth only works if stencil is enabled. 
	// http://forum.openframeworks.cc/index.php/topic,6837.0.html
#ifdef TARGET_OPENGLES
	if(_settings.useDepth){
	  	_settings.useStencil = true;
	}
    if( _settings.depthStencilAsTexture ){
        _settings.depthStencilAsTexture = false;
        ofLogWarning("ofFbo") << "allocate(): depthStencilAsTexture is not available for iOS";
    }
#endif
    
	GLenum depthAttachment = GL_DEPTH_ATTACHMENT;

	if( _settings.useDepth && _settings.useStencil ){
		_settings.depthStencilInternalFormat = GL_DEPTH_STENCIL;
		#ifdef TARGET_OPENGLES
			depthAttachment = GL_DEPTH_ATTACHMENT;
		#else
			depthAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
		#endif
	}else if(_settings.useDepth){
		depthAttachment = GL_DEPTH_ATTACHMENT;
	}else if(_settings.useStencil){
		depthAttachment = GL_STENCIL_ATTACHMENT;
		_settings.depthStencilInternalFormat = GL_STENCIL_INDEX;
	}

	// set needed values for allocation on instance settings
	// the rest will be set by the corresponding methods during allocation
	settings.width = _settings.width;
	settings.height = _settings.height;
	settings.numSamples = _settings.numSamples;

	// create main fbo
	// this is the main one we bind for drawing into
	// all the renderbuffers are attached to this (whether MSAA is enabled or not)
	glGenFramebuffers(1, &fbo);
	retainFB(fbo);

	GLint previousFboId = 0;

	// note that we are using a glGetInteger method here, which may stall the pipeline.
	// in the allocate() method, this is not that tragic since this will not be called 
	// within the draw() loop. Here, we need not optimise for performance, but for 
	// simplicity and readability .

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//- USE REGULAR RENDER BUFFER
	if(!_settings.depthStencilAsTexture){
		if(_settings.useDepth && _settings.useStencil){
			stencilBuffer = depthBuffer = createAndAttachRenderbuffer(_settings.depthStencilInternalFormat, depthAttachment);
			retainRB(stencilBuffer);
			retainRB(depthBuffer);
		}else if(_settings.useDepth){
			depthBuffer = createAndAttachRenderbuffer(_settings.depthStencilInternalFormat, depthAttachment);
			retainRB(depthBuffer);
		}else if(_settings.useStencil){
			stencilBuffer = createAndAttachRenderbuffer(_settings.depthStencilInternalFormat, depthAttachment);
			retainRB(stencilBuffer);
		}
	//- INSTEAD USE TEXTURE
	}else{
		if(_settings.useDepth || _settings.useStencil){
			createAndAttachDepthStencilTexture(_settings.textureTarget,_settings.depthStencilInternalFormat,depthAttachment);
			#ifdef TARGET_OPENGLES
				// if there's depth and stencil the texture should be attached as
				// depth and stencil attachments
				// http://www.khronos.org/registry/gles/extensions/OES/OES_packed_depth_stencil.txt
				if(_settings.useDepth && _settings.useStencil){
					glFramebufferTexture2D(GL_FRAMEBUFFER,
										   GL_STENCIL_ATTACHMENT,
										   GL_TEXTURE_2D, depthBufferTex.texData.textureID, 0);
				}
			#endif
		}
	}
    
    settings.useDepth = _settings.useDepth;
    settings.useStencil = _settings.useStencil;
    settings.depthStencilInternalFormat = _settings.depthStencilInternalFormat;
    settings.depthStencilAsTexture = _settings.depthStencilAsTexture;
    settings.textureTarget = _settings.textureTarget;
    settings.wrapModeHorizontal = _settings.wrapModeHorizontal;
    settings.wrapModeVertical = _settings.wrapModeVertical;
    settings.maxFilter = _settings.maxFilter;
    settings.minFilter = _settings.minFilter;

	// if we want MSAA, create a new fbo for textures
	#ifndef TARGET_OPENGLES
		if(_settings.numSamples){
			glGenFramebuffers(1, &fboTextures);
			retainFB(fboTextures);
		}else{
			fboTextures = fbo;
		}
	#else
		fboTextures = fbo;
		if(_settings.numSamples){
			ofLogWarning("ofFbo") << "allocate(): multisampling not supported in OpenGL ES";
		}
	#endif

	// now create all textures and color buffers
	if(_settings.colorFormats.size() > 0) {
		for(int i=0; i<(int)_settings.colorFormats.size(); i++) createAndAttachTexture(_settings.colorFormats[i], i);
	} else if(_settings.numColorbuffers > 0) {
		for(int i=0; i<_settings.numColorbuffers; i++) createAndAttachTexture(_settings.internalformat, i);
		_settings.colorFormats = settings.colorFormats;
	} else {
#ifndef TARGET_OPENGLES
		glDrawBuffer(GL_NONE);
#else
		ofLogWarning("ofFbo") << "allocate(): no color buffers specified for frame buffer object " << fbo;
#endif
	}
	settings.internalformat = _settings.internalformat;
	
	dirty.resize(_settings.colorFormats.size(), true); // we start with all color buffers dirty.

	// if textures are attached to a different fbo (e.g. if using MSAA) check it's status
	if(fbo != fboTextures) {
		glBindFramebuffer(GL_FRAMEBUFFER, fboTextures);
	}

	// check everything is ok with this fbo
	bIsAllocated = checkStatus();

	// restore previous framebuffer id
	glBindFramebuffer(GL_FRAMEBUFFER, previousFboId);

    /* UNCOMMENT OUTSIDE OF DOING RELEASES
	
    // this should never happen
	if(settings != _settings) ofLogWarning("ofFbo") << "allocation not complete, passed settings not equal to created ones, this is an internal OF bug";
    
    */
#ifdef TARGET_ANDROID
	ofAddListener(ofxAndroidEvents().reloadGL,this,&ofFbo::reloadFbo);
#endif
}

//--------------------------------------------------------------
void ofFbo::reloadFbo(){
	if(bIsAllocated){
		allocate(settings);
	}
}

//--------------------------------------------------------------
bool ofFbo::isAllocated() const {
	return bIsAllocated;
}

//----------------------------------------------------------
GLuint ofFbo::createAndAttachRenderbuffer(GLenum internalFormat, GLenum attachmentPoint) {
	GLuint buffer;
	glGenRenderbuffers(1, &buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
#ifndef TARGET_OPENGLES
	if (settings.numSamples==0) {
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, settings.width, settings.height);
	} else {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, settings.numSamples, internalFormat, settings.width, settings.height);
	}
#else
	if(ofGLSupportsNPOTTextures()){
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, settings.width, settings.height);
	}else{
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, ofNextPow2(settings.width), ofNextPow2(settings.height));
	}
#endif
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentPoint, GL_RENDERBUFFER, buffer);
	return buffer;
}

//----------------------------------------------------------
void ofFbo::createAndAttachTexture(GLenum internalFormat, GLenum attachmentPoint) {
	
	ofTextureData texData;

	texData.textureTarget = settings.textureTarget;
	texData.width = settings.width;
	texData.height = settings.height;
	texData.glInternalFormat = internalFormat;
	texData.bFlipTexture = false;
	texData.wrapModeHorizontal = settings.wrapModeHorizontal;
	texData.wrapModeVertical = settings.wrapModeVertical;
	texData.magFilter = settings.maxFilter;
	texData.minFilter = settings.minFilter;

	ofTexture tex;
	tex.allocate(texData);

    attachTexture(tex, internalFormat, attachmentPoint);
	dirty.push_back(true);
	activeDrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + attachmentPoint);
}

//----------------------------------------------------------
void ofFbo::attachTexture(ofTexture & tex, GLenum internalFormat, GLenum attachmentPoint) {
    // bind fbo for textures (if using MSAA this is the newly created fbo, otherwise its the same fbo as before)
	GLint temp;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &temp);
	glBindFramebuffer(GL_FRAMEBUFFER, fboTextures);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPoint, tex.texData.textureTarget, tex.texData.textureID, 0);
    if(attachmentPoint >= textures.size()) {
        textures.resize(attachmentPoint+1);
    }
    textures[attachmentPoint] = tex;
    
	settings.colorFormats.resize(attachmentPoint + 1);
	settings.colorFormats[attachmentPoint] = internalFormat;
	settings.numColorbuffers = settings.colorFormats.size();
    
	// if MSAA, bind main fbo and attach renderbuffer
	if(settings.numSamples) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        
		GLuint colorBuffer = createAndAttachRenderbuffer(internalFormat, GL_COLOR_ATTACHMENT0 + attachmentPoint);
		colorBuffers.push_back(colorBuffer);
		retainRB(colorBuffer);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, temp);

}

//----------------------------------------------------------
void ofFbo::createAndAttachDepthStencilTexture(GLenum target, GLint internalformat, GLenum  attachment, GLenum transferFormat, GLenum transferType){


	// allocate depthBufferTex as depth buffer;
	depthBufferTex.texData.glInternalFormat = internalformat;
	depthBufferTex.texData.textureTarget = target;
	depthBufferTex.texData.bFlipTexture = false;
	depthBufferTex.texData.width = settings.width;
	depthBufferTex.texData.height = settings.height;
	
	depthBufferTex.allocate(depthBufferTex.texData,transferFormat,transferType);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, depthBufferTex.texData.textureID, 0);
}

//----------------------------------------------------------
void ofFbo::createAndAttachDepthStencilTexture(GLenum target, GLint internalformat, GLenum  attachment){

	// allocate depthBufferTex as depth buffer;
	depthBufferTex.texData.glInternalFormat = internalformat;
	depthBufferTex.texData.textureTarget = target;
	depthBufferTex.texData.bFlipTexture = false;
	depthBufferTex.texData.width = settings.width;
	depthBufferTex.texData.height = settings.height;
	
	depthBufferTex.allocate(depthBufferTex.texData);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, depthBufferTex.texData.textureID, 0);
}

//----------------------------------------------------------
void ofFbo::begin(bool setupScreen) const{
	auto renderer = settings.renderer.lock();
	if(renderer){
        if(setupScreen){
            renderer->begin(*this, OF_FBOMODE_PERSPECTIVE | OF_FBOMODE_MATRIXFLIP);
        }else{
            renderer->begin(*this, OF_FBOMODE_NODEFAULTS);
        }
	}
}


void ofFbo::begin(ofFboMode mode){
    auto renderer = settings.renderer.lock();
    if(renderer){
        renderer->begin(*this, mode);
    }
}


//----------------------------------------------------------
/*void ofFbo::begin() const {
	auto renderer = settings.renderer.lock();
	if (renderer) {
		renderer->begin(*this, true);
	}
}

//----------------------------------------------------------
void ofFbo::beginNoPerspective() const {
	auto renderer = settings.renderer.lock();
	if (renderer) {
		renderer->begin(*this, false);
	}
}

//----------------------------------------------------------
void ofFbo::beginNoMatrixFlip() const {
	auto renderer = settings.renderer.lock();
	if (renderer) {
		renderer->beginNoMatrixFlip(*this);
	}
}

//----------------------------------------------------------
void ofFbo::beginNoMatrixFlipNoPerspective() const {
	auto renderer = settings.renderer.lock();
	if (renderer) {
		renderer->beginNoMatrixFlipNoPerspective(*this);
	}
}*/

//----------------------------------------------------------
void ofFbo::end() const{
	auto renderer = settings.renderer.lock();
	if(renderer){
		renderer->end(*this);
	}
}

//----------------------------------------------------------
void ofFbo::bind() const{
	auto renderer = settings.renderer.lock();
	if(renderer){
		renderer->bind(*this);
	}
}

//----------------------------------------------------------
void ofFbo::unbind() const{
	auto renderer = settings.renderer.lock();
	if(renderer){
		renderer->unbind(*this);
	}
}

//----------------------------------------------------------
void ofFbo::flagDirty() const{
	if (fbo != fboTextures){
		// ---------| if fbo != fboTextures, we are dealing with an MSAA enabled FBO.
		//
		// All currently active draw buffers need to be flagged dirty
		//
		// If a draw buffer has been activated and then de-activated, it has been 
		// flagged dirty at activation, so we can be sure all buffers which have 
		// been rendered to are flagged dirty.
		// 
		int numBuffersToFlag = min(dirty.size(), activeDrawBuffers.size());
		for(int i=0; i < numBuffersToFlag; i++){
			dirty[i] = true;
		}
	}
}

//----------------------------------------------------------
int ofFbo::getNumTextures() const {
	return textures.size();
}

//----------------------------------------------------------
void ofFbo::setActiveDrawBuffer(int i){
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
	vector<int> activebuffers(1, i);
	setActiveDrawBuffers(activebuffers);
#endif
}

//----------------------------------------------------------
void ofFbo::setActiveDrawBuffers(const vector<int>& ids){
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
    int numBuffers = activeDrawBuffers.size();
	activeDrawBuffers.clear();
	activeDrawBuffers.resize(numBuffers, GL_NONE); // we initialise the vector with GL_NONE, so a buffer will not be written to unless activated.
    for(int i=0; i < (int)ids.size(); i++){
      int id = ids[i];
        if (id < getNumTextures()){
            GLenum e = GL_COLOR_ATTACHMENT0 + id;
            activeDrawBuffers[id] = e; // activate requested buffers
			dirty[id] = true; // dirty activated draw buffers.
        }else{
            ofLogWarning("ofFbo") << "setActiveDrawBuffers(): fbo " << fbo << " couldn't set texture " << i << ", only " << getNumTextures() << "allocated";
        }
    }
    glDrawBuffers(activeDrawBuffers.size(),&activeDrawBuffers[0]);
#endif
}

//----------------------------------------------------------
void ofFbo::activateAllDrawBuffers(){
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
    vector<int> activeBuffers(getNumTextures(),0);
    for(int i=0; i < getNumTextures(); i++){
    	activeBuffers[i] = i;
    }
    setActiveDrawBuffers(activeBuffers);
#endif
}

//----------------------------------------------------------
void ofFbo::setDefaultTextureIndex(int defaultTexture)
{
	defaultTextureIndex = defaultTexture;
}

//----------------------------------------------------------
int ofFbo::getDefaultTextureIndex() const
{
	return defaultTextureIndex;
}

//----------------------------------------------------------
ofTexture& ofFbo::getTextureReference(){
	return getTexture();
}

//----------------------------------------------------------
ofTexture& ofFbo::getTextureReference(int attachmentPoint) {
	return getTexture(attachmentPoint);
}

//----------------------------------------------------------
const ofTexture& ofFbo::getTextureReference() const{
	return getTexture();
}

//----------------------------------------------------------
const ofTexture& ofFbo::getTextureReference(int attachmentPoint) const{
	return getTexture(attachmentPoint);
}

//----------------------------------------------------------
ofTexture& ofFbo::getTexture(){
	return getTexture(defaultTextureIndex);
}

//----------------------------------------------------------
ofTexture& ofFbo::getTexture(int attachmentPoint) {
	updateTexture(attachmentPoint);
    
    return textures[attachmentPoint];
}

//----------------------------------------------------------
const ofTexture& ofFbo::getTexture() const{
	return getTexture(defaultTextureIndex);
}

//----------------------------------------------------------
const ofTexture& ofFbo::getTexture(int attachmentPoint) const{
	ofFbo * mutThis = const_cast<ofFbo*>(this);
	mutThis->updateTexture(attachmentPoint);

    return textures[attachmentPoint];
}

//----------------------------------------------------------
void ofFbo::setAnchorPercent(float xPct, float yPct){
	getTexture().setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofFbo::setAnchorPoint(float x, float y){
	getTexture().setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofFbo::resetAnchor(){
	getTexture().resetAnchor();
}

//----------------------------------------------------------
void ofFbo::readToPixels(ofPixels & pixels, int attachmentPoint) const{
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
	getTexture(attachmentPoint).readToPixels(pixels);
#else
	pixels.allocate(settings.width,settings.height,ofGetImageTypeFromGLType(settings.internalformat));
	bind();
	int format = ofGetGLFormatFromInternal(settings.internalformat);
	glReadPixels(0,0,settings.width, settings.height, format, GL_UNSIGNED_BYTE, pixels.getData());
	unbind();
#endif
}

//----------------------------------------------------------
void ofFbo::readToPixels(ofShortPixels & pixels, int attachmentPoint) const{
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
	getTexture(attachmentPoint).readToPixels(pixels);
#else
	pixels.allocate(settings.width,settings.height,ofGetImageTypeFromGLType(settings.internalformat));
	bind();
	int format = ofGetGLFormatFromInternal(settings.internalformat);
	glReadPixels(0,0,settings.width, settings.height, format, GL_UNSIGNED_SHORT, pixels.getData());
	unbind();
#endif
}

//----------------------------------------------------------
void ofFbo::readToPixels(ofFloatPixels & pixels, int attachmentPoint) const{
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
	getTexture(attachmentPoint).readToPixels(pixels);
#else
	pixels.allocate(settings.width,settings.height,ofGetImageTypeFromGLType(settings.internalformat));
	bind();
	int format = ofGetGLFormatFromInternal(settings.internalformat);
	glReadPixels(0,0,settings.width, settings.height, format, GL_FLOAT, pixels.getData());
	unbind();
#endif
}

#ifndef TARGET_OPENGLES
//----------------------------------------------------------
void ofFbo::copyTo(ofBufferObject & buffer) const{
	if(!bIsAllocated) return;
	bind();
	buffer.bind(GL_PIXEL_PACK_BUFFER);
	glReadPixels(0, 0, settings.width, settings.height, ofGetGLFormatFromInternal(settings.internalformat), ofGetGLTypeFromInternal(settings.internalformat), NULL);
	buffer.unbind(GL_PIXEL_PACK_BUFFER);
	unbind();
}
#endif

//----------------------------------------------------------
void ofFbo::updateTexture(int attachmentPoint) {
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
	if(fbo != fboTextures && dirty[attachmentPoint]) {
		
		// if fbo != fboTextures, we are dealing with an MSAA enabled FBO.
		// and we need to blit one fbo into another to see get the texture
		// content

		if (!ofIsGLProgrammableRenderer()){
			// save current drawbuffer
			glPushAttrib(GL_COLOR_BUFFER_BIT);
		}

		auto renderer = settings.renderer.lock();
		if(renderer){
			renderer->bindForBlitting(*this,*this,attachmentPoint);
			glBlitFramebuffer(0, 0, settings.width, settings.height, 0, 0, settings.width, settings.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			renderer->unbind(*this);
		
			glReadBuffer(GL_BACK);
		}

		if(!ofIsGLProgrammableRenderer()){
			// restore current drawbuffer
			glPopAttrib();
		}
		dirty[attachmentPoint] = false;
	}
#endif
}

//----------------------------------------------------------
void ofFbo::draw(float x, float y) const{
	draw(x, y, settings.width, settings.height);
}

//----------------------------------------------------------
void ofFbo::draw(float x, float y, float width, float height) const{
	if(!bIsAllocated || settings.numColorbuffers==0) return;
    getTexture().draw(x, y, width, height);
}

//----------------------------------------------------------
GLuint ofFbo::getFbo() const {
	return fbo;
}

//----------------------------------------------------------
GLuint ofFbo::getId() const {
	return fbo;
}

//----------------------------------------------------------
GLuint ofFbo::getIdDrawBuffer() const{
	return fboTextures;
}

//----------------------------------------------------------
float ofFbo::getWidth() const {
	return settings.width;
}


//----------------------------------------------------------
float ofFbo::getHeight() const {
	return settings.height;
}


//----------------------------------------------------------
bool ofFbo::checkStatus() const {
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE:
			ofLogVerbose("ofFbo") << "FRAMEBUFFER_COMPLETE - OK";
			return true;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			ofLogError("ofFbo") << "FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			ofLogError("ofFbo") << "FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
			ofLogError("ofFbo") << "FRAMEBUFFER_INCOMPLETE_DIMENSIONS";
			break;
#ifndef TARGET_PROGRAMMABLE_GL
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
			ofLogError("ofFbo") << "FRAMEBUFFER_INCOMPLETE_FORMATS";
			break;
#endif
		case GL_FRAMEBUFFER_UNSUPPORTED:
			ofLogError("ofFbo") << "FRAMEBUFFER_UNSUPPORTED";
			break;
#ifndef TARGET_OPENGLES
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			ofLogWarning("ofFbo") << "FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			ofLogError("ofFbo") << "FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			ofLogError("ofFbo") << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
			break;
#endif
		default:
			ofLogError("ofFbo") << "UNKNOWN ERROR " << status;
			break;

	}

	return false;
}

//----------------------------------------------------------
ofTexture & ofFbo::getDepthTexture(){
	if(!settings.depthStencilAsTexture){
		ofLogError("ofFbo") << "getDepthTexture(): frame buffer object " << fbo << " not allocated with depthStencilAsTexture";
	}
	return depthBufferTex;
}

//----------------------------------------------------------
const ofTexture & ofFbo::getDepthTexture() const{
	if(!settings.depthStencilAsTexture){
		ofLogError("ofFbo") << "getDepthTexture(): frame buffer object " << fbo << " not allocated with depthStencilAsTexture";
	}
	return depthBufferTex;
}

//#endif
