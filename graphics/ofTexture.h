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

		void allocate(int w, int h, int internalGlDataType, bool bUseARBExtention = true);
		void clear();
		void loadData(unsigned char * data, int w, int h, int glDataType);
		void loadScreenData(int x, int y, int w, int h);
		void draw(float x, float y, float w, float h);
		void draw(float x, float y);

		bool bAllocated();

		ofTextureData getTextureData();

		float 			getHeight();
		float 			getWidth();

	protected:
		ofTextureData   texData;

};

#endif
