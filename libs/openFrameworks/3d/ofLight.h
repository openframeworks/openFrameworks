/*
 *  ofLight.h
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 14/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

// TODO:
// attenuation
// spotlight stuff


#pragma once

#include "ofNode.h"
#include "ofColor.h"

#define OF_MAX_LIGHTS		8		// max number of lights allowed by default opengl

void ofEnableLighting();
void ofDisableLighting();
bool ofGetLightingEnabled();
void ofSetSmoothLighting(bool b);
void ofSetGlobalAmbientColor(const ofColor& c);
void ofSetGlobalAmbientColor(float r, float g, float b, float a=255.f);

//----------------------------------------
void ofMaterialAmbient(const ofColor& c);
void ofMaterialAmbient(float r, float g, float b, float a=255.f);
void ofMaterialDiffuse(const ofColor& c);
void ofMaterialDiffuse(float r, float g, float b, float a=255.f);
void ofMaterialSpecular(const ofColor& c);
void ofMaterialSpecular(float r, float g, float b, float a=255.f);
void ofMaterialEmission(const ofColor& c);
void ofMaterialEmission(float r, float g, float b, float a=255.f);
void ofMaterialShininess(float s);

//----------------------------------------
// Use the public API of ofNode for all transformations
class ofLight : public ofNode {
public:
	ofLight();
	~ofLight();
	
	void enable();
	void disable();
	bool getIsEnabled() const;
	
	void setDirectional(bool b=true);
	bool getIsDirectional() const;
	
	void setSpotlight( float spotCutOff=45.f, float exponent=0.f );
	bool getIsSpotlight();
	void setSpotlightCutOff( float spotCutOff );
	void setSpotConcentration( float exponent );
	
	void setPointLight();
	bool getIsPointLight();
	
	void setAttenuation( float constant=2.f, float linear=1.f, float quadratic=0.5f );
	
	void setAmbientColor(const ofColor& c);
	void setAmbientColor(float r, float g, float b, float a=255.f);
	void setDiffuseColor(const ofColor& c);
	void setDiffuseColor(float r, float g, float b, float a=255.f);
	void setSpecularColor(const ofColor& c);
	void setSpecularColor(float r, float g, float b, float a=255.f);
	
	ofColor getAmbientColor() const;
	ofColor getDiffuseColor() const;
	ofColor getSpecularColor() const;
	
	void customDraw() {
		if (!isSpotlight) {
			ofBox(10);
		} else {
			
		}
		
		ofDrawAxis(20);
	}
	
	
	// this method overrides ofNode to catch the changes and update glLightv(GL_POSITION)
protected:
	ofColor ambientColor;
	ofColor diffuseColor;
	ofColor specularColor;
	
	int glIndex;
	int isEnabled;
	bool isDirectional;
	bool isSpotlight;
	
	
	// update opengl light 
	virtual void onPositionChanged();
	virtual void onOrientationChanged();
};
