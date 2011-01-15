/*
 *  ofLight.h
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 14/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#pragma once

#include "ofNode.h"
#include "ofColor.h"

#define OF_MAX_LIGHTS		8		// max number of lights allowed by opengl

void ofEnableLighting();
void ofDisableLighting();
bool ofGetLightingEnabled();

//----------------------------------------
class ofLight : public ofNode {
public:
	ofLight();
	~ofLight();
	
	void enable();
	void disable();
	void getIsEnabled();
	
	void setAmbientColor(const ofColor& c);
	void setDiffuseColor(const ofColor& c);
	void setSpecularColor(const ofColor& c);
	
	ofColor getAmbientColor() const;
	ofColor getDiffuseColor() const;
	ofColor getSpecularColor() const;
	
protected:
	ofColor ambientColor;
	ofColor diffuseColor;
	ofColor specularColor;
	
	int index;
	int isEnabled;
	
};
