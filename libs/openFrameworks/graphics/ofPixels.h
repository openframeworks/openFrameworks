#pragma once

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofColor.h"
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
typedef ofPixels_<unsigned short> ofShortPixels;


typedef ofPixels& ofPixelsRef;


// sorry for these ones, being templated functions inside a template i needed to do it in the .h
// they allow to do things like:
//
// ofPixels pix;
// ofFloatPixels pixf;
// pix = pixf

template<typename T>
template<typename T2>
ofPixels_<T>::ofPixels_(const ofPixels_<T2> & mom){
	(*this).template copyFrom( mom );
}

template<typename T>
template<typename T2>
ofPixels_<T>& ofPixels_<T>::operator=(const ofPixels_<T2> & mom){
	(*this).template copyFrom( mom );
	return *this;
}

template<typename T>
template<typename T2>
void ofPixels_<T>::copyFrom(const ofPixels_<T2> & mom){
	if(mom.isAllocated()){
		bytesPerChannel = sizeof(T);
		bitsPerChannel = bytesPerChannel*8;
		allocate(mom.getWidth(),mom.getHeight(),mom.getNumChannels());

		float factor;

		if(sizeof(T2)==4){
			factor = numeric_limits<T>::max();
			for(int i=0; i<mom.size(); i++){
				pixels[i] = ofClamp(mom[i],0,1)*factor;
			}
			return;
		}else if(sizeof(T)==4){
			factor = 1.f/float(numeric_limits<T2>::max());
		}else{
			factor = float(numeric_limits<T>::max())/float(std::numeric_limits<T2>::max());
		}

		for(int i=0; i<mom.size(); i++){
			pixels[i] = mom[i]*factor;
		}
	}
}

