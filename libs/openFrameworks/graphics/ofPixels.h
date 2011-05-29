#pragma once

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofColor.h"
#include "ofMath.h"
#include <limits>

template <typename T>
class ofPixels_ {

	friend class ofPixelUtils;

public:

	ofPixels_();
	~ofPixels_();
	ofPixels_(const ofPixels_<T> & mom);
	ofPixels_<T>& operator=(const ofPixels_<T> & mom);

	template<typename T2>
	ofPixels_(const ofPixels_<T2> & mom);

	template<typename T2>
	ofPixels_<T>& operator=(const ofPixels_<T2> & mom);

	void allocate(int w, int h, int channels);
	void allocate(int w, int h, ofPixelFormat type);
	void allocate(int w, int h, ofImageType type);

	void set(T val);
	void set(int channel,T val);
	void setFromPixels(const T * newPixels,int w, int h, int channels);
	void setFromPixels(const T * newPixels,int w, int h, ofImageType type);
	void setFromExternalPixels(T * newPixels,int w, int h, int channels);
	void setFromAlignedPixels(const T * newPixels, int width, int height, int channels, int stride);

	void swapRgb();

	void clear();
	
	T * getPixels();
	const T * getPixels() const;

	int getPixelIndex(int x, int y) const;
	ofColor getColor(int x, int y) const;
	void setColor(int x, int y, ofColor color);

	const T& operator[](int pos) const;
	T& operator[](int pos);

	bool isAllocated() const;

	int getWidth() const;
	int getHeight() const;

	int getBytesPerPixel() const;
	int getBitsPerPixel() const;
	int getBytesPerChannel() const;
	int getBitsPerChannel() const;
	int getNumChannels() const;

	ofPixels_<T> getChannel(int channel) const;
	void setChannel(int channel, const ofPixels_<T> channelPixels);

	ofImageType getImageType() const;

	int size() const;

private:

	void copyFrom( const ofPixels_<T>& mom );

	template<typename T2>
	void copyFrom( const ofPixels_<T2>& mom );
	
	T * pixels;
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

template<typename DstType>
template<typename SrcType>
ofPixels_<DstType>::ofPixels_(const ofPixels_<SrcType> & mom){
	(*this).template copyFrom( mom );
}

template<typename DstType>
template<typename SrcType>
ofPixels_<DstType>& ofPixels_<DstType>::operator=(const ofPixels_<SrcType> & mom){
	(*this).template copyFrom( mom );
	return *this;
}

template<typename DstType>
template<typename SrcType>
void ofPixels_<DstType>::copyFrom(const ofPixels_<SrcType> & mom){
	if(mom.isAllocated()){
		allocate(mom.getWidth(),mom.getHeight(),mom.getNumChannels());

		float srcMax = ( (sizeof(SrcType) == sizeof(float) ) ? 1.f : numeric_limits<SrcType>::max() );
		float dstMax = ( (sizeof(DstType) == sizeof(float) ) ? 1.f : numeric_limits<DstType>::max() );
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

