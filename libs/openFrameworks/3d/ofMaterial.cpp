#include "ofMaterial.h"

ofMaterial::ofMaterial() {
}

ofMaterial& ofMaterial::addTexture(
	 ofTexture& pTexture
	,ofTextureMapType nMapType
) 
{
	ofTextureMap tex_map(nMapType, &pTexture);
	textures.insert(tex_map);
	return *this;
}


ofMaterial& ofMaterial::setColors(ofColor oDiffuse, ofColor oAmbient, ofColor oSpecular) {
	setDiffuseColor(oDiffuse);
	setAmbientColor(oAmbient);
	setSpecularColor(oSpecular);
}

ofMaterial& ofMaterial::setDiffuseColor(ofColor oDiffuse) {
	diffuse = oDiffuse;
	return *this;
}

ofMaterial& ofMaterial::setAmbientColor(ofColor oAmbient) {
	ambient = oAmbient;
	return *this;
}

ofMaterial& ofMaterial::setSpecularColor(ofColor oSpecular) {
	specular = oSpecular;
	return *this;
}

ofMaterial& ofMaterial::setShininess(float nShininess) {
	shininess = nShininess;
	return *this;
}

