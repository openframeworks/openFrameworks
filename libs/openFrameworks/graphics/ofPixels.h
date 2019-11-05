#pragma once

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofColor.h"
#include "ofLog.h"
#include "ofMath.h"
#include <limits>


/// \file
/// ofPixels is an object for working with blocks of pixels, those pixels can
/// be copied from an image that you've loaded, something that you've drawn
/// using ofGraphics, or a ofVideoGrabber instance.
///
/// You can create an image from pixels, using on ofPixels object like so:
///
/// ~~~~{.cpp}
/// ofPixels p;
/// ofLoadImage(p, "pathToImage.jpg");
/// ~~~~
///
/// ofPixels represents pixels data on the CPU as opposed to an ofTexture
/// which represents pixel data on the GPU. They can easily be made inter-
/// operational though:
///
/// ~~~~{.cpp}
/// ofTexture tex;
/// // do some stuff with tex
/// ofPixels pix;
/// tex.readToPixels(pix); // now all the pixels from tex are in pix
/// ~~~~
///
/// You can access the pixels in an ofPixels object with the `[]` operator.
///
/// ~~~~{.cpp}
/// ofPixels pix;
/// // put some stuff in the pixels
/// size_t i = 0;
/// while( i < pix.size()) {
/// 	char c = pix[i];
/// 	i++;
/// }
/// ~~~~
///
/// You can think of the ofPixels as the CPU side representation of pixel data
/// that can be sent to the GPU as an ofTexture object. To draw pixels, you
/// need to put them into an ofTexture and to manipulate an ofTextures pixel
/// data you need an ofPixels object.


enum ofInterpolationMethod {
	OF_INTERPOLATE_NEAREST_NEIGHBOR =1,
	OF_INTERPOLATE_BILINEAR			=2,
	OF_INTERPOLATE_BICUBIC			=3
};


/// \brief Used to represent the available pixel formats.
///
/// \sa ofPixels
enum ofPixelFormat: short{
	/// \brief A single-channel pixel, typically used for greyscale images.
	///
	/// This has 1 channel and a type-dependent number of bits per-pixel.
	OF_PIXELS_GRAY = 0,
	/// \brief A single-channel pixel with an alpha channel.
	///
	/// This has 2 channels and a type-dependent number of bits per-pixel.
	OF_PIXELS_GRAY_ALPHA = 1,

	/// \brief An RGB pixel with no alpha channel.
	///
	/// This has 3 channels and a type-dependent number of bits per-pixel.
	///
	/// \sa http://www.fourcc.org/rgb.php#BI_RGB
	OF_PIXELS_RGB=2,
	/// \brief A pixel used for color data with a blue/green/red channel order.
	///
	/// This has 3 channels and a type-dependent number of bits per-pixel.
	OF_PIXELS_BGR=3,
	/// \brief An RGBA pixel. This is typically used for color with transparency.
	///
	/// This has 4 channels and a type-dependent number of bits per-pixel.
	///
	/// \sa http://www.fourcc.org/rgb.php#RGBA
	OF_PIXELS_RGBA=4,
	/// \brief A pixel used for color/transparency with a blue/green/red/alpha channel order.
	///
	/// This has 4 channels and a type-dependent number of bits per-pixel.
	OF_PIXELS_BGRA=5,

	// \brief A 16-bit color pixel with 5-bit red and blue channels and a 6-bit green channel.
	OF_PIXELS_RGB565=6,

	/// \brief A 12-bit YUV 4:2:0 pixel with an interleaved U/V plane.
	///
	/// YUV 4:2:0 image with a plane of 8-bit Y samples followed by an
	/// interleaved U/V plane containing 8-bit 2x2 subsampled color difference
	/// samples.
	///
	/// \sa http://www.fourcc.org/yuv.php#NV12
	OF_PIXELS_NV12=7,
	/// \brief A 12-bit YUV 4:2:0 pixel with an interleaved V/U plane.
	///
	/// YUV 4:2:0 image with a plane of 8-bit Y samples followed by an
	/// interleaved V/U plane containing 8-bit 2x2 subsampled chroma samples.
	/// The same as NV12 except the interleave order of U and V is reversed.
	///
	/// \sa http://www.fourcc.org/yuv.php#NV21
	OF_PIXELS_NV21=8,
	/// \brief A 12-bit YUV NxM Y plane followed by (N/2)x(M/2) V and U planes.
	///
	/// \sa http://www.fourcc.org/yuv.php#YV12
	OF_PIXELS_YV12=9,
	/// \brief A 12-bit YUV format similar to ::OF_PIXELS_YV12, but with U & V reversed.
	///
	/// Note that IYUV and I420 appear to be identical.
	///
	/// \sa http://www.fourcc.org/yuv.php#IYUV
	OF_PIXELS_I420=10,
	/// \brief A 16-bit YUV 4:2:2 format.
	///
	/// \sa http://www.fourcc.org/yuv.php#YUY2
	OF_PIXELS_YUY2=11,
	/// \brief A 16-bit YUV 4:2:2 format.
	///
	/// \sa http://www.fourcc.org/yuv.php#UYVY
	OF_PIXELS_UYVY=12,

	/// \brief A single channel pixel, typically used for the luma component of YUV.
	OF_PIXELS_Y,
	/// \brief A single channel pixel, typically used (with V) for the chroma component of YUV.
	OF_PIXELS_U,
	/// \brief A single channel pixel, typically used (with U) for the chroma component of YUV.
	OF_PIXELS_V,
	/// \brief A two channel pixel, with U first, representing both chroma components of YUV.
	OF_PIXELS_UV,
	/// \brief A two channel pixel, with V first, representing both chroma components of YUV.
	OF_PIXELS_VU,

	/// \brief This is a placeholder to indicate the last valid enum.
	OF_PIXELS_NUM_FORMATS,

	/// \brief This indicates an unknown pixel type.
	OF_PIXELS_UNKNOWN=-1,
	/// \brief This indicates an unknown, native pixel type.
	OF_PIXELS_NATIVE=-2
};

#define OF_PIXELS_MONO OF_PIXELS_GRAY
#define OF_PIXELS_R OF_PIXELS_GRAY
#define OF_PIXELS_RG OF_PIXELS_GRAY_ALPHA

template<typename T>
std::string ofToString(const T & v);
template<>
std::string ofToString(const ofPixelFormat & pixelType);

enum ofImageType: short;

/// \brief A class representing a collection of pixels.
template <typename PixelType>
class ofPixels_ {
public:

	/// \name Construction And Allocation
	/// \{
	ofPixels_();
	~ofPixels_();
	ofPixels_(const ofPixels_<PixelType> & mom);
	ofPixels_(ofPixels_<PixelType> && mom);

	template<typename SrcType>
	ofPixels_(const ofPixels_<SrcType> & mom);


	/// \brief Allocates space for pixel data
	/// \param w Width of pixel array
	/// \param h Height of pixel array
	/// \param channels Number of channels per pixel
	void allocate(size_t w, size_t h, size_t channels);

	/// \brief Allocates space for pixel data
	///
	/// The pixelFormat can be one of the following:
	///
	///     OF_PIXELS_RGB
	///     OF_PIXELS_RGBA
	///     OF_PIXELS_BGRA
	///     OF_PIXELS_MONO
	///
	/// \param w Width of pixel array
	/// \param h Height of pixel array
	/// \param pixelFormat ofPixelFormat defining number of channels per pixel
	void allocate(size_t w, size_t h, ofPixelFormat pixelFormat);

	/// \brief Allocates space for pixel data
	///
	/// The imageType can be one of the following:
	///
	///     OF_IMAGE_GRAYSCALE
	///     OF_IMAGE_COLOR
	///     OF_IMAGE_COLOR_ALPHA
	///
	/// \param w Width of pixel array
	/// \param h Height of pixel array
	/// \param imageType ofImageType defining number of channels per pixel
	void allocate(size_t w, size_t h, ofImageType imageType);

	/// \brief Get whether memory has been allocated for an ofPixels object or not
	///
	/// Many operations like copying pixels, etc, automatically allocate
	/// the memory needed, but it's sometimes good to check.
	bool isAllocated() const;

	/// \brief Clear all the data from the ofPixels objects.
	/// After calling this you'll need to allocate()
	/// the ofPixels object again to use it.
	void clear();

	ofPixels_<PixelType>& operator=(const ofPixels_<PixelType> & mom);
	ofPixels_<PixelType>& operator=(ofPixels_<PixelType> && mom);

	template<typename SrcType>
	ofPixels_<PixelType>& operator=(const ofPixels_<SrcType> & mom);

	/// \}
	/// \name Set Pixel Data
	/// \{

	void set(PixelType val);
	void set(size_t channel,PixelType val);
	void setFromPixels(const PixelType * newPixels,size_t w, size_t h, size_t channels);
	void setFromPixels(const PixelType * newPixels,size_t w, size_t h, ofPixelFormat pixelFormat);
	void setFromPixels(const PixelType * newPixels,size_t w, size_t h, ofImageType type);
	void setFromExternalPixels(PixelType * newPixels,size_t w, size_t h, size_t channels);
	void setFromExternalPixels(PixelType * newPixels,size_t w, size_t h, ofPixelFormat pixelFormat);
	void setFromAlignedPixels(const PixelType * newPixels, size_t width, size_t height, size_t channels, size_t stride);
	void setFromAlignedPixels(const PixelType * newPixels, size_t width, size_t height, ofPixelFormat pixelFormat, size_t stride);
	/// \brief used to copy i420 pixels from gstreamer when (width % 4) != 0
	void setFromAlignedPixels(const PixelType * newPixels, size_t width, size_t height, ofPixelFormat pixelFormat, std::vector<size_t> strides);

	void swap(ofPixels_<PixelType> & pix);

	/// \}
	/// \name Modify Existing Data
	/// \{

	/// \brief Crop the pixels to a new width and height.
	///
	/// As a word of caution this reallocates memory and can be a bit
	/// expensive if done a lot.
	void crop(size_t x, size_t y, size_t width, size_t height);

	/// \brief Crop the pixels into the ofPixels reference passed in by `toPix.
	/// at the `x` and `y` and width the new width and height.
	///
	/// As a word of caution this reallocates memory and can be a bit
	/// expensive if done a lot.
	void cropTo(ofPixels_<PixelType> &toPix, size_t x, size_t y, size_t width, size_t height) const;

	// crop to a new width and height, this reallocates memory.
	void rotate90(int nClockwiseRotations);
	void rotate90To(ofPixels_<PixelType> & dst, int nClockwiseRotations) const;
	void mirrorTo(ofPixels_<PixelType> & dst, bool vertically, bool horizontal) const;

	/// \brief Mirror the pixels across the vertical and/or horizontal axis.
	/// \param vertically Set to true to mirror vertically
	/// \param horizontal Set to true to mirror horizontal
	void mirror(bool vertically, bool horizontal);

	/// \brief Resize the ofPixels instance to the dstHeight and dstWidth.
	///
	/// The options for the interpolation methods are as follows:
	///
	///     OF_INTERPOLATE_NEAREST_NEIGHBOR
	///     OF_INTERPOLATE_BILINEAR
	///     OF_INTERPOLATE_BICUBIC
	bool resize(size_t dstWidth, size_t dstHeight, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR);

	/// \brief Resize the ofPixels instance to the size of the ofPixels object passed in dst.
	///
	/// The options for the interpolation methods are as follows:
	///
	///     OF_INTERPOLATE_NEAREST_NEIGHBOR
	///     OF_INTERPOLATE_BILINEAR
	///     OF_INTERPOLATE_BICUBIC
	bool resizeTo(ofPixels_<PixelType> & dst, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR) const;

	/// \brief Paste the ofPixels object into another ofPixels object at the
	/// specified index, copying data from the ofPixels that the method is
	/// being called on to the ofPixels object at `&dst`. If the data being
	/// copied doesn't fit into the destination then the image is cropped.
	bool pasteInto(ofPixels_<PixelType> &dst, size_t x, size_t y) const;

	bool blendInto(ofPixels_<PixelType> &dst, size_t x, size_t y) const;

	/// \brief Swaps the R and B channels of an
	/// image, leaving the G and A channels as is.
	void swapRgb();

	/// \}
	/// \name Pixels Access
	/// \{

	/// \brief Retrieves pixel data from the ofPixel object.
	///
	/// \returns A raw pointer to the pixel data.
	PixelType * getData();
	const PixelType * getData() const;
	OF_DEPRECATED_MSG("Use getData instead",PixelType * getPixels());
	OF_DEPRECATED_MSG("Use getData instead",const PixelType * getPixels() const);

	/// \brief Get the pixel index at a x,y position
	///
	/// ~~~~{.cpp}
	/// ofColor yellow = ofColor::yellow;
	/// size_t ind = pix.getPixelIndex(mouseX, mouseY);
	/// pix.setPixel(ind, yellow);
	/// ~~~~
	size_t getPixelIndex(size_t x, size_t y) const;

	/// \brief Get the color at a x,y position
	ofColor_<PixelType> getColor(size_t x, size_t y) const;

	/// \brief Get the color at a specific index
	ofColor_<PixelType> getColor(size_t index) const;

	/// \brief Set the color of the pixel at the x,y location
	void setColor(size_t x, size_t y, const ofColor_<PixelType>& color);

	/// \brief Set the color of the pixel at a specific index
	void setColor(size_t index, const ofColor_<PixelType>& color);

	/// \brief Set the color of all pixels
	void setColor(const ofColor_<PixelType>& color);

	/// \brief Provides access to each channel of each pixel. If you have RGB pixel
	/// data, then you'll have 3 values for each pixel, if you have RGBA,
	/// you'll have 4
	const PixelType& operator[](size_t pos) const;
	PixelType& operator[](size_t pos);

	/// \}
	/// \name Getters
	/// \{

	/// \brief Get the width of the pixel array.
	size_t getWidth() const;

	/// \brief Get the height of the pixel array.
	size_t getHeight() const;

	/// \brief Get the number of bytes per pixel
	size_t getBytesPerPixel() const;

	/// \brief Get number of bits per pixel
	///
	/// If you have RGB pixel data, this will return 24, if you have RGBA,
	/// you'll have 32, if you have grayscale, this will return 8.
	size_t getBitsPerPixel() const;

	/// \brief Get how large each channel of a pixel is
	///
	/// ofPixels objects that store pixel data as unsigned char are smaller
	/// than  ofPixels objects that store pixel data as floats.
	///
	/// \note This returns bytes, not bits, so you should see ofPixels<float>
	/// return 4 and ofPixels<unsigned char> return 1.
	size_t getBytesPerChannel() const;

	/// \brief Get how large each channel of a pixels is.
	///
	/// ofPixels objects that store pixel data as `unsigned char` are smaller
	/// than ofPixels objects that store pixel data as 'float`.
	///
	/// \note This returns bits, not bytes, so you should see ofPixels<float>
	/// return 32 and ofPixels<unsigned char> return 8.
	size_t getBitsPerChannel() const;
	size_t getBytesStride() const;

	/// \brief Get the number of channels that the ofPixels object contains.
	/// RGB is 3 channels, RGBA is 4, and grayscale is 1.
	size_t getNumChannels() const;

	size_t getTotalBytes() const;

	size_t getNumPlanes() const;

	ofPixels_<PixelType> getPlane(size_t plane);

	/// \brief Get all values of one channel
	///
	/// For instance, the Red pixel values, from the
	/// ofPixels object, this gives you a grayscale representation of the
	/// specific channel
	///
	/// ~~~~{.cpp}
	///     // Get red pixels
	/// 	ofPixels rpix = pix.getChannel(0);
	///     // Get green pixels
	/// 	ofPixels gpix = pix.getChannel(1);
	///     // Get blue pixels
	/// 	ofPixels bpix = pix.getChannel(2);
	/// ~~~~
	ofPixels_<PixelType> getChannel(size_t channel) const;

	ofPixelFormat getPixelFormat() const;

	/// \brief Get the number of values that the ofPixels object
	/// contains, so an RGB data 400x400 would be 480,000, whereas RGBA data
	/// of the same dimensions would be 640,000.
	size_t size() const;

	/// \brief Get the type of the image
	/// \returns One of the following types: `OF_IMAGE_GRAYSCALE`,
	/// `OF_IMAGE_COLOR`, `OF_IMAGE_COLOR_ALPHA`
	ofImageType getImageType() const;

	/// \}
	/// \name Setters
	/// \{

	/// \brief Set all the pixel data for a single channel, for instance, the
	/// Red pixel values, from an ofPixels object assumed to be a grayscale
	/// representation of the data that should go into that one channel.
	void setChannel(size_t channel, const ofPixels_<PixelType> channelPixels);

	/// \brief Changes the image type for the ofPixels object
	///
	/// \param imageType Can be one of the following: OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA
	void setImageType(ofImageType imageType);

	void setNumChannels(size_t numChannels);

    static size_t pixelBitsFromPixelFormat(ofPixelFormat format);
    static size_t bytesFromPixelFormat(size_t w, size_t h, ofPixelFormat format);

	/// \}
	/// \name Iterator
	/// \{

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

	/// \}

    /// \cond INTERNAL

    struct ConstPixel: public std::iterator<std::forward_iterator_tag,ConstPixel>{
        ConstPixel(const PixelType * pixel, size_t bytesPerPixel, ofPixelFormat pixelFormat);
        const ConstPixel& operator*() const;
        const ConstPixel* operator->() const;
        ConstPixel& operator++();
        ConstPixel operator++(int);
        ConstPixel operator+(size_t) const;
        ConstPixel operator-(size_t) const;
        ConstPixel operator+=(size_t);
        bool operator!=(ConstPixel const& rhs) const;
        bool operator<(ConstPixel const& rhs) const;
        const PixelType & operator[](size_t idx) const;
        size_t getComponentsPerPixel() const;
        ofPixelFormat getPixelFormat() const;
        ofColor_<PixelType> getColor() const;

    private:
        const PixelType * pixel;
        size_t componentsPerPixel;
        ofPixelFormat pixelFormat;
    };

	struct Pixel: public std::iterator<std::forward_iterator_tag,Pixel>{
		Pixel(PixelType * pixel, size_t bytesPerPixel, ofPixelFormat pixelFormat);
        const Pixel& operator*() const;
        const Pixel* operator->() const;
        Pixel& operator++();
        Pixel operator++(int);
        Pixel& operator--();
        Pixel operator--(int);
        Pixel operator+(size_t) const;
        Pixel operator-(size_t) const;
        Pixel operator+=(size_t);
        bool operator!=(Pixel const& rhs) const;
        bool operator<(Pixel const& rhs) const;
        Pixel & operator=(Pixel const& rhs);
        Pixel & operator=(ConstPixel const& rhs);
        PixelType & operator[](size_t idx);
        const PixelType & operator[](size_t idx) const;
		size_t getComponentsPerPixel() const;
		ofPixelFormat getPixelFormat() const;
		ofColor_<PixelType> getColor() const;

	private:
		PixelType * pixel;
		size_t componentsPerPixel;
		ofPixelFormat pixelFormat;
	};

	struct Pixels{
		Pixels(PixelType * begin, PixelType * end, size_t componentsPerPixel, ofPixelFormat pixelFormat);
		Pixels(Pixel begin, Pixel end);
		Pixel begin();
		Pixel end();
	private:
		PixelType * _begin;
		PixelType * _end;
		size_t componentsPerPixel;
		ofPixelFormat pixelFormat;
	};

	struct Line: public std::iterator<std::forward_iterator_tag,Line>{
		Line(PixelType * _begin, size_t stride, size_t componentsPerPixel, size_t lineNum, ofPixelFormat pixelFormat);
        const Line& operator*() const;
        const Line* operator->() const;
        Line& operator++();
        Line operator++(int);
        Line& operator--();
        Line operator--(int);
        Line operator+(size_t) const;
        Line operator+=(size_t);
        bool operator!=(Line const& rhs) const;
        bool operator<(Line const& rhs) const;
        bool operator>(Line const& rhs) const;
        bool operator>=(Line const& rhs) const;
        PixelType * begin();
        PixelType * end();
        const PixelType * begin() const;
        const PixelType * end() const;
		size_t getLineNum() const;
		ofPixels_<PixelType> asPixels();
		const ofPixels_<PixelType> asPixels() const;
		size_t getStride() const;
		Pixel getPixel(size_t pixel);
		Pixels getPixels();
        Pixels getPixels(size_t first, size_t numPixels);

	private:
		PixelType * _begin;
		PixelType * _end;
		size_t stride;
		size_t componentsPerPixel;
		size_t lineNum;
		ofPixelFormat pixelFormat;
	};

	struct Lines{
		Lines(PixelType * _begin, PixelType * _end, size_t stride, size_t componentsPerPixel, size_t lines, ofPixelFormat pixelFormat);

        Line begin();

        Line end();


	private:
        PixelType * _begin;
		PixelType * _end;
		size_t stride;
		size_t componentsPerPixel;
		size_t lines;
		ofPixelFormat pixelFormat;
	};

	struct ConstPixels{
		ConstPixels(const PixelType * begin, const PixelType * end, size_t componentsPerPixel, ofPixelFormat pixelFormat);
		ConstPixels(const ConstPixel & begin, const ConstPixel & end);
		ConstPixel begin() const;
		ConstPixel end() const;
	private:
		const PixelType * _begin;
		const PixelType * _end;
		size_t componentsPerPixel;
		ofPixelFormat pixelFormat;
	};

	struct ConstLine: public std::iterator<std::forward_iterator_tag,Line>{
		ConstLine(const PixelType * _begin, size_t stride, size_t componentsPerPixel, size_t lineNum, ofPixelFormat pixelFormat);
		const ConstLine& operator*() const;
		const ConstLine* operator->() const;
		ConstLine& operator++();
		ConstLine operator++(int);
		ConstLine operator+(size_t) const;
		ConstLine operator+=(size_t);
		bool operator!=(ConstLine const& rhs) const;
		bool operator<(ConstLine const& rhs) const;
		const PixelType * begin() const;
		const PixelType * end() const;
		size_t getLineNum() const;
		size_t getStride() const;
        ConstPixel getPixel(size_t pixel) const;
		ConstPixels getPixels() const;
		ConstPixels getPixels(size_t first, size_t numPixels) const;

	private:
		const PixelType * _begin;
		const PixelType * _end;
		size_t stride;
		size_t componentsPerPixel;
		size_t lineNum;
		ofPixelFormat pixelFormat;
	};

	struct ConstLines{
		ConstLines(const PixelType * _begin, const PixelType * _end, size_t stride, size_t componentsPerPixel, size_t lines, ofPixelFormat pixelFormat);

		ConstLine begin() const;

		ConstLine end() const;

	private:
		const PixelType * _begin;
		const PixelType * _end;
		size_t stride;
		size_t componentsPerPixel;
		size_t lines;
		ofPixelFormat pixelFormat;
	};

	Line getLine(size_t line);
	Lines getLines();
    Lines getLines(size_t first, size_t numLines);
	Pixels getPixelsIter();
	ConstLine getConstLine(size_t line) const;
	ConstLines getConstLines() const;
	ConstLines getConstLines(size_t first, size_t numLines) const;
	ConstPixels getConstPixelsIter() const;

    /// \endcond

private:
	static float bicubicInterpolate(const float *patch, float x,float y, float x2,float y2, float x3,float y3);

	void copyFrom( const ofPixels_<PixelType>& mom );

	template<typename SrcType>
	void copyFrom( const ofPixels_<SrcType>& mom );

	PixelType * pixels = nullptr;
	size_t 	width = 0;
	size_t 	height = 0;

	//int 	channels; // 1, 3, 4 channels per pixel (grayscale, rgb, rgba)
	size_t 	pixelsSize = 0;
	bool	bAllocated = false;
	bool	pixelsOwner = true;			// if set from external data don't delete it
	ofPixelFormat pixelFormat = OF_PIXELS_UNKNOWN;

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
	pixels = nullptr;
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

		const float srcMax = ( (sizeof(SrcType) == sizeof(float) ) ? 1.f : std::numeric_limits<SrcType>::max() );
		const float dstMax = ( (sizeof(PixelType) == sizeof(float) ) ? 1.f : std::numeric_limits<PixelType>::max() );
		const float factor = dstMax / srcMax;

		if(sizeof(SrcType) == sizeof(float)) {
			// coming from float we need a special case to clamp the values
			for(size_t i = 0; i < mom.size(); i++){
				pixels[i] = ofClamp(mom[i], 0, 1) * factor;
			}
		} else{
			// everything else is a straight scaling
			for(size_t i = 0; i < mom.size(); i++){
				pixels[i] = mom[i] * factor;
			}
		}
	}
}
//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::iterator ofPixels_<PixelType>::begin(){
	return pixels;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::iterator ofPixels_<PixelType>::end(){
	return pixels + size();
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::const_iterator ofPixels_<PixelType>::begin() const{
	return pixels;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::const_iterator ofPixels_<PixelType>::end() const{
	return pixels + size();
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::reverse_iterator ofPixels_<PixelType>::rbegin(){
	return pixels + (size() - 1);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::reverse_iterator ofPixels_<PixelType>::rend(){
	return pixels - 1;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::const_reverse_iterator ofPixels_<PixelType>::rbegin() const{
	return pixels + (size() - 1);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::const_reverse_iterator ofPixels_<PixelType>::rend() const{
	return pixels - 1;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::Pixel::Pixel(PixelType * pixel, size_t componentsPerPixel, ofPixelFormat pixelFormat)
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
inline typename ofPixels_<PixelType>::Pixel& ofPixels_<PixelType>::Pixel::operator--(){
	pixel -= componentsPerPixel;
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel ofPixels_<PixelType>::Pixel::operator--(int){
	Pixel tmp(*this);
	operator--();
	return tmp;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel ofPixels_<PixelType>::Pixel::operator+(size_t i) const{
	return Pixel(pixel + componentsPerPixel * i, componentsPerPixel, pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel ofPixels_<PixelType>::Pixel::operator-(size_t i) const{
	return Pixel(pixel - componentsPerPixel * i, componentsPerPixel, pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel ofPixels_<PixelType>::Pixel::operator+=(size_t i){
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
inline typename ofPixels_<PixelType>::Pixel & ofPixels_<PixelType>::Pixel::operator=(Pixel const& rhs){
    for(size_t i=0;i<componentsPerPixel;++i){
        pixel[i] = rhs[i];
    }
    return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel & ofPixels_<PixelType>::Pixel::operator=(ConstPixel const& rhs){
    for(size_t i=0;i<componentsPerPixel;++i){
        pixel[i] = rhs[i];
    }
    return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline PixelType & ofPixels_<PixelType>::Pixel::operator[](size_t idx){
	return pixel[idx];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline const PixelType & ofPixels_<PixelType>::Pixel::operator[](size_t idx) const{
	return pixel[idx];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline size_t ofPixels_<PixelType>::Pixel::getComponentsPerPixel() const{
	return componentsPerPixel;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixelFormat ofPixels_<PixelType>::Pixel::getPixelFormat() const{
	return pixelFormat;
}

//----------------------------------------------------------------------
template<typename PixelType>
ofColor_<PixelType> ofPixels_<PixelType>::Pixel::getColor() const{
	ofColor_<PixelType> c;
	switch(pixelFormat){
		case OF_PIXELS_RGB:
			c.set( pixel[0], pixel[1], pixel[2] );
			break;
		case OF_PIXELS_BGR:
			c.set( pixel[2], pixel[1], pixel[0] );
			break;
		case OF_PIXELS_RGBA:
			c.set( pixel[0], pixel[1], pixel[2], pixel[3] );
			break;
		case OF_PIXELS_BGRA:
			c.set( pixel[2], pixel[1], pixel[0], pixel[3] );
			break;
		case OF_PIXELS_GRAY:
			c.set( pixel[0] );
			break;
		case OF_PIXELS_GRAY_ALPHA:
			c.set( pixel[0], pixel[0], pixel[0], pixel[1] );
			break;
		case OF_PIXELS_RGB565:
		case OF_PIXELS_NV12:
		case OF_PIXELS_NV21:
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
		case OF_PIXELS_YUY2:
		case OF_PIXELS_UYVY:
		case OF_PIXELS_Y:
		case OF_PIXELS_U:
		case OF_PIXELS_V:
		case OF_PIXELS_UV:
		case OF_PIXELS_VU:
		case OF_PIXELS_UNKNOWN:
		default:
			ofLogWarning() << "returning color not supported yet for " << ofToString(pixelFormat) << " format";
			return 0;
			break;
	}

	return c;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::Pixels::Pixels(PixelType * begin, PixelType * end, size_t componentsPerPixel, ofPixelFormat pixelFormat)
:_begin(begin)
,_end(end)
,componentsPerPixel(componentsPerPixel)
,pixelFormat(pixelFormat){}


//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::Pixels::Pixels(Pixel begin, Pixel end)
:_begin(&begin[0])
,_end(&end[0])
,componentsPerPixel(begin.getComponentsPerPixel())
,pixelFormat(begin.getPixelFormat()){}

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
inline ofPixels_<PixelType>::Line::Line(PixelType * _begin, size_t stride, size_t componentsPerPixel, size_t lineNum, ofPixelFormat pixelFormat)
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
inline typename ofPixels_<PixelType>::Line& ofPixels_<PixelType>::Line::operator--(){
	_end = _begin;
	_begin = _end - stride;
	--lineNum;
	return *this;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line ofPixels_<PixelType>::Line::operator--(int) {
	Line tmp(*this);
	operator--();
	return tmp;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line  ofPixels_<PixelType>::Line::operator+(size_t i) const{
	return Line(_begin+stride*i,stride,componentsPerPixel,lineNum+i,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Line  ofPixels_<PixelType>::Line::operator+=(size_t i){
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
	return _begin < rhs._begin;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::Line::operator>(typename ofPixels_<PixelType>::Line const& rhs) const{
	return _begin > rhs._begin;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline bool ofPixels_<PixelType>::Line::operator>=(typename ofPixels_<PixelType>::Line const& rhs) const{
	return _begin >= rhs._begin;
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
inline size_t ofPixels_<PixelType>::Line::getLineNum() const{
	return lineNum;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType> ofPixels_<PixelType>::Line::asPixels(){
	ofPixels_<PixelType> pixels;
	pixels.setFromExternalPixels(_begin,stride/componentsPerPixel,1,pixelFormat);
	return pixels;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline const ofPixels_<PixelType> ofPixels_<PixelType>::Line::asPixels() const{
	ofPixels_<PixelType> pixels;
	pixels.setFromExternalPixels(_begin,stride/componentsPerPixel,1,pixelFormat);
	return pixels;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline size_t ofPixels_<PixelType>::Line::getStride() const{
	return stride;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixel ofPixels_<PixelType>::Line::getPixel(size_t pixel){
    return Pixel(_begin + (pixel*componentsPerPixel), componentsPerPixel, pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixels ofPixels_<PixelType>::Line::getPixels(){
	return Pixels(_begin,_end,componentsPerPixel,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixels ofPixels_<PixelType>::Line::getPixels(size_t first, size_t numPixels){
    return Pixels(&getPixel(first)[0], &getPixel(first+numPixels)[0], componentsPerPixel, pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::Lines::Lines(PixelType * _begin, PixelType * _end, size_t stride, size_t componentsPerPixel, size_t lines, ofPixelFormat pixelFormat)
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
inline typename ofPixels_<PixelType>::Line ofPixels_<PixelType>::getLine(size_t line){
	return Line(pixels+(width*getNumChannels()*line), width*getNumChannels(), getNumChannels(), line,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Lines ofPixels_<PixelType>::getLines(){
	return Lines(begin(),end(),width*getNumChannels(),getNumChannels(),getHeight(),pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Lines ofPixels_<PixelType>::getLines(size_t first, size_t numLines){
    return Lines(getLine(first).begin(),getLine(first+numLines).begin(),width*getNumChannels(),getNumChannels(),numLines,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::Pixels ofPixels_<PixelType>::getPixelsIter(){
	return Pixels(begin(),end(),getNumChannels(),pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::ConstPixel::ConstPixel(const PixelType * pixel, size_t componentsPerPixel, ofPixelFormat pixelFormat)
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
inline typename ofPixels_<PixelType>::ConstPixel ofPixels_<PixelType>::ConstPixel::operator+(size_t i) const{
	return ConstPixel(pixel + componentsPerPixel * i, componentsPerPixel, pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixel ofPixels_<PixelType>::ConstPixel::operator-(size_t i) const{
	return ConstPixel(pixel - componentsPerPixel * i, componentsPerPixel, pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixel ofPixels_<PixelType>::ConstPixel::operator+=(size_t i){
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
inline const PixelType & ofPixels_<PixelType>::ConstPixel::operator[](size_t idx) const{
	return pixel[idx];
}

//----------------------------------------------------------------------
template<typename PixelType>
inline size_t ofPixels_<PixelType>::ConstPixel::getComponentsPerPixel() const{
	return componentsPerPixel;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixelFormat ofPixels_<PixelType>::ConstPixel::getPixelFormat() const{
	return pixelFormat;
}

//----------------------------------------------------------------------
template<typename PixelType>
ofColor_<PixelType> ofPixels_<PixelType>::ConstPixel::getColor() const{
	ofColor_<PixelType> c;
	switch(pixelFormat){
		case OF_PIXELS_RGB:
			c.set( pixel[0], pixel[1], pixel[2] );
			break;
		case OF_PIXELS_BGR:
			c.set( pixel[2], pixel[1], pixel[0] );
			break;
		case OF_PIXELS_RGBA:
			c.set( pixel[0], pixel[1], pixel[2], pixel[3] );
			break;
		case OF_PIXELS_BGRA:
			c.set( pixel[2], pixel[1], pixel[0], pixel[3] );
			break;
		case OF_PIXELS_GRAY:
			c.set( pixel[0] );
			break;
		case OF_PIXELS_GRAY_ALPHA:
			c.set( pixel[0], pixel[0], pixel[0], pixel[1] );
			break;
		case OF_PIXELS_RGB565:
		case OF_PIXELS_NV12:
		case OF_PIXELS_NV21:
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
		case OF_PIXELS_YUY2:
		case OF_PIXELS_UYVY:
		case OF_PIXELS_Y:
		case OF_PIXELS_U:
		case OF_PIXELS_V:
		case OF_PIXELS_UV:
		case OF_PIXELS_VU:
		case OF_PIXELS_UNKNOWN:
		default:
			ofLogWarning() << "returning color not supported yet for " << ofToString(pixelFormat) << " format";
			return 0;
			break;
	}

	return c;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::ConstPixels::ConstPixels(const PixelType * begin, const PixelType * end, size_t componentsPerPixel, ofPixelFormat pixelFormat)
:_begin(begin)
,_end(end)
,componentsPerPixel(componentsPerPixel)
,pixelFormat(pixelFormat){}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::ConstPixels::ConstPixels(const ConstPixel & begin, const ConstPixel & end)
:_begin(&begin[0])
,_end(&end[0])
,componentsPerPixel(begin.getComponentsPerPixel())
,pixelFormat(begin.getPixelFormat()){}

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
inline ofPixels_<PixelType>::ConstLine::ConstLine(const PixelType * _begin, size_t stride, size_t componentsPerPixel, size_t lineNum, ofPixelFormat pixelFormat)
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
inline typename ofPixels_<PixelType>::ConstLine  ofPixels_<PixelType>::ConstLine::operator+(size_t i) const{
	return ConstLine(_begin+stride*i,stride,componentsPerPixel,lineNum+i,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLine  ofPixels_<PixelType>::ConstLine::operator+=(size_t i){
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
inline size_t ofPixels_<PixelType>::ConstLine::getLineNum() const{
	return lineNum;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline size_t ofPixels_<PixelType>::ConstLine::getStride() const{
	return stride;
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixel ofPixels_<PixelType>::ConstLine::getPixel(size_t pixel) const{
    return ConstPixel(_begin + (pixel*componentsPerPixel), componentsPerPixel, pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixels ofPixels_<PixelType>::ConstLine::getPixels() const{
	return ConstPixels(_begin,_end,componentsPerPixel,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstPixels ofPixels_<PixelType>::ConstLine::getPixels(size_t first, size_t numPixels) const{
    return ConstPixels(&getPixel(first)[0], &getPixel(first+numPixels)[0], componentsPerPixel, pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline ofPixels_<PixelType>::ConstLines::ConstLines(const PixelType * _begin, const PixelType * _end, size_t stride, size_t componentsPerPixel, size_t lines, ofPixelFormat pixelFormat)
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
inline typename ofPixels_<PixelType>::ConstLine ofPixels_<PixelType>::getConstLine(size_t line) const{
	return ConstLine(pixels+(width*getNumChannels()*line), width*getNumChannels(), getNumChannels(), line,pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLines ofPixels_<PixelType>::getConstLines() const{
	return ConstLines(begin(),end(),width*getNumChannels(),getNumChannels(),getHeight(),pixelFormat);
}

//----------------------------------------------------------------------
template<typename PixelType>
inline typename ofPixels_<PixelType>::ConstLines ofPixels_<PixelType>::getConstLines(size_t first, size_t numLines) const{
    return ConstLines(getConstLine(first).begin(),getConstLine(first+numLines).begin(),width*getNumChannels(),getNumChannels(),numLines,pixelFormat);
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
