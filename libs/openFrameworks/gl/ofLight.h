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
#include "ofTypes.h"

enum ofLightType {
	OF_LIGHT_POINT=0,
	OF_LIGHT_DIRECTIONAL=1,
	OF_LIGHT_SPOT=2,
	OF_LIGHT_AREA=3 // Only programmable renderer
};

void ofEnableLighting();
void ofDisableLighting();
void ofEnableSeparateSpecularLight();
void ofDisableSeparateSpecularLight();
bool ofGetLightingEnabled();
void ofSetSmoothLighting(bool b);
void ofSetGlobalAmbientColor(const ofFloatColor& c);
const ofFloatColor & ofGetGlobalAmbientColor();

//----------------------------------------
// Use the public API of ofNode for all transformations
class ofLight : public ofNode {
public:
	ofLight();
	
    void setup();
	void enable();
	void disable();
	bool getIsEnabled() const;
	
	void setDirectional();
	bool getIsDirectional() const;
	
	void setSpotlight( float spotCutOff=45.f, float exponent=0.f );
	bool getIsSpotlight() const;
	void setSpotlightCutOff( float spotCutOff );
    float getSpotlightCutOff() const;
	void setSpotConcentration( float exponent );
    float getSpotConcentration() const;
	
	void setPointLight();
	bool getIsPointLight() const;
	void setAttenuation( float constant=1.f, float linear=0.f, float quadratic=0.f );
    float getAttenuationConstant() const;
    float getAttenuationLinear() const;
    float getAttenuationQuadratic() const;

    void setAreaLight(float width, float height);
    bool getIsAreaLight() const;
	
	int getType() const;
	
	void setAmbientColor(const ofFloatColor& c);
	void setDiffuseColor(const ofFloatColor& c);
	void setSpecularColor(const ofFloatColor& c);
	
	ofFloatColor getAmbientColor() const;
	ofFloatColor getDiffuseColor() const;
	ofFloatColor getSpecularColor() const;
	
	int getLightID() const;

	class Data{
	public:
		Data();
		~Data();

		ofFloatColor ambientColor;
		ofFloatColor diffuseColor;
		ofFloatColor specularColor;

	    float attenuation_constant;
	    float attenuation_linear;
	    float attenuation_quadratic;

		ofLightType lightType;

		int glIndex;
		int isEnabled;
	    float spotCutOff;
	    float exponent;
	    ofVec4f position;
	    ofVec3f direction;

	    float width;
	    float height;
	    ofVec3f up;
	    ofVec3f right;
	};
	
private:
	void customDraw(const ofBaseRenderer * renderer) const;
	shared_ptr<Data> data;
	// update opengl light 
	// this method overrides ofNode to catch the changes and update glLightv(GL_POSITION)
	virtual void onPositionChanged();
	virtual void onOrientationChanged();
};


vector<weak_ptr<ofLight::Data> > & ofLightsData();
