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
	OF_UNKOWN_MAP			= 0
	,OF_DIFFUSE_MAP			= (1 << 0)
	,OF_SPECULAR_MAP		= (1 << 1)
	,OF_AMBIENT_MAP			= (1 << 2)
	,OF_HEIGHT_MAP			= (1 << 3)
	,OF_NORMAL_MAP			= (1 << 4)
	,OF_SHININESS_MAP		= (1 << 5)
	,OF_OPACITY_MAP			= (1 << 6)
	,OF_DISPLACEMENT_MAP	= (1 << 7)
	,OF_LIGHT_MAP			= (1 << 8)
	,OF_REFLECTION_MAP		= (1 << 9)
};

typedef std::pair<ofTextureMapType, ofTexture*> ofTextureMap;
typedef std::map<ofTextureMapType, ofTexture*> ofTextureMaps;

class ofMaterial {
public:
	ofMaterial();
	
	// set colors
	ofMaterial& setColors(ofColor oDiffuse, ofColor oAmbient, ofColor oSpecular);
	ofMaterial& setDiffuseColor(ofColor oDiffuse);
	ofMaterial& setAmbientColor(ofColor oAmbient);
	ofMaterial& setSpecularColor(ofColor oSpecular);
	ofMaterial& setShininess(float nShininess);
	ofColor getDiffuseColor() { return diffuse; }
	ofColor getAmbientColor() { return ambient; }
	ofColor getSpecularColor() { return specular; }
	
	
	// set maps.
	ofMaterial& setDiffuseMap(ofTexture& oDiffuseMap) {	return addMap(oDiffuseMap, OF_DIFFUSE_MAP);	}
	ofMaterial& setSpecularMap(ofTexture& oSpecularMap) { return addMap(oSpecularMap, OF_SPECULAR_MAP); }
	ofMaterial& setAmbientMap(ofTexture& oAmbientMap) { return addMap(oAmbientMap, OF_AMBIENT_MAP); }
	ofMaterial& setHeightMap(ofTexture& oHeightMap) { return addMap(oHeightMap, OF_HEIGHT_MAP); }
	ofMaterial& setNormalMap(ofTexture& oNormalMap) { return addMap(oNormalMap, OF_NORMAL_MAP); }
	ofMaterial& setShininessMap(ofTexture& oShininessMap) { return addMap(oShininessMap, OF_SHININESS_MAP);}
	ofMaterial& setOpactiyMap(ofTexture& oOpacityMap) { return addMap(oOpacityMap, OF_OPACITY_MAP); }
	ofMaterial& setDisplacementMap(ofTexture& oDispMap) { return addMap(oDispMap, OF_DISPLACEMENT_MAP); }
	ofMaterial& setLightMap(ofTexture& oLightMap) { return addMap(oLightMap, OF_LIGHT_MAP); }
	ofMaterial& setReflectionMap(ofTexture& oReflectionMap) { return addMap(oReflectionMap, OF_REFLECTION_MAP); }
		
	ofMaterial& addMap(
		 ofTexture& pTexture
		,ofTextureMapType nType = OF_DIFFUSE_MAP
	);
	
	// find maps
	ofTexture* getMap(ofTextureMapType nType);
	ofTexture* getDiffuseMap() { return getMap(OF_DIFFUSE_MAP); };

	// apply the material
	virtual void begin();
	virtual void end();
	
	ofColor diffuse;
	ofColor ambient;
	ofColor specular;
	ofColor emissive;
	float shininess;
	ofTextureMaps texture_maps;
	uint enabled_maps;
};