
#include "ofPixelUtils.h"
#include "ofPixels.h"


// TODO: more optimized non in place versions of these functions. 

void ofPixelUtils::crop(ofPixels & pix, int x, int y, int _width, int _height){
	
	
	if (_width < 0 || _height < 0){
		return;
	}
	
	if (pix.bAllocated == true){
		
		int width = pix.width;
		int height = pix.height;
		int bytesPerPixel = pix.bytesPerPixel;
		
		int newWidth = _width;
		int newHeight = _height;
		unsigned char * newPixels = new unsigned char [newWidth*newHeight*bytesPerPixel];
		memset(newPixels, 0, newWidth*newHeight*pix.bytesPerPixel);
		
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

static void ofPixelUtils::cropFromTo(ofPixels &frompix, ofPixels &topix, int x, int y, int _width, int _height){
	
	if (_width < 0 || _height < 0){
		return;
	}
	
	if (frompix.bAllocated == true){
		
		int width = frompix.width;
		int height = frompix.height;
		int bytesPerPixel = frompix.bytesPerPixel;
		
		
		if ((topix.width != _width) || (topix.height != _height) || (topix.imageType != frompix.imageType)){
			topix.allocate(_width, _height, frompix.imageType);
		} 
		
		int newWidth = _width;
		int newHeight = _height;
		unsigned char * newPixels = topix.pixels;
		
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
					newPixels[newPixel*bytesPerPixel + k] = frompix.pixels[oldPixel*bytesPerPixel + k];
				}
			}
		}
		
		
	}
	
}




void ofPixelUtils::rotate90(ofPixels & pix, int nClockwiseRotations){

	
	if (pix.bAllocated == false){
		return;
	}
	
	// first, figure out which type of rotation we have
	int rotation = nClockwiseRotations;
	while (rotation < 0){
		rotation+=4;
	}
	rotation %= 4;
	
	// if it's 0, do nothing.  if it's 2, do it by a mirror operation.
	if (rotation == 0) {
		return; 
		// do nothing! 
	} else if (rotation == 2) {
		mirror(pix, true, true);
		return;
	}
	
	// otherwise, we will need to do some new allocaiton. 
	int width = pix.width;
	int height = pix.height;
	int bytesPerPixel = pix.bytesPerPixel;
	unsigned char * oldPixels = pix.pixels;
	int newWidth = height;
	int newHeight = width;
	unsigned char * newPixels = new unsigned char [newWidth*newHeight*bytesPerPixel];
	
	if(rotation == 1){
		for (int i = 0; i < width; i++){
			for (int j = 0; j < height; j++){
					
				int pixela = (j*width + i);
				int pixelb = ((i) * newWidth + (height - j - 1));
				for (int k = 0; k < bytesPerPixel; k++){
					newPixels[pixelb*bytesPerPixel + k] = oldPixels[pixela*bytesPerPixel + k];
				}
				
			}
		}
	} else if(rotation == 3){
		for (int i = 0; i < width; i++){
			for (int j = 0; j < height; j++){
				
				int pixela = (j*width + i);
				int pixelb = ((width-i-1) * newWidth + j);
				for (int k = 0; k < bytesPerPixel; k++){
					newPixels[pixelb*bytesPerPixel + k] = oldPixels[pixela*bytesPerPixel + k];
				}			
			}		
		}
	}
	
	delete [] pix.pixels;
	pix.pixels = newPixels;
	pix.width = newWidth;
	pix.height = newHeight;

}

void ofPixelUtils::mirror(ofPixels & pix, bool vertically, bool horizontal){
	
	if (!vertically && !horizontal){
		return; 
	}
	
	int width = pix.width;
	int height = pix.height;
	int bytesPerPixel = pix.bytesPerPixel;
	unsigned char * oldPixels = pix.pixels;
	unsigned char tempVal;
	
	if (! (vertically && horizontal)){
		int wToDo = horizontal ? width/2 : width;
		int hToDo = vertically ? height/2 : height;
		
		for (int i = 0; i < wToDo; i++){
			for (int j = 0; j < hToDo; j++){
			
				int pixelb = (vertically ? (height - j - 1) : j) * width + (horizontal ? (width - i - 1) : i);
				int pixela = j*width + i;
				for (int k = 0; k < bytesPerPixel; k++){
					
					tempVal = oldPixels[pixela*bytesPerPixel + k];
					oldPixels[pixela*bytesPerPixel + k] = oldPixels[pixelb*bytesPerPixel + k];
					oldPixels[pixelb*bytesPerPixel + k] = tempVal;
					
				}			
			}		
		}
	} else {
		// I couldn't think of a good way to do this in place.  I'm sure there is. 
		mirror(pix, true, false);
		mirror(pix, false, true);
	}
	
}

