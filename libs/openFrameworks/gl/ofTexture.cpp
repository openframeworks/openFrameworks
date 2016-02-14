#include "ofTexture.h"
#include "ofUtils.h"		// for nextPow2()
#include "ofAppRunner.h"	// for getWidth()
#include "ofGraphics.h"
#include "ofPixels.h"
#include "ofGLUtils.h"
#include <map>

//----------------------------------------------------------
// static
static bool bTexHackEnabled = true;
static bool	bUsingArbTex		= true;
static bool bUsingNormalizedTexCoords = false;
static bool bUseCustomMinMagFilters = false;


//---------------------------------
void ofEnableTextureEdgeHack(){
	bTexHackEnabled = true;
}

//---------------------------------
void ofDisableTextureEdgeHack(){
	bTexHackEnabled = false;
}

//---------------------------------
bool ofIsTextureEdgeHackEnabled(){
	return bTexHackEnabled;
}

//---------------------------------
bool ofGetUsingNormalizedTexCoords(){
	return bUsingNormalizedTexCoords;
}

//---------------------------------
void ofEnableNormalizedTexCoords(){
	bUsingNormalizedTexCoords = true;
}

//---------------------------------
void ofDisableNormalizedTexCoords(){
	bUsingNormalizedTexCoords = false;
}



//***** add global functions to override texture settings
//----------------------------------------------------------
static bool bUseCustomTextureWrap = false;

//----------------------------------------------------------
void ofSetTextureWrap(GLfloat wrapS, GLfloat wrapT){
	bUseCustomTextureWrap = true;
	GLenum textureTarget = GL_TEXTURE_2D;
#ifndef TARGET_OPENGLES
	if (ofGetUsingArbTex() && GL_ARB_texture_rectangle){
		textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	};
#endif
	glTexParameterf(textureTarget, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameterf(textureTarget, GL_TEXTURE_WRAP_T, wrapT);
}

//----------------------------------------------------------
bool ofGetUsingCustomTextureWrap(){
	return bUseCustomTextureWrap;
}

//----------------------------------------------------------
void ofRestoreTextureWrap(){
	bUseCustomTextureWrap = false;
}

//----------------------------------------------------------
void ofSetMinMagFilters(GLfloat minFilter, GLfloat magFilter){
	bUseCustomMinMagFilters = true;
	GLenum textureTarget = GL_TEXTURE_2D;
#ifndef TARGET_OPENGLES
	if (ofGetUsingArbTex() && GL_ARB_texture_rectangle){
		textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	};
#endif
	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, magFilter);
}

//----------------------------------------------------------
bool ofGetUsingCustomMinMagFilters(){
	return bUseCustomMinMagFilters;
}

//----------------------------------------------------------
void ofRestoreMinMagFilters(){
	bUseCustomMinMagFilters = false;
}

//***** global functions to override texture settings


//----------------------------------------------------------
bool ofGetUsingArbTex(){
	return bUsingArbTex;
}

//----------------------------------------------------------
void ofEnableArbTex(){
	bUsingArbTex = true;
}

//----------------------------------------------------------
void ofDisableArbTex(){
	bUsingArbTex = false;
}


static map<GLuint,int> & getTexturesIndex(){
	static map<GLuint,int> * textureReferences = new map<GLuint,int>;
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
				glDeleteTextures(1, (GLuint *)&id);
				getTexturesIndex().erase(id);
			}
		}else{
			ofLogError("ofTexture") << "release(): something's wrong here, releasing unknown texture id " << id;
			glDeleteTextures(1, (GLuint *)&id);
		}
	}
}

#ifdef TARGET_ANDROID
static set<ofTexture*> & allTextures(){
	static set<ofTexture*> * allTextures = new set<ofTexture*>;
	return *allTextures;
}

static void registerTexture(ofTexture * texture){
	allTextures().insert(texture);
}

static void unregisterTexture(ofTexture * texture){
	allTextures().erase(texture);
}

void ofRegenerateAllTextures(){
	for(auto tex: allTextures()){
		tex->clear();
	}
}
#endif

//----------------------------------------------------------
ofTexture::ofTexture(){
	resetAnchor();
	bWantsMipmap = false;
}

//----------------------------------------------------------
ofTexture::ofTexture(const ofTexture & mom){
	anchor = mom.anchor;
	bAnchorIsPct = mom.bAnchorIsPct;
	texData = mom.texData;
	bWantsMipmap = mom.bWantsMipmap;
	retain(texData.textureID);
#ifdef TARGET_ANDROID
	registerTexture(this);
#endif
}

ofTexture::ofTexture(ofTexture && mom){
    anchor = mom.anchor;
    bAnchorIsPct = mom.bAnchorIsPct;
    texData = mom.texData;
    bWantsMipmap = mom.bWantsMipmap;
    mom.texData.bAllocated = 0;
    mom.texData.textureID = 0;
#ifdef TARGET_ANDROID
    registerTexture(this);
#endif
}

//----------------------------------------------------------
ofTexture& ofTexture::operator=(const ofTexture & mom){
	if(!texData.bUseExternalTextureID){
		release(texData.textureID);
	}
	anchor = mom.anchor;
	bAnchorIsPct = mom.bAnchorIsPct;
	texData = mom.texData;
	bWantsMipmap = mom.bWantsMipmap;
	retain(texData.textureID);
#ifdef TARGET_ANDROID
	unregisterTexture(this);
#endif
	return *this;
}

//----------------------------------------------------------
ofTexture& ofTexture::operator=(ofTexture && mom){
    if(!texData.bUseExternalTextureID){
        release(texData.textureID);
    }
    anchor = mom.anchor;
    bAnchorIsPct = mom.bAnchorIsPct;
    texData = mom.texData;
    bWantsMipmap = mom.bWantsMipmap;
    mom.texData.bAllocated = 0;
    mom.texData.textureID = 0;
#ifdef TARGET_ANDROID
    unregisterTexture(this);
#endif
    return *this;
}

//----------------------------------------------------------
bool ofTexture::bAllocated() const {
	return texData.bAllocated;
}

//----------------------------------------------------------
bool ofTexture::isAllocated() const {
	return texData.bAllocated;
}


//----------------------------------------------------------
ofTextureData& ofTexture::getTextureData(){
	if(!texData.bAllocated){
		ofLogError("ofTexture") << "getTextureData(): texture has not been allocated";
	}
	
	return texData;
}

const ofTextureData& ofTexture::getTextureData() const {
	if(!texData.bAllocated){
		ofLogError("ofTexture") << "getTextureData(): texture has not been allocated";
	}
	
	return texData;
}

//----------------------------------------------------------
ofTexture::~ofTexture(){
	if(!texData.bUseExternalTextureID){
		release(texData.textureID);
	}
#ifdef TARGET_ANDROID
	unregisterTexture(this);
#endif
}

//----------------------------------------------------------
void ofTexture::clear(){
	if(!texData.bUseExternalTextureID){
		release(texData.textureID);
	}
	texData.bUseExternalTextureID = false;
	texData.textureID  = 0;
	texData.bAllocated = false;
}

//----------------------------------------------------------
void ofTexture::setUseExternalTextureID(GLuint externTexID){
	clear();
	texData.textureID = externTexID;
	texData.bAllocated = true;
	texData.bUseExternalTextureID = true;
}

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int glInternalFormat){
	allocate(w, h, glInternalFormat, ofGetUsingArbTex(), ofGetGLFormatFromInternal(glInternalFormat), ofGetGlTypeFromInternal(glInternalFormat));
}

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int glInternalFormat, bool bUseARBExtension){
	allocate(w, h, glInternalFormat, bUseARBExtension, ofGetGLFormatFromInternal(glInternalFormat), ofGetGlTypeFromInternal(glInternalFormat));
}

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int glInternalFormat, int glFormat, int pixelType){
	allocate(w, h, glInternalFormat, ofGetUsingArbTex(), glFormat, pixelType);
}

//----------------------------------------------------------
void ofTexture::allocate(const ofPixels& pix){
	allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), ofGetUsingArbTex(), ofGetGlFormat(pix), ofGetGlType(pix));
	if((pix.getPixelFormat()==OF_PIXELS_GRAY || pix.getPixelFormat()==OF_PIXELS_GRAY_ALPHA) && ofIsGLProgrammableRenderer()){
		setRGToRGBASwizzles(true);
	}
	if(texData.bAllocated) loadData(pix);
}

//----------------------------------------------------------
void ofTexture::allocate(const ofPixels& pix, bool bUseARBExtention){
	allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), bUseARBExtention, ofGetGlFormat(pix), ofGetGlType(pix));
	if((pix.getPixelFormat()==OF_PIXELS_GRAY || pix.getPixelFormat()==OF_PIXELS_GRAY_ALPHA) && ofIsGLProgrammableRenderer()){
		setRGToRGBASwizzles(true);
	}
	if(texData.bAllocated) loadData(pix);
}

//----------------------------------------------------------
void ofTexture::allocate(const ofShortPixels& pix){
	allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), ofGetUsingArbTex(), ofGetGlFormat(pix), ofGetGlType(pix));
	if((pix.getPixelFormat()==OF_PIXELS_GRAY || pix.getPixelFormat()==OF_PIXELS_GRAY_ALPHA) && ofIsGLProgrammableRenderer()){
		setRGToRGBASwizzles(true);
	}
	if(texData.bAllocated) loadData(pix);
}

//----------------------------------------------------------
void ofTexture::allocate(const ofShortPixels& pix, bool bUseARBExtention){
	allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), bUseARBExtention, ofGetGlFormat(pix), ofGetGlType(pix));
	if((pix.getPixelFormat()==OF_PIXELS_GRAY || pix.getPixelFormat()==OF_PIXELS_GRAY_ALPHA) && ofIsGLProgrammableRenderer()){
		setRGToRGBASwizzles(true);
	}
	if(texData.bAllocated) loadData(pix);
}


//----------------------------------------------------------
void ofTexture::allocate(const ofFloatPixels& pix){
	allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), ofGetUsingArbTex(), ofGetGlFormat(pix), ofGetGlType(pix));
	if((pix.getPixelFormat()==OF_PIXELS_GRAY || pix.getPixelFormat()==OF_PIXELS_GRAY_ALPHA) && ofIsGLProgrammableRenderer()){
		setRGToRGBASwizzles(true);
	}
	if(texData.bAllocated) loadData(pix);
}

//----------------------------------------------------------
void ofTexture::allocate(const ofFloatPixels& pix, bool bUseARBExtention){
	allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), bUseARBExtention, ofGetGlFormat(pix), ofGetGlType(pix));
	if((pix.getPixelFormat()==OF_PIXELS_GRAY || pix.getPixelFormat()==OF_PIXELS_GRAY_ALPHA) && ofIsGLProgrammableRenderer()){
		setRGToRGBASwizzles(true);
	}
	if(texData.bAllocated) loadData(pix);
}

#ifndef TARGET_OPENGLES
//----------------------------------------------------------
void ofTexture::allocateAsBufferTexture(const ofBufferObject & buffer, int glInternalFormat){
	texData.glInternalFormat = glInternalFormat;
	texData.textureTarget = GL_TEXTURE_BUFFER;
	texData.bufferId = buffer.getId();
	allocate(texData,0,0);
	buffer.bind(GL_TEXTURE_BUFFER);
}
#endif

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int glInternalFormat, bool bUseARBExtension, int glFormat, int pixelType){
	texData.width = w;
	texData.height = h;
	texData.bFlipTexture = false;
	texData.glInternalFormat = glInternalFormat;
	//our graphics card might not support arb so we have to see if it is supported.
#ifndef TARGET_OPENGLES
	if (bUseARBExtension && GL_ARB_texture_rectangle){
		texData.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	} else 
#endif
	{
		texData.textureTarget = GL_TEXTURE_2D;
	}
	
	allocate(texData,glFormat,pixelType);
}

//----------------------------------------------------------

void ofTexture::allocate(const ofTextureData & textureData){
	allocate(textureData,ofGetGLFormatFromInternal(textureData.glInternalFormat),ofGetGlTypeFromInternal(textureData.glInternalFormat));
}

//----------------------------------------------------------

void ofTexture::allocate(const ofTextureData & textureData, int glFormat, int pixelType){
#ifndef TARGET_OPENGLES
	if(texData.textureTarget == GL_TEXTURE_2D || texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
#else
	if(texData.textureTarget == GL_TEXTURE_2D){
#endif
		if( textureData.width <= 0.0 || textureData.height <= 0.0 ){
			ofLogError("ofTexture") << "allocate(): ofTextureData has 0 width and/or height: " << textureData.width << "x" << textureData.height;
			return;
		}
	}

	texData = textureData;
	//our graphics card might not support arb so we have to see if it is supported.
#ifndef TARGET_OPENGLES
	if( texData.textureTarget==GL_TEXTURE_RECTANGLE_ARB && ofGLSupportsNPOTTextures() ){
		texData.tex_w = texData.width;
		texData.tex_h = texData.height;
		texData.tex_t = texData.width;
		texData.tex_u = texData.height;
	}else if(texData.textureTarget == GL_TEXTURE_2D)
#endif
	{
		if(ofGLSupportsNPOTTextures()){
			texData.tex_w = texData.width;
			texData.tex_h = texData.height;
		}else{
			//otherwise we need to calculate the next power of 2 for the requested dimensions
			//ie (320x240) becomes (512x256)
			texData.tex_w = ofNextPow2(texData.width);
			texData.tex_h = ofNextPow2(texData.height);
		}

		texData.tex_t = texData.width / texData.tex_w;
		texData.tex_u = texData.height / texData.tex_h;
	}

	// attempt to free the previous bound texture, if we can:
	clear();

	glGenTextures(1, (GLuint *)&texData.textureID);   // could be more then one, but for now, just one
	retain(texData.textureID);

#ifndef TARGET_OPENGLES
	if(texData.textureTarget == GL_TEXTURE_2D || texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
#else
	if(texData.textureTarget == GL_TEXTURE_2D){
#endif
		glBindTexture(texData.textureTarget,texData.textureID);
		glTexImage2D(texData.textureTarget, 0, texData.glInternalFormat, (GLint)texData.tex_w, (GLint)texData.tex_h, 0, glFormat, pixelType, 0);  // init to black...

		glTexParameterf(texData.textureTarget, GL_TEXTURE_MAG_FILTER, texData.magFilter);
		glTexParameterf(texData.textureTarget, GL_TEXTURE_MIN_FILTER, texData.minFilter);
		glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_S, texData.wrapModeHorizontal);
		glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_T, texData.wrapModeVertical);

		#ifndef TARGET_PROGRAMMABLE_GL
			if (!ofIsGLProgrammableRenderer()){
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
		#endif
		glBindTexture(texData.textureTarget,0);
	}

	texData.bAllocated = true;

#ifdef TARGET_ANDROID
	registerTexture(this);
#endif
}


void ofTexture::setRGToRGBASwizzles(bool rToRGBSwizzles){
#ifndef TARGET_OPENGLES
	glBindTexture(texData.textureTarget,texData.textureID);
	if(rToRGBSwizzles){
		if(texData.glInternalFormat==GL_R8 ||
				texData.glInternalFormat==GL_R16 ||
				texData.glInternalFormat==GL_R32F){
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_R, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_G, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_B, GL_RED);

		}else if(texData.glInternalFormat==GL_RG8 ||
				texData.glInternalFormat==GL_RG16 ||
				texData.glInternalFormat==GL_RG32F){
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_R, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_G, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_B, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
		}
	}else{
		if(texData.glInternalFormat==GL_R8 ||
				texData.glInternalFormat==GL_R16 ||
				texData.glInternalFormat==GL_R32F){
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_R, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_B, GL_BLUE);

		}else if(texData.glInternalFormat==GL_RG8 ||
				texData.glInternalFormat==GL_RG16 ||
				texData.glInternalFormat==GL_RG32F){
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_R, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
		}
	}
	glBindTexture(texData.textureTarget,0);
#endif
}

void ofTexture::setSwizzle(GLenum srcSwizzle, GLenum dstChannel){
#ifndef TARGET_OPENGLES
	glBindTexture(texData.textureTarget,texData.textureID);
	glTexParameteri(texData.textureTarget, srcSwizzle, dstChannel);
	glBindTexture(texData.textureTarget,0);
#endif
}

//----------------------------------------------------------
void ofTexture::loadData(const unsigned char * data, int w, int h, int glFormat){
	ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT,w,1,ofGetNumChannelsFromGLFormat(glFormat));
	loadData(data, w, h, glFormat, GL_UNSIGNED_BYTE);
}

//----------------------------------------------------------
void ofTexture::loadData(const unsigned short * data, int w, int h, int glFormat){
	ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT,w,2,ofGetNumChannelsFromGLFormat(glFormat));
	loadData(data, w, h, glFormat, GL_UNSIGNED_SHORT);
}

//----------------------------------------------------------
void ofTexture::loadData(const float * data, int w, int h, int glFormat){
	ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT,w,4,ofGetNumChannelsFromGLFormat(glFormat));
	loadData(data, w, h, glFormat, GL_FLOAT);
}

//----------------------------------------------------------
void ofTexture::loadData(const ofPixels & pix){
	if(!isAllocated()){
		allocate(pix);
	}else{
		ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT,pix.getBytesStride());
		loadData(pix.getData(), pix.getWidth(), pix.getHeight(), ofGetGlFormat(pix), ofGetGlType(pix));
	}
}

//----------------------------------------------------------
void ofTexture::loadData(const ofShortPixels & pix){
	if(!isAllocated()){
		allocate(pix);
	}else{
		ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT,pix.getBytesStride());
		loadData(pix.getData(), pix.getWidth(), pix.getHeight(), ofGetGlFormat(pix), ofGetGlType(pix));
	}
}

//----------------------------------------------------------
void ofTexture::loadData(const ofFloatPixels & pix){
	if(!isAllocated()){
		allocate(pix);
	}else{
		ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT,pix.getBytesStride());
		loadData(pix.getData(), pix.getWidth(), pix.getHeight(), ofGetGlFormat(pix), ofGetGlType(pix));
	}
}

//----------------------------------------------------------
void ofTexture::loadData(const ofPixels & pix, int glFormat){
	if(!isAllocated()){
		allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), ofGetUsingArbTex(), glFormat, ofGetGlType(pix));
	}
	ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT,pix.getWidth(),pix.getBytesPerChannel(),ofGetNumChannelsFromGLFormat(glFormat));
	loadData(pix.getData(), pix.getWidth(), pix.getHeight(), glFormat, ofGetGlType(pix));
}

//----------------------------------------------------------
void ofTexture::loadData(const ofShortPixels & pix, int glFormat){
	if(!isAllocated()){
		allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), ofGetUsingArbTex(), glFormat, ofGetGlType(pix));
	}
	ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT,pix.getWidth(),pix.getBytesPerChannel(),ofGetNumChannelsFromGLFormat(glFormat));
	loadData(pix.getData(), pix.getWidth(), pix.getHeight(), glFormat, ofGetGlType(pix));
}

//----------------------------------------------------------
void ofTexture::loadData(const ofFloatPixels & pix, int glFormat){
	if(!isAllocated()){
		allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), ofGetUsingArbTex(), glFormat, ofGetGlType(pix));
	}
	ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT,pix.getWidth(),pix.getBytesPerChannel(),ofGetNumChannelsFromGLFormat(glFormat));
	loadData(pix.getData(), pix.getWidth(), pix.getHeight(), glFormat, ofGetGlType(pix));
}

#ifndef TARGET_OPENGLES
//----------------------------------------------------------
void ofTexture::loadData(const ofBufferObject & buffer, int glFormat, int glType){
	buffer.bind(GL_PIXEL_UNPACK_BUFFER);
	loadData(0,texData.width,texData.height,glFormat,glType);
	buffer.unbind(GL_PIXEL_UNPACK_BUFFER);
}
#endif

//----------------------------------------------------------
void ofTexture::loadData(const void * data, int w, int h, int glFormat, int glType){

	if(w > texData.tex_w || h > texData.tex_h) {
		if(isAllocated()){
			allocate(w, h, texData.glInternalFormat, glFormat, glType);
		}else{
			// TODO: guess correct internal from glFormat
			allocate(w, h, glFormat, glFormat, glType);
		}
	}
	
	// compute new tex co-ords based on the ratio of data's w, h to texture w,h;
#ifndef TARGET_OPENGLES
	if (texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
		texData.tex_t = w;
		texData.tex_u = h;
	} else 
#endif
	{
		texData.tex_t = (float)(w) / (float)texData.tex_w;
		texData.tex_u = (float)(h) / (float)texData.tex_h;
	}
	
	// bind texture
	glBindTexture(texData.textureTarget, (GLuint) texData.textureID);
	//update the texture image:
	glTexSubImage2D(texData.textureTarget, 0, 0, 0, w, h, glFormat, glType, data);
	// unbind texture target by binding 0
	glBindTexture(texData.textureTarget, 0);
	
	if (bWantsMipmap) {
		// auto-generate mipmap, since this ofTexture wants us to.
		generateMipmap();
	}
	
}

//----------------------------------------------------------
void ofTexture::generateMipmap(){

	// Generate mipmaps using hardware-accelerated core GL methods.
	
	// 1. Check whether the current OpenGL version supports mipmap generation:
	//    glGenerateMipmap() was introduced to OpenGL core in 3.0, and
	//    OpenGLES core in 2.0 but earlier versions may support it if they
	//	  support extension GL_EXT_framebuffer_object

	bool isGlGenerateMipmapAvailable = false;
	if(ofIsGLProgrammableRenderer()){
		isGlGenerateMipmapAvailable = true;
	}
	
	
	if (!isGlGenerateMipmapAvailable && !ofGLCheckExtension("GL_EXT_framebuffer_object")) {
		static bool versionWarningIssued = false;
		if (!versionWarningIssued) ofLogWarning() << "Your current OpenGL version does not support mipmap generation via glGenerateMipmap().";
		versionWarningIssued = true;
		texData.hasMipmap = false;
		return;
	}

	// 2. Check whether the texture's texture target supports mipmap generation.
	
	switch (texData.textureTarget) {
			/// OpenGL ES only supports mipmap for the following two texture targets:
		case GL_TEXTURE_2D:
		case GL_TEXTURE_CUBE_MAP:
#ifndef TARGET_OPENGLES
			/// OpenGL supports mipmaps for additional texture targets:
		case GL_TEXTURE_1D:
		case GL_TEXTURE_3D:
		case GL_TEXTURE_1D_ARRAY:
		case GL_TEXTURE_2D_ARRAY:
#endif
		{
			// All good, this particular texture target supports mipmaps.
			
			// glEnable(texData.textureTarget);	/// < uncomment this hack if you are unlucky enough to run an older ATI card.
			// See also: https://www.opengl.org/wiki/Common_Mistakes#Automatic_mipmap_generation

			glBindTexture(texData.textureTarget, (GLuint) texData.textureID);
			glGenerateMipmap(texData.textureTarget);
			glBindTexture(texData.textureTarget, 0);
			texData.hasMipmap = true;
			break;
		}
		default:
		{
			// This particular texture target does not support mipmaps.
			static bool warningIssuedAlready = false;
			
			if (!warningIssuedAlready){
				ofLogWarning() << "Mipmaps are not supported for textureTarget 0x" << hex << texData.textureTarget << endl
				<< "Most probably you are trying to create mipmaps from a GL_TEXTURE_RECTANGLE texture." << endl
				<< "Try ofDisableArbTex() before loading this texture.";
				warningIssuedAlready = true;
			}
			texData.hasMipmap = false;
			break;
		}
	} // end switch(texData.textureTarget)
		
}

//----------------------------------------------------------
void ofTexture::loadScreenData(int x, int y, int w, int h){
	// TODO: this should go into the renderers so it
	// doesn't depend on global calls
	int screenHeight = ofGetViewportHeight();
	y = screenHeight - y;
	y -= h; // top, bottom issues
	texData.bFlipTexture = true;
	
	if ( w > texData.tex_w || h > texData.tex_h) {
		ofLogError("ofTexture") << "loadScreenData(): " << w << "x" << h << " image data too big for "
		<< texData.tex_w << "x " << texData.tex_h << " allocated texture, not uploading";
		return;
	}
	
	//update our size with the new dimensions - this should be the same size or smaller than the allocated texture size
	texData.width 	= w;
	texData.height 	= h;
	
	//compute new tex co-ords based on the ratio of data's w, h to texture w,h;
#ifndef TARGET_OPENGLES // DAMIAN
	if (texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
		texData.tex_t = (float)(w);
		texData.tex_u = (float)(h);
	} else 
#endif
	{
		texData.tex_t = (float)(w) / (float)texData.tex_w;
		texData.tex_u = (float)(h) / (float)texData.tex_h;
	}
	
	
	glBindTexture(texData.textureTarget,texData.textureID);

	glCopyTexSubImage2D(texData.textureTarget, 0,0,0,x,y,w,h);

	glBindTexture(texData.textureTarget,0);
	
	if (bWantsMipmap) {
		generateMipmap();
	}
}


//we could cap these values - but it might be more useful
//to be able to set anchor points outside the image

//----------------------------------------------------------
void ofTexture::setAnchorPercent(float xPct, float yPct){
	anchor.x  = xPct;
	anchor.y  = yPct;
	
	bAnchorIsPct = true;
}

//----------------------------------------------------------
void ofTexture::setAnchorPoint(float x, float y){
	anchor.x = x;
	anchor.y = y;
	
	bAnchorIsPct = false;
}

//----------------------------------------------------------
void ofTexture::resetAnchor(){
	anchor.set( 0 );
	bAnchorIsPct = false;
}

//----------------------------------------------------------
void ofTexture::bind(int textureLocation) const{
	ofGetGLRenderer()->bind(*this,textureLocation);
}

//----------------------------------------------------------
void ofTexture::unbind(int textureLocation) const{
	ofGetGLRenderer()->unbind(*this,textureLocation);
}

#if !defined(TARGET_OPENGLES) && defined(glBindImageTexture)
//----------------------------------------------------------
void ofTexture::bindAsImage(GLuint unit, GLenum access, GLint level, GLboolean layered, GLint layer){
	glBindImageTexture(unit,texData.textureID,level,layered,layer,access,texData.glInternalFormat);
}
#endif

//----------------------------------------------------------
void ofTexture::setAlphaMask(ofTexture & mask){
	if(mask.texData.textureTarget!=this->texData.textureTarget){
		ofLogError("ofTexture") << "Cannot set alpha mask with different texture target";
	}else{
		texData.alphaMask = shared_ptr<ofTexture>(new ofTexture(mask));
	}
}

//----------------------------------------------------------
const ofTexture * ofTexture::getAlphaMask() const{
	return texData.alphaMask.get();
}

//----------------------------------------------------------
void ofTexture::disableAlphaMask(){
	if(texData.alphaMask){
		texData.alphaMask.reset();
	}
}


//----------------------------------------------------------
ofPoint ofTexture::getCoordFromPoint(float xPos, float yPos) const{
	
	ofPoint temp;
	
	if (!isAllocated()) return temp;
	
#ifndef TARGET_OPENGLES	
	if (texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
		
		temp.set(xPos, yPos);
		
	} else {
#endif		
		// non arb textures are 0 to 1, so we 
		// (a) convert to a pct: 
		
		float pctx = xPos / texData.width;
		float pcty = yPos / texData.height;
		
		// (b) mult by our internal pct (since we might not be 0-1 internally)
		
		pctx *= texData.tex_t;
		pcty *= texData.tex_u;
		
		temp.set(pctx, pcty);
		
#ifndef TARGET_OPENGLES	
	}
#endif		
	
	return temp;
	
}

//----------------------------------------------------------
/// Sets a texture matrix that will be uploaded whenever the texture is
/// binded.
void ofTexture::setTextureMatrix(const ofMatrix4x4 & m){
	texData.textureMatrix = m;
	texData.useTextureMatrix = true;
}

//----------------------------------------------------------
/// Disable the texture matrix.
void ofTexture::disableTextureMatrix(){
	texData.useTextureMatrix = false;
	texData.textureMatrix.makeIdentityMatrix();
}


//----------------------------------------------------------
const ofMatrix4x4 & ofTexture::getTextureMatrix() const{
	return texData.textureMatrix;
}

//----------------------------------------------------------
bool ofTexture::isUsingTextureMatrix() const{
	return texData.useTextureMatrix;
}

//----------------------------------------------------------
ofPoint ofTexture::getCoordFromPercent(float xPct, float yPct) const{
	
	ofPoint temp;
	
	if (!isAllocated()) return temp;
	
#ifndef TARGET_OPENGLES	
	if (texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
		
		temp.set(xPct * texData.width, yPct * texData.height);
		
	} else {
#endif	
		xPct *= texData.tex_t;
		yPct *= texData.tex_u;
		temp.set(xPct, yPct);
		
#ifndef TARGET_OPENGLES	
	}
#endif	
	return temp;
}


//----------------------------------------------------------
void ofTexture::setTextureWrap(GLint wrapModeHorizontal, GLint wrapModeVertical) {
	glBindTexture(texData.textureTarget,texData.textureID);
	glTexParameteri(texData.textureTarget, GL_TEXTURE_WRAP_S, wrapModeHorizontal);
	glTexParameteri(texData.textureTarget, GL_TEXTURE_WRAP_T, wrapModeVertical);
	texData.wrapModeVertical = wrapModeVertical;
	texData.wrapModeHorizontal = wrapModeHorizontal;
	glBindTexture(texData.textureTarget,0);
}

//----------------------------------------------------------
void ofTexture::setTextureMinMagFilter(GLint minFilter, GLint magFilter){

	// Issue warning if mipmaps not present for mipmap based min filter.
	
	if ( (minFilter > GL_LINEAR) && texData.hasMipmap == false ){
		static bool hasWarnedNoMipmapsForMinFilter = false;
		if(!hasWarnedNoMipmapsForMinFilter) {
			ofLogWarning() << "Texture has no mipmaps - but minFilter 0x"<< hex << minFilter << " requires mipmaps."
			<< endl << "Call ofTexture::generateMipmaps() first.";
		}
		hasWarnedNoMipmapsForMinFilter = true;
		return;
	}

	// Issue warning if invalid magFilter specified.
	
	if ( (magFilter > GL_LINEAR ) ) {
		static bool hasWarnedInvalidMagFilter = false;
		if (!hasWarnedInvalidMagFilter) {
			ofLogWarning() << "magFilter must be either GL_LINEAR or GL_NEAREST.";
		}
		hasWarnedInvalidMagFilter = true;
		return;
	}

	glBindTexture(texData.textureTarget,texData.textureID);
	glTexParameteri(texData.textureTarget, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(texData.textureTarget, GL_TEXTURE_MIN_FILTER, minFilter);
	texData.magFilter = magFilter;
	texData.minFilter = minFilter;
	glBindTexture(texData.textureTarget,0);
}

//----------------------------------------------------------
void ofTexture::setCompression(ofTexCompression compression){
	texData.compressionType = compression;
}

//------------------------------------
void ofTexture::enableMipmap(){
	bWantsMipmap = true;
	texData.minFilter = GL_LINEAR_MIPMAP_LINEAR;
}

//------------------------------------
void ofTexture::disableMipmap(){
	bWantsMipmap = false;
	texData.minFilter = GL_LINEAR;
}

//------------------------------------
bool ofTexture::hasMipmap() const{
	return texData.hasMipmap;
}

//------------------------------------
void ofTexture::draw(float x, float y) const{
	draw(x,y,0,getWidth(),getHeight());
}

//------------------------------------
void ofTexture::draw(float x, float y, float z) const{
	draw(x,y,z,getWidth(),getHeight());
}

//------------------------------------
void ofTexture::draw(float x, float y, float w, float h) const{
	draw(x,y,0,w,h);
}

//------------------------------------
void ofTexture::draw(float x, float y, float z, float w, float h) const{
	drawSubsection(x,y,z,w,h,0,0,getWidth(),getHeight());
}

//------------------------------------
void ofTexture::drawSubsection(float x, float y, float w, float h, float sx, float sy) const{
	drawSubsection(x,y,0,w,h,sx,sy,w,h);
}

//------------------------------------
void ofTexture::drawSubsection(float x, float y, float w, float h, float sx, float sy, float _sw, float _sh) const{
	drawSubsection(x,y,0,w,h,sx,sy,_sw,_sh);
}

//------------------------------------
void ofTexture::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy) const{
	drawSubsection(x,y,z,w,h,sx,sy,w,h);
}

//----------------------------------------------------------
void ofTexture::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->draw(*this,x,y,z,w,h,sx,sy,sw,sh);
	}
}


//------------------------------------
ofMesh ofTexture::getMeshForSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh, bool vflipped, ofRectMode rectMode) const{
	ofMesh quad;
	if(!texData.bAllocated){
		return quad;
	}

	GLfloat px0 = x;		// up to you to get the aspect ratio right
	GLfloat py0 = y;
	GLfloat px1 = w+x;
	GLfloat py1 = h+y;

	if (texData.bFlipTexture == vflipped){
		swap(py0,py1);
	}

	// for rect mode center, let's do this:
	if (rectMode == OF_RECTMODE_CENTER){
		px0 -= w/2;
		py0 -= h/2;
		px1 -= w/2;
		py1 -= h/2;
	}

	//we translate our drawing points by our anchor point.
	//we still respect ofRectMode so if you have rect mode set to
	//OF_RECTMODE_CENTER your anchor will be relative to that.
	GLfloat anchorX;
	GLfloat anchorY;

	if(bAnchorIsPct){
		anchorX = anchor.x * w;
		anchorY = anchor.y * h;
	}else{
		anchorX = anchor.x;
		anchorY = anchor.y;
	}

	px0 -= anchorX;
	py0 -= anchorY;
	px1 -= anchorX;
	py1 -= anchorY;


	// -------------------------------------------------
	// complete hack to remove border artifacts.
	// slightly, slightly alters an image, scaling...
	// to remove the border.
	// we need a better solution for this, but
	// to constantly add a 2 pixel border on all uploaded images
	// is insane..

	GLfloat offsetw = 0.0f;
	GLfloat offseth = 0.0f;

	if (!ofGLSupportsNPOTTextures() && bTexHackEnabled) {
		offsetw = 1.0f / (texData.tex_w);
		offseth = 1.0f / (texData.tex_h);
	}
	// -------------------------------------------------

	ofPoint topLeft = getCoordFromPoint(sx, sy);
	ofPoint bottomRight = getCoordFromPoint(sx + sw, sy + sh);

	GLfloat tx0 = topLeft.x + offsetw;
	GLfloat ty0 = topLeft.y + offseth;
	GLfloat tx1 = bottomRight.x - offsetw;
	GLfloat ty1 = bottomRight.y - offseth;

	quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	quad.getVertices().resize(4);
	quad.getTexCoords().resize(4);
	quad.getVertices()[0].set(px0,py0,z);
	quad.getVertices()[1].set(px1,py0,z);
	quad.getVertices()[2].set(px1,py1,z);
	quad.getVertices()[3].set(px0,py1,z);

	quad.getTexCoords()[0].set(tx0,ty0);
	quad.getTexCoords()[1].set(tx1,ty0);
	quad.getTexCoords()[2].set(tx1,ty1);
	quad.getTexCoords()[3].set(tx0,ty1);

	return quad;
}

// ROGER
//----------------------------------------------------------
void ofTexture::draw(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3, const ofPoint & p4) const{

	// make sure we are on unit 0 - we may change this when setting shader samplers
	// before glEnable or else the shader gets confused
	/// ps: maybe if bUsingArbTex is enabled we should use glActiveTextureARB?
	//glActiveTexture(GL_TEXTURE0);
	shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		bind(0);
		renderer->draw(getQuad(p1,p2,p3,p4),OF_MESH_FILL);
		unbind(0);
	}
}

ofMesh ofTexture::getQuad(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3, const ofPoint & p4) const{
	// -------------------------------------------------
	// complete hack to remove border artifacts.
	// slightly, slightly alters an image, scaling...
	// to remove the border.
	// we need a better solution for this, but
	// to constantly add a 2 pixel border on all uploaded images
	// is insane..
	
	GLfloat offsetw = 0.0f;
	GLfloat offseth = 0.0f;
	
	if (texData.textureTarget == GL_TEXTURE_2D && bTexHackEnabled) {
		offsetw = 1.0f / (texData.tex_w);
		offseth = 1.0f / (texData.tex_h);
	}
	// -------------------------------------------------
	
	GLfloat tx0 = 0+offsetw;
	GLfloat ty0 = 0+offseth;
	GLfloat tx1 = texData.tex_t - offsetw;
	GLfloat ty1 = texData.tex_u - offseth;

	ofMesh quad;
	quad.getVertices().resize(4);
	quad.getTexCoords().resize(4);
	quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	quad.getVertices()[0].set(p1.x, p1.y, p1.z);
	quad.getVertices()[1].set(p2.x, p2.y, p2.z);
	quad.getVertices()[2].set(p3.x, p3.y, p3.z);
	quad.getVertices()[3].set(p4.x, p4.y, p4.z);
	
	quad.getTexCoords()[0].set(tx0,ty0);
	quad.getTexCoords()[1].set(tx1,ty0);
	quad.getTexCoords()[2].set(tx1,ty1);
	quad.getTexCoords()[3].set(tx0,ty1);
	return quad;
}

//----------------------------------------------------------
void ofTexture::readToPixels(ofPixels & pixels) const {
#ifndef TARGET_OPENGLES
	pixels.allocate(texData.width,texData.height,ofGetImageTypeFromGLType(texData.glInternalFormat));
	ofSetPixelStoreiAlignment(GL_PACK_ALIGNMENT,pixels.getWidth(),pixels.getBytesPerChannel(),pixels.getNumChannels());
	glBindTexture(texData.textureTarget,texData.textureID);
	glGetTexImage(texData.textureTarget,0,ofGetGlFormat(pixels),GL_UNSIGNED_BYTE, pixels.getData());
	glBindTexture(texData.textureTarget,0);
#endif
}

//----------------------------------------------------------
void ofTexture::readToPixels(ofShortPixels & pixels) const {
#ifndef TARGET_OPENGLES
	pixels.allocate(texData.width,texData.height,ofGetImageTypeFromGLType(texData.glInternalFormat));
	ofSetPixelStoreiAlignment(GL_PACK_ALIGNMENT,pixels.getWidth(),pixels.getBytesPerChannel(),pixels.getNumChannels());
	glBindTexture(texData.textureTarget,texData.textureID);
	glGetTexImage(texData.textureTarget,0,ofGetGlFormat(pixels),GL_UNSIGNED_SHORT,pixels.getData());
	glBindTexture(texData.textureTarget,0);
#endif
}

void ofTexture::readToPixels(ofFloatPixels & pixels) const {
#ifndef TARGET_OPENGLES
	pixels.allocate(texData.width,texData.height,ofGetImageTypeFromGLType(texData.glInternalFormat));
	ofSetPixelStoreiAlignment(GL_PACK_ALIGNMENT,pixels.getWidth(),pixels.getBytesPerChannel(),pixels.getNumChannels());
	glBindTexture(texData.textureTarget,texData.textureID);
	glGetTexImage(texData.textureTarget,0,ofGetGlFormat(pixels),GL_FLOAT,pixels.getData());
	glBindTexture(texData.textureTarget,0);
#endif
}

#ifndef TARGET_OPENGLES
//----------------------------------------------------------
void ofTexture::copyTo(ofBufferObject & buffer) const{
	ofSetPixelStoreiAlignment(GL_PACK_ALIGNMENT,getWidth(),ofGetBytesPerChannelFromGLType(ofGetGlTypeFromInternal(texData.glInternalFormat)),ofGetNumChannelsFromGLFormat(ofGetGLFormatFromInternal(texData.glInternalFormat)));
	buffer.bind(GL_PIXEL_PACK_BUFFER);
	glBindTexture(texData.textureTarget,texData.textureID);
	glGetTexImage(texData.textureTarget,0,ofGetGLFormatFromInternal(texData.glInternalFormat),ofGetGlTypeFromInternal(texData.glInternalFormat),0);
	glBindTexture(texData.textureTarget,0);
	buffer.unbind(GL_PIXEL_PACK_BUFFER);

}
#endif

//----------------------------------------------------------
float ofTexture::getHeight() const {
	return texData.height;
}

//----------------------------------------------------------
float ofTexture::getWidth() const {
	return texData.width;
}
