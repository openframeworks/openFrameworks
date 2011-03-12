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
#include "Poco/SharedPtr.h"

#define OF_MAX_LIGHTS		8		// max number of lights allowed by default opengl

void ofEnableLighting();
void ofDisableLighting();
void ofEnableSeparateSpecularLight();
void ofDisableSeparateSpecularLight();
bool ofGetLightingEnabled();

//----------------------------------------
// Use the public API of ofNode for all transformations
class ofLight : public ofNode {
public:
	ofLight();
	virtual ~ofLight(){};
	
	void setup();
	void enable();
	void disable();
	bool getIsEnabled() const;
	
	void setDirectional(bool b);
	bool getIsDirectional() const;
	
	void setAmbientColor(const ofColor& c);
	void setDiffuseColor(const ofColor& c);
	void setSpecularColor(const ofColor& c);
	
	ofColor getAmbientColor() const;
	ofColor getDiffuseColor() const;
	ofColor getSpecularColor() const;
	
	void customDraw() {
		ofPushMatrix();
		ofTranslate(getPosition());
		ofBox(10);
		ofDrawAxis(20);
		ofPopMatrix();
	}
	
	
	// this method overrides ofNode to catch the changes and update glLightv(GL_POSITION)
protected:
	struct Data{
		Data(ofLight*light);
		~Data();

		ofColor ambientColor;
		ofColor diffuseColor;
		ofColor specularColor;

		int glIndex;
		int isEnabled;
		bool isDirectional;

		ofLight * light;
	};
	
	Poco::SharedPtr<Data> data;
	
	
	// update opengl light 
	virtual void onPositionChanged();
	virtual void onOrientationChanged();
};
