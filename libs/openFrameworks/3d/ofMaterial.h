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
	void setDiffuseMap(ofTexture& oDiffuseMap) {	return addMap(oDiffuseMap, OF_TEXMAP_DIFFUSE);	}
	void setSpecularMap(ofTexture& oSpecularMap) { return addMap(oSpecularMap, OF_TEXMAP_SPECULAR); }
	void setAmbientMap(ofTexture& oAmbientMap) { return addMap(oAmbientMap, OF_TEXMAP_AMBIENT); }
	void setHeightMap(ofTexture& oHeightMap) { return addMap(oHeightMap, OF_TEXMAP_HEIGHT); }
	void setNormalMap(ofTexture& oNormalMap) { return addMap(oNormalMap, OF_TEXMAP_NORMAL); }
	void setShininessMap(ofTexture& oShininessMap) { return addMap(oShininessMap, OF_TEXMAP_SHININESS);}
	void setOpactiyMap(ofTexture& oOpacityMap) { return addMap(oOpacityMap, OF_TEXMAP_OPACITY); }
	void setDisplacementMap(ofTexture& oDispMap) { return addMap(oDispMap, OF_TEXMAP_DISPLACEMENT); }
	void setLightMap(ofTexture& oLightMap) { return addMap(oLightMap, OF_TEXMAP_LIGHT); }
	void setReflectionMap(ofTexture& oReflectionMap) { return addMap(oReflectionMap, OF_TEXMAP_REFLECTION); }
		
	void addMap(
		 ofTexture& pTexture
		,ofTextureMapType nType = OF_TEXMAP_DIFFUSE
	);
	
	// find maps
	ofTexture& getMap(ofTextureMapType nType);
	ofTexture& getDiffuseMap() { return getMap(OF_TEXMAP_DIFFUSE); };

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
