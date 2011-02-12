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
	 OF_DIFFUSE_MAP
	,OF_SPECULAR_MAP
	,OF_AMBIENT_MAP
	,OF_HEIGHT_MAP
	,OF_NORMAL_MAP
	,OF_SHININESS_MAP
	,OF_OPACITY_MAP
	,OF_DISPLACEMENT_MAP
	,OF_LIGHT_MAP
	,OF_REFLECTION_MAP
	,OF_UNKOWN_MAP
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
	
	
	// set maps.
	ofMaterial& setDiffuseMap(ofTexture& oDiffuseMap) {	return addTexture(oDiffuseMap, OF_DIFFUSE_MAP);	}
	ofMaterial& setSpecularMap(ofTexture& oSpecularMap) { return addTexture(oSpecularMap, OF_SPECULAR_MAP); }
	ofMaterial& setAmbientMap(ofTexture& oAmbientMap) { return addTexture(oAmbientMap, OF_AMBIENT_MAP); }
	ofMaterial& setHeightMap(ofTexture& oHeightMap) { return addTexture(oHeightMap, OF_HEIGHT_MAP); }
	ofMaterial& setNormalMap(ofTexture& oNormalMap) { return addTexture(oNormalMap, OF_NORMAL_MAP); }
	ofMaterial& setShininessMap(ofTexture& oShininessMap) { return addTexture(oShininessMap, OF_SHININESS_MAP);}
	ofMaterial& setOpactiyMap(ofTexture& oOpacityMap) { return addTexture(oOpacityMap, OF_OPACITY_MAP); }
	ofMaterial& setDisplacementMap(ofTexture& oDispMap) { return addTexture(oDispMap, OF_DISPLACEMENT_MAP); }
	ofMaterial& setLightMap(ofTexture& oLightMap) { return addTexture(oLightMap, OF_LIGHT_MAP); }
	ofMaterial& setReflectionMap(ofTexture& oReflectionMap) { return addTexture(oReflectionMap, OF_REFLECTION_MAP); }
		
	ofMaterial& addTexture(
		 ofTexture& pTexture
		,ofTextureMapType nType = OF_DIFFUSE_MAP
	);

	
	ofColor diffuse;
	ofColor ambient;
	ofColor specular;
	ofColor emissive;
	float shininess;
	ofTextureMaps textures;
};