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

enum ofTextureMapType {
	OF_TEXMAP_DIFFUSE
	,OF_TEXMAP_SPECULAR
	,OF_TEXMAP_AMBIENT
	,OF_TEXMAP_HEIGHT
	,OF_TEXMAP_NORMAL
	,OF_TEXMAP_SHININESS
	,OF_TEXMAP_OPACITY
	,OF_TEXMAP_DISPLACEMENT
	,OF_TEXMAP_LIGHT
	,OF_TEXMAP_REFLECTION
	,OF_TEXMAP_NUM
};


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
	
	
	// set maps.
	void setDiffuseMap(ofTexture& oDiffuseMap);
	void setSpecularMap(ofTexture& oSpecularMap);
	void setAmbientMap(ofTexture& oAmbientMap);
	void setHeightMap(ofTexture& oHeightMap);
	void setNormalMap(ofTexture& oNormalMap);
	void setShininessMap(ofTexture& oShininessMap);
	void setOpactiyMap(ofTexture& oOpacityMap);
	void setDisplacementMap(ofTexture& oDispMap);
	void setLightMap(ofTexture& oLightMap);
	void setReflectionMap(ofTexture& oReflectionMap);
		
	void addMap(ofTexture& pTexture, ofTextureMapType nType = OF_TEXMAP_DIFFUSE);
	
	// find maps
	ofTexture& getMap(ofTextureMapType nType);
	ofTexture& getDiffuseMap() { getMap(OF_TEXMAP_DIFFUSE); };

	// apply the material
	virtual void begin();
	virtual void end();
	
private:
	ofColor diffuse;
	ofColor ambient;
	ofColor specular;
	ofColor emissive;
	float shininess;
	//float opacity;
	//float specularStrength;
	ofTexture texture_maps[OF_TEXMAP_NUM];
};
