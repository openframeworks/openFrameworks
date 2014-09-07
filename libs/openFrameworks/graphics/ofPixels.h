#pragma once

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofColor.h"
#include "ofMath.h"
#include <limits>


//---------------------------------------
enum ofInterpolationMethod {
	OF_INTERPOLATE_NEAREST_NEIGHBOR =1,
	OF_INTERPOLATE_BILINEAR			=2,
	OF_INTERPOLATE_BICUBIC			=3
};


/// \brief A class representing a collection of pixels.
template <typename PixelType>
class ofPixels_ {
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
	void setFromPixels(const PixelType * newPixels, int w, int h, int channels);
	void setFromPixels(const PixelType * newPixels, int w, int h, ofImageType type);
	void setFromExternalPixels(PixelType * newPixels, int w, int h, int channels);
	void setFromAlignedPixels(const PixelType * newPixels, int width, int height, int channels, int stride);
	void swap(ofPixels_<PixelType> & pix);

	//From ofPixelsUtils
	// crop to a new width and height, this reallocates memory.
	void crop(int x, int y, int width, int height);
	// not in place
	
	void cropTo(ofPixels_<PixelType> &toPix, int x, int y, int _width, int _height) const;

	// crop to a new width and height, this reallocates memory.
	void rotate90(int nClockwiseRotations);
	void rotate90To(ofPixels_<PixelType> & dst, int nClockwiseRotations) const;
	void mirrorTo(ofPixels_<PixelType> & dst, bool vertically, bool horizontal) const;
	void mirror(bool vertically, bool horizontal);
	bool resize(int dstWidth, int dstHeight, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR);	
	bool resizeTo(ofPixels_<PixelType> & dst, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR) const;
	bool pasteInto(ofPixels_<PixelType> &dst, int x, int y) const;

	void swapRgb();

	void clear();

	PixelType * getPixels();

	/// \brief Retrieves pixel data from the ofPixel object.
	///
	/// \returns A raw pointer to the pixel data.
	const PixelType * getPixels() const;

	int getPixelIndex(int x, int y) const;
	ofColor_<PixelType> getColor(int x, int y) const;
	void setColor(int x, int y, const ofColor_<PixelType>& color);
	void setColor(int index, const ofColor_<PixelType>& color);
	void setColor(const ofColor_<PixelType>& color);

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
	
	/// \brief Changes the image type for the ofPixels object
	///
	/// \param imageType Can be one of the following: OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA
	void setImageType(ofImageType imageType);
	/// \brief Sets the number of color channels for the ofPixel object.
	///
	/// \param channel The number of color channels you would like the ofPixel object to have. Conventional values would be 1 (grayscale), 3 (RGB), and 4 (CMYK).
	void setNumChannels(int numChannels);

	int size() const;

	typedef PixelType * iterator;
	typedef const PixelType * const_iterator;
	typedef PixelType * reverse_iterator;
	typedef const PixelType * const_reverse_iterator;
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	reverse_iterator rbegin();
	reverse_iterator rend();
	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;

	struct Pixel: public std::iterator<std::forward_iterator_tag,Pixel>{
		Pixel(PixelType * pixel, int channels);
        const Pixel& operator*() const;
        const Pixel* operator->() const;
        Pixel& operator++();
        Pixel operator++(int);
        Pixel operator+(int) const;
        bool operator!=(Pixel const& rhs) const;
        PixelType & operator[](int idx);

	private:
		PixelType * pixel;
		int channels;
	};

	struct Pixels{
		Pixels(PixelType * begin, PixelType * end, int channels);
		Pixel begin();
		Pixel end();
	private:
		PixelType * _begin;
		PixelType * _end;
		int channels;
	};

	struct Line: public std::iterator<std::forward_iterator_tag,Line>{
		Line(PixelType * _begin, int stride, int channels, int lineNum);
        const Line& operator*() const;
        const Line* operator->() const;
        Line& operator++();
        Line operator++(int);
        Line operator+(int) const;
        bool operator!=(Line const& rhs) const;
        PixelType * begin();
        PixelType * end();
        const PixelType * begin() const;
        const PixelType * end() const;
		int getLineNum() const;
		ofPixels_<PixelType> asPixels();
		const ofPixels_<PixelType> asPixels() const;
		int getStride() const;
		Pixels getPixels();

	private:
		PixelType * _begin;
		PixelType * _end;
		int stride;
		int channels;
		int lineNum;
	};

	struct Lines{
		Lines(PixelType * _begin, PixelType * _end, int stride, int channels, int lines);

        Line begin();

        Line end();

	private:
        PixelType * _begin;
		PixelType * _end;
		int stride;
		int channels;
		int lines;
	};

	struct ConstPixel: public std::iterator<std::forward_iterator_tag,Pixel>{
		ConstPixel(const PixelType * pixel, int channels);
        const ConstPixel& operator*() const;
        const ConstPixel* operator->() const;
        ConstPixel& operator++();
        ConstPixel operator++(int);
        ConstPixel operator+(int) const;
        bool operator!=(ConstPixel const& rhs) const;
        const PixelType & operator[](int idx);

	private:
		const PixelType * pixel;
		int channels;
	};

	struct ConstPixels{
		ConstPixels(const PixelType * begin, const PixelType * end, int channels);
		ConstPixel begin();
		ConstPixel end();
	private:
		const PixelType * _begin;
		const PixelType * _end;
		int channels;
	};

	struct ConstLine: public std::iterator<std::forward_iterator_tag,Line>{
		ConstLine(const PixelType * _begin, int stride, int channels, int lineNum);
        const ConstLine& operator*() const;
        const ConstLine* operator->() const;
        ConstLine& operator++();
        ConstLine operator++(int);
        ConstLine operator+(int) const;
        bool operator!=(ConstLine const& rhs) const;
        const PixelType * begin() const;
        const PixelType * end() const;
		int getLineNum() const;
		int getStride() const;
		ConstPixels getPixels() const;

	private:
		const PixelType * _begin;
		const PixelType * _end;
		int stride;
		int channels;
		int lineNum;
	};

	struct ConstLines{
		ConstLines(const PixelType * _begin, const PixelType * _end, int stride, int channels, int lines);

        ConstLine begin() const;

        ConstLine end() const;

	private:
        const PixelType * _begin;
		const PixelType * _end;
		int stride;
		int channels;
		int lines;
	};


	Line getLine(int line);
	ConstLine getLine(int line) const;
	Lines getLines();
	ConstLines getLines() const;
	Pixels getPixelsIter();


private:
	static float bicubicInterpolate(const float *patch, float x,float y, float x2,float y2, float x3,float y3);

	void copyFrom( const ofPixels_<PixelType>& mom );

	template<typename SrcType>
	void copyFrom( const ofPixels_<SrcType>& mom );
	
	PixelType * pixels;
	int 	width;
	int 	height;

	int 	channels; // 1, 3, 4 channels per pixel (grayscale, rgb, rgba)
	bool	bAllocated;
	bool	pixelsOwner;			// if set from external data don't delete it

};


typedef ofPixels_<unsigned char> ofPixels;
typedef ofPixels_<float> ofFloatPixels;
typedef ofPixels_<unsigned short> ofShortPixels;


typedef ofPixels& ofPixelsRef;
typedef ofFloatPixels& ofFloatPixelsRef;
typedef ofShortPixels& ofShortPixelsRef;

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
	width = 0;
	height = 0;
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

		const float srcMax = ( (sizeof(SrcType) == sizeof(float) ) ? 1.f : numeric_limits<SrcType>::max() );
		const float dstMax = ( (sizeof(PixelType) == sizeof(float) ) ? 1.f : numeric_limits<PixelType>::max() );
		const float factor = dstMax / srcMax;

		if(sizeof(SrcType) == sizeof(float)) {
			// coming from float we need a special case to clamp the values
			for(int i = 0; i < mom.size(); i++){
				pixels[i] = CLAMP(mom[i], 0, 1) * factor;
			}
		} else{
			// everything else is a straight scaling
			for(int i = 0; i < mom.size(); i++){
				pixels[i] = mom[i] * factor;
			}
		}
	}
}
//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::iterator ofPixels_<PixelType>::begin(){
	return &pixels[0];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::iterator ofPixels_<PixelType>::end(){
	return &pixels[size()];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::const_iterator ofPixels_<PixelType>::begin() const{
	return &pixels[0];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::const_iterator ofPixels_<PixelType>::end() const{
	return &pixels[size()];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::reverse_iterator ofPixels_<PixelType>::rbegin(){
	return &pixels[size()];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::reverse_iterator ofPixels_<PixelType>::rend(){
	return &pixels[-1];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::const_reverse_iterator ofPixels_<PixelType>::rbegin() const{
	return &pixels[size()];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::const_reverse_iterator ofPixels_<PixelType>::rend() const{
	return &pixels[-1];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::Pixel::Pixel(PixelType * pixel, int channels)
:pixel(pixel)
,channels(channels){

}

//----------------------------------------------------------------------
template<typename PixelType>
inline const typename ofPixels_<PixelType>::Pixel& ofPixels_<PixelType>::Pixel::operator*() const{
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline const typename ofPixels_<PixelType>::Pixel* ofPixels_<PixelType>::Pixel::operator->() const{
	return this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel& ofPixels_<PixelType>::Pixel::operator++(){
	pixel += channels;
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel ofPixels_<PixelType>::Pixel::operator++(int){
	Pixel tmp(*this);
	operator++();
	return tmp;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel ofPixels_<PixelType>::Pixel::operator+(int i) const{
	return Pixel(pixel + channels * i, channels);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::Pixel::operator!=(Pixel const& rhs) const{
	return pixel != rhs.pixel;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline PixelType & ofPixels_<PixelType>::Pixel::operator[](int idx){
	return pixel[idx];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::Pixels::Pixels(PixelType * begin, PixelType * end, int channels)
:_begin(begin)
,_end(end)
,channels(channels){}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel ofPixels_<PixelType>::Pixels::begin(){
	return Pixel(_begin,channels);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel ofPixels_<PixelType>::Pixels::end(){
	return Pixel(_end,channels);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::ConstPixel::ConstPixel(const PixelType * pixel, int channels)
:pixel(pixel)
,channels(channels){

}

//----------------------------------------------------------------------
template<typename PixelType>
inline const typename ofPixels_<PixelType>::ConstPixel& ofPixels_<PixelType>::ConstPixel::operator*() const{
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline const typename ofPixels_<PixelType>::ConstPixel* ofPixels_<PixelType>::ConstPixel::operator->() const{
	return this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixel& ofPixels_<PixelType>::ConstPixel::operator++(){
	pixel += channels;
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixel ofPixels_<PixelType>::ConstPixel::operator++(int){
	ConstPixel tmp(*this);
	operator++();
	return tmp;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixel ofPixels_<PixelType>::ConstPixel::operator+(int i) const{
	return ConstPixel(pixel + channels * i, channels);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::ConstPixel::operator!=(ConstPixel const& rhs) const{
	return pixel != rhs.pixel;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline const PixelType & ofPixels_<PixelType>::ConstPixel::operator[](int idx){
	return pixel[idx];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::ConstPixels::ConstPixels(const PixelType * begin, const PixelType * end, int channels)
:_begin(begin)
,_end(end)
,channels(channels){}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixel ofPixels_<PixelType>::ConstPixels::begin(){
	return ConstPixel(_begin,channels);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixel ofPixels_<PixelType>::ConstPixels::end(){
	return ConstPixel(_end,channels);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::Line::Line(PixelType * _begin, int stride, int channels, int lineNum)
:_begin(_begin)
,_end(_begin+stride)
,stride(stride)
,channels(channels)
,lineNum(lineNum){}

//----------------------------------------------------------------------
template<typename PixelType>
inline const typename ofPixels_<PixelType>::Line& ofPixels_<PixelType>::Line::operator*() const{
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline const typename ofPixels_<PixelType>::Line* ofPixels_<PixelType>::Line::operator->() const{
	return this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line& ofPixels_<PixelType>::Line::operator++(){
	_begin = _end;
	_end = _begin + stride;
	++lineNum;
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line ofPixels_<PixelType>::Line::operator++(int) {
	Line tmp(*this);
	operator++();
	return tmp;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line  ofPixels_<PixelType>::Line::operator+(int i) const{
	return Line(_begin+stride*i,stride,channels,lineNum+i);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::Line::operator!=(typename ofPixels_<PixelType>::Line const& rhs) const{
	return rhs._begin != _begin || rhs._end != _end || rhs.stride != stride || rhs.lineNum!=lineNum;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::iterator ofPixels_<PixelType>::Line::begin(){
	return _begin;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::iterator ofPixels_<PixelType>::Line::end(){
	return _end;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::const_iterator ofPixels_<PixelType>::Line::begin() const{
	return _begin;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::const_iterator ofPixels_<PixelType>::Line::end() const{
	return _end;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline int ofPixels_<PixelType>::Line::getLineNum() const{
	return lineNum;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType> ofPixels_<PixelType>::Line::asPixels(){
	ofPixels_<PixelType> pixels;
	pixels.setFromExternalPixels(_begin,stride,channels,1);
	return pixels;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline const ofPixels_<PixelType> ofPixels_<PixelType>::Line::asPixels() const{
	ofPixels_<PixelType> pixels;
	pixels.setFromExternalPixels(_begin,stride,channels,1);
	return pixels;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline int ofPixels_<PixelType>::Line::getStride() const{
	return stride;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixels ofPixels_<PixelType>::Line::getPixels(){
	return Pixels(_begin,_end,channels);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::Lines::Lines(PixelType * _begin, PixelType * _end, int stride, int channels, int lines)
:_begin(_begin)
,_end(_end)
,stride(stride)
,channels(channels)
,lines(lines){}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line ofPixels_<PixelType>::Lines::begin(){
	return Line(_begin,stride,channels,0);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line ofPixels_<PixelType>::Lines::end(){
	return Line(_end,stride,channels,lines);
}



//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::ConstLine::ConstLine(const PixelType * _begin, int stride, int channels, int lineNum)
:_begin(_begin)
,_end(_begin+stride)
,stride(stride)
,channels(channels)
,lineNum(lineNum){}

//----------------------------------------------------------------------
template<typename PixelType>
inline const typename ofPixels_<PixelType>::ConstLine& ofPixels_<PixelType>::ConstLine::operator*() const{
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline const typename ofPixels_<PixelType>::ConstLine* ofPixels_<PixelType>::ConstLine::operator->() const{
	return this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLine& ofPixels_<PixelType>::ConstLine::operator++(){
	_begin = _end;
	_end = _begin + stride;
	++lineNum;
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLine ofPixels_<PixelType>::ConstLine::operator++(int) {
	ConstLine tmp(*this);
	operator++();
	return tmp;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLine  ofPixels_<PixelType>::ConstLine::operator+(int i) const{
	return ConstLine(_begin+stride*i,stride,channels,lineNum+i);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::ConstLine::operator!=(typename ofPixels_<PixelType>::ConstLine const& rhs) const{
	return rhs._begin != _begin || rhs._end != _end || rhs.stride != stride || rhs.lineNum!=lineNum;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::const_iterator ofPixels_<PixelType>::ConstLine::begin() const{
	return _begin;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::const_iterator ofPixels_<PixelType>::ConstLine::end() const{
	return _end;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline int ofPixels_<PixelType>::ConstLine::getLineNum() const{
	return lineNum;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline int ofPixels_<PixelType>::ConstLine::getStride() const{
	return stride;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixels ofPixels_<PixelType>::ConstLine::getPixels() const{
	return ConstPixels(_begin,_end,channels);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::ConstLines::ConstLines(const PixelType * _begin, const PixelType * _end, int stride, int channels, int lines)
:_begin(_begin)
,_end(_end)
,stride(stride)
,channels(channels)
,lines(lines){}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLine ofPixels_<PixelType>::ConstLines::begin() const{
	return ConstLine(_begin,stride,channels,0);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLine ofPixels_<PixelType>::ConstLines::end() const{
	return ConstLine(_end,stride,channels,lines);
}


//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line ofPixels_<PixelType>::getLine(int line){
	return Line(pixels+(width*channels*line), width*channels, channels, line);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLine ofPixels_<PixelType>::getLine(int line) const{
	return ConstLine(pixels+(width*channels*line), width*channels, channels, line);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Lines ofPixels_<PixelType>::getLines(){
	return Lines(begin(),end(),width*channels,channels,getHeight());
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLines ofPixels_<PixelType>::getLines() const{
	return ConstLines(begin(),end(),width*channels,channels,getHeight());
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixels ofPixels_<PixelType>::getPixelsIter(){
	return Pixels(begin(),end(),channels);
}

namespace std{
template<typename PixelType>
void swap(ofPixels_<PixelType> & src, ofPixels_<PixelType> & dst){
	src.swap(dst);
}
}
