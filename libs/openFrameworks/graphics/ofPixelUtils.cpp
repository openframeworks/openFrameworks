
#include "ofPixelUtils.h"
#include "ofPixels.h"


// TODO: more optimized non in place versions of these functions. 

void ofPixelUtils::crop(ofPixels & pix, int x, int y, int _width, int _height){
	
	
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

void ofPixelUtils::rotate90(ofPixels & pix, int nClockwiseRotations){

		
}

void ofPixelUtils::mirror(ofPixels & pix, bool vertically, bool horizontal){
	
	
}

//
////--------------------------------------------------------------
//void setFromSubImage(ofImage &src, ofImage &dst, ofRectangle subRect) {
//	int oldw = src.width;
//	int oldh = src.height;
//	int w = subRect.width;
//	int h = subRect.height;
//	int x = (int)subRect.x;
//	int y = (int)subRect.y;
//	if (w > oldw) return;
//	if (h > oldh) return; 
//	int nPix = w * h * 3;
//	unsigned char * tempPixels = new unsigned char[ nPix ];
//	memset(tempPixels, 0, nPix);
//	//if (dst.width != subRect.width || dst.height != subRect.height){
//	//		dst.clear();	// clean up if necessary?
//	//		dst.allocate(subRect.width, subRect.height, OF_IMAGE_COLOR);
//	//	}
//	unsigned char * srcPixel = src.getPixels() + (y * oldw + x) * 3;
//	for (int i = 0; i < h; i++){
//		memcpy(tempPixels + i*w*3, srcPixel + i*oldw*3, w*3);
//		//memset(tempPixels + i*w*3, 0, w*3);
//	}
//	
//	dst.setFromPixels(tempPixels, w, h, OF_IMAGE_COLOR);
//	dst.update();
//	delete tempPixels;
//}
