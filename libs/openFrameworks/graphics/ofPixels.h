#pragma once

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofColor.h"
#include "ofMath.h"
#include <limits>

template <typename PixelType>
class ofPixels_ {

	friend class ofPixelUtils;

public:

	ofPixels_();
	~ofPixels_();
	ofPixels_(const ofPixels_<PixelType> & mom);
	ofPixels_<PixelType>& operator=(const ofPixels_<PixelType> & mom);

	template<typename SrcType>
	ofPixels_(const ofPixels_<SrcType> & mom);

	template<typename SrcType>
	ofPixels_<PixelType>& operator=(const ofPixels_<SrcType> & mom);

	void allocate(int w, int h, int channels);
	void allocate(int w, int h, ofPixelFormat type);
	void allocate(int w, int h, ofImageType type);

	void set(PixelType val);
	void set(int channel,PixelType val);
	void setFromPixels(const PixelType * newPixels,int w, int h, int channels);
	void setFromPixels(const PixelType * newPixels,int w, int h, ofImageType type);
	void setFromExternalPixels(PixelType * newPixels,int w, int h, int channels);
	void setFromAlignedPixels(const PixelType * newPixels, int width, int height, int channels, int stride);

	void swapRgb();

	void clear();
	
	PixelType * getPixels();
	const PixelType * getPixels() const;

	int getPixelIndex(int x, int y) const;
	ofColor getColor(int x, int y) const;
	void setColor(int x, int y, ofColor color);

	const PixelType& operator[](int pos) const;
	PixelType& operator[](int pos);

	bool isAllocated() const;

	int getWidth() const;
	int getHeight() const;

	int getBytesPerPixel() const;
	int getBitsPerPixel() const;
	int getBytesPerChannel() const;
	int getBitsPerChannel() const;
	int getNumChannels() const;

	ofPixels_<PixelType> getChannel(int channel) const;
	void setChannel(int channel, const ofPixels_<PixelType> channelPixels);

	ofImageType getImageType() const;

	int size() const;

private:

	void copyFrom( const ofPixels_<PixelType>& mom );

	template<typename SrcType>
	void copyFrom( const ofPixels_<SrcType>& mom );
	
	PixelType * pixels;
	int width;
	int height;

	int channels; // 1, 3, 4 channels per pixel (grayscale, rgb, rgba)
	//GLint	glDataType;			// GL_LUMINANCE, GL_RGB, GL_RGBA
	//ofImageType imageType;		// OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA
	bool	bAllocated;
	bool	pixelsOwner;			// if set from external data don't delete it

};


typedef ofPixels_<unsigned char> ofPixels;
typedef ofPixels_<float> ofFloatPixels;
typedef ofPixels_<unsigned short> ofShortPixels;


typedef ofPixels& ofPixelsRef;


// sorry for these ones, being templated functions inside a template i needed to do it in the .h
// they allow to do things like:
//
// ofPixels pix;
// ofFloatPixels pixf;
// pix = pixf;

template<typename PixelType>
template<typename SrcType>
ofPixels_<PixelType>::ofPixels_(const ofPixels_<SrcType> & mom){
	bAllocated = false;
	pixelsOwner = false;
	channels = 0;
	pixels = NULL;
	copyFrom( mom );
}

template<typename PixelType>
template<typename SrcType>
ofPixels_<PixelType>& ofPixels_<PixelType>::operator=(const ofPixels_<SrcType> & mom){
	copyFrom( mom );
	return *this;
}

template<typename PixelType>
template<typename SrcType>
void ofPixels_<PixelType>::copyFrom(const ofPixels_<SrcType> & mom){
	if(mom.isAllocated()){
		allocate(mom.getWidth(),mom.getHeight(),mom.getNumChannels());

		float srcMax = ( (sizeof(SrcType) == sizeof(float) ) ? 1.f : numeric_limits<SrcType>::max() );
		float dstMax = ( (sizeof(PixelType) == sizeof(float) ) ? 1.f : numeric_limits<PixelType>::max() );
		float factor = dstMax / srcMax;

		if(sizeof(SrcType) == sizeof(float)) {
			// coming from float we need a special case to clamp the values
			for(int i = 0; i < mom.size(); i++){
				pixels[i] = ofClamp(mom[i], 0, 1) * factor;
			}
		} else{
			// everything else is a straight scaling
			for(int i = 0; i < mom.size(); i++){
				pixels[i] = mom[i] * factor;
			}
		}
	}
}

