/*
 *  ofLight.cpp
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 14/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#include "ofLight.h"
#include "ofMain.h"
#include "ofConstants.h"

// http://glprogramming.com/red/chapter05.html#name11 //
// Not sure where material functions should be placed //

//----------------------------------------
void ofMaterialAmbient(const ofColor& c) {
	GLfloat cc[] = {c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, cc);
}

//----------------------------------------
void ofMaterialAmbient(float r, float g, float b, float a) {
	ofMaterialAmbient(ofColor(r, g, b, a));
}

//----------------------------------------
void ofMaterialDiffuse(const ofColor& c) {
	GLfloat cc[] = {c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cc);
}

//----------------------------------------
void ofMaterialDiffuse(float r, float g, float b, float a) {
	ofMaterialDiffuse(ofColor(r, g, b, a));
}

//----------------------------------------
void ofMaterialSpecular(const ofColor& c) {
	GLfloat cc[] = {c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f};
	glMaterialfv(GL_FRONT, GL_SPECULAR, cc);
}

//----------------------------------------
void ofMaterialSpecular(float r, float g, float b, float a) {
	ofMaterialSpecular(ofColor(r, g, b, a));
}

//----------------------------------------
void ofMaterialEmission(const ofColor& c) {
	GLfloat cc[] = {c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f};
	glMaterialfv(GL_FRONT, GL_EMISSION, cc);
}

//----------------------------------------
void ofMaterialEmission(float r, float g, float b, float a) {
	ofMaterialEmission(ofColor(r, g, b, a));
}

// 0 - 128
//----------------------------------------
void ofMaterialShininess(float s) {
	glMaterialf(GL_FRONT, GL_SHININESS, CLAMP(s, 0, 128));
}




//----------------------------------------
void ofEnableLighting() {
	glEnable(GL_LIGHTING);
	#ifndef TARGET_OF_IPHONE 
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	#endif
	glEnable(GL_COLOR_MATERIAL);
}

//----------------------------------------
void ofDisableLighting() {
	glDisable(GL_LIGHTING);
}

//----------------------------------------
bool ofGetLightingEnabled() {
	return glIsEnabled(GL_LIGHTING);
}

//----------------------------------------
void ofSetSmoothLighting(bool b) {
	if (b) glShadeModel(GL_SMOOTH);
	else glShadeModel(GL_FLAT);
}

//----------------------------------------
void ofSetGlobalAmbientColor(const ofColor& c) {
	GLfloat cc[] = {c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, cc);
}

//----------------------------------------
void ofSetGlobalAmbientColor(float r, float g, float b, float a) {
	ofSetGlobalAmbientColor( ofColor(r, g, b, a) );
}

//----------------------------------------
bool lightsActive[OF_MAX_LIGHTS];
bool lightsActiveInited = false;

ofLight::ofLight():glIndex(0), isEnabled(false) {
	// if array hasn't been inited to false, init it
	if(lightsActiveInited == false) {
		for(int i=0; i<OF_MAX_LIGHTS; i++) lightsActive[i] = false;
		lightsActiveInited = true;
	}
	
	// search for the first free block
	for(int i=0; i<OF_MAX_LIGHTS; i++) {
		if(lightsActive[i] == false) {
			glIndex = i;
			lightsActive[i] = true;
			enable();
			return;
		}
	}
	
	ofLog(OF_LOG_ERROR, "Trying to create too many lights: " + ofToString(glIndex));
	return;
}


//----------------------------------------
ofLight::~ofLight() {
	if(glIndex<OF_MAX_LIGHTS) {
		disable();
		lightsActive[glIndex] = false;
	}
}

//----------------------------------------
void ofLight::enable() {
	if(glIndex<OF_MAX_LIGHTS) {
		ofEnableLighting();
		glEnable(GL_LIGHT0 + glIndex);
	}
}


//----------------------------------------
void ofLight::disable() {
	if(glIndex<OF_MAX_LIGHTS) {
		glDisable(GL_LIGHT0 + glIndex);
	}
}

//----------------------------------------
bool ofLight::getIsEnabled() const {
	return isEnabled;
}


//----------------------------------------
void ofLight::setDirectional(bool b) {
	isDirectional = b;
}

//----------------------------------------
bool ofLight::getIsDirectional() const {
	return isDirectional;
}

//----------------------------------------
void ofLight::setSpotlight(float spotCutOff, float exponent) {
	setDirectional(false);
	isSpotlight		= true;
	setSpotlightCutOff( spotCutOff );
	setSpotConcentration( exponent );
}

//----------------------------------------
bool ofLight::getIsSpotlight() {
	return isSpotlight;
}

//----------------------------------------
void ofLight::setSpotlightCutOff( float spotCutOff ) {
	glLightf(GL_LIGHT0 + glIndex, GL_SPOT_CUTOFF, CLAMP(spotCutOff, 0, 90) );
}

//----------------------------------------
void ofLight::setSpotConcentration( float exponent ) {
	glLightf(GL_LIGHT0 + glIndex, GL_SPOT_EXPONENT, exponent);
}



//----------------------------------------
void ofLight::setPointLight() {
	setDirectional( false );
	isSpotlight		= false;
}

//----------------------------------------
bool ofLight::getIsPointLight() {
	return (!isDirectional && !isSpotlight);
}



//----------------------------------------
void ofLight::setAttenuation( float constant, float linear, float quadratic ) {
	glLightf(GL_LIGHT0 + glIndex, GL_CONSTANT_ATTENUATION, constant);
	glLightf(GL_LIGHT0 + glIndex, GL_LINEAR_ATTENUATION, linear);
	glLightf(GL_LIGHT0 + glIndex, GL_QUADRATIC_ATTENUATION, quadratic);
}




//----------------------------------------
void ofLight::setAmbientColor(const ofColor& c) {
	ambientColor = c/255.0f;
	ambientColor.a = c.a/255.0f;
	GLfloat cc[] = {ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a};
	glLightfv(GL_LIGHT0 + glIndex, GL_AMBIENT, cc);
}

//----------------------------------------
void ofLight::setAmbientColor(float r, float g, float b, float a) {
	setAmbientColor(r, g, b, a);
}

//----------------------------------------
void ofLight::setDiffuseColor(const ofColor& c) {
	diffuseColor = c/255.0f;
	diffuseColor.a = c.a/255.0f;
	GLfloat cc[] = {diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a};
	glLightfv(GL_LIGHT0 + glIndex, GL_DIFFUSE, cc);
}

//----------------------------------------
void ofLight::setDiffuseColor(float r, float g, float b, float a) {
	setDiffuseColor(ofColor(r, g, b, a));
}

//----------------------------------------
void ofLight::setSpecularColor(const ofColor& c) {
	specularColor = c/255.0f;
	specularColor.a = c.a/255.0f;
	GLfloat cc[] = {specularColor.r, specularColor.g, specularColor.b, specularColor.a};
	glLightfv(GL_LIGHT0 + glIndex, GL_SPECULAR, cc);
}

//----------------------------------------
void ofLight::setSpecularColor(float r, float g, float b, float a) {
	setSpecularColor(ofColor(r, g, b, a));
}

//----------------------------------------
ofColor ofLight::getAmbientColor() const {
	return ofColor(ambientColor.r * 255.f, ambientColor.g * 255.f, ambientColor.b * 255.f, ambientColor.a * 255.f);
}

//----------------------------------------
ofColor ofLight::getDiffuseColor() const {
	return ofColor(diffuseColor.r * 255.0f, diffuseColor.g * 255.f, diffuseColor.b * 255.f, diffuseColor.a * 255.f);
}

//----------------------------------------
ofColor ofLight::getSpecularColor() const {
	return ofColor(specularColor.r * 255.f, specularColor.g * 255.f, specularColor.b * 255.f, specularColor.a * 255.f);
}


//----------------------------------------
void ofLight::onPositionChanged() {
	// if we are a positional light and not directional, update light position
	if(isDirectional == false) {
		GLfloat cc[] = {getPosition().x, getPosition().y, getPosition().z, 1};
		glLightfv(GL_LIGHT0 + glIndex, GL_POSITION, cc);
	}
}

//----------------------------------------
void ofLight::onOrientationChanged() {
	// if we are a directional light and not positional, update light position (direction)
	if(isDirectional == true) {
		GLfloat cc[] = {getLookAtDir().x, getLookAtDir().y, getLookAtDir().z, 0};
		glLightfv(GL_LIGHT0 + glIndex, GL_POSITION, cc);
	} else {
		if(isSpotlight) {
			// determines the axis of the cone light //
			GLfloat spot_direction[] = { getLookAtDir().x, getLookAtDir().y, getLookAtDir().z, 1.0 };
			glLightfv(GL_LIGHT0 + glIndex, GL_SPOT_DIRECTION, spot_direction);
		}
	}
}





