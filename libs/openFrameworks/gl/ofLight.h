/*
 *  ofLight.h
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 14/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

// TODO:


#pragma once

#include "ofNode.h"
#include "ofColor.h"
#include "of3dGraphics.h"

#define OF_MAX_LIGHTS		8		// max number of lights allowed by default opengl

enum ofLightType {
	OF_LIGHT_POINT,
	OF_LIGHT_SPOT,
	OF_LIGHT_DIRECTIONAL
};

void ofEnableLighting();
void ofDisableLighting();
void ofEnableSeparateSpecularLight();
void ofDisableSeparateSpecularLight();
bool ofGetLightingEnabled();
void ofSetSmoothLighting(bool b);
void ofSetGlobalAmbientColor(const ofColor& c);

//----------------------------------------
// Use the public API of ofNode for all transformations
class ofLight : public ofNode {
public:
	ofLight();
	ofLight(const ofLight & mom);
	ofLight & operator=(const ofLight & mom);
	virtual ~ofLight();
	void destroy();
	
    void setup();
	void enable();
	void disable();
	bool getIsEnabled() const;
	
	void setDirectional();
	bool getIsDirectional() const;
	
	void setSpotlight( float spotCutOff=45.f, float exponent=0.f );
	bool getIsSpotlight();
	void setSpotlightCutOff( float spotCutOff );
    float getSpotlightCutOff();
	void setSpotConcentration( float exponent );
    float getSpotConcentration();
	
	void setPointLight();
	bool getIsPointLight();
	void setAttenuation( float constant=1.f, float linear=0.f, float quadratic=0.f );
    float getAttenuationConstant();
    float getAttenuationLinear();
    float getAttenuationQuadratic();
	
	int getType();
	
	void setAmbientColor(const ofFloatColor& c);
	void setDiffuseColor(const ofFloatColor& c);
	void setSpecularColor(const ofFloatColor& c);
	
	ofFloatColor getAmbientColor() const;
	ofFloatColor getDiffuseColor() const;
	ofFloatColor getSpecularColor() const;
	
	int getLightID() const;

	void customDraw();
	
	
	// this method overrides ofNode to catch the changes and update glLightv(GL_POSITION)
private:

	ofFloatColor ambientColor;
	ofFloatColor diffuseColor;
	ofFloatColor specularColor;
    
    float attenuation_constant;
    float attenuation_linear;
    float attenuation_quadratic;

	ofLightType lightType;
	
	int glIndex;
	int isEnabled;
	bool isDirectional;
	bool isSpotlight;
    float spotCutOff;
    float exponent;
	
	// update opengl light 
	virtual void onPositionChanged();
	virtual void onOrientationChanged();
};
