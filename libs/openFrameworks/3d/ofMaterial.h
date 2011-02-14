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
	OF_TEXMAP_UNKNOWN			= 0
	,OF_TEXMAP_DIFFUSE			= (1 << 0)
	,OF_TEXMAP_SPECULAR			= (1 << 1)
	,OF_TEXMAP_AMBIENT			= (1 << 2)
	,OF_TEXMAP_HEIGHT			= (1 << 3)
	,OF_TEXMAP_NORMAL			= (1 << 4)
	,OF_TEXMAP_SHININESS		= (1 << 5)
	,OF_TEXMAP_OPACITY			= (1 << 6)
	,OF_TEXMAP_DISPLACEMENT		= (1 << 7)
	,OF_TEXMAP_LIGHT			= (1 << 8)
	,OF_TEXMAP_REFLECTION		= (1 << 9)
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
	ofMaterial& setDiffuseMap(ofTexture& oDiffuseMap) {	return addMap(oDiffuseMap, OF_TEXMAP_DIFFUSE);	}
	ofMaterial& setSpecularMap(ofTexture& oSpecularMap) { return addMap(oSpecularMap, OF_TEXMAP_SPECULAR); }
	ofMaterial& setAmbientMap(ofTexture& oAmbientMap) { return addMap(oAmbientMap, OF_TEXMAP_AMBIENT); }
	ofMaterial& setHeightMap(ofTexture& oHeightMap) { return addMap(oHeightMap, OF_TEXMAP_HEIGHT); }
	ofMaterial& setNormalMap(ofTexture& oNormalMap) { return addMap(oNormalMap, OF_TEXMAP_NORMAL); }
	ofMaterial& setShininessMap(ofTexture& oShininessMap) { return addMap(oShininessMap, OF_TEXMAP_SHININESS);}
	ofMaterial& setOpactiyMap(ofTexture& oOpacityMap) { return addMap(oOpacityMap, OF_TEXMAP_OPACITY); }
	ofMaterial& setDisplacementMap(ofTexture& oDispMap) { return addMap(oDispMap, OF_TEXMAP_DISPLACEMENT); }
	ofMaterial& setLightMap(ofTexture& oLightMap) { return addMap(oLightMap, OF_TEXMAP_LIGHT); }
	ofMaterial& setReflectionMap(ofTexture& oReflectionMap) { return addMap(oReflectionMap, OF_TEXMAP_REFLECTION); }
		
	ofMaterial& addMap(
		 ofTexture& pTexture
		,ofTextureMapType nType = OF_TEXMAP_DIFFUSE
	);
	
	// find maps
	ofTexture* getMap(ofTextureMapType nType);
	ofTexture* getDiffuseMap() { return getMap(OF_TEXMAP_DIFFUSE); };

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