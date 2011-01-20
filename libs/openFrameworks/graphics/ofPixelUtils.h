

#pragma once


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

};