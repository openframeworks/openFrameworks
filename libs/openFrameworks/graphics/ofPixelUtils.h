

#pragma once

//---------------------------------------
enum ofInterpolationMethod {
	OF_INTERPOLATE_NEAREST_NEIGHBOR =1,
	OF_INTERPOLATE_BILINEAR			=2,
	OF_INTERPOLATE_BICUBIC			=3
};


//---------------------------------------

class ofPixels;

class ofPixelUtils{


public:

	// crop to a new width and height, this reallocates memory.
	static void crop(ofPixels &pix, int x, int y, int width, int height);
	// not in place
	static void cropFromTo(ofPixels &frompix, ofPixels &topix, int x, int y, int width, int height);

	// crop to a new width and height, this reallocates memory.
	static void rotate90(ofPixels &pix, int nClockwiseRotations);

	static void mirror(ofPixels &pix, bool vertically, bool horizontal);

	static bool resize(ofPixels &pix, int dstWidth, int dstHeight, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR);
	static bool resize (ofPixels &pix, ofPixels& dst, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR);

protected:

	static float bicubicInterpolate (const int *patch, float x, float y, float x2,float y2, float x3,float y3);

};
