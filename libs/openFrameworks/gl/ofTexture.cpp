#include "ofTexture.h"
#include "ofUtils.h"		// for nextPow2()
#include "ofAppRunner.h"	// for getWidth()
#include "ofGraphics.h"
#include "ofPixels.h"
#include <map>

static bool bTexHackEnabled = true;

//---------------------------------
int ofGetGlInternalFormat(const ofPixels& pix) {
	switch(pix.getNumChannels()) {
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default: return GL_LUMINANCE;
	}
}

//---------------------------------
int ofGetGlInternalFormat(const ofShortPixels& pix) {
#ifndef TARGET_OPENGLES
	switch(pix.getNumChannels()) {
		case 3: return GL_RGB16;
		case 4: return GL_RGBA16;
		default: return GL_LUMINANCE16;
	}
#else
	ofLogWarning()<< "16bit textures not supported in GLES";
	switch(pix.getNumChannels()) {
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default: return GL_LUMINANCE;
	}
#endif
}

//---------------------------------
int ofGetGlInternalFormat(const ofFloatPixels& pix) {
#ifndef TARGET_OPENGLES
	switch(pix.getNumChannels()) {
		case 3: return GL_RGB32F_ARB;
		case 4: return GL_RGBA32F_ARB;
		default: return GL_LUMINANCE32F_ARB;
	}
#else
	ofLogWarning()<< "float textures not supported in GLES";
	switch(pix.getNumChannels()) {
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default: return GL_LUMINANCE;
	}
#endif
}

//---------------------------------
// this is helpful for debugging ofTexture
string ofGetGlInternalFormatName(int glInternalFormat) {
	switch(glInternalFormat) {
		case GL_RGBA: return "GL_RGBA";
#ifndef TARGET_OPENGLES
		case GL_RGBA8: return "GL_RGBA8";
#endif
		case GL_RGB: return "GL_RGB";
#ifndef TARGET_OPENGLES
		case GL_RGB8: return "GL_RGB8";
#endif
		case GL_LUMINANCE: return "GL_LUMINANCE";
#ifndef TARGET_OPENGLES
		case GL_LUMINANCE8: return "GL_LUMINANCE8";
		case GL_RGBA16: return "GL_RGBA16";
		case GL_RGB16: return "GL_RGB16";
		case GL_LUMINANCE16: return "GL_LUMINANCE16";
		case GL_RGBA32F_ARB: return "GL_RGBA32F_ARB";
		case GL_RGB32F_ARB: return "GL_RGB32F_ARB";
		case GL_LUMINANCE32F_ARB: return "GL_LUMINANCE32F_ARB";
#endif
		case GL_LUMINANCE_ALPHA: return "GL_LUMINANCE_ALPHA";
#ifndef TARGET_OPENGLES
		case GL_LUMINANCE8_ALPHA8: return "GL_LUMINANCE8_ALPHA8";
#endif
		default: return "unknown glInternalFormat";
	}
}

//---------------------------------
void ofGetGlFormatAndType(int glInternalFormat, int& glFormat, int& glType) {
	switch(glInternalFormat) {
		// common 8-bit formats: rgba, rgb, grayscale
		case GL_RGBA:
#ifndef TARGET_OPENGLES
		case GL_RGBA8:
#endif
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_BYTE;
			break;
		case GL_RGB:
#ifndef TARGET_OPENGLES
		case GL_RGB8:
#endif
			glFormat = GL_RGB;
			glType = GL_UNSIGNED_BYTE;
			break;
		case GL_LUMINANCE:
#ifndef TARGET_OPENGLES
		case GL_LUMINANCE8:
#endif
			glFormat = GL_LUMINANCE;
			glType = GL_UNSIGNED_BYTE;
			break;
			
#ifndef TARGET_OPENGLES
		// 16-bit unsigned short formats
		case GL_RGBA16:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_SHORT;
			break;
		case GL_RGB16:
			glFormat = GL_RGB;
			glType = GL_UNSIGNED_SHORT;
			break;
		case GL_LUMINANCE16:
			glFormat = GL_LUMINANCE;
			glType = GL_UNSIGNED_SHORT;
			break;
			
		// 32-bit float formats
		case GL_RGBA32F_ARB:
			glFormat = GL_RGBA;
			glType = GL_FLOAT;
			break;
		case GL_RGB32F_ARB:
			glFormat = GL_RGB;
			glType = GL_FLOAT;
			break;
		case GL_LUMINANCE32F_ARB:
			glFormat = GL_LUMINANCE;
			glType = GL_FLOAT;
			break;


		// 16-bit float formats
		case GL_RGBA16F_ARB:
			glFormat = GL_RGBA;
			glType = GL_FLOAT;
			break;
		case GL_RGB16F_ARB:
			glFormat = GL_RGB;
			glType = GL_FLOAT;
			break;
		case GL_LUMINANCE16F_ARB:
			glFormat = GL_LUMINANCE;
			glType = GL_FLOAT;
			break;
#endif

		// used by prepareBitmapTexture(), not supported by ofPixels
		case GL_LUMINANCE_ALPHA:
#ifndef TARGET_OPENGLES
		case GL_LUMINANCE8_ALPHA8:
#endif
			glFormat = GL_LUMINANCE_ALPHA;
			glType = GL_UNSIGNED_BYTE;
			break;
		
		default:
			glFormat = GL_RGB;
			glType = GL_UNSIGNED_BYTE;
			ofLogError() << "ofGetGlFormatAndType(): glInternalFormat not recognized";
			break;
	}
}

static bool ofCheckGLTypesEqual(int type1, int type2){
#ifndef TARGET_OPENGLES
	if(type1==GL_LUMINANCE || type1==GL_LUMINANCE8){
		if(type2==GL_LUMINANCE || type2==GL_LUMINANCE8){
			return true;
		}else{
			return false;
		}
	}

	else if(type1==GL_RGB || type1==GL_RGB8){
		if(type2==GL_RGB || type2==GL_RGB8){
			return true;
		}else{
			return false;
		}
	}

	else if(type1==GL_RGBA || type1==GL_RGBA8){
		if(type2==GL_RGBA || type2==GL_RGBA8){
			return true;
		}else{
			return false;
		}
	}

	else
#endif
		return type1==type2;
}

//---------------------------------
void ofEnableTextureEdgeHack(){
	bTexHackEnabled = true;
}

//---------------------------------
void ofDisableTextureEdgeHack(){
	bTexHackEnabled = false;
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
			ofLog(OF_LOG_ERROR, "trying to delete a non indexed texture, something weird is happening. Deleting anyway");
			glDeleteTextures(1, (GLuint *)&id);
		}
	}
}

//----------------------------------------------------------
ofTexture::ofTexture()
{
	resetAnchor();
}

//----------------------------------------------------------
ofTexture::ofTexture(const ofTexture & mom){
	anchor = mom.anchor;
	bAnchorIsPct = mom.bAnchorIsPct;
	texData = mom.texData;
	retain(texData.textureID);
}

//----------------------------------------------------------
ofTexture& ofTexture::operator=(const ofTexture & mom){
	release(texData.textureID);
	anchor = mom.anchor;
	bAnchorIsPct = mom.bAnchorIsPct;
	texData = mom.texData;
	retain(texData.textureID);
	return *this;
}

//----------------------------------------------------------
bool ofTexture::bAllocated(){
	return texData.bAllocated;
}


//----------------------------------------------------------
ofTextureData ofTexture::getTextureData(){
	if(!texData.bAllocated){
		ofLog(OF_LOG_ERROR, "getTextureData() - texture has not been allocated");
	}
	
	return texData;
}

//----------------------------------------------------------
ofTexture::~ofTexture(){
	release(texData.textureID);
}

//----------------------------------------------------------
void ofTexture::clear(){
	release(texData.textureID);
	texData.textureID  = 0;
	texData.bAllocated = false;
}

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int internalGlDataType){
	allocate(w, h, internalGlDataType, ofGetUsingArbTex());
}

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int internalGlDataType, bool bUseARBExtention){
	//our graphics card might not support arb so we have to see if it is supported.
#ifndef TARGET_OPENGLES
	if (bUseARBExtention && GL_ARB_texture_rectangle){
		texData.tex_w = w;
		texData.tex_h = h;
		texData.tex_t = w;
		texData.tex_u = h;
		texData.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	} else 
#endif
	{
		//otherwise we need to calculate the next power of 2 for the requested dimensions
		//ie (320x240) becomes (512x256)
		texData.tex_w = ofNextPow2(w);
		texData.tex_h = ofNextPow2(h);
		texData.tex_t = 1.0f;
		texData.tex_u = 1.0f;
		texData.textureTarget = GL_TEXTURE_2D;
	}
	
	texData.glTypeInternal = internalGlDataType;
	// get the glType (format) and pixelType (type) corresponding to the glTypeInteral (internalFormat)
	ofGetGlFormatAndType(texData.glTypeInternal, texData.glType, texData.pixelType);
	
	// attempt to free the previous bound texture, if we can:
	clear();
	
	glGenTextures(1, (GLuint *)&texData.textureID);   // could be more then one, but for now, just one
	retain(texData.textureID);
	
	glEnable(texData.textureTarget);
	
	glBindTexture(texData.textureTarget, (GLuint)texData.textureID);
#ifndef TARGET_OPENGLES
	// can't do this on OpenGL ES: on full-blown OpenGL, 
	// glInternalFormat and glFormat (GL_LUMINANCE below)
	// can be different; on ES they must be exactly the same.
	//		glTexImage2D(texData.textureTarget, 0, texData.glTypeInternal, (GLint)texData.tex_w, (GLint)texData.tex_h, 0, GL_LUMINANCE, PIXEL_TYPE, 0);  // init to black...
	glTexImage2D(texData.textureTarget, 0, texData.glTypeInternal, (GLint)texData.tex_w, (GLint)texData.tex_h, 0, texData.glType, texData.pixelType, 0);  // init to black...
#else
	glTexImage2D(texData.textureTarget, 0, texData.glTypeInternal, texData.tex_w, texData.tex_h, 0, texData.glTypeInternal, texData.pixelType, 0);
#endif
	
	
	glTexParameterf(texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glDisable(texData.textureTarget);
	
	texData.width = w;
	texData.height = h;
	texData.bFlipTexture = false;
	texData.bAllocated = true;
}

//----------------------------------------------------------
void ofTexture::loadData(unsigned char * data, int w, int h, int glInternalFormat){
	loadData( (void *)data, w, h, glInternalFormat);
}

//----------------------------------------------------------
void ofTexture::loadData(float * data, int w, int h, int glInternalFormat){
	loadData( (void *)data, w, h, glInternalFormat);
}

//----------------------------------------------------------
void ofTexture::loadData(unsigned short * data, int w, int h, int glInternalFormat){
	loadData( (void *)data, w, h, glInternalFormat);
}

//----------------------------------------------------------
void ofTexture::loadData(ofPixels & pix){
	loadData(pix.getPixels(), pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix));
}




//----------------------------------------------------------
void ofTexture::loadData(void * data, int w, int h, int glInternalFormat){
	
	//	can we allow for uploads bigger then texture and
	//	just take as much as the texture can?
	//
	//	ie:
	//	int uploadW = MIN(w, tex_w);
	//	int uploadH = MIN(h, tex_h);
	//  but with a "step" size of w?
	// 	check "glTexSubImage2D"
	
	if(!ofCheckGLTypesEqual(glInternalFormat,texData.glTypeInternal)) {
		ofLogError() << "ofTexture::loadData() failed to upload internalFormat " <<  ofGetGlInternalFormatName(glInternalFormat) << " data to " << ofGetGlInternalFormatName(texData.glTypeInternal) << " texture";
		return;
	}
	
	if(w > texData.tex_w || h > texData.tex_h) {
		ofLogError() << "ofTexture::loadData() failed to upload " <<  w << "x" << h << " data to " << texData.tex_w << "x" << texData.tex_h << " texture";
		return;
	}
	
	// update our size with the new dimensions
	texData.width = w;
	texData.height = h;
	
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
	
	//------------------------ likely, we are uploading continuous data
	GLint prevAlignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &prevAlignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	
	//Sosolimited: texture compression
	if (texData.compressionType == OF_COMPRESS_NONE) {
		//STANDARD openFrameworks: no compression
		
		//update the texture image: 
		glEnable(texData.textureTarget);
		glBindTexture(texData.textureTarget, (GLuint) texData.textureID);
 		glTexSubImage2D(texData.textureTarget, 0, 0, 0, w, h, texData.glType, texData.pixelType, data);
		glDisable(texData.textureTarget);
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
			
			//printf("ofTexture::loadData w:%.1f, h:%.1f, tex_t:%.1f, tex_u:%.1f \n", texData.width,texData.height,texData.tex_t,texData.tex_u);
		}
#endif
		glEnable(texData.textureTarget);
		glBindTexture(texData.textureTarget, (GLuint)texData.textureID);
		
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
			if(texData.glType == GL_RGBA)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_SRGB_ALPHA, w, h, texData.glType, texData.pixelType, data);
			
			else if(texData.glType == GL_RGB)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_SRGB_ALPHA, w, h, texData.glType, texData.pixelType, data);
			
			else if(texData.glType == GL_LUMINANCE_ALPHA)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_SRGB_ALPHA, w, h, texData.glType, texData.pixelType, data);
			
			else if(texData.glType == GL_LUMINANCE)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_SRGB_ALPHA, w, h, texData.glType, texData.pixelType, data);
		}
		
		//using ARB compression: default
		else
		{
			if(texData.glType == GL_RGBA)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_RGBA_ARB, w, h, texData.glType, texData.pixelType, data);
			
			else if(texData.glType == GL_RGB)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_RGB_ARB, w, h, texData.glType, texData.pixelType, data);
			
			else if(texData.glType == GL_LUMINANCE_ALPHA)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_LUMINANCE_ALPHA_ARB, w, h, texData.glType, texData.pixelType, data);
			
			else if(texData.glType == GL_LUMINANCE)
				gluBuild2DMipmaps(texData.textureTarget, GL_COMPRESSED_LUMINANCE_ARB, w, h, texData.glType, texData.pixelType, data);
		}
#endif
		
		
		glDisable(texData.textureTarget);
		
	}
	//------------------------ back to normal.
	glPixelStorei(GL_UNPACK_ALIGNMENT, prevAlignment);
	
	texData.bFlipTexture = false;
	
}


//----------------------------------------------------------
void ofTexture::loadScreenData(int x, int y, int w, int h){
	
	int screenHeight = ofGetViewportHeight(); // this call fails if we are in a different viewport or FBO: ofGetHeight();
	y = screenHeight - y;
	y -= h; // top, bottom issues
	texData.bFlipTexture = true;
	
	if ( w > texData.tex_w || h > texData.tex_h) {
		ofLog(OF_LOG_ERROR,"image data too big for allocated texture. not uploading...");
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
	
	
	glEnable(texData.textureTarget);
	glBindTexture(texData.textureTarget, (GLuint)texData.textureID);
	glCopyTexSubImage2D(texData.textureTarget, 0,0,0,x,y,w,h);
	glDisable(texData.textureTarget);
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
	anchor       = 0;
	bAnchorIsPct = false;
}

//----------------------------------------------------------
void ofTexture::bind(){
	//we could check if it has been allocated - but we don't do that in draw() 
	glEnable(texData.textureTarget);
	glBindTexture( texData.textureTarget, (GLuint)texData.textureID);
	
	if(ofGetUsingNormalizedTexCoords()) {
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
		
#ifndef TARGET_OPENGLES	
		if(texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB)
			glScalef(texData.width, texData.height, 1.0f);
		else 
#endif			
			glScalef(texData.width / texData.tex_w, texData.height / texData.tex_h, 1.0f);
		
		glMatrixMode(GL_MODELVIEW);  		
	}
}

//----------------------------------------------------------
void ofTexture::unbind(){
	glDisable(texData.textureTarget);
	
	if(ofGetUsingNormalizedTexCoords()) {
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW); 
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

void ofTexture::setCompression(ofTexCompression compression){
	texData.compressionType = compression;
}

//----------------------------------------------------------
void ofTexture::draw(const ofRectangle & r){
	draw(r.x, r.y, 0.0f, r.width, r.height);
}

//----------------------------------------------------------
void ofTexture::draw(const ofPoint & p, float w, float h){
	draw(p.x, p.y, p.z, w, h);
}

//----------------------------------------------------------
void ofTexture::draw(float x, float y, float w, float h){
	draw(x, y, 0.0f, w, h);
}

//----------------------------------------------------------
void ofTexture::draw(float x, float y, float z, float w, float h){
	
	// make sure we are on unit 0 - we may change this when setting shader samplers
	// before glEnable or else the shader gets confused
	/// ps: maybe if bUsingArbTex is enabled we should use glActiveTextureARB?
	glActiveTexture(GL_TEXTURE0);
	
	// Enable texturing
	glEnable(texData.textureTarget);
	
	// bind the texture
	glBindTexture( texData.textureTarget, (GLuint)texData.textureID );
	
	GLfloat px0 = 0.0f;		// up to you to get the aspect ratio right
	GLfloat py0 = 0.0f;
	GLfloat px1 = w;
	GLfloat py1 = h;
	
	if (texData.bFlipTexture == true){
		GLint temp = (GLint)py0;
		py0 = py1;
		py1 = temp;
	}
	
	// for rect mode center, let's do this:
	if (ofGetRectMode() == OF_RECTMODE_CENTER){
		px0 = -w/2;
		py0 = -h/2;
		px1 = +w/2;
		py1 = +h/2;
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
	
	if (texData.textureTarget == GL_TEXTURE_2D && bTexHackEnabled) {
		offsetw = 1.0f / (texData.tex_w);
		offseth = 1.0f / (texData.tex_h);
	}
	// -------------------------------------------------
	
	GLfloat tx0 = 0+offsetw;
	GLfloat ty0 = 0+offseth;
	GLfloat tx1 = texData.tex_t - offsetw;
	GLfloat ty1 = texData.tex_u - offseth;
	
	glPushMatrix(); 
	
	glTranslatef(x,y,z);
	
	GLfloat tex_coords[] = {
		tx0,ty0,
		tx1,ty0,
		tx1,ty1,
		tx0,ty1
	};
	GLfloat verts[] = {
		px0,py0,
		px1,py0,
		px1,py1,
		px0,py1
	};
	
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
	glEnableClientState(GL_VERTEX_ARRAY);		
	glVertexPointer(2, GL_FLOAT, 0, verts );
	glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	
	glPopMatrix();
	glDisable(texData.textureTarget);
	
}


// ROGER
//----------------------------------------------------------
void ofTexture::draw(ofPoint p1, ofPoint p2, ofPoint p3, ofPoint p4){
	
	// Enable alpha channel if has one
	bool blending = ofGetStyle().blendingMode;
	if (texData.glType == GL_RGBA && blending == OF_BLENDMODE_DISABLED) {
		ofEnableAlphaBlending();
	}
	
	// make sure we are on unit 0 - we may change this when setting shader samplers
	// before glEnable or else the shader gets confused
	/// ps: maybe if bUsingArbTex is enabled we should use glActiveTextureARB?
	glActiveTexture(GL_TEXTURE0);
	
	// Enable texturing
	glEnable(texData.textureTarget);
	
	// bind the texture
	glBindTexture( texData.textureTarget, (GLuint)texData.textureID );
	
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
	
	glPushMatrix(); 
	
	GLfloat tex_coords[] = {
		tx0,ty0,
		tx1,ty0,
		tx1,ty1,
		tx0,ty1
	};
	GLfloat verts[] = {
		p1.x, p1.y,
		p2.x, p2.y,
		p3.x, p3.y,
		p4.x, p4.x
	};
	
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
	glEnableClientState(GL_VERTEX_ARRAY);		
	glVertexPointer(2, GL_FLOAT, 0, verts );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	
	glPopMatrix();
	glDisable(texData.textureTarget);
	
	// Disable alpha channel if it was disabled
	if (texData.glType == GL_RGBA && blending == OF_BLENDMODE_DISABLED) {
		ofDisableAlphaBlending();
	}
}


//----------------------------------------------------------
void ofTexture::draw(const ofPoint & p){
	draw(p.x, p.y, p.z, texData.width, texData.height);
}

//----------------------------------------------------------
void ofTexture::draw(float x, float y){
	draw(x, y, 0.0f, texData.width, texData.height);
}

//----------------------------------------------------------
void ofTexture::draw(float x, float y, float z){
	draw(x, y, z, texData.width, texData.height);
}

//----------------------------------------------------------
float ofTexture::getHeight(){
	return texData.height;
}

//----------------------------------------------------------
float ofTexture::getWidth(){
	return texData.width;
}
