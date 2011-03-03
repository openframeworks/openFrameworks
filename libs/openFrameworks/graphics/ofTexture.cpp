#include "ofTexture.h"
#include "ofUtils.h"		// for nextPow2()
#include "ofAppRunner.h"	// for getWidth()
#include "ofGraphics.h"
#include "ofPixels.h"

static bool bTexHackEnabled = true;

//---------------------------------
void ofEnableTextureEdgeHack(){
	bTexHackEnabled = true;
}

//---------------------------------
void ofDisableTextureEdgeHack(){
	bTexHackEnabled = false;
}

//----------------------------------------------------------
ofTexture::ofTexture()
:texDataPtr(new ofTextureData)
{
	resetAnchor();
}

//----------------------------------------------------------
bool ofTexture::bAllocated(){
	return texDataPtr->bAllocated;
}


//----------------------------------------------------------
ofTextureData ofTexture::getTextureData(){
	if(!texDataPtr->bAllocated){
		ofLog(OF_LOG_ERROR, "getTextureData() - texture has not been allocated");
	}

	return *texDataPtr;
}

//----------------------------------------------------------
ofTexture::~ofTexture(){

}

//----------------------------------------------------------
void ofTexture::clear(){
	texDataPtr->clear();
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
			texDataPtr->tex_w = w;
			texDataPtr->tex_h = h;
			texDataPtr->tex_t = w;
			texDataPtr->tex_u = h;
			texDataPtr->textureTarget = GL_TEXTURE_RECTANGLE_ARB;
		} else 
	#endif
	{
		//otherwise we need to calculate the next power of 2 for the requested dimensions
		//ie (320x240) becomes (512x256)
		texDataPtr->tex_w = ofNextPow2(w);
		texDataPtr->tex_h = ofNextPow2(h);
		texDataPtr->tex_t = 1.0f;
		texDataPtr->tex_u = 1.0f;
		texDataPtr->textureTarget = GL_TEXTURE_2D;
	}

#ifdef TARGET_ANDROID
	texDataPtr->glTypeInternal = internalGlDataType==GL_RGB565_OES?GL_RGB:internalGlDataType;
#else
	texDataPtr->glTypeInternal = internalGlDataType;
#endif
	
	// MEMO: todo, add more types
	switch(internalGlDataType) {
#ifndef TARGET_OPENGLES	
		case GL_RGBA32F_ARB:
		case GL_RGBA16F_ARB:
			texDataPtr->glType		= GL_RGBA;
			texDataPtr->pixelType	= GL_FLOAT;
			break;
			
		case GL_RGB32F_ARB:
			texDataPtr->glType		= GL_RGB;
			texDataPtr->pixelType	= GL_FLOAT;
			break;

		case GL_LUMINANCE32F_ARB:
			texDataPtr->glType		= GL_LUMINANCE;
			texDataPtr->pixelType	= GL_FLOAT;
			break;
#elif defined(TARGET_ANDROID)
		case GL_RGB565_OES:
			texDataPtr->glType		= GL_RGB;
			texDataPtr->pixelType	= GL_UNSIGNED_SHORT_5_6_5;
			break;
#endif
		default:
			texDataPtr->glType		= GL_LUMINANCE;
			texDataPtr->pixelType	= GL_UNSIGNED_BYTE;
	}

	// attempt to free the previous bound texture, if we can:
	clear();

	glGenTextures(1, (GLuint *)&texDataPtr->textureID);   // could be more then one, but for now, just one

	glEnable(texDataPtr->textureTarget);

		glBindTexture(texDataPtr->textureTarget, (GLuint)texDataPtr->textureID);
	#ifndef TARGET_OPENGLES
		// can't do this on OpenGL ES: on full-blown OpenGL, 
		// internalGlDataType and glDataType (GL_LUMINANCE below)
		// can be different; on ES they must be exactly the same.
//		glTexImage2D(texDataPtr->textureTarget, 0, texDataPtr->glTypeInternal, (GLint)texDataPtr->tex_w, (GLint)texDataPtr->tex_h, 0, GL_LUMINANCE, PIXEL_TYPE, 0);  // init to black...
		glTexImage2D(texDataPtr->textureTarget, 0, texDataPtr->glTypeInternal, (GLint)texDataPtr->tex_w, (GLint)texDataPtr->tex_h, 0, texDataPtr->glType, texDataPtr->pixelType, 0);  // init to black...
	#else
		glTexImage2D(texDataPtr->textureTarget, 0, texDataPtr->glTypeInternal, texDataPtr->tex_w, texDataPtr->tex_h, 0, texDataPtr->glTypeInternal, texDataPtr->pixelType, 0);
	#endif

	
	glTexParameterf(texDataPtr->textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(texDataPtr->textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(texDataPtr->textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(texDataPtr->textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glDisable(texDataPtr->textureTarget);

	texDataPtr->width = w;
	texDataPtr->height = h;
	texDataPtr->bFlipTexture = false;
	texDataPtr->bAllocated = true;
}

//----------------------------------------------------------
void ofTexture::loadData(unsigned char * data, int w, int h, int glDataType){
	loadData( (void *)data, w, h, glDataType);
}

//----------------------------------------------------------
void ofTexture::loadData(float * data, int w, int h, int glDataType){
	loadData( (void *)data, w, h, glDataType);
}

//----------------------------------------------------------
void ofTexture::loadData(ofPixels & pix){
	loadData(pix.getPixels(), pix.getWidth(), pix.getHeight(), pix.getGlDataType());
}




//----------------------------------------------------------
void ofTexture::loadData(void * data, int w, int h, int glDataType){

	//SOSOLIMITED: image load step 5 - sets tex.glType to match 

	//	can we allow for uploads bigger then texture and
	//	just take as much as the texture can?
	//
	//	ie:
	//	int uploadW = MIN(w, tex_w);
	//	int uploadH = MIN(h, tex_h);
	//  but with a "step" size of w?
	// 	check "glTexSubImage2D"

	if ( w > texDataPtr->tex_w || h > texDataPtr->tex_h) {
		ofLog(OF_LOG_ERROR,"image data too big for allocated texture. %i > %f || %i > %f not uploading...",w , texDataPtr->tex_w , h , texDataPtr->tex_h);
		return;
	}

	//update our size with the new dimensions - this should be the same size or smaller than the allocated texture size
	texDataPtr->width 	= w;
	texDataPtr->height 	= h;
	texDataPtr->glType  = glDataType;

	//compute new tex co-ords based on the ratio of data's w, h to texture w,h;
	#ifndef TARGET_OPENGLES
		if (texDataPtr->textureTarget == GL_TEXTURE_RECTANGLE_ARB){
			texDataPtr->tex_t = w;
			texDataPtr->tex_u = h;
		} else 
	#endif
	{
		texDataPtr->tex_t = (float)(w) / (float)texDataPtr->tex_w;
		texDataPtr->tex_u = (float)(h) / (float)texDataPtr->tex_h;
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
	if (texDataPtr->compressionType == OF_COMPRESS_NONE)
	{
		//STANDARD openFrameworks: no compression

		//update the texture image: 
		glEnable(texDataPtr->textureTarget);
		glBindTexture(texDataPtr->textureTarget, (GLuint)texDataPtr->textureID);
 		glTexSubImage2D(texDataPtr->textureTarget, 0, 0, 0, w, h, texDataPtr->glType, texDataPtr->pixelType, data); // MEMO: important to use pixelType here
		glDisable(texDataPtr->textureTarget);
	}
	else
	{
		//SOSOLIMITED: setup mipmaps and use compression
		//TODO: activate at least mimaps for OPENGL_ES
		//need proper tex_u and tex_t positions, with mipmaps they are the nearest power of 2
#ifndef TARGET_OPENGLES		
		if (texDataPtr->textureTarget == GL_TEXTURE_RECTANGLE_ARB){
			
			//need to find closest powers of two
			int last_h = ofNextPow2(texDataPtr->height)>>1;
			int next_h = ofNextPow2(texDataPtr->height);
			if ((texDataPtr->height - last_h) < (next_h - texDataPtr->height)) texDataPtr->tex_u = last_h;
			else texDataPtr->tex_u = next_h;

			int last_w = ofNextPow2(texDataPtr->width)>>1;
			int next_w = ofNextPow2(texDataPtr->width);
			if ((texDataPtr->width - last_w) < (next_w - texDataPtr->width)) texDataPtr->tex_t = last_w;
			else texDataPtr->tex_t = next_w;

			//printf("ofTexture::loadData w:%.1f, h:%.1f, tex_t:%.1f, tex_u:%.1f \n", texDataPtr->width,texDataPtr->height,texDataPtr->tex_t,texDataPtr->tex_u);
		}
#endif
		glEnable(texDataPtr->textureTarget);
		glBindTexture(texDataPtr->textureTarget, (GLuint)texDataPtr->textureID);

		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifndef TARGET_OPENGLES		
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, true);
#endif
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		
#ifndef TARGET_OPENGLES		
		//using sRGB compression
		if (texDataPtr->compressionType == OF_COMPRESS_SRGB)
		{
			if(texDataPtr->glType == GL_RGBA)
				gluBuild2DMipmaps(texDataPtr->textureTarget, GL_COMPRESSED_SRGB_ALPHA, w, h, texDataPtr->glType, GL_UNSIGNED_BYTE, data);

			else if(texDataPtr->glType == GL_RGB)
				gluBuild2DMipmaps(texDataPtr->textureTarget, GL_COMPRESSED_SRGB_ALPHA, w, h, texDataPtr->glType, GL_UNSIGNED_BYTE, data);

			else if(texDataPtr->glType == GL_LUMINANCE_ALPHA)
				gluBuild2DMipmaps(texDataPtr->textureTarget, GL_COMPRESSED_SRGB_ALPHA, w, h, texDataPtr->glType, GL_UNSIGNED_BYTE, data);

			else if(texDataPtr->glType == GL_LUMINANCE)
				gluBuild2DMipmaps(texDataPtr->textureTarget, GL_COMPRESSED_SRGB_ALPHA, w, h, texDataPtr->glType, GL_UNSIGNED_BYTE, data);
		}

		//using ARB compression: default
		else
		{
			if(texDataPtr->glType == GL_RGBA)
				gluBuild2DMipmaps(texDataPtr->textureTarget, GL_COMPRESSED_RGBA_ARB, w, h, texDataPtr->glType, GL_UNSIGNED_BYTE, data);

			else if(texDataPtr->glType == GL_RGB)
				gluBuild2DMipmaps(texDataPtr->textureTarget, GL_COMPRESSED_RGB_ARB, w, h, texDataPtr->glType, GL_UNSIGNED_BYTE, data);

			else if(texDataPtr->glType == GL_LUMINANCE_ALPHA)
				gluBuild2DMipmaps(texDataPtr->textureTarget, GL_COMPRESSED_LUMINANCE_ALPHA_ARB, w, h, texDataPtr->glType, GL_UNSIGNED_BYTE, data);

			else if(texDataPtr->glType == GL_LUMINANCE)
				gluBuild2DMipmaps(texDataPtr->textureTarget, GL_COMPRESSED_LUMINANCE_ARB, w, h, texDataPtr->glType, GL_UNSIGNED_BYTE, data);
		}
#endif
		

		glDisable(texDataPtr->textureTarget);

	}
	//------------------------ back to normal.
	glPixelStorei(GL_UNPACK_ALIGNMENT, prevAlignment);

	texDataPtr->bFlipTexture = false;

}


//----------------------------------------------------------
void ofTexture::loadScreenData(int x, int y, int w, int h){

	int screenHeight = ofGetViewportHeight(); // this call fails if we are in a different viewport or FBO: ofGetHeight();
	y = screenHeight - y;
	y -= h; // top, bottom issues
	texDataPtr->bFlipTexture = true;

	if ( w > texDataPtr->tex_w || h > texDataPtr->tex_h) {
		ofLog(OF_LOG_ERROR,"image data too big for allocated texture. not uploading...");
		return;
	}

	//update our size with the new dimensions - this should be the same size or smaller than the allocated texture size
	texDataPtr->width 	= w;
	texDataPtr->height 	= h;
	texDataPtr->glType  = GL_RGB;

	//compute new tex co-ords based on the ratio of data's w, h to texture w,h;
	#ifndef TARGET_OPENGLES // DAMIAN
		if (texDataPtr->textureTarget == GL_TEXTURE_RECTANGLE_ARB){
			texDataPtr->tex_t = (float)(w);
			texDataPtr->tex_u = (float)(h);
		} else 
	#endif
	{
		texDataPtr->tex_t = (float)(w) / (float)texDataPtr->tex_w;
		texDataPtr->tex_u = (float)(h) / (float)texDataPtr->tex_h;
	}


	glEnable(texDataPtr->textureTarget);
	glBindTexture(texDataPtr->textureTarget, (GLuint)texDataPtr->textureID);
	glCopyTexSubImage2D(texDataPtr->textureTarget, 0,0,0,x,y,w,h);
	glDisable(texDataPtr->textureTarget);
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
	glEnable(texDataPtr->textureTarget);
	glBindTexture( texDataPtr->textureTarget, (GLuint)texDataPtr->textureID);
	
	if(ofGetUsingNormalizedTexCoords()) {
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
		
#ifndef TARGET_OPENGLES	
		if(texDataPtr->textureTarget == GL_TEXTURE_RECTANGLE_ARB)
			glScalef(texDataPtr->width, texDataPtr->height, 1.0f);
		else 
#endif			
			glScalef(texDataPtr->width / texDataPtr->tex_w, texDataPtr->height / texDataPtr->tex_h, 1.0f);
		
		glMatrixMode(GL_MODELVIEW);  		
	}
}

//----------------------------------------------------------
void ofTexture::unbind(){
	glDisable(texDataPtr->textureTarget);
	
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
	if (texDataPtr->textureTarget == GL_TEXTURE_RECTANGLE_ARB){
		
		temp.set(xPos, yPos);
		
	} else {
#endif		
		// non arb textures are 0 to 1, so we 
		// (a) convert to a pct: 
		
		float pctx = xPos / texDataPtr->width;
		float pcty = yPos / texDataPtr->height;
		
		// (b) mult by our internal pct (since we might not be 0-1 insternally)
		
		pctx *= texDataPtr->tex_t;
		pcty *= texDataPtr->tex_u;
		
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
	if (texDataPtr->textureTarget == GL_TEXTURE_RECTANGLE_ARB){
		
		temp.set(xPct * texDataPtr->width, yPct * texDataPtr->height);
		
	} else {
#endif	
		xPct *= texDataPtr->tex_t;
		yPct *= texDataPtr->tex_u;
		temp.set(xPct, yPct);
		
#ifndef TARGET_OPENGLES	
	}
#endif	
	return temp;
}


//----------------------------------------------------------
void ofTexture::setTextureWrap(GLint wrapModeHorizontal, GLint wrapModeVertical) {
	bind();
	glTexParameterf(texDataPtr->textureTarget, GL_TEXTURE_WRAP_S, wrapModeHorizontal);
	glTexParameterf(texDataPtr->textureTarget, GL_TEXTURE_WRAP_T, wrapModeVertical);
	unbind();
}

//----------------------------------------------------------
void ofTexture::setTextureMinMagFilter(GLint minFilter, GLint maxFilter){
	bind();
	glTexParameteri(texDataPtr->textureTarget, GL_TEXTURE_MAG_FILTER, maxFilter);
	glTexParameteri(texDataPtr->textureTarget, GL_TEXTURE_MIN_FILTER, minFilter);
	unbind();
}

void ofTexture::setCompression(ofTexCompression compression){
	texDataPtr->compressionType = compression;
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
	glEnable(texDataPtr->textureTarget);

	// bind the texture
	glBindTexture( texDataPtr->textureTarget, (GLuint)texDataPtr->textureID );

		GLfloat px0 = 0.0f;		// up to you to get the aspect ratio right
		GLfloat py0 = 0.0f;
		GLfloat px1 = w;
		GLfloat py1 = h;

		if (texDataPtr->bFlipTexture == true){
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

		if (texDataPtr->textureTarget == GL_TEXTURE_2D && bTexHackEnabled) {
			offsetw = 1.0f / (texDataPtr->tex_w);
			offseth = 1.0f / (texDataPtr->tex_h);
		}
		// -------------------------------------------------

		GLfloat tx0 = 0+offsetw;
		GLfloat ty0 = 0+offseth;
		GLfloat tx1 = texDataPtr->tex_t - offsetw;
		GLfloat ty1 = texDataPtr->tex_u - offseth;

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
	glDisable(texDataPtr->textureTarget);

}


// ROGER
//----------------------------------------------------------
void ofTexture::draw(ofPoint p1, ofPoint p2, ofPoint p3, ofPoint p4){
	
	// Enable alpha channel if has one
	bool blending = ofGetStyle().blendingMode;
	if (texDataPtr->glType == GL_RGBA && blending==OF_BLENDMODE_DISABLED)
		ofEnableAlphaBlending();
	
	// make sure we are on unit 0 - we may change this when setting shader samplers
	// before glEnable or else the shader gets confused
	/// ps: maybe if bUsingArbTex is enabled we should use glActiveTextureARB?
	glActiveTexture(GL_TEXTURE0);
	
	// Enable texturing
	glEnable(texDataPtr->textureTarget);
	
	// bind the texture
	glBindTexture( texDataPtr->textureTarget, (GLuint)texDataPtr->textureID );
	
	// -------------------------------------------------
	// complete hack to remove border artifacts.
	// slightly, slightly alters an image, scaling...
	// to remove the border.
	// we need a better solution for this, but
	// to constantly add a 2 pixel border on all uploaded images
	// is insane..
	
	GLfloat offsetw = 0.0f;
	GLfloat offseth = 0.0f;
	
	if (texDataPtr->textureTarget == GL_TEXTURE_2D && bTexHackEnabled) {
		offsetw = 1.0f / (texDataPtr->tex_w);
		offseth = 1.0f / (texDataPtr->tex_h);
	}
	// -------------------------------------------------
	
	GLfloat tx0 = 0+offsetw;
	GLfloat ty0 = 0+offseth;
	GLfloat tx1 = texDataPtr->tex_t - offsetw;
	GLfloat ty1 = texDataPtr->tex_u - offseth;
	
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
	glDisable(texDataPtr->textureTarget);
	
	// Disable alpha channel if it was disabled
	if (texDataPtr->glType == GL_RGBA && blending == OF_BLENDMODE_DISABLED)
		ofDisableAlphaBlending();
}


//----------------------------------------------------------
void ofTexture::draw(const ofPoint & p){
	draw(p.x, p.y, p.z, texDataPtr->width, texDataPtr->height);
}

//----------------------------------------------------------
void ofTexture::draw(float x, float y){
	draw(x, y, 0.0f, texDataPtr->width, texDataPtr->height);
}

//----------------------------------------------------------
void ofTexture::draw(float x, float y, float z){
	draw(x, y, z, texDataPtr->width, texDataPtr->height);
}

//----------------------------------------------------------
float ofTexture::getHeight(){
	return texDataPtr->height;
}

//----------------------------------------------------------
float ofTexture::getWidth(){
	return texDataPtr->width;
}
