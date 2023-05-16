#pragma once

class ofShader;
class ofGLProgrammableRenderer;

// enum ofMaterialTextureType: short{
// 	OF_MATERIAL_TEXTURE_NONE=0,
// 	OF_MATERIAL_TEXTURE_DIFFUSE,
// 	OF_MATERIAL_TEXTURE_SPECULAR,
// 	OF_MATERIAL_TEXTURE_AMBIENT,
// 	OF_MATERIAL_TEXTURE_EMISSIVE,
// 	OF_MATERIAL_TEXTURE_NORMAL,
// 	OF_MATERIAL_TEXTURE_OCCLUSION,
// 	OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC,
// 	OF_MATERIAL_TEXTURE_ROUGHNESS_METALLIC,
// 	OF_MATERIAL_TEXTURE_ROUGHNESS,
// 	OF_MATERIAL_TEXTURE_METALLIC,
// 	OF_MATERIAL_TEXTURE_DISPLACEMENT,
// 	OF_MATERIAL_TEXTURE_CLEARCOAT, // INTENSITY
// 	OF_MATERIAL_TEXTURE_CLEARCOAT_ROUGHNESS,
// 	OF_MATERIAL_TEXTURE_CLEARCOAT_INTENSITY_ROUGHNESS,
// 	OF_MATERIAL_TEXTURE_CLEARCOAT_NORMAL
// };

// Material concept: "Anything graphical applied to the polygons"
//
// Diederick Huijbers <diederick[at]apollomedia[dot]nl>
//
// references:
//   * Wavefront material file spec: http://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
//   * Ogre3D: http://www.ogre3d.org/docs/manual/manual_11.html#SEC14
//   * assimp material: http://assimp.sourceforge.net/lib_html/ai_material_8h.html#7dd415ff703a2cc53d1c22ddbbd7dde0

// PBR Material based on GLTF spec from KhronosGroup: https://github.com/KhronosGroup/glTF


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

