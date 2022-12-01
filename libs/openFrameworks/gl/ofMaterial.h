#pragma once
#include "ofColor.h"
#include "ofShader.h"
#include "ofConstants.h"
#include "glm/fwd.hpp"

enum ofMaterialTextureType: short{
	OF_MATERIAL_TEXTURE_NONE=0,
	OF_MATERIAL_TEXTURE_DIFFUSE,
	OF_MATERIAL_TEXTURE_SPECULAR,
	OF_MATERIAL_TEXTURE_AMBIENT,
	OF_MATERIAL_TEXTURE_EMISSIVE,
	OF_MATERIAL_TEXTURE_NORMAL,
	OF_MATERIAL_TEXTURE_OCCLUSION,
	OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC,
	OF_MATERIAL_TEXTURE_ROUGHNESS_METALLIC,
	OF_MATERIAL_TEXTURE_ROUGHNESS,
	OF_MATERIAL_TEXTURE_METALLIC,
	OF_MATERIAL_TEXTURE_DISPLACEMENT,
	OF_MATERIAL_TEXTURE_CLEARCOAT, // INTENSITY
	OF_MATERIAL_TEXTURE_CLEARCOAT_ROUGHNESS,
	OF_MATERIAL_TEXTURE_CLEARCOAT_INTENSITY_ROUGHNESS,
	OF_MATERIAL_TEXTURE_CLEARCOAT_NORMAL
};

// Material concept: "Anything graphical applied to the polygons"
//
// Diederick Huijbers <diederick[at]apollomedia[dot]nl>
//
// references:
//   * Wavefront material file spec: http://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
//   * Ogre3D: http://www.ogre3d.org/docs/manual/manual_11.html#SEC14
//   * assimp material: http://assimp.sourceforge.net/lib_html/ai_material_8h.html#7dd415ff703a2cc53d1c22ddbbd7dde0

// PBR Material based on GLTF spec from KhronosGroup: https://github.com/KhronosGroup/glTF

class ofGLProgrammableRenderer;

/// \class ofMaterialSettings
/// wrapper for material color properties and other settings
///
/// customUniforms: adds some uniforms to the shader so they can be accessed
/// from the postFragment function
///
/// postFragment: Adds a function to the material shader that will get
/// executed after all lighting and material calculations
///
/// The source passed has to include a function with the
/// signature:
///
/// vec4 postFragment(vec4 localColor){
///     return localColor;
/// }
///
/// which will receive the final color after calculating all
/// the lights and material and can modify it and return a new color
///
/// The function has access to the following variables:
///
///     vec2 v_texcoord;          // texture coordinate
///     vec3 v_normal;            // normal at this fragment
///     vec3 v_transformedNormal; // normal multiplied by the normal matrix
///     vec3 v_eyePosition;       // position of this fragment in eye coordinates
///     vec3 v_worldPosition;     // position of this fragment in world coordinates
///     vec4 v_color;             // color interpolated from the vertex colors
///     SAMPLER tex0;             // the bound texture if there's any
///
///     vec4 mat_ambient;         // material ambient color
///     vec4 mat_diffuse;         // material diffuse color
///     vec4 mat_specular;        // material specular
///     vec4 mat_emissive;        // material emissive
///     float mat_shininess;      // material shininess
///
///     vec4 global_ambient;      // global ambient light
///     mat4 modelViewMatrix;     // model view matrix
///     mat4 projectionMatrix;    // projection matrix
///     mat4 textureMatrix;       // texture matrix
///     mat4 modelViewProjectionMatrix; // model view projection matrix
///
///     MAX_LIGHTS                // the total number of lights in the scen
///
/// And an array of lights
/// each light has the following properties:
///
///     float lights[i].enabled;
///     vec4 lights[i].ambient;
///     float lights[i].type;     // 0 = pointlight
///                               // 1 = directionlight
///                               // 2 = spotlight
///                               // 3 = area
///     vec4 lights[i].position;  // where are we
///     vec4 lights[i].diffuse;   // how diffuse
///     vec4 lights[i].specular;  // what kinda specular stuff we got going on?
///
///     // attenuation, how the light attenuates with the distance
///     float lights[i].constantAttenuation;
///     float lights[i].linearAttenuation;
///     float lights[i].quadraticAttenuation;
///
///     // only for spot
///     float lights[i].spotCutoff;
///     float lights[i].spotCosCutoff;
///     float lights[i].spotExponent;
///
///     // spot and area
///     vec3 lights[i].spotDirection;
///
///     // only for directional
///     vec3 lights[i].halfVector;
///
///     // only for area
///     float lights[i].width;
///     float lights[i].height;
///     vec3 lights[i].right;
///     vec3 lights[i].up;
///
struct ofMaterialSettings {
    ofFloatColor diffuse{ 0.8f, 0.8f, 0.8f, 1.0f }; ///< diffuse reflectance
    ofFloatColor ambient{ 0.2f, 0.2f, 0.2f, 1.0f }; ///< ambient reflectance
    ofFloatColor specular{ 0.0f, 0.0f, 0.0f, 1.0f }; ///< specular reflectance
    ofFloatColor emissive{ 0.0f, 0.0f, 0.0f, 1.0f }; ///< emitted light intensity
    float shininess{ 0.2f }; ///< specular exponent
	
	bool isPbr = false;
	// PBR //
	// the diffuse color is used for the albedo color, metallic color
	float metallic{0.0}; ///< metallic properties, can be fraction between 0 -> 1
	float roughness{0.5}; ///< roughness
	float reflectance{0.5}; ///< reflectance only applies to dielectric materials (non-metallic)
	bool clearCoatEnabled = false; ///<this must be enabled for clear coat to have effect, will be enabled if clear coat texture set
	float clearCoatStrength = 1.0;
	float clearCoatRoughness = 0.1;
	float displacementStrength=0.2; ///< strength of vertex displacement from displacement map in world space
	float displacementNormalsStrength=50.0; ///< strength of normal calculation based on displacement map
	float normalGeomToNormalMapMix = 0.95; ///< if a normal map is set, the mix between the geometry normal and normal from the normal / displacement map
	
	glm::vec2 texCoordScale ={1.0,1.0};
	
	// these are not to be set manually
    std::string postFragment="";
    std::string customUniforms=""; ///set by ofMaterial::setCustomUniform*  not to be set manually
	std::string uniqueIdString=""; /// set by ofMaterial to cache shader
	std::string mainFragment=""; /// override the default main function in the frag shader
	std::string mainFragmentKey=""; /// access fragment main function with this key make unique for new instances
	
	std::string mainVertex=""; /// override the default main function in the vertex shader
	std::string mainVertexKey=""; /// access vertex main function with this key make unique for new instances
	
};

/// \class ofBaseMaterial
/// \brief material parameter properties that can be applied to vertices in the OpenGL lighting model
/// used in determining both the intensity and color of reflected light based on the lighting model in use
/// and if the vertices are on a front or back sided face
class ofBaseMaterial{
public:

	virtual ~ofBaseMaterial(){};

	/// \return the diffuse reflectance
	virtual ofFloatColor getDiffuseColor() const=0;

	/// \return the ambient reflectance
	virtual ofFloatColor getAmbientColor() const=0;

	/// \return the specular reflectance
	virtual ofFloatColor getSpecularColor() const=0;

	/// \return the emitted light intensity
	virtual ofFloatColor getEmissiveColor() const=0;

	/// \return the specular exponent
	virtual float getShininess() const=0;

	/// \brief begin using this material's properties
	virtual void begin() const=0;

	/// \brief end using this material's properties
	virtual void end() const=0;

	/// \brief create and return a shader used to implement the materials effect for a given renderer
	/// \param textureTarget an implementation-specific value to specify the type of shader to use
	/// \param renderer programmable renderer instance to create the material shader for
	virtual const ofShader & getShader(int textureTarget, bool geometryHasColor, ofGLProgrammableRenderer & renderer) const=0;
	
	/// \brief set a custom shader controlled by the user. 
	/// \param aCustomShader the material shader, created and maintained by the user
	virtual void setCustomShader( std::shared_ptr<ofShader> aCustomShader) = 0;

	/// \brief upload the given renderer's normal matrix to the material shader
	/// \param shader the material shader, created by getShader()
	/// \param renderer programmable renderer instance that uses the material shader
	virtual void uploadMatrices(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;

	/// \brief update the material properties to the material shader
	/// \param shader the material shader, created by getShader()
	/// \param renderer programmable renderer instance that uses the material shader
	virtual void updateMaterial(const ofShader & shader,ofGLProgrammableRenderer & renderer) const=0;

	/// \brief update the given renderer's lights to the material shader
	/// \param shader the material shader, created by getShader()
	/// \param renderer programmable renderer instance that uses the material shader
	virtual void updateLights(const ofShader & shader,ofGLProgrammableRenderer & renderer) const=0;

	/// \brief update the given renderer's shadows to the material shader
	/// \param shader the material shader, created by getShader()
	/// \param renderer programmable renderer instance that uses the material shader
	virtual void updateShadows(const ofShader & shader,ofGLProgrammableRenderer & renderer) const=0;
	
	/// \brief update the given renderer's environment maps / cube maps to the material shader
	/// \param shader the material shader, created by getShader()
	/// \param renderer programmable renderer instance that uses the material shader
	virtual void updateEnvironmentMaps(const ofShader & shader,ofGLProgrammableRenderer & renderer) const=0;
	
	/// \brief when begin() is called, the material is set to bound by the renderer.\nSet to unbound via renderer when end() is called.
	/// \return is the shader bound to a renderer.
	virtual bool isBound() const;
	
protected:
	friend class ofGLProgrammableRenderer;
	/// \brief set to bound. Later used if certain properties are set so the shader can be updated without calling end(), set var and then begin() again.
	virtual void bind(ofGLProgrammableRenderer & renderer) const {
		mBound = true;
	}
	
	/// \brief unbind the shader. 
	virtual void unbind(ofGLProgrammableRenderer & renderer) const {
		mBound = false;
	}
	
	mutable bool mBound = false;
};


/// \class ofMaterial
/// \brief material parameter properties that can be applied to vertices in the OpenGL lighting model
/// used in determining both the intensity and color of reflected light based on the lighting model in use
/// and if the vertices are on a front or back sided face
class ofMaterial: public ofBaseMaterial {
public:
	ofMaterial();
	virtual ~ofMaterial(){};
	
	/// \brief get the shader uniform name for the material texture type.
	/// \param aMaterialTextureType the material texture type to query
	/// \return the shader uniform name
	static std::string getUniformName( const ofMaterialTextureType& aMaterialTextureType );
	
	/// \brief is this material pbr. Setting PBR functions or textures will automatically set the material to pbr.\nCan also be set calling setPBR(bool);
	/// \return is the material pbr.
	const bool isPBR() const { return data.isPbr; }
	/// \brief enable or disable PBR for this material. Default is disabled.
	void setPBR(bool ab) { data.isPbr = ab; }

	/// \brief setup using settings struct
	/// \param settings color & other properties struct
	void setup(const ofMaterialSettings & settings);
	
	/// \brief override the default main shader functions for vert or frag shader. (PBR)
	/// \param aShaderSrc the shader source as a string
	/// \param atype GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	/// \param skey unique key to identify the vertex and fragment sources. If loading dynamically, use same key to overwrite previous instances.
	void setShaderMain(std::string aShaderSrc, GLenum atype, std::string skey);
	
	/// \brief set all material colors: reflectance type & light intensity. (Phong)
	/// \param oDiffuse the diffuse reflectance
	/// \param oAmbient the ambient reflectance
	/// \param oSpecular the specular reflectance
	/// \param oEmmissive the emitted light intensity
	void setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor emissive);
	
	/// \brief set the diffuse reflectance. (Phong, PBR)
	/// \param oDiffuse the diffuse reflectance
	void setDiffuseColor(ofFloatColor oDiffuse);
	
	/// \brief set the ambient reflectance. (Phong)
	/// \param oAmbient the ambient reflectance
	void setAmbientColor(ofFloatColor oAmbient);
	
	/// \brief set the specular reflectance. (Phong)
	/// \param oSpecular the specular reflectance
	void setSpecularColor(ofFloatColor oSpecular);
	
	/// \brief set the emitted light intensity. (Phong, PBR)
	/// \param oEmmissive the emitted light intensity
	void setEmissiveColor(ofFloatColor oEmmisive);
	
	/// \brief set the specular exponent. (Phong)
	void setShininess(float nShininess);
	
	/// \brief set the tex coord scale used in the shader. (Phong, PBR)
	/// \param xscale texture scale in x
	/// \param yscale texture scale in y
	void setTexCoordScale( float xscale, float yscale );
	
	/// \brief load a texture that is stored in the material. Check which textures are PBR using isPBRTexture(ofMaterialTextureType)
	/// \param aMaterialTextureType type of texture.
	/// \param apath file path to texture.
	/// \return if the load was successful.
	bool loadTexture( const ofMaterialTextureType& aMaterialTextureType, std::string apath );
	bool loadTexture( const ofMaterialTextureType& aMaterialTextureType, std::string apath, bool bTex2d, bool mirrorY );
	/// \brief check if texture is PBR only.
	/// \return is texture only PBR.
	bool isPBRTexture(const ofMaterialTextureType& aMaterialTextureType);
	/// \brief set additonal textures to use in the shader. If texture is PBR only, will set material to PBR.
	void setTexture(const ofMaterialTextureType& aMaterialTextureType,const ofTexture & aTex);
	/// \brief set a diffuse texture. (Phong, PBR).
	/// \param aTex texture with diffuse color information.
	void setDiffuseTexture(const ofTexture & aTex);
	/// \brief set a specular map texture. (Phong)
	/// \param aTex texture with specular map data.
	void setSpecularTexture(const ofTexture & aTex);
	/// \brief set an ambient light texture. (Phong)
	/// \param aTex texture with ambient lighting data.
	void setAmbientTexture(const ofTexture & aTex);
	/// \brief set an emissive texture. (Phong, PBR)
	/// \param aTex texture with emissive color information.
	void setEmissiveTexture(const ofTexture & aTex);
	/// \brief set a normal texture. (Phong, PBR)
	/// \param aTex texture with normal information.
	void setNormalTexture(const ofTexture & aTex);
	/// \brief set an occlusion texture. (Phong, PBR)
	/// \param aTex texture with r = occlusion, g = n/a and b = n/a.
	void setOcclusionTexture(const ofTexture & aTex);
	
	// PBR only textures //
	/// \brief set an occlusion, roughness, metallic texture. (PBR)
	/// \param aTex texture with r = occlusion, g = roughness and b = metallic.
	void setAoRoughnessMetallicTexture(const ofTexture & aTex);
	/// \brief set a roughness, metallic texture. (PBR)
	/// \param aTex texture with r = n/a, g = roughness and b = metallic.
	void setRoughnessMetallicTexture(const ofTexture & aTex);
	/// \brief set a roughness texture. (PBR)
	/// \param aTex texture with r = n/a, g = roughness and b = n/a.
	void setRoughnessTexture(const ofTexture & aTex);
	/// \brief set a metallic texture. (PBR)
	/// \param aTex texture with r = n/a, g = n/a and b = metallic.
	void setMetallicTexture(const ofTexture& aTex);
	/// \brief set a vertex displacement texture. (PBR)
	/// \param aTex rgb texture displacement map.
	void setDisplacementTexture(const ofTexture & aTex);
	/// \brief set a clear coat texture. (PBR)
	/// \param aTex with r = intensity, g = n/a and b = n/a.
	void setClearCoatTexture( const ofTexture& aTex );
	
	//-- PBR properties --//
	/// \brief set metalness. (PBR)
	/// \param ametallic metal property of material.
	void setMetallic( const float& ametallic );
	/// \brief set roughness. (PBR)
	/// \param aroughness roughness of material.
	void setRoughness( const float& aroughness );
	/// \brief set reflectance - only applies to dielectrics, ie, non metals. (PBR)
	/// \param areflectance amount of light material reflects.
	void setReflectance( const float& areflectance );
	/// \brief set clear coat. Disabled by default. (PBR)
	/// \param ab is clear coat enabled.
	void setClearCoatEnabled( bool ab );
	/// \brief set clear coat strength. Must call setClearCoatEnabled(true) to take effect. (PBR)
	/// \param astrength strength of clear coat.
	void setClearCoatStrength( const float& astrength );
	/// \brief set clear coat roughness. Must call setClearCoatEnabled(true) to take effect. (PBR)
	/// \param aroughness roughness of clear coat.
	void setClearCoatRoughness( const float& aroughness );
	/// \brief amount of vertex displacement. Must have set OF_MATERIAL_TEXTURE_DISPLACEMENT to take effect. (PBR)
	/// \param astrength amount to displace vertices along normal accoring to displacement texture.
	void setDisplacementStrength( const float& astrength );
	/// \brief offset strength to calculate normals for vertex displacement. (PBR)
	/// \param astrength strength to calculate normals, higher values = more distinct normals.
	void setDisplacementNormalsStrength( const float& astrength );
	/// \brief percent to mix between geometry normal and normal map normal / displacement normal. (PBR)
	/// \param astrength percent to mix between the two.
	void setNormalGeomToNormalMapMix( const float& astrength );
	//--! PBR properties !--//

	// documented in ofBaseMaterial
	ofFloatColor getDiffuseColor() const;
	ofFloatColor getAmbientColor() const;
	ofFloatColor getSpecularColor() const;
	ofFloatColor getEmissiveColor() const;
	float getShininess() const;
	
	float getMetallic() const;
	float getRoughness() const;
	float getReflectance() const;
	bool isClearCoatEnabled() const;
	float getClearCoatStrength() const;
	float getClearCoatRoughness() const;
	float getDisplacementStrength() const;
	float getDisplacementNormalsStrength() const;
	float getNormalGeomToNormalMapMix() const;
	
	/// \return material color properties data struct
	typedef ofMaterialSettings Data;
	OF_DEPRECATED_MSG("Use getSettings() instead", Data getData() const);
	ofMaterialSettings getSettings() const;
	
	/// \brief set the material color properties data struct
	OF_DEPRECATED_MSG("Use setup(settings) instead", void setData(const ofMaterial::Data& data));
	
	// documented in ofBaseMaterial
	void begin() const;
	void end() const;
	
	virtual void uploadMatrices(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;

	/// \brief set custom uniforms to be used by the shader. as of 0.12.0 onwards these are added to the fragment shader header
	void setCustomUniform1f(const std::string & name, float value);
	void setCustomUniform2f(const std::string & name, glm::vec2 value);
	void setCustomUniform3f(const std::string & name, glm::vec3 value);
	void setCustomUniform4f(const std::string & name, glm::vec4 value);
	void setCustomUniformMatrix4f(const std::string & name, glm::mat4 value);
	void setCustomUniformMatrix3f(const std::string & name, glm::mat3 value);

	void setCustomUniform1i(const std::string & name, int value);
	void setCustomUniform2i(const std::string & name, glm::vec<2, int, glm::precision::defaultp> value);
	void setCustomUniform3i(const std::string & name, glm::vec<3, int, glm::precision::defaultp> value);
	void setCustomUniform4i(const std::string & name, glm::vec<4, int, glm::precision::defaultp> value);
	void setCustomUniformTexture(const std::string & name, const ofTexture & value );
	void setCustomUniformTexture(const std::string & name, int textureTarget, GLint textureID);
	void setCustomUniformTexture(const std::string & name, const ofTexture & value, int textureLocation);
	void setCustomUniformTexture(const std::string & name, int textureTarget, GLint textureID, int textureLocation);
	bool removeCustomUniformTexture(const ofMaterialTextureType& aMaterialTextureType);
	bool removeCustomUniformTexture(const std::string & name);
	int getHighestUniformTextureLocation() const;
	bool hasTexture(const ofMaterialTextureType& aMaterialTextureType) const;
	
	void addShaderDefine( const std::string & aDefineName );
	void addShaderDefine( const std::string & aDefineName, const std::string & aDefineValue );
	bool removeShaderDefine( const std::string & aDefineName );
	/// \brief string holding all of the defines added to the shader. (Phong, PBR)
	/// \return string containing all defines.
	const std::string getDefinesString() const;
	
	void setCustomShader(std::shared_ptr<ofShader> aCustomShader);
	
	
protected:
	/// \brief unbind the material, override the default so we can set current shader to nullptr
	void unbind(ofGLProgrammableRenderer & renderer) const;

private:
	
	void mergeCustomUniformTextures();
	void mergeCustomUniformTextures(ofMaterialTextureType mainType, std::vector<ofMaterialTextureType> mergeTypes);
	
	const std::string getShaderStringId() const;

	void initShaders(ofGLProgrammableRenderer & renderer) const;
	const ofShader & getShader(int textureTarget, bool geometryHasColor, ofGLProgrammableRenderer & renderer) const;
	void updateMaterial(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;
	void updateLights(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;
	void updateShadows(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;
	void updateEnvironmentMaps(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;
	
	ofMaterialSettings data;

	struct Shaders{
		ofShader noTexture;
		ofShader color;
		ofShader texture2DColor;
		ofShader textureRectColor;
		ofShader texture2D;
		ofShader textureRect;
		size_t numLights;
		size_t numCubeMaps;
	};
	struct TextureUnifom{
		int textureTarget;
		GLint textureID;
		int textureLocation;
		std::string shaderDefine;
	};
	const TextureUnifom getCustomUniformTexture(const ofMaterialTextureType& aMaterialTextureType) const;
	TextureUnifom getCustomUniformTexture(const ofMaterialTextureType& aMaterialTextureType);
	TextureUnifom getCustomUniformTexture(const std::string & name);

	mutable std::map<ofGLProgrammableRenderer*,std::shared_ptr<Shaders>> shaders;
	static std::map<ofGLProgrammableRenderer*, std::map<std::string,std::weak_ptr<Shaders>>> shadersMap;
	static std::string vertexShader;
	static std::string fragmentShader;
	std::map<std::string, float> uniforms1f;
	std::map<std::string, glm::vec2> uniforms2f;
	std::map<std::string, glm::vec3> uniforms3f;
	std::map<std::string, glm::vec4> uniforms4f;
	std::map<std::string, float> uniforms1i;
	std::map<std::string, glm::vec<2, int, glm::precision::defaultp>> uniforms2i;
	std::map<std::string, glm::vec<3, int, glm::precision::defaultp>> uniforms3i;
	std::map<std::string, glm::vec<4, int, glm::precision::defaultp>> uniforms4i;
	std::map<std::string, glm::mat4> uniforms4m;
	std::map<std::string, glm::mat3> uniforms3m;
	std::map<std::string, TextureUnifom> uniformstex;
	
	std::map<std::string, std::string> mCustomUniforms;
	std::map<std::string, std::string> mDefines;

	mutable std::map<std::string, int> mShaderIdsToRemove;
	
	std::map<ofMaterialTextureType, std::shared_ptr<ofTexture> > mLocalTextures;
	
	std::shared_ptr<ofShader> customShader;
	bool bHasCustomShader = false;
	bool mBDefinesDirty = true;
	mutable const ofShader* currentRenderShader = nullptr;
	
};
