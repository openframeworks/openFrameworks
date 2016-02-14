#pragma once

#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofBaseTypes.h"
#include "ofConstants.h"
#include "ofVboMesh.h"

/// \file
/// ofTexture is used to create OpenGL textures that live on your graphics card
/// (GPU). While you can certainly use ofTexture directly to manipulate and
/// textures, more often ofTexture will be used internally by classes like
/// ofImage or ofVideoGrabber.  That said, ofImage and other classes that use
/// ofTexture internally often provide access to the ofTexture.
///
/// ofTexture allows use of non-power of 2 textures in OpenGL and has a set
/// of functions simplify the process of transfering pixel data to and from
/// the GPU in various formats.
///
/// Users must _allocate_ the texture before drawing it or loading data into it.



/// \name Global texture settings
/// \{

/// \brief Check whether OF is using GL_TEXTURE_RECTANGLE rectangular or GL_TEXTURE_2D textures.
/// \sa ofEnableArbTex()
/// \returns true if using GL_TEXTURE_RECTANGLE textures, false if using GL_TEXTURE_2D textures.
bool ofGetUsingArbTex();

/// \brief Use GL_TEXTURE_RECTANGLE textures.
///
/// GL_TEXTURE_RECTANGLE textures are more intuitive since they allow pixel
/// based coordinates and are enabled by default.
///
/// GL_TEXTURE_2D textures use normalised texture coordinates (a float value
/// between 0 and 1 is used to express texture coordinates along width and
/// height).
///
/// GL_TEXTURE_2D textures are more widely supported and allow advanced features
/// such as mipmaps and texture compression.
///
/// \sa ofDisableArbTex()
/// \warning GL_TEXTURE_RECTANGLE is not available in OpenGL ES.
/// \warning GL_TEXTURE_RECTANGLE does not support mipmaps.
void ofEnableArbTex();

/// \brief Use GL_TEXTURE_2D textures.
///
/// GL_TEXTURE_2D is OpenGL's default way of handling textures and supports a
/// wider range of core OpenGL features such as mipmaps.
///
/// \sa ofEnableArbTex()
void ofDisableArbTex();

/// \brief Check whether OF is using normalized or pixel texture coordinates.
/// \sa ofEnableNormalizedTexCoords()
/// \returns true if using normalized (0 - 1) texture coordinates.
bool ofGetUsingNormalizedTexCoords();

/// \brief Use normalized (0 - 1) texture coordinates.
///
/// By default, textures in OF work with pixel-based coordinates based on given
/// content dimensions (images size, fbo size, etc). Normalized (0 - 1)
/// coordinates are the traditional OpenGL default as they allow working with
/// internal texture data without having to worry about specific dimensions of
/// the content.
///
/// This overrides individual ofTexture wrap settings.
///
/// \sa http://www.opengl.org/sdk/docs/man4/html/glTexParameter.xhtml
void ofEnableNormalizedTexCoords();

/// \brief Use pixel-based texture coordinates.
/// \sa ofEnableNormalizedTexCoords()
void ofDisableNormalizedTexCoords();

/// \brief Set custom global texture wrapping.
///
/// By default, textures are clamped to their edges with GL_CLAMP_TO_EDGE.
/// Setting a repeat mode like GL_REPEAT allows you to create tiled backgrounds
/// with small textures.
///
/// This overrides individual ofTexture wrap settings.
///
/// \sa ofTexture::setTextureWrap
/// \sa http://www.opengl.org/sdk/docs/man4/html/glTexParameter.xhtml
///
/// \warning Deprecated. Use member methods instead.
///
/// \param wrapS wrap parameter for texture coordinate s.
/// \param wrapT wrap parameter for texture coordinate t.
OF_DEPRECATED_MSG("Use member method ofTexture::setTextureWrap() instead.",void ofSetTextureWrap(GLfloat wrapS = GL_CLAMP_TO_EDGE, GLfloat wrapT = GL_CLAMP_TO_EDGE));

/// \brief Check whether OF is using custom global texture wrapping.
///
/// \warning Deprecated. Use member methods instead.
///
/// \sa ofSetTextureWrap()
/// \returns true if OF is currently using custom global texture wrapping. 
OF_DEPRECATED_MSG("Use member method ofTexture::setTextureWrap() instead.",bool ofGetUsingCustomTextureWrap());

/// \brief Removes global custom texture wrapping.
///
/// Restores individual ofTexture wrap settings.
///
/// \warning Deprecated. Use member methods instead.
///
/// \sa ofSetTextureWrap()
OF_DEPRECATED_MSG("Use member method ofTexture::setTextureWrap() instead.",void ofRestoreTextureWrap());

/// \brief Set custom global texture minification/magnification scaling filters.
///
/// This setting allows global control over how OpenGL scales textures. It
/// overrides individual ofTexture min & mag filter settings.
///
/// \warning Deprecated. Use member methods instead.
///
/// \sa ofTexture::setTextureMinMagFilter()
/// \param minFilter minifying filter for scaling a pixel to a smaller area.
/// \param magFilter magnifying filter for scaling a pixel to a larger area.
OF_DEPRECATED_MSG("Use member method ofTexture::setTextureMinMagFilter() instead.",void ofSetMinMagFilters(GLfloat minFilter = GL_LINEAR, GLfloat magFilter = GL_LINEAR));

/// \brief Check whether OF is using custom global texture scaling filters.
/// \returns true if OF is currently using custom texture scaling filters.
/// \warning Deprecated. Use member methods instead.
OF_DEPRECATED_MSG("Use member method ofTexture::setTextureMinMagFilter() instead.",bool ofGetUsingCustomMinMagFilters());

/// \brief Removes global custom texture wrapping.
///
/// Restores individual ofTexture min mag filter settings.
/// \warning Deprecated. Use member methods instead.
OF_DEPRECATED_MSG("Use member method ofTexture::setTextureMinMagFilter() instead.",void ofRestoreMinMagFilters());

/// \}

/// \brief Texture compression types.
///
/// Compression is only available through OpenGL for textures using
/// GL_TEXTURE_2D, *not* GL_TEXTURE_RECTANGLE, also note that most compression
/// algorithms work on blocks of 4x4 pixels, and therefore expect compressed
/// textures to have multiple-of-four dimensions.
enum ofTexCompression {
	OF_COMPRESS_NONE,	///< No compression.
	OF_COMPRESS_SRGB,	///< sRGB compression.
	OF_COMPRESS_ARB		///< ARB compression.
};


/// \cond INTERNAL

/// \class ofTextureData
/// \brief Internal texture data structure.
///
/// Used by ofTexture internally. You won't need to work with this in most cases.
class ofTextureData {
public:
	ofTextureData() {
		textureID = 0;
#ifndef TARGET_OPENGLES
		glInternalFormat = GL_RGB8;
		textureTarget = GL_TEXTURE_RECTANGLE_ARB;
#else
		glInternalFormat = GL_RGB;
		textureTarget = GL_TEXTURE_2D;
#endif

		tex_t = 0;
		tex_u = 0;
		tex_w = 0;
		tex_h = 0;
		width = 0;
		height = 0;
		
		bFlipTexture = false;
		compressionType = OF_COMPRESS_NONE;
		bAllocated = false;
		bUseExternalTextureID = false;
		useTextureMatrix = false;
		
		minFilter = GL_LINEAR;
		magFilter = GL_LINEAR;
		
		wrapModeHorizontal = GL_CLAMP_TO_EDGE;
		wrapModeVertical = GL_CLAMP_TO_EDGE;
		hasMipmap = false;
		bufferId = 0;

	}

	unsigned int textureID; ///< GL internal texture ID.
	int textureTarget; ///< GL texture type, either GL_TEXTURE_2D or
	                   ///< GL_TEXTURE_RECTANGLE_ARB.
	int glInternalFormat; ///< GL internal format, e.g. GL_RGB8.
                        ///< \sa http://www.opengl.org/wiki/Image_Format
	
	float tex_t; ///< Texture horizontal coordinate, ratio of width to display width.
	float tex_u; ///< Texture vertical coordinate, ratio of height to display height.
	float tex_w; ///< Texture width (in pixels).
	float tex_h; ///< Texture height (in pixels).
	float width, height; ///< Texture display size.
	
	bool bFlipTexture; ///< Should the texture be flipped vertically?
	ofTexCompression compressionType; ///< Texture compression type.
	bool bAllocated; ///< Has the texture been allocated?

	GLint minFilter; ///< Filter to use for minification (reduction).
	GLint magFilter; ///< Filter to use for magnification (enlargement).

	GLint wrapModeHorizontal; ///< How will the texture wrap around horizontally?
	GLint wrapModeVertical; ///< How will the texture wrap around vertically?
	
	unsigned int bufferId; ///< Optionally if the texture is backed by a buffer so we can bind it
private:
	shared_ptr<ofTexture> alphaMask; ///< Optional alpha mask to bind
	bool bUseExternalTextureID; ///< Are we using an external texture ID? 
	ofMatrix4x4 textureMatrix; ///< For required transformations.
	bool useTextureMatrix; ///< Apply the transformation matrix?
	bool hasMipmap; ///< True if mipmap has been generated for this texture, false by default.

	friend class ofTexture;

};

/// \endcond

/// \brief Enable the global texture "edge hack" to compensate for edge artifacts.
///
/// Adds a 2 pixel offset to avoid possible edge artifacts (typically a black or
/// white border). This *very slightly* alters the image by scaling.  This is
/// enabled by default.
void ofEnableTextureEdgeHack();

/// \todo Add docs on why the "edge hack" is needed.

/// \brief Disable global texture "edge hack".
/// \sa ofEnableTextureEdgeHack()
void ofDisableTextureEdgeHack();

/// \brief Check whether OF is using the texture "edge hack".
/// \sa ofEnableTextureEdgeHack()
/// \returns true if OF is currently using the texture "edge hack".
bool ofIsTextureEdgeHackEnabled();

/// \class ofTexture
/// \brief A wrapper class for an OpenGL texture.
class ofTexture : public ofBaseDraws {
	public :

	/// \name Construction and allocation
	/// \{

	/// \brief Construct an ofTexture instance.
	ofTexture();

	/// \brief Construct an ofTexture from an existing ofTexture.
	/// \param mom The ofTexture to copy. Reuses internal GL texture ID.
	ofTexture(const ofTexture & mom);
    ofTexture(ofTexture && mom);

	/// \brief Allocate the texture using the given settings.
	///
	/// This is useful if you need manual control over loading a number of
	/// textures with the same settings. Make sure to set the texture data 
	/// parameters first.
	///
	/// \param textureData The settings to use when allocating the ofTexture.
	virtual void allocate(const ofTextureData & textureData);

	/// \brief Allocate the texture using the given settings and custom format.
	/// \param textureData The settings to use when allocating the ofTexture.
	/// \param glFormat GL texture format: GL_RGBA, GL_LUMINANCE, etc.
	/// \param pixelType GL pixel type: GL_UNSIGNED_BYTE, GL_FLOAT, etc.
	virtual void allocate(const ofTextureData & textureData, int glFormat, int pixelType);

	/// \brief Allocate texture of a given size and format.
	///
	/// The width (w) and height (h) do not necessarily need to be powers of 2,
	/// but they do need to be large enough to contain the data you will upload
	/// to the texture.
	///
	/// The internal data type `glFormat` describes how OpenGL will store this
	/// texture internally. For example, if you want a grayscale texture, you
	/// can use `GL_LUMINANCE`. You can upload what ever type of data you want
	/// (using `loadData()`) but internally, opengl will store the information
	/// as grayscale. Other types include: `GL_RGB`, `GL_RGBA`.
	///
	/// This method applies the currently set OF texture type and defaults to
	/// ARB rectangular textures if they are supported. (They are not supported
	/// on OpenGL ES).
	///
	/// \param w Desired width in pixels.
	/// \param h Desired height in pixels.
	/// \param glInternalFormat OpenGL internal data format: `GL_RGBA`, `GL_LUMINANCE`, etc.
	virtual void allocate(int w, int h, int glInternalFormat);

	/// \brief Allocate texture of a given size and format.
	///
	/// \sa allocate(int w, int h, int glInternalFormat)
	/// \param w Desired width in pixels.
	/// \param h Desired height in pixels.
	/// \param glInternalFormat The internal openGL format.
	/// \param glFormat The openGL format.
	/// \param pixelType GL pixel type: GL_UNSIGNED_BYTE, GL_FLOAT, etc.
	virtual void allocate(int w, int h, int glInternalFormat, int glFormat, int pixelType);
	
	/// \brief Allocate texture of a given size and format. Specify texture type.
	///
	/// This allows to enable the ARBE extension for this texture.
	/// This will overide the default OF texture type, in case you need a
	/// square texture (`GL_TEXTURE_2D`).
	///
	/// \warning ARB textures are not available in OpenGL ES.
	/// \sa ofEnableArbTex()
	/// \sa allocate(int w, int h, int glInternalFormat)
	/// \param w Desired width in pixels.
	/// \param h Desired height in pixels.
	/// \param glInternalFormat The internal openGL format.
	/// \param bUseARBExtension Set to true to use rectangular textures.
	virtual void allocate(int w, int h, int glInternalFormat, bool bUseARBExtension);

	/// \brief Allocate texture of a given size, format, & type.
	///
	/// \sa allocate(int w, int h, int glInternalFormat)
	/// \param w Desired width in pixels.
	/// \param h Desired height in pixels.
	/// \param glInternalFormat OpenGL data format: `GL_RGBA`, `GL_LUMINANCE`, etc.
	/// \param bUseARBExtension Set to true to use rectangular textures.
	/// \param glFormat The OpenGL format.
	/// \param pixelType OpenGL pixel type: `GL_UNSIGNED_BYTE`, `GL_FLOAT`, etc.
	virtual void allocate(int w, int h, int glInternalFormat, bool bUseARBExtension, int glFormat, int pixelType);

	/// \todo Not sure how the two texture format parameters are different:
	/// glFormat & glInternalFormat.

	/// \brief Allocate texture using an ofPixels instance.
	///
	/// Pixel type and OpenGL format are determined from pixel settings.
	///
	/// \param pix Reference to ofPixels instance.
	virtual void allocate(const ofPixels& pix);

	/// \brief Allocate texture using an ofPixels instance and type.
	///
	/// This lets you overide the default OF texture type in case you need a
	/// square GL_TEXTURE_2D texture.
	///
	/// \warning ARB textures are not available in OpenGL ES.
	/// \sa ofEnableArbTex()
	/// \sa allocate(const ofPixels& pix)
	/// \param pix Reference to ofPixels instance.
	/// \param bUseARBExtension Set to true to use rectangular textures.
	virtual void allocate(const ofPixels& pix, bool bUseARBExtension);
	
	/// \brief Allocate texture using an ofShortPixels instance.
	///
	/// Same as void allocate(const ofPixels& pix), except using ofShortPixels.
	///
	/// \sa allocate(const ofPixels& pix)
	/// \param pix Reference to ofShortPixels instance.
	virtual void allocate(const ofShortPixels& pix);

	/// \brief Allocate texture using an ofShortPixels instance and type.
	///
	/// Same as void void allocate(const ofPixels& pix), except using ofShortPixels.
	///
	/// \sa allocate(const ofShortPixels& pix)
	/// \param pix Reference to ofShortPixels instance.
	/// \param bUseARBExtension Set to true to use rectangular textures.
	virtual void allocate(const ofShortPixels& pix, bool bUseARBExtension);
	
	/// \brief Allocate texture using an ofFloatPixels instance.
	///
	/// Same as void allocate(const ofPixels& pix), except using ofFloatPixels.
	///
	/// \sa allocate(const ofPixels& pix)
	/// \param pix Reference to ofFloatPixels instance.
	virtual void allocate(const ofFloatPixels& pix);

	/// \brief Allocate texture using an ofShortPixels instance and type.
	///
	/// Same as void void allocate(const ofPixels& pix), except using ofShortPixels.
	///
	/// \sa allocate(const ofFloatPixels& pix)
	/// \param pix Reference to ofFloatPixels instance.
	/// \param bUseARBExtension Set to true to use rectangular textures.
	virtual void allocate(const ofFloatPixels& pix, bool bUseARBExtension);

#ifndef TARGET_OPENGLES
	/// \brief Allocate texture as a Buffer Texture.
	///
	/// Uses a GPU buffer as data for the texture instead of pixels in RAM
	/// Allows to use texture buffer objects (TBO) which make it easier to send big
	/// amounts of data to a shader as a uniform.
	/// 
	/// Buffer textures are 1D textures, and may only be sampled using texelFetch 
	/// in GLSL.
	///
	/// See textureBufferInstanceExample and https://www.opengl.org/wiki/Buffer_Texture
	///
	/// \sa allocate(const ofBufferObject & buffer, int glInternalFormat)
	/// \param buffer Reference to ofBufferObject instance.
	/// \param glInternalFormat Internal pixel format of the data.
	void allocateAsBufferTexture(const ofBufferObject & buffer, int glInternalFormat);
#endif


	/// \brief Determine whether the texture has been allocated.
	///
	/// This lets you check if a texture is safe to draw.  The texture can both
	/// be allocated by using `allocate()` or loading it with data `loadData()`.
	///
	/// \returns true if the texture has been allocated.
	bool isAllocated() const;


	/// \brief Has the texture been allocated?
	///
	/// Legacy function for backwards compatibility.
	///
	/// \returns true if the texture has been allocated.
	OF_DEPRECATED_MSG("Use isAllocated instead",bool bAllocated() const);

	/// \brief Destroy an ofTexture instance.
	///
	/// ofTexture keeps a reference count for the internal OpenGL texture ID.
	/// Thus, the texture ID is only released if there are no additional
	/// references to the internal texture ID.
	virtual ~ofTexture();


	/// \}

	/// \name Update texture
	/// \{

	/// \brief Copy a given ofTexture into this texture.
	/// \param mom The ofTexture to copy from. Reuses internal GL texture ID.
	ofTexture& operator=(const ofTexture & mom);
    ofTexture& operator=(ofTexture && mom);


	/// \brief Clears the texture.
	///
	/// Clears / frees the texture memory, if something was already allocated.
	/// Useful if you need to control the memory on the graphics card.
	///
	/// The internal GL texture ID is only released if this is the last texture
	/// using it.
	void clear();

	/// \brief Set the texture ID.
	///
	/// Allows you to point the texture id to an externally allocated id
	/// (perhaps from another texture). It's up to you to set the rest of the
	/// textData parameters manually. 
	///
	/// \warning When setting an external texture ID, the user must set the
	/// remaining ofTextureData parameters manually.
	/// \param externTexID New texture ID.
	void setUseExternalTextureID(GLuint externTexID);

	/// \brief Load byte pixel data.
	///
	/// glFormat can be different to the internal format of the texture on each
	/// load, i.e. we can upload GL_BGRA pixels into a GL_RGBA texture but the
	/// number of channels need to match according to the OpenGL standard.
	/// 
	/// \param data Pointer to byte pixel data. Must not be nullptr.
	/// \param w Pixel data width.
	/// \param h Pixel data height.
	/// \param glFormat GL pixel type: GL_RGBA, GL_LUMINANCE, etc.
	void loadData(const unsigned char* const data, int w, int h, int glFormat);

	/// \brief Load short (2 byte) pixel data.
	/// \sa loadData(const unsigned char* const data, int w, int h, int glFormat)
	/// \param data Pointer to byte pixel data. Must not be nullptr.
	/// \param w Pixel data width.
	/// \param h Pixel data height.
	/// \param glFormat GL pixel type: GL_RGBA, GL_LUMINANCE, etc.
	void loadData(const unsigned short* data, int w, int h, int glFormat);

	/// \brief Load float pixel data.
	/// \sa loadData(const unsigned char* const data, int w, int h, int glFormat)
	/// \param data Pointer to byte pixel data. Must not be nullptr.
	/// \param w Pixel data width.
	/// \param h Pixel data height.
	/// \param glFormat GL pixel type: GL_RGBA, GL_LUMINANCE, etc.
	void loadData(const float* data, int w, int h, int glFormat);

	/// \brief Load pixels from an ofPixels instance.
	/// \param pix Reference to ofPixels instance.
	void loadData(const ofPixels & pix);

	/// \brief Load pixels from an ofShortPixels instance.
	///
	/// Same as loadData(ofPixels &) but for ofShortPixels.
	///
	/// \sa loadData(const ofPixels & pix)
	/// \param pix Reference to ofShortPixels instance.
	void loadData(const ofShortPixels & pix);

	/// \brief Load pixels from an ofFloatPixels instance.
	///
	/// Same as loadData(ofPixels &) but for ofFloatPixels.
	///
	/// \sa loadData(const ofPixels & pix)
	/// \param pix Reference to ofFloatPixels instance.
	void loadData(const ofFloatPixels & pix);

	/// \brief Load pixels from an ofPixels instance and specify the format.
	///
	/// glFormat can be different to the internal format of the texture on each
	/// load, ie. we can upload GL_BGRA pixels into a GL_RGBA texture, but the
	/// number of channels need to match according to the OpenGL standard.
	///
	/// \param pix Reference to ofPixels instance.
	/// \param glFormat GL pixel type: GL_RGBA, GL_LUMINANCE, etc.
	void loadData(const ofPixels & pix, int glFormat);

	/// \brief Load pixels from an ofShortPixels instance & specify the format.
	///
	/// \sa loadData(const ofPixels & pix, int glFormat)
	/// \param pix Reference to ofShortPixels instance.
	/// \param glFormat GL pixel type: GL_RGBA, GL_LUMINANCE, etc.
	void loadData(const ofShortPixels & pix, int glFormat);

	/// \brief Load pixels from an ofFloatPixels instance and specify the format.
	///
	/// \sa loadData(const ofPixels & pix, int glFormat)
	/// \param pix Reference to ofFloatPixels instance.
	/// \param glFormat GL pixel type: GL_RGBA, GL_LUMINANCE, etc.
	void loadData(const ofFloatPixels & pix, int glFormat);

#ifndef TARGET_OPENGLES
	/// \brief Load pixels from an ofBufferObject
	///
	/// This is different to allocate(ofBufferObject,internal). That
	/// creates a texture which data lives in GL buffer while this
	/// copies the data from the buffer to the texture.
	///
	/// This is usually used to upload data to be shown asynchronously
	/// by using a buffer object binded as a PBO
	///
	/// \param buffer The buffer to load.
	/// \param glFormat GL pixel type: GL_RGBA, GL_LUMINANCE, etc.
	/// \param glType the GL type to load.
	void loadData(const ofBufferObject & buffer, int glFormat, int glType);
#endif

	/// \brief Copy an area of the screen into this texture.
	///
	/// Specifiy the position (x,y) you wish to grab from, with the width (w)
	/// and height (h) of the region.
	///
	/// Make sure that you have allocated your texture (using `allocate()`)
	/// to be large enough to hold the region of the screen you wish to load.
	///
	/// \sa http://www.opengl.org/sdk/docs/man4/html/glCopyTexSubImage2D.xhtml
	///
	/// \param x Upper left corner horizontal screen position.
	/// \param y Upper left corner vertical screen position.
	/// \param w Width of the area to copy in pixels.
	/// \param h Height of the area to copy in pixels.
	void loadScreenData(int x, int y, int w, int h);
	
	/// \}


	/// \name Drawing
	/// \{

	using ofBaseDraws::draw;
	
	void draw(float x, float y) const;
	void draw(float x, float y, float z) const;

	void draw(float x, float y, float w, float h) const;

	/// \brief Draw the texture at a given size witdh and depth.
	///
	/// \param x Draw position on the x axis.
	/// \param y Draw position on the y axis.
	/// \param z Draw position on the z axis.
	/// \param w Draw width.
	/// \param h Draw height.
	void draw(float x, float y, float z, float w, float h) const;
	
	/// \brief Draws the texture at 4 points passed in as if you created 4 glVertices.
	///
	/// \param p1 Upper left position on the x axis.
	/// \param p2 Upper left position on the y axis.
	/// \param p3 Lower right position on the x axis.
	/// \param p4 Lower right position on the y axis.
	void draw(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3, const ofPoint & p4) const;

	/// \brief Draw a subsection of the texture.
	///
	/// Like ofRect() depend on the current `OF_RECT_MODE`:
	///
	/// * `OF_RECT_MODE_CORNER`: drawn with the upper left corner = (x,y)
	/// * `OF_RECT_MODE_CENTER`: drawn centered on (x,y)
	///
	/// \sa ofSetRectMode()
	///
	/// \param x Draw position on the x axis.
	/// \param y Draw position on the y axis.
	/// \param w Draw width.
	/// \param h Draw height.
	/// \param sx Subsection x axis offset within the texture.
	/// \param sy Subsection y axis offset within the texture.
	void drawSubsection(float x, float y, float w, float h, float sx, float sy) const;

	/// \brief Draw a subsection of the texture with depth.
	///
	/// \sa drawSubsection(float x, float y, float w, float h, float sx, float sy)
	/// \param x Draw position on the x axis.
	/// \param y Draw position on the y axis.
	/// \param z Draw position on the z axis.
	/// \param w Draw width.
	/// \param h Draw height.
	/// \param sx Subsection x axis offset within the texture.
	/// \param sy Subsection y axis offset within the texture.
	void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy) const;

	/// \brief Draw a subsection of the texture with an offset.
	///
	/// \sa drawSubsection(float x, float y, float w, float h, float sx, float sy)
	/// \param x Draw position on the x axis.
	/// \param y Draw position on the y axis.
	/// \param w Draw width.
	/// \param h Draw height.
	/// \param sx Subsection x axis offset within the texture.
	/// \param sy Subsection y axis offset within the texture.
	/// \param sw Subsection width within the texture.
	/// \param sh Subsection height within the texture.
	void drawSubsection(float x, float y, float w, float h, float sx, float sy, float sw, float sh) const;
	
	/// \brief Draw a subsection of the texture with an offset and depth.
	///
	/// \sa drawSubsection(float x, float y, float w, float h, float sx, float sy)
	/// \param x Draw position on the x axis.
	/// \param y Draw position on the y axis.
	/// \param z Draw position on the z axis.
	/// \param w Draw width.
	/// \param h Draw height.
	/// \param sx Subsection x axis offset within the texture.
	/// \param sy Subsection y axis offset within the texture.
	/// \param sw Subsection width within the texture.
	/// \param sh Subsection height within the texture.
	void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;

	ofMesh getQuad(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3, const ofPoint & p4) const;

	/// \brief Get a mesh that has the texture coordinates set.
	///
	/// \sa drawSubsection(float x, float y, float w, float h, float sx, float sy)
	/// \param x Draw position on the x axis.
	/// \param y Draw position on the y axis.
	/// \param z Draw position on the z axis.
	/// \param w Draw width.
	/// \param h Draw height.
	/// \param sx Subsection x axis offset within the texture.
	/// \param sy Subsection y axis offset within the texture.
	/// \param sw Subsection width within the texture.
	/// \param sh Subsection height within the texture.
	/// \param vflipped Takes into account the flipped state in OF.
	/// \param rectMode rectMode Taking x,y as the center or the top left corner.
	ofMesh getMeshForSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh, bool vflipped, ofRectMode rectMode) const;

	/// \brief Bind the texture.
	///
	/// For advanced users who need to manually manage texture drawing without
	/// calling ofTexture::draw.
	///
	/// \sa http://www.opengl.org/sdk/docs/man4/html/glBindTexture.xhtml
	void bind(int textureLocation=0) const;
	
	/// \brief Unbind the texture.
	///
	/// For advanced users who need to manually manage texture drawing without
	/// calling ofTexture::draw.
	///
	/// \sa http://www.opengl.org/sdk/docs/man4/html/glBindTexture.xhtml
	///
	void unbind(int textureLocation=0) const;

#if !defined(TARGET_OPENGLES) && defined(glBindImageTexture)
	/// Calls glBindImageTexture on the texture
	///
	/// Binds the texture as an read or write image, only available since OpenGL 4.2
	/// \warning This is not available in OpenGLES
	/// \sa http://www.opengl.org/wiki/GLAPI/glBindImageTexture
	void bindAsImage(GLuint unit, GLenum access, GLint level=0, GLboolean layered=0, GLint layer=0);
#endif

	const ofTexture * getAlphaMask() const;
	/// \}

	/// \name Size and coordinates
	/// \{

	/// \brief Display height of texture.
	///
	/// Return value is pixel size (default) or normalized (0 - 1) if ofEnableNormalizedTextures() is set to true.
	///
	/// \sa ofEnabledNormalizedTextures()
	///
	/// \returns Display height of texture in pixels.
	float getHeight() const;

	/// \brief Display width of texture.
	///
	/// Return value is pixel size (default) or normalized (0 - 1) if ofEnableNormalizedTextures() is set to true.
	///
	/// \sa ofEnabledNormalizedTextures()
	///
	/// \returns Display width of texture in pixels.
	float getWidth() const;

	/// \brief Set the anchor point the texture is drawn around as a percentage.
	///
	/// This can be useful if you want to rotate an image around a particular
	/// point.
	///
	/// \param xPct Horizontal texture position as a percentage (0 - 1).
	/// \param yPct Vertical texture position as a percentage (0 - 1).
	void setAnchorPercent(float xPct, float yPct);

	/// \brief Set the anchor point the texture is drawn around in pixels.
	///
	/// This can be useful if you want to rotate an image around a particular
	/// point.
	///
	/// \param x Horizontal texture position in pixels.
	/// \param y Vertical texture position in pixels.
	void setAnchorPoint(float x, float y);

	/// \brief Reset the anchor point to (0, 0).
	void resetAnchor();

	/// \brief Helper to convert display coordinate to texture coordinate.
	/// \param xPos Horizontal position in pixels.
	/// \param yPos Vertical position in pixels.
	/// \returns Texture coordinate or ofPoint::zero() if texture is not allocated.
	ofPoint getCoordFromPoint(float xPos, float yPos) const;
	
	/// \brief Helper to convert display coordinate to texture coordinate.
	/// \param xPts Horizontal position in a normalized percentage (0 - 1).
	/// \param yPts Vertical position in a normalized percentage (0 - 1).
	/// \returns Texture coordinate or ofPoint::zero() if texture is not allocated.
	ofPoint getCoordFromPercent(float xPts, float yPts) const;

	/// \}

	/// \name Texture settings
	/// \{

	/// \brief Set another ofTexture to use as an alpha mask.
	/// \param mask The texture to use as alpha mask.
	void setAlphaMask(ofTexture & mask);

	/// \brief Disable the alpha mask.
	void disableAlphaMask();

	/// \brief Set texture wrapping.
	///
	/// By default, textures are clamped to their edges with `GL_CLAMP_TO_EDGE`.
	/// Setting a repeat mode like `GL_REPEAT` allows you to create tiled
	/// backgrounds with small textures.
	///
	/// \sa ofTextureSetWrap()
	/// \sa http://www.opengl.org/sdk/docs/man4/html/glTexParameter.xhtml
	///
	/// \warning May be overridden.
	///
	/// \param wrapModeHorizontal wrap parameter for texture coordinate s.
	/// \param wrapModeVertical wrap parameter for texture coordinate t.
	void setTextureWrap(GLint wrapModeHorizontal, GLint wrapModeVertical);

	/// \brief Set texture minification/magnification scaling filters.
	///
	/// Controls how OpenGL will scale this texture.
	///
	/// \sa http://www.opengl.org/sdk/docs/man4/html/glTexParameter.xhtml
	/// \sa ofTextureSetMinMagFilters()
	///
	/// \warning May be overridden.
	///
	/// \param minFilter minifying filter for scaling a pixel to a smaller area.
	/// \param magFilter magnifying filter for scaling a pixel to a larger area.
	void setTextureMinMagFilter(GLint minFilter, GLint magFilter);

	/// \brief Sets a texture matrix to be uploaded whenever the texture is bound.
	/// \param m The 4x4 texture matrix.
	void setTextureMatrix(const ofMatrix4x4 & m);

	const ofMatrix4x4 & getTextureMatrix() const;

	bool isUsingTextureMatrix() const;

	/// Disable the texture matrix.
	/// \brief Disable the texture matrix.
	void disableTextureMatrix();

	/// \brief Set the texture compression.
	///
	/// \warning: not yet implemented.
	/// \sa ofTexCompression
	void setCompression(ofTexCompression compression);

	/// \todo Define Swizzle in the documentation.
	/// \brief Swizzle RGBA to grayscale with alpha in the red channel.
	///
	/// Use 1 channel GL_R as luminance instead of red channel in OpenGL 3+.
	///
	/// \warning This is not supported in OpenGL ES and does nothing.
	/// \sa https://en.wikipedia.org/wiki/Swizzling_(computer_graphics)
	void setRGToRGBASwizzles(bool rToRGBSwizzles);

	/// \brief Swizzle a channel to another
	///
	/// Example:
	///
	/// ~~~~~
	/// ofTexture tex;
	/// tex.setSwizzle(GL_TEXTURE_SWIZZLE_R,GL_ALPHA);
	/// ~~~~~
	///
	/// will make channel 0 appear as alpha in the shader.
	///
	/// \warning This is not supported in OpenGL ES and does nothing.
	/// \sa https://en.wikipedia.org/wiki/Swizzling_(computer_graphics)
	void setSwizzle(GLenum srcSwizzle, GLenum dstChannel);

	/// \}

	/// \name Read pixel data
	/// \{

	/// \brief Read current texture data from the GPU into pixels.
	///
	/// \warning This is not supported in OpenGL ES and does nothing.
	///
	/// \param pixels Target ofPixels reference.
	void readToPixels(ofPixels & pixels) const;

	/// \brief Read current texture data from the GPU into pixels.
	///
	/// \warning This is not supported in OpenGL ES and does nothing.
	///
	/// \param pixels Target pixels reference.
	void readToPixels(ofShortPixels & pixels) const;

	/// \brief Read current texture data from the GPU into pixels.
	///
	/// \warning This is not supported in OpenGL ES and does nothing.
	///
	/// \param pixels Target pixels reference.
	void readToPixels(ofFloatPixels & pixels) const;

#ifndef TARGET_OPENGLES
	/// \brief Copy the texture to an ofBufferObject.
	/// \param buffer the target buffer to copy to.
	void copyTo(ofBufferObject & buffer) const;
#endif

	/// \}

	/// \name Texture data
	/// \{

	/// \brief Internal texture data access.
	///
	/// This returns the internal texture data for this texture, for instance,
	/// its textureID, type of texture, whether it's been allocated and other
	/// data about the state of the texture.
	///
	/// \returns a reference to the internal texture data struct.
	ofTextureData& getTextureData();

	/// \brief Const version of getTextureData().
	/// \sa ofTextureData::getTextureData()
	const ofTextureData& getTextureData() const;

	/// \}


	/// \name Mipmapping
	/// \{

	/// \brief Sets flag allowing texture to auto-generate a mipmap.
	///
	/// By default, this will set your minFilter to GL_LINEAR_MIPMAP_LINEAR.
	/// If you want to change your minFilter later use setTextureMinMagFilter()
	///
	///	If you want to generate a mipmap later, or at a specific
	/// point in your code, use generateMipmap() instead.
	///
	/// \sa generateMipmap()
	/// \sa disableMipmap()
	/// \sa setTextureMinMagFilter()
	void enableMipmap();

	/// \brief Sets flag disallowing texture to auto-generate mipmap.
	///
	/// By default, this will set your minFilter to GL_LINEAR_MIPMAP_LINEAR.
	/// If you want to change your minFilter later use setTextureMinMagFilter()
	///
	///	If you want to generate a mipmap later, or at a specific
	/// point in your code, use ofTexture::generateMipmap() instead.
	///
	/// \sa generateMipmap()
	/// \sa enableMipmap()
	/// \sa setTextureMinMagFilter()
	void disableMipmap();
	

	/// \brief Generate mipmap for the current texture.
	///
	/// \warning Only GL_TEXTURE_RECTANGLE - which is the default openFrameworks
	/// texture target - does *not* support mipmaps, so make sure to call
	/// ofDisableArbTex() before loading texture
	/// data for a texture you want to generate mipmaps for.
	///
	/// \sa ofEnableArbTex()
	/// \sa ofDisableArbTex()
	void generateMipmap();

	/// \brief Find out if a mipmap has been generated for the current texture.
	///
	/// \sa generateMipmap()
	/// \sa enableMipmap()
	bool hasMipmap() const;
	
	/// \}

	/// \cond INTERNAL
	ofTextureData texData; ///< Internal texture data access.
	                       ///< For backwards compatibility.

protected:
	/// \brief Load byte pixel data.
	///
	/// glFormat can be different to the internal format of the texture on each
	/// load, i.e. we can upload GL_BGRA pixels into a GL_RGBA texture but the
	/// number of channels need to match according to the OpenGL standard.
	///
	/// \param data Pointer to byte pixel data. Must not be nullptr.
	/// \param w Pixel data width.
	/// \param h Pixel data height.
	/// \param glFormat GL pixel type: GL_RGBA, GL_LUMINANCE, etc.
	/// \param glType the OpenGL type of the data.
    void loadData(const void * data, int w, int h, int glFormat, int glType);

	/// \brief Enable a texture target.
	/// \param textureLocation the OpenGL texture ID to enable as a target.
	void enableTextureTarget(int textureLocation) const;

	/// \brief Disable a texture target.
	/// \param textureLocation the OpenGL texture ID to enable as a target.
	void disableTextureTarget(int textureLocation) const;

	ofPoint anchor; ///< The texture's anchor point.

	bool bAnchorIsPct; ///< Is the anchor point represented as a normalized
					   ///< (0 - 1) coordinate?

	/// \endcond

private:
	bool bWantsMipmap; ///< Should mipmaps be created?
	
};
