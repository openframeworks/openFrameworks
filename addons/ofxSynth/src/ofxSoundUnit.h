/*
 * ofSoundUnit.h
 * ofSound
 *
 * Created by damian on 10/01/11.
 * Copyright 2011 frey damian@frey.co.nz. All rights reserved.
 *
 */

#pragma once

#include "ofSoundStream.h"
#include "ofTypes.h"
#include "ofUtils.h"
#include "ofConstants.h"
#include <vector>

class ofxSoundSource;


/** ofSoundUnit
Base class for sound units that can be strung together into a DSP-style chain.
@author damian
 */

class ofxSoundUnit
{
public:
	virtual ~ofxSoundUnit() {};

	/// Return the name of this synth unit.
	virtual string getName() = 0;

	/// Return our inputs in a vector (but at ofSoundUnit level we have no inputs).
	virtual vector<ofxSoundSource*> getInputs() { return vector<ofxSoundSource*>(); }

protected:


};




/** ofSoundSource
Base class for ofSoundUnits that generate some kind of sound output.
@author damian
 */


class ofxSoundSource: public virtual ofxSoundUnit, public ofBaseSoundOutput
{
public:
	virtual ~ofxSoundSource() {};

	/// Set the sample rate. If you need to know about sample rate changes, override this function.
	virtual void setSampleRate( int rate ) {};

	/// Fill buffer with (numFrames*numChannels) floats of data, interleaved
	virtual void audioOut( float* buffer, int numFrames, int numChannels, long unsigned long  tickCount ) = 0;

protected:


};



/** ofSoundBuffer
Wrapper for an interleaved floating point buffer holding a fixed number of frames
of a fixed number of channels of audio.
@author damian
 */

class ofxSoundBuffer
{
public:

	float* buffer;
	int numFrames;
	int numChannels;

	ofxSoundBuffer();
	ofxSoundBuffer( const ofxSoundBuffer& other );
	ofxSoundBuffer( int nFrames, int nChannels );
	ofxSoundBuffer& operator=( const ofxSoundBuffer& other );

	~ofxSoundBuffer();
	void copyFrom( const ofxSoundBuffer& other );
	/// Set audio data to 0.
	void clear();

	/// Allocate the buffer to the given size if necessary.
	void allocate( int nFrames, int nChannels );

	/// Copy just a single channel to output. output should have space for numFrames floats.
	void copyChannel( int channel, float* output ) const ;

	/// Copy safely to out. Copy as many frames as possible, repeat channels as necessary.
	void copyTo( float* outBuffer, int outNumFrames, int outNumChannels );


};


/** ofSoundSink
Base class for ofSoundUnits that receive audio data, either by pulling from upstream
inputs, or from the audioReceived function which is called from outside in special
cases (eg when used as microphone/line input).
@author damian
 */

class ofxSoundSink: public virtual ofxSoundUnit, public ofBaseSoundInput
{
public:
	ofxSoundSink() { input = NULL; sampleRate = 44100; }
	virtual ~ofxSoundSink() {};

	/// Set the sample rate of this synth unit. If you overload this remember to call the base class.
	virtual void setSampleRate( int rate );

	/// Add an input to this unit from the given source unit. If overloading remember to call base.
	virtual bool addInputFrom( ofxSoundSource* source );

	/// Return our inputs in a vector.
	virtual vector<ofxSoundSource*> getInputs();

	/// Receive incoming audio from elsewhere (eg coming from a microphone or line input source)
	virtual void audioIn( float* buffer, int numFrames, int numChannels, long unsigned long  tickCount );

protected:
	// fill our input buffer from the upstream source (input)
	void fillInputBufferFromUpstream( int numFrames, int numChannels,long unsigned long tickCount   );

	// walk the DSP graph and see if adding test_input as an input to ourselves; return true if it would.
	bool addingInputWouldCreateCycle( ofxSoundSource* test_input );


	void lock() { mutex.lock(); }
	void unlock() { mutex.unlock(); }
	ofMutex mutex;



	ofxSoundSource* input;
	ofxSoundBuffer inputBuffer;

	int sampleRate;
};




/** ofSoundMixer
Mixes together inputs from multiple ofSoundSources.
@author damian
 */

class ofxSoundMixer : public ofxSoundSink, public ofxSoundSource
{
public:
	ofxSoundMixer() { working = NULL; masterVolume = 1.0f; source = NULL; }
	ofxSoundMixer( const ofxSoundMixer& other ) { copyFrom( other ); }
	ofxSoundMixer& operator=( const ofxSoundMixer& other ) { copyFrom( other ); return *this; }
	~ofxSoundMixer();

	string getName() { return "ofSoundMixer"; }

	/// Set the master out volume of this mixer
	void setMasterVolume( float vol ) { if ( !isnan(vol) && isfinite(vol) ) { masterVolume = vol; } }
	/// Set the volume of the mixer input for the given unit to vol.
	void setVolume( ofxSoundSource* input, float vol );
	/// Set the pan of the mixer input for the given unit to pan
	void setPan( ofxSoundSource* input, float pan );

	/// Add an input to this unit from the given source unit.
	bool addInputFrom( ofxSoundSource* source );

	/// Add an input to this unit from the given source unit.
	void setAudioInput( ofBaseSoundOutput* _source ){
		source = _source;
	}

	/// render
	void audioOut( float* buffer, int numFrames, int numChannels,long unsigned long tickCount );

	/// return all our inputs
	vector<ofxSoundSource*> getInputs();

	/// pass sample rate changes on to inputs
	void setSampleRate( int rate );

private:
	// safely copy from the other mixer (probably a bad idea though)
	void copyFrom( const ofxSoundMixer& other );

	// Inputs into the mixer, with volume and pan
	struct MixerInput
	{
		ofxSoundSource* input;
		ofxSoundBuffer inputBuffer;
		float volume;
		float pan;
		MixerInput( ofxSoundSource* i, float v, float p )
		{
			input = i;
			volume = v;
			pan = p;
		}
	};
	vector<MixerInput*> inputs;

	float masterVolume;

	float* working;

	ofBaseSoundOutput* source;
};




/** ofSoundDeclickedFloat
Declick a changing float value by using a 64 sample ramp (around 1.5ms at 44.1kHz).
You must call rebuildRampIfNecessary before processing every audio block, in order to apply incoming
value changes.
Also, you must call frameTick() for every audio frame (sample) to advance the ramp.
@author damian
 */

class ofxSoundDeclickedFloat
{
public:
	ofxSoundDeclickedFloat( float startValue=0.0f ) { rampPos = 0; current = startValue; setValue( startValue ); }

	/// Return the current value, declicked
	float getDeclickedValue() { return current; }
	/// Return the raw value (the target for declicking)
	float getRawValue() { return target; }
	/// Set a new value + rebuild ramp
	void setValue( float newValue ) { if ( !isnan(newValue)&&finite(newValue) ) { target = newValue; rampNeedsRebuild = true; } }

	/// Rebuild the ramp, if necessary. Call before processing a block of samples.
	void rebuildRampIfNecessary() { if ( rampNeedsRebuild ) rebuildRamp(); rampNeedsRebuild = false; }

	/// Update, to be called for every frame
	void frameTick() { current = ramp[rampPos]; ramp[rampPos] = target; rampPos = (rampPos+1)%64; }

	/// operator overloading
	ofxSoundDeclickedFloat& operator=( float new_value ) { setValue( new_value ); return *this; }
	ofxSoundDeclickedFloat& operator+=( float adjustment ) { setValue( target+adjustment ); return *this; }
	ofxSoundDeclickedFloat& operator-=( float adjustment ) { setValue( target-adjustment ); return *this; }

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

class ofxSoundSourceTestTone: public ofxSoundSource
{
public:
	static const int TESTTONE_SINE = 0;
	static const int TESTTONE_SAWTOOTH = 1;

	ofxSoundSourceTestTone() { sampleRate = 44100; phase = 0; sawPhase = 0; setFrequency( 440.0f ); waveform = TESTTONE_SINE; }

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

	void audioOut( float* output, int numFrames, int numChannels,long unsigned long tickCount );

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


class ofxSoundSinkMicrophone: public ofxSoundSink, public ofxSoundSource
{
public:
	ofxSoundSinkMicrophone() {  }
	virtual ~ofxSoundSinkMicrophone() {  }

	string getName() { return "ofSoundSinkMicrophone"; }

	/// return last received microphone input
	void audioOut( float* output, int numFrames, int numChannels,long unsigned long tickCount   );



private:


};


/** ofSoundSourceMultiplexor
An ofSoundSource that allows an input ofSoundSource to be used an input to multiple downstream units.

@author damian
 */

static const char* OF_SOUND_SOURCE_MULTIPLEXOR_NAME = "ofSoundSourceMultiplexor";

class ofxSoundSourceMultiplexor: public ofxSoundSink, public ofxSoundSource
{
public:
	ofxSoundSourceMultiplexor() { lastRenderedTick=0; }

	string getName() { return OF_SOUND_SOURCE_MULTIPLEXOR_NAME; }

	/// cache input and share to downstream
	void audioOut( float* output, int numFrames, int numChannels,long unsigned long tickCount );

	/// pass through sample rate to upstream
	void setSampleRate( int rate );

private:

	long unsigned long lastRenderedTick;

};
