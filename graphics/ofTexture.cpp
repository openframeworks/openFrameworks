#include "ofTexture.h"
#include "ofUtils.h"		// for nextPow2()
#include "ofAppRunner.h"	// for getWidth()

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
		//OFLOG message
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
void ofTexture::allocate(int w, int h, int internalGlDataType, bool bUseARBExtention){

	//our graphics card might not support arb so we have to see if it is supported.
	if (bUseARBExtention && GLEE_ARB_texture_rectangle){
		texData.tex_w = w;
		texData.tex_h = h;
		texData.tex_t = w;
		texData.tex_u = h;
		texData.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	} else {
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
		glTexImage2D(texData.textureTarget, 0, texData.glTypeInternal, texData.tex_w, texData.tex_h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);  // init to black...
		glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glDisable(texData.textureTarget);

	texData.width = w;
	texData.height = h;
	texData.bFlipTexture = false;
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
		ofLog(OF_ERROR,"image data too big for allocated texture. not uploading...");
		return;
	}

	//update our size with the new dimensions - this should be the same size or smaller than the allocated texture size
	texData.width 	= w;
	texData.height 	= h;
	texData.glType  = glDataType;

	//compute new tex co-ords based on the ratio of data's w, h to texture w,h;
	if (texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
		texData.tex_t = w;
		texData.tex_u = h;
	} else {
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

	texData.bFlipTexture = false;
}


//----------------------------------------------------------
void ofTexture::loadScreenData(int x, int y, int w, int h){

	int screenHeight = ofGetHeight();
	y = screenHeight - y;
	y -= h; // top, bottom issues
	texData.bFlipTexture = true;

	if ( w > texData.tex_w || h > texData.tex_h) {
		ofLog(OF_ERROR,"image data too big for allocated texture. not uploading...");
		return;
	}

	//update our size with the new dimensions - this should be the same size or smaller than the allocated texture size
	texData.width 	= w;
	texData.height 	= h;
	texData.glType  = GL_RGB;

	//compute new tex co-ords based on the ratio of data's w, h to texture w,h;

	if (texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
		texData.tex_t = (float)(w);
		texData.tex_u = (float)(h);
	} else {
		texData.tex_t = (float)(w) / (float)texData.tex_w;
		texData.tex_u = (float)(h) / (float)texData.tex_h;
	}


	glEnable(texData.textureTarget);
	glBindTexture(texData.textureTarget, (GLuint)texData.textureName[0]);
	glCopyTexSubImage2D(texData.textureTarget, 0,0,0,x,y,w,h);
	glDisable(texData.textureTarget);
}


//----------------------------------------------------------
void ofTexture::draw(float x, float y, float w, float h){

	glEnable(texData.textureTarget);

	// bind the texture
	glBindTexture( texData.textureTarget, (GLuint)texData.textureName[0] );

		GLint px0 = 0;		// up to you to get the aspect ratio right
		GLint py0 = 0;
		GLint px1 = (GLint)w;
		GLint py1 = (GLint)h;

		if (texData.bFlipTexture == true){
			GLint temp = py0;
			py0 = py1;
			py1 = temp;
		}

		// for rect mode center, let's do this:
		if (ofGetRectMode() == OF_RECTMODE_CENTER){
			px0 = (GLint)-w/2;
			py0 = (GLint)-h/2;
			px1 = (GLint)+w/2;
			py1 = (GLint)+h/2;
		}

		// -------------------------------------------------
		// complete hack to remove border artifacts.
		// slightly, slightly alters an image, scaling...
		// to remove the border.
		// we need a better solution for this, but
		// to constantly add a 2 pixel border on all uploaded images
		// is insane..

		GLfloat offsetw = 0;
		GLfloat offseth = 0;

		if (texData.textureTarget == GL_TEXTURE_2D){
			offsetw = 1.0f/(texData.tex_w);
			offseth = 1.0f/(texData.tex_h);
		}
		// -------------------------------------------------

		GLfloat tx0 = 0+offsetw;
		GLfloat ty0 = 0+offseth;
		GLfloat tx1 = texData.tex_t - offsetw;
		GLfloat ty1 = texData.tex_u - offseth;

		glPushMatrix();
		glTranslated(x, y, 0);
		glBegin( GL_QUADS );
			glTexCoord2f(tx0,ty0);			glVertex3i(px0, py0,0);
			glTexCoord2f(tx1,ty0);			glVertex3i(px1, py0,0);
			glTexCoord2f(tx1,ty1);			glVertex3i(px1, py1,0);
			glTexCoord2f(tx0,ty1);			glVertex3i(px0, py1,0);
		glEnd();
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
