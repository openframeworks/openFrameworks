//
//  ofCubeMap.h
//
//  Created by Nick Hardeman on 10/16/22.
//

#pragma once
#include "ofShader.h"
#include "ofNode.h"
#include "ofFbo.h"


class ofGLProgrammableRenderer;

class ofCubeMap {
public:
	
	class Data{
	public:
		GLuint cubeMapId = 0;
		bool bCubeMapAllocated = false;
		GLuint irradianceMapId = 0;
		bool bIrradianceAllocated = false;
		GLuint preFilteredMapId = 0;
		bool bPreFilteredMapAllocated = false;
		//ofTexture brdfLutTex;
		
		int index = -1;
		bool isEnabled = true;
		bool useLutTex = false;
		
		int resolution = 1024;
		int irradianceRes = 64;
		int preFilterRes = 256;
		int maxMipLevels = 5;
	};
    
	static GLenum getTextureTarget();
	static bool hasActiveCubeMap();
	static std::shared_ptr<ofCubeMap::Data> getActiveData();
	static const ofTexture& getBrdfLutTexture();
	
	ofCubeMap();
	~ofCubeMap();
	
	bool load( std::string apath, int aFaceResolution, bool aBFlipY=true );
	
//	void set( const ofImage& aPosX, const ofImage& aNegX, const ofImage& aPosY, const ofImage& aNegY, const ofImage& aPosZ, const ofImage& aNegZ );
//	void set( const ofFloatImage& aPosX, const ofFloatImage& aNegX, const ofFloatImage& aPosY, const ofFloatImage& aNegY, const ofFloatImage& aPosZ, const ofFloatImage& aNegZ );
	
	void draw();
	void drawCubeMap();
	void drawIrradiance();
	void drawPrefilteredCube(float aRoughness);
	
	void beginDrawingIntoFace( int aCubeFace );
	void endDrawingIntoFace( int aCubeFace );
	
	void begin3DDrawingIntoFace( int aCubeFace );
	void end3DDrawingIntoFace( int aCubeFace );
	
	int getFaceResolution() { return data->resolution; }
	GLuint getTextureId();
	bool isHdr();
	
	void setExposure(float aExposure) {mExposure=aExposure;}
	float getExposure() { return mExposure; }
	
	void setUseBrdfLutTexture( bool ab );
	bool isUsingLutBrdfTexture() { return data->useLutTex; }
	
	GLuint getIrradianceMapId() { return data->irradianceMapId; }
	GLuint getPrefilterMapId() { return data->preFilteredMapId; }
//	const ofTexture& getBrdfLutTexture() { return data->brdfLutTex; }
	
protected:
	
	ofFbo fbo;
//	ofFbo lutFbo;
	std::shared_ptr<ofCubeMap::Data> data;
	
	void _drawCubeStart(GLuint aCubeMapId);
	void _drawCubeEnd();
	
	void _checkSetup();
	void _createCubeMap();
	void _createIrradianceMap();
	void _createPrefilteredCubeMap();
	static void _createBrdfLUT();
	
	void _configureCubeTextures(GLuint aCubeMapId);
	void _initEmptyTextures(GLuint aCubeMapId, int aSize);
	void _allocateFbo(int asize);
	void _allocateCubeMesh();
	std::vector<glm::mat4> _getViewMatrices(const glm::vec3& apos);
	
	bool _loadRenderShader();
	GLuint getTexStorageFormat();
	
	GLuint texFormat = GL_RGB;
	
	ofTexture mSourceTex;
	bool mBFlipY = true;
	float mExposure = 1.0;
	
	glm::mat4 projectionMat;
	
	
	static ofTexture sBrdfLutTex;
	static ofVboMesh sCubeMesh;
	ofShader shaderEquiRectToCubeMap;
	ofShader shaderRender;
	ofShader shaderIrradianceMap;
	ofShader shaderPreFilterMap;
	static ofShader shaderBrdfLUT;
	
};

std::vector<std::weak_ptr<ofCubeMap::Data> > & ofCubeMapsData();
