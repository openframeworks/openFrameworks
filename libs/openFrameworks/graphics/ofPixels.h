#pragma once

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofColor.h"

class ofPixels {
public:

	ofPixels(){
		bAllocated = false;
		pixels = NULL;
		clear();
	}

	ofPixels(const ofPixels & mom){
		bAllocated = false;
		pixels = NULL;
		if(mom.isAllocated()){
			allocate(mom.getWidth(),mom.getHeight(),mom.getImageType());
			memcpy(pixels,mom.getPixels(),mom.getWidth()*mom.getHeight()*mom.getBytesPerPixel());
		}
	}

	void operator=(const ofPixels & mom){
		if(mom.isAllocated()){
			allocate(mom.getWidth(),mom.getHeight(),mom.getImageType());
			memcpy(pixels,mom.getPixels(),mom.getWidth()*mom.getHeight()*mom.getBytesPerPixel());
		}
	}

	void allocate(int w, int h, int bytesPerPixel){
		ofImageType type;
		switch(bytesPerPixel){
		case 8:
			type=OF_IMAGE_GRAYSCALE;
			break;
		case 24:
			type=OF_IMAGE_COLOR;
			break;
		case 32:
			type=OF_IMAGE_COLOR_ALPHA;
			break;
		default:
			ofLog(OF_LOG_ERROR,"%i bytes per pixel is not a supported image type",bytesPerPixel);
			return;
		}
		allocate(w,h,type);
	}

	void allocate(int w, int h, ofImageType type){
		if(bAllocated){
			if(w==width && h==height && type==imageType)
				return;
			else
				clear();
		}
		imageType = type;
		width= w;
		height = h;
		switch(imageType){
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

	void set(unsigned char val){
		memset(pixels,val,width*height*bytesPerPixel);
	}

	void setFromPixels(unsigned char * newPixels,int w, int h, ofImageType newType){
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
				pixels_ptr+=bytesPerPixel;
			}
		}
	}

	void clear(){
		if(pixels){
			delete[] pixels;
			pixels = NULL;
		}
		width			= 0;
		height			= 0;
		bytesPerPixel	= 0;
		bitsPerPixel	= 0;
		bAllocated		= false;
		glDataType		= GL_LUMINANCE;
		imageType		= OF_IMAGE_UNDEFINED;
	}

	unsigned char * getPixels(){
		return pixels;
	}

	unsigned char * const getPixels() const{
		return pixels;
	}

	int getPixelIndex(int x, int y){
		if( !bAllocated ){
			return 0;
		}else{
			return ( x + y * width ) * bytesPerPixel;
		}
	}

	ofColor getPixel(int x, int y){
		ofColor c;
		int index = getPixelIndex(x, y);
		
		if( bytesPerPixel == 1 ){
			c.set( pixels[index] );
		}else if( bytesPerPixel == 3 ){
			c.set( pixels[index], pixels[index+1], pixels[index+2] );
		}else if( bytesPerPixel == 4 ){
			c.set( pixels[index], pixels[index+1], pixels[index+2], pixels[index+3] );
		}
		
		return c;
	}

	unsigned char operator[](int pos){
		return pixels[pos];
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

	ofImageType getImageType() const{
		return imageType;
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
	ofImageType imageType;		// OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA
	bool	bAllocated;

};
