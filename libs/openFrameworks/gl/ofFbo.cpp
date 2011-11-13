#include "ofFbo.h"
#include "ofAppRunner.h"
#include "ofUtils.h"
#include "ofGraphics.h"
#include "ofGLRenderer.h"
#include <map>

//#ifndef TARGET_OPENGLES


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
	#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS			GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS				GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT

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
	#define GL_WRITE_FRAMEBUFFER							GL_WRITE_FRAMEBUFFER_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS			GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS				GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT

	#ifdef TARGET_WIN32
	#define glBlitFramebuffer									glBlitFramebufferEXT
	#define GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_EXT
	#define GL_DEPTH_ATTACHMENT								GL_DEPTH_ATTACHMENT_EXT
	#define GL_DRAW_FRAMEBUFFER								GL_DRAW_FRAMEBUFFER_EXT
	#define GL_FRAMEBUFFER									GL_FRAMEBUFFER_EXT
	#define GL_FRAMEBUFFER_BINDING							GL_FRAMEBUFFER_BINDING_EXT
	#define GL_FRAMEBUFFER_COMPLETE							GL_FRAMEBUFFER_COMPLETE_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT			GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT	GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER			GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER			GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE			GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT
	#define GL_FRAMEBUFFER_UNSUPPORTED						GL_FRAMEBUFFER_UNSUPPORTED_EXT
	#define GL_MAX_COLOR_ATTACHMENTS						GL_MAX_COLOR_ATTACHMENTS_EXT
	#define GL_MAX_SAMPLES									GL_MAX_SAMPLES_EXT
	#define GL_STENCIL_ATTACHMENT							GL_STENCIL_ATTACHMENT_EXT
	#define GL_READ_FRAMEBUFFER								GL_READ_FRAMEBUFFER_EXT
	#define GL_RENDERBUFFER									GL_RENDERBUFFER_EXT
	#endif //TARGET_WIN32
*/
#else
	#define glGenFramebuffers								glGenFramebuffersOES
	#define glGenRenderbuffers								glGenRenderbuffersOES
	#define	glDeleteFramebuffers							glDeleteFramebuffersOES
	#define	glDeleteRenderbuffers							glDeleteRenderbuffersOES
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
	//#define GL_DEPTH_STENCIL_ATTACHMENT						GL_DEPTH_STENCIL_ATTACHMENT_OES
	#define GL_DEPTH_STENCIL								GL_DEPTH24_STENCIL8_OES
	#define GL_DEPTH_COMPONENT								GL_DEPTH_COMPONENT16_OES
	#define GL_STENCIL_INDEX								GL_STENCIL_INDEX8_OES
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
ofFbo::Settings::Settings() {
	width					= 0;
	height					= 0;
	numColorbuffers			= 1;
	useDepth				= false;
	useStencil				= false;
	depthAsTexture			= false;
#ifndef TARGET_OPENGLES
	textureTarget			= GL_TEXTURE_RECTANGLE_ARB;
#else
	textureTarget			= GL_TEXTURE_2D;
#endif
	internalformat			= GL_RGBA;
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
		ofLog(OF_LOG_WARNING,"ofFbo: releasing id not found, this shouldn't be happening releasing anyway");
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
		ofLog(OF_LOG_WARNING,"ofFbo: releasing id not found, this shouldn't be happening releasing anyway");
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
defaultTextureIndex(0)
{

}

ofFbo::ofFbo(const ofFbo & mom){
	settings = mom.settings;
	isBound = mom.isBound;

	fbo = mom.fbo;
	retainFB(fbo);
	fboTextures = mom.fboTextures;
	if(settings.numSamples){
		retainFB(fboTextures);
	}
	depthBuffer = mom.depthBuffer;
	retainRB(depthBuffer);
	stencilBuffer = mom.stencilBuffer;
	retainRB(stencilBuffer);

	savedFramebuffer = mom.savedFramebuffer;

	colorBuffers = mom.colorBuffers;
	for(int i=0;i<(int)colorBuffers.size();i++){
		retainRB(colorBuffers[i]);
	}
	textures = mom.textures;
}

ofFbo & ofFbo::operator=(const ofFbo & mom){
	if(&mom==this) return *this;
	settings = mom.settings;
	isBound = mom.isBound;

	fbo = mom.fbo;
	retainFB(fbo);
	fboTextures = mom.fboTextures;
	if(settings.numSamples){
		retainFB(fboTextures);
	}
	depthBuffer = mom.depthBuffer;
	retainRB(depthBuffer);
	stencilBuffer = mom.stencilBuffer;
	retainRB(stencilBuffer);

	savedFramebuffer = mom.savedFramebuffer;

	colorBuffers = mom.colorBuffers;
	for(int i=0;i<(int)colorBuffers.size();i++){
		retainRB(colorBuffers[i]);
	}
	textures = mom.textures;
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
}

static GLboolean CheckExtension( const char *extName ){
	/*
	 ** Search for extName in the extensions string.  Use of strstr()
	 ** is not sufficient because extension names can be prefixes of
	 ** other extension names.  Could use strtok() but the constant
	 ** string returned by glGetString can be in read-only memory.
	 */
	char *p = (char *) glGetString(GL_EXTENSIONS);
	char *end;
	int extNameLen;

	extNameLen = strlen(extName);
	end = p + strlen(p);

	while (p < end) {
		int n = strcspn(p, " ");
		if ((extNameLen == n) && (strncmp(extName, p, n) == 0)) {
			return GL_TRUE;
		}
		p += (n + 1);
	}
	return GL_FALSE;
}


bool ofFbo::checkGLSupport() {
#ifndef TARGET_OPENGLES
	if(CheckExtension("GL_EXT_framebuffer_object")){
		ofLog(OF_LOG_VERBOSE,"FBO supported");
	}else{
		ofLog(OF_LOG_ERROR, "FBO not supported by this graphics card");
		return false;
	}
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &_maxColorAttachments);
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &_maxDrawBuffers);
	glGetIntegerv(GL_MAX_SAMPLES, &_maxSamples);

	ofLog(OF_LOG_NOTICE, string("ofFbo::checkGLSupport()\n") +
		  "maxColorAttachments: " + ofToString(_maxColorAttachments) + "\n" +
		  "maxDrawBuffers: " + ofToString(_maxDrawBuffers) + "\n" +
		  "maxSamples: " + ofToString(_maxSamples)
		  );
#else

	if(CheckExtension("GL_OES_framebuffer_object")){
		ofLog(OF_LOG_VERBOSE,"FBO supported");
	}else{
		ofLog(OF_LOG_ERROR, "FBO not supported by this graphics card");
		return false;
	}
	string extensions = (char*)glGetString(GL_EXTENSIONS);
	ofLog(OF_LOG_VERBOSE,extensions);
#endif

	return true;
}


void ofFbo::allocate(int width, int height, int internalformat, int numSamples) {

	settings.width			= width;
	settings.height			= height;
	settings.internalformat	= internalformat;
	settings.numSamples		= numSamples;
	settings.useDepth		= true;
	settings.useStencil		= true;
	
	allocate(settings);
}


void ofFbo::allocate(Settings _settings) {
	if(!checkGLSupport()) return;

	destroy();

	if(_settings.width == 0) _settings.width = ofGetWidth();
	if(_settings.height == 0) _settings.height = ofGetHeight();
	settings = _settings;

	// create main fbo
	// this is the main one we bind for drawing into
	// all the renderbuffers are attached to this (whether MSAA is enabled or not)
	glGenFramebuffers(1, &fbo);
	retainFB(fbo);
	bind();

		
	// If we want both a depth AND a stencil buffer tehn combine them into a single buffer
#ifndef TARGET_OPENGLES
	if( settings.useDepth && settings.useStencil )
	{
		stencilBuffer = depthBuffer = createAndAttachRenderbuffer(GL_DEPTH_STENCIL, GL_DEPTH_STENCIL_ATTACHMENT);
		retainRB(depthBuffer);
		retainRB(stencilBuffer);
	}else
#endif
	{
		// if we want a depth buffer, create it, and attach to our main fbo
		if(settings.useDepth){
			if(!settings.depthAsTexture){
				depthBuffer = createAndAttachRenderbuffer(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
				retainRB(depthBuffer);
			}else{
				glGenTextures(1, &depthBuffer);
				//retainRB(depthBuffer);
				glBindTexture(GL_TEXTURE_2D, depthBuffer);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			#ifndef TARGET_OPENGLES
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
			#else
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			#endif
				glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, settings.width, settings.height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0 );
				glBindTexture( GL_TEXTURE_2D, 0 );
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, depthBuffer, 0);
			}
		}

		// if we want a stencil buffer, create it, and attach to our main fbo
		if(settings.useStencil){
			stencilBuffer = createAndAttachRenderbuffer(GL_STENCIL_INDEX, GL_STENCIL_ATTACHMENT);
			retainRB(stencilBuffer);
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
		ofLog(OF_LOG_WARNING,"ofFbo: multisampling not supported in opengles");
	}
#endif
	// now create all textures and color buffers
	for(int i=0; i<settings.numColorbuffers; i++) createAndAttachTexture(i);

	// if textures are attached to a different fbo (e.g. if using MSAA) check it's status
	if(fbo != fboTextures) {
		glBindFramebuffer(GL_FRAMEBUFFER, fboTextures);
	}

	// check everything is ok with this fbo
	checkStatus();
	
	// unbind it
	unbind();
}

/*  removed by now, was crashing on draw
 *
void ofFbo::allocateForShadow( int width, int height )
{
//#ifndef TARGET_OPENGLES
	int old;
	glGetIntegerv( GL_FRAMEBUFFER_BINDING, &old );
	
	settings.width = width;
	settings.height = height;
	
	glGenTextures(1, &depthBuffer);
	retainRB(depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#ifndef TARGET_OPENGLES
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
#else
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
#endif
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, settings.width, settings.height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
	
	glGenFramebuffers( 1, &fbo );
	retainFB(fbo);
	glBindFramebuffer( GL_FRAMEBUFFER, fbo );
	
#ifndef TARGET_OPENGLES
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );
#endif

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, depthBuffer, 0);
	
	if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
		printf("Can't use FBOs !\n");
	
	glBindFramebuffer( GL_FRAMEBUFFER, old );
}*/

GLuint ofFbo::createAndAttachRenderbuffer(GLenum internalFormat, GLenum attachmentPoint) {
	GLuint buffer;
	glGenRenderbuffers(1, &buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
#ifndef TARGET_OPENGLES
	if(settings.numSamples==0) glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, settings.width, settings.height);
	else glRenderbufferStorageMultisample(GL_RENDERBUFFER, settings.numSamples, internalFormat, settings.width, settings.height);
#else
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, settings.width, settings.height);
#endif
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentPoint, GL_RENDERBUFFER, buffer);
	return buffer;
}


void ofFbo::createAndAttachTexture(GLenum attachmentPoint) {
	// bind fbo for textures (if using MSAA this is the newly created fbo, otherwise its the same fbo as before)
	glBindFramebuffer(GL_FRAMEBUFFER, fboTextures);

	ofTexture tex;
	tex.texData.bFlipTexture = true;
	tex.allocate(settings.width, settings.height, settings.internalformat, settings.textureTarget == GL_TEXTURE_2D ? false : true);
	tex.setTextureWrap(settings.wrapModeHorizontal, settings.wrapModeVertical);
	tex.setTextureMinMagFilter(settings.minFilter, settings.maxFilter);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPoint, tex.texData.textureTarget, tex.texData.textureID, 0);
	textures.push_back(tex);


	// if MSAA, bind main fbo and attach renderbuffer
	if(settings.numSamples) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		GLuint colorBuffer = createAndAttachRenderbuffer(settings.internalformat, GL_COLOR_ATTACHMENT0 + attachmentPoint);
		colorBuffers.push_back(colorBuffer);
		retainRB(colorBuffer);
	}
}


void ofFbo::begin() {
	bind();
	ofPushView();
	if(ofGetGLRenderer()){
		ofGetGLRenderer()->setCurrentFBO(this);
	}
	ofViewport(0, 0, getWidth(), getHeight(), false);
	ofSetupScreenPerspective(getWidth(), getHeight(), ofGetOrientation(), false);
}

//void ofViewport(float x = 0, float y = 0, float width = 0, float height = 0, bool invertY = true);
//void ofSetupScreenPerspective(float width = 0, float height = 0, int orientation = 0, bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0);


void ofFbo::end() {
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

void ofFbo::setDefaultTextureIndex(int defaultTexture)
{
	defaultTextureIndex = defaultTexture;
}
	
int ofFbo::getDefaultTextureIndex()
{
	return defaultTextureIndex;
}

ofTexture& ofFbo::getTextureReference() {
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
#ifndef TARGET_OPENGLES
	getTextureReference(attachmentPoint).readToPixels(pixels);
#else
	bind();
	int format,type;
	ofGetGlFormatAndType(settings.internalformat,format,type);
	glReadPixels(0,0,settings.width, settings.height, format, GL_UNSIGNED_BYTE, pixels.getPixels());
	unbind();
#endif
}

void ofFbo::readToPixels(ofShortPixels & pixels, int attachmentPoint){
#ifndef TARGET_OPENGLES
	getTextureReference(attachmentPoint).readToPixels(pixels);
#else
	bind();
	int format,type;
	ofGetGlFormatAndType(settings.internalformat,format,type);
	glReadPixels(0,0,settings.width, settings.height, format, GL_UNSIGNED_SHORT, pixels.getPixels());
	unbind();
#endif
}

void ofFbo::readToPixels(ofFloatPixels & pixels, int attachmentPoint){
#ifndef TARGET_OPENGLES
	getTextureReference(attachmentPoint).readToPixels(pixels);
#else
	bind();
	int format,type;
	ofGetGlFormatAndType(settings.internalformat,format,type);
	glReadPixels(0,0,settings.width, settings.height, format, GL_FLOAT, pixels.getPixels());
	unbind();
#endif
}

void ofFbo::updateTexture(int attachmentPoint) {
	// TODO: flag to see if this is dirty or not
#ifndef TARGET_OPENGLES
	if(fbo != fboTextures && dirty) {
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
		glBlitFramebuffer(0, 0, settings.width, settings.height, 0, 0, settings.width, settings.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, savedFramebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, savedFramebuffer);
		glBindFramebuffer( GL_FRAMEBUFFER, savedFramebuffer );

		// restore readbuffer
		glReadBuffer(readBuffer);

		// restore drawbuffer
		glPopAttrib();
		dirty = false;

	}
#endif
}



void ofFbo::draw(float x, float y) {
	draw(x, y, settings.width, settings.height);
}


void ofFbo::draw(float x, float y, float width, float height) {
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
		case GL_FRAMEBUFFER_UNSUPPORTED:
			ofLog(OF_LOG_ERROR, "FRAMEBUFFER_UNSUPPORTED");
			break;
#ifndef TARGET_OPENGLES
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			ofLog(OF_LOG_ERROR, "FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			ofLog(OF_LOG_ERROR, "FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			ofLog(OF_LOG_ERROR, "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
			break;
#endif
		default:
			ofLog(OF_LOG_ERROR, "UNKNOWN ERROR");
			break;

	}

	return false;
}

//#endif
