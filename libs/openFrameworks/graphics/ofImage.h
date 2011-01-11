#ifndef _OF_IMAGE_H_
#define _OF_IMAGE_H_

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"		// for height()
#include "FreeImage.h"
#include "ofUtils.h"

class ofPixels {
public:

	ofPixels(){
		pixels = NULL;
		clear();
	}

	ofPixels(const ofPixels & mom){
		if(mom.isAllocated()){
			allocate(mom.getWidth(),mom.getHeight(),mom.getImageType());
			memcpy(pixels,mom.getPixels(),mom.getWidth()*mom.getHeight()*mom.getBytesPerPixel());
		}
	}

	void allocate(int w, int h, int _ofImageType){
		if(bAllocated){
			if(w==width && h==height && _ofImageType==ofImageType)
				return;
			else
				clear();
		}
		ofImageType = _ofImageType;
		width= w;
		height = h;
		switch(ofImageType){
		case OF_IMAGE_GRAYSCALE:
			bytesPerPixel = 1;
			glDataType = GL_LUMINANCE;
			break;
		case OF_IMAGE_COLOR:
			bytesPerPixel = 3;
			glDataType = GL_RGB;
			break;
		case OF_IMAGE_COLOR_ALPHA:
			bytesPerPixel = 4;
			glDataType = GL_RGBA;
			break;
		}
		bitsPerPixel = bytesPerPixel * 8;
		pixels = new unsigned char[w*h*bytesPerPixel];
		bAllocated = true;

	}

	void setFromPixels(unsigned char * newPixels,int w, int h, int newType){
		allocate(w,h,newType);
		memcpy(pixels,newPixels,w*h*bytesPerPixel);
	}

	void swapRgb(){
		if (bitsPerPixel != 8){
			int sizePixels		= width*height;
			int cnt				= 0;
			unsigned char * pixels_ptr = pixels;

			while (cnt < sizePixels){
				std::swap(pixels_ptr[0],pixels_ptr[2]);
				cnt++;
				pixels_ptr+=3;
			}
		}
	}

	void clear(){
		if(pixels) delete[] pixels;

		bytesPerPixel = 0;
		bitsPerPixel = 0;
		bAllocated = false;
		glDataType = GL_LUMINANCE;
		ofImageType = OF_IMAGE_UNDEFINED;
	}

	unsigned char * getPixels(){
		return pixels;
	}

	unsigned char * const getPixels() const{
		return pixels;
	}

	bool isAllocated() const{
		return bAllocated;
	}

	int getWidth() const{
		return width;
	}

	int getHeight() const{
		return height;
	}

	int getBytesPerPixel() const{
		return bytesPerPixel;
	}

	int getBitsPerPixel() const{
		return bitsPerPixel;
	}

	int getImageType() const{
		return ofImageType;
	}

	int getGlDataType() const{
		return glDataType;
	}

private:
	unsigned char * pixels;
	int width;
	int height;

	int		bitsPerPixel;		// 8 = gray, 24 = rgb, 32 = rgba
	int		bytesPerPixel;		// 1, 3, 4 bytes per pixels
	GLint	glDataType;			// GL_LUMINANCE, GL_RGB, GL_RGBA
	int		ofImageType;		// OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA
	bool	bAllocated;

};


//----------------------------------------------------
// freeImage based stuff:
void 	ofCloseFreeImage();		// when we exit, we shut down ofImage


//----------------------------------------------------
class ofImage : public ofBaseImage{

	public :

		ofImage();
		virtual ~ofImage();

		// alloation / deallocation routines
		void 				allocate(int w, int h, int type);
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
		void 				saveImage(string fileName);

		//Sosolimited: texture compression and mipmaps
		void				setCompression(ofTexCompression compression);

		// getting the data
		unsigned char * 	getPixels();			// up to you to get this right
		ofPixels		 	getOFPixels();
		ofPixels		 	getOFPixels() const;

		// alter the image
		void 				setFromPixels(unsigned char * pixels, int w, int h, int newType, bool bOrderIsRGB = true);
		void 				setImageType(int type);
		void 				resize(int newWidth, int newHeight);
		void 				grabScreen(int x, int y, int w, int h);		// grab pixels from opengl, using glreadpixels

		// if you've altered the pixels (from getPixels()) call update() to see a change:
		void				update();


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

	protected:

		// freeImage related functionality:

		bool				loadImageIntoPixels(string fileName, ofPixels &pix);
		void				saveImageFromPixels(string fileName, ofPixels &pix);
		void				changeTypeOfPixels(ofPixels &pix, int newType);
		void				resizePixels(ofPixels &pix, int newWidth, int newHeight);
		FIBITMAP *			getBmpFromPixels(ofPixels &pix);
		void				putBmpIntoPixels(FIBITMAP * bmp, ofPixels &pix);

		ofPixels			myPixels;
		bool				bUseTexture;
		ofTexture			tex;



};



#endif
