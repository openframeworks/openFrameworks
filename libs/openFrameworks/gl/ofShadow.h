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
class ofMaterial;

/// \class ofShadow
/// Used for determining the amount of light blocked, emanating from ofLights.
/// Intended use as class inside ofLight.
/// see ofLight::shadow
class ofShadow {
public:
	static GLenum getTextureTarget( int aLightType );
	static int getNumTotalPossibleShadows( int aLightType );
	
	/// \brief Set the depth map resolution, default is 1024 and 512 for OPENGL_ES.
	/// \param aLightType, ofLightType, as int, ie. OF_LIGHT_DIRECTIONAL.
	/// \param ares, int, depth resolution.
	static void setDepthMapResolution( int aLightType, int ares );
	static void setDepthMapResolution( int aLightType, int awidth, int aheight );
	
	/// \brief Get the depth map width for a light type.
	/// \param aLightType, ofLightType, as int, ie. OF_LIGHT_DIRECTIONAL.
	static int getDepthMapWidth(int aLightType);
	/// \brief Get the depth map height for a light type.
	/// \param aLightType, ofLightType, as int, ie. OF_LIGHT_DIRECTIONAL.
	static int getDepthMapHeight(int aLightType);
	
	/// \brief Get the GL texture ids for the corresponding light type for passing to an ofShader.
	static GLuint getPointTexId();
	static GLuint getDirectionalTexId();
	static GLuint getSpotTexId();
	static GLuint getAreaTexId();
	/// \brief Determine if any shadow is active.
	/// \return bool true if at least one is enabled.
	static bool hasActiveShadows();
	/// \brief Get the ofShadowType as a string representation.
	/// \return std::string.
	static std::string getShadowTypeAsString( ofShadowType atype );
	
	static void enableAllShadows();
	static void disableAllShadows();
	static void setAllShadowTypes( ofShadowType atype );
	static void setAllShadowDepthResolutions( int awidth, int aheight );
	static void setAllShadowBias( float bias );
	static void setAllShadowNormalBias( float normalBias );
	static void setAllShadowSampleRadius( float sampleRadius );
	
	static bool areShadowsSupported();
		
	ofShadow();
	~ofShadow();
	
	/// \brief Clear out any unused texture resources.
	void clear();
	/// \brief Get the enabled state.
	/// \return bool, true if enabled.
	bool getIsEnabled() { return data->isEnabled; }
	/// \brief Set the enabled state.
	/// \param bool, true to enable.
	void setEnabled( bool ab );
	/// \brief Get the near clip.
	/// \return float, near clip value.
	float getNearClip() { return data->nearClip; }
	/// \brief Get the far clip.
	/// \return float, far clip value.
	float getFarClip() { return data->farClip; }
	/// \brief Set the near clip.
	/// \param anear float, near clip value.
	void setNearClip( float anear ) { data->nearClip = anear; }
	/// \brief Set the far clip.
	/// \param afar float, far clip value.
	void setFarClip( float afar ) { data->farClip = afar; }
	/// \brief Is multi pass rendering enabled for point lights.
	/// \return bool, true if multi cube pass rendering is enabled.
	bool isMultiCubeFacePass() const;
	bool isSingleOmniPass() const;
	/// \brief Set omni pass render for point lights. Cube maps are rendered for point lights.
	/// Omni pass is enabled by default.
	/// \warning Disabled for OPENGL_ES.
	/// \param ab bool to enable or disable omni pass rendering.
	void setSingleOmniPass( bool ab );
	/// \brief Get number of shadow depth pases, 1 or 6 point lights without single omni pass.
	/// \return int, number of passes required.
	int getNumShadowDepthPasses() const;
	/// \brief Get the depth map width for this shadow type.
	/// \return int, depth width resolution.
	int getDepthMapWidth();
	/// \brief Get the depth map height for this shadow type.
	/// \return int, depth height resolution.
	int getDepthMapHeight();
	
	/// \brief Set the shadow type, the fidelity of the shadow.
	/// \param aWorldWidth width of the bounds.
	/// \param aWorldHeight height of the bounds.
	void setDirectionalBounds( float aWorldWidth, float aWorldHeight );
	
	/// \brief Set the shadow type, the fidelity of the shadow.
	/// \param atype ofShadowType.
	void setShadowType(ofShadowType atype) {data->shadowType = atype;}
	/// \brief Is culling of triangles enabled. Change the winding order using setFrontFaceWindingOrder()
	/// \return bool true if front facing triangles are culled.
	bool isCullingEnabled() { return mBEnableCulling; }
	/// \brief Is culling of triangles enabled. Change the winding order using setFrontFaceWindingOrder()
	/// \param ab set to true to enable culling of front facing triangles.
	void setCullingEnabled(bool ab) { mBEnableCulling=ab;}
	/// \brief Get winding order of front faces used for culling.
	/// \return GLenum, GL_CW or GL_CCW.
	GLenum getFrontFaceWindingOrder() { return mGlFrontFaceWindingOrder; }
	/// \brief Set winding order of front faces used for culling.
	/// \param GLenum aw GL_CW or GL_CCW.
	void setFrontFaceWindingOrder( GLenum aw ) { mGlFrontFaceWindingOrder = aw; }
	
	/// \brief Get the depth map fbo id.
	/// \return GLuint representing the fbo id.
	GLuint getDepthMapFboId();
	/// \brief Get the depth map GL texture id.
	/// \return GLuint representing the GL texture id.
	GLuint getDepthMapTexId();
	/// \brief Get the strength of the shadow.
	/// /// \return float representing the shadow strength.
	const float& getStrength() { return data->strength; }
	/// \brief Set the strength of the shadow, higher values yield darker results.
	/// \param astrength float value of strength, can be higher than 1.0.
	void setStrength(float astrength) { data->strength = astrength; }
	/// \brief Get the bias of the shadow depth sampling.
	/// \return float representing the bias amount.
	float getBias() const { return data->bias; }
	/// \brief Set the bias of the shadow depth sampling, helpful for reducing shadow acne.
	/// \param abias float value of the bias amount.
	void setBias( float abias ) { data->bias = abias; }
	/// \brief Get the bias the shadow depth sampling along the normal by this amount.
	/// \return float value of the bias along normal amount.
	float getNormalBias() const { return data->normalBias; }
	/// \brief Bias the shadow depth sampling along the normal by this amount.
	/// \param abias float value of the bias along normal amount.
	void setNormalBias( float abias ) { data->normalBias = abias; }
	/// \brief Get the sample radius which the shadow is sampled around a disk.
	/// \return float value sample radius.
	float getSampleRadius() const { return data->sampleRadius; }
	/// \brief Set the sample radius which the shadow is sampled around a disk.
	/// \param aradius float value of the sample radius.
	void setSampleRadius( float aradius ) { data->sampleRadius = aradius; }
	
	/// \brief Draw the bounds of the shadow. Helpful for determing if the area of interest
	/// is inside the shadow frustum. If outside, the shadow will have no effect.
	void drawFrustum();
	/// \brief Get the ofShadowType as a string representation.
	/// \return std::string.
	std::string getShadowTypeAsString();
	
	/// \brief Configure the ofShader with the correct defines and uniforms for this shadow type.
	/// \param ashader ofShader to configure.
	/// \param aShaderMain string, the shader source main function as a string, vert shader only.
	///
	/// \see: "openFrameworks/gl/shaders/shadowDepth.vert"
	/// \note: If using in conjunction with ofMaterials, try ofMaterial::setDepthShaderMain(std::string aShaderSrc, std::string skey).
	/// Which will use the depth shader only on that ofMaterial.
	///
	/// 
	/// \note: #define OF_SHADOW_DEPTH_PASS is added to the file.
	///
	/// \note: Find a sample vert shader below to pass as a string to the setupShadowDepthShader() function as aShaderMain.
	///
	///	void main (void){
	///		#if defined(OF_SHADOW_DEPTH_PASS) // Define added by the setupShadowDepthShader() function. Unnecessary here, shown for reference.
	///		vec3 worldPosition = (modelMatrix * vec4(position.xyz, 1.0)).xyz;
	///		// this method is added via ofShadow.setupShadowDepthShader
	///		sendShadowDepthWorldPosition(worldPosition);
	///		#endif
	///	}
	bool setupShadowDepthShader(ofShader& ashader, const std::string aShaderMain) const;
	/// \brief Configure the ofShader with the correct defines and uniforms a light type, ie. OF_LIGHT_POINT.
	/// \param ashader ofShader to configure.
	/// \param aLightType int the shader source main function as a string, vert shader only.
	/// \param aShaderMain string, the shader source main function as a string, vert shader only.
	/// \note: "openFrameworks/gl/shaders/shadowDepth.vert"
	bool setupShadowDepthShader(ofShader& ashader, int aLightType, const std::string aShaderMain, bool abSinglePass) const;
	
	
protected:
	/// \class ofShadowData
	/// Used for holding information for rendering, accessed by other classes like ofShader.
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
	
	static std::vector<std::weak_ptr<ofShadow::Data> > & getShadowsData();
	
	const std::shared_ptr<ofShadow::Data>& getData() const { return data; }
	
	/// \note: These protected functions are called from ofLight.
	void setLightType( int atype );
	void setAreaLightSize( float awidth, float aheight ) {mAreaLightWidth=awidth;mAreaLightHeight=aheight;};
	
	void update( const ofLight& alight );
	
	bool beginDepth();
	bool endDepth();
	
	bool beginDepth(GLenum aCubeFace);
	bool endDepth(GLenum aCubeFace);
	
	friend class ofLight;
	friend class ofGLProgrammableRenderer;
	friend class ofMaterial;
	friend class ofShader;
	
	static std::string getShaderDefinesAsString();
	
	std::vector<glm::vec3> getFrustumCorners( const glm::vec3& aup, const glm::vec3& aright, const glm::vec3& afwd );
	
	/// \note: The following 3 functions are used by the renderer, ofGLProgrammableRenderer for example.
	const ofShader & getDepthShader(ofGLProgrammableRenderer & renderer) const;
	void updateDepth(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;
	void updateDepth(const ofShader & shader,GLenum aCubeFace,ofGLProgrammableRenderer & renderer) const;
	
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
	struct Shaders{
		ofShader depth;
		ofShader depthCube;
		ofShader depthCubeMultiPass;
	};
	void initShaders(ofGLProgrammableRenderer & renderer) const;
	
	mutable std::map<ofGLProgrammableRenderer*,std::shared_ptr<Shaders>> shaders;
	
};


