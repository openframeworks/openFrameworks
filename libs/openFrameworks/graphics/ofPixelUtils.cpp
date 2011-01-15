
#include "ofPixelUtils.h"
#include "ofPixels.h"




void ofPixelUtils::crop(ofPixels & pix, int x, int y, int _width, int _height){
	
	
	if (pix.bAllocated == true){
		
		int width = pix.width;
		int height = pix.height;
		int bytesPerPixel = pix.bytesPerPixel;
		
		unsigned char * newPixels = new unsigned char [width*height*bytesPerPixel];
		int newWidth = _width;
		int newHeight = _height;
		memset(newPixels, 0, width*height*pix.bytesPerPixel);
		
		// this prevents having to do a check for bounds in the for loop;
		int minX = MAX(x, 0);
		int maxX = MIN(x+_width, width);
		int minY = MAX(y, 0);
		int maxY = MIN(y+_height, height);
		
		// TODO: point math can help speed this up:
		for (int i = minX; i < maxX; i++){
			for (int j = minY; j < maxY; j++){
				
				int newPixel = (j-y) * newWidth + (i-x);
				int oldPixel = (j) * width + (i);
				
				for (int k = 0; k < bytesPerPixel; k++){
					newPixels[newPixel*bytesPerPixel + k] = pix.pixels[oldPixel*bytesPerPixel + k];
				}
			}
		}
		
		delete [] pix.pixels;
		pix.pixels = newPixels;
		pix.width = newWidth;
		pix.height = newHeight;
	}
		
	
}

void ofPixelUtils::rotate90(ofPixels & pix, int nClockwiseRotations){
	
	
}

void ofPixelUtils::mirror(ofPixels & pix, bool vertically, bool horizontal){
	
	
}