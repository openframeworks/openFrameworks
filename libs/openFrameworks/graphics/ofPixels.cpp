/*
 * ofPixels.cpp
 *
 *  Created on: 13/01/2011
 *      Author: arturo
 */

#include "ofPixels.h"

ofPixels::ofPixels(){
	bAllocated = false;
	pixels = NULL;
	clear();
}

ofPixels::~ofPixels(){
	clear();
}

ofPixels::ofPixels(const ofPixels & mom){
	bAllocated = false;
	pixels = NULL;
	if(mom.isAllocated()){
		allocate(mom.getWidth(),mom.getHeight(),mom.getImageType());
		memcpy(pixels,mom.getPixels(),mom.getWidth()*mom.getHeight()*mom.getBytesPerPixel());
	}
}


/*ofPixels::ofPixels(ofPixels && mom){
	pixels = mom.pixels;
	width = mom.width;
	height = mom.height;

	bitsPerPixel = mom.bitsPerPixel;
	bytesPerPixel = mom.bytesPerPixel;
	glDataType = mom.glDataType;
	imageType = mom.imageType;
	bAllocated = mom.bAllocated;

	mom.pixels = NULL;
	mom.bAllocated = false;
}*/

void ofPixels::operator=(const ofPixels & mom){
	if(mom.isAllocated()){
		allocate(mom.getWidth(),mom.getHeight(),mom.getImageType());
		memcpy(pixels,mom.getPixels(),mom.getWidth()*mom.getHeight()*mom.getBytesPerPixel());
	}
}

void ofPixels::allocate(int w, int h, int bitsPerPixel){
	ofImageType type;
	switch(bitsPerPixel){
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
		ofLog(OF_LOG_ERROR,"%i bits per pixel is not a supported image type", bitsPerPixel);
		return;
	}
	allocate(w,h,type);
}

void ofPixels::allocate(int w, int h, ofImageType type){

	if (w < 0 || h < 0) return; 
	
	//we check if we are already allocated at the right size
	if(bAllocated && w==width && h==height && type==imageType){
		return; //we don't need to allocate
	}

	//we do need to allocate, clear the data
	clear();

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
	memset(pixels, 0, w*h*bytesPerPixel);
	bAllocated = true;

}

void ofPixels::set(unsigned char val){
	memset(pixels,val,width*height*bytesPerPixel);
}

void ofPixels::setFromPixels(unsigned char * newPixels,int w, int h, ofImageType newType){
	allocate(w,h,newType);
	memcpy(pixels,newPixels,w*h*bytesPerPixel);
}


void ofPixels::swapRgb(){
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


void ofPixels::clear(){
	
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

unsigned char * ofPixels::getPixels(){
	return pixels;
}

unsigned char * const ofPixels::getPixels() const{
	return pixels;
}

int ofPixels::getPixelIndex(int x, int y){
	if( !bAllocated ){
		return 0;
	}else{
		return ( x + y * width ) * bytesPerPixel;
	}
}

ofColor ofPixels::getPixel(int x, int y){
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

unsigned char ofPixels::operator[](int pos){
	return pixels[pos];
}

bool ofPixels::isAllocated() const{
	return bAllocated;
}

int ofPixels::getWidth() const{
	return width;
}

int ofPixels::getHeight() const{
	return height;
}

int ofPixels::getBytesPerPixel() const{
	return bytesPerPixel;
}

int ofPixels::getBitsPerPixel() const{
	return bitsPerPixel;
}

ofImageType ofPixels::getImageType() const{
	return imageType;
}

int ofPixels::getGlDataType() const{
	return glDataType;
}

