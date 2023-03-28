//
//  ofCubeMap.h
//
//  Created by Nick Hardeman on 10/16/22.
//

#pragma once

#include "ofFbo.h"
#include "ofVboMesh.h"

class ofShader;
class ofGLProgrammableRenderer;

class ofCubeMap {
public:
	
	struct ofCubeMapSettings {
		std::string filePath = "";
		std::string cacheDirectory = "";
		
		bool overwriteCache = false;
		bool useCache = false;
		bool useLutTex = false;
		bool flipVertically = true;
				
		int resolution = 512;
#ifdef TARGET_OPENGLES
		int irradianceRes = 16;
		int preFilterRes = 64;
#else
		int irradianceRes = 32;
		int preFilterRes = 128;
#endif
	};
	
	class Data{
	public:
		GLuint cubeMapId = 0;
		bool bCubeMapAllocated = false;
		GLuint irradianceMapId = 0;
		bool bIrradianceAllocated = false;
		GLuint preFilteredMapId = 0;
		bool bPreFilteredMapAllocated = false;
		
		int index = -1;
		bool isEnabled = true;
		
		ofCubeMapSettings settings;
		
		int maxMipLevels = 5;
		float exposure = 1.0;
	};
    
	static GLenum getTextureTarget();
	static bool hasActiveCubeMap();
	static std::shared_ptr<ofCubeMap::Data> getActiveData();
	static void clearTextureData(std::shared_ptr<ofCubeMap::Data> adata);
	static void regenerateAllTextures();
	static const ofTexture& getBrdfLutTexture();
	
	ofCubeMap();
	ofCubeMap(const ofCubeMap & mom);
	ofCubeMap(ofCubeMap && mom);
	
	~ofCubeMap();
	
	/// \load an image and convert to cube map.
	/// \param apath path to the image to load.
	/// \param aFaceResolution resolution of the cube map image sides.
	/// \param aBFlipY flip the images upside down.
	bool load( std::string apath, int , bool aBFlipY=true );
	/// \load an image and convert to cube map.
	/// \param apath path to the image to load.
	/// \param aFaceResolution resolution of the cube map image sides.
	/// \param aBFlipY flip the images upside down.
	/// \param aIrradianceRes resolution of the irradiance map. (default is 32).
	/// \param aPreFilterRes resolution of the prefiltered map. (default is 128).
	bool load( std::string apath, int aFaceResolution, bool aBFlipY, int aIrradianceRes, int aPreFilterRes );
	
	bool load( ofCubeMapSettings aSettings );
	
	/// \section Update ofCubeMap
	/// \brief Copy a given ofCubeMap into this cube map.
	/// \param mom The ofCubeMap to copy from. Reuses internal GL texture IDs.
	ofCubeMap& operator=(const ofCubeMap & mom);
	ofCubeMap& operator=(ofCubeMap && mom);
	
	void clear();
	
	void draw();
	void drawCubeMap();
	void drawIrradiance();
	void drawPrefilteredCube(float aRoughness);
	
	bool isEnabled() { return data->isEnabled;}
	const bool isEnabled() const { return data->isEnabled;}
	void setEnabled(bool ab) {data->isEnabled=ab;}
	
	bool hasCubeMap();
	bool hasPrefilteredMap();
	bool hasIrradianceMap();
	
	int getFaceResolution() { return data->settings.resolution; }
	GLuint getTextureId();
	bool isHdr();
	
	void setExposure(float aExposure) {data->exposure=ofClamp(aExposure, 0.0f, 1.0f);}
	float getExposure() { return data->exposure; }
	
	void setUseBrdfLutTexture( bool ab );
	bool isUsingLutBrdfTexture() { return data->settings.useLutTex; }
	
	GLuint getIrradianceMapId() { return data->irradianceMapId; }
	GLuint getPrefilterMapId() { return data->preFilteredMapId; }
	
protected:
	
	std::shared_ptr<ofCubeMap::Data> data;
	
	void _drawCubeStart(GLuint aCubeMapId);
	void _drawCubeEnd();
	
	void _checkSetup();
	void _createCubeMap(ofTexture& aSrcTex);
	GLuint _createFloatCubeMap(ofTexture& aSrcTex, int aSrcRes);
	void _equiRectToCubeMap( GLuint& aCubeTexId, ofTexture& aSrcTex, int aSrcRes, bool aBConvertToNonFloat );
	
	void _createIrradianceMap(GLuint aSrcCubeFid, bool aBMakeCache, std::string aCachePath);
	bool _loadIrradianceMap(std::string aCachePath);
	
	void _createPrefilteredCubeMap(GLuint aSrcCubeFid, int aSrcRes, bool aBMakeCache, std::string aCachePath);
	bool _loadPrefilterMap( std::string aCachePath );
	
	static void _createBrdfLUT();
	
	
	void _configureCubeTextures(GLuint aCubeMapId,bool abLinearMipLinear);
	void _initEmptyTextures(GLuint aCubeMapId, int aSize);
	void _initEmptyTextures(GLuint aCubeMapId, GLuint aInternalFormat, int aSize, int aNumMipMaps );
	void _allocateCubeMesh();
	std::vector<glm::mat4> _getViewMatrices(const glm::vec3& apos);
	
	bool _loadRenderShader();
	bool _loadEquiRectToCubeMapShader();
	
	GLuint getTexStorageFormat();
	GLuint getTexStorageFormat( GLuint aInternalFormat );
	
	GLuint getGlTypeFromInternalFormat();
	GLuint getGlTypeFromInternalFormat(GLuint aInternalFormat);
	int getNumPixelChannels();
	
	GLuint texFormat = GL_RGB;
	
	glm::mat4 projectionMat;
	
	static ofTexture sBrdfLutTex;
	static ofVboMesh sCubeMesh;
	//ofMesh sCubeMesh;
	ofShader shaderEquiRectToCubeMap;
	ofShader shaderRender;
	ofShader shaderIrradianceMap;
	ofShader shaderPreFilterMap;
	static ofShader shaderBrdfLUT;
	
};

std::vector<std::weak_ptr<ofCubeMap::Data> > & ofCubeMapsData();
