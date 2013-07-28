#pragma once
#include "ofColor.h"

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


class ofMaterial {
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
	virtual void begin();
	virtual void end();
	
private:
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
};
