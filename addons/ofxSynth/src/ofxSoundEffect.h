/*
 *  ofSound.h
 *  openFrameworksLib
 *
 *  Created by damian on 12/01/11.
 *  Copyright 2011 frey damian@frey.co.nz. All rights reserved.
 *
 */

#pragma once

#include "ofxSoundUnit.h"
#include <vector>
#include <deque>
using namespace std;


/** ofSoundEffect

 Base class for a sound effect, which takes input from an upstream ofSoundSource and processes it.

 Subclasses implement the process() method to perform processing.

 @author damian
 */

class ofxSoundEffect: public ofxSoundSource, public ofxSoundSink
{
public:

	/// Pass sample rate changes through; if your override this remember to call base.
	void setSampleRate( int rate );

	/// Override this function to do your own processing. Be sure to match numInChannels and numOutChannels
	virtual void process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels ) = 0;


	/// implementation of the generate function from ofSoundSource
	void audioOut( float* buffer, int numFrames, int numChannels,long unsigned long tickCount  );

};




/** ofSoundEffectPassthrough

 An ofSoundEffect that simply passes audio through, allowing access to the ofSoundBuffer of audio
 passing through it.

 @author damian
 */

class ofxSoundEffectPassthrough: public ofxSoundEffect
{
	ofxSoundBuffer inputBuffer;
public:
	string getName() { return "ofSoundEffectPassthrough"; }

	virtual void process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels );

	/// return the buffer we saw as we passed it through
	ofxSoundBuffer& getBuffer() { return inputBuffer; }
};

/** ofSoundEffectVolume

 An ofSoundEffect that applies a volume (gain) to audio samples passing through it.

 @author damian


class ofSoundEffectVolume : public ofSoundEffect
{
public:
	ofSoundEffectVolume() { setVolume( 1.0f ); }

	/// Return our name
	string getName() { return "ofSoundEffectVolume"; }

	/// Set our volume to vol
	void setVolume( float vol ) { 	if ( !isnan(vol) && isfinite(vol) ) { volume = vol; } }
	/// Adjust our current volume by adjustment
	void adjustVolume( float adjustment ) { if ( !isnan(adjustment) && isfinite(adjustment) ) { volume += adjustment; } }
	/// Get current volume (not declicked)
	float getVolume() { return volume.getRawValue(); }

	void process( float* input, float* output, int numFrames, int numInChannels, int numOutChannels );

private:
	ofSoundDeclickedFloat volume;


};*/





