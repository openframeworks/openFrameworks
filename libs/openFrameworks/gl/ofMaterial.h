#pragma once
#include "ofColor.h"
#include "ofShader.h"
#include "ofBaseTypes.h"

/**
 * Material concept: "Anything graphical applied to the polygons"
 * 
 * @author	Diederick Huijbers <diederick[at]apollomedia[dot]nl> 
 *
 * @link	Wavefront material file spec:
 *			http://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
 * @link	Ogre3D
 *			http://www.ogre3d.org/docs/manual/manual_11.html#SEC14
 * @link	assim material
 *			http://assimp.sourceforge.net/lib_html/ai_material_8h.html#7dd415ff703a2cc53d1c22ddbbd7dde0
 */


class ofMaterial: public ofBaseMaterial {
public:
	ofMaterial();
	virtual ~ofMaterial(){};
	
	// set colors
	void setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor emissive);
	void setDiffuseColor(ofFloatColor oDiffuse);
	void setAmbientColor(ofFloatColor oAmbient);
	void setSpecularColor(ofFloatColor oSpecular);
	void setEmissiveColor(ofFloatColor oEmmisive);
	void setShininess(float nShininess);

	ofFloatColor getDiffuseColor();
	ofFloatColor getAmbientColor();
	ofFloatColor getSpecularColor();
	ofFloatColor getEmissiveColor();
	float getShininess();
	
	// apply the material
	void begin();
	void end();

private:
	void initShaders();
	void beginShader(int texType);
	string vertexSource(int maxLights, bool hasTexture, bool textureRect);
	string fragmentSource(int maxLights, bool hasTexture, bool textureRect);

	ofFloatColor diffuse;
	ofFloatColor ambient;
	ofFloatColor specular;
	ofFloatColor emissive;
	float shininess;


	ofFloatColor prev_diffuse, prev_diffuse_back;
	ofFloatColor prev_ambient, prev_ambient_back;
	ofFloatColor prev_specular, prev_specular_back;
	ofFloatColor prev_emissive, prev_emissive_back;
	float prev_shininess, prev_shininess_back;

	mutable const ofShader * currentShader;

	static ofShader shaderNoTexture;
	static ofShader shaderTexture2D;
	static ofShader shaderTextureRect;
	static bool shadersInitialized;
	static size_t shaderLights;
	static string vertexShader;
	static string fragmentShader;
};
