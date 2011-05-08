#include "ofPixels.h"

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


/*template<typename T>
ofImageType ofPixels_<T>::getImageType() const{
	return imageType;
}

template<typename T>
int ofPixels_<T>::getGlDataType() const{
	return glDataType;
}*/

int ofPixels::getGlDataType() const{
	switch(getNumChannels()){
	case 1:
		return GL_LUMINANCE;
	case 3:
		return GL_RGB;
	case 4:
		return GL_RGBA;
	default:
		return GL_LUMINANCE;
	}
}

ofImageType ofPixels::getImageType() const{
	switch(getNumChannels()){
	case 1:
		return OF_IMAGE_GRAYSCALE;
	case 3:
		return OF_IMAGE_COLOR;
	case 4:
		return OF_IMAGE_COLOR_ALPHA;
	default:
		return OF_IMAGE_UNDEFINED;
	}
}

int ofFloatPixels::getGlDataType() const{
	switch(getNumChannels()){
	case 1:
		return GL_LUMINANCE;
	case 3:
		return GL_RGB;
	case 4:
		return GL_RGBA;
	default:
		return GL_LUMINANCE;
	}
}

ofImageType ofFloatPixels::getImageType() const{
	switch(getNumChannels()){
	case 1:
		return OF_IMAGE_GRAYSCALE;
	case 3:
		return OF_IMAGE_COLOR;
	case 4:
		return OF_IMAGE_COLOR_ALPHA;
	default:
		return OF_IMAGE_UNDEFINED;
	}
}
