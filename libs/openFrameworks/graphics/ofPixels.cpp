#include "ofPixels.h"

ofPixels::ofPixels(){
	bAllocated = false;
	pixels = NULL;
	pixelsOwner = false;
	clear();
}

ofPixels::~ofPixels(){
	clear();
}

ofPixels::ofPixels(const ofPixels & mom){
	copyFrom( mom );
}

ofPixels& ofPixels::operator=(const ofPixels & mom){
	if(&mom==this) return * this;
	copyFrom( mom );
	return *this;
}

void ofPixels::copyFrom(const ofPixels & mom){
	if(mom.isAllocated()){
		allocate(mom.getWidth(),mom.getHeight(),mom.getImageType());
		memcpy(pixels,mom.getPixels(),mom.getWidth()*mom.getHeight()*mom.getBytesPerPixel());
	}
}


static ofImageType getImageTypeFromBits(int bitsPerPixel){
	switch(bitsPerPixel){
	case 8:
		return OF_IMAGE_GRAYSCALE;
		break;
	case 24:
		 return OF_IMAGE_COLOR;
		break;
	case 32:
		 return OF_IMAGE_COLOR_ALPHA;
		break;
	default:
		ofLog(OF_LOG_ERROR,"%i bits per pixel is not a supported image type", bitsPerPixel);
		return OF_IMAGE_UNDEFINED;
	}
}

void ofPixels::allocate(int w, int h, int bitsPerPixel){
	ofImageType type = getImageTypeFromBits(bitsPerPixel);
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
	default:
		ofLog(OF_LOG_ERROR,"ofPixels: format not supported");
		break;
	}

	bitsPerPixel = bytesPerPixel * 8;
	pixels = new unsigned char[w*h*bytesPerPixel];
	memset(pixels, 0, w*h*bytesPerPixel);
	bAllocated = true;
	pixelsOwner = true;

}

void ofPixels::allocate(int w, int h, ofPixelFormat type){

	if (w < 0 || h < 0) return;

	ofImageType imgType;
	switch(type){
	case OF_PIXELS_RGB:
		imgType = OF_IMAGE_COLOR;
		break;
	case OF_PIXELS_RGBA:
	case OF_PIXELS_BGRA:
		imgType = OF_IMAGE_COLOR_ALPHA;
		break;
	case OF_PIXELS_MONO:
		imgType = OF_IMAGE_GRAYSCALE;
		break;
	default:
		ofLog(OF_LOG_ERROR,"ofPixels: format not supported");
		break;

	}
	allocate(w,h,imgType);

}

void ofPixels::set(unsigned char val){
	memset(pixels,val,width*height*bytesPerPixel);
}

void ofPixels::setFromPixels(unsigned char * newPixels,int w, int h, int bitsPerPixel){
	ofImageType type = getImageTypeFromBits(bitsPerPixel);
	setFromPixels(newPixels,w,h,type);
}

void ofPixels::setFromPixels(unsigned char * newPixels,int w, int h, ofImageType newType){
	allocate(w,h,newType);
	memcpy(pixels,newPixels,w*h*bytesPerPixel);
}

void ofPixels::setFromExternalPixels(unsigned char * newPixels,int w, int h, int bitsPerPixel){
	ofImageType type = getImageTypeFromBits(bitsPerPixel);
	setFromExternalPixels(newPixels,w,h,type);
}

void ofPixels::setFromExternalPixels(unsigned char * newPixels,int w, int h, ofImageType newType){
	clear();
	imageType = newType;
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
	default:
		ofLog(OF_LOG_ERROR, "format not supported");
		break;
	}

	bitsPerPixel = bytesPerPixel * 8;
	pixels = newPixels;
	pixelsOwner = false;
	bAllocated = true;
}

void ofPixels::setFromAlignedPixels(unsigned char * newPixels,int w, int h, int bitsPerPixel, int widthStep){
	ofImageType type = getImageTypeFromBits(bitsPerPixel);
	setFromAlignedPixels(newPixels,w,h,type,widthStep);
}

void ofPixels::setFromAlignedPixels(unsigned char * newPixels,int w, int h, ofImageType newType, int widthStep){
	allocate(w,h,newType);
	if(widthStep==width*bytesPerPixel){
		memcpy(pixels,newPixels,w*h*bytesPerPixel);
	}else{
		for( int i = 0; i < height; i++ ) {
			memcpy( pixels + (i*width*bytesPerPixel),
					newPixels + (i*widthStep),
					width*bytesPerPixel );
		}
	}

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
		if(pixelsOwner) delete[] pixels;
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

int ofPixels::getPixelIndex(int x, int y) const {
	if( !bAllocated ){
		return 0;
	}else{
		return ( x + y * width ) * bytesPerPixel;
	}
}

ofColor ofPixels::getColor(int x, int y) const {
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

void ofPixels::setColor(int x, int y, ofColor color) {
	int index = getPixelIndex(x, y);

	if( bytesPerPixel == 1 ){
		pixels[index] = color.getBrightness();
	}else if( bytesPerPixel == 3 ){
		pixels[index] = color.r;
		pixels[index+1] = color.g;
		pixels[index+2] = color.b;
	}else if( bytesPerPixel == 4 ){
		pixels[index] = color.r;
		pixels[index+1] = color.g;
		pixels[index+2] = color.b;
		pixels[index+3] = color.a;
	}
}

unsigned char & ofPixels::operator[](int pos){
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

