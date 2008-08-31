#include "ofTexture.h"
#include "ofUtils.h"		// for nextPow2()
#include "ofAppRunner.h"	// for getWidth()

//----------------------------------------------------------
ofTexture::ofTexture(){
	textureName[0]	= 0;
	bFlipTexture	= false;
	textureTarget	= GL_TEXTURE_2D;
}

//----------------------------------------------------------
ofTexture::ofTexture(const ofTexture& mom){
	// do nothing. (since copying a texture via graphics card is not possible)
	textureName[0]	= 0;
	bFlipTexture	= false;
	textureTarget	= GL_TEXTURE_2D;
}	

//----------------------------------------------------------			
ofTexture& ofTexture::operator=(const ofTexture& mom){
	// do nothing. (since copying a texture via graphics card is not possible)
	return * this;
}

//----------------------------------------------------------
ofTexture::~ofTexture(){
	clear();
}

//----------------------------------------------------------
void ofTexture::clear(){
	// try to free up the texture memory so we don't reallocate
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/deletetextures.html
	if (textureName[0] != 0){
		glDeleteTextures(1, (GLuint *)textureName);
	}
	
	width			= 0;
	height			= 0;
	bFlipTexture		= false;
}

//----------------------------------------------------------
void ofTexture::allocate(int w, int h, int internalGlDataType, bool bUseARBExtention){

	//our graphics card might not support arb so we have to see if it is supported. 
	if (bUseARBExtention && GLEE_ARB_texture_rectangle){
		tex_w = w;
		tex_h = h;
		tex_t = w;
		tex_u = h;
		textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	} else {
		//otherwise we need to calculate the next power of 2 for the requested dimensions
		//ie (320x240) becomes (512x256)
		tex_w = ofNextPow2(w);
		tex_h = ofNextPow2(h);
		tex_t = 1.0f;
		tex_u = 1.0f;
		textureTarget = GL_TEXTURE_2D;
	}

	// attempt to free the previous bound texture, if we can:
	clear();

	glGenTextures(1, (GLuint *)textureName);   // could be more then one, but for now, just one

	glEnable(textureTarget);

		glBindTexture(textureTarget, (GLuint)textureName[0]);
		glTexImage2D(textureTarget, 0, internalGlDataType, tex_w, tex_h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);  // init to black...
		glTexParameterf(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glDisable(textureTarget);

	width = w;
	height = h;
	bFlipTexture = false;
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

	if ( w > tex_w || h > tex_h) {
		printf("image data too big for allocated texture. not uploading... \n");
		return;
	}

	//update our size with the new dimensions - this should be the same size or smaller than the allocated texture size
	width 	= w;
	height 	= h;

	//compute new tex co-ords based on the ratio of data's w, h to texture w,h;
	if (textureTarget == GL_TEXTURE_RECTANGLE_ARB){
		tex_t = w;
		tex_u = h;
	} else {
		tex_t = (float)(w) / (float)tex_w;
		tex_u = (float)(h) / (float)tex_h;
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
	glEnable(textureTarget);
		glBindTexture(textureTarget, (GLuint)textureName[0]);
 		glTexSubImage2D(textureTarget,0,0,0,w,h,glDataType,GL_UNSIGNED_BYTE,data);
	glDisable(textureTarget);

	//------------------------ back to normal.
	glPixelStorei(GL_UNPACK_ALIGNMENT, prevAlignment);

	bFlipTexture = false;
}


//----------------------------------------------------------
void ofTexture::loadScreenData(int x, int y, int w, int h){

	int screenHeight = ofGetHeight();
	y = screenHeight - y;
	y -= h; // top, bottom issues
	bFlipTexture = true;

	if ( w > tex_w || h > tex_h) {
		printf("image data too big for allocated texture. not uploading... \n");
		return;
	}

	//update our size with the new dimensions - this should be the same size or smaller than the allocated texture size
	width 	= w;
	height 	= h;

	//compute new tex co-ords based on the ratio of data's w, h to texture w,h;

	if (textureTarget == GL_TEXTURE_RECTANGLE_ARB){
		tex_t = (float)(w);
		tex_u = (float)(h);
	} else {
		tex_t = (float)(w) / (float)tex_w;
		tex_u = (float)(h) / (float)tex_h;
	}


	glEnable(textureTarget);
	glBindTexture(textureTarget, (GLuint)textureName[0]);
	glCopyTexSubImage2D(textureTarget, 0,0,0,x,y,w,h);
	glDisable(textureTarget);
}


//----------------------------------------------------------
void ofTexture::draw(float x, float y, float w, float h){

	glEnable(textureTarget);

	// bind the texture
	glBindTexture( textureTarget, (GLuint)textureName[0] );

		GLint px0 = 0;		// up to you to get the aspect ratio right
		GLint py0 = 0;
		GLint px1 = (GLint)w;
		GLint py1 = (GLint)h;

		if (bFlipTexture == true){
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

		if (textureTarget == GL_TEXTURE_2D){
			offsetw = 1.0f/(tex_w);
			offseth = 1.0f/(tex_h);
		}
		// -------------------------------------------------

		GLfloat tx0 = 0+offsetw;
		GLfloat ty0 = 0+offseth;
		GLfloat tx1 = tex_t - offsetw;
		GLfloat ty1 = tex_u - offseth;

		glPushMatrix();
		glTranslated(x, y, 0);
		glBegin( GL_QUADS );
			glTexCoord2f(tx0,ty0);			glVertex3i(px0, py0,0);
			glTexCoord2f(tx1,ty0);			glVertex3i(px1, py0,0);
			glTexCoord2f(tx1,ty1);			glVertex3i(px1, py1,0);
			glTexCoord2f(tx0,ty1);			glVertex3i(px0, py1,0);
		glEnd();
		glPopMatrix();

	glDisable(textureTarget);
}


//----------------------------------------------------------
void ofTexture::draw(float x, float y){
	draw(x,y,width, height);
}


