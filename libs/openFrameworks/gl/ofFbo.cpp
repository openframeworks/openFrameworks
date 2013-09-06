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
ofFbo::Settings::Settings() {
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
	depthStencilInternalFormat		= GL_DEPTH_COMPONENT24;
	wrapModeHorizontal		= GL_CLAMP_TO_EDGE;
	wrapModeVertical		= GL_CLAMP_TO_EDGE;
	minFilter				= GL_LINEAR;
	maxFilter				= GL_LINEAR;
	numSamples				= 0;
}

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


ofFbo::ofFbo():
isBound(0),
fbo(0),
fboTextures(0),
depthBuffer(0),
stencilBuffer(0),
savedFramebuffer(0),
dirty(false),
defaultTextureIndex(0),
bIsAllocated(false)
{
#ifdef TARGET_OPENGLES
	if(!bglFunctionsInitialized){
		if(ofGetGLProgrammableRenderer()){
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

ofFbo::ofFbo(const ofFbo & mom){
	settings = mom.settings;
	isBound = mom.isBound;
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

	savedFramebuffer = mom.savedFramebuffer;

	colorBuffers = mom.colorBuffers;
	for(int i=0;i<(int)colorBuffers.size();i++){
		retainRB(colorBuffers[i]);
	}
	textures = mom.textures;
	dirty = mom.dirty;
	defaultTextureIndex = mom.defaultTextureIndex;
}

ofFbo & ofFbo::operator=(const ofFbo & mom){
	if(&mom==this) return *this;
	destroy();
	settings = mom.settings;
	isBound = mom.isBound;
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

	savedFramebuffer = mom.savedFramebuffer;

	colorBuffers = mom.colorBuffers;
	for(int i=0;i<(int)colorBuffers.size();i++){
		retainRB(colorBuffers[i]);
	}
	textures = mom.textures;
	dirty = mom.dirty;
	defaultTextureIndex = mom.defaultTextureIndex;
	return *this;
}

ofFbo::~ofFbo(){
	destroy();
}

int	ofFbo::maxColorAttachments() {
	if(_maxColorAttachments<0) checkGLSupport();
	return _maxColorAttachments;
}

int	ofFbo::maxDrawBuffers() {
	if(_maxDrawBuffers<0) checkGLSupport();
	return _maxDrawBuffers;
}

int	ofFbo::maxSamples() {
	if(_maxSamples<0) checkGLSupport();
	return _maxSamples;
}


void ofFbo::destroy() {
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

	for(int i=0; i<(int)colorBuffers.size(); i++) releaseRB(colorBuffers[i]);
	colorBuffers.clear();

	isBound = 0;
	bIsAllocated = false;
}

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

	ofLogNotice("ofFbo") << "checkGLSupport(): "
                          << "maxColorAttachments: " << _maxColorAttachments << ", "
                          << "maxDrawBuffers: " << _maxDrawBuffers << ", "
                          << "maxSamples: " << _maxSamples;
#else

	if(ofGetGLProgrammableRenderer() || ofGLCheckExtension("GL_OES_framebuffer_object")){
		ofLogVerbose("ofFbo") << "GL frame buffer object supported";
	}else{
		ofLogError("ofFbo") << "GL frame buffer object not supported by this graphics card";
		return false;
	}
	string extensions = (char*)glGetString(GL_EXTENSIONS);
	ofLogVerbose("ofFbo") << extensions;
#endif

	return true;
}


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

void ofFbo::allocate(Settings _settings) {
	if(!checkGLSupport()) return;

	destroy();

	if(_settings.width == 0) _settings.width = ofGetWidth();
	if(_settings.height == 0) _settings.height = ofGetHeight();
	if(_settings.numSamples > maxSamples() && maxSamples() > -1) {
		ofLogWarning("ofFbo") << "allocate(): clamping numSamples " << _settings.numSamples << " to maxSamples " << maxSamples() << " for frame buffer object" << fbo;
		_settings.numSamples = maxSamples();
	}
	settings = _settings;

	// create main fbo
	// this is the main one we bind for drawing into
	// all the renderbuffers are attached to this (whether MSAA is enabled or not)
	glGenFramebuffers(1, &fbo);
	retainFB(fbo);
	bind();

	if(settings.depthStencilAsTexture && settings.numSamples){
		ofLogWarning("ofFbo") << "allocate(): multisampling not supported with depthStencilAsTexture, setting 0 samples for frame buffer object " << fbo;
		settings.numSamples = 0;
	}

	//currently depth only works if stencil is enabled. 
	// http://forum.openframeworks.cc/index.php/topic,6837.0.html
#ifdef TARGET_OPENGLES
	if(settings.useDepth){
	  	settings.useStencil = true;
	}
    if( settings.depthStencilAsTexture ){
        settings.depthStencilAsTexture = false;
        ofLogWarning("ofFbo") << "allocate(): depthStencilAsTexture is not available for iOS";
    }
#endif
    
	GLenum depthAttachment = GL_DEPTH_ATTACHMENT;

	if( settings.useDepth && settings.useStencil ){
		settings.depthStencilInternalFormat = GL_DEPTH_STENCIL;
		#ifdef TARGET_OPENGLES
			depthAttachment = GL_DEPTH_ATTACHMENT;
		#else
			depthAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
		#endif
	}else if(settings.useDepth){
		depthAttachment = GL_DEPTH_ATTACHMENT;
	}else if(settings.useStencil){
		depthAttachment = GL_STENCIL_ATTACHMENT;
		settings.depthStencilInternalFormat = GL_STENCIL_INDEX;
	}

	//- USE REGULAR RENDER BUFFER
	if(!settings.depthStencilAsTexture){
		if(settings.useDepth && settings.useStencil){
			stencilBuffer = depthBuffer = createAndAttachRenderbuffer(settings.depthStencilInternalFormat, depthAttachment);
			retainRB(stencilBuffer);
			retainRB(depthBuffer);	
		}else if(settings.useDepth){
			depthBuffer = createAndAttachRenderbuffer(settings.depthStencilInternalFormat, depthAttachment);
			retainRB(depthBuffer);
		}else if(settings.useStencil){
			stencilBuffer = createAndAttachRenderbuffer(settings.depthStencilInternalFormat, depthAttachment);
			retainRB(stencilBuffer);
		}
	//- INSTEAD USE TEXTURE
	}else{
		if(settings.useDepth || settings.useStencil){
		createAndAttachDepthStencilTexture(settings.textureTarget,settings.depthStencilInternalFormat,depthAttachment);
#ifdef TARGET_OPENGLES
		// if there's depth and stencil the texture should be attached as
		// depth and stencil attachments
		// http://www.khronos.org/registry/gles/extensions/OES/OES_packed_depth_stencil.txt
		if(settings.useDepth && settings.useStencil){
	        glFramebufferTexture2D(GL_FRAMEBUFFER,
	                               GL_STENCIL_ATTACHMENT,
	                               GL_TEXTURE_2D, depthBufferTex.texData.textureID, 0);
		}
#endif
	}
	}

	// if we want MSAA, create a new fbo for textures
	#ifndef TARGET_OPENGLES
		if(settings.numSamples){
			glGenFramebuffers(1, &fboTextures);
			retainFB(fboTextures);
		}else{
			fboTextures = fbo;
		}
	#else
		fboTextures = fbo;
		if(settings.numSamples){
			ofLogWarning("ofFbo") << "allocate(): multisampling not supported in OpenGL ES";
		}
	#endif

	// now create all textures and color buffers
	if(settings.colorFormats.size() > 0) {
		for(int i=0; i<(int)settings.colorFormats.size(); i++) createAndAttachTexture(settings.colorFormats[i], i);
	}
	else if(settings.numColorbuffers > 0) {
		for(int i=0; i<settings.numColorbuffers; i++) createAndAttachTexture(settings.internalformat, i);
	}
	else {
		ofLogWarning("ofFbo") << "allocate(): no color buffers specified for frame buffer object " << fbo;
	}


	// if textures are attached to a different fbo (e.g. if using MSAA) check it's status
	if(fbo != fboTextures) {
		glBindFramebuffer(GL_FRAMEBUFFER, fboTextures);
	}

	// check everything is ok with this fbo
	bIsAllocated = checkStatus();

	// unbind it
	unbind();
}

bool ofFbo::isAllocated(){
	return bIsAllocated;
}

GLuint ofFbo::createAndAttachRenderbuffer(GLenum internalFormat, GLenum attachmentPoint) {
	GLuint buffer;
	glGenRenderbuffers(1, &buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
#ifndef TARGET_OPENGLES
	if(settings.numSamples==0) glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, settings.width, settings.height);
	else glRenderbufferStorageMultisample(GL_RENDERBUFFER, settings.numSamples, internalFormat, settings.width, settings.height);
#else
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, ofNextPow2(settings.width), ofNextPow2(settings.height));
#endif
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentPoint, GL_RENDERBUFFER, buffer);
	return buffer;
}


void ofFbo::createAndAttachTexture(GLenum internalFormat, GLenum attachmentPoint) {
	// bind fbo for textures (if using MSAA this is the newly created fbo, otherwise its the same fbo as before)
	GLint temp;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &temp);
	glBindFramebuffer(GL_FRAMEBUFFER, fboTextures);

	ofTexture tex;
	tex.allocate(settings.width, settings.height, internalFormat, settings.textureTarget == GL_TEXTURE_2D ? false : true);
	tex.texData.bFlipTexture = false;
	tex.setTextureWrap(settings.wrapModeHorizontal, settings.wrapModeVertical);
	tex.setTextureMinMagFilter(settings.minFilter, settings.maxFilter);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPoint, tex.texData.textureTarget, tex.texData.textureID, 0);
	textures.push_back(tex);
	
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

void ofFbo::createAndAttachDepthStencilTexture(GLenum target, GLint internalformat, GLenum  attachment, GLenum transferFormat, GLenum transferType){


	// allocate depthBufferTex as depth buffer;
	depthBufferTex.texData.glTypeInternal = internalformat;
	depthBufferTex.texData.textureTarget = target;
	depthBufferTex.texData.bFlipTexture = false;
	depthBufferTex.texData.width = settings.width;
	depthBufferTex.texData.height = settings.height;
	
	depthBufferTex.allocate(depthBufferTex.texData,transferFormat,transferType);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, depthBufferTex.texData.textureID, 0);
}

void ofFbo::createAndAttachDepthStencilTexture(GLenum target, GLint internalformat, GLenum  attachment){

	// allocate depthBufferTex as depth buffer;
	depthBufferTex.texData.glTypeInternal = internalformat;
	depthBufferTex.texData.textureTarget = target;
	depthBufferTex.texData.bFlipTexture = false;
	depthBufferTex.texData.width = settings.width;
	depthBufferTex.texData.height = settings.height;
	
	depthBufferTex.allocate(depthBufferTex.texData);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, depthBufferTex.texData.textureID, 0);
}


void ofFbo::begin(bool setupScreen) {
	if(!bIsAllocated) return;
	ofPushView();
	if(ofGetGLRenderer()){
		ofGetGLRenderer()->setCurrentFBO(this);
	}
	ofViewport();
	if(setupScreen){
		ofSetupScreenPerspective();
	}
	bind();
}

void ofFbo::end() {
	if(!bIsAllocated) return;
	unbind();
	if(ofGetGLRenderer()){
		ofGetGLRenderer()->setCurrentFBO(NULL);
	}
	ofPopView();
}

void ofFbo::bind() {
	if(isBound == 0) {
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &savedFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}
	isBound++;
}


void ofFbo::unbind() {
	if(isBound) {
		glBindFramebuffer(GL_FRAMEBUFFER, savedFramebuffer);
		isBound = 0;
		dirty = true;
	}
}

int ofFbo::getNumTextures() {
	return textures.size();
}

//TODO: Should we also check against card's max attachments or can we assume that's taken care of in texture setup? Still need to figure out MSAA in conjunction with MRT
void ofFbo::setActiveDrawBuffer(int i){
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
    if (i < getNumTextures()){
        GLenum e = GL_COLOR_ATTACHMENT0 + i;
        glDrawBuffer(e);
    }else{
		ofLogWarning("ofFbo") << "setActiveDrawBuffer(): fbo " << fbo << " couldn't set texture " << i << ", only " << getNumTextures() << "allocated";
    }
#endif
}

void ofFbo::setActiveDrawBuffers(const vector<int>& ids){
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
    vector<GLenum> attachments;
    for(int i=0; i < (int)ids.size(); i++){
      int id = ids[i];
        if (id < getNumTextures()){
            GLenum e = GL_COLOR_ATTACHMENT0 + id;
            attachments.push_back(e);
        }else{
            ofLogWarning("ofFbo") << "setActiveDrawBuffers(): fbo " << fbo << " couldn't set texture " << i << ", only " << getNumTextures() << "allocated";
        }
    }
    glDrawBuffers(attachments.size(),&attachments[0]);
#endif
}

void ofFbo::activateAllDrawBuffers(){
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
    vector<GLenum> attachments;
    for(int i=0; i < getNumTextures(); i++){
        if (i < getNumTextures()){
            GLenum e = GL_COLOR_ATTACHMENT0 + i;
            attachments.push_back(e);
        }else{
            ofLogWarning("ofFbo") << "activateAllDrawBuffers(): fbo " << fbo << " couldn't set texture " << i << ", only " << getNumTextures() << "allocated";
        }
    }
    glDrawBuffers(attachments.size(),&attachments[0]);
#endif
}


void ofFbo::setDefaultTextureIndex(int defaultTexture)
{
	defaultTextureIndex = defaultTexture;
}

int ofFbo::getDefaultTextureIndex()
{
	return defaultTextureIndex;
}

ofTexture& ofFbo::getTextureReference(){
	return getTextureReference(defaultTextureIndex);
}

ofTexture& ofFbo::getTextureReference(int attachmentPoint) {
	updateTexture(attachmentPoint);
    
    return textures[attachmentPoint];
}

void ofFbo::setAnchorPercent(float xPct, float yPct){
	getTextureReference().setAnchorPercent(xPct, yPct);
}

void ofFbo::setAnchorPoint(float x, float y){
	getTextureReference().setAnchorPoint(x, y);
}

void ofFbo::resetAnchor(){
	getTextureReference().resetAnchor();
}

void ofFbo::readToPixels(ofPixels & pixels, int attachmentPoint){
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
	getTextureReference(attachmentPoint).readToPixels(pixels);
#else
	pixels.allocate(settings.width,settings.height,ofGetImageTypeFromGLType(settings.internalformat));
	bind();
	int format = ofGetGLFormatFromInternal(settings.internalformat);
	glReadPixels(0,0,settings.width, settings.height, format, GL_UNSIGNED_BYTE, pixels.getPixels());
	unbind();
#endif
}

void ofFbo::readToPixels(ofShortPixels & pixels, int attachmentPoint){
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
	getTextureReference(attachmentPoint).readToPixels(pixels);
#else
	pixels.allocate(settings.width,settings.height,ofGetImageTypeFromGLType(settings.internalformat));
	bind();
	int format = ofGetGLFormatFromInternal(settings.internalformat);
	glReadPixels(0,0,settings.width, settings.height, format, GL_UNSIGNED_SHORT, pixels.getPixels());
	unbind();
#endif
}

void ofFbo::readToPixels(ofFloatPixels & pixels, int attachmentPoint){
	if(!bIsAllocated) return;
#ifndef TARGET_OPENGLES
	getTextureReference(attachmentPoint).readToPixels(pixels);
#else
	pixels.allocate(settings.width,settings.height,ofGetImageTypeFromGLType(settings.internalformat));
	bind();
	int format = ofGetGLFormatFromInternal(settings.internalformat);
	glReadPixels(0,0,settings.width, settings.height, format, GL_FLOAT, pixels.getPixels());
	unbind();
#endif
}

void ofFbo::updateTexture(int attachmentPoint) {
	if(!bIsAllocated) return;
	// TODO: flag to see if this is dirty or not
#ifndef TARGET_OPENGLES
	if(fbo != fboTextures && dirty) {
		glGetIntegerv( GL_FRAMEBUFFER_BINDING, &savedFramebuffer );

		if (!ofIsGLProgrammableRenderer()){
			// save current drawbuffer
			glPushAttrib(GL_COLOR_BUFFER_BIT);
		}
		// save current readbuffer
		GLint readBuffer;
		glGetIntegerv(GL_READ_BUFFER, &readBuffer);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboTextures);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachmentPoint);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentPoint);
		glBlitFramebuffer(0, 0, settings.width, settings.height, 0, 0, settings.width, settings.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, savedFramebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, savedFramebuffer);
		glBindFramebuffer( GL_FRAMEBUFFER, savedFramebuffer );

		// restore readbuffer
		glReadBuffer(readBuffer);
		
		if(!ofIsGLProgrammableRenderer()){
		// restore drawbuffer
			glPopAttrib();
		}
	
		dirty = false;

	}
#endif
}



void ofFbo::draw(float x, float y) {
	draw(x, y, settings.width, settings.height);
}


void ofFbo::draw(float x, float y, float width, float height) {
	if(!bIsAllocated) return;
    getTextureReference().draw(x, y, width, height);
}


GLuint ofFbo::getFbo() {
	return fbo;
}

float ofFbo::getWidth() {
	return settings.width;
}


float ofFbo::getHeight() {
	return settings.height;
}


bool ofFbo::checkStatus() {
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
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
			ofLogError("ofFbo") << "FRAMEBUFFER_INCOMPLETE_FORMATS";
			break;
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

ofTexture & ofFbo::getDepthTexture(){
	if(!settings.depthStencilAsTexture){
		ofLogError("ofFbo") << "getDepthTexture(): frame buffer object " << fbo << " not allocated with depthStencilAsTexture";
	}
	return depthBufferTex;
}

//#endif
