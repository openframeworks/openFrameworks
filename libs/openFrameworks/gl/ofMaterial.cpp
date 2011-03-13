#include "ofMaterial.h"

ofMaterial::ofMaterial() {

    diffuse.set(0.8f, 0.8f, 0.8f, 1.0f);
    specular.set(0.0f, 0.0f, 0.0f, 1.0f);
    ambient.set(0.2f, 0.2f, 0.2f, 1.0f);
    emissive.set(0.0f, 0.0f, 0.0f, 1.0f);
    shininess = 0.2;
}

void ofMaterial::addMap(ofTexture& pTexture, ofTextureMapType nMapType) {
	texture_maps[nMapType]=pTexture;
}

ofTexture& ofMaterial::getMap(ofTextureMapType nType) {
	return texture_maps[nType];
}

void ofMaterial::setColors(ofColor oDiffuse, ofColor oAmbient, ofColor oSpecular, ofColor oEmissive) {
	setDiffuseColor(oDiffuse);
	setAmbientColor(oAmbient);
	setSpecularColor(oSpecular);
	setEmissiveColor(oEmissive);
}

void ofMaterial::setDiffuseColor(ofColor oDiffuse) {
	diffuse = oDiffuse/255.f;
	diffuse.a /= 255.f;
}

void ofMaterial::setAmbientColor(ofColor oAmbient) {
	ambient = oAmbient/255.f;
	ambient.a /= 255.f;
}

void ofMaterial::setSpecularColor(ofColor oSpecular) {
	specular = oSpecular/255.f;
	specular.a /= 255.f;
}

void ofMaterial::setEmissiveColor(ofColor oEmissive) {
	emissive = oEmissive/255.f;
	emissive.a /= 255.f;
}

void ofMaterial::setShininess(float nShininess) {
	shininess = nShininess;
}

void ofMaterial::setDiffuseMap(ofTexture& oDiffuseMap) {
	addMap(oDiffuseMap, OF_TEXMAP_DIFFUSE);
}

void ofMaterial::setSpecularMap(ofTexture& oSpecularMap) {
	addMap(oSpecularMap, OF_TEXMAP_SPECULAR);
}

void ofMaterial::setAmbientMap(ofTexture& oAmbientMap) {
	addMap(oAmbientMap, OF_TEXMAP_AMBIENT);
}

void ofMaterial::setHeightMap(ofTexture& oHeightMap) {
	addMap(oHeightMap, OF_TEXMAP_HEIGHT);
}

void ofMaterial::setNormalMap(ofTexture& oNormalMap) {
	addMap(oNormalMap, OF_TEXMAP_NORMAL);
}

void ofMaterial::setShininessMap(ofTexture& oShininessMap) {
	addMap(oShininessMap, OF_TEXMAP_SHININESS);
}

void ofMaterial::setOpactiyMap(ofTexture& oOpacityMap) {
	addMap(oOpacityMap, OF_TEXMAP_OPACITY);
}

void ofMaterial::setDisplacementMap(ofTexture& oDispMap) {
	addMap(oDispMap, OF_TEXMAP_DISPLACEMENT);
}

void ofMaterial::setLightMap(ofTexture& oLightMap) {
	addMap(oLightMap, OF_TEXMAP_LIGHT);
}

void ofMaterial::setReflectionMap(ofTexture& oReflectionMap) {
	addMap(oReflectionMap, OF_TEXMAP_REFLECTION);
}

float ofMaterial::getShininess(){
	return shininess;
}

ofColor ofMaterial::getDiffuseColor() {
	ofColor ret = diffuse*255.f;
	ret.a *= 255.f;
	return ret;
}

ofColor ofMaterial::getAmbientColor() {
	ofColor ret = ambient*255.f;
	ret.a *= 255.f;
	return ret;
}

ofColor ofMaterial::getSpecularColor() {
	ofColor ret = specular*255.f;
	ret.a *= 255.f;
	return ret;
}

ofColor ofMaterial::getEmissiveColor() {
	ofColor ret = emissive*255.f;
	ret.a *= 255.f;
	return ret;
}

void ofMaterial::begin() {
    // save previous values, opengl es cannot use push/pop attrib
	glGetMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,&prev_diffuse.r);
	glGetMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,&prev_specular.r);
	glGetMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,&prev_ambient.r);
	glGetMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,&prev_emissive.r);
	glGetMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);

    // Material colors and properties
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &diffuse.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &specular.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &ambient.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &emissive.r);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	/*for(int i=0; i<OF_TEXMAP_NUM;i++){
		if(texture_maps[i].bAllocated()) {
			getMap((ofTextureMapType)i).bind();
		}
	}*/
}

void ofMaterial::end() {
	/*for(int i=0; i<OF_TEXMAP_NUM;i++){
		if(texture_maps[i].bAllocated()) {
			getMap((ofTextureMapType)i).unbind();
		}
	}*/
    // Set previous material colors and properties
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &prev_diffuse.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &prev_specular.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &prev_ambient.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &prev_emissive.r);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, prev_shininess);
}

