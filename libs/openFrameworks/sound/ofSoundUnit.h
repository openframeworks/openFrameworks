/*
 *  ofSoundUnit.h
 *  ofSound
 *
 *  Created by damian on 10/01/11.
 *  Copyright 2011 frey damian@frey.co.nz. All rights reserved.
 *
 */

#pragma once

#include "ofSoundStream.h"
#include "ofTypes.h"
#include "ofUtils.h"
#include <vector>
using namespace std;

class ofSoundSource;


/** ofSoundUnit
 
 Base class for sound units that can be strung together into a DSP-style chain.
 
 @author damian
 */

class ofSoundUnit
{
public:	
	
	/// Return the name of this synth unit.
	virtual string getName() = 0;

	/// Return our inputs in a vector (but at ofSoundUnit level we have no inputs).
	virtual vector<ofSoundSource*> getInputs()  { return vector<ofSoundSource*>(); }
	
protected:
	
	
};




/** ofSoundSource
 
 Base class for ofSoundUnits that generate some kind of sound output.
 
 @author damian
 */


class ofSoundSource: public virtual ofSoundUnit
{
public:

	/// Set the sample rate. If you need to know about sample rate changes, override this function.
	virtual void setSampleRate( int rate ) {};
	
	/// Fill buffer with (numFrames*numChannels) floats of data, interleaved
	virtual void audioRequested( float* buffer, int numFrames, int numChannels ) = 0;
	
protected:
	
	
};



/** ofSoundBuffer
 
 Wrapper for an interleaved floating point buffer holding a fixed number of frames
 of a fixed number of channels of audio.
 
 @author damian
 */

class ofSoundBuffer
{
public:
	
	float* buffer;
	int numFrames;
	int numChannels;
	
	ofSoundBuffer() { numFrames = 0; numChannels = 0; buffer = NULL; }
	ofSoundBuffer( const ofSoundBuffer& other ) { copyFrom( other ); }
	ofSoundBuffer( int nFrames, int nChannels ) { numFrames = nFrames; numChannels = nChannels; buffer = new float[nFrames*nChannels]; }
	ofSoundBuffer& operator=( const ofSoundBuffer& other ) { copyFrom( other ); return *this; }
	
	~ofSoundBuffer() { if ( buffer ) delete[] buffer; }

	void copyFrom( const ofSoundBuffer& other ) { 
		numFrames = 0; numChannels = 0;
		if ( other.buffer ) {
			allocate( other.numFrames, other.numChannels );
			memcpy( buffer, other.buffer, numFrames*numChannels*sizeof(float) );
		} 
		else {
			buffer = NULL;
		}
	}
	
	/// Set audio data to 0.
	void clear() { if ( buffer ) memset( buffer, 0, sizeof(float)*numFrames*numChannels); }
	
	/// Allocate the buffer to the given size if necessary.
	void allocate( int nFrames, int nChannels ) { 
		if ( !buffer || numFrames != nFrames || numChannels != nChannels ) {
			numFrames = nFrames; numChannels = nChannels; 
			if ( buffer ) {
				delete[] buffer;
			}
			buffer = new float[numFrames*numChannels];
		}
	}
	
	/// Copy just a single channel to output. output should have space for numFrames floats.
	void copyChannel( int channel, float* output ) const {
		if ( buffer && channel < numChannels ) {
			for ( int i=0; i<numFrames; i++ ) {
				output[i] = buffer[numChannels*i + channel];
			}
		}
	}
	
	/// Copy safely to out. Copy as many frames as possible, repeat channels as necessary.
	void copyTo( float* outBuffer, int outNumFrames, int outNumChannels ) {
		if ( outNumFrames>numFrames ) {
			ofLog( OF_LOG_WARNING, "ofSoundBuffer::copyTo: %i frames requested but only %i are available", outNumFrames, numFrames );
		}
		if ( outNumChannels>numChannels ) {
			ofLog( OF_LOG_WARNING, "ofSoundBuffer::copyTo: %i channels requested but only %i are available, looping to make up the difference", outNumChannels, numChannels );
		}
		for ( int i=0; i<min(numFrames,outNumFrames); i++ ) {
			for ( int j=0; j<outNumChannels; j++ ) {
				// copy input to output; in cases where input has fewer channels than output, loop through input frames repeatedly
				outBuffer[i*outNumChannels+j] = buffer[i*numChannels+(j%numChannels)];
			}
		}		
	}
};





/** ofSoundSink
 
 Base class for ofSoundUnits that receive audio data, either by pulling from upstream 
 inputs, or from the audioReceived function which is called from outside in special 
 cases (eg when used as microphone/line input).
 
 @author damian
 */

class ofSoundSink: public virtual ofSoundUnit
{
public:
	ofSoundSink() { input = NULL; sampleRate = 44100; }
	
	/// Set the sample rate of this synth unit. If you overload this remember to call the base class.
	virtual void setSampleRate( int rate );

	/// Add an input to this unit from the given source unit. If overloading remember to call base.
	virtual bool addInputFrom( ofSoundSource* source );
	
	/// Return our inputs in a vector.
	virtual vector<ofSoundSource*> getInputs();
	
	/// Receive incoming audio from elsewhere (eg coming from a microphone or line input source)
	virtual void audioReceived( float* buffer, int numFrames, int numChannels );
	
protected:
	// fill our input buffer from the upstream source (input)
	void fillInputBufferFromUpstream( int numFrames, int numChannels );
	
	// walk the DSP graph and see if adding test_input as an input to ourselves; return true if it would.
	bool addingInputWouldCreateCycle( ofSoundSource* test_input );

	
	void lock() { mutex.lock(); }
	void unlock() { mutex.unlock(); }	
	ofMutex mutex;
		
	
	
	ofSoundSource* input;
	ofSoundBuffer inputBuffer;

	int sampleRate;
};




/** ofSoundMixer
 
 Mixes together inputs from multiple ofSoundSources.
 
 @author damian
 */

class ofSoundMixer : public ofSoundSink, public ofSoundSource
{
public:	
	string getName() { return "ofSoundMixer"; }
	
	/// Set the volume of the mixer input for the given unit to vol.
	void setVolume( ofSoundSource* input, float vol );
	
	/// Add an input to this unit from the given source unit.
	bool addInputFrom( ofSoundSource* source );
	
	/// render
	void audioRequested( float* buffer, int numFrames, int numChannels );
	
	/// return all our inputs
	vector<ofSoundSource*> getInputs();
	
	/// pass sample rate changes on to inputs
	void setSampleRate( int rate );
	
private:
	
	
	// Inputs into the mixer, with volume and pan
	struct MixerInput
	{
		ofSoundSource* input;
		ofSoundBuffer inputBuffer;
		float volume;
		float pan;
		MixerInput( ofSoundSource* i, float v, float p )
		{
			input = i;
			volume = v; 
			pan = p;
		}
	};
	vector<MixerInput> inputs;
	
};




/** ofSoundDeclickedFloat
 
 Declick a changing float value by using a 64 sample ramp (around 1.5ms at 44.1kHz).
 
 You must call rebuildRampIfNecessary before processing every audio block, in order to apply incoming
 value changes.
 
 Also, you must call frameTick() for every audio frame (sample) to advance the ramp.
 
 @author damian
 */

class ofSoundDeclickedFloat
{
public:
	ofSoundDeclickedFloat( float startValue=0.0f ) { rampPos = 0; current = startValue; setValue( startValue ); }
	
	/// Return the current value, declicked
	float getDeclickedValue() { return current; }
	/// Return the raw value (the target for declicking)
	float getRawValue() { return target; }
	/// Set a new value + rebuild ramp
	void setValue( float newValue ) { target = newValue; rampNeedsRebuild = true; }
	
	/// Rebuild the ramp, if necessary. Call before processing a block of samples.
	void rebuildRampIfNecessary() { if ( rampNeedsRebuild ) rebuildRamp(); rampNeedsRebuild = false; }
	
	/// Update, to be called for every frame
	void frameTick() { current = ramp[rampPos]; ramp[rampPos] = target; rampPos = (rampPos+1)%64; }
	
	/// operator overloading
	ofSoundDeclickedFloat& operator=( float new_value ) { setValue( new_value ); return *this; }
	ofSoundDeclickedFloat& operator+=( float adjustment ) { setValue( target+adjustment ); return *this; }
	ofSoundDeclickedFloat& operator-=( float adjustment ) { setValue( target-adjustment ); return *this; }
	
private:
	
	void rebuildRamp() { float v = current; float step = (target-current)/63; for ( int i=0; i<64; i++ ) { ramp[(i+rampPos)%64] = v; v += step; } }
	
	float current;
	float target;
	
	bool rampNeedsRebuild;
	int rampPos;
	float ramp[64];
};


/** ofSoundSourceTestTone
 
 An ofSoundSource that generates a test tone with a given frequency.
 
 @author damian
 */

class ofSoundSourceTestTone: public ofSoundSource
{
public:	
	static const int TESTTONE_SINE = 0;
	static const int TESTTONE_SAWTOOTH = 1;
	
	ofSoundSourceTestTone() { sampleRate = 44100; phase = 0; sawPhase = 0; setFrequency( 440.0f ); waveform = TESTTONE_SINE; }
	
	/// Return our name
	string getName() { return "ofSoundUnitTestTone"; }
	
	/// Set our frequency
	void setFrequency( float freq );
	/// Set our frequency using a midi note
	void setFrequencyMidiNote( float midiNote ) { setFrequency(440.0f*powf(2.0f,(midiNote-60.0f)/12.0f)); };
	/// Update generation for the new sample rate
	void setSampleRate( int rate );
	/// Set waveform
	void setSineWaveform() { waveform = TESTTONE_SINE; }
	void setSawtoothWaveform() { waveform = TESTTONE_SAWTOOTH; }
	
	void audioRequested( float* output, int numFrames, int numChannels );
	
private:
	
	float phase;
	float frequency;
	int sampleRate;
	float phaseAdvancePerFrame;
	
	int waveform;
	
	float sawPhase;
	float sawAdvancePerFrame;
	
};


/** ofSoundSinkMicrophone
 
 An ofSoundSink object that catches microphone input + passes it downstream, taking care of upmixing to higher
 output channel counts by repeating input channels as necessary.
 
 Automatically registers itself with the ofSoundStream system using ofSoundStreamAddSoundSink( this ).
 
 @author damian
 */

class ofSoundSinkMicrophone: public ofSoundSink, public ofSoundSource
{
public:
	ofSoundSinkMicrophone() { ofSoundStreamAddSoundSink( this ); }
	virtual ~ofSoundSinkMicrophone() { ofSoundStreamRemoveSoundSink( this ); }
	
	string getName() { return "ofSoundSinkMicrophone"; }
	
	/// return last received microphone input
	void audioRequested( float* output, int numFrames, int numChannels );
	
	
	
private:
	
	
};


/** ofSoundSourceMultiplexor
 
 An ofSoundSource that allows an input ofSoundSource to be used an input to multiple downstream units.

 @author damian
 */

static const string OF_SOUND_SOURCE_MULTIPLEXOR_NAME = "ofSoundSourceMultiplexor";

class ofSoundSourceMultiplexor: public ofSoundSink, public ofSoundSource
{
public:
	ofSoundSourceMultiplexor() { lastRenderedTick=0; }
	
	string getName() { return OF_SOUND_SOURCE_MULTIPLEXOR_NAME; }
	
	/// cache input and share to downstream
	void audioRequested( float* output, int numFrames, int numChannels );
	
	/// pass through sample rate to upstream
	void setSampleRate( int rate );
	
private:
	
	long unsigned long lastRenderedTick;
	
};
