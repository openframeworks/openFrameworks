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
	
	void enable();
	void disable();
	bool getIsEnabled() const;
	
	void setDirectional();
	bool getIsDirectional() const;
	
	void setSpotlight( float spotCutOff=45.f, float exponent=0.f );
	bool getIsSpotlight();
	void setSpotlightCutOff( float spotCutOff );
	void setSpotConcentration( float exponent );
	
	void setPointLight();
	bool getIsPointLight();
	void setAttenuation( float constant=2.f, float linear=1.f, float quadratic=0.5f );
	
	int getType();
	
	void setAmbientColor(const ofFloatColor& c);
	void setDiffuseColor(const ofFloatColor& c);
	void setSpecularColor(const ofFloatColor& c);
	
	ofFloatColor getAmbientColor() const;
	ofFloatColor getDiffuseColor() const;
	ofFloatColor getSpecularColor() const;
	
	int getLightID() const;

	void customDraw() {
		ofPushMatrix();
		ofTranslate(getPosition());
		ofBox(10);
		ofDrawAxis(20);
		ofPopMatrix();
	}
	
	
	// this method overrides ofNode to catch the changes and update glLightv(GL_POSITION)
private:

	ofFloatColor ambientColor;
	ofFloatColor diffuseColor;
	ofFloatColor specularColor;

	ofLightType lightType;
	
	int glIndex;
	int isEnabled;
	bool isDirectional;
	bool isSpotlight;
	
	// update opengl light 
	virtual void onPositionChanged();
	virtual void onOrientationChanged();
};
