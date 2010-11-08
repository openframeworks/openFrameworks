#include "ofxFbo.h"


/*
 
 See 
 http://www.gandogames.com/2010/07/tutorial-using-anti-aliasing-msaa-in-the-iphone/
 and
 http://stackoverflow.com/questions/3340189/how-do-you-activate-multisampling-in-opengl-es-on-the-iphone
 for multisampling on iphone
 
 */

// mapping to allow simple opengl EXT and opengl ES OES
// commented out ones are already defined
#ifndef TARGET_OPENGLES
	/*#define glGenFramebuffers								glGenFramebuffersEXT
	#define glGenRenderbuffers								glGenRenderbuffersEXT
	#define	glDeleteFramebuffers							glDeleteFramebuffersEXT
	#define	glBindFramebuffer								glBindFramebufferEXT
	#define	glBindRenderbuffer								glBindRenderbufferEXT
	#define glRenderbufferStorage							glRenderbufferStorageEXT
	#define glFramebufferRenderbuffer						glFramebufferRenderbufferEXT
	#define glRenderbufferStorageMultisample				glRenderbufferStorageMultisampleEXT
	#define glFramebufferTexture2D							glFramebufferTexture2DEXT
	#define glCheckFramebufferStatus						glCheckFramebufferStatusEXT
	#define GL_FRAMEBUFFER									GL_FRAMEBUFFER_EXT
	#define GL_RENDERBUFFER									GL_RENDERBUFFER_EXT
	#define GL_DEPTH_ATTACHMENT								GL_DEPTH_ATTACHMENT_EXT
	#define GL_STENCIL_ATTACHMENT							GL_STENCIL_ATTACHMENT_EXT
	#define GL_FRAMEBUFFER_BINDING							GL_FRAMEBUFFER_BINDING_EXT
	#define GL_MAX_COLOR_ATTACHMENTS						GL_MAX_COLOR_ATTACHMENTS_EXT
	#define GL_MAX_SAMPLES									GL_MAX_SAMPLES_EXT
	#define GL_READ_FRAMEBUFFER								GL_READ_FRAMEBUFFER_EXT
	#define GL_WRITE_FRAMEBUFFER							GL_WRITE_FRAMEBUFFER_EXT
	#define GL_DRAW_FRAMEBUFFER								GL_DRAW_FRAMEBUFFER_EXT
	#define GL_FRAMEBUFFER_COMPLETE							GL_FRAMEBUFFER_COMPLETE_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT			GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT	GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS			GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS				GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER			GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER			GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT
	#define GL_FRAMEBUFFER_UNSUPPORTED						GL_FRAMEBUFFER_UNSUPPORTED_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE			GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT*/
	#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS			GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS				GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT
#else
	#define glGenFramebuffers								glGenFramebuffersOES
	#define glGenRenderbuffers								glGenRenderbuffersOES
	#define	glDeleteFramebuffers							glDeleteFramebuffersOES
	#define	glBindFramebuffer								glBindFramebufferOES
	#define	glBindRenderbuffer								glBindRenderbufferOES
	#define glRenderbufferStorage							glRenderbufferStorageOES
	#define glFramebufferRenderbuffer						glFramebufferRenderbufferOES
	#define glRenderbufferStorageMultisample				glRenderbufferStorageMultisampleOES
	#define glFramebufferTexture2D							glFramebufferTexture2DOES
	#define glCheckFramebufferStatus						glCheckFramebufferStatusOES
	#define GL_FRAMEBUFFER									GL_FRAMEBUFFER_OES
	#define GL_RENDERBUFFER									GL_RENDERBUFFER_OES
	#define GL_DEPTH_ATTACHMENT								GL_DEPTH_ATTACHMENT_OES
	#define GL_STENCIL_ATTACHMENT							GL_STENCIL_ATTACHMENT_OES
	#define GL_FRAMEBUFFER_BINDING							GL_FRAMEBUFFER_BINDING_OES
	#define GL_MAX_COLOR_ATTACHMENTS						GL_MAX_COLOR_ATTACHMENTS_OES
	#define GL_MAX_SAMPLES									GL_MAX_SAMPLES_OES
	#define GL_READ_FRAMEBUFFER								GL_READ_FRAMEBUFFER_OES
	#define GL_DRAW_FRAMEBUFFER								GL_DRAW_FRAMEBUFFER_OES
	#define GL_WRITE_FRAMEBUFFER							GL_WRITE_FRAMEBUFFER_OES
	#define GL_FRAMEBUFFER_COMPLETE							GL_FRAMEBUFFER_COMPLETE_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT			GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT	GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS			GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS				GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER			GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER			GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_OES
	#define GL_FRAMEBUFFER_UNSUPPORTED						GL_FRAMEBUFFER_UNSUPPORTED_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE			GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_OES
	#define GL_COLOR_ATTACHMENT0							GL_COLOR_ATTACHMENT0_OES

#endif



//-------------------------------------------------------------------------------------
ofxFbo::Settings::Settings() {
	width					= 0;
	height					= 0;
	numColorbuffers			= 1;
	useDepth				= true;
	useStencil				= false;
	textureTarget			= GL_TEXTURE_RECTANGLE_ARB;
	internalformat			= GL_RGBA;
	wrapModeHorizontal		= GL_CLAMP_TO_EDGE;
	wrapModeVertical		= GL_CLAMP_TO_EDGE;
	minFilter				= GL_LINEAR;
	maxFilter				= GL_LINEAR;
	numSamples				= 0;
}



//-------------------------------------------------------------------------------------

int	ofxFbo::_maxColorAttachments = -1;
int	ofxFbo::_maxDrawBuffers = -1;
int	ofxFbo::_maxSamples = -1;


ofxFbo::ofxFbo():
isBound(0),
fbo(0),
fboTextures(0),
depthBuffer(0),
stencilBuffer(0),
savedFramebuffer(0)
{
}

ofxFbo::~ofxFbo() {
	destroy();
}


int	ofxFbo::maxColorAttachments() {
	if(_maxColorAttachments<0) checkGLSupport();
	return _maxColorAttachments;
}

int	ofxFbo::maxDrawBuffers() {
	if(_maxDrawBuffers<0) checkGLSupport();
	return _maxDrawBuffers;
}

int	ofxFbo::maxSamples() {
	if(_maxSamples<0) checkGLSupport();
	return _maxSamples;
}


void ofxFbo::destroy() {
	unbind();
	
	if(fbo) glDeleteFramebuffers(1, &fbo);
	if(depthBuffer) glDeleteFramebuffers(1, &depthBuffer);
	if(stencilBuffer) glDeleteFramebuffers(1, &stencilBuffer);
	
	if(settings.numSamples && fboTextures) glDeleteFramebuffers(1, &fboTextures);
	
	for(int i=0; i<textures.size(); i++) delete textures[i];
	textures.clear();
	
	for(int i=0; i<colorBuffers.size(); i++) glDeleteFramebuffers(1, &colorBuffers[i]);
	colorBuffers.clear();
	
	
	settings = Settings();
}


void ofxFbo::checkGLSupport() {
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &_maxColorAttachments);
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &_maxDrawBuffers);
	glGetIntegerv(GL_MAX_SAMPLES, &_maxSamples);
	
	ofLog(OF_LOG_NOTICE, string("ofxFbo::checkGLSupport()\n") + 
		  "maxColorAttachments: " + ofToString(_maxColorAttachments) + "\n" +
		  "maxDrawBuffers: " + ofToString(_maxDrawBuffers) + "\n" + 
		  "maxSamples: " + ofToString(_maxSamples)
		  );
}


void ofxFbo::setup(int width, int height, int internalformat, int numSamples) {
	settings.width			= width;
	settings.height			= height;
	settings.internalformat	= internalformat;
	settings.numSamples		= numSamples;
	setup(settings);
}


void ofxFbo::setup(Settings settings) {
	checkGLSupport();
	
	destroy();
	
	if(settings.width == 0) settings.width = ofGetWidth();
	if(settings.height == 0) settings.height = ofGetHeight();
	this->settings = settings;
	
	// create main fbo
	// this is the main one we bind for drawing into
	// all the renderbuffers are attached to this (whether MSAA is enabled or not)
	glGenFramebuffers(1, &fbo);
	bind();
	
	// if we want a depth buffer, create it, and attach to our main fbo
	if(settings.useDepth) depthBuffer = createAndAttachRenderbuffer(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	
	// if we want a stencil buffer, create it, and attach to our main fbo
	if(settings.useStencil) stencilBuffer = createAndAttachRenderbuffer(GL_STENCIL_INDEX, GL_STENCIL_ATTACHMENT); 
	
	// if we want MSAA, create a new fbo for textures
	if(settings.numSamples) glGenFramebuffers(1, &fboTextures);
	else fboTextures = fbo;
	
	// check everything is ok with this fbo
	checkStatus();
	
	// now create all textures and color buffers
	for(int i=0; i<settings.numColorbuffers; i++) createAndAttachTexture(i);
	
	// if textures are attached to a different fbo (e.g. if using MSAA) check it's status
	if(fbo != fboTextures) {
		glBindFramebuffer(GL_FRAMEBUFFER, fboTextures);
		checkStatus();
	}
	
	// unbind it
	unbind();
}


GLuint ofxFbo::createAndAttachRenderbuffer(GLenum internalFormat, GLenum attachmentPoint) {
	GLuint buffer;
	glGenRenderbuffers(1, &buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
	if(settings.numSamples==0) glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, settings.width, settings.height);
	else glRenderbufferStorageMultisample(GL_RENDERBUFFER, settings.numSamples, internalFormat, settings.width, settings.height);
	glFramebufferRenderbuffer(GL_RENDERBUFFER, attachmentPoint, GL_RENDERBUFFER, buffer);
	return buffer;
}


void ofxFbo::createAndAttachTexture(GLenum attachmentPoint) {
	// bind fbo for textures (if using MSAA this is the newly created fbo, otherwise its the same fbo as before)
	glBindFramebuffer(GL_FRAMEBUFFER, fboTextures);
	
	ofTexture *tex = new ofTexture;
	tex->allocate(settings.width, settings.height, settings.internalformat, settings.textureTarget == GL_TEXTURE_2D ? false : true);
	tex->setTextureWrap(settings.wrapModeHorizontal, settings.wrapModeVertical);
	tex->setTextureMinMagFilter(settings.minFilter, settings.maxFilter);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPoint, tex->texData.textureTarget, tex->texData.textureID, 0);
	textures.push_back(tex);
	
	
	// if MSAA, bind main fbo and attach renderbuffer
	if(settings.numSamples) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		
		GLuint colorBuffer = createAndAttachRenderbuffer(settings.internalformat, GL_COLOR_ATTACHMENT0 + attachmentPoint);
		colorBuffers.push_back(colorBuffer);
	}
}


void ofxFbo::begin() {
	bind();
	ofPushView();
	ofSetupScreenPerspective(getWidth(), getHeight(), false);
	ofViewport(0, 0, getWidth(), getHeight());
}


void ofxFbo::end() {
	unbind();
	ofPopView();
}

void ofxFbo::bind() {
	if(isBound == 0) {
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &savedFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}
	isBound++;
}


void ofxFbo::unbind() {
	if(isBound) {
		glBindFramebuffer(GL_FRAMEBUFFER, savedFramebuffer);
		isBound = 0;
	}
}


int ofxFbo::getNumTextures() {
	return textures.size();
}

ofTexture& ofxFbo::getTexture(int attachmentPoint) {
	updateTexture(attachmentPoint);
	return *textures.at(attachmentPoint);
}

void ofxFbo::updateTexture(int attachmentPoint) {
	// TODO: flag to see if this is dirty or not 
	if(fbo != fboTextures) {
		glGetIntegerv( GL_FRAMEBUFFER_BINDING, &savedFramebuffer );
		
		// save current drawbuffer
		glPushAttrib(GL_COLOR_BUFFER_BIT);
		
		// save current readbuffer
		GLint readBuffer;
		glGetIntegerv(GL_READ_BUFFER, &readBuffer);
		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboTextures);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachmentPoint);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentPoint);
#ifndef TARGET_OPENGLES
		glBlitFramebuffer(0, 0, settings.width, settings.height, 0, 0, settings.width, settings.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
#else
		glRenderbufferStorageMultisampleAPPLE();	// untested
#endif
		glBindFramebuffer(GL_READ_FRAMEBUFFER, savedFramebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, savedFramebuffer);
		glBindFramebuffer( GL_FRAMEBUFFER, savedFramebuffer );
		
		// restore readbuffer
		glReadBuffer(readBuffer);
		
		// restore drawbuffer
		glPopAttrib();
		
	}	   
}



void ofxFbo::draw(float x, float y) {
	draw(x, y, settings.width, settings.height);
}


void ofxFbo::draw(float x, float y, float width, float height) {
	getTexture(0).draw(x, y, width, height);
}


GLuint ofxFbo::getFbo() {
	return fbo;
}

int ofxFbo::getWidth() {
	return settings.width;
}


int ofxFbo::getHeight() {
	return settings.height;
}


bool ofxFbo::checkStatus() {
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE:
			ofLog(OF_LOG_NOTICE, "FRAMEBUFFER_COMPLETE - OK");
			return true;
			
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			ofLog(OF_LOG_ERROR, "FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			ofLog(OF_LOG_ERROR, "FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
			ofLog(OF_LOG_ERROR, "FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
			ofLog(OF_LOG_ERROR, "FRAMEBUFFER_INCOMPLETE_FORMATS");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			ofLog(OF_LOG_ERROR, "FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			ofLog(OF_LOG_ERROR, "FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			ofLog(OF_LOG_ERROR, "FRAMEBUFFER_UNSUPPORTED");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			ofLog(OF_LOG_ERROR, "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
			break;
		default:
			ofLog(OF_LOG_ERROR, "UNKNOWN ERROR");
			break;
			
	}
	
	return false;
}

