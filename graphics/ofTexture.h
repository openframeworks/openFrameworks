#ifndef _IMAGE_TEXTURE_H_
#define _IMAGE_TEXTURE_H_

#include "ofConstants.h"
#include "ofGraphics.h"


typedef struct{

	bool bAllocated;
	int glType;
	int glTypeInternal;
	int textureTarget;
	float tex_t;
	float tex_u;
	float tex_w;
	float tex_h;
	float width;
	float height;
	bool bFlipTexture;

	//we do this because openGL expects an array
	//but we don't want people to work with textureName[1]
	//so we make textureID point to the same location
	union{
		struct{
			unsigned int textureID;	//use me
		};		
		unsigned int textureName[1];  //don't use me
	};
		
}ofTextureData;

//enable / disable the slight offset we add to ofTexture's texture coords to compensate for bad edge artifiacts
//enabled by default
void ofEnableTextureEdgeHack();
void ofDisableTectureEdgeHack();

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
	void loadData(unsigned char * data, int w, int h, int glDataType);
	void loadScreenData(int x, int y, int w, int h);

	//the anchor is the point the image is drawn around.
	//this can be useful if you want to rotate an image around a particular point.
	void setAnchorPercent(float xPct, float yPct); //set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
	void setAnchorPoint(int x, int y); //set the anchor point in pixels
	void resetAnchor(); //resets the anchor to (0, 0)

	void draw(float x, float y, float w, float h);
	void draw(float x, float y);

	//for the advanced user who wants to draw textures in their own way
	void bind();
	void unbind();

	bool bAllocated();

	ofTextureData getTextureData();

	float getHeight();
	float getWidth();

	ofTextureData texData;
protected:
	ofPoint anchor;
	bool bAnchorIsPct;


};

#endif
