#ifndef _OF_IMAGE_H_
#define _OF_IMAGE_H_

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"		// for height()
#include "FreeImage.h"




//----------------------------------------------------
void 	ofCloseFreeImage();		// when we exit, we shut down ofImage



//----------------------------------------------------
class 	ofImage {

	public :

		ofImage();
		~ofImage();
		ofImage(const ofImage& mom);						// allocate in pass by copy
		ofImage& operator= (const ofImage& mom); 	// copy via =, ie. imga = imgb;

		void 				loadImage(string fileName);
		void 				saveImage(string fileName);

		void 				allocate(int w, int h, int type);
		void 				clear();

		unsigned char * 	getPixels();			// up to you to get this right
		void 				setFromPixels(unsigned char * pixels, int w, int h, int newType, bool bOrderIsRGB = true);

		void 				setImageType(int type);
		void 				resize(int newWidth, int newHeight);
		void 				grabScreen(int x, int y, int w, int h);		// grab pixels from opengl, using glreadpixels

		void 				clone(const ofImage &mom);

		int 				type;			// what type of image it is (as above)
		int 				width, height, bpp;		// w,h, bits per pixel

		void 				setUseTexture(bool bUse);
		void 				draw(float x, float y, float w, float h);
		void 				draw(float x, float y);

	protected:

		void 				swapChannels(bool bAlpha);
		void 				update();
		ofTexture 			tex;
		bool				bUseTexture;
		bool				bAllocatedPixels;

		// ----------  stuff one shouldn't touch at all ! ------------------

		FIBITMAP 			* bmp;

		// when OF users ask for pixels
		// we will get them contiguous memory
		// ie, non-aligned
		// see the pixel access chapter of the freeImage pdf
		// these pixel will also be RGB
		// on both platforms
		// bmp will be RGB / BGR depending
		// on the endianess of the platform.

		unsigned char 		* imgPixels;
		inline bool 		isValid();
		inline void 		swap(FIBITMAP *dst);

};



#endif
