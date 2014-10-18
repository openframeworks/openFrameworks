#pragma once

#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofBaseTypes.h"
#include "ofConstants.h"
#include "ofVboMesh.h"

/// \brief Check whether OF is using GL_TEXTURE_RECTANGLE rectangular or GL_TEXTURE_2D textures.
/// \sa ofEnableArbTex()
/// \returns true if using GL_TEXTURE_RECTANGLE textures, false if using GL_TEXTURE_2D textures.
bool ofGetUsingArbTex();

/// \brief Use GL_TEXTURE_RECTANGLE textures.
///
/// GL_TEXTURE_RECTANGLE textures are more intuitive since they allow pixel based coordinates
/// and are enabled by default.
///
/// GL_TEXTURE_2D textures use normalised texture coordinates (a float value between 0 and 1
/// is used to express texture coordinates along width and height.)
///
/// GL_TEXTURE_2D textures are more widely supported and allow advanced features such as
/// mipmaps and texture compression.
///
/// \sa ofDisableArbTex()
/// \warning GL_TEXTURE_RECTANGLE is not available in OpenGL ES.
/// \warning GL_TEXTURE_RECTANGLE does not support mipmaps.
void ofEnableArbTex();

/// \brief Use GL_TEXTURE_2D textures.
///
/// GL_TEXTURE_2D is OpenGL's default way of handling textures and does support
/// a wider range of core OpenGL features such as mipmaps.
///
/// \sa ofEnableArbTex()
void ofDisableArbTex();

/// \brief Check whether OF is using normalized or pixel texture coordinates.
/// \sa ofEnableNormalizedTexCoords()
/// \returns true if using normalized (0 - 1) texture coordinates.
bool ofGetUsingNormalizedTexCoords();

/// \brief Use normalized (0 - 1) texture coordinates.
///
/// By default, textures in OF work with pixel based coordinates based on given
/// content dimensions (images size, fbo size, etc). Normalized (0 - 1)
/// coordinates are the traditional GL default as they allow working with
/// internal texture data without having to worry about specific dimensions of
/// the content.
///
/// This overrides individual ofTexture wrap settings.
///
/// \sa http://www.opengl.org/sdk/docs/man4/html/glTexParameter.xhtml
void ofEnableNormalizedTexCoords();

/// \brief Use pixel based texture coordinates.
/// \sa ofEnableNormalizedTexCoords()
void ofDisableNormalizedTexCoords();

/// \brief Set custom global texture wrapping.
///
/// By default, textures are clamped to their edges with GL_CLAMP_TO_EDGE.
/// Setting a repeat mode like GL_REPEAT allows you to create tiled backgrounds
/// with small textures.
///
/// See <http://www.opengl.org/sdk/docs/man4/html/glTexParameter.xhtml> for more
/// info.
///
/// This overrides individual ofTexture wrap settings.
/// \sa ofTexture::setTextureWrap
///
/// \warning Deprecated. Use member methods instead.
///
/// \param wrapS wrap parameter for texture coordinate s.
/// \param wrapT wrap parameter for texture coordinate t.
OF_DEPRECATED_MSG("Use member method ofTexture::setTextureWrap() instead.",void ofSetTextureWrap(GLfloat wrapS = GL_CLAMP_TO_EDGE, GLfloat wrapT = GL_CLAMP_TO_EDGE));

/// \brief Check whether OF is using custom global texture wrapping.
///
/// \warning Deprecated. Use member methods instead.
/// \sa ofSetTextureWrap()
/// \returns true if OF is currently using custom global texture wrapping. 
OF_DEPRECATED_MSG("Use member method ofTexture::setTextureWrap() instead.",bool ofGetUsingCustomTextureWrap());

/// \brief Removes global custom texture wrapping.
///
/// Restores individual ofTexture wrap settings.
/// \warning Deprecated. Use member methods instead.
/// \sa ofSetTextureWrap()
OF_DEPRECATED_MSG("Use member method ofTexture::setTextureWrap() instead.",void ofRestoreTextureWrap());

/// \brief Set custom global texture minification/magnification scaling filters.
///
/// This setting allows global control over how OpenGL scales textures. It
/// overrides individual ofTexture min & mag filter settings.
///
/// \warning Deprecated. Use member methods instead.
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

/// Compression is only available through OpenGL
/// for textures using GL_TEXTURE_2D, *not* GL_TEXTURE_RECTANGLE,
/// also note that most compression algorithms work on blocks of 4x4 pixels,
/// and therefore expect compressed textures to have multiple-of-four dimensions.
enum ofTexCompression {
	OF_COMPRESS_NONE,	///< no compression
	OF_COMPRESS_SRGB,	///< sRGB compression
	OF_COMPRESS_ARB		///< ARB compression
};

/// \class ofTextureData
/// \brief Internal texture settings struct.
///
/// Used by ofTexture internally. You won't need to work with this in most cases.
class ofTextureData {
public:
	ofTextureData() {
		textureID = 0;
#ifndef TARGET_OPENGLES
		glTypeInternal = GL_RGB8;
		textureTarget = GL_TEXTURE_RECTANGLE_ARB;
#else
		glTypeInternal = GL_RGB;
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
		isBound = false;
		
		minFilter = GL_LINEAR;
		magFilter = GL_LINEAR;
		
		wrapModeHorizontal = GL_CLAMP_TO_EDGE;
		wrapModeVertical = GL_CLAMP_TO_EDGE;
		hasMipmap = false;

	}

	unsigned int textureID; ///< GL internal texture ID
	int textureTarget; ///< GL texture type, either GL_TEXTURE_2D or
	                   ///< GL_TEXTURE_RECTANGLE_ARB.
	int glTypeInternal; ///< GL internal format, e.g. GL_RGB8.
                        ///< \sa http://www.opengl.org/wiki/Image_Format
	
	float tex_t; ///< Texture horiz coordinate, ratio of width to display width. 
	float tex_u; ///< Texture vert coordinate, ratio of height to display height.
	float tex_w; ///< Texture width (in pixels).
	float tex_h; ///< Texture height (in pixels).
	float width, height; ///< Texture display size.
	
	bool bFlipTexture; ///< Should the texture be flipped vertically?
	ofTexCompression compressionType; ///< Texture compression type.
	bool bAllocated; ///< Has the texture been allocated?

	GLint minFilter; ///< Filter to use for minification (reduction)
	GLint magFilter; ///< Filter to use for magnification (enlargement)

	GLint wrapModeHorizontal; ///< How will the texture wrap around horizontally?
	GLint wrapModeVertical; ///< How will the texture wrap around vertically?
	
private:
	mutable bool isBound;  ///< Is the texture already bound
	shared_ptr<ofTexture> alphaMask; ///< Optional alpha mask to bind
	bool bUseExternalTextureID; ///< Are we using an external texture ID? 
	ofMatrix4x4 textureMatrix; ///< For required transformations.
	bool useTextureMatrix; ///< Apply the transformation matrix?
	bool hasMipmap; ///< True if mipmap has been generated for this texture, false by default.
	friend class ofTexture;

};

/// \brief Enable global texture "edge hacking" to compensate for edge artifacts.
///
/// Adds a 2 pixel offset to avoid possible edge artifacts (typically a black or
/// white border). This *very slightly* alters the image by scaling.  This is
/// enabled by default.
void ofEnableTextureEdgeHack();

/// \todo Add docs on why the "edge hacking" is needed.

/// \brief Disable global texture "edge hacking".
/// \sa ofEnableTextureEdgeHack()
void ofDisableTextureEdgeHack();

/// \brief Check whether OF is using the "texture edge hack".
/// \sa ofEnableTextureEdgeHack()
/// \returns true if OF is currently using the "texture edge hack".
bool ofIsTextureEdgeHackEnabled();

/// \class ofTexture
/// \brief An OpenGL image on the the graphics card.
///
/// ofTexture is a wrapper for the OpenGL texture API. It allows use of
/// non-power of 2 textures and to upload and draw graphical data.
///
/// \sa https://www.opengl.org/wiki/Texture
class ofTexture : public ofBaseDraws {
	public :

	/// \brief Construct an ofTexture instance.
	ofTexture();

	/// \brief Construct an ofTexture from an existing ofTexture.
	/// \param mom The ofTexture to copy. Reuses internal GL texture ID.
	ofTexture(const ofTexture & mom);

	/// \brief Copy a given ofTexture into this texture.
	/// \param mom The ofTexture to copy from. Reuses internal GL texture ID.
	ofTexture& operator=(const ofTexture & mom);
	
	/// \brief Destroy an ofTexture instance.
	///
	/// The internal GL texture ID is only rleased if this is the last texture
	/// using it.
	virtual ~ofTexture();

	/// \brief Allocate texture using the given settings.
	///
	/// This is useful if you need manual control over loading a number of
	/// textures with the same settings. Make sure to set the parameters first.
	///
	/// \param textureData Reference to settings to use when allocating.
	virtual void allocate(const ofTextureData & textureData);

	/// \brief Allocate texture using the given settings & specify the format.
	/// \param textureData Reference to settings to use when allocating.
	/// \param glFormat GL texture format: GL_RGBA, GL_LUMINANCE, etc.
	/// \param pixelType GL pixel type: GL_UNSIGNED_BYTE, GL_FLOAT, etc.
	virtual void allocate(const ofTextureData & textureData, int glFormat, int pixelType);

	/// \brief Allocate texture of a given size and format.
	///
	/// Uses the currently set OF texture type. Defaults to ARB rectangular 
	/// textures if they are supported. (They are not supported on OpenGL ES).
	///
	/// \param w Desired width in pixels.
	/// \param h Desired height in pixels.
	/// \param glInternalFormat GL texture format: GL_RGBA, GL_LUMINANCE, etc.
	virtual void allocate(int w, int h, int glInternalFormat);

	/// \brief Allocate texture of a given size and format.
	///
	/// Same as allocate(w, h, glInternalFormat) with the addition of:
	///
	/// \param pixelType GL pixel type: GL_UNSIGNED_BYTE, GL_FLOAT, etc.
	virtual void allocate(int w, int h, int glInternalFormat, int glFormat, int pixelType);
	
	/// \brief Allocate texture of a given size and format. Specify texture type.
	///
	/// This lets you overide the default OF texture type, in case you need a
	/// square GL_TEXTURE_2D texture.
	///
	/// \warning ARB textures are not available in OpenGL ES.
	/// \sa ofEnableArbTex()
	///
	/// Same as allocate(w, h, glInternalFormat) with the addition of:
	///
	/// \param bUseARBExtension Set to true to use rectangular textures.
	virtual void allocate(int w, int h, int glInternalFormat, bool bUseARBExtention);

	/// \brief Allocate texture of a given size, format, & type.
	///
	/// \TODO Not sure how the two texture format parameters are different:
	/// glFormat & glInternalFormat.
	///
	/// Same as allocate(w, h, glInternalFormat, bUseARBExtension) with the
	/// addition of:
	///
	/// \param glFormat GL texture format: GL_RGBA, GL_LUMINANCE, etc.
	/// \param pixelType GL pixel type: GL_UNSIGNED_BYTE, GL_FLOAT, etc.
	virtual void allocate(int w, int h, int glInternalFormat, bool bUseARBExtention, int glFormat, int pixelType);
	
	/// \brief Allocate texture using an ofPixels instance.
	///
	/// Pixel type and GL format are determined from pixel settings.
	///
	/// \param pix Reference to ofPixels instance.
	virtual void allocate(const ofPixels& pix);

	/// \brief Allocate texture using an ofPixels instance & type.
	///
	/// This lets you overide the default OF texture type, in case you need a
	/// square GL_TEXTURE_2D texture.
	///
	/// \warning ARB textures are not available in OpenGL ES.
	/// \sa ofEnableArbTex()
	///
	/// Same as loadData(ofPixels &) with the addition of:
	/// \param bUseARBExtension Set to true to use rectangular textures.
	virtual void allocate(const ofPixels& pix, bool bUseARBExtention);
	
	/// \brief Allocate texture using an ofShortPixels instance.
	/// Same as loadData(ofPixels &), except using ofShortPixels.
	virtual void allocate(const ofShortPixels& pix);

	/// \brief Allocate texture using an ofShortPixels instance & type.
	/// Same as loadData(ofPixels &, bool), except using ofShortPixels.
	virtual void allocate(const ofShortPixels& pix, bool bUseARBExtention);
	
	/// \brief Allocate texture using an ofFloatPixels instance.
	/// Same as loadData(ofPixels &), except using ofFloatPixels.
	virtual void allocate(const ofFloatPixels& pix);

	/// \brief Allocate texture using an ofFloatPixels instance & type.
	/// Same as loadData(ofPixels &, bool), except using ofFloatPixels.
	virtual void allocate(const ofFloatPixels& pix, bool bUseARBExtention);
	
	/// \brief Clears the texture.
	///
	/// The internal GL texture ID is only rleased if this is the last texture
	/// using it.
	void clear();

	/// \brief Set the texture ID.
	///
	/// Allows you to point the texture id to an externally allocated id
	/// (perhaps from another texture). Its up to you to set the rest of the
	/// textData parameters manually. 
	///
	/// \param externTexID New texture ID.
	void setUseExternalTextureID(GLuint externTexID);

	/// \brief Load byte pixel data.
	///
	/// glFormat can be different to the internal format of the texture on each
	/// load, ie. we can upload GL_BGRA pixels into a GL_RGBA texture, but the
	/// number of channels need to match according to the OpenGL standard.
	/// 
	/// \param data Pointer to byte pixel data. Must not be NULL.
	/// \param w Pixel data width.
	/// \param h Pixel data height.
	/// \param glFormat GL pixel type: GL_RGBA, GL_LUMINANCE, etc.
	void loadData(const unsigned char* const data, int w, int h, int glFormat);

	/// \brief Load short (2 byte) pixel data.
	/// Same as loadData(unsigned char, ...)  but for short pixel data.
	void loadData(const unsigned short* data, int w, int h, int glFormat);

	/// \brief Load float pixel data.
	/// Same as loadData(unsigned char, ...) but for float pixel data.
	void loadData(const float* data, int w, int h, int glFormat);

	/// \brief Load pixels from an ofPixels instance.
	/// \param pix Reference to ofPixels instance.
	void loadData(const ofPixels & pix);

	/// \brief Load pixels from an ofShortPixels instance.
	/// Same as loadData(ofPixels &) but for ofShortPixels.
	void loadData(const ofShortPixels & pix);

	/// \brief Load pixels from an ofFloatPixels instance.
	/// Same as loadData(ofPixels &) but for ofFloatPixels.
	void loadData(const ofFloatPixels & pix);

	/// \brief Load pixels from an ofPixels instance & specify the format.
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
	/// Similar to loadData(ofPixels &, int).
	///
	void loadData(const ofShortPixels & pix, int glFormat);

	/// \brief Load pixels from an ofFloatPixels instance & specify the format.
	///
	/// Similar to loadData(ofPixels &, int).
	///
	void loadData(const ofFloatPixels & pix, int glFormat);

#ifndef TARGET_OPENGLES
	void loadData(const ofBufferObject & buffer, int glFormat, int glType);
#endif

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
	
    /// \todo Define Swizzle in the documentation.

	/// \brief Swizzle RGBA to grayscale with alpha in the red channel.
	///
	/// Use 1 channel GL_R as luminance instead of red channel in OpenGL 3+.
	///
	/// \warning This is not supported in OpenGL ES and does nothing.
	///
	void setRGToRGBASwizzles(bool rToRGBSwizzles);

	/// \brief Swizzle a channel to another
	///
	/// Example:
	/// tex.setSwizzle(GL_TEXTURE_SWIZZLE_R,GL_ALPHA)
	/// will make channel 0 appear as alpha in the shader
	///
	/// \warning This is not supported in OpenGL ES and does nothing.
	///
	void setSwizzle(GLenum srcSwizzle, GLenum dstChannel);

	/// \brief Copy an area of the screen into this texture.
	///
	/// \sa http://www.opengl.org/sdk/docs/man4/html/glCopyTexSubImage2D.xhtml
	///
	/// \param x Upper left corner horizontal screen position.
	/// \param y Upper left corner vertical screen position.
	/// \param w Width of the area to copy in pixels.
	/// \param h Height of the area to copy in pixels.
	void loadScreenData(int x, int y, int w, int h);

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

	using ofBaseDraws::draw;

	/// \brief Draw the texture by it's upper left & lower right corners.
	///
	/// \param p1 Upper left position on the x axis.
	/// \param p2 Upper left position on the y axis.
	/// \param p3 Lower right position on the x axis.
	/// \param p4 Lower right position on the y axis.
	void draw(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3, const ofPoint & p4) const;
	
	/// \brief Draw the texture at it's normal size.
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

	/// \brief Draw the texture at a given size.
	///
	/// \param x Draw position on the x axis.
	/// \param y Draw position on the y axis.
	/// \param w Draw width.
	/// \param h Draw height.
	void draw(float x, float y, float w, float h) const;

	/// \brief Draw the texture at a given size with depth.
	///
	/// \param x Draw position on the x axis.
	/// \param y Draw position on the y axis.
	/// \param z Draw position on the z axis.
	/// \param w Draw width.
	/// \param h Draw height.
	void draw(float x, float y, float z, float w, float h) const;
	
	/// \brief Draw a subsection of the texture.
	///
	/// Like ofRect, (x, y) depend on the current OF_RECT_MODE:
	///
	/// * OF_RECT_MODE_CORNER: drawn with the upper left corner = (x,y)
	/// * OF_RECT_MODE_CENTER: drawn centered on (x,y)
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
	/// Same as drawSubsection(x, y, w, h, sx, sy) with the addition of:
	///
	/// \param z Draw position on the z axis.
	void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy) const;

	/// \brief Draw a subsection of the texture with an offset.
	///
	/// Same as drawSubsection(x, y, w, h, sx, sy) with the addition of:
	///
	/// \param sw Subsection width within the texture.
	/// \param sh Subsection height within the texture.
	void drawSubsection(float x, float y, float w, float h, float sx, float sy, float sw, float sh) const;
	
	/// \brief Draw a subsection of the texture with an offset and depth.
	///
	/// Same as drawSubsection(x, y, w, h, sx, sy, sw, sh) with the addition of:
	///
	/// \param sw Subsection width within the texture.
	/// \param sh Subsection height within the texture.
	/// \param z Draw position on the z axis.
	void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;

	ofMesh getMeshForSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;

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
	void copyTo(ofBufferObject & buffer) const;
#endif

	/// \brief Bind the texture.
	///
	/// For advanced users who need to manually manage texture drawing without
	/// calling ofTexture::draw.
	///
	/// \sa http://www.opengl.org/sdk/docs/man4/html/glBindTexture.xhtml
	///
	void bind(int textureLocation=0) const;
	
	/// \brief Unbind the texture.
	///
	/// For advanced users who need to manually manage texture drawing without
	/// calling ofTexture::draw.
	///
	/// \sa http://www.opengl.org/sdk/docs/man4/html/glBindTexture.xhtml
	///
	void unbind(int textureLocation=0) const;

#ifndef TARGET_OPENGLES
	/// glBindImageTexture: https://www.opengl.org/wiki/GLAPI/glBindImageTexture
	///
	/// binds the texture as an read or write image, only available since 4.2
	void bindAsImage(GLuint unit, GLenum access, GLint level=0, GLboolean layered=0, GLint layer=0);
#endif

	void setAlphaMask(ofTexture & mask);
	void disableAlphaMask();

	/// \brief Helper to convert display coordinate to texture coordinate.
	/// \param xPos Horizontal position in pixels.
	/// \param yPos Vertical position in pixels.
	/// \returns Texture coordinate or (0, 0) if texture is not allocated.
	ofPoint getCoordFromPoint(float xPos, float yPos) const;		
	
	/// \brief Helper to convert display coordinate to texture coordinate.
	/// \param xPts Horizontal position in a normalized percentage (0 - 1).
	/// \param yPts Vertical position in a normalized percentage (0 - 1).
	/// \returns Texture coordinate or (0, 0) if texture is not allocated.
	ofPoint getCoordFromPercent(float xPts, float yPts) const;		
	
	/// \brief Set texture wrapping.
	///
	/// By default, textures are clamped to their edges with GL_CLAMP_TO_EDGE.
	/// Setting a repeat mode like GL_REPEAT allows you to create tiled
	/// backgrounds with small textures.
	///
	/// \sa ofTextureSetWrap()
	/// \sa http://www.opengl.org/sdk/docs/man4/html/glTexParameter.xhtml
	///
	/// \warning May be overridden.
	///
	/// \param wrapS wrap parameter for texture coordinate s.
	/// \param wrapT wrap parameter for texture coordinate t.
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

	/// Sets a texture matrix that will be uploaded whenever the texture is
	/// binded.
	void setTextureMatrix(const ofMatrix4x4 & m);

	/// Disable the texture matrix.
	void disableTextureMatrix();

	/// \brief Set the texture compression.
	///
	/// \warning: not yet implemented.
	/// \sa ofTexCompression
	void setCompression(ofTexCompression compression);

	/// \brief Sets flag allowing texture to auto-generate mipmap
	///
	/// By default, this will set your minFilter to GL_LINEAR_MIPMAP_LINEAR.
	/// If you want to change your minFilter later use setTextureMinMagFilter()
	///
	///	If you want to generate a mipmap later, or at a specific
	/// point in your code, use ofTexture::generateMipmap() instead.
	///
	/// \sa generateMipmap()
	/// \sa disableMipmap()
	/// \sa setTextureMinMagFilter()
	void enableMipmap();

	/// \brief Sets flag allowing texture to auto-generate mipmap
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
	
	/// \brief Has the texture been allocated?
	///
	/// Legacy function for backwards compatibility.
	///
	/// \returns true if the texture has been allocated.
	bool bAllocated() const;
	
	/// \brief Has the texture been allocated?
	/// \returns true if the texture has been allocated.
	bool isAllocated() const;

	/// \brief Internal texture data access.
	/// \returns a reference to the internal texture data struct.
	ofTextureData& getTextureData();

	/// \brief Const version of getTextureData().
	/// \sa ofTextureData::getTextureData()
	const ofTextureData& getTextureData() const;

	ofTextureData texData; ///< Internal texture data access.
	                       ///< For backwards compatibility.

	/// \brief Display height of texture.
	///
	/// Return value is pixel size (default) or normalized (0 - 1).
	///
	/// \sa ofEnabledNormalizedTextures()
	///
	/// \returns Display height of texture in pixels.
	float getHeight() const;

	/// \brief Display width of texture.
	///
	/// Return value is pixel size (default) or normalized (0 - 1).
	///
	/// \sa ofEnabledNormalizedTextures()
	///
	/// \returns Display width of texture in pixels.
	float getWidth() const;

protected:
    void loadData(const void * data, int w, int h, int glFormat, int glType);

	void enableTextureTarget(int textureLocation) const;
	void disableTextureTarget(int textureLocation) const;

	ofPoint anchor;
	bool bAnchorIsPct;

private:
	
	bool bWantsMipmap;
	
};
