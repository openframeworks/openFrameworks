
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
		int bytesPerPixel = pix.channels;

		int newWidth = _width;
		int newHeight = _height;
		unsigned char * newPixels = new unsigned char [newWidth*newHeight*bytesPerPixel];
		memset(newPixels, 0, newWidth*newHeight*pix.channels);

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

void ofPixelUtils::cropFromTo(ofPixels &frompix, ofPixels &topix, int x, int y, int _width, int _height){

	if (_width < 0 || _height < 0){
		return;
	}

	if (frompix.bAllocated == true){

		int width = frompix.width;
		int height = frompix.height;
		int bytesPerPixel = frompix.channels;


		if ((topix.width != _width) || (topix.height != _height) || (topix.channels != frompix.channels)){
			topix.allocate(_width, _height, frompix.channels);
		}

		int newWidth = _width;
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
	int bytesPerPixel = pix.channels;
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
	int bytesPerPixel = pix.channels;
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

//===============================================================================================================
bool ofPixelUtils::resize (ofPixels &pix, int dstWidth, int dstHeight, ofInterpolationMethod interpMethod){

	if ((dstWidth<=0) || (dstHeight<=0) || !(pix.isAllocated())) return false;

	ofPixels dstPixels;
	dstPixels.allocate(dstWidth,dstHeight,pix.getImageType());

	if(!resize(pix,dstPixels,interpMethod)) return false;

	delete [] pix.pixels;
	pix.pixels = dstPixels.getPixels();
	pix.width  = dstWidth;
	pix.height = dstHeight;
	dstPixels.pixelsOwner = false;
	return true;
}

//===============================================================================================================
bool ofPixelUtils::resize (ofPixels &pix, ofPixels& dst, ofInterpolationMethod interpMethod){

	if (!(pix.isAllocated()) || !(dst.isAllocated()) || pix.getBytesPerPixel() != dst.getBytesPerPixel()) return false;

	int srcWidth      = pix.getWidth();
	int srcHeight     = pix.getHeight();
	int dstWidth	  = dst.getWidth();
	int dstHeight	  = dst.getHeight();
	int bytesPerPixel = pix.getBytesPerPixel();


	unsigned char* dstPixels = dst.getPixels();

	switch (interpMethod){

			//----------------------------------------
		case OF_INTERPOLATE_NEAREST_NEIGHBOR:{
			int dstIndex = 0;
			float srcxFactor = (float)srcWidth/dstWidth;
			float srcyFactor = (float)srcHeight/dstHeight;
			float srcy = 0.5;
			for (int dsty=0; dsty<dstHeight; dsty++){
				float srcx = 0.5;
				int srcIndex = int(srcy)*srcWidth;
				for (int dstx=0; dstx<dstWidth; dstx++){
					int pixelIndex = int(srcIndex + srcx) * bytesPerPixel;
					for (int k=0; k<bytesPerPixel; k++){
						dstPixels[dstIndex] = pix.pixels[pixelIndex];
						dstIndex++;
						pixelIndex++;
					}
					srcx+=srcxFactor;
				}
				srcy+=srcyFactor;
			}
		}break;

			//----------------------------------------
		case OF_INTERPOLATE_BILINEAR:
			// not implemented yet

			//----------------------------------------
		case OF_INTERPOLATE_BICUBIC:
			float px1, py1;
			float px2, py2;
			float px3, py3;

			unsigned char srcColor;
			unsigned char interpCol;
			int patchRow;
			int patchIndex;
			int patch[16];

			int srcRowBytes = srcWidth*bytesPerPixel;
			int loIndex = (srcRowBytes)+1;
			int hiIndex = (srcWidth*srcHeight*bytesPerPixel)-(srcRowBytes)-1;

			for (int dsty=0; dsty<dstHeight; dsty++){
				for (int dstx=0; dstx<dstWidth; dstx++){

					int   dstIndex0 = (dsty*dstWidth + dstx) * bytesPerPixel;
					float srcxf = srcWidth  * (float)dstx/(float)dstWidth;
					float srcyf = srcHeight * (float)dsty/(float)dstHeight;
					int   srcx = (int) MIN(srcWidth-1,   srcxf);
					int   srcy = (int) MIN(srcHeight-1,  srcyf);
					int   srcIndex0 = (srcy*srcWidth + srcx) * bytesPerPixel;

					px1 = srcxf - srcx;
					py1 = srcyf - srcy;
					px2 = px1 * px1;
					px3 = px2 * px1;
					py2 = py1 * py1;
					py3 = py2 * py1;

					for (int k=0; k<bytesPerPixel; k++){
						int   dstIndex = dstIndex0+k;
						int   srcIndex = srcIndex0+k;

						for (int dy=0; dy<4; dy++) {
							patchRow = srcIndex + ((dy-1)*srcRowBytes);
							for (int dx=0; dx<4; dx++) {
								patchIndex = patchRow + (dx-1)*bytesPerPixel;
								if ((patchIndex >= loIndex) && (patchIndex < hiIndex)) {
									srcColor = pix.pixels[patchIndex];
								}
								patch[dx*4 + dy] = srcColor;
							}
						}

						interpCol = (unsigned char) bicubicInterpolate (patch, px1,py1, px2,py2, px3,py3);
						dstPixels[dstIndex] = interpCol;
					}

				}
			}
			break;
	}

	return true;
}

//=============================================
float ofPixelUtils::bicubicInterpolate (const int *patch, float x,float y, float x2,float y2, float x3,float y3) {
	// adapted from http://www.paulinternet.nl/?page=bicubic
	// Note that this code can produce values outside of 0...255, due to cubic overshoot.
	// The ofClamp() prevents this from happening.

	int p00 = patch[ 0];
	int p10 = patch[ 4];
	int p20 = patch[ 8];
	int p30 = patch[12];

	int p01 = patch[ 1];
	int p11 = patch[ 5];
	int p21 = patch[ 9];
	int p31 = patch[13];

	int p02 = patch[ 2];
	int p12 = patch[ 6];
	int p22 = patch[10];
	int p32 = patch[14];

	int p03 = patch[ 3];
	int p13 = patch[ 7];
	int p23 = patch[11];
	int p33 = patch[15];

	int a00 =    p11;
	int a01 =   -p10 +   p12;
	int a02 =  2*p10 - 2*p11 +   p12 -   p13;
	int a03 =   -p10 +   p11 -   p12 +   p13;
	int a10 =   -p01 +   p21;
	int a11 =    p00 -   p02 -   p20 +   p22;
	int a12 = -2*p00 + 2*p01 -   p02 +   p03 + 2*p20 - 2*p21 +   p22 -   p23;
	int a13 =    p00 -   p01 +   p02 -   p03 -   p20 +   p21 -   p22 +   p23;
	int a20 =  2*p01 - 2*p11 +   p21 -   p31;
	int a21 = -2*p00 + 2*p02 + 2*p10 - 2*p12 -   p20 +   p22 +   p30 -   p32;
	int a22 =  4*p00 - 4*p01 + 2*p02 - 2*p03 - 4*p10 + 4*p11 - 2*p12 + 2*p13 + 2*p20 - 2*p21 + p22 - p23 - 2*p30 + 2*p31 - p32 + p33;
	int a23 = -2*p00 + 2*p01 - 2*p02 + 2*p03 + 2*p10 - 2*p11 + 2*p12 - 2*p13 -   p20 +   p21 - p22 + p23 +   p30 -   p31 + p32 - p33;
	int a30 =   -p01 +   p11 -   p21 +   p31;
	int a31 =    p00 -   p02 -   p10 +   p12 +   p20 -   p22 -   p30 +   p32;
	int a32 = -2*p00 + 2*p01 -   p02 +   p03 + 2*p10 - 2*p11 +   p12 -   p13 - 2*p20 + 2*p21 - p22 + p23 + 2*p30 - 2*p31 + p32 - p33;
	int a33 =    p00 -   p01 +   p02 -   p03 -   p10 +   p11 -   p12 +   p13 +   p20 -   p21 + p22 - p23 -   p30 +   p31 - p32 + p33;

	float out =
    a00      + a01 * y      + a02 * y2      + a03 * y3 +
    a10 * x  + a11 * x  * y + a12 * x  * y2 + a13 * x  * y3 +
    a20 * x2 + a21 * x2 * y + a22 * x2 * y2 + a23 * x2 * y3 +
    a30 * x3 + a31 * x3 * y + a32 * x3 * y2 + a33 * x3 * y3;

	return MIN(255, MAX(out, 0));
}


bool ofPixelUtils::pasteInto(ofPixels &src, ofPixels &dst, int xTo, int yTo){
	if (!(src.isAllocated()) || !(dst.isAllocated()) || src.getBytesPerPixel() != dst.getBytesPerPixel() || xTo>=dst.getWidth() || yTo>=dst.getHeight()) return false;


	int bytesToCopyPerRow = (xTo + src.getWidth()<=dst.getWidth() ? src.getWidth() : dst.getWidth()-xTo) * src.getBytesPerPixel();
	int columnsToCopy = yTo + src.getHeight() <= dst.getHeight() ? src.getHeight() : dst.getHeight()-yTo;
	unsigned char * dstPix = dst.getPixels() + ((xTo + yTo*dst.getWidth())*src.getBytesPerPixel());
	unsigned char * srcPix = src.getPixels();
	int srcStride = src.getWidth()*src.getBytesPerPixel();
	int dstStride = dst.getWidth()*dst.getBytesPerPixel();


	for(int y=0;y<columnsToCopy; y++){
		memcpy(dstPix,srcPix,bytesToCopyPerRow);
		dstPix += dstStride;
		srcPix += srcStride;
	}

	return true;
}
