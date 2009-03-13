#include "ofTexture.h"
#include "ofUtils.h"		// for nextPow2()
#include "ofAppRunner.h"	// for getWidth()

static bool bTexHackEnabled = true;

//---------------------------------
void ofEnableTextureEdgeHack(){
	bTexHackEnabled = true;
}

//---------------------------------
void ofDisableTectureEdgeHack(){
	bTexHackEnabled = false;
}

//----------------------------------------------------------
ofTexture::ofTexture(){
	texData.bAllocated		= false;
	texData.textureName[0]	= 0;
	texData.textureID		= 0;
	texData.bFlipTexture	= false;
	texData.textureTarget	= GL_TEXTURE_2D;
	texData.glTypeInternal  = 0;
	texData.glType			= 0;
	texData.width			= 0;
	texData.height			= 0;
	texData.tex_w			= 0;
	texData.tex_h			= 0;
	texData.tex_t			= 0;
	texData.tex_u			= 0;

	resetAnchor();
}

//----------------------------------------------------------
bool ofTexture::bAllocated(){
	return texData.bAllocated;
}

//----------------------------------------------------------
ofTexture::ofTexture(const ofTexture& mom){
	texData = mom.texData;
}

//----------------------------------------------------------
ofTexture& ofTexture::operator=(const ofTexture& mom){
	texData = mom.texData;
	return *this;
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
	clear();
}

//----------------------------------------------------------
void ofTexture::clear(){
	// try to free up the texture memory so we don't reallocate
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/deletetextures.html
	if (texData.textureName[0] != 0){
		glDeleteTextures(1, (GLuint *)texData.textureName);
	}

	texData.bAllocated = false;
}

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int internalGlDataType){
	allocate(w, h, internalGlDataType, ofGetUsingArbTex());
}

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int internalGlDataType, bool bUseARBExtention){

	//our graphics card might not support arb so we have to see if it is supported.
	#ifndef TARGET_OF_IPHONE
		if (bUseARBExtention && GLEE_ARB_texture_rectangle){
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

	// attempt to free the previous bound texture, if we can:
	clear();

	glGenTextures(1, (GLuint *)texData.textureName);   // could be more then one, but for now, just one

	glEnable(texData.textureTarget);

		glBindTexture(texData.textureTarget, (GLuint)texData.textureName[0]);
	#ifndef TARGET_OF_IPHONE 
		// can't do this on OpenGL ES: on full-blown OpenGL, 
		// internalGlDataType and glDataType (GL_LUMINANCE below)
		// can be different; on ES they must be exactly the same.
		glTexImage2D(texData.textureTarget, 0, texData.glTypeInternal, (GLint)texData.tex_w, (GLint)texData.tex_h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);  // init to black...
	#else
		glTexImage2D(texData.textureTarget, 0, texData.glTypeInternal, texData.tex_w, texData.tex_h, 0, texData.glTypeInternal, GL_UNSIGNED_BYTE, 0);
	#endif

		glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glDisable(texData.textureTarget);

	texData.width = w;
	texData.height = h;
	#ifdef TARGET_OF_IPHONE
		texData.bFlipTexture = true; // textures need to be flipped for the iphone
	#else
		texData.bFlipTexture = false;
	#endif
	texData.bAllocated = true;
}

//----------------------------------------------------------
void ofTexture::loadData(unsigned char * data, int w, int h, int glDataType){

	//	can we allow for uploads bigger then texture and
	//	just take as much as the texture can?
	//
	//	ie:
	//	int uploadW = MIN(w, tex_w);
	//	int uploadH = MIN(h, tex_h);
	//  but with a "step" size of w?
	// 	check "glTexSubImage2D"

	if ( w > texData.tex_w || h > texData.tex_h) {
		ofLog(OF_LOG_ERROR,"image data too big for allocated texture. not uploading...");
		return;
	}

	//update our size with the new dimensions - this should be the same size or smaller than the allocated texture size
	texData.width 	= w;
	texData.height 	= h;
	texData.glType  = glDataType;

	//compute new tex co-ords based on the ratio of data's w, h to texture w,h;
	#ifndef TARGET_OF_IPHONE	
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

	// update the texture image:
	glEnable(texData.textureTarget);
		glBindTexture(texData.textureTarget, (GLuint)texData.textureName[0]);
 		glTexSubImage2D(texData.textureTarget,0,0,0,w,h,texData.glType,GL_UNSIGNED_BYTE,data);
	glDisable(texData.textureTarget);

	//------------------------ back to normal.
	glPixelStorei(GL_UNPACK_ALIGNMENT, prevAlignment);

	#ifdef TARGET_OF_IPHONE
		texData.bFlipTexture = true; // textures need to be flipped for the iphone
	#else
		texData.bFlipTexture = false;
	#endif

}


//----------------------------------------------------------
void ofTexture::loadScreenData(int x, int y, int w, int h){

	int screenHeight = ofGetHeight();
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
	texData.glType  = GL_RGB;

	//compute new tex co-ords based on the ratio of data's w, h to texture w,h;
	#ifndef TARGET_OF_IPHONE // DAMIAN
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
	glBindTexture(texData.textureTarget, (GLuint)texData.textureName[0]);
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
void ofTexture::setAnchorPoint(int x, int y){
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
	glBindTexture( texData.textureTarget, (GLuint)texData.textureName[0]);
}

//----------------------------------------------------------
void ofTexture::unbind(){
	glDisable(texData.textureTarget);
}

//----------------------------------------------------------
void ofTexture::draw(float x, float y, float w, float h){

	glEnable(texData.textureTarget);

	// bind the texture
	glBindTexture( texData.textureTarget, (GLuint)texData.textureName[0] );

		GLfloat px0 = 0;		// up to you to get the aspect ratio right
		GLfloat py0 = 0;
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

		GLfloat offsetw = 0;
		GLfloat offseth = 0;

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
	
		glTranslatef(x,y,0.0f);
		
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


//----------------------------------------------------------
void ofTexture::draw(float x, float y){
	draw(x,y,texData.width, texData.height);
}

//----------------------------------------------------------
float ofTexture::getHeight(){
	return texData.height;
}

//----------------------------------------------------------
float ofTexture::getWidth(){
	return texData.width;
}
