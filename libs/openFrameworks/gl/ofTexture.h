#pragma once

#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofBaseTypes.h"
#include "ofConstants.h"

int ofGetGlInternalFormat(const ofPixels& pix);
int ofGetGlInternalFormat(const ofShortPixels& pix);
int ofGetGlInternalFormat(const ofFloatPixels& pix);
void ofGetGlFormatAndType(int glInternalFormat, int& glFormat, int& glType);
ofImageType ofGetImageTypeFromGLType(int glType);

//set whether OF uses ARB rectangular texture or the more traditonal GL_TEXTURE_2D
bool ofGetUsingArbTex();
void ofEnableArbTex();
void ofDisableArbTex();


bool ofGetUsingNormalizedTexCoords();
void ofEnableNormalizedTexCoords();
void ofDisableNormalizedTexCoords();


//***** add global functions to override texture settings
void ofSetTextureWrap(GLfloat wrapS = GL_CLAMP_TO_EDGE, GLfloat wrapT = GL_CLAMP_TO_EDGE);
bool ofGetUsingCustomTextureWrap();
void ofRestoreTextureWrap();

void ofSetMinMagFilters(GLfloat minFilter = GL_LINEAR, GLfloat maxFilter = GL_LINEAR);
bool ofGetUsingCustomMinMagFilters();
void ofRestoreMinMagFilters();
//*****

//Sosolimited: texture compression
enum ofTexCompression
{
	OF_COMPRESS_NONE,
	OF_COMPRESS_SRGB,
	OF_COMPRESS_ARB
};

class ofTextureData {
public:
	ofTextureData() {
		textureID = 0;
#ifndef TARGET_OPENGLES
		glTypeInternal = GL_RGB8;
		textureTarget = GL_TEXTURE_RECTANGLE_ARB;
#else
		glTypeInternal = GL_RGB;
		textureTarget = GL_TEXTURE_2D;
#endif
		glType = GL_RGB;
		pixelType = GL_UNSIGNED_BYTE;
		

		tex_t = 0;
		tex_u = 0;
		tex_w = 0;
		tex_h = 0;
		width = 0;
		height = 0;
		
		bFlipTexture = false;
		compressionType = OF_COMPRESS_NONE;
		bAllocated = false;
	}

	unsigned int textureID;
	int textureTarget;
	int glTypeInternal; // internalFormat, e.g., GL_RGB8.
	int glType; // format, e.g., GL_RGB. should be named glFormat
	int pixelType;  // type, e.g., GL_UNSIGNED_BYTE. should be named glType
	
	float tex_t;
	float tex_u;
	float tex_w;
	float tex_h;
	float width, height;
	
	bool bFlipTexture;
	ofTexCompression compressionType;
	bool bAllocated;
};

//enable / disable the slight offset we add to ofTexture's texture coords to compensate for bad edge artifiacts
//enabled by default
void ofEnableTextureEdgeHack();
void ofDisableTextureEdgeHack();

class ofTexture : public ofBaseDraws {
	public :

	ofTexture();
	ofTexture(const ofTexture & mom);
	ofTexture& operator=(const ofTexture & mom);
	virtual ~ofTexture();

	// -----------------------------------------------------------------------

	virtual void allocate(const ofTextureData & textureData);
	virtual void allocate(int w, int h, int glInternalFormat); //uses the currently set OF texture type - default ARB texture
	virtual void allocate(int w, int h, int glInternalFormat, bool bUseARBExtention); //lets you overide the default OF texture type
	virtual void allocate(const ofPixels& pix);
	void clear();

	void loadData(const unsigned char* const data, int w, int h, int glFormat);
	void loadData(const unsigned short* data, int w, int h, int glFormat);
	void loadData(const float* data, int w, int h, int glFormat);
	void loadData(const ofPixels & pix);		
	void loadData(const ofShortPixels & pix);
	void loadData(const ofFloatPixels & pix);
	
	void loadScreenData(int x, int y, int w, int h);

	//the anchor is the point the image is drawn around.
	//this can be useful if you want to rotate an image around a particular point.
	void setAnchorPercent(float xPct, float yPct); //set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
	void setAnchorPoint(float x, float y); //set the anchor point in pixels
	void resetAnchor(); //resets the anchor to (0, 0)

	void draw(const ofRectangle & r);
	void draw(const ofPoint & p);
	void draw(const ofPoint & p, float w, float h);
	void draw(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3, const ofPoint & p4);
	void draw(float x, float y);
	void draw(float x, float y, float z);
	void draw(float x, float y, float w, float h);
	void draw(float x, float y, float z, float w, float h);
	
	void drawSubsection(float x, float y, float w, float h, float sx, float sy);
	void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy);
	void drawSubsection(float x, float y, float w, float h, float sx, float sy, float sw, float sh);
	void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh);

	void readToPixels(ofPixels & pixels);
	void readToPixels(ofShortPixels & pixels);
	void readToPixels(ofFloatPixels & pixels);

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
	bool isAllocated();

	ofTextureData& getTextureData();
	const ofTextureData& getTextureData() const;

	// reference to the actual textureData inside the smart pointer
	// for backwards compatibility
	ofTextureData texData;

	float getHeight();
	float getWidth();

protected:
	void loadData(void * data, int w, int h, int glFormat);

	ofPoint anchor;
	bool bAnchorIsPct;
};
