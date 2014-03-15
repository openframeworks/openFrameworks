#pragma once


#include "ofFileUtils.h"
#include "ofTexture.h"
#include "ofPixels.h"
#include "ofBaseTypes.h"
#include "ofConstants.h"

enum ofImageQualityType {
	OF_IMAGE_QUALITY_BEST,
	OF_IMAGE_QUALITY_HIGH,
	OF_IMAGE_QUALITY_MEDIUM,
	OF_IMAGE_QUALITY_LOW,
	OF_IMAGE_QUALITY_WORST
};

enum ofImageFormat {
	OF_IMAGE_FORMAT_BMP		= 0,
	OF_IMAGE_FORMAT_ICO		= 1,
	OF_IMAGE_FORMAT_JPEG	= 2,
	OF_IMAGE_FORMAT_JNG		= 3,
	OF_IMAGE_FORMAT_KOALA	= 4,
	OF_IMAGE_FORMAT_LBM		= 5,
	OF_IMAGE_FORMAT_IFF = OF_IMAGE_FORMAT_LBM,
	OF_IMAGE_FORMAT_MNG		= 6,
	OF_IMAGE_FORMAT_PBM		= 7,
	OF_IMAGE_FORMAT_PBMRAW	= 8,
	OF_IMAGE_FORMAT_PCD		= 9,
	OF_IMAGE_FORMAT_PCX		= 10,
	OF_IMAGE_FORMAT_PGM		= 11,
	OF_IMAGE_FORMAT_PGMRAW	= 12,
	OF_IMAGE_FORMAT_PNG		= 13,
	OF_IMAGE_FORMAT_PPM		= 14,
	OF_IMAGE_FORMAT_PPMRAW	= 15,
	OF_IMAGE_FORMAT_RAS		= 16,
	OF_IMAGE_FORMAT_TARGA	= 17,
	OF_IMAGE_FORMAT_TIFF	= 18,
	OF_IMAGE_FORMAT_WBMP	= 19,
	OF_IMAGE_FORMAT_PSD		= 20,
	OF_IMAGE_FORMAT_CUT		= 21,
	OF_IMAGE_FORMAT_XBM		= 22,
	OF_IMAGE_FORMAT_XPM		= 23,
	OF_IMAGE_FORMAT_DDS		= 24,
	OF_IMAGE_FORMAT_GIF     = 25,
	OF_IMAGE_FORMAT_HDR		= 26,
	OF_IMAGE_FORMAT_FAXG3	= 27,
	OF_IMAGE_FORMAT_SGI		= 28,
	OF_IMAGE_FORMAT_EXR		= 29,
	OF_IMAGE_FORMAT_J2K		= 30,
	OF_IMAGE_FORMAT_JP2		= 31,
	OF_IMAGE_FORMAT_PFM		= 32,
	OF_IMAGE_FORMAT_PICT	= 33,
	OF_IMAGE_FORMAT_RAW		= 34
};

//----------------------------------------------------
// FreeImage based stuff:
bool ofLoadImage(ofPixels & pix, string path);
bool ofLoadImage(ofPixels & pix, const ofBuffer & buffer);

bool ofLoadImage(ofFloatPixels & pix, string path);
bool ofLoadImage(ofFloatPixels & pix, const ofBuffer & buffer);

bool ofLoadImage(ofShortPixels & pix, string path);
bool ofLoadImage(ofShortPixels & pix, const ofBuffer & buffer);

bool ofLoadImage(ofTexture & tex, string path);
bool ofLoadImage(ofTexture & tex, const ofBuffer & buffer);

void ofSaveImage(ofPixels & pix, string path, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);
void ofSaveImage(ofPixels & pix, ofBuffer & buffer, ofImageFormat format = OF_IMAGE_FORMAT_PNG, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

void ofSaveImage(ofFloatPixels & pix, string path, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);
void ofSaveImage(ofFloatPixels & pix, ofBuffer & buffer, ofImageFormat format = OF_IMAGE_FORMAT_PNG, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

void ofSaveImage(ofShortPixels & pix, string path, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);
void ofSaveImage(ofShortPixels & pix, ofBuffer & buffer, ofImageFormat format = OF_IMAGE_FORMAT_PNG, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

// when we exit, we shut down ofImage
void ofCloseFreeImage();

//----------------------------------------------------
template<typename PixelType>
class ofImage_ : public ofBaseImage_<PixelType>{

	public :

		ofImage_();
		virtual ~ofImage_();

		ofImage_(const ofPixels_<PixelType> & pix);
		ofImage_(const ofFile & file);
		ofImage_(const string & filename);

		// allocation / deallocation routines
		/// \brief This allocates space in the ofImage, both the ofPixels and the ofTexture that the ofImage contains.
		/// 
		/// You don't need to call this before loading an image, but for when you want to allocate 
		/// space ahead of when you are going to use the image.
		///
		/// \param w Width of image to allocate.
		/// \param h Height of image to allocate.
		/// \param type The type can be of three types: OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA.
		void 				allocate(int w, int h, ofImageType type);
		/// \brief This clears the texture and pixels contained within the ofImage.
		void 				clear();

		// default copy overwriting (for = or std::vector)
		ofImage_(const ofImage_<PixelType>& mom);
		ofImage_<PixelType>& operator= (const ofImage_<PixelType>& mom);

		template<typename SrcType>
		ofImage_(const ofImage_<SrcType>& mom);

		template<typename SrcType>
		ofImage_<PixelType>& operator= (const ofImage_<SrcType>& mom);

		// copying:
		template<typename SrcType>
		/// \brief Create an ofImage from another ofImage instance.
		///
		/// This allows you to create an ofImage from another ofImage instance, 
		/// copying all the pixels and the texture data while creating a new textureID.
		void 				clone(const ofImage_<SrcType> &mom);

		// enable or disable using the texture of this image
		/// \brief Turns on or off the allocation and use of a texture. 
		///
		/// \param bUse Allocate and use a texture or not 
		void 				setUseTexture(bool bUse);
		/// \brief Returns whether the ofImage has a texture or not. 
		///
		/// If the ofImage doesn't have a texture, nothing will be drawn to the screen.
		/// \returns Returns whether the ofImage has a texture or not. 
		bool				isUsingTexture();

		// for getting a reference to the texture
		/// \brief Returns the texture reference that the ofImage contains. 
		///
		/// You can use this to directly manipulate the texture itself, but keep in 
		/// mind that if you manipulate the texture directly, there is no simple way 
		/// to copy the data from the texture back to the pixels and keep the ofImage in sync.
		///
		/// \returns Returns the texture reference that the ofImage contains. 
		ofTexture & getTextureReference();

		// quick texture binding shortcut
		/// \brief Binds the oftexture instance that the ofImage contains so that it can be used for advanced drawing.
		void bind();
		/// \brief Unbinds the ofTexture instance that the ofImage contains. 
		///
		/// Call this after you call bind().
		void unbind();

		// file loading / saving
		/// \brief Loads an image given by fileName.
		/// \param fileName Program looks for image given by fileName, relative to the data folder.
		/// \returns Returns true if image loaded correctly.
		bool 				loadImage(string fileName);
		/// \brief Loads an image from an ofBuffer instance created by, for instance, ofFile::readToBuffer(). 
		///
		/// This actually loads the image data into an ofPixels object and then into the texture.
		bool				loadImage(const ofBuffer & buffer);
		/// \brief Loads an image from an ofFile instance created by, for instance, ofDirectory::getFiles().
		///
		/// This actually loads the image data into an ofPixels object and then into the texture.
		bool 				loadImage(const ofFile & file);
		/// \brief Saves the image to the file path in fileName with the image quality specified by compressionLevel.
		/// \param fileName Saves image to this path, relative to the data folder.
		/// \param compressionLevel The different compression levels are: OF_IMAGE_QUALITY_BEST, OF_IMAGE_QUALITY_HIGH, 
		/// \param OF_IMAGE_QUALITY_MEDIUM, OF_IMAGE_QUALITY_LOW, OF_IMAGE_QUALITY_WORST
		void 				saveImage(string fileName, ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST);
		/// \brief This saves the image to the ofBuffer passed with the image quality specified by compressionLevel. 
		/// \param buffer ofBuffer to save image to
		/// \param compressionLevel The different compression levels are: OF_IMAGE_QUALITY_BEST, OF_IMAGE_QUALITY_HIGH, 
		/// \param OF_IMAGE_QUALITY_MEDIUM, OF_IMAGE_QUALITY_LOW, OF_IMAGE_QUALITY_WORST
		void 				saveImage(ofBuffer & buffer, ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST);
		/// \brief This saves the image to the ofFile passed with the image quality specified by compressionLevel. 
		/// \param file ofFile to save image to
		/// \param compressionLevel The different compression levels are: OF_IMAGE_QUALITY_BEST, OF_IMAGE_QUALITY_HIGH, 
		/// \param OF_IMAGE_QUALITY_MEDIUM, OF_IMAGE_QUALITY_LOW, OF_IMAGE_QUALITY_WORST
		void 				saveImage(const ofFile & file, ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST);
		// Sosolimited: texture compression and mipmaps
		/// \brief This sets the compression level used when creating mipmaps for the ofTexture contained by the ofImage. 
		/// \param compression The different texture compression levels are: OF_COMPRESS_NONE, 
		/// \param OF_COMPRESS_SRGB, OF_COMPRESS_ARB
		void				setCompression(ofTexCompression compression);

		// getting the data
		/// \brief Returns a raw pointer to the pixel data.
		///
		/// This function will give you access to a continuous block of pixels. you can grab the data
		/// and do what you like with it. If you have a grayscale image, you will have (widthheight) 
		/// number of pixels. Color images will have (widthheight3) number of pixels (interlaced R,G,B), 
		/// and coloralpha images will have (widthheight*4) number of pixels (interlaced R,G,B,A).
		///
		/// \returns Returns a raw pointer to the pixel data.
		PixelType * 				getPixels();			// up to you to get this right
		/// \brief This returns an ofPixels reference that you can use to manipulate the raw pixel data of the ofImage.
		/// 
		/// Make sure you call either update() or reloadTexture() after making changes to the ofPixels.
		///
		/// \returns Returns an ofPixels reference that you can use to manipulate the raw pixel data of the ofImage.
		ofPixels_<PixelType> &	 	getPixelsRef();

		operator ofPixels_<PixelType>&();
		
		ofColor_<PixelType> getColor(int x, int y) const;

		// alter the image
		void				setColor(int x, int y, const ofColor_<PixelType>& color);
		void				setColor(int index, const ofColor_<PixelType>& color);
		void				setColor(const ofColor_<PixelType>& color);
		void 				setFromPixels(const PixelType * pixels, int w, int h, ofImageType type, bool bOrderIsRGB = true);
		void				setFromPixels(const ofPixels_<PixelType> & pixels);
		void 				setImageType(ofImageType type);
		void 				resize(int newWidth, int newHeight);
		void 				grabScreen(int x, int y, int w, int h);		// grab pixels from opengl, using glreadpixels
		// this does an inplace crop. 
		// NOTE: this reallocates memory.
		void				crop(int x, int y, int w, int h); 
		// this does a crop from another image.
		// NOTE: this will reallocate memory if the image types are different, or if the w & h do not
		// equal this images w & h
		void				cropFrom(ofImage_<PixelType> & otherImage, int x, int y, int w, int h);
		// perform rotation of 90 degress clockwise rotation amont times. 
		void				rotate90(int rotation);
		void				mirror(bool vertical, bool horizontal); 
	
	
	
		// if you've altered the pixels (e.g., from getPixels())
		// call update() to see a change (move the pixels to the texture)
		void update();

		// the anchor is the point the image is drawn around.
		// this can be useful if you want to rotate an image around a particular point.
        void				setAnchorPercent(float xPct, float yPct);	// set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
        void				setAnchorPoint(float x, float y);			// set the anchor point in pixels
        void				resetAnchor();								// resets the anchor to (0, 0)

		// draw:
		using ofBaseDraws::draw;
		void draw(float x, float y);
		void draw(float x, float y, float z);
		void draw(float x, float y, float w, float h);
		void draw(float x, float y, float z, float w, float h);
		
		void drawSubsection(float x, float y, float w, float h, float sx, float sy);
		void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy);
		void drawSubsection(float x, float y, float w, float h, float sx, float sy, float sw, float sh);
		void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh);

		/// \brief Returns height of image as a float. 
		/// \returns float
		float 				getHeight();
		float 				getWidth();
		
		bool 				bAllocated() {return pixels.isAllocated();};	// legacy function
		bool 				isAllocated() {return pixels.isAllocated();};

		void				reloadTexture();

		int 				width, height, bpp;		// w,h, bits per pixel
		int					type;					// OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA

		ofImage_<PixelType> & operator=(ofPixels_<PixelType> & pixels);
	protected:
	
		void				changeTypeOfPixels(ofPixels_<PixelType> &pix, ofImageType type);
		void				resizePixels(ofPixels_<PixelType> &pix, int newWidth, int newHeight);

		ofPixels_<PixelType>		pixels;
		bool						bUseTexture;
		ofTexture					tex;

};

typedef ofImage_<unsigned char> ofImage;
typedef ofImage_<float> ofFloatImage;
typedef ofImage_<unsigned short> ofShortImage;




//----------------------------------------------------------
template<typename PixelType>
template<typename SrcType>
ofImage_<PixelType>& ofImage_<PixelType>::operator=(const ofImage_<SrcType>& mom) {
	clone(mom);
	update();
	return *this;
}

//----------------------------------------------------------
template<typename PixelType>
template<typename SrcType>
ofImage_<PixelType>::ofImage_(const ofImage_<SrcType>& mom) {
	clear();
	clone(mom);
	update();
}

//------------------------------------
template<typename PixelType>
template<typename SrcType>
void ofImage_<PixelType>::clone(const ofImage_<SrcType> &mom){
	ofImage_<SrcType> & nonConst = const_cast<ofImage_<SrcType> & >(mom);
	pixels = nonConst.getPixelsRef();

	tex.clear();
	bUseTexture = nonConst.isUsingTexture();
	if (bUseTexture == true && nonConst.getTextureReference().isAllocated()){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
	}

	update();
}
