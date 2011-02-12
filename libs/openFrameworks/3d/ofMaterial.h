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
	 OF_DUMMY_MAP
	,OF_DIFFUSE_MAP
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
	ofMaterial& setDiffuse(ofColor oDiffuse);
	ofMaterial& setAmbient(ofColor oAmbient);
	ofMaterial& setSpecular(ofColor oSpecular);
	ofMaterial& setShininess(float nShininess);
	
	ofMaterial& addTexture(
		 ofTexture* pTexture
		,ofTextureMapType nType = OF_DUMMY_MAP
	);

	ofColor diffuse;
	ofColor ambient;
	ofColor specular;
	ofColor emissive;
	float shininess;
	ofTextureMaps textures;
};