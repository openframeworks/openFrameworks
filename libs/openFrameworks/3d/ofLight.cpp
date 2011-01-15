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

void ofEnableLighting() {
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

void ofDisableLighting() {
	glDisable(GL_LIGHTING);
}

bool ofGetLightingEnabled() {
	return glIsEnabled(GL_LIGHTING);
}


bool lightsActive[OF_MAX_LIGHTS];
bool lightsActiveInited = false;

//----------------------------------------
ofLight::ofLight():index(0), isEnabled(false) {
	// if array hasn't been inited to false, init it
	if(lightsActiveInited == false) {
		for(int i=0; i<OF_MAX_LIGHTS; i++) lightsActive[i] = false;
	}
	
	// search for the first free block
	for(int i=0; i<OF_MAX_LIGHTS; i++) {
		if(lightsActive[i] == false) {
			index = i;
			enable();
			return;
		}
	}
	
	ofLog(OF_LOG_ERROR, "Trying to create too many lights: " + ofToString(index));
	return;
}


//----------------------------------------
ofLight::~ofLight() {
	if(index<OF_MAX_LIGHTS) {
		disable();
		lightsActive[index] = false;
	}
}

//----------------------------------------
void ofLight::enable() {
	if(index<OF_MAX_LIGHTS) {
		ofEnableLighting();
		glEnable(GL_LIGHT0 + index);
	}
}


//----------------------------------------
void ofLight::disable() {
	if(index<OF_MAX_LIGHTS) {
		glDisable(GL_LIGHT0 + index);
	}
}


//----------------------------------------
void ofLight::setAmbientColor(const ofColor& c) {
	ambientColor = c/255.0f;
	GLfloat cc[] = {ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a};
	glLightfv(GL_LIGHT0 + index, GL_AMBIENT, cc);
}

//----------------------------------------
void ofLight::setDiffuseColor(const ofColor& c) {
	diffuseColor = c/255.0f;
	GLfloat cc[] = {diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a};
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, cc);
}


//----------------------------------------
void ofLight::setSpecularColor(const ofColor& c) {
	specularColor = c/255.0f;
	GLfloat cc[] = {specularColor.r, specularColor.g, specularColor.b, specularColor.a};
	glLightfv(GL_LIGHT0 + index, GL_SPECULAR, cc);
}

//----------------------------------------
ofColor ofLight::getAmbientColor() const {
	return ambientColor * 255.0f;
}

//----------------------------------------
ofColor ofLight::getDiffuseColor() const {
	return diffuseColor * 255.0f;
}

//----------------------------------------
ofColor ofLight::getSpecularColor() const {
	return specularColor * 255.0f;
}
