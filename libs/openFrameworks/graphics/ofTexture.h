#pragma once

#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofBaseTypes.h"
#include "Poco/SharedPtr.h"

//Sosolimited: texture compression
enum ofTexCompression
{
	OF_COMPRESS_NONE,
	OF_COMPRESS_SRGB,
	OF_COMPRESS_ARB
};

class ofTextureData{
public:
	ofTextureData(){
		isCopy = false;

		bAllocated		= false;
		textureID		= 0;
		bFlipTexture	= false;
		textureTarget	= GL_TEXTURE_2D;
		glTypeInternal  = 0;
		glType			= 0;
		pixelType		= GL_UNSIGNED_BYTE;
		width			= 0;
		height			= 0;
		tex_w			= 0;
		tex_h			= 0;
		tex_t			= 0;
		tex_u			= 0;
		compressionType = OF_COMPRESS_NONE;
	}

	~ofTextureData(){
		if(!isCopy) clear();
	}

	ofTextureData(const ofTextureData & mom){

		isCopy = true;

		bAllocated		= mom.bAllocated;
		textureID		= mom.textureID;
		bFlipTexture	= mom.bFlipTexture;
		textureTarget	= mom.textureTarget;
		glTypeInternal  = mom.glTypeInternal;
		glType			= mom.glType;
		pixelType		= mom.pixelType;
		width			= mom.width;
		height			= mom.height;
		tex_w			= mom.tex_w;
		tex_h			= mom.tex_h;
		tex_t			= mom.tex_t;
		tex_u			= mom.tex_u;
		compressionType = mom.compressionType;
	}


	int glType;
	int glTypeInternal;
	int textureTarget;
	int pixelType;  // MEMO: added this (GL_FLOAT, GL_UNSIGNED_BYTE etc.
	float tex_t;
	float tex_u;
	float tex_w;
	float tex_h;
	float width;
	float height;
	bool bFlipTexture;
	unsigned int textureID;
	bool bAllocated;
	ofTexCompression compressionType;


	void clear(){
		// try to free up the texture memory so we don't reallocate
		// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/deletetextures.html
		if (textureID != 0){
			glDeleteTextures(1, (GLuint *)&textureID);
			textureID  = 0;
		}
		bAllocated = false;
	}

private:

	bool isCopy;
};

//enable / disable the slight offset we add to ofTexture's texture coords to compensate for bad edge artifiacts
//enabled by default
void ofEnableTextureEdgeHack();
void ofDisableTextureEdgeHack();

class ofTexture : public ofBaseDraws{

	Poco::SharedPtr<ofTextureData> texDataPtr;

	public :

	ofTexture();
	virtual ~ofTexture();
	ofTexture(const ofTexture & mom);
	void operator=(const ofTexture & mom);

	// -----------------------------------------------------------------------

	virtual void allocate(int w, int h, int internalGlDataType); //uses the currently set OF texture type - default ARB texture
	virtual void allocate(int w, int h, int internalGlDataType, bool bUseARBExtention); //lets you overide the default OF texture type
	void clear();

	void loadData(float * data, int w, int h, int glDataType);
	void loadData(unsigned char * data, int w, int h, int glDataType);
	void loadData(ofPixels & pix);		
	
	void loadScreenData(int x, int y, int w, int h);

	//the anchor is the point the image is drawn around.
	//this can be useful if you want to rotate an image around a particular point.
	void setAnchorPercent(float xPct, float yPct); //set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
	void setAnchorPoint(float x, float y); //set the anchor point in pixels
	void resetAnchor(); //resets the anchor to (0, 0)

	void draw(const ofRectangle & r);
	void draw(const ofPoint & p, float w, float h);
	void draw(float x, float y, float w, float h);
	void draw(float x, float y, float z, float w, float h);
	void draw(const ofPoint & p);
	void draw(float x, float y);
	void draw(float x, float y, float z);
	void draw(ofPoint p1, ofPoint p2, ofPoint p3, ofPoint p4);

	//for the advanced user who wants to draw textures in their own way
	void bind();
	void unbind();
	
	// these are helpers to allow you to get points for the texture ala "glTexCoordf" 
	// but are texture type independent. 
	// use them for immediate or non immediate mode
	ofPoint getCoordFromPoint(float xPos, float yPos);		
	ofPoint getCoordFromPercent(float xPts, float yPts);		
	
	void setTextureWrap(GLint wrapModeHorizontal, GLint wrapModeVertical);
	void setTextureMinMagFilter(GLint minFilter, GLint maxFilter);

	void setCompression(ofTexCompression compression);

	bool bAllocated();

	ofTextureData getTextureData();

	// reference to the actual textureData inside the smart pointer
	// for backwards compatibility
	ofTextureData & texData;

	float getHeight();
	float getWidth();

	friend ostream& operator<<(ostream& os, const ofTexture& texture);
	
protected:
	void loadData(void * data, int w, int h, int glDataType);

	ofPoint anchor;
	bool bAnchorIsPct;
};

/*
// We need to add const methods to retrieve the width (?)
ostream& operator<<(ostream& os, const ofTexture& texture) {
	os << "widht: " << texture.getWidth();
	return os;
}
 */

