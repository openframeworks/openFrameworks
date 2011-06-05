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
#include <map>


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
void ofEnableSeparateSpecularLight(){
#ifndef TARGET_OPENGLES
	glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
#endif
}

//----------------------------------------
void ofDisableSeparateSpecularLight(){
#ifndef TARGET_OPENGLES
	glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL,GL_SINGLE_COLOR);
#endif
}

//----------------------------------------
bool ofGetLightingEnabled() {
	return glIsEnabled(GL_LIGHTING);
}

//----------------------------------------
bool* getActiveLights(){
	static bool * lightsActive = new bool[OF_MAX_LIGHTS];
	static bool lightsActiveInited = false;
	// if array hasn't been inited to false, init it
	if(lightsActiveInited == false) {
		for(int i=0; i<OF_MAX_LIGHTS; i++) lightsActive[i] = false;
		lightsActiveInited = true;
	}
	return lightsActive;
}

static map<GLuint,int> & getIds(){
	static map<GLuint,int> * ids = new map<GLuint,int>;
	return *ids;
}

//--------------------------------------------------------------
static void retain(int id){
	if(id==-1) return;
	if(getIds().find(id)!=getIds().end()){
		getIds()[id]++;
	}else{
		getIds()[id]=1;
	}
}

//--------------------------------------------------------------
static void release(ofLight & light){
	int id = light.getLightID();
	if(id==-1) return;
	bool lastRef=false;
	if(getIds().find(id)!=getIds().end()){
		getIds()[id]--;
		if(getIds()[id]==0){
			lastRef=true;
			getIds().erase(id);
		}
	}else{
		ofLog(OF_LOG_WARNING,"ofVbo: releasing id not found, this shouldn't be happening releasing anyway");
		lastRef=true;
	}
	if(lastRef){
		light.setAmbientColor(ofColor(0,0,0,255));
		if(id>0){
			light.setDiffuseColor(ofColor(0,0,0,255));
			light.setSpecularColor(ofColor(0,0,0,255));
		}else{
			light.setDiffuseColor(ofColor(255,255,255,255));
			light.setSpecularColor(ofColor(255,255,255,255));
		}
		GLfloat cc[] = {0,0,1, 0};
		glLightfv(GL_LIGHT0 + id, GL_POSITION, cc);

		light.disable();
		getActiveLights()[id] = false;
	}
}

//----------------------------------------
ofLight::ofLight(){
	glIndex = -1;
	isEnabled = false;
	isDirectional = false;
}

//----------------------------------------
ofLight::~ofLight(){
	release(*this);
}

//----------------------------------------
ofLight::ofLight(const ofLight & mom){
	ambientColor = mom.ambientColor;
	diffuseColor = mom.diffuseColor;
	specularColor = mom.specularColor;

	glIndex = mom.glIndex;
	retain(glIndex);
	isEnabled = mom.isEnabled;
	isDirectional  = mom.isDirectional;
}

//----------------------------------------
ofLight & ofLight::operator=(const ofLight & mom){
	if(&mom == this) return *this;
	ambientColor = mom.ambientColor;
	diffuseColor = mom.diffuseColor;
	specularColor = mom.specularColor;

	glIndex = mom.glIndex;
	retain(glIndex);
	isEnabled = mom.isEnabled;
	isDirectional  = mom.isDirectional;
	return *this;
}

//----------------------------------------
void ofLight::enable() {
	if(glIndex==-1){
		// search for the first free block
		for(int i=0; i<OF_MAX_LIGHTS; i++) {
			if(getActiveLights()[i] == false) {
				glIndex = i;
				retain(glIndex);
				enable();
				return;
			}
		}
	}
	if(glIndex!=-1) {
		ofEnableLighting();
		glEnable(GL_LIGHT0 + glIndex);
	}else{
		ofLog(OF_LOG_ERROR, "Trying to create too many lights: " + ofToString(glIndex));
	}
}

//----------------------------------------
void ofLight::disable() {
	if(glIndex!=-1) {
		glDisable(GL_LIGHT0 + glIndex);
	}
}

//----------------------------------------
int ofLight::getLightID() const{
	return glIndex;
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
void ofLight::setAmbientColor(const ofFloatColor& c) {
	if(glIndex==-1) return;
	glLightfv(GL_LIGHT0 + glIndex, GL_AMBIENT, &ambientColor.r);
}

//----------------------------------------
void ofLight::setDiffuseColor(const ofFloatColor& c) {
	if(glIndex==-1) return;
	glLightfv(GL_LIGHT0 + glIndex, GL_DIFFUSE, &diffuseColor.r);
}


//----------------------------------------
void ofLight::setSpecularColor(const ofFloatColor& c) {
	if(glIndex==-1) return;
	glLightfv(GL_LIGHT0 + glIndex, GL_SPECULAR, &specularColor.r);
}

//----------------------------------------
ofFloatColor ofLight::getAmbientColor() const {
	return ambientColor;
}

//----------------------------------------
ofFloatColor ofLight::getDiffuseColor() const {
	return diffuseColor;
}

//----------------------------------------
ofFloatColor ofLight::getSpecularColor() const {
	return specularColor;
}


//----------------------------------------
void ofLight::onPositionChanged() {
	if(glIndex==-1) return;
	// if we are a positional light and not directional, update light position
	if(isDirectional == false) {
		GLfloat cc[] = {getPosition().x, getPosition().y, getPosition().z, 1};
		glLightfv(GL_LIGHT0 + glIndex, GL_POSITION, cc);
	}
}

//----------------------------------------
void ofLight::onOrientationChanged() {
	if(glIndex==-1) return;
	// if we are a directional light and not positional, update light position (direction)
	if(isDirectional == true) {
		GLfloat cc[] = {getLookAtDir().x, getLookAtDir().y, getLookAtDir().z, 0};
		glLightfv(GL_LIGHT0 + glIndex, GL_POSITION, cc);
	}
}
