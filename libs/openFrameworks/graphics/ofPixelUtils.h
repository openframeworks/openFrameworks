

#pragma once
#include "ofPixels.h"

//---------------------------------------
enum ofInterpolationMethod {
	OF_INTERPOLATE_NEAREST_NEIGHBOR =1,
	OF_INTERPOLATE_BILINEAR			=2,
	OF_INTERPOLATE_BICUBIC			=3
};


//---------------------------------------

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

	static bool pasteInto(ofPixels &src, ofPixels &dst, int x, int y);

	// crop to a new width and height, this reallocates memory.
	static void crop(ofFloatPixels &pix, int x, int y, int width, int height){//TODO

	}
	// not in place
	static void cropFromTo(ofFloatPixels &frompix, ofFloatPixels &topix, int x, int y, int width, int height){//TODO

	}

	// crop to a new width and height, this reallocates memory.
	static void rotate90(ofFloatPixels &pix, int nClockwiseRotations){//TODO

	}

	static void mirror(ofFloatPixels &pix, bool vertically, bool horizontal){//TODO

	}

	static bool resize(ofFloatPixels &pix, int dstWidth, int dstHeight, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR){//TODO
		return false;
	}

	static bool resize (ofFloatPixels &pix, ofFloatPixels& dst, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR){//TODO
		return false;
	}

	// crop to a new width and height, this reallocates memory.
	static void crop(ofShortPixels &pix, int x, int y, int width, int height){//TODO

	}
	// not in place
	static void cropFromTo(ofShortPixels &frompix, ofShortPixels &topix, int x, int y, int width, int height){//TODO

	}

	// crop to a new width and height, this reallocates memory.
	static void rotate90(ofShortPixels &pix, int nClockwiseRotations){//TODO

	}

	static void mirror(ofShortPixels &pix, bool vertically, bool horizontal){//TODO

	}

	static bool resize(ofShortPixels &pix, int dstWidth, int dstHeight, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR){//TODO
		return false;
	}

	static bool resize (ofShortPixels &pix, ofShortPixels& dst, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR){//TODO
		return false;
	}
protected:

	static float bicubicInterpolate (const int *patch, float x, float y, float x2,float y2, float x3,float y3);

};
