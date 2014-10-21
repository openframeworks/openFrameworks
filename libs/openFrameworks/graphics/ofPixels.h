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
	void setFromPixels(const PixelType * newPixels,int w, int h, int channels);
	void setFromPixels(const PixelType * newPixels,int w, int h, ofPixelFormat pixelFormat);
	void setFromPixels(const PixelType * newPixels,int w, int h, ofImageType type);
	void setFromExternalPixels(PixelType * newPixels,int w, int h, int channels);
	void setFromExternalPixels(PixelType * newPixels,int w, int h, ofPixelFormat pixelFormat);
	void setFromAlignedPixels(const PixelType * newPixels, int width, int height, int channels, int stride);
	void setFromAlignedPixels(const PixelType * newPixels, int width, int height, ofPixelFormat pixelFormat, int stride);
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

	/// \brief Retrieves pixel data from the ofPixel object.
	///
	/// \returns A raw pointer to the pixel data.
	PixelType * getPixels();
	const PixelType * getPixels() const;

	OF_DEPRECATED_MSG("Accessing pixels as unsigned char*, instead use ofPixels or getPixels().getPixels() if you are really trying to access the raw data",operator PixelType*());
	OF_DEPRECATED_MSG("Accessing pixels as unsigned char*, instead use ofPixels or getPixels().getPixels() if you are really trying to access the raw data",operator const PixelType*());

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
	int getBytesStride() const;
	int getNumChannels() const;
	int getTotalBytes() const;
	int getNumPlanes() const;
	ofPixels_<PixelType> getPlane(int plane);

	ofPixels_<PixelType> getChannel(int channel) const;
	
	void setChannel(int channel, const ofPixels_<PixelType> channelPixels);

	ofImageType getImageType() const;
	
	/// \brief Changes the image type for the ofPixels object
	///
	/// \param imageType Can be one of the following: OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA
	void setImageType(ofImageType imageType);

	ofPixelFormat getPixelFormat() const;

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
		Pixel(PixelType * pixel, int bytesPerPixel, ofPixelFormat pixelFormat);
        const Pixel& operator*() const;
        const Pixel* operator->() const;
        Pixel& operator++();
        Pixel operator++(int);
        Pixel operator+(int) const;
        Pixel operator+=(int);
        bool operator!=(Pixel const& rhs) const;
        bool operator<(Pixel const& rhs) const;
        PixelType & operator[](int idx);

	private:
		PixelType * pixel;
		int componentsPerPixel;
		ofPixelFormat pixelFormat;
	};

	struct Pixels{
		Pixels(PixelType * begin, PixelType * end, int componentsPerPixel, ofPixelFormat pixelFormat);
		Pixel begin();
		Pixel end();
	private:
		PixelType * _begin;
		PixelType * _end;
		int componentsPerPixel;
		ofPixelFormat pixelFormat;
	};

	struct Line: public std::iterator<std::forward_iterator_tag,Line>{
		Line(PixelType * _begin, int stride, int componentsPerPixel, int lineNum, ofPixelFormat pixelFormat);
        const Line& operator*() const;
        const Line* operator->() const;
        Line& operator++();
        Line operator++(int);
        Line operator+(int) const;
        Line operator+=(int);
        bool operator!=(Line const& rhs) const;
        bool operator<(Line const& rhs) const;
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
		int componentsPerPixel;
		int lineNum;
		ofPixelFormat pixelFormat;
	};

	struct Lines{
		Lines(PixelType * _begin, PixelType * _end, int stride, int componentsPerPixel, int lines, ofPixelFormat pixelFormat);

        Line begin();

        Line end();

	private:
        PixelType * _begin;
		PixelType * _end;
		int stride;
		int componentsPerPixel;
		int lines;
		ofPixelFormat pixelFormat;
	};

	struct ConstPixel: public std::iterator<std::forward_iterator_tag,Pixel>{
		ConstPixel(const PixelType * pixel, int bytesPerPixel, ofPixelFormat pixelFormat);
		const ConstPixel& operator*() const;
		const ConstPixel* operator->() const;
		ConstPixel& operator++();
		ConstPixel operator++(int);
		ConstPixel operator+(int) const;
		ConstPixel operator+=(int);
		bool operator!=(ConstPixel const& rhs) const;
		bool operator<(ConstPixel const& rhs) const;
		const PixelType & operator[](int idx) const;

	private:
		const PixelType * pixel;
		int componentsPerPixel;
		ofPixelFormat pixelFormat;
	};

	struct ConstPixels{
		ConstPixels(const PixelType * begin, const PixelType * end, int componentsPerPixel, ofPixelFormat pixelFormat);
		ConstPixel begin() const;
		ConstPixel end() const;
	private:
		const PixelType * _begin;
		const PixelType * _end;
		int componentsPerPixel;
		ofPixelFormat pixelFormat;
	};

	struct ConstLine: public std::iterator<std::forward_iterator_tag,Line>{
		ConstLine(const PixelType * _begin, int stride, int componentsPerPixel, int lineNum, ofPixelFormat pixelFormat);
		const ConstLine& operator*() const;
		const ConstLine* operator->() const;
		ConstLine& operator++();
		ConstLine operator++(int);
		ConstLine operator+(int) const;
		ConstLine operator+=(int);
		bool operator!=(ConstLine const& rhs) const;
		bool operator<(ConstLine const& rhs) const;
		const PixelType * begin() const;
		const PixelType * end() const;
		int getLineNum() const;
		int getStride() const;
		ConstPixels getPixels() const;

	private:
		const PixelType * _begin;
		const PixelType * _end;
		int stride;
		int componentsPerPixel;
		int lineNum;
		ofPixelFormat pixelFormat;
	};

	struct ConstLines{
		ConstLines(const PixelType * _begin, const PixelType * _end, int stride, int componentsPerPixel, int lines, ofPixelFormat pixelFormat);

		ConstLine begin() const;

		ConstLine end() const;

	private:
		const PixelType * _begin;
		const PixelType * _end;
		int stride;
		int componentsPerPixel;
		int lines;
		ofPixelFormat pixelFormat;
	};

	Line getLine(int line);
	Lines getLines();
	Pixels getPixelsIter();
	ConstLine getConstLine(int line) const;
	ConstLines getConstLines() const;
	ConstPixels getConstPixelsIter() const;


private:
	static float bicubicInterpolate(const float *patch, float x,float y, float x2,float y2, float x3,float y3);

	void copyFrom( const ofPixels_<PixelType>& mom );

	template<typename SrcType>
	void copyFrom( const ofPixels_<SrcType>& mom );
	
	PixelType * pixels;
	int 	width;
	int 	height;

	//int 	channels; // 1, 3, 4 channels per pixel (grayscale, rgb, rgba)
	int 	pixelsSize;
	bool	bAllocated;
	bool	pixelsOwner;			// if set from external data don't delete it
	ofPixelFormat pixelFormat;

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
	pixelsSize = 0;
	pixels = NULL;
	width = 0;
	height = 0;
	pixelFormat = OF_PIXELS_UNKNOWN;
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
inline ofPixels_<PixelType>::Pixel::Pixel(PixelType * pixel, int componentsPerPixel, ofPixelFormat pixelFormat)
:pixel(pixel)
,componentsPerPixel(componentsPerPixel)
,pixelFormat(pixelFormat){

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
	pixel += componentsPerPixel;
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
	return Pixel(pixel + componentsPerPixel * i, componentsPerPixel, pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel ofPixels_<PixelType>::Pixel::operator+=(int i){
	pixel += componentsPerPixel * i;
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::Pixel::operator!=(Pixel const& rhs) const{
	return pixel != rhs.pixel;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::Pixel::operator<(Pixel const& rhs) const{
	return pixel < rhs.pixel;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline PixelType & ofPixels_<PixelType>::Pixel::operator[](int idx){
	return pixel[idx];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::Pixels::Pixels(PixelType * begin, PixelType * end, int componentsPerPixel, ofPixelFormat pixelFormat)
:_begin(begin)
,_end(end)
,componentsPerPixel(componentsPerPixel)
,pixelFormat(pixelFormat){}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel ofPixels_<PixelType>::Pixels::begin(){
	return Pixel(_begin,componentsPerPixel,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel ofPixels_<PixelType>::Pixels::end(){
	return Pixel(_end,componentsPerPixel,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::Line::Line(PixelType * _begin, int stride, int componentsPerPixel, int lineNum, ofPixelFormat pixelFormat)
:_begin(_begin)
,_end(_begin+stride)
,stride(stride)
,componentsPerPixel(componentsPerPixel)
,lineNum(lineNum)
,pixelFormat(pixelFormat){}

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
	return Line(_begin+stride*i,stride,componentsPerPixel,lineNum+i,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line  ofPixels_<PixelType>::Line::operator+=(int i){
	_begin = _begin+stride*i;
	_end = _begin + stride;
	lineNum += i;
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::Line::operator!=(typename ofPixels_<PixelType>::Line const& rhs) const{
	return rhs._begin != _begin || rhs._end != _end || rhs.stride != stride || rhs.lineNum!=lineNum;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::Line::operator<(typename ofPixels_<PixelType>::Line const& rhs) const{
	return _begin < rhs._begin || _end < rhs._end;
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
	pixels.setFromExternalPixels(_begin,stride,pixelFormat,1);
	return pixels;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline const ofPixels_<PixelType> ofPixels_<PixelType>::Line::asPixels() const{
	ofPixels_<PixelType> pixels;
	pixels.setFromExternalPixels(_begin,stride,pixelFormat,1);
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
	return Pixels(_begin,_end,componentsPerPixel,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::Lines::Lines(PixelType * _begin, PixelType * _end, int stride, int componentsPerPixel, int lines, ofPixelFormat pixelFormat)
:_begin(_begin)
,_end(_end)
,stride(stride)
,componentsPerPixel(componentsPerPixel)
,lines(lines)
,pixelFormat(pixelFormat){}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line ofPixels_<PixelType>::Lines::begin(){
	return Line(_begin,stride,componentsPerPixel,0,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line ofPixels_<PixelType>::Lines::end(){
	return Line(_end,stride,componentsPerPixel,lines,pixelFormat);
}


//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line ofPixels_<PixelType>::getLine(int line){
	return Line(pixels+(width*getNumChannels()*line), width*getNumChannels(), getNumChannels(), line,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Lines ofPixels_<PixelType>::getLines(){
	return Lines(begin(),end(),width*getNumChannels(),getNumChannels(),getHeight(),pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixels ofPixels_<PixelType>::getPixelsIter(){
	return Pixels(begin(),end(),getNumChannels(),pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::ConstPixel::ConstPixel(const PixelType * pixel, int componentsPerPixel, ofPixelFormat pixelFormat)
:pixel(pixel)
,componentsPerPixel(componentsPerPixel)
,pixelFormat(pixelFormat){

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
	pixel += componentsPerPixel;
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
	return ConstPixel(pixel + componentsPerPixel * i, componentsPerPixel, pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixel ofPixels_<PixelType>::ConstPixel::operator+=(int i){
	pixel += componentsPerPixel * i;
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::ConstPixel::operator!=(ConstPixel const& rhs) const{
	return pixel != rhs.pixel;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::ConstPixel::operator<(ConstPixel const& rhs) const{
	return pixel < rhs.pixel;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline const PixelType & ofPixels_<PixelType>::ConstPixel::operator[](int idx) const{
	return pixel[idx];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::ConstPixels::ConstPixels(const PixelType * begin, const PixelType * end, int componentsPerPixel, ofPixelFormat pixelFormat)
:_begin(begin)
,_end(end)
,componentsPerPixel(componentsPerPixel)
,pixelFormat(pixelFormat){}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixel ofPixels_<PixelType>::ConstPixels::begin() const{
	return ConstPixel(_begin,componentsPerPixel,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixel ofPixels_<PixelType>::ConstPixels::end() const{
	return ConstPixel(_end,componentsPerPixel,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::ConstLine::ConstLine(const PixelType * _begin, int stride, int componentsPerPixel, int lineNum, ofPixelFormat pixelFormat)
:_begin(_begin)
,_end(_begin+stride)
,stride(stride)
,componentsPerPixel(componentsPerPixel)
,lineNum(lineNum)
,pixelFormat(pixelFormat){}

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
	return ConstLine(_begin+stride*i,stride,componentsPerPixel,lineNum+i,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLine  ofPixels_<PixelType>::ConstLine::operator+=(int i){
	_begin = _begin+stride*i;
	_end = _begin + stride;
	lineNum += i;
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::ConstLine::operator!=(typename ofPixels_<PixelType>::ConstLine const& rhs) const{
	return rhs._begin != _begin || rhs._end != _end || rhs.stride != stride || rhs.lineNum!=lineNum;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::ConstLine::operator<(typename ofPixels_<PixelType>::ConstLine const& rhs) const{
	return _begin < rhs._begin || _end < rhs._end;
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
	return ConstPixels(_begin,_end,componentsPerPixel,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::ConstLines::ConstLines(const PixelType * _begin, const PixelType * _end, int stride, int componentsPerPixel, int lines, ofPixelFormat pixelFormat)
:_begin(_begin)
,_end(_end)
,stride(stride)
,componentsPerPixel(componentsPerPixel)
,lines(lines)
,pixelFormat(pixelFormat){}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLine ofPixels_<PixelType>::ConstLines::begin() const{
	return ConstLine(_begin,stride,componentsPerPixel,0,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLine ofPixels_<PixelType>::ConstLines::end() const{
	return ConstLine(_end,stride,componentsPerPixel,lines,pixelFormat);
}


//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLine ofPixels_<PixelType>::getConstLine(int line) const{
	return ConstLine(pixels+(width*getNumChannels()*line), width*getNumChannels(), getNumChannels(), line,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLines ofPixels_<PixelType>::getConstLines() const{
	return ConstLines(begin(),end(),width*getNumChannels(),getNumChannels(),getHeight(),pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixels ofPixels_<PixelType>::getConstPixelsIter() const{
	return ConstPixels(begin(),end(),getNumChannels(),pixelFormat);
}

namespace std{
template<typename PixelType>
void swap(ofPixels_<PixelType> & src, ofPixels_<PixelType> & dst){
	src.swap(dst);
}
}
