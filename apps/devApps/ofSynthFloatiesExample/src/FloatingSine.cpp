/*
 *  FloatingSine.cpp
 *  ofSynth
 *
 *  Created by damian on 12/01/11.
 *  Copyright 2011 frey damian@frey.co.nz. All rights reserved.
 *
 */


#include "FloatingSine.h"


static const float RADIUS = 30.0f;


// pentatonic
static const int	SCALE_STEPS_PENTATONIC = 5;
static const float	SCALE_PENTATONIC[SCALE_STEPS_PENTATONIC] = { 0, 2, 5, 7, 10 };

// lydian Tone, tone, semitone, tone, tone, tone, semitone
static const int	SCALE_STEPS_LYDIAN = 8;
static const float	SCALE_LYDIAN[SCALE_STEPS_LYDIAN] = { 0, 2, 3, 5, 7, 9, 10 };

static const float BASE_MIDI_NOTE = 62.0f;


float midiToFrequency( float midiNote )
{
	return 440.0f*powf(2.0f,(midiNote-60.0f)/12.0f);
}

void FloatingSine::setup( ofSoundMixer* mixer, vector<FloatingSine*> * n, ofImage* particleImage )
{
	neighbours = n;
	particle = particleImage;

	setScale( 0 );
	setBaseMidiNote( BASE_MIDI_NOTE );

	// setup the sound
	//
	// we have a chain like this:
	// [tone generator]---->[volume control]------>[mixer]
	//
	// the mixer is shared amongst all objects
	//
	int whichScaleNote = ofRandom( 0, 0.99999f*scaleSteps );
	float midiNote = baseMidiNote + scale[whichScaleNote];
	frequency = midiToFrequency( midiNote );
	octaveOffset = ofRandom( -2.99f, 0.99f );
	tone.setFrequency( frequency );
	// tone goes through volume control
	volume.addInputFrom( &tone );
	volume.setVolume( 1.0f/(n->size()) );
	// we add the volume control to the mixer
	mixer->addInputFrom( &volume );
	
	
	
	sawtooth = false;
	bBackgroundAuto = false;
	velocity = 0;

	// colours for sine and sawtooth waveforms
	sineColour.setHsb( 255.0f*(ofRandom( -10, 10 ) + 221.0f)/360.0f, 
					   255.0f*(ofRandom( -0.1f, 0.1f ) + 0.79), 
					   255.0f*(ofRandom( -0.1f, 0.1f ) + 0.66f), 200.0f );
	sawColour.setHsb(  255.0f*(ofRandom( -10, 10 ) + 33.0f)/360.0f, 
					   255.0f*(ofRandom( -0.1f, 0.1f ) + 0.79), 
					   255.0f*(ofRandom( -0.1f, 0.1f ) + 0.66f), 200.0f );
	
	// setup motion behaviour
	// each dot has a 'buddy' that it tries to maintain a fixed multiple of shellDistance away from
	// each dot also has an 'enemy' that it tries to run away from
	shellDistance = ofRandom( 100.0f, 200.0f );
	position.set( ofRandom( 3.0f*ofGetWidth()/8, 5.0f*ofGetWidth()/8 ), ofRandom( 3.0f*ofGetHeight()/8, 5.0f*ofGetHeight()/8 ) );
	if ( neighbours->size() >= 3 )
	{
		buddy = ofRandomuf()*0.99999f*neighbours->size();
		// avoid selecting ourselves as the buddy
		while ( n->at( buddy ) == this )
		{
			buddy = (buddy+1)%neighbours->size();
		}
		enemy = ofRandomuf()*0.99999f*neighbours->size();
		// avoid selecting ourselves as the buddy
		while ( n->at( enemy ) == this || buddy == enemy )
		{
			enemy = (enemy+1)%neighbours->size();
		}
	}
	else 
	{
		buddy = 0; 
		enemy = 0;
	}
	
}


void FloatingSine::setScale( int which )
{
	which = min(max(0,which),1);
	if ( which == 0 )
	{
		scale = SCALE_PENTATONIC;
		scaleSteps = SCALE_STEPS_PENTATONIC;
	}
	else {
		scale = SCALE_LYDIAN;
		scaleSteps = SCALE_STEPS_LYDIAN;
	}
}


void FloatingSine::toggleWaveform() { 
	setWaveform ( !sawtooth );
}

void FloatingSine::setWaveform( bool toSawtooth ) {
	sawtooth = toSawtooth;
	
	if ( sawtooth ) { 
		tone.setSawtoothWaveform(); 
	} else { 
		tone.setSineWaveform(); 
	} 
}

void FloatingSine::update( )
{
	// update the position
	position += velocity*ofGetLastFrameTime();
	
	// damp the velocity
	velocity *= powf(0.99f,ofGetLastFrameTime());

	ofVec2f delta = neighbours->at( buddy )->position - position;
	float distance = delta.length();
	ofVec2f deltaNorm = delta/distance;
	
	distanceUnits = distance/shellDistance;
	
	// calculate distance in terms of shellDistance
	int whichScaleNote = distanceUnits;
	float remainder = distanceUnits - whichScaleNote;
	if ( remainder > 0.5f ) {
		// round
		distanceUnits += 1.0f;
		whichScaleNote += 1.0f;
		remainder = 1.0f-remainder;
	}

	// update frequency
	float midiNote = baseMidiNote;
	while ( whichScaleNote>=scaleSteps ) {
		midiNote += 12; 
		whichScaleNote -= scaleSteps;
	}

	// convert scale steps to actual scale notes
	float scaleNote = scale[whichScaleNote%scaleSteps];
	float nextScaleNote = scale[(whichScaleNote+1)%scaleSteps];
	float prevScaleNote = scale[(whichScaleNote+4)%scaleSteps];
	if ( prevScaleNote>nextScaleNote) {
		// wrap
		nextScaleNote += 12.0f;
	} 

	// set the actual frequency, adding on a detune calculated from remainder
	tone.setFrequency( midiToFrequency( float(octaveOffset*12) + midiNote + scaleNote + remainder*0.1f /* (remainder>0.0f?1.0f:-1.0f)*remainder*remainder*/ ) ); 
	
	// set volume
	float vol = 1.0f-min(1.0f,(distance/ofGetWidth()));
	volume.setVolume( 1.0f*vol/(neighbours->size()) );
	
	// calculate forces
	static const float BUDDY_FORCE_MUL = 1000.0f;
	static const float ENEMY_FORCE_MUL = 100.0f;
	static const float CENTRE_FORCE_MUL = 0.3f;

	// move towards the nearest shell out from our buddy
	ofVec2f dv = ((remainder>0.0f)&&(distanceUnits>1.0f)>0.0f?-1.0f:1.0f)*
		remainder*remainder*deltaNorm*ofGetLastFrameTime()*BUDDY_FORCE_MUL;
	velocity += dv*0.5f;
	neighbours->at(buddy)->velocity -= dv*0.5f;
	
	
	// run away from enemy
	ofVec2f enemyDelta = neighbours->at( enemy )->position - position;
	float enemyDistance = enemyDelta.length();
	ofVec2f enemyDeltaNorm = enemyDelta/enemyDistance;
	dv = enemyDeltaNorm*(1.0f/enemyDistance)*ofGetLastFrameTime()*ENEMY_FORCE_MUL;
	velocity -= dv*0.5f;
	neighbours->at( enemy )->velocity += dv*0.5f;
	
	
	// don't get too far away from the centre of the screen
	ofVec2f centreDelta = ofVec2f(ofGetWidth()/2,ofGetHeight()/2)-position;
	velocity += centreDelta*ofGetLastFrameTime()*CENTRE_FORCE_MUL;
}


void FloatingSine::draw(){
	float vol = volume.getVolume();
	float volSq = vol*vol;
	float alpha = fabsf(2.0f*(distanceUnits-int(distanceUnits)-0.5f));
	// select colour based on waveform
	ofColor colour = sawtooth?sawColour:sineColour;
	
	// draw the particle
	float particleAlpha = (bBackgroundAuto?1.0f:0.2f)*(128+128.0f*alpha);
	ofSetColor( colour.r, colour.g, colour.b, particleAlpha );
	ofFill();
	particle->setAnchorPercent(0.5,0.5);
	ofCircle( position.x, position.y, RADIUS*(0.4f+0.6f*alpha*volume.getVolume()*neighbours->size()) );
	
	// draw the connecting lines and shell lines
	float lineAlpha = (bBackgroundAuto?1.0f:0.2f)*(255.0f*alpha);
	ofSetColor( colour.r, colour.g, colour.b, lineAlpha );
	ofNoFill();
	ofVec2f buddyPosition = neighbours->at(buddy)->position;
	ofLine( position.x, position.y, buddyPosition.x, buddyPosition.y );
	ofCircle( buddyPosition.x, buddyPosition.y, distanceUnits*shellDistance );
}


void FloatingSine::setBackgroundAuto( bool bAuto )
{
	bBackgroundAuto = bAuto;
}

