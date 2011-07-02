#include "ofMaterial.h"
#include "ofConstants.h"

ofMaterial::ofMaterial() {

    diffuse.set(0.8f, 0.8f, 0.8f, 1.0f);
    specular.set(0.0f, 0.0f, 0.0f, 1.0f);
    ambient.set(0.2f, 0.2f, 0.2f, 1.0f);
    emissive.set(0.0f, 0.0f, 0.0f, 1.0f);
    shininess = 0.2;
}

void ofMaterial::setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor oEmissive) {
	setDiffuseColor(oDiffuse);
	setAmbientColor(oAmbient);
	setSpecularColor(oSpecular);
	setEmissiveColor(oEmissive);
}

void ofMaterial::setDiffuseColor(ofFloatColor oDiffuse) {
	diffuse = oDiffuse;
}

void ofMaterial::setAmbientColor(ofFloatColor oAmbient) {
	ambient = oAmbient;
}

void ofMaterial::setSpecularColor(ofFloatColor oSpecular) {
	specular = oSpecular;
}

void ofMaterial::setEmissiveColor(ofFloatColor oEmissive) {
	emissive = oEmissive;
}

void ofMaterial::setShininess(float nShininess) {
	shininess = nShininess;
}

float ofMaterial::getShininess(){
	return shininess;
}

ofFloatColor ofMaterial::getDiffuseColor() {
	return diffuse;
}

ofFloatColor ofMaterial::getAmbientColor() {
	return ambient;
}

ofFloatColor ofMaterial::getSpecularColor() {
	return specular;
}

ofFloatColor ofMaterial::getEmissiveColor() {
	return emissive;
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
}

void ofMaterial::end() {
    // Set previous material colors and properties
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &prev_diffuse.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &prev_specular.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &prev_ambient.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &prev_emissive.r);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, prev_shininess);
}

