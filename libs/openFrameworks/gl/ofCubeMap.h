#pragma once

#include "ofShader.h"

#if !defined(GLM_FORCE_CTOR_INIT)
	#define GLM_FORCE_CTOR_INIT
#endif
#if !defined(GLM_ENABLE_EXPERIMENTAL)
	#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/mat4x4.hpp>

class ofVboMesh;
class ofGLProgrammableRenderer;
class ofMaterial;


struct ofCubeMapSettings {
	of::filesystem::path filePath { "" }; ///< path to load the cube map, a single texture
	of::filesystem::path cacheDirectory { "" }; ///< directory to cache the irradiance and pre filter maps
	
	bool overwriteCache = false; ///< overwrite the cache of the irradiance and pre filter maps
	bool useCache = false; ///< check for cached irradiance and pre filter images on disk
	bool useLutTex = false; ///< create a lut texture to pass to the shaders
	bool flipVertically = true; ///< flip the loaded cubemap image vertically
	bool useMaximumPrecision = false; ///< use 32 bit float (GL_RGB32F) precision, default is 16 bit (GL_RGB16F)
	
	int resolution = 512; ///< resolution of each side of the cube map to be generated
#ifdef TARGET_OPENGLES
	int irradianceRes = 16; ///< resolution of each side of the irradiance map to be generated
	int preFilterRes = 64;///< resolution of each side of the pre filter map to be generated
#else
	int irradianceRes = 32; ///< resolution of each side of the irradiance map to be generated
	int preFilterRes = 128; ///< resolution of each side of the pre filter map to be generated
#endif
};

class ofCubeMap {
protected:
	
	/// \class ofCubeMap::Data
	///
	/// \brief Data class structure for storing GL texture ids and other data regarding cube maps
	/// Used internally for access to cube map data in the rendering pipeline, ie. ofMaterial.
	/// A Data class is created for each instance of ofCubeMap and all of them can be accessed by ofCubeMapsData().
	/// Which is declared towards the bottom of this file.
	class Data {
	public:
		GLuint cubeMapId = 0; ///< GL texture id of the cube map
		bool bCubeMapAllocated = false;
		GLuint irradianceMapId = 0; ///< GL texture id of the irradiance map
		bool bIrradianceAllocated = false;
		GLuint preFilteredMapId = 0; ///< GL texture id of the pre filter map
		bool bPreFilteredMapAllocated = false;
		
		int index = -1;
		bool isEnabled = true;
		
		ofCubeMapSettings settings; ///< settings associated with this cube map
		
		int maxMipLevels = 5; ///< number of mip map levels for generating the prefilted map
		float exposure = 1.0; ///< used for storing the exposure of the cube map, which is set with ofCubeMap::setExposure()
	};
	
	/// \brief Global storage for all cube maps created. The Data class holds infomation about textures and other information
	/// relevant for rendering. This function is utilized internally.
	/// More about the Data class above.
	/// \return std::vector<std::weak_ptr<ofCubeMap::Data>>&.
	static std::vector<std::weak_ptr<ofCubeMap::Data>>& getCubeMapsData();
	
	/// \brief Is there any active cube map.
	/// \return true if there is any active cube map.
	static bool hasActiveCubeMap();
	/// \brief Get the first cube map data that is enabled.
	/// \return std::shared_ptr<ofCubeMap::Data> of the active cube map data.
	static std::shared_ptr<ofCubeMap::Data> getActiveData();
	/// \brief Internal function for releasing the GL texture memory.
	/// \param std::shared_ptr<ofCubeMap::Data> cube map data holding texture ids to clear.
	static void clearTextureData(std::shared_ptr<ofCubeMap::Data> adata);
	#ifdef TARGET_ANDROID
	static void regenerateAllTextures();
	#endif
	static const ofTexture & getBrdfLutTexture();
	
	friend class ofMaterial;
	friend class ofShader;
	
public:
	
	/// \brief Return the GL texture type.
	/// \return GLenum GL_TEXTURE_CUBE_MAP.
	static GLenum getTextureTarget();

	ofCubeMap();
	ofCubeMap(const ofCubeMap & mom);
	ofCubeMap(ofCubeMap && mom);

	~ofCubeMap();

	/// \load An image and convert to cube map.
	/// \param apath path to the image to load.
	/// \param aFaceResolution resolution of the cube map image sides.
	/// \param aBFlipY flip the images upside down.
	bool load(const of::filesystem::path & apath, int aFaceResolution, bool aBFlipY = true);
	/// \load An image and convert to cube map.
	/// \param apath path to the image to load.
	/// \param aFaceResolution resolution of the cube map image sides.
	/// \param aBFlipY flip the images upside down.
	/// \param aIrradianceRes resolution of the irradiance map. (default is 32).
	/// \param aPreFilterRes resolution of the prefiltered map. (default is 128).
	bool load(const of::filesystem::path & apath, int aFaceResolution, bool aBFlipY, int aIrradianceRes, int aPreFilterRes);

	bool load(ofCubeMapSettings aSettings);

	/// \section Update ofCubeMap
	/// \brief Copy a given ofCubeMap into this cube map.
	/// \param mom The ofCubeMap to copy from. Reuses internal GL texture IDs.
	ofCubeMap & operator=(const ofCubeMap & mom);
	ofCubeMap & operator=(ofCubeMap && mom);
	/// \brief Clear the GL textures.
	void clear();
	/// \brief Calls drawCubeMap();
	void draw();
	/// \brief Render the cube map.
	void drawCubeMap();
	/// \brief Render the irradiance map.
	void drawIrradiance();
	/// \brief Render prefilted map.
	/// \param aRoughness from 0 - 1.
	void drawPrefilteredCube(float aRoughness);
	
	bool isEnabled() { return data->isEnabled; }
	bool isEnabled() const { return data->isEnabled; }
	void setEnabled(bool ab) { data->isEnabled = ab; }

	bool hasCubeMap();
	bool hasPrefilteredMap();
	bool hasIrradianceMap();

	int getFaceResolution() { return data->settings.resolution; }
	/// \brief Get the GL texture id of the cube map.
	/// \return GLuint.
	GLuint getTextureId();
	/// \brief Does this platform support HDR.
	/// \return bool.
	bool doesSupportHdr();
	/// \brief Is this cube map HDR.
	/// \return bool.
	bool isHdr();
	/// \brief Are the textures GL_RGB16F.
	/// \return bool.
	bool isMediumPrecision();
	/// \brief Set the exposure of the cube map.
	/// \param aExposure from 0 - 1.
	void setExposure(float aExposure);
	/// \brief Get the exposure of the cube map.
	/// \return float from 0 - 1.
	float getExposure() { return data->exposure; }
	/// \brief Use as precalculated BRDF Lut texture.
	/// \param ab bool to use texture.
	void setUseBrdfLutTexture(bool ab);
	/// \brief Is Using BRDF Lut texture.
	/// \return bool.
	bool isUsingLutBrdfTexture() { return data->settings.useLutTex; }
	
	/// \brief Get the texture id of the irradiance map.
	/// \return GLuint, texture id.
	GLuint getIrradianceMapId() { return data->irradianceMapId; }
	/// \brief Get the texture id of the pre filtered map.
	/// \return GLuint, texture id.
	GLuint getPrefilterMapId() { return data->preFilteredMapId; }

protected:
	std::shared_ptr<ofCubeMap::Data> data;

	void _drawCubeStart(GLuint aCubeMapId);
	void _drawCubeEnd();

	void _checkSetup();
	void _createCubeMap(GLuint aSrcTexId);
	GLuint _createFloatCubeMap(GLuint aSrcTexId, int aSrcRes);
	void _equiRectToCubeMap(GLuint & aCubeTexId, GLuint aSrcTexId, int aSrcRes, bool aBConvertToNonFloat);

	void _createIrradianceMap(GLuint aSrcCubeFid, bool aBMakeCache, const of::filesystem::path & aCachePath);
	bool _loadIrradianceMap(const of::filesystem::path & aCachePath);

	void _createPrefilteredCubeMap(GLuint aSrcCubeFid, int aSrcRes, bool aBMakeCache, const of::filesystem::path & aCachePath);
	bool _loadPrefilterMap(const of::filesystem::path & aCachePath);

	static void _createBrdfLUT();

	void _configureCubeTextures(GLuint aCubeMapId, bool abLinearMipLinear);
	void _initEmptyTextures(GLuint aCubeMapId, int aSize);
	void _initEmptyTextures(GLuint aCubeMapId, GLint aInternalFormat, int aSize, int aNumMipMaps);
	void _allocateCubeMesh();
	std::vector<glm::mat4> _getViewMatrices(const glm::vec3 & apos);

	bool _loadRenderShader();
	bool _loadEquiRectToCubeMapShader();

	/// \brief get the GL format from the internalFormat, ie. GL_RGB, GL_RGBA
	/// \return the GL format as GLenum
	GLenum getGLFormatFromInternalFormat();
	GLenum getGLFormatFromInternalFormat(GLint aInternalFormat);

	/// \brief get the GL type from the internalFormat, ie. GL_FLOAT
	/// \return the GL type as GLenum
	GLenum getGLTypeFromInternalFormat();
	GLenum getGLTypeFromInternalFormat(GLint aInternalFormat);

	int getNumPixelChannels();

	GLint mGLInternalFormat;

	glm::mat4 projectionMat;

	static ofTexture sBrdfLutTex;
	static ofVboMesh sCubeMesh;
	ofShader shaderEquiRectToCubeMap;
	ofShader shaderRender;
	ofShader shaderIrradianceMap;
	ofShader shaderPreFilterMap;
	static ofShader shaderBrdfLUT;
};

