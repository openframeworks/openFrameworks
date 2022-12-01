//
//  ofShadow.h
//  openFrameworksLib
//
//  Created by Nick Hardeman on 10/3/22.
//

#pragma once
#include "ofShader.h"
#include "ofNode.h"

enum ofShadowType {
	OF_SHADOW_TYPE_HARD=0,
	OF_SHADOW_TYPE_PCF_LOW,
	OF_SHADOW_TYPE_PCF_MED,
	OF_SHADOW_TYPE_PCF_HIGH,
	OF_SHADOW_TYPE_TOTAL
};

class ofLight;
class ofGLProgrammableRenderer;

class ofShadow {
public:
	
	struct GLData {
		GLuint fboId = 0;
		GLuint texId = 0;
		bool bAllocated = false;
		bool bFboAllocated = false;
		
#if defined(TARGET_OPENGLES)
		int width = 512;
		int height = 512;
#else
		int width = 1024;
		int height = 1024;
#endif
		
		int totalShadows = 0;
	};
	
	class Data{
	public:
		// position, direction, up and right are in world space 
		glm::vec3 position = {0,0,0};
		glm::vec3 direction = {1,0,0};
		
		glm::vec3 up = {0,1,0};
		glm::vec3 right = {1,0,0};
		
		int lightType = 0;
#if defined(TARGET_OPENGLES)
		ofShadowType shadowType = OF_SHADOW_TYPE_HARD;
#else
		ofShadowType shadowType = OF_SHADOW_TYPE_PCF_LOW;
#endif
		
		glm::mat4 shadowMatrix;
		float strength = 0.5;
		
		int texIndex = 0;
		
		bool isEnabled = false;
		float bias = 0.005f;
		float normalBias = 0.0;
		
		int numDepthPasses = 1;
		
		int index = -1;
		float nearClip = 1;
		float farClip = 1500;
		float sampleRadius = 1.f;
	};
	
	static GLenum getTextureTarget( int aLightType );
	static int getNumTotalPossibleShadows( int aLightType );
	
	static void setDepthMapResolution( int aLightType, int ares );
	static void setDepthMapResolution( int aLightType, int awidth, int aheight );
	
	static int getDepthMapWidth(int aLightType);
	static int getDepthMapHeight(int aLightType);
	
	static GLuint getPointTexId();
	static GLuint getDirectionalTexId();
	static GLuint getSpotTexId();
	static GLuint getAreaTexId();
	
	static bool hasActiveShadows();
	static std::string getShadowTypeAsString( ofShadowType atype );
	
	static void enableAllShadows();
	static void disableAllShadows();
	static void setAllShadowTypes( ofShadowType atype );
	static void setAllShadowDepthResolutions( int awidth, int aheight );
	static void setAllShadowBias( float bias );
	static void setAllShadowNormalBias( float normalBias );
	static void setAllShadowSampleRadius( float sampleRadius );
	static std::string getShaderDefinesAsString();
	
	ofShadow();
	~ofShadow();
	
	void setLightType( int atype );
	
	void update( const ofLight& alight );
	
	bool beginDepth();
	bool endDepth();
	
	bool beginDepth(GLenum aCubeFace);
	bool endDepth(GLenum aCubeFace);
		
	void clear();
	
	const std::shared_ptr<ofShadow::Data>& getData() const { return data; }
	bool getIsEnabled() { return data->isEnabled; }
	void setEnabled( bool ab );
	
	float getNearClip() { return data->nearClip; }
	float getFarClip() { return data->farClip; }
	void setNearClip( float anear ) { data->nearClip = anear; }
	void setFarClip( float afar ) { data->farClip = afar; }
	
	const bool isMultiCubeFacePass() const;
	const bool isSingleOmniPass() const;
	const int getNumShadowDepthPasses() const;
	void setSingleOmniPass( bool ab );
	int getDepthMapWidth();
	int getDepthMapHeight();
	
	void setDirectionalBounds( float aWorldWidth, float aWorldHeight );
	
	void setShadowType(ofShadowType atype) {data->shadowType = atype;}
	
	bool isGlCullingEnabled() { return mBEnableCulling; }
	void setGlCullingEnabled(bool ab) { mBEnableCulling=ab;}
	GLenum getFrontFaceWindingOrder() { return mGlFrontFaceWindingOrder; }
	void setFrontFaceWindingOrder( GLenum aw ) { mGlFrontFaceWindingOrder = aw; }
	
	GLuint getDepthMapFboId();
	GLuint getDepthMapTexId();
	
	const float& getStrength() { return data->strength; }
	void setStrength(float astrength) { data->strength = astrength; }
	
	const float getBias() { return data->bias; }
	void setBias( float abias ) { data->bias = abias; }
	
	const float getNormalBias() { return data->normalBias; }
	void setNormalBias( float abias ) { data->normalBias = abias; }
	
	const float getSampleRadius() { return data->sampleRadius; }
	void setSampleRadius( float aradius ) { data->sampleRadius = aradius; }
	
	void setAreaLightSize( float awidth, float aheight ) {mAreaLightWidth=awidth;mAreaLightHeight=aheight;};
	
	void drawFrustum();
	std::vector<glm::vec3> getFrustumCorners( const glm::vec3& aup, const glm::vec3& aright, const glm::vec3& afwd );
	
	std::string getShadowTypeAsString();
		
	const ofShader & getDepthShader(ofGLProgrammableRenderer & renderer) const;
	void updateDepth(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;
	void updateDepth(const ofShader & shader,GLenum aCubeFace,ofGLProgrammableRenderer & renderer) const;
	
protected:
	
	static void _updateTexDataIds();
	
	void _drawFrustum( const glm::vec3& aup, const glm::vec3& aright, const glm::vec3& afwd );
	
	void _allocate();
	
	void _checkSetup();
	void _allocateFbo();
	void _checkFbos();
	void _updateNumShadows();
	
	std::shared_ptr<ofShadow::Data> data;
	
	bool mBSinglePass = true;
	
	glm::mat4 mShadowProjection = glm::mat4(1.0);
	
	std::vector<glm::mat4> mLookAtMats;
	std::vector<glm::mat4> mViewProjMats;
	
	float mOrthoScaleX = 1.0;
	float mOrthoScaleY = 1.0;
	float mDirectionalBoundsWidth = -1;
	float mDirectionalBoundsHeight = -1;
	
	float mAreaLightWidth = 100;
	float mAreaLightHeight = 50.0;
	
	float mFov = 90;
	
	bool mBEnableCulling = true;
	GLenum mGlFrontFaceWindingOrder = GL_CW;
	
	const glm::mat4 biasMatrix = glm::mat4(
										   0.5, 0.0, 0.0, 0.0,
										   0.0, 0.5, 0.0, 0.0,
										   0.0, 0.0, 0.5, 0.0,
										   0.5, 0.5, 0.5, 1.0
										   );
    
private:
//	int getNumTotalPossibleShadows( int aLightType );
	
	struct Shaders{
		ofShader depth;
		ofShader depthCube;
		ofShader depthCubeMultiPass;
	};
	void initShaders(ofGLProgrammableRenderer & renderer) const;
	
	mutable std::map<ofGLProgrammableRenderer*,std::shared_ptr<Shaders>> shaders;
};

std::vector<std::weak_ptr<ofShadow::Data> > & ofShadowsData();
