/*
 *  Swarm.cpp
 *  advanced3dExample
 *
 *  Created by Elliot Woods on 19/01/2011.
 *  Copyright 2011 Kimchi and Chips. All rights reserved.
 *
 */

#include "Swarm.h"

Swarm::Swarm()
{
	//constructor, let's set some defaults
	nParticles = 0;	
	light.setAmbientColor(ofColor(0,0,0));
}

void Swarm::init(int _nParticles, float positionDispersion, float velocityDispersion)
{
	///////////////////////////////////////////
	//if nParticles>0, we must already have
	//initialised our arrays, so let's clear them
	//
	if (nParticles > 0)
	{
		ofLog(OF_LOG_WARNING, "Swarm: Already initialised");
		
		//delete[] = delete array from memory
		delete[] positions;
		delete[] velocities;
		delete[] colors;
		
		//superfluous line of code..
		nParticles = 0;
	}
	///////////////////////////////////////////
	
	
	//Swarm's internal variable set from argument
	nParticles = _nParticles;	
	
	
	///////////////////////////////////////////
	// SETUP ARRAYS
	///////////////////////////////////////////
	//
	positions = new glm::vec3[nParticles];
	velocities = new glm::vec3[nParticles];
	colors = new ofColor[nParticles];
	//
	///////////////////////////////////////////
	
	
	///////////////////////////////////////////
	// INITIALISE VALUES
	///////////////////////////////////////////
	//
	ofSeedRandom();
	//
	for (int i=0; i< nParticles; i++)
	{
		positions[i].x = (ofRandom(1.0f)-0.5f)  * positionDispersion;
		positions[i].y = (ofRandom(1.0f)-0.5f)  * positionDispersion;
		positions[i].z = (ofRandom(1.0f)-0.5f)  * positionDispersion;
		
		velocities[i].x = (ofRandom(1.0f)-0.5f)  * velocityDispersion;
		velocities[i].y = (ofRandom(1.0f)-0.5f)  * velocityDispersion;
		velocities[i].z = (ofRandom(1.0f)-0.5f)  * velocityDispersion;
		
		colors[i].r = ofRandom(255.0f);
		colors[i].g = ofRandom(255.0f);
		colors[i].b = 150.0f;
		colors[i].a = 255.0f;
	}
	//
	///////////////////////////////////////////
	
}

void Swarm::customDraw()
{
	///we run the update ourselves manually
	update();
	
	//we use the position of the first
	//particle as the position of the
	//light
	ofPushStyle();
	ofEnableLighting();
	light.enable();
	light.setPosition(positions[0]);

	///////////////////////////////////////////
	// DRAW SPHERES
	///////////////////////////////////////////
	//
	for (int i=0; i< nParticles; i++)
	{
		ofPushStyle();
		ofSetColor(colors[i]);
		
		ofDrawSphere(positions[i], 1.0);
		
		ofPopStyle();
	}
	//
	///////////////////////////////////////////
	
	light.disable();
	ofDisableLighting();
	
	//render light as white sphere
	ofSetColor(255, 255, 255);
	ofDrawSphere(positions[0], 2.0);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
	ofDrawBitmapString(" light", (glm::vec3)positions[0]);
	ofPopStyle();
}

void Swarm::update()
{
	
	//calculate time past per frame
	float dt = ofGetElapsedTimef() - timeLastUpdate;
	timeLastUpdate = ofGetElapsedTimef();
	
	//update positions, velocities
	for (int i=0; i< nParticles; i++)
	{
		////////////////////////////////
		// 
		//	MOTION MATHS
		//
		//		'Simple Harmonic Motion'
		//
		////////////////////////////////
		// (1) integrate the velocity
		//
		//v = dx / dt (*)
		//x = x + dx [every frame]
		//
		//therefore
		//x = x + v * dt (*)
		//
		//velcotity is taken from previous frame
		//
		positions[i] += velocities[i] * dt;
		//
		/////////////////////////////
		//
		/////////////////////////////
		// (2) integrate the acceleration
		//
		//a = -k * x
		//a = dv / dt
		//
		//therefore from (*)s above
		//(v = v + dv)
		//
		//v = v + (dt * a)
		//v = v + (dt * -k * x)
		//
		velocities[i] += - SPRING_CONSTANT * positions[i] * dt;		
		//
		/////////////////////////////////

	}
}


