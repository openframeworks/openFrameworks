#include "ofMaterial.h"

ofMaterial::ofMaterial() {
}

ofMaterial& ofMaterial::addTexture(
	 ofTexture* pTexture
	,ofTextureMapType nMapType
) 
{
	ofTextureMap tex_map(nMapType, pTexture);
	textures.insert(tex_map);
	return *this;
}

ofMaterial& ofMaterial::setDiffuse(ofColor oDiffuse) {
	diffuse = oDiffuse;
	return *this;
}

ofMaterial& ofMaterial::setAmbient(ofColor oAmbient) {
	ambient = oAmbient;
	return *this;
}

ofMaterial& ofMaterial::setSpecular(ofColor oSpecular) {
	specular = oSpecular;
	return *this;
}

ofMaterial& ofMaterial::setShininess(float nShininess) {
	shininess = nShininess;
	return *this;
}

