#pragma once

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofColor.h"
#include "ofPixelUtils.h"

class ofPixels {
public:

	ofPixels();
	~ofPixels();
	ofPixels(const ofPixels & mom) { copyFrom( mom ); }
	//ofPixels(ofPixels && mom);

	ofPixels& operator=(const ofPixels & mom) { copyFrom( mom ); return *this; }

	void allocate(int w, int h, int bitsPerPixel);
	void allocate(int w, int h, ofImageType type);
	void set(unsigned char val);
	void setFromPixels(unsigned char * newPixels,int w, int h, ofImageType newType);

	void swapRgb();

	void clear();
	
	unsigned char * getPixels();
	unsigned char * const getPixels() const;

	int getPixelIndex(int x, int y) const;
	ofColor getPixel(int x, int y) const;

	unsigned char& operator[](int pos);

	bool isAllocated() const;

	int getWidth() const;
	int getHeight() const;

	int getBytesPerPixel() const;
	int getBitsPerPixel() const;

	ofImageType getImageType() const;
	int getGlDataType() const;

private:
	void copyFrom( const ofPixels& mom );
	
	friend class ofPixelUtils;
	
	unsigned char * pixels;
	int width;
	int height;

	int		bitsPerPixel;		// 8 = gray, 24 = rgb, 32 = rgba
	int		bytesPerPixel;		// 1, 3, 4 bytes per pixels
	GLint	glDataType;			// GL_LUMINANCE, GL_RGB, GL_RGBA
	ofImageType imageType;		// OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA
	bool	bAllocated;

};
