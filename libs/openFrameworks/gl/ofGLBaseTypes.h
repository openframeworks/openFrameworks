#pragma once


#include <vector>
#include "ofGraphicsBaseTypes.h"

class ofTexture;
class ofShader;

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
	virtual ~ofBaseImage_<T>(){};
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
	virtual void draw(const ofTexture & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const=0;
	virtual void draw(const ofVbo & vbo, GLuint drawMode, int first, int total) const=0;
	virtual void drawElements(const ofVbo & vbo, GLuint drawMode, int amt, int offsetelements) const=0;
	virtual void drawInstanced(const ofVbo & vbo, GLuint drawMode, int first, int total, int primCount) const=0;
	virtual void drawElementsInstanced(const ofVbo & vbo, GLuint drawMode, int amt, int primCount) const=0;
	virtual void draw(const ofVboMesh & mesh, ofPolyRenderMode renderType) const=0;
	virtual void drawInstanced(const ofVboMesh & mesh, ofPolyRenderMode renderType, int primCount) const=0;


	virtual void enableTextureTarget(const ofTexture & tex, int textureLocation)=0;
	virtual void disableTextureTarget(int textureTarget, int textureLocation)=0;
	virtual void setAlphaMaskTex(const ofTexture & tex)=0;
	virtual void disableAlphaMask()=0;
	virtual void enablePointSprites()=0;
	virtual void disablePointSprites()=0;

	// lighting
	virtual void enableLighting()=0;
	virtual void disableLighting()=0;
	virtual void enableSeparateSpecularLight()=0;
	virtual void disableSeparateSpecularLight()=0;
	virtual bool getLightingEnabled()=0;
	virtual void setSmoothLighting(bool b)=0;
	virtual void setGlobalAmbientColor(const ofColor& c)=0;
	virtual void enableLight(int lightIndex)=0;
	virtual void disableLight(int lightIndex)=0;
	virtual void setLightSpotlightCutOff(int lightIndex, float spotCutOff)=0;
	virtual void setLightSpotConcentration(int lightIndex, float exponent)=0;
	virtual void setLightAttenuation(int lightIndex, float constant, float linear, float quadratic )=0;
	virtual void setLightAmbientColor(int lightIndex, const ofFloatColor& c)=0;
	virtual void setLightDiffuseColor(int lightIndex, const ofFloatColor& c)=0;
	virtual void setLightSpecularColor(int lightIndex, const ofFloatColor& c)=0;
	virtual void setLightPosition(int lightIndex, const glm::vec4 & position)=0;
	virtual void setLightSpotDirection(int lightIndex, const glm::vec4 & direction)=0;

	virtual int getGLVersionMajor()=0;
	virtual int getGLVersionMinor()=0;

	virtual void saveScreen(int x, int y, int w, int h, ofPixels & pixels)=0;
	virtual void saveFullViewport(ofPixels & pixels)=0;

	// bindings
	using ofBaseRenderer::bind;
	using ofBaseRenderer::unbind;
	virtual void bind(const ofBaseMaterial & material)=0;
	virtual void bind(const ofShader & shader)=0;
	virtual void bind(const ofTexture & texture, int location)=0;
	virtual void bind(const ofBaseVideoDraws & video)=0;
	virtual void unbind(const ofBaseMaterial & material)=0;
	virtual void unbind(const ofShader & shader)=0;
	virtual void unbind(const ofTexture & texture, int location)=0;
	virtual void unbind(const ofBaseVideoDraws & video)=0;
	virtual void bind(const ofFbo & fbo)=0;
	virtual void unbind(const ofFbo & fbo)=0;
#ifndef TARGET_OPENGLES
	virtual void bindForBlitting(const ofFbo & fboSrc, ofFbo & fboDst, int attachmentPoint=0)=0;
#endif
	virtual void begin(const ofFbo & fbo, ofFboMode mode)=0;
	virtual void end(const ofFbo & fbo)=0;

};
