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
