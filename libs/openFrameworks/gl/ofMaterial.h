#pragma once
#include "ofColor.h"
#include "ofShader.h"
#include "ofBaseTypes.h"

// Material concept: "Anything graphical applied to the polygons"
//
// Diederick Huijbers <diederick[at]apollomedia[dot]nl>
//
// references:
//   * Wavefront material file spec: http://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
//   * Ogre3D: http://www.ogre3d.org/docs/manual/manual_11.html#SEC14
//   * assim material: http://assimp.sourceforge.net/lib_html/ai_material_8h.html#7dd415ff703a2cc53d1c22ddbbd7dde0

/// \class ofMaterial
/// \brief material parameter properties that can be applied to vertices in the OpenGL lighting model
/// used in determining both the intensity and color of reflected light based on the lighting model in use
/// and if the vertices are on a front or back sided face
class ofMaterial: public ofBaseMaterial {
public:
	ofMaterial();
	virtual ~ofMaterial(){};


	/// \class Data
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
	struct Settings {
		ofFloatColor diffuse{ 0.8f, 0.8f, 0.8f, 1.0f }; ///< diffuse reflectance
		ofFloatColor ambient{ 0.2f, 0.2f, 0.2f, 1.0f }; //< ambient reflectance
		ofFloatColor specular{ 0.0f, 0.0f, 0.0f, 1.0f }; //< specular reflectance
		ofFloatColor emissive{ 0.0f, 0.0f, 0.0f, 1.0f }; //< emitted light intensity
		float shininess{ 0.2f }; //< specular exponent
		std::string postFragment;
		std::string customUniforms;
	};

	void setup(const ofMaterial::Settings & settings);
	
	/// \brief set all material colors: reflectance type & light intensity
	/// \param oDiffuse the diffuse reflectance
	/// \param oAmbient the ambient reflectance
	/// \param oSpecular the specular reflectance
	/// \param oEmmissive the emitted light intensity
	void setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor emissive);
	
	/// \brief set the diffuse reflectance
	/// \param oDiffuse the diffuse reflectance
	void setDiffuseColor(ofFloatColor oDiffuse);
	
	/// \brief set the ambient reflectance
	/// \param oAmbient the ambient reflectance
	void setAmbientColor(ofFloatColor oAmbient);
	
	/// \brief set the specular reflectance
	/// \param oSpecular the specular reflectance
	void setSpecularColor(ofFloatColor oSpecular);
	
	/// \brief set the emitted light intensity
	/// \param oEmmissive the emitted light intensity
	void setEmissiveColor(ofFloatColor oEmmisive);
	
	/// \brief set the specular exponent
	void setShininess(float nShininess);

	// documented in ofBaseMaterial
	ofFloatColor getDiffuseColor() const;
	ofFloatColor getAmbientColor() const;
	ofFloatColor getSpecularColor() const;
	ofFloatColor getEmissiveColor() const;
	float getShininess() const;
	
	/// \return material color properties data struct
	typedef Settings Data;
	OF_DEPRECATED_MSG("Use getSettings instead", Data getData() const);
	Settings getSettings() const;
	
	/// \brief set the material color properties data struct
	OF_DEPRECATED_MSG("Use setup(settings) instead", void setData(const ofMaterial::Data& data));
	
	// documented in ofBaseMaterial
	void begin() const;
	void end() const;


	void setCustomUniform1f(const std::string & name, float value);
	void setCustomUniform2f(const std::string & name, glm::vec2 value);
	void setCustomUniform3f(const std::string & name, glm::vec3 value);
	void setCustomUniform4f(const std::string & name, glm::vec4 value);

	void setCustomUniform1i(const std::string & name, int value);
	void setCustomUniform2i(const std::string & name, glm::tvec2<int> value);
	void setCustomUniform3i(const std::string & name, glm::tvec3<int> value);
	void setCustomUniform4i(const std::string & name, glm::tvec4<int> value);
	void setCustomUniformTexture(const std::string & name, const ofTexture & value, int textureLocation);
	void setCustomUniformTexture(const string & name, int textureTarget, GLint textureID, int textureLocation);



private:
	void initShaders(ofGLProgrammableRenderer & renderer) const;
	const ofShader & getShader(int textureTarget, bool geometryHasColor, ofGLProgrammableRenderer & renderer) const;
	void updateMaterial(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;
	void updateLights(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;

	Settings data;

	struct Shaders{
		ofShader noTexture;
		ofShader color;
		ofShader texture2DColor;
		ofShader textureRectColor;
		ofShader texture2D;
		ofShader textureRect;
		size_t numLights;
	};
	struct TextureUnifom{
		int textureTarget;
		GLint textureID;
		int textureLocation;
	};

	mutable std::map<ofGLProgrammableRenderer*,std::shared_ptr<Shaders>> shaders;
	static std::map<ofGLProgrammableRenderer*, std::map<std::string,std::weak_ptr<Shaders>>> shadersMap;
	static string vertexShader;
	static string fragmentShader;
	std::map<std::string, float> uniforms1f;
	std::map<std::string, glm::vec2> uniforms2f;
	std::map<std::string, glm::vec3> uniforms3f;
	std::map<std::string, glm::vec4> uniforms4f;
	std::map<std::string, float> uniforms1i;
	std::map<std::string, glm::tvec2<int>> uniforms2i;
	std::map<std::string, glm::tvec3<int>> uniforms3i;
	std::map<std::string, glm::tvec4<int>> uniforms4i;
	std::map<std::string, TextureUnifom> uniformstex;
};
