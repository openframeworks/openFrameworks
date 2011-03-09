/*
 *  ofLight.cpp
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 14/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#include "ofLight.h"
#include "ofConstants.h"
#include "ofLog.h"
#include "ofUtils.h"


//----------------------------------------
void ofEnableLighting() {
	glEnable(GL_LIGHTING);
#ifndef TARGET_OPENGLES  //TODO: fix this
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
bool lightsActive[OF_MAX_LIGHTS];
bool lightsActiveInited = false;


//----------------------------------------
ofLight::Data::Data(ofLight* _light){
	light = _light;
	glIndex = -1;
	isEnabled = false;
	isDirectional = false;
}

//----------------------------------------
ofLight::Data::~Data(){
	light->setAmbientColor(ofColor(0,0,0,255));
	if(glIndex>0){
		light->setDiffuseColor(ofColor(0,0,0,255));
		light->setSpecularColor(ofColor(0,0,0,255));
	}else{
		light->setDiffuseColor(ofColor(255,255,255,255));
		light->setSpecularColor(ofColor(255,255,255,255));
	}
	GLfloat cc[] = {0,0,1, 0};
	glLightfv(GL_LIGHT0 + glIndex, GL_POSITION, cc);

	light->disable();
	lightsActive[glIndex] = false;
}

//----------------------------------------
ofLight::ofLight():data(NULL) {
	// if array hasn't been inited to false, init it
	if(lightsActiveInited == false) {
		for(int i=0; i<OF_MAX_LIGHTS; i++) lightsActive[i] = false;
		lightsActiveInited = true;
	}
}

//----------------------------------------
void ofLight::setup(){
	data = new Data(this);
	// search for the first free block
	for(int i=0; i<OF_MAX_LIGHTS; i++) {
		if(lightsActive[i] == false) {
			data->glIndex = i;
			enable();
			return;
		}
	}

	ofLog(OF_LOG_ERROR, "Trying to create too many lights: " + ofToString(data->glIndex));
}

//----------------------------------------
void ofLight::enable() {
	if(data && data->glIndex>-1) {
		ofEnableLighting();
		glEnable(GL_LIGHT0 + data->glIndex);
	}
}

//----------------------------------------
void ofLight::disable() {
	if(data->glIndex<OF_MAX_LIGHTS && data->glIndex>-1) {
		glDisable(GL_LIGHT0 + data->glIndex);
	}
}

//----------------------------------------
bool ofLight::getIsEnabled() const {
	return data && data->isEnabled;
}

//----------------------------------------
void ofLight::setDirectional(bool b) {
	if(data) data->isDirectional = b;
}

//----------------------------------------
bool ofLight::getIsDirectional() const {
	return data && data->isDirectional;
}

//----------------------------------------
void ofLight::setAmbientColor(const ofColor& c) {
	if(!data) return;
	data->ambientColor = c/255.0f;
	data->ambientColor.a /= 255.f;
	glLightfv(GL_LIGHT0 + data->glIndex, GL_AMBIENT, &data->ambientColor.r);
}

//----------------------------------------
void ofLight::setDiffuseColor(const ofColor& c) {
	if(!data) return;
	data->diffuseColor = c/255.0f;
	data->diffuseColor.a /= 255.f;
	glLightfv(GL_LIGHT0 + data->glIndex, GL_DIFFUSE, &data->diffuseColor.r);
}


//----------------------------------------
void ofLight::setSpecularColor(const ofColor& c) {
	if(!data) return;
	data->specularColor = c/255.0f;
	data->specularColor.a /= 255.f;
	glLightfv(GL_LIGHT0 + data->glIndex, GL_SPECULAR, &data->specularColor.r);
}

//----------------------------------------
ofColor ofLight::getAmbientColor() const {
	if(!data) return ofColor();
	ofColor ret = data->ambientColor * 255.0f;
	ret.a*=255.f;
	return ret;
}

//----------------------------------------
ofColor ofLight::getDiffuseColor() const {
	if(!data) return ofColor();
	ofColor ret = data->diffuseColor * 255.0f;
	ret.a*=255.f;
	return ret;
}

//----------------------------------------
ofColor ofLight::getSpecularColor() const {
	if(!data) return ofColor();
	ofColor ret = data->specularColor * 255.0f;
	ret.a*=255.f;
	return ret;
}


//----------------------------------------
void ofLight::onPositionChanged() {
	// if we are a positional light and not directional, update light position
	if(data && data->isDirectional == false) {
		GLfloat cc[] = {getPosition().x, getPosition().y, getPosition().z, 1};
		glLightfv(GL_LIGHT0 + data->glIndex, GL_POSITION, cc);
	}
}

//----------------------------------------
void ofLight::onOrientationChanged() {
	// if we are a directional light and not positional, update light position (direction)
	if(data && data->isDirectional == true) {
		GLfloat cc[] = {getLookAtDir().x, getLookAtDir().y, getLookAtDir().z, 0};
		glLightfv(GL_LIGHT0 + data->glIndex, GL_POSITION, cc);
	}
}
