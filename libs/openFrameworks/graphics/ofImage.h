#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofPixels.h"
#include "ofGLBaseTypes.h"
#include "ofGraphicsConstants.h"
#include "ofGLUtils.h"

class ofFile;
class ofBuffer;


/// \file
/// ofImage is used for loading, saving and drawing images in openFrameworks.
/// ofImage is a convenient class that lets you both draw images to the screen
/// and manipulate their pixel data. The ofImage allows you to load an image
/// from disk, manipulate the pixels, and create an OpenGL texture that you
/// can display and manipulate on the graphics card. Loading a file into the
/// ofImage allocates an ofPixels object and creates the ofTexture to display
/// the pixels.
///

/// \brief Used for controlling the save quality of JPEGs.
///
/// These values are directly mapped to FreeImage's JPEG quality flags.
/// \sa http://freeimage.sourceforge.net/fnet/html/E77CD483.htm
enum ofImageQualityType {
    /// \brief Equivalent to FreeImage's JPEG_QUALITYSUPERB (100:1 ratio)
    OF_IMAGE_QUALITY_BEST,
    /// \brief Equivalent to FreeImage's JPEG_QUALITYGOOD (75:1 ratio)
    OF_IMAGE_QUALITY_HIGH,
    /// \brief Equivalent to FreeImage's JPEG_QUALITYNORMAL (50:1 ratio)
    OF_IMAGE_QUALITY_MEDIUM,
    /// \brief Equivalent to FreeImage's JPEG_QUALITYAVERAGE (25:1 ratio)
    OF_IMAGE_QUALITY_LOW,
    /// \brief Equivalent to FreeImage's JPEG_QUALITYBAD (10:1 ratio)
    OF_IMAGE_QUALITY_WORST
};

/// \todo Needs documentation.
enum ofImageFormat {
    OF_IMAGE_FORMAT_BMP     = 0,
    OF_IMAGE_FORMAT_ICO     = 1,
    OF_IMAGE_FORMAT_JPEG    = 2,
    OF_IMAGE_FORMAT_JNG     = 3,
    OF_IMAGE_FORMAT_KOALA   = 4,
    OF_IMAGE_FORMAT_LBM     = 5,
    OF_IMAGE_FORMAT_IFF = OF_IMAGE_FORMAT_LBM,
    OF_IMAGE_FORMAT_MNG     = 6,
    OF_IMAGE_FORMAT_PBM     = 7,
    OF_IMAGE_FORMAT_PBMRAW  = 8,
    OF_IMAGE_FORMAT_PCD     = 9,
    OF_IMAGE_FORMAT_PCX     = 10,
    OF_IMAGE_FORMAT_PGM     = 11,
	OF_IMAGE_FORMAT_PGMRAW  = 12,
    OF_IMAGE_FORMAT_PNG     = 13,
    OF_IMAGE_FORMAT_PPM     = 14,
    OF_IMAGE_FORMAT_PPMRAW  = 15,
    OF_IMAGE_FORMAT_RAS     = 16,
    OF_IMAGE_FORMAT_TARGA   = 17,
    OF_IMAGE_FORMAT_TIFF    = 18,
    OF_IMAGE_FORMAT_WBMP    = 19,
    OF_IMAGE_FORMAT_PSD     = 20,
    OF_IMAGE_FORMAT_CUT     = 21,
    OF_IMAGE_FORMAT_XBM     = 22,
    OF_IMAGE_FORMAT_XPM     = 23,
    OF_IMAGE_FORMAT_DDS     = 24,
    OF_IMAGE_FORMAT_GIF     = 25,
    OF_IMAGE_FORMAT_HDR     = 26,
    OF_IMAGE_FORMAT_FAXG3   = 27,
    OF_IMAGE_FORMAT_SGI     = 28,
    OF_IMAGE_FORMAT_EXR     = 29,
    OF_IMAGE_FORMAT_J2K     = 30,
    OF_IMAGE_FORMAT_JP2     = 31,
    OF_IMAGE_FORMAT_PFM     = 32,
    OF_IMAGE_FORMAT_PICT    = 33,
    OF_IMAGE_FORMAT_RAW     = 34
};

inline std::string ofImageFormatExtension(ofImageFormat format){
    switch(format){
    case OF_IMAGE_FORMAT_BMP: return "bmp";
    case OF_IMAGE_FORMAT_ICO: return "ico";
    case OF_IMAGE_FORMAT_JPEG: return "jpg";
    case OF_IMAGE_FORMAT_JNG: return "jng";
    case OF_IMAGE_FORMAT_KOALA: return "koala";
    case OF_IMAGE_FORMAT_LBM: return "lbm";
    case OF_IMAGE_FORMAT_MNG: return "mng";
    case OF_IMAGE_FORMAT_PBM: return "pbm";
    case OF_IMAGE_FORMAT_PBMRAW: return "pbm";
    case OF_IMAGE_FORMAT_PCD: return "pcd";
    case OF_IMAGE_FORMAT_PCX: return "pcx";
    case OF_IMAGE_FORMAT_PGM: return "pgm";
    case OF_IMAGE_FORMAT_PGMRAW: return "pgm";
    case OF_IMAGE_FORMAT_PNG: return "png";
    case OF_IMAGE_FORMAT_PPM: return "ppm";
    case OF_IMAGE_FORMAT_PPMRAW: return "ppm";
    case OF_IMAGE_FORMAT_RAS: return "ras";
    case OF_IMAGE_FORMAT_TARGA: return "tga";
    case OF_IMAGE_FORMAT_TIFF: return "tif";
    case OF_IMAGE_FORMAT_WBMP: return "wbmp";
    case OF_IMAGE_FORMAT_PSD: return "psd";
    case OF_IMAGE_FORMAT_CUT: return "cut";
    case OF_IMAGE_FORMAT_XBM: return "xbm";
    case OF_IMAGE_FORMAT_XPM: return "xpm";
    case OF_IMAGE_FORMAT_DDS: return "dds";
    case OF_IMAGE_FORMAT_GIF: return "gif";
    case OF_IMAGE_FORMAT_HDR: return "hdr";
    case OF_IMAGE_FORMAT_FAXG3: return "faxg3";
    case OF_IMAGE_FORMAT_SGI: return "sgi";
    case OF_IMAGE_FORMAT_EXR: return "exr";
    case OF_IMAGE_FORMAT_J2K: return "j2k";
    case OF_IMAGE_FORMAT_JP2: return "jp2";
    case OF_IMAGE_FORMAT_PFM: return "pfm";
    case OF_IMAGE_FORMAT_PICT: return "pict";
    case OF_IMAGE_FORMAT_RAW: return "raw";
	default: return "unkown";
    }
}


/// \todo Needs documentation.
struct ofImageLoadSettings {
	bool accurate = false;
	bool exifRotate = false;
	bool grayscale = false;
	bool separateCMYK = false;
};

//----------------------------------------------------
// FreeImage based stuff


/// \todo Needs documentation.
bool ofLoadImage(ofPixels & pix, const std::filesystem::path& path, const ofImageLoadSettings &settings = ofImageLoadSettings());
bool ofLoadImage(ofPixels & pix, const ofBuffer & buffer, const ofImageLoadSettings &settings = ofImageLoadSettings());
bool ofLoadImage(ofFloatPixels & pix, const std::filesystem::path& path, const ofImageLoadSettings &settings = ofImageLoadSettings());
bool ofLoadImage(ofFloatPixels & pix, const ofBuffer & buffer, const ofImageLoadSettings &settings = ofImageLoadSettings());
bool ofLoadImage(ofShortPixels & pix, const std::filesystem::path& path, const ofImageLoadSettings &settings = ofImageLoadSettings());
bool ofLoadImage(ofShortPixels & pix, const ofBuffer & buffer, const ofImageLoadSettings &settings = ofImageLoadSettings());

/// \todo Needs documentation.
bool ofLoadImage(ofTexture & tex, const std::filesystem::path& path, const ofImageLoadSettings &settings = ofImageLoadSettings());
bool ofLoadImage(ofTexture & tex, const ofBuffer & buffer, const ofImageLoadSettings &settings = ofImageLoadSettings());

/// \todo Needs documentation.
bool ofSaveImage(const ofPixels & pix, const std::filesystem::path& path, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);
bool ofSaveImage(const ofPixels & pix, ofBuffer & buffer, ofImageFormat format = OF_IMAGE_FORMAT_PNG, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

/// \todo Needs documentation.
bool ofSaveImage(const ofFloatPixels & pix, const std::filesystem::path& path, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);
bool ofSaveImage(const ofFloatPixels & pix, ofBuffer & buffer, ofImageFormat format = OF_IMAGE_FORMAT_PNG, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

/// \todo Needs documentation.
bool ofSaveImage(const ofShortPixels & pix, const std::filesystem::path& path, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);
bool ofSaveImage(const ofShortPixels & pix, ofBuffer & buffer, ofImageFormat format = OF_IMAGE_FORMAT_PNG, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

/// \brief Deallocates FreeImage resources.
///
/// Used internally during shutdown.
void ofCloseFreeImage();

/// \brief A class representing an image using memory and gpu based pixels.
/// \tparam PixelType The data type used to represent a single pixel value.
template<typename PixelType>
class ofImage_ : public ofBaseImage_<PixelType>{
public:
    /// \name Image Construction
    /// \{

    ofImage_();

    ofImage_(const ofPixels_<PixelType> & pix);
	ofImage_(const std::filesystem::path & fileName, const ofImageLoadSettings &settings = ofImageLoadSettings());
    ofImage_(const ofImage_<PixelType>& mom);
    ofImage_(ofImage_<PixelType>&& mom);

    template<typename SrcType>
    ofImage_(const ofImage_<SrcType>& mom);


    /// This allocates space in the ofImage, both the ofPixels and the
    /// ofTexture that the ofImage contains.
    ///
    /// You don't need to call this before loading an image, but for when you
    /// want to allocate. space ahead of when you are going to use the image.
    ///
    /// The types of images can be `OF_IMAGE_COLOR`, `OF_IMAGE_COLOR_ALPHA`
    /// or `OF_IMAGE_GRAYSCALE`.
    ///
    /// You need to call update() to update the texture after updating
    /// the pixels manually.
    ///
    /// \param w Width of image to allocate.
    /// \param h Height of image to allocate.
    /// \param type The ofImageType.
    void allocate(int w, int h, ofImageType type);

    /// \brief Whether the image has been allocated either by a call to
    /// allocate or by loading pixel data into the image.
    /// \returns true if the image has been allocated.
    bool isAllocated() const {return pixels.isAllocated();}

    /// \brief Whether the image has been allocated either by a call to
    /// allocate or by loading pixel data into the image.
    /// \deprecated Use isAllocated() instead
    /// \returns true if the image has been allocated.
    OF_DEPRECATED_MSG("Use isAllocated()", bool bAllocated());

    /// \brief This clears the texture and pixels contained within the ofImage.
	void clear();

    /// \brief Loads an image given by fileName.
    /// \param fileName Program looks for image given by fileName, relative to
    /// the data folder.
    /// \param settings Load options
    /// \returns true if image loaded correctly.
	bool load(const std::filesystem::path& fileName, const ofImageLoadSettings &settings = ofImageLoadSettings());

    /// \brief Loads an image from an ofBuffer instance created by, for
    /// instance, ofFile::readToBuffer().
    ///
    /// This actually loads the image data into an ofPixels object and then
    /// into the texture.
	bool load(const ofBuffer & buffer, const ofImageLoadSettings &settings = ofImageLoadSettings());

    OF_DEPRECATED_MSG("Use load instead",bool loadImage(const std::string& fileName));
    OF_DEPRECATED_MSG("Use load instead",bool loadImage(const ofBuffer & buffer));
    OF_DEPRECATED_MSG("Use load instead",bool loadImage(const ofFile & file));

    virtual ~ofImage_();

    /// \}
    /// \name Drawing
    /// \{

    using ofBaseDraws::draw;

    /// \brief Draw the image at it's normal size.
    ///
    /// \param x Draw position on the x axis.
    /// \param y Draw position on the y axis.
    void draw(float x, float y) const;

    /// \brief Draw the texture at it's normal size with depth.
    ///
    /// \param x Draw position on the x axis.
    /// \param y Draw position on the y axis.
    /// \param z Draw position on the z axis.
    void draw(float x, float y, float z) const;

	void draw(const glm::vec3 & pos) const;

    /// \brief Draw the image at a given size.
    ///
    /// \param x Draw position on the x axis.
    /// \param y Draw position on the y axis.
    /// \param w Draw width.
    /// \param h Draw height.
    void draw(float x, float y, float w, float h) const;

    /// \brief Draw the image at a given size with depth.
    ///
    /// \param x Draw position on the x axis.
    /// \param y Draw position on the y axis.
    /// \param z Draw position on the z axis.
    /// \param w Draw width.
    /// \param h Draw height.
    void draw(float x, float y, float z, float w, float h) const;

	void draw(const glm::vec3 & pos, float w, float h) const;

    /// \brief Draws a subsection of the image.
    ///
    /// This functions like a clipping mask. Does not altering any pixel
    /// data.
    ///
    /// \param x X position to draw cropped image at.
    /// \param y Y position to draw cropped image at.
    /// \param w Width of subsection to draw.
    /// \param h Height of subsection to draw.
    /// \param sx X position in image to begin cropping from.
    /// \param sy Y position in image to begin cropping from.
    void drawSubsection(float x, float y, float w, float h, float sx, float sy) const;

    /// \brief Draws a subsection of the image.
    ///
    /// This functions like a clipping mask. Does not altering any pixel
    /// data.
    ///
    /// \param x X position to draw cropped image at.
    /// \param y Y position to draw cropped image at.
    /// \param z Z position to draw cropped image at.
    /// \param w Width of subsection to draw.
    /// \param h Height of subsection to draw.
    /// \param sx X position in image to begin cropping from.
    /// \param sy Y position in image to begin cropping from.
    void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy) const;

    /// \brief Draws a subsection of the image.
    ///
    /// This functions like a clipping mask. Does not altering any pixel
    /// data.
    ///
    /// \param x X position to draw cropped image at.
    /// \param y Y position to draw cropped image at.
    /// \param w Width of subsection to draw.
    /// \param h Height of subsection to draw.
    /// \param sx X position in image to begin cropping from.
    /// \param sy Y position in image to begin cropping from.
    /// \param sw Source width of cropped area.
    /// \param sh Source height of cropped area.
    void drawSubsection(float x, float y, float w, float h, float sx, float sy, float sw, float sh) const;

    /// \brief Draws a subsection of the image.
    ///
    /// This functions like a clipping mask. Does not altering any pixel
    /// data.
    ///
    /// \param x X position to draw cropped image at.
    /// \param y Y position to draw cropped image at.
    /// \param z Z position to draw cropped image at.
    /// \param w Width of subsection to draw.
    /// \param h Height of subsection to draw.
    /// \param sx X position in image to begin cropping from.
    /// \param sy Y position in image to begin cropping from.
    /// \param sw Source width of cropped area.
    /// \param sh Source height of cropped area.
    void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;

    /// \}
    /// \name Texture
    /// \{

    /// \brief Call to ensure that changes to pixels are reflected in the ofTexture of the image.
    ///
    /// Many of the ofImage methods call this after they change the pixels,
    /// but if you directly manipulate  the pixels of the ofImage, then you
    /// should make sure to call update() before trying to draw the  texture
    /// of the image to the screen.
    void update();

    /// \brief Turns on or off the allocation and use of a texture.
    ///
    /// \param bUse Allocate and use a texture or not.
    void setUseTexture(bool bUse);

    /// \brief Returns whether the ofImage has a texture or not.
    ///
    /// If the ofImage doesn't have a texture, nothing will be drawn to the screen.
    /// \returns true if the ofImage is using a texture.
    bool isUsingTexture() const;

    /// \brief Returns a reference to the texture that the ofImage contains.
    ///
    /// You can use this to directly manipulate the texture itself, but keep in
    /// mind that if you manipulate the texture directly, there is no simple way
    /// to copy the data from the texture back to the pixels and keep the ofImage in sync.
    ///
    /// \returns A reference to the texture that the ofImage contains.
    ofTexture & getTexture();

	/// \brief Returns a const reference to the texture that the ofImage contains.
	/// \returns A const reference to the texture that the ofImage contains.
    const ofTexture & getTexture() const;

    OF_DEPRECATED_MSG("Use getTexture",ofTexture & getTextureReference());
    OF_DEPRECATED_MSG("Use getTexture",const ofTexture & getTextureReference() const);

    /// \brief Binds the oftexture instance that the ofImage contains so that
    /// it can be used for advanced drawing.
    void bind(int textureLocation=0) const;

    /// \brief Unbinds the ofTexture instance that the ofImage contains.
    ///
    /// Call this after you call bind().
    void unbind(int textureLocation=0) const;

    /// \brief This sets the compression level used when creating mipmaps for
    /// the ofTexture contained by the ofImage.
    /// \param compression The ofTexCompression to set.
    void setCompression(ofTexCompression compression);


    /// \}
    /// \name Pixel Getters
    /// \{

    /// \brief Returns a raw pointer to the pixel data.
    ///
    /// This function will give you access to a continuous block of pixels.
    /// you can grab the data and do what you like with it. If you have a
    /// grayscale image, you will have (width*height)  number of pixels. Color
    /// images will have (width*height*3) number of pixels (interlaced R,G,B),
    /// and coloralpha images will have (width*height*4) number of pixels
    /// (interlaced R,G,B,A).
    ///
    /// \warning This is a raw pointer. It's up to you to get this right.
    ///
    /// \returns A raw pointer to the pixel data.
    ofPixels_<PixelType> & getPixels();
    const ofPixels_<PixelType> & getPixels() const;

    /// \brief This returns an ofPixels reference that you can use to
    /// manipulate the raw pixel data of the ofImage.
    ///
    /// Make sure you call either update() after making changes to the ofPixels.
    ///
    /// \returns An ofPixels reference that you can use to manipulate the raw pixel data of the ofImage.
    OF_DEPRECATED_MSG("Use getPixels() instead ", ofPixels_<PixelType> & getPixelsRef());
    OF_DEPRECATED_MSG("Use getPixels() instead ", const ofPixels_<PixelType> & getPixelsRef() const);

    operator ofPixels_<PixelType>&();

    /// \brief This returns the ofColor representing the pixels at the x and y
    /// position passed in.
    /// \param x x position of pixel
    /// \param y y position of pixel
    /// \returns The ofColor representing the pixels at the x and y position passed in.
    ofColor_<PixelType> getColor(int x, int y) const;

	/// \brief This returns the ofColor representing the pixels at the index
	/// passed in.
	/// \param index index into pixel data
	/// \returns The ofColor representing the pixels at the index position passed in.
	ofColor_<PixelType> getColor(int index) const;

    /// \brief Get height of image as a float.
    /// \returns Height of image as float.
    float getHeight() const;

    /// \brief Get width of image as a float.
    /// \returns Width of image as float.
    float getWidth() const;

    /// \}
    /// \name Pixel Setters
    /// \{

    /// \brief Sets the pixel at the x,y position passed in.
    ///
    /// The ofColor type needs to match the ofImage type, i.e. ofFloatImage
    /// requires that you use ofFloatPixels.
    ///
    /// \param x x position of pixel.
    /// \param y y position of pixel.
    /// \param color Color to set pixel to.
    void setColor(int x, int y, const ofColor_<PixelType>& color);

    /// \brief Sets the pixel at the given pixel buffer index
    ///
    /// \param index Index of pixel to set.
    /// \param color Color to set pixel to.
    void setColor(int index, const ofColor_<PixelType>& color);

    /// \brief Sets all pixels to a color.
    /// \param color Color to set pixel to.
    void setColor(const ofColor_<PixelType>& color);

    /// \brief Sets the pixels of the image from an array of values.
    ///
    /// Set the pixels of the image from an array of values, for an
    /// ofFloatImage these need to be floats, for an ofImage  these need to be
    /// unsigned chars. The w and h values are important so that the correct
    /// dimensions are set in the  image. This assumes that you're setting the
    /// pixels from 0,0 or the upper left hand corner of the image. The
    /// bOrderIsRGB flag allows you pass in pixel data that is BGR by setting
    /// bOrderIsRGB=false.
    ///
    /// \param pixels Array of pixel values.
    /// \param w Width of image being passed in.
    /// \param h Height of image being passed in.
    /// \param type The image type can be OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, or OF_IMAGE_COLOR_ALPHA.
    /// \param bOrderIsRGB Pass in pixel data that is BGR by setting bOrderIsRGB=false.
    void setFromPixels(const PixelType * pixels, int w, int h, ofImageType type, bool bOrderIsRGB = true);

    /// \brief Set the pixels of the image from an ofPixels instance.
    ///
    /// Set the pixels of the image from an ofPixels instance, for an
    /// ofFloatImage these need to be ofFloatPixels, for an ofImage these need
    /// to be unsigned chars.
    void setFromPixels(const ofPixels_<PixelType> & pixels);

    /// \brief Grabs pixels from the opengl window specified by the region
    /// (x, y, w, h) and turns them into an image.
    ///
    /// It resizes or allocates the ofImage if it's necessary.
    ///
    /// \warning Uses glReadPixels() which can be slow.
    ///
    /// \param x x position of upper-left corner of region.
    /// \param y y position of upper-left corner of region.
    /// \param w Width of region.
    /// \param h Height of region.
    void grabScreen(int x, int y, int w, int h);

    /// \brief Set type of image to one of the following: OF_IMAGE_GRAYSCALE,
    /// OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA
    ///
    /// This does cause the image to be reallocated and the texture to be
    /// updated, so it can be an expensive operation  if done frequently.
    /// Converting down, for example from color to grayscale, loses
    /// information and is a destructive change.
    ///
    /// \param type The type of image, one of the following:
    /// `OF_IMAGE_GRAYSCALE`, `OF_IMAGE_COLOR`, `OF_IMAGE_COLOR_ALPHA`
    void setImageType(ofImageType type);
    ofImageType getImageType() const;

    /// \}
    /// \name Modifiers
    /// \{

    /// \brief Resizes the image to a new size (w, h); Can be used to scale up
    /// or down an image.
    ///
    /// \param newWidth New width of image.
    /// \param newHeight New height of image.
    void resize(int newWidth, int newHeight);

    /// \brief This crops the image to the w,h passed in from the x,y position.
    ///
    /// This does an in place crop and **allocates memory**.
    ///
    /// \param x x position of upper-left corner of region to crop.
    /// \param y y position of upper-left corner of region to crop.
    /// \param w Width of region to crop.
    /// \param h Height of region to crop.
    void crop(int x, int y, int w, int h);

    /// \brief Replaces region in caller image specified by w,h,x,y with pixels from otherImage.
    ///
    /// The w,h are measured from the x,y, so passing 100, 100, 300, 300 will grab
    /// a 300x300 pixel block of data starting from 100, 100.
    ///
    /// \param otherImage Image to crop from.
    /// \param x x position of upper-left corner of region to crop.
    /// \param y y position of upper-left corner of region to crop.
    /// \param w Width of region to crop.
    /// \param h Height of region to crop.
    void cropFrom(const ofImage_<PixelType>& otherImage, int x, int y, int w, int h);

    /// \brief Rotates the image by a multiple of 90 degrees.
    /// \param rotation Amount to rotate in multiples of 90. For instance, if you pass
    /// in 2, then the image will be rotated 180 degrees.
    void rotate90(int rotation);

    /// \brief This reflects the pixels of the image across the vertical and/or horizontal axis.
    /// \param vertical Set to true to reflect image across vertical axis.
    /// \param horizontal Set to true to reflect image across horizontal axis.
    void mirror(bool vertical, bool horizontal);

    /// \}
    /// \name Anchor
    /// \{

    /// \brief Change the drawing anchor from top-left corner to a position
    /// specified by xPct and yPct.
    ///
    /// Changes the drawing position specified by draw() from the normal top-
    /// left corner of the image to a position specified by xPct and yPct in
    /// relation to the dimensions of the image. This can be useful  for
    /// aligning and centering images as well as rotating an image around its
    /// center. Note: range of  xPct and yPct is 0.0 to 1.0. For xPct, 1.0
    /// represents the width of the image. For yPct, 1.0 represents  the
    /// height of the image. These values are not capped.
    ///
    /// \param xPct X position of the new anchor, specified as a percent of the width of the image.
    /// \param yPct Y position of the new anchor, specified as a percent of the height of the image.
    void setAnchorPercent(float xPct, float yPct);

    /// \brief Changes drawing position from top-left corner to position specified by x,y.
    ///
    /// Changes the drawing position specified by draw() from the normal top-
    /// left corner of the image to a  position specified by x and y, measured
    /// in pixels. This can be useful for aligning and centering  images as
    /// well as rotating an image around its center.
    ///
    /// \param x X position of the new anchor.
    /// \param y Y position of the new anchor.
    void setAnchorPoint(float x, float y);

    /// \brief Removes anchor positioning.
    ///
    /// Resets the anchor to (0, 0) so the image will be drawn from its
    /// upper left hand corner.
    void resetAnchor();

    /// \}

    /// \name Saving
    /// \{


    /// \brief Saves the image to the file path in fileName with the image
    /// quality specified by compressionLevel.
    ///
    /// \param fileName Saves image to this path, relative to the data folder.
    /// \param compressionLevel The ofImageQualityType.
	bool save(const std::filesystem::path & fileName, ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST) const;

    /// \brief This saves the image to the ofBuffer passed with the image
    /// quality specified by compressionLevel.
    ///
    /// \param buffer ofBuffer to save image to.
    /// \param compressionLevel The ofImageQualityType.
	bool save(ofBuffer & buffer, ofImageFormat imageFormat = OF_IMAGE_FORMAT_PNG, ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST) const;

    OF_DEPRECATED_MSG("Use save instead",void saveImage(const std::string& fileName, ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST) const);
    OF_DEPRECATED_MSG("Use save instead",void saveImage(ofBuffer & buffer, ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST) const);
    OF_DEPRECATED_MSG("Use save instead",void saveImage(const ofFile & file, ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST) const);

    /// \}
    /// \name Operators
    /// \{

    ofImage_<PixelType> & operator=(ofPixels_<PixelType> & pixels);

    // default copy overwriting (for = or std::vector)
    ofImage_<PixelType>& operator= (const ofImage_<PixelType>& mom);

    template<typename SrcType>
    ofImage_<PixelType>& operator= (const ofImage_<SrcType>& mom);

    //move assignment
    ofImage_<PixelType>& operator=(ofImage_<PixelType>&& mom);

    /// \}
    ///< \sa ofImageType

protected:
    /// \cond INTERNAL
    void changeTypeOfPixels(ofPixels_<PixelType> &pix, ofImageType type);
    void resizePixels(ofPixels_<PixelType> &pix, int newWidth, int newHeight);
    void unloadTexture();

    ofPixels_<PixelType> pixels;
    bool bUseTexture;
    ofTexture tex;

    int width;  ///< \brief Image width in pixels.
    int height; ///< \brief Image Height in pixels.
    int bpp;    ///< \brief Bits per image pixel.
    ofImageType type;   ///< \brief Image type.

	template<typename SrcType>
	void clone(const ofImage_<SrcType> &mom);
    /// \endcond
};

/// \name Variants
/// \{
typedef ofImage_<unsigned char> ofImage;
typedef ofImage_<float> ofFloatImage;
typedef ofImage_<unsigned short> ofShortImage;
/// \}



//----------------------------------------------------------
template<typename PixelType>
template<typename SrcType>
ofImage_<PixelType>& ofImage_<PixelType>::operator=(const ofImage_<SrcType>& mom) {
	clone(mom);
    return *this;
}

//----------------------------------------------------------
template<typename PixelType>
template<typename SrcType>
ofImage_<PixelType>::ofImage_(const ofImage_<SrcType>& mom) {
    clear();
	clone(mom);
}

//------------------------------------
template<typename PixelType>
template<typename SrcType>
void ofImage_<PixelType>::clone(const ofImage_<SrcType> &mom){
    pixels = mom.getPixels();

    tex.clear();
    bUseTexture = mom.isUsingTexture();
    if (bUseTexture == true && mom.getTexture().isAllocated()){
        tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGLInternalFormat(pixels));
    }

    update();
}


template<typename T>
std::string ofToString(const T & v);
template<>
std::string ofToString(const ofImageType & imgType);
