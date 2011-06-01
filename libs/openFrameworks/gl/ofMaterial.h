#pragma once
#include "ofMain.h"

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
	
	// set colors
	void setColors(ofColor oDiffuse, ofColor oAmbient, ofColor oSpecular, ofColor emissive);
	void setDiffuseColor(ofColor oDiffuse);
	void setAmbientColor(ofColor oAmbient);
	void setSpecularColor(ofColor oSpecular);
	void setEmissiveColor(ofColor oEmmisive);
	void setShininess(float nShininess);

	ofColor getDiffuseColor();
	ofColor getAmbientColor();
	ofColor getSpecularColor();
	ofColor getEmissiveColor();
	float getShininess();
	
	// apply the material
	virtual void begin();
	virtual void end();
	
private:
	ofColor diffuse;
	ofColor ambient;
	ofColor specular;
	ofColor emissive;
	float shininess;


	ofColor prev_diffuse;
	ofColor prev_ambient;
	ofColor prev_specular;
	ofColor prev_emissive;
	float prev_shininess;
};
