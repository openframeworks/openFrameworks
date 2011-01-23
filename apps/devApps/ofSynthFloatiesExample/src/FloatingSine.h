/*
 *  FloatingSine.h
 *  ofSynth
 *
 *  Created by damian on 12/01/11.
 *  Copyright 2011 frey damian@frey.co.nz. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class FloatingSine
{
public:
	void setup( ofSoundMixer* mixer, vector<FloatingSine*> * neighbours, ofImage* particle );
	
	void update();
	
	void draw();
	
	float getFrequency() { return frequency; }
	ofPoint getPosition() { return position; }
	ofSoundSource* getSoundOutputNodePtr() { return &volume; }
	void toggleWaveform();
	void setWaveform( bool toSawtooth );
	
	ofVec2f position;
	ofVec2f velocity;
	
	void setBaseMidiNote( float base ) { baseMidiNote = base; }
	/// 0 = pentatonic, 1 = lydian
	void setScale( int which );
	
	void setBackgroundAuto( bool bAuto );

private:
	
	ofSoundSourceTestTone tone;
	ofSoundEffectVolume volume;
	
	vector<FloatingSine*> *neighbours;
	
	ofImage* particle;
	ofColor sawColour;
	ofColor sineColour;
	
	float frequency;
	int buddy;
	int enemy;
	float distanceUnits;
	
	bool sawtooth;
	
	bool bBackgroundAuto;
	
	const float* scale;
	int scaleSteps;
	float baseMidiNote;
	
	float shellDistance;
	
	int octaveOffset;
};
