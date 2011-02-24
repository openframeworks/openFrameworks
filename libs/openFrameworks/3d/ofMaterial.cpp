#include "ofMaterial.h"

ofMaterial::ofMaterial() {
	enabled_maps = OF_TEXMAP_UNKNOWN;
}

ofMaterial& ofMaterial::addMap(ofTexture& pTexture, ofTextureMapType nMapType) {
	enabled_maps |= nMapType;
	ofTextureMap tex_map(nMapType, &pTexture);
	texture_maps.insert(tex_map);
	return *this;
}

ofTexture* ofMaterial::getMap(ofTextureMapType nType) {
	ofTexture* tex = NULL;
	ofTextureMaps::iterator it = texture_maps.find(nType);
	if(it != texture_maps.end()) {
		tex = it->second;
	}
	return tex;
}

ofMaterial& ofMaterial::setColors(ofColor oDiffuse, ofColor oAmbient, ofColor oSpecular) {
	setDiffuseColor(oDiffuse);
	setAmbientColor(oAmbient);
	setSpecularColor(oSpecular);
	return *this;
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

void ofMaterial::begin() {
	// TODO: needs some work here
	if(enabled_maps & OF_TEXMAP_DIFFUSE) {
		ofTexture* diffuse_tex = getDiffuseMap();
		if(diffuse_tex != NULL) {
			diffuse_tex->bind();
		}
	}
}

void ofMaterial::end() {
	// TODO: needs some work here
	if(enabled_maps & OF_TEXMAP_DIFFUSE) {
		ofTexture* diffuse_tex = getDiffuseMap();
		if(diffuse_tex != NULL) {
			diffuse_tex->unbind();
		}
	}

}

