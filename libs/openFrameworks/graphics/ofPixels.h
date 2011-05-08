#pragma once

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofColor.h"

template <typename T>
class ofPixels_ {

	friend class ofPixelUtils;

public:

	ofPixels_();
	~ofPixels_();
	ofPixels_(const ofPixels_<T> & mom);
	ofPixels_<T>& operator=(const ofPixels_<T> & mom);


	void allocate(int w, int h, int channels);
	void allocate(int w, int h, ofPixelFormat type);
	void allocate(int w, int h, ofImageType type);

	void set(T val);
	void setFromPixels(const T * newPixels,int w, int h, int channels);
	void setFromPixels(const T * newPixels,int w, int h, ofImageType type);
	void setFromExternalPixels(T * newPixels,int w, int h, int channels);
	void setFromAlignedPixels(const T * newPixels,int w, int h, int channels, int widthStep);

	void swapRgb();

	void clear();
	
	T * getPixels();
	const T * getPixels() const;

	int getPixelIndex(int x, int y) const;
	ofColor getColor(int x, int y) const;
	void setColor(int x, int y, ofColor color);

	T& operator[](int pos);

	bool isAllocated() const;

	int getWidth() const;
	int getHeight() const;

	int getBytesPerPixel() const;
	int getBitsPerPixel() const;
	int getBytesPerChannel() const;
	int getBitsPerChannel() const;
	int getNumChannels() const;

	ofImageType getImageType() const;
	int getGlDataType() const;

private:
	void copyFrom( const ofPixels_<T>& mom );
	
	
	T * pixels;
	int width;
	int height;

	int		bitsPerChannel;		// 8 = gray, 24 = rgb, 32 = rgba
	int		bytesPerChannel;		// 1, 3, 4 bytes per pixels
	int		channels;
	//GLint	glDataType;			// GL_LUMINANCE, GL_RGB, GL_RGBA
	//ofImageType imageType;		// OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA
	bool	bAllocated;
	bool	pixelsOwner;			// if set from external data don't delete it

};


typedef ofPixels_<unsigned char> ofPixels;
typedef ofPixels_<float> ofFloatPixels;

typedef ofPixels& ofPixelsRef;

