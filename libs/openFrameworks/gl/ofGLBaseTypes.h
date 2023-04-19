#pragma once

#include "ofGraphicsBaseTypes.h"
#include <vector>

class ofTexture;
class ofShader;
class ofShadow;

/// \brief An abstract class representing an object that can have an ofTexture.
class ofBaseHasTexture{
public:
	/// \brief Destroy the ofBaseHasTexture.
	virtual ~ofBaseHasTexture(){}

	/// \returns a reference to the ofTexture.
	virtual ofTexture & getTexture()=0;

	/// \returns a const reference to the ofTexture.
	virtual const ofTexture & getTexture() const=0;

	/// \brief Enable or disable internal ofTexture use.
	/// \param bUseTex true if an ofTexture should be used.
	virtual void setUseTexture(bool bUseTex)=0;

	/// \returns true if an internal ofTexture is being used.
	virtual bool isUsingTexture() const=0;
};


/// \brief An abstract class representing an object that ofTexture planes.
class ofBaseHasTexturePlanes: public ofBaseHasTexture{
public:
	/// \brief Destroy the ofBaseHasTexturePlanes.
	virtual ~ofBaseHasTexturePlanes(){}

	/// \returns a reference to a std::vector containing the ofTexture planes.
	virtual std::vector<ofTexture> & getTexturePlanes()=0;

	/// \returns a const reference to a std::vector containing the ofTexture planes.
	virtual const std::vector<ofTexture> & getTexturePlanes() const=0;
};


/// \brief An abstract class representing an image.
///
/// This empty class primarily exists to allow templated subclasses of different
/// types to be stored as raw or shared pointers in collections such as
/// std::vector.
///
/// Example:
/// \code{.cpp}
///
/// std::vector<ofAbstractImage*> imageProviders;
///
/// ofImage image;
/// ofFloatImage floatImage;
/// ofShortImage shortImage;
///
/// // ...
///
/// imageProviders.push_back(&image);
/// imageProviders.push_back(&floatImage);
/// imageProviders.push_back(&shortImage);
///
/// \endcode
class ofAbstractImage: public ofBaseDraws, public ofBaseHasTexture{
public:
	/// \brief Destroy the ofAbstractImage.
	virtual ~ofAbstractImage(){}
};

/// \brief A base class represeting an image.
/// \tparam T The pixel data type.
template<typename T>
class ofBaseImage_: public ofAbstractImage, virtual public ofBaseHasPixels_<T>{
public:
	/// \brief Destroy the ofBaseImage_.
	virtual ~ofBaseImage_(){};
};


/// \brief A typedef for an unsigned char ofBaseImage_.
typedef ofBaseImage_<unsigned char> ofBaseImage;

/// \brief A typedef for an float ofBaseImage_.
typedef ofBaseImage_<float> ofBaseFloatImage;

/// \brief A typedef for an unsigned short ofBaseImage_.
typedef ofBaseImage_<unsigned short> ofBaseShortImage;

class of3dPrimitive;


class ofBaseGLRenderer: public ofBaseRenderer{
public:
	using ofBaseRenderer::draw;

	/// \section Drawing

	/// \brief Draw a texture with this renderer.
	///
	/// \param image The texture to draw with this renderer.
	/// \param x The x coordinate to use when drawing the texture with this
	///        renderer.
	/// \param y The y coordinate to use to draw the texture with this renderer.
	/// \param z The z coordinate to use to draw the texture with this renderer.
	/// \param w The width to use to draw the the texture with this renderer.
	/// \param h The height to use to draw the the texture with this renderer.
	/// \param sx The subsection x axis offset within the texture.
	/// \param sy The subsection y axis offset within the texture.
	/// \param sw The subsection width offset within the texture.
	/// \param sh The subsection height offset within the texture.
	virtual void draw(const ofTexture & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const=0;

	/// \brief Draw vertices from a vertext buffer with this renderer.
	///
	/// \p drawMode may be any of the OpenGL primitive draw modes:
	///    GL_POINTS,
	///    GL_LINE_STRIP,
	///    GL_LINE_LOOP,
	///    GL_LINES,
	///    GL_TRIANGLE_STRIP,
	///    GL_TRIANGLE_FAN,
	///    GL_TRIANGLES
	///
	/// ofGetGLPrimitiveMode() can also be used to get the GLuint draw mode
	/// from an ofPrimitiveMode.
	///
	/// \param vbo The vertext buffer object to draw vertices from with this
	/// renderer.
	/// \param drawMode OpenGL primitive draw mode to use when drawing the vbo's
	/// vertices with this renderer.
	/// \param first The index of the first vertex to draw from \p vbo.
	/// \param total The total number of indices to draw from \p vbo.
	/// \sa ofGetGLPrimitiveMode()
	/// \sa glDrawArrays()
	/// \sa https://www.khronos.org/opengles/sdk/docs/man/xhtml/glDrawArrays.xml
	virtual void draw(const ofVbo & vbo, GLuint drawMode, int first, int total) const=0;

	/// \brief Draw vertices from a vertext buffer with this renderer.
	///
	/// \p drawMode may be any of the OpenGL primitive draw modes:
	///    GL_POINTS,
	///    GL_LINE_STRIP,
	///    GL_LINE_LOOP,
	///    GL_LINES,
	///    GL_TRIANGLE_STRIP,
	///    GL_TRIANGLE_FAN,
	///    GL_TRIANGLES
	///
	/// ofGetGLPrimitiveMode() can also be used to get the GLuint draw mode
	/// from an ofPrimitiveMode.
	///
	/// \param vbo The vertext buffer object to draw vertices from with this
	///        renderer.
	/// \param drawMode OpenGL primitive draw mode to use when drawing the vbo's
	///        vertices with this renderer.
	/// \param amt The number of elements to be rendered.
	/// \param offsetelements The number of elements (indices) offset from the
	///        first element to start drawing from.
	/// \sa ofGetGLPrimitiveMode()
	/// \sa glDrawElements()
	/// \sa http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-9-vbo-indexing/
	/// \sa https://www.opengl.org/sdk/docs/man/html/glDrawElements.xhtml
	virtual void drawElements(const ofVbo & vbo, GLuint drawMode, int amt, int offsetelements) const=0;

	/// \brief Draw vertices from a vertext buffer using instanced arrays.
	///
	/// \p drawMode may be any of the OpenGL primitive draw modes:
	///    GL_POINTS,
	///    GL_LINE_STRIP,
	///    GL_LINE_LOOP,
	///    GL_LINES,
	///    GL_TRIANGLE_STRIP,
	///    GL_TRIANGLE_FAN,
	///    GL_TRIANGLES
	///
	/// ofGetGLPrimitiveMode() can also be used to get the GLuint draw mode
	/// from an ofPrimitiveMode.
	///
	/// \param vbo The vertext buffer object to draw vertices from with this
	///        renderer.
	/// \param drawMode OpenGL primitive draw mode to use when drawing the vbo's
	///        vertices with this renderer.
	/// \param first The index of the first vertex to draw from \p vbo.
	/// \param total The total number of indices to draw from \p vbo.
	/// \param primCount Specifies the number of instances of the specified
	///        range of indices to be rendered.
	/// \warning This method may be unsupported when using OpenGLES.
	/// \sa ofGetGLPrimitiveMode()
	/// \sa glDrawArraysInstanced()
	/// \sa https://www.khronos.org/opengles/sdk/docs/man3/html/glDrawArraysInstanced.xhtml
	virtual void drawInstanced(const ofVbo & vbo, GLuint drawMode, int first, int total, int primCount) const=0;

	/// \brief Draw vertices from a vertex buffer using this renderer.
	///
	/// \p drawMode may be any of the OpenGL primitive draw modes:
	///    GL_POINTS,
	///    GL_LINE_STRIP,
	///    GL_LINE_LOOP,
	///    GL_LINES,
	///    GL_TRIANGLE_STRIP,
	///    GL_TRIANGLE_FAN,
	///    GL_TRIANGLES
	///
	/// ofGetGLPrimitiveMode() can also be used to get the GLuint draw mode
	/// from an ofPrimitiveMode.
	///
	/// \param vbo The vertext buffer object to draw vertices from with this
	///        renderer.
	/// \param drawMode OpenGL primitive draw mode to use when drawing the vbo's
	///        vertices with this renderer.
	/// \param amt The number of elements to be rendered.
	/// \param primCount Specifies the number of instances of the specified
	///        range of indices to be rendered.
	/// \warning This method may be unsupported when using OpenGLES.
	/// \sa ofGetGLPrimitiveMode()
	/// \sa glDrawElementsInstanced()
	/// \sa https://www.opengl.org/sdk/docs/man/html/glDrawElementsInstanced.xhtml
	virtual void drawElementsInstanced(const ofVbo & vbo, GLuint drawMode, int amt, int primCount) const=0;

	/// \brief Draw a vertex buffer mesh using a specific poly render mode.
	///
	/// \p renderType defines how the \p mesh will be rendered and may be:
	///    OF_MESH_POINTS,
	///    OF_MESH_WIREFRAME,
	///    OF_MESH_FILL
	/// \param mesh The vertex buffer mesh to draw with this renderer.
	/// \param renderType The poly render mode to use when drawing \p mesh with
	///        this renderer.
	/// \sa ofPolyRenderMode
	virtual void draw(const ofVboMesh & mesh, ofPolyRenderMode renderType) const=0;

	/// \brief Draw a vertex buffer mesh with instancing using a specific poly
	/// render mode.
	///
	/// \p renderType defines how the \p mesh will be rendered and may be:
	///    OF_MESH_POINTS,
	///    OF_MESH_WIREFRAME,
	///    OF_MESH_FILL
	///
	/// \param mesh The vertext buffer mesh to draw vertices from with this
	///        renderer.
	/// \param renderType The poly render mode to use when drawing \p mesh with
	///        this renderer.
	/// \param primCount Specifies the number of instances of the specified
	/// range of indices to be rendered.
	virtual void drawInstanced(const ofVboMesh & mesh, ofPolyRenderMode renderType, int primCount) const=0;

	/// \section Textures

	/// \brief Bind the texture at the texture location.
	///
	/// \param tex The texture to bind with this renderer.
	/// \param textureLocation The location to bind this texture.
	virtual void enableTextureTarget(const ofTexture & tex, int textureLocation)=0;

	/// \brief Unbind the texture target at the texture location.
	///
	/// \p textureTarget can be accessed from an ofTexture with
	/// ofTexture::texData::textureTarget.
	///
	/// \param textureTarget The texture target to unbind from this renderer.
	/// \param textureLocation The location that texture was bound with.
	/// \sa ofTextureData()
	virtual void disableTextureTarget(int textureTarget, int textureLocation)=0;

	/// \brief Enable alpha masking using this texture as the mask source.
	///
	/// \p tex should be a grayscale image whose pixels will be used to set the
	/// alpha value of the previously bound texture or framebuffer. Darker
	/// pixels cause transparency in the masked image. Disable a mask being
	/// used in this way with disableAlphaMask().
	///
	/// \param tex The texture to use as the alpha mask.
	/// \sa disableAlphaMask()
	virtual void setAlphaMaskTex(const ofTexture & tex)=0;

	/// \brief Disable this renderer's current alpha mask texture.
	///
	/// \sa setAlphaMaskTex()
	virtual void disableAlphaMask()=0;

	/// \section Point Sprites

	/// \brief Enable point sprites when using this renderer.
	virtual void enablePointSprites()=0;

	/// \brief Disable point sprites when using this renderer.
	virtual void disablePointSprites()=0;

	/// \section Lighting

	/// \brief Enable lighting with this renderer.
	virtual void enableLighting()=0;

	/// \brief Disable lighting with this renderer.
	virtual void disableLighting()=0;

	/// \brief Enable seperate specular lighting parameters.
	virtual void enableSeparateSpecularLight()=0;

	/// \brief Disable seperate specular lighting parameters.
	virtual void disableSeparateSpecularLight()=0;

	/// \brief Returns true if lighting is enabled with this renderer.
	///
	/// \returns True if lighting is enabled with this renderer.
	/// \sa enableLighting()
	/// \sa disableLighting()
	virtual bool getLightingEnabled()=0;

	/// \brief Enable/disable smooth light shading.
	///
	/// \param b True to enable smooth light shading.
	virtual void setSmoothLighting(bool b)=0;

	/// \brief Set the global ambient light color.
	///
	/// \param c The color to set this renderer to use as ambient lighting.
	virtual void setGlobalAmbientColor(const ofColor& c)=0;

	/// \brief Enable a light at a specific index.
	///
	/// \param lightIndex The index of the light to enable.
	virtual void enableLight(int lightIndex)=0;

	/// \brief Disable a light at a specific index.
	///
	/// \param lightIndex The index of the light to disable.
	virtual void disableLight(int lightIndex)=0;

	/// \brief Set the spot light cutoff for a light at a specific index.
	///
	/// \param lightIndex The index of the light whose spot cutoff will be set.
	/// \param spotCutOff The spot light cut off value.
	/// \sa GL_SPOT_CUTOFF
	virtual void setLightSpotlightCutOff(int lightIndex, float spotCutOff)=0;

	/// \brief Set the spotlight concentration (exponent) for a light at a
	///        specific index.
	/// \param lightIndex The index of the light whose spot concentration
	///        will be set.
	/// \param exponent The spot light exponent value.
	/// \sa GL_SPOT_EXPONENT
	virtual void setLightSpotConcentration(int lightIndex, float exponent)=0;

	/// \brief Set the light attenuation for a light at a specific index.
	///
	/// \param lightIndex The index of the light whose spot concentration
	///        will be set.
	/// \param constant Set the constant attenuation factor of the light at
	///        \p lightIndex.
	/// \param linear Set the linear attenuation factor of the light at
	///        \p lightIndex.
	/// \param quadratic Set the quadratic attenuation factor of the light at
	///        \p lightIndex.
	/// \sa glLightf()
	/// \sa GL_CONSTANT_ATTENUATION
	/// \sa GL_LINEAR_ATTENUATION
	/// \sa GL_QUADRATIC_ATTENUATION
	virtual void setLightAttenuation(int lightIndex, float constant, float linear, float quadratic )=0;

	/// \brief Set the ambient light color for a light at a specific index.
	///
	/// \param lightIndex The index of the light to set the ambient color of.
	/// \param c The color to set the ambient light.
	virtual void setLightAmbientColor(int lightIndex, const ofFloatColor& c)=0;

	/// \brief Set the diffues light color for a light at a specific index.
	///
	/// \param lightIndex The index of the light to set the diffuse color of.
	/// \param c The color to set the diffuse light.
	virtual void setLightDiffuseColor(int lightIndex, const ofFloatColor& c)=0;

	/// \brief Set the specular light color for a light at a specific index.
	///
	/// \param lightIndex The index of the light to set the specular color of.
	/// \param c The color to set the specular light.
	virtual void setLightSpecularColor(int lightIndex, const ofFloatColor& c)=0;

	/// \brief Set the position of a light at a specific index.
	///
	/// \param lightIndex The index of the light to set the position color of.
	/// \param position A vector of four values that specify the position of the light in homogeneous object coordinates.
	/// \sa GL_POSITION
	virtual void setLightPosition(int lightIndex, const glm::vec4 & position)=0;

	/// \brief Set the spot direction of a light at a specific index.
	///
	/// \param lightIndex The index of the light to set the spot direction of.
	/// \param direction A vector of four values that specify the direction of
	///        the light in homogeneous object coordinates.
	/// \sa GL_SPOT_DIRECTION
	virtual void setLightSpotDirection(int lightIndex, const glm::vec4 & direction)=0;

	/// \section GL Version

	/// \brief Get the major OpenGL version number this renderer is using.
	///
	/// \returns The major OpenGL version number this renderer is using.
	virtual int getGLVersionMajor()=0;

	/// \brief Get the minor OpenGL version number this renderer is using.
	///
	/// \returns The minor OpenGL version number this renderer is using.
	virtual int getGLVersionMinor()=0;

	/// \section Saving Screen Pixels

	/// \brief Fill an ofPixels object with a subsection of the current screen.
	///
	/// \param x The x coordinate to use when specifying the bounds of the
	///        subsection of the screen to save into \p pixels.
	/// \param y The y coordinate to use when specifying the bounds of the
	///        subsection of the screen to save into \p pixels.
	/// \param w The width to use when specifying the bounds of the
	///        subsection of the screen to save into \p pixels.
	/// \param h The height to use when specifying the bounds of the
	///        subsection of the screen to save into \p pixels.
	/// \param pixels The pixels object to fill with the pixels from the screen.
	virtual void saveScreen(int x, int y, int w, int h, ofPixels & pixels)=0;

	/// \brief Fill an ofPixels object with the contents of the current screen.
	///
	/// \param pixels The pixels object to fill with the pixels from the screen.
	virtual void saveFullViewport(ofPixels & pixels)=0;

	using ofBaseRenderer::bind;
	using ofBaseRenderer::unbind;

	/// \section Binding/unbinding
	/// \brief Bind a material to be used with this renderer.
	///
	/// The material bound with this method will be used by this renderer until
	/// it is unbound with unbind().
	///
	/// \param material The material to bind to this renderer.
	/// \sa glMaterialfv()
	virtual void bind(const ofBaseMaterial & material)=0;

	virtual void bind(const ofShadow & shadow)=0;
	virtual void bind(const ofShadow & shadow, GLenum aCubeFace)=0;

	/// \brief Bind a shader to be used with this renderer.
	///
	/// The shader bound with this method will be used by this renderer until
	/// it is unbound with unbind().
	///
	/// \param shader The shader to bind to this renderer.
	/// \sa glUseProgram()
	virtual void bind(const ofShader & shader)=0;

	/// \brief Bind a texture to be used with this renderer at a location.
	///
	/// The texture bound with this method will be used by this renderer until
	/// it is unbound with unbind().
	///
	/// \param texture The texture to bind with this renderer.
	/// \param location The texture location to bind this texture to.
	virtual void bind(const ofTexture & texture, int location)=0;

	/// \brief Bind a video's texture to this renderer.
	///
	/// The video bound with this method will be used by this renderer
	/// until it is unbound with unbind().
	///
	/// \param video The video whose texture should be bound to this renderer.
	virtual void bind(const ofBaseVideoDraws & video)=0;

	/// \brief Unbind a material previously bound to this renderer with bind().
	///
	/// \param material The material that is currently bound to this renderer.
	virtual void unbind(const ofBaseMaterial & material)=0;

	virtual void unbind(const ofShadow & shadow)=0;
	virtual void unbind(const ofShadow & shadow, GLenum aCubeFace)=0;

	/// \brief Unbind a shader previously bound to this renderer with bind().
	///
	/// \param shader The shader that is currently bound to this renderer.
	virtual void unbind(const ofShader & shader)=0;

	/// \brief Unbind a texture previously bound to this renderer with bind().
	///
	/// \param texture The texture that is currently bound to this renderer.
	/// \param location The location \p texture was bound at.
	virtual void unbind(const ofTexture & texture, int location)=0;

	/// \brief Unbind a video previously bound to this renderer with bind().
	///
	/// \param video The video that is currently bound to this renderer.
	virtual void unbind(const ofBaseVideoDraws & video)=0;

	/// \brief Bind a frame buffer to this renderer.
	///
	/// The fbo bound with this method will be used as this renderer
	/// until it is unbound with unbind().
	///
	/// \param fbo The frame buffer whose texture should be bound to this
	/// renderer.
	virtual void bind(const ofFbo & fbo)=0;

	/// \brief Unbind a frame buffer previously bound to this renderer with bind().
	///
	/// \param fbo The frame buffer that is currently bound to this renderer.
	virtual void unbind(const ofFbo & fbo)=0;

#ifndef TARGET_OPENGLES
	/// \brief Bind source and destination frame buffers for blitting.
	///
	/// \param fboSrc The source frame buffer to bind for blitting.
	/// \param fboDst The destination frame buffer to bind for blitting.
	/// \param attachmentPoint The attatchement point to use when binding.
	/// \warning This method is unavailable when using OpenGLES.
	/// \sa https://en.wikipedia.org/wiki/Bit_blit
	virtual void bindForBlitting(const ofFbo & fboSrc, ofFbo & fboDst, int attachmentPoint=0)=0;
#endif

	/// \section Setting The Render Surface

	/// \brief Begin using a frame buffer as this renderer's render surface.
	///
	/// The fbo used with this method will be used by this renderer
	/// until it is unbound with unbind().
	///
	/// \param fbo The frame buffer to use as this renderer's render surface.
	/// \param mode The fbo mode to use.
	/// \sa ofFboMode
	virtual void begin(const ofFbo & fbo, ofFboMode mode)=0;

	/// \brief Stop using the specified frame buffer as this renderer's render
	/// surface.
	///
	/// \param fbo An fbo that is currently being used as this renderer's render
	/// surface with a call to begin().
	virtual void end(const ofFbo & fbo)=0;

};
