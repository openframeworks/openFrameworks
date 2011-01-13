#pragma once

#include "ofConstants.h"
#include "ofFileUtils.h"
#include "ofTexture.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"		// for height()
#include "FreeImage.h"
#include "ofUtils.h"
#include "ofPixels.h"

//----------------------------------------------------
// freeImage based stuff:
void	ofLoadImage(ofPixels & pix, string path);
void	ofLoadImageFromMemory(ofPixels & pix, unsigned char * bytes, int numBytes);

//TODO: add load from buffer
//void	ofLoadImage(ofPixels & pix, ofBuffer buf); 


void 	ofCloseFreeImage();		// when we exit, we shut down ofImage

//----------------------------------------------------
class ofImage : public ofBaseImage{

	public :

		ofImage();
		virtual ~ofImage();

		// alloation / deallocation routines
		void 				allocate(int w, int h, ofImageType type);
		void 				clear();

		// default copy overwriting (for = or std::vector)
		ofImage(const ofImage& mom);
		ofImage& operator= (const ofImage& mom);

		// copying:
		void 				clone(const ofImage &mom);

		// enable or disable using the texture of this image
		void 				setUseTexture(bool bUse);

		//for getting a reference to the texture
		ofTexture & getTextureReference();

		// quick texture binding shortcut
		void bind();
		void unbind();

		// file loading / saving
		bool 				loadImage(string fileName);
		bool				loadImage(const ofBuffer & buffer);
		void 				saveImage(string fileName);

		//Sosolimited: texture compression and mipmaps
		void				setCompression(ofTexCompression compression);

		// getting the data
		unsigned char * 	getPixels();			// up to you to get this right
		ofPixels		 	getOFPixels();
		ofPixels		 	getOFPixels() const;

		// alter the image
		void 				setFromPixels(unsigned char * pixels, int w, int h, ofImageType type, bool bOrderIsRGB = true);
		void 				setImageType(ofImageType type);
		void 				resize(int newWidth, int newHeight);
		void 				grabScreen(int x, int y, int w, int h);		// grab pixels from opengl, using glreadpixels

		// if you've altered the pixels (e.g., from getPixels())
		// call update() to see a change (move the pixels to the texture)
		void update();

		// if you've altered the texture (e.g., using a shader)
		// call updatePixels() to get the pixels (move the texture to the pixels)
		void updatePixels();

		//the anchor is the point the image is drawn around.
		//this can be useful if you want to rotate an image around a particular point.
        void				setAnchorPercent(float xPct, float yPct);	//set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
        void				setAnchorPoint(float x, float y);				//set the anchor point in pixels
        void				resetAnchor();								//resets the anchor to (0, 0)

		// draw:
		void 				draw(const ofRectangle & r);
		void 				draw(const ofPoint & p, float w, float h);
		void 				draw(float x, float y, float w, float h);
		void 				draw(float x, float y, float z, float w, float h);
		void 				draw(const ofPoint & p);
		void 				draw(float x, float y);
		void 				draw(float x, float y, float z);

		float 				getHeight();
		float 				getWidth();
		bool 				bAllocated() {return myPixels.isAllocated();};

		int 				width, height, bpp;		// w,h, bits per pixel
		int					type;					// OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA


		// freeImage related functionality:

		static bool			loadImageIntoPixels(string fileName, ofPixels &pix);
		static void			saveImageFromPixels(string fileName, ofPixels &pix);

		static bool			loadImageFromMemory(const ofBuffer & buffer, ofPixels &pix);

	protected:
	
		void				changeTypeOfPixels(ofPixels &pix, ofImageType type);
		void				resizePixels(ofPixels &pix, int newWidth, int newHeight);
		static FIBITMAP *	getBmpFromPixels(ofPixels &pix);
		static void			putBmpIntoPixels(FIBITMAP * bmp, ofPixels &pix);


		ofPixels			myPixels;
		bool				bUseTexture;
		ofTexture			tex;
};
