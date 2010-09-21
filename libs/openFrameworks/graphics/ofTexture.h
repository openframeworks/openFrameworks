#ifndef _IMAGE_TEXTURE_H_
#define _IMAGE_TEXTURE_H_

#include "ofConstants.h"
#include "ofGraphics.h"

//Sosolimited: texture compression
enum ofTexCompression
{
	OF_COMPRESS_NONE,
	OF_COMPRESS_SRGB,
	OF_COMPRESS_ARB
};

typedef struct{

	bool bAllocated;
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

	//Sosolimited: texture compression
	ofTexCompression compressionType;
		
}ofTextureData;

//enable / disable the slight offset we add to ofTexture's texture coords to compensate for bad edge artifiacts
//enabled by default
void ofEnableTextureEdgeHack();
void ofDisableTextureEdgeHack();

class ofTexture : public ofBaseDraws{

	public :

	ofTexture();
	virtual ~ofTexture();

	// -----------------------------------------------------------------------
	// we allow pass by copy and assignment operator
	// it does a straight copy but you are getting the textureID of mom's texture
	// so this means that your texture and mom's texture are the same thing
	// so in other words be careful! calling clear on your texture will trash mom's
	// texture and vice versa.
	ofTexture(const ofTexture& mom);
	ofTexture& operator=(const ofTexture& mom);
	// -----------------------------------------------------------------------

	void allocate(int w, int h, int internalGlDataType); //uses the currently set OF texture type - default ARB texture
	void allocate(int w, int h, int internalGlDataType, bool bUseARBExtention); //lets you overide the default OF texture type
	void clear();

	void loadData(float * data, int w, int h, int glDataType);
	void loadData(unsigned char * data, int w, int h, int glDataType);

	void loadScreenData(int x, int y, int w, int h);

	//the anchor is the point the image is drawn around.
	//this can be useful if you want to rotate an image around a particular point.
	void setAnchorPercent(float xPct, float yPct); //set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
	void setAnchorPoint(float x, float y); //set the anchor point in pixels
	void resetAnchor(); //resets the anchor to (0, 0)

	void draw(float x, float y, float w, float h);
	void draw(float x, float y);

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

	float getHeight();
	float getWidth();

	ofTextureData texData;
protected:
	void loadData(void * data, int w, int h, int glDataType);

	ofPoint anchor;
	bool bAnchorIsPct;


};

#endif
