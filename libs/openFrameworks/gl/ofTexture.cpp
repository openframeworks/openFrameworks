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
void ofSetMinMagFilters(GLfloat minFilter, GLfloat maxFilter){
	bUseCustomMinMagFilters = true;
	GLenum textureTarget = GL_TEXTURE_2D;
#ifndef TARGET_OPENGLES
	if (ofGetUsingArbTex() && GL_ARB_texture_rectangle){
		textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	};
#endif
	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, maxFilter);
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

//----------------------------------------------------------
ofTexture::ofTexture(){
	resetAnchor();
	quad.getVertices().resize(4);
	quad.getTexCoords().resize(4);
	quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
}

//----------------------------------------------------------
ofTexture::ofTexture(const ofTexture & mom){
	anchor = mom.anchor;
	bAnchorIsPct = mom.bAnchorIsPct;
	texData = mom.texData;
	quad = mom.quad;
	retain(texData.textureID);
}

//----------------------------------------------------------
ofTexture& ofTexture::operator=(const ofTexture & mom){
	if(!texData.bUseExternalTextureID){
		release(texData.textureID);
	}
	anchor = mom.anchor;
	bAnchorIsPct = mom.bAnchorIsPct;
	texData = mom.texData;
	quad = mom.quad;
	retain(texData.textureID);
	return *this;
}

//----------------------------------------------------------
bool ofTexture::bAllocated(){
	return texData.bAllocated;
}

//----------------------------------------------------------
bool ofTexture::isAllocated(){
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


void ofTexture::enableTextureTarget(){
	if(ofGetGLRenderer()) ofGetGLRenderer()->enableTextureTarget(texData.textureTarget);
}

void ofTexture::disableTextureTarget(){
	if(ofGetGLRenderer()) ofGetGLRenderer()->disableTextureTarget(texData.textureTarget);
}

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int internalGlDataType){
	allocate(w, h, internalGlDataType, ofGetUsingArbTex(), ofGetGLFormatFromInternal(internalGlDataType), ofGetGlTypeFromInternal(internalGlDataType));
}

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int internalGlDataType, bool bUseARBExtention){
	allocate(w, h, internalGlDataType, bUseARBExtention, ofGetGLFormatFromInternal(internalGlDataType), ofGetGlTypeFromInternal(internalGlDataType));
}

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int internalGlDataType, int glFormat, int pixelType){
	allocate(w, h, internalGlDataType, ofGetUsingArbTex(), glFormat, pixelType);
}

//----------------------------------------------------------
void ofTexture::allocate(const ofPixels& pix){
	allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), ofGetUsingArbTex(), ofGetGlFormat(pix), ofGetGlType(pix));
}

//----------------------------------------------------------
void ofTexture::allocate(const ofPixels& pix, bool bUseARBExtention){
	allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), bUseARBExtention, ofGetGlFormat(pix), ofGetGlType(pix));
}

//----------------------------------------------------------
void ofTexture::allocate(const ofShortPixels& pix){
	allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), ofGetUsingArbTex(), ofGetGlFormat(pix), ofGetGlType(pix));
}

//----------------------------------------------------------
void ofTexture::allocate(const ofShortPixels& pix, bool bUseARBExtention){
	allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), bUseARBExtention, ofGetGlFormat(pix), ofGetGlType(pix));
}


//----------------------------------------------------------
void ofTexture::allocate(const ofFloatPixels& pix){
	allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), ofGetUsingArbTex(), ofGetGlFormat(pix), ofGetGlType(pix));
}

//----------------------------------------------------------
void ofTexture::allocate(const ofFloatPixels& pix, bool bUseARBExtention){
	allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix), bUseARBExtention, ofGetGlFormat(pix), ofGetGlType(pix));
}

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int internalGlDataType, bool bUseARBExtention, int glFormat, int pixelType){
	texData.width = w;
	texData.height = h;
	texData.bFlipTexture = false;
	texData.glTypeInternal = internalGlDataType;
	//our graphics card might not support arb so we have to see if it is supported.
#ifndef TARGET_OPENGLES
	if (bUseARBExtention && GL_ARB_texture_rectangle){
		texData.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	} else 
#endif
	{
		texData.textureTarget = GL_TEXTURE_2D;
	}
	
	allocate(texData,glFormat,pixelType);
}

void ofTexture::allocate(const ofTextureData & textureData){
	allocate(textureData,ofGetGLFormatFromInternal(textureData.glTypeInternal),ofGetGlTypeFromInternal(textureData.glTypeInternal));
}

void ofTexture::allocate(const ofTextureData & textureData, int glFormat, int pixelType){
	if( textureData.width <= 0.0 || textureData.height <= 0.0 ){
		ofLogError("ofTexture") << "allocate(): ofTextureData has 0 width and/or height: " << textureData.width << "x" << textureData.height;
		return;
	}

	texData = textureData;
	//our graphics card might not support arb so we have to see if it is supported.
#ifndef TARGET_OPENGLES
	if( texData.textureTarget==GL_TEXTURE_RECTANGLE_ARB && ofGLSupportsNPOTTextures() ){
		texData.tex_w = texData.width;
		texData.tex_h = texData.height;
		texData.tex_w = texData.width;
		texData.tex_h = texData.height;		
	}else
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

#ifndef TARGET_OPENGLES
		if( texData.textureTarget==GL_TEXTURE_RECTANGLE_ARB ) texData.textureTarget = GL_TEXTURE_2D;
#endif
	}

	// attempt to free the previous bound texture, if we can:
	clear();

	glGenTextures(1, (GLuint *)&texData.textureID);   // could be more then one, but for now, just one
	retain(texData.textureID);

	enableTextureTarget();

	glBindTexture(texData.textureTarget, (GLuint)texData.textureID);
	glTexImage2D(texData.textureTarget, 0, texData.glTypeInternal, (GLint)texData.tex_w, (GLint)texData.tex_h, 0, glFormat, pixelType, 0);  // init to black...

	glTexParameterf(texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (!ofIsGLProgrammableRenderer()){
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	disableTextureTarget();



	texData.bAllocated = true;

}


void ofTexture::setRGToRGBASwizzles(bool rToRGBSwizzles){
#ifndef TARGET_OPENGLES
	enableTextureTarget();

	glBindTexture(texData.textureTarget, (GLuint)texData.textureID);
	if(rToRGBSwizzles){
		if(texData.glTypeInternal==GL_R8 ||
				texData.glTypeInternal==GL_R16 ||
				texData.glTypeInternal==GL_R32F){
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_R, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_G, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_B, GL_RED);

		}else if(texData.glTypeInternal==GL_RG8 ||
				texData.glTypeInternal==GL_RG16 ||
				texData.glTypeInternal==GL_RG32F){
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_R, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_G, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_B, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
		}
	}else{
		if(texData.glTypeInternal==GL_R8 ||
				texData.glTypeInternal==GL_R16 ||
				texData.glTypeInternal==GL_R32F){
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_R, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_B, GL_BLUE);

		}else if(texData.glTypeInternal==GL_RG8 ||
				texData.glTypeInternal==GL_RG16 ||
				texData.glTypeInternal==GL_RG32F){
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_R, GL_RED);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
			 glTexParameteri(texData.textureTarget, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
		}
	}

	glBindTexture( texData.textureTarget, 0);
	disableTextureTarget();
#endif
}

//----------------------------------------------------------
void ofTexture::loadData(const unsigned char * data, int w, int h, int glFormat){
	ofSetPixelStorei(w,1,ofGetNumChannelsFromGLFormat(glFormat));
	loadData(data, w, h, glFormat, GL_UNSIGNED_BYTE);
}

//----------------------------------------------------------
void ofTexture::loadData(const unsigned short * data, int w, int h, int glFormat){
	ofSetPixelStorei(w,2,ofGetNumChannelsFromGLFormat(glFormat));
	loadData(data, w, h, glFormat, GL_UNSIGNED_SHORT);
}

//----------------------------------------------------------
void ofTexture::loadData(const float * data, int w, int h, int glFormat){
	ofSetPixelStorei(w,4,ofGetNumChannelsFromGLFormat(glFormat));
	loadData(data, w, h, glFormat, GL_FLOAT);
}

//----------------------------------------------------------
void ofTexture::loadData(const ofPixels & pix){
	ofSetPixelStorei(pix.getWidth(),pix.getBytesPerChannel(),pix.getNumChannels());
	loadData(pix.getPixels(), pix.getWidth(), pix.getHeight(), ofGetGlFormat(pix), ofGetGlType(pix));
}

//----------------------------------------------------------
void ofTexture::loadData(const ofShortPixels & pix){
	ofSetPixelStorei(pix.getWidth(),pix.getBytesPerChannel(),pix.getNumChannels());
	loadData(pix.getPixels(), pix.getWidth(), pix.getHeight(), ofGetGlFormat(pix), ofGetGlType(pix));
}

//----------------------------------------------------------
void ofTexture::loadData(const ofFloatPixels & pix){
	ofSetPixelStorei(pix.getWidth(),pix.getBytesPerChannel(),pix.getNumChannels());
	loadData(pix.getPixels(), pix.getWidth(), pix.getHeight(), ofGetGlFormat(pix), ofGetGlType(pix));
}

//----------------------------------------------------------
void ofTexture::loadData(const ofPixels & pix, int glFormat){
	ofSetPixelStorei(pix.getWidth(),pix.getBytesPerChannel(),ofGetNumChannelsFromGLFormat(glFormat));
	loadData(pix.getPixels(), pix.getWidth(), pix.getHeight(), glFormat, ofGetGlType(pix));
}

//----------------------------------------------------------
void ofTexture::loadData(const ofShortPixels & pix, int glFormat){
	ofSetPixelStorei(pix.getWidth(),pix.getBytesPerChannel(),ofGetNumChannelsFromGLFormat(glFormat));
	loadData(pix.getPixels(), pix.getWidth(), pix.getHeight(), glFormat, ofGetGlType(pix));
}

//----------------------------------------------------------
void ofTexture::loadData(const ofFloatPixels & pix, int glFormat){
	ofSetPixelStorei(pix.getWidth(),pix.getBytesPerChannel(),ofGetNumChannelsFromGLFormat(glFormat));
	loadData(pix.getPixels(), pix.getWidth(), pix.getHeight(), glFormat, ofGetGlType(pix));
}


//----------------------------------------------------------
void ofTexture::loadData(const void * data, int w, int h, int glFormat, int glType){

	if(w > texData.tex_w || h > texData.tex_h) {
		allocate(w, h, glFormat, glFormat, glType);
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
	
	
	// 	ok this is an ultra annoying bug :
	// 	opengl texels and linear filtering -
	// 	when we have a sub-image, and we scale it
	// 	we can clamp the border pixels to the border,
	//  but the borders of the sub image get mixed with
	//  neighboring pixels...
	//  grr...
	//
	//  the best solution would be to pad out the image
	// 	being uploaded with 2 pixels on all sides, and
	//  recompute tex_t coordinates..
	//  another option is a gl_arb non pow 2 textures...
	//  the current hack is to alter the tex_t, tex_u calcs, but
	//  that makes the image slightly off...
	//  this is currently being done in draw...
	//
	// 	we need a good solution for this..
	//
	//  http://www.opengl.org/discussion_boards/ubb/ultimatebb.php?ubb=get_topic;f=3;t=014770#000001
	//  http://www.opengl.org/discussion_boards/ubb/ultimatebb.php?ubb=get_topic;f=3;t=014770#000001
	
	
	//Sosolimited: texture compression
	if (texData.compressionType == OF_COMPRESS_NONE) {
		//STANDARD openFrameworks: no compression
		
		//update the texture image: 
		enableTextureTarget();

		glBindTexture(texData.textureTarget, (GLuint) texData.textureID);
		//glTexImage2D(texData.textureTarget, 0, texData.glTypeInternal, (GLint)w, (GLint)h, 0, glFormat, glType, data);
		glTexSubImage2D(texData.textureTarget, 0, 0, 0, w, h, glFormat, glType, data);

 		disableTextureTarget();
	} else {
		//SOSOLIMITED: setup mipmaps and use compression
		//TODO: activate at least mimaps for OPENGL_ES
		//need proper tex_u and tex_t positions, with mipmaps they are the nearest power of 2
#ifndef TARGET_OPENGLES		
		if (texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
			
			//need to find closest powers of two
			int last_h = ofNextPow2(texData.height)>>1;
			int next_h = ofNextPow2(texData.height);
			if ((texData.height - last_h) < (next_h - texData.height)) texData.tex_u = last_h;
			else texData.tex_u = next_h;
			
			int last_w = ofNextPow2(texData.width)>>1;
			int next_w = ofNextPow2(texData.width);
			if ((texData.width - last_w) < (next_w - texData.width)) texData.tex_t = last_w;
			else texData.tex_t = next_w;
		}
#endif
		enableTextureTarget();
		glBindTexture(texData.textureTarget, (GLuint)texData.textureID);
		
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		if(!ofGetGLProgrammableRenderer()){
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
#ifndef TARGET_OPENGLES		
		glTexParameteri(texData.textureTarget, GL_GENERATE_MIPMAP_SGIS, true);
#endif
		glTexParameteri( texData.textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri( texData.textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri( texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri( texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
		
		
#ifndef TARGET_OPENGLES		
		//using sRGB compression
		if (texData.compressionType == OF_COMPRESS_SRGB)
		{
			if(texData.glTypeInternal == GL_RGBA)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_SRGB_ALPHA, w, h, glFormat, glType, data);
			
			else if(texData.glTypeInternal == GL_RGB)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_SRGB_ALPHA, w, h, glFormat, glType, data);
			
			else if(texData.glTypeInternal == GL_LUMINANCE_ALPHA)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_SRGB_ALPHA, w, h, glFormat, glType, data);
			
			else if(texData.glTypeInternal == GL_LUMINANCE)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_SRGB_ALPHA, w, h, glFormat, glType, data);
		}
		
		//using ARB compression: default
		else
		{
			if(texData.glTypeInternal == GL_RGBA)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_RGBA_ARB, w, h, glFormat, glType, data);
			
			else if(texData.glTypeInternal == GL_RGB)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_RGB_ARB, w, h, glFormat, glType, data);
			
			else if(texData.glTypeInternal == GL_LUMINANCE_ALPHA)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_LUMINANCE_ALPHA_ARB, w, h, glFormat, glType, data);
			
			else if(texData.glTypeInternal == GL_LUMINANCE)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_LUMINANCE_ARB, w, h, glFormat, glType, data);
		}
#endif
		

		disableTextureTarget();
		
	}
	
}


//----------------------------------------------------------
void ofTexture::loadScreenData(int x, int y, int w, int h){
	
	int screenHeight = ofGetViewportHeight(); // this call fails if we are in a different viewport or FBO: ofGetHeight();
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
	//texData.glType  = GL_RGB; // this was probably a bug, because you might be resetting the glType to something other than what the texture was created for
	
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
	
	
	enableTextureTarget();

	glBindTexture(texData.textureTarget, (GLuint)texData.textureID);
	glCopyTexSubImage2D(texData.textureTarget, 0,0,0,x,y,w,h);

	disableTextureTarget();
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
void ofTexture::bind(){
	//we could check if it has been allocated - but we don't do that in draw() 
	enableTextureTarget();
	glBindTexture( texData.textureTarget, (GLuint)texData.textureID);
	
	if(ofGetUsingNormalizedTexCoords()) {
		ofSetMatrixMode(OF_MATRIX_TEXTURE);
		ofPushMatrix();
		ofMatrix4x4 m;
		
#ifndef TARGET_OPENGLES	
		if(texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB)
			m.makeScaleMatrix(texData.width, texData.height, 1.0f);
		else 
#endif			
			m.makeScaleMatrix(texData.width / texData.tex_w, texData.height / texData.tex_h, 1.0f);
		
		ofLoadMatrix(m);
		ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	}
	if(texData.useTextureMatrix){
		ofSetMatrixMode(OF_MATRIX_TEXTURE);
		if(!ofGetUsingNormalizedTexCoords()) ofPushMatrix();
		ofMultMatrix(texData.textureMatrix);
		ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	}
}

//----------------------------------------------------------
void ofTexture::unbind(){

	glBindTexture( texData.textureTarget, 0);
	disableTextureTarget();

	if(texData.useTextureMatrix || ofGetUsingNormalizedTexCoords()) {
		ofSetMatrixMode(OF_MATRIX_TEXTURE);
		ofPopMatrix();
		ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	}
}


//----------------------------------------------------------
ofPoint ofTexture::getCoordFromPoint(float xPos, float yPos){
	
	ofPoint temp;
	
	if (!bAllocated()) return temp;
	
#ifndef TARGET_OPENGLES	
	if (texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
		
		temp.set(xPos, yPos);
		
	} else {
#endif		
		// non arb textures are 0 to 1, so we 
		// (a) convert to a pct: 
		
		float pctx = xPos / texData.width;
		float pcty = yPos / texData.height;
		
		// (b) mult by our internal pct (since we might not be 0-1 insternally)
		
		pctx *= texData.tex_t;
		pcty *= texData.tex_u;
		
		temp.set(pctx, pcty);
		
#ifndef TARGET_OPENGLES	
	}
#endif		
	
	return temp;
	
}

//----------------------------------------------------------
ofPoint ofTexture::getCoordFromPercent(float xPct, float yPct){
	
	ofPoint temp;
	
	if (!bAllocated()) return temp;
	
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
	bind();
	glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_S, wrapModeHorizontal);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_T, wrapModeVertical);
	unbind();
}

//----------------------------------------------------------
void ofTexture::setTextureMinMagFilter(GLint minFilter, GLint maxFilter){
	bind();
	glTexParameteri(texData.textureTarget, GL_TEXTURE_MAG_FILTER, maxFilter);
	glTexParameteri(texData.textureTarget, GL_TEXTURE_MIN_FILTER, minFilter);
	unbind();
}

//----------------------------------------------------------
void ofTexture::setCompression(ofTexCompression compression){
	texData.compressionType = compression;
}

//------------------------------------
void ofTexture::draw(float x, float y){
	draw(x,y,0,getWidth(),getHeight());
}

//------------------------------------
void ofTexture::draw(float x, float y, float z){
	draw(x,y,z,getWidth(),getHeight());
}

//------------------------------------
void ofTexture::draw(float x, float y, float w, float h){
	draw(x,y,0,w,h);
}

//------------------------------------
void ofTexture::draw(float x, float y, float z, float w, float h){
	drawSubsection(x,y,z,w,h,0,0,getWidth(),getHeight());
}

//------------------------------------
void ofTexture::drawSubsection(float x, float y, float w, float h, float sx, float sy){
	drawSubsection(x,y,0,w,h,sx,sy,w,h);
}

//------------------------------------
void ofTexture::drawSubsection(float x, float y, float w, float h, float sx, float sy, float _sw, float _sh){
	drawSubsection(x,y,0,w,h,sx,sy,_sw,_sh);
}

//------------------------------------
void ofTexture::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy){
	drawSubsection(x,y,z,w,h,sx,sy,w,h);
}

//----------------------------------------------------------
void ofTexture::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) {
	
	
	GLfloat px0 = x;		// up to you to get the aspect ratio right
	GLfloat py0 = y;
	GLfloat px1 = w+x;
	GLfloat py1 = h+y;
	
	if (texData.bFlipTexture == ofIsVFlipped()){
		swap(py0,py1);
	}

	// for rect mode center, let's do this:
	if (ofGetRectMode() == OF_RECTMODE_CENTER){
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
	
	/*if(z>0 || z<0){
		ofPushMatrix();

		ofTranslate(0,0,z);
	}*/
	quad.getVertices()[0].set(px0,py0,z);
	quad.getVertices()[1].set(px1,py0,z);
	quad.getVertices()[2].set(px1,py1,z);
	quad.getVertices()[3].set(px0,py1,z);
	
	quad.getTexCoords()[0].set(tx0,ty0);
	quad.getTexCoords()[1].set(tx1,ty0);
	quad.getTexCoords()[2].set(tx1,ty1);
	quad.getTexCoords()[3].set(tx0,ty1);

	// make sure we are on unit 0 - we may change this when setting shader samplers
	// before glEnable or else the shader gets confused
	/// ps: maybe if bUsingArbTex is enabled we should use glActiveTextureARB?
	glActiveTexture(GL_TEXTURE0);
	
	bind();
	quad.draw();
	unbind();

	/*if(z>0 || z<0){
		ofPopMatrix();
	}*/
	
}


// ROGER
//----------------------------------------------------------
void ofTexture::draw(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3, const ofPoint & p4){
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

	quad.getVertices()[0].set(p1.x, p1.y);
	quad.getVertices()[1].set(p2.x, p2.y);
	quad.getVertices()[2].set(p3.x, p3.y);
	quad.getVertices()[3].set(p4.x, p4.y);
	
	quad.getTexCoords()[0].set(tx0,ty0);
	quad.getTexCoords()[1].set(tx1,ty0);
	quad.getTexCoords()[2].set(tx1,ty1);
	quad.getTexCoords()[3].set(tx0,ty1);
	
	// make sure we are on unit 0 - we may change this when setting shader samplers
	// before glEnable or else the shader gets confused
	/// ps: maybe if bUsingArbTex is enabled we should use glActiveTextureARB?
	glActiveTexture(GL_TEXTURE0);
	
	bind();
	quad.draw();
	unbind();
}

//----------------------------------------------------------
void ofTexture::readToPixels(ofPixels & pixels){
#ifndef TARGET_OPENGLES
	pixels.allocate(texData.width,texData.height,ofGetImageTypeFromGLType(texData.glTypeInternal));
	bind();
	glGetTexImage(texData.textureTarget,0,ofGetGlFormat(pixels),GL_UNSIGNED_BYTE, pixels.getPixels());
	unbind();
#endif
}

//----------------------------------------------------------
void ofTexture::readToPixels(ofShortPixels & pixels){
#ifndef TARGET_OPENGLES
	pixels.allocate(texData.width,texData.height,ofGetImageTypeFromGLType(texData.glTypeInternal));
	bind();
	glGetTexImage(texData.textureTarget,0,ofGetGlFormat(pixels),GL_UNSIGNED_SHORT,pixels.getPixels());
	unbind();
#endif
}

//----------------------------------------------------------
void ofTexture::readToPixels(ofFloatPixels & pixels){
#ifndef TARGET_OPENGLES
	pixels.allocate(texData.width,texData.height,ofGetImageTypeFromGLType(texData.glTypeInternal));
	bind();
	glGetTexImage(texData.textureTarget,0,ofGetGlFormat(pixels),GL_FLOAT,pixels.getPixels());
	unbind();
#endif
}

//----------------------------------------------------------
float ofTexture::getHeight(){
	return texData.height;
}

//----------------------------------------------------------
float ofTexture::getWidth(){
	return texData.width;
}
