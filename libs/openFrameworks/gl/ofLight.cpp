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


static bool normalsEnabled=false;

//----------------------------------------
void ofEnableLighting() {
	glEnable(GL_LIGHTING);
#ifndef TARGET_OPENGLES  //TODO: fix this
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
#endif
	glEnable(GL_COLOR_MATERIAL);

	// FIXME: we do this so the 3d ofDraw* functions work with lighting
	// but if someone enables it between ofEnableLighting it'll be disabled
	// on ofDisableLighting. by now it seems the best option to not loose
	// performance when drawing lots of primitives
	normalsEnabled = glIsEnabled( GL_NORMALIZE );
	glEnable(GL_NORMALIZE);
}

//----------------------------------------
void ofDisableLighting() {
	glDisable(GL_LIGHTING);
	if(!normalsEnabled){
		glDisable(GL_NORMALIZE);
	}
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

//--------------------------------------------------------------
static map<GLuint,int> & getIds(){
	static map<GLuint,int> * ids = new map<GLuint,int>;
	return *ids;
}

//--------------------------------------------------------------
static void retain(int id){
	if(id==-1) return;
	getActiveLights()[id] = true;
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
		ofLogWarning("ofLight") << "release(): something's wrong here, releasing unknown light id " << id;
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
	glIndex			= -1;
	isEnabled		= false;
    setAmbientColor(ofColor(0,0,0));
    setDiffuseColor(ofColor(255,255,255));
    setSpecularColor(ofColor(255,255,255));
	setPointLight();
    
    // assume default attenuation factors //
    setAttenuation(1.f,0.f,0.f);
}

//----------------------------------------
ofLight::~ofLight(){
	destroy();
}

//----------------------------------------
void ofLight::destroy(){
	release(*this);
}

//----------------------------------------
ofLight::ofLight(const ofLight & mom){
	ambientColor	= mom.ambientColor;
	diffuseColor	= mom.diffuseColor;
	specularColor	= mom.specularColor;

	glIndex = mom.glIndex;
	retain(glIndex);
	isEnabled		= mom.isEnabled;
	isDirectional	= mom.isDirectional;
	isSpotlight		= mom.isSpotlight;
	lightType		= mom.lightType;
	exponent		= mom.exponent;
	attenuation_constant = mom.attenuation_constant;
	spotCutOff 		= mom.spotCutOff;
	attenuation_linear = mom.attenuation_linear;
	attenuation_quadratic = mom.attenuation_quadratic;
}

//----------------------------------------
ofLight & ofLight::operator=(const ofLight & mom){
	if(&mom == this) return *this;
	release(*this);

	ambientColor = mom.ambientColor;
	diffuseColor = mom.diffuseColor;
	specularColor = mom.specularColor;

	glIndex = mom.glIndex;
	retain(glIndex);
	isEnabled		= mom.isEnabled;
	isDirectional	= mom.isDirectional;
	isSpotlight		= mom.isSpotlight;
	lightType		= mom.lightType;
	exponent		= mom.exponent;
	attenuation_constant = mom.attenuation_constant;
	spotCutOff 		= mom.spotCutOff;
	attenuation_linear = mom.attenuation_linear;
	attenuation_quadratic = mom.attenuation_quadratic;
	return *this;
}

//----------------------------------------
void ofLight::setup() {
    if(glIndex==-1){
		bool bLightFound = false;
		// search for the first free block
		for(int i=0; i<OF_MAX_LIGHTS; i++) {
			if(getActiveLights()[i] == false) {
				glIndex = i;
				retain(glIndex);
				bLightFound = true;
				break;
			}
		}
		if( !bLightFound ){
			ofLogError("ofLight") << "setup(): couldn't get active GL light, maximum number of "<< OF_MAX_LIGHTS << " reached";
		}
        if(bLightFound) {
            // run this the first time, since it was not found before //
            onPositionChanged();
            setAmbientColor( getAmbientColor() );
            setDiffuseColor( getDiffuseColor() );
            setSpecularColor( getSpecularColor() );
            setAttenuation( getAttenuationConstant(), getAttenuationLinear(), getAttenuationQuadratic() );
            if(getIsSpotlight()) {
                setSpotlightCutOff(getSpotlightCutOff());
                setSpotConcentration(getSpotConcentration());
            }
            if(getIsSpotlight() || getIsDirectional()) {
                onOrientationChanged();
            }
        }
	}
}

//----------------------------------------
void ofLight::enable() {
    setup();
    onPositionChanged(); // update the position //
	onOrientationChanged();
	ofEnableLighting();
	glEnable(GL_LIGHT0 + glIndex);
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
void ofLight::setDirectional() {
	isDirectional	= true;
	isSpotlight		= false;
	lightType		= OF_LIGHT_DIRECTIONAL;
}

//----------------------------------------
bool ofLight::getIsDirectional() const {
	return isDirectional;
}

//----------------------------------------
void ofLight::setSpotlight(float spotCutOff, float exponent) {
	isDirectional	= false;
	isSpotlight		= true;
	lightType		= OF_LIGHT_SPOT;
	setSpotlightCutOff( spotCutOff );
	setSpotConcentration( exponent );
}

//----------------------------------------
bool ofLight::getIsSpotlight() {
	return isSpotlight;
}

//----------------------------------------
void ofLight::setSpotlightCutOff( float spotCutOff ) {
    this->spotCutOff = CLAMP(spotCutOff, 0, 90);
	glLightf(GL_LIGHT0 + glIndex, GL_SPOT_CUTOFF, this->spotCutOff );
}

//----------------------------------------
float ofLight::getSpotlightCutOff() {
    if(!getIsSpotlight()) {
        ofLogWarning("ofLight") << "getSpotlightCutOff(): light " << glIndex << " is not a spot light";
    }
    return spotCutOff;
}

//----------------------------------------
void ofLight::setSpotConcentration( float exponent ) {
    this->exponent = CLAMP(exponent, 0, 128);
	glLightf(GL_LIGHT0 + glIndex, GL_SPOT_EXPONENT, this->exponent);
}

//----------------------------------------
float ofLight::getSpotConcentration() {
    if(!getIsSpotlight()) {
        ofLogWarning("ofLight") << "getSpotConcentration(): light " << glIndex << " is not a spot light";
    }
    return exponent;
}

//----------------------------------------
void ofLight::setPointLight() {
	isDirectional	= false;
	isSpotlight	= false;
	lightType	= OF_LIGHT_POINT;
}

//----------------------------------------
bool ofLight::getIsPointLight() {
	return (!isDirectional && !isSpotlight);
}

//----------------------------------------
void ofLight::setAttenuation( float constant, float linear, float quadratic ) {
    // falloff = 0 -> 1, 0 being least amount of fallof, 1.0 being most //
    attenuation_constant    = constant;
    attenuation_linear      = linear; 
    attenuation_quadratic   = quadratic;
    
    if(glIndex==-1) return;
	glLightf(GL_LIGHT0 + glIndex, GL_CONSTANT_ATTENUATION, attenuation_constant);
	glLightf(GL_LIGHT0 + glIndex, GL_LINEAR_ATTENUATION, attenuation_linear);
	glLightf(GL_LIGHT0 + glIndex, GL_QUADRATIC_ATTENUATION, attenuation_quadratic);

}

//----------------------------------------
float ofLight::getAttenuationConstant() {
    return attenuation_constant;
}

//----------------------------------------
float ofLight::getAttenuationLinear() {
    return attenuation_linear;
}

//----------------------------------------
float ofLight::getAttenuationQuadratic() {
    return attenuation_quadratic;
}

//----------------------------------------
int ofLight::getType() {
	return lightType;
}

//----------------------------------------
void ofLight::setAmbientColor(const ofFloatColor& c) {
	ambientColor = c;
    if(glIndex==-1) return;
	glLightfv(GL_LIGHT0 + glIndex, GL_AMBIENT, &ambientColor.r);
}

//----------------------------------------
void ofLight::setDiffuseColor(const ofFloatColor& c) {
	diffuseColor = c;
    if(glIndex==-1) return;
	glLightfv(GL_LIGHT0 + glIndex, GL_DIFFUSE, &diffuseColor.r);
}

//----------------------------------------
void ofLight::setSpecularColor(const ofFloatColor& c) {
	specularColor = c;
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
void ofLight::customDraw() {
    if(getIsPointLight()) {
        ofDrawSphere( 0,0,0, 10);
    } else if (getIsSpotlight()) {
        float coneHeight = (sin(spotCutOff*DEG_TO_RAD) * 30.f) + 1;
        float coneRadius = (cos(spotCutOff*DEG_TO_RAD) * 30.f) + 8;
		ofPushMatrix();
		ofRotate(-90, 1, 0, 0);
		ofDrawCone(0, -(coneHeight*.5), 0, coneHeight, coneRadius);
		ofPopMatrix();
    } else {
        ofDrawBox(10);
    }
    ofDrawAxis(20);
}


//----------------------------------------
void ofLight::onPositionChanged() {
	// TODO: (tig) fix this.  this breaks udpate() thread safety (openGL should not be called in update() but only draw() ),
	// since this method will most likely be called during update()
	// if the light is parented and the parent node changes position during update().

	if(glIndex==-1) return;
	// if we are a positional light and not directional, update light position
	if(isDirectional == false) {
		// (tig) this fixes an issue in case the light is parented
		GLfloat cc[] = {getGlobalPosition().x, getGlobalPosition().y, getGlobalPosition().z, 1};
		glLightfv(GL_LIGHT0 + glIndex, GL_POSITION, cc);
	}


}

//----------------------------------------
void ofLight::onOrientationChanged() {
	// TODO: (tig) fix this.  this breaks udpate() thread safety (openGL should not be called in update() but only draw() ),
	// since this method will most likely be called during update()
	// if the light is parented and the parent node changes orientation during update().

	if(glIndex==-1) return;
	// if we are a directional light and not positional, update light position (direction)
	if(isDirectional == true) {
		// (tig) takes into account global orientation should node be parented.
		ofVec3f lookAtDir = ( getGlobalTransformMatrix().getInverse() * ofVec4f(0,0,-1, 1) ).getNormalized();
		GLfloat cc[] = {lookAtDir.x, lookAtDir.y, lookAtDir.z, 0};
		glLightfv(GL_LIGHT0 + glIndex, GL_POSITION, cc);
	} else {
		if(isSpotlight) {
			// determines the axis of the cone light //
			
			// (tig) takes into account global orientation should node be parented.
			ofVec3f lookAtDir = ( getGlobalTransformMatrix().getInverse() * ofVec4f(0,0,-1, 1) ).getNormalized();
			
			GLfloat spot_direction[] = { lookAtDir.x, lookAtDir.y, lookAtDir.z, 1.0};
			glLightfv(GL_LIGHT0 + glIndex, GL_SPOT_DIRECTION, spot_direction);
		}
	}
}
