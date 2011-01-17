/*
 *  ofSoundUnit.cpp
 *  ofSound
 *
 *  Created by damian on 10/01/11.
 *  Copyright 2011 frey damian@frey.co.nz. All rights reserved.
 *
 */

#include "ofSoundUnit.h"
#include "ofUtils.h"

#include <deque>
using namespace std;

/*
vector<ofSoundSource*> ofSoundUnit::getInputs()
{
	ofLog(OF_LOG_WARNING, "ofSoundUnit::getInputs() called");
	// no inputs
	return vector<ofSoundSource*>();
}*/



void ofSoundSink::setSampleRate( int rate )
{

	vector<ofSoundSource*> inputs = getInputs();
	for ( int i=0; i<(int)inputs.size(); i++ )
	{ 
		inputs[i]->setSampleRate( rate ); 
	}
	sampleRate = rate;
}

bool ofSoundSink::addInputFrom( ofSoundSource* from ) 
{
	// check for existing input
	if ( input != NULL )
	{
		ofLog( OF_LOG_ERROR, "ofSoundUnit: can't connect '%s' (%x) to '%s' (%x): it already has an input",
			  from->getName().c_str(), from, this->getName().c_str(), this );
		return false;
	}
	// check for branches/cycles
	if ( addingInputWouldCreateCycle( from ) )
	{
		ofLog( OF_LOG_ERROR, "ofSoundUnit: can't connect '%s' (%x) to '%s' (%x): this would create a cycle in the DSP graph",
			  from->getName().c_str(), from, this->getName().c_str(), this );
		return false;
	}

	input = from;
	from->setSampleRate( sampleRate );
	return true;
}

vector<ofSoundSource*> ofSoundSink::getInputs()
{
	lock();
	vector<ofSoundSource*> result;
	if ( input )
	{
		result.push_back( input );
	}
	unlock();
	return result;
	
}

/// Receive incoming audio from elsewhere (eg coming from a microphone or line input source)
void ofSoundSink::audioReceived( float* buffer, int numFrames, int numChannels )
{
	lock();
	inputBuffer.allocate( numFrames, numChannels );
	memcpy( inputBuffer.buffer, buffer, numFrames*numChannels*sizeof(float) );
	unlock();
}


void ofSoundSink::fillInputBufferFromUpstream( int numFrames, int numChannels )
{
	// fetch data from input, and render via process() function
	// create/recreate input buffer if necessary
	if ( inputBuffer.numFrames != numFrames || inputBuffer.numChannels != numChannels )
	{
		inputBuffer.allocate( numFrames, numChannels );
	}
	
	// call input's generate
	if ( !input )
	{
		inputBuffer.clear();
	}
	else
	{
		input->audioRequested( inputBuffer.buffer, inputBuffer.numFrames, inputBuffer.numChannels );
	}
	
}


// Return true if adding this edge to the graph would create a cycle
bool ofSoundSink::addingInputWouldCreateCycle( ofSoundSource* test )
{
	// assuming the graph has no cycles from the start, can we already trace a path 
	// from test_input to ourselves? if we can, then adding test_input will create 
	// a cycle.
	
	// do a depth-first traversal
	deque<ofSoundUnit*> stack;
	stack.push_back( test );
	while ( !stack.empty() )
	{
		ofSoundUnit* u = stack.back();
		stack.pop_back();
		// if u is this, then we have looped round to the beginning
		if ( u == this ) {
			return true;
		}
		
		// fetch all immediate upstream neighbours
		vector<ofSoundSource*> upstream = u->getInputs();
		// copy downstream to the back of stack
		copy( upstream.begin(), upstream.end(), back_inserter( stack ) );
	}
	
	
	// if we made it here, the cycle test has failed to find a cycle
	return false;
}





ofSoundBuffer::ofSoundBuffer() { 
	numFrames = 0; 
	numChannels = 0; 
	buffer = NULL; 
}
ofSoundBuffer::ofSoundBuffer( const ofSoundBuffer& other ) { 
	copyFrom( other ); 
}
ofSoundBuffer::ofSoundBuffer( int nFrames, int nChannels ) { 
	numFrames = nFrames; 
	numChannels = nChannels; 
	buffer = new float[nFrames*nChannels]; 
}
ofSoundBuffer& ofSoundBuffer::operator=( const ofSoundBuffer& other ) { 
	copyFrom( other ); 
	return *this; 
}

ofSoundBuffer::~ofSoundBuffer() { 
	if ( buffer ) {
		delete[] buffer; 
	}
}

void ofSoundBuffer::copyFrom( const ofSoundBuffer& other ) { 
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
void ofSoundBuffer::clear() { 
	if ( buffer ) { 
		memset( buffer, 0, sizeof(float)*numFrames*numChannels); 
	}
}

/// Allocate the buffer to the given size if necessary.
void ofSoundBuffer::allocate( int nFrames, int nChannels ) { 
	if ( !buffer || numFrames != nFrames || numChannels != nChannels ) {
		numFrames = nFrames; numChannels = nChannels; 
		if ( buffer ) {
			delete[] buffer;
		}
		buffer = new float[numFrames*numChannels];
	}
}

/// Copy just a single channel to output. output should have space for numFrames floats.
void ofSoundBuffer::copyChannel( int channel, float* output ) const {
	if ( buffer && channel < numChannels ) {
		for ( int i=0; i<numFrames; i++ ) {
			output[i] = buffer[numChannels*i + channel];
		}
	}
}

/// Copy safely to out. Copy as many frames as possible, repeat channels as necessary.
void ofSoundBuffer::copyTo( float* outBuffer, int outNumFrames, int outNumChannels ) {
	if ( numFrames == 0 || numChannels == 0 ) {
		ofLog( OF_LOG_WARNING, "ofSoundBuffer::copyTo: copy requested on empty buffer, returning nothing (check your addInputTo() methods!)" );
		memset( outBuffer, 0, sizeof(float)*outNumFrames*outNumChannels );
	}
	if ( outNumFrames>numFrames ) {
		ofLog( OF_LOG_WARNING, "ofSoundBuffer::copyTo: %i frames requested but only %i	are available", outNumFrames, numFrames );
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





ofSoundMixer::~ofSoundMixer() { 
	if ( working ) 
		delete[] working;  	
	for ( int i =0; i<(int)inputs.size(); i++ ) {
		delete inputs[i];
	}
}

void ofSoundMixer::copyFrom( const ofSoundMixer& other ) {
	ofLog( OF_LOG_WARNING, "Copying ofSoundMixer, this will probably make things break" );
	working = NULL;
	for ( int i=0; i<(int)other.inputs.size(); i++ ) {
		addInputFrom( other.inputs[i]->input );
		setVolume( other.inputs[i]->input, other.inputs[i]->volume );
		setPan( other.inputs[i]->input, other.inputs[i]->pan );
	}
	masterVolume = other.masterVolume;
}

void ofSoundMixer::setVolume( ofSoundSource* input, float vol ) {
	if ( isnan(vol) || !isfinite(vol) ) {
		return;
	}
	for ( int i=0; i<(int)inputs.size(); i++ ) {
		if ( inputs[i]->input == input ) {
			inputs[i]->volume = vol;
		}
	}
}

void ofSoundMixer::setPan( ofSoundSource* input, float pan ) {
	if ( isnan(pan) || !isfinite(pan) ) {
		return;
	}
	pan = min(1.0f,max(0.0f,pan));
	bool found = false;
	for ( int i=0; i<(int)inputs.size(); i++ ) {
		if ( inputs[i]->input == input ) {
			found = true;
			inputs[i]->pan = pan;
		}
	}
	if ( !found ) { 
		ofLog( OF_LOG_WARNING, "ofSoundMixer: setPan couldn't find input for '%s' (%x), not setting pan", input->getName(), input );
		
	}
}




// Render the DSP chain. output is interleaved and has space for 
// numFrames*numChannels floats.
void ofSoundMixer::audioRequested( float* output, int numFrames, int numChannels )
{
	// advance DSP
	if ( numChannels != 1 && numChannels != 2 ) {
		ofLog( OF_LOG_ERROR, "only 1 or 2 channels supported");
		return;
	}
	// write to output
	// clear output array
	memset( output, 0, numFrames*numChannels*sizeof(float) );
	
	mutex.lock();
	
	// allocate working space
	if ( !working ) {
		working = new float[numFrames*numChannels];
	}
	vector<float> volumePerChannel;
	volumePerChannel.resize( numChannels );
	for ( int i=0; i<(int)inputs.size(); i++ ) {
		// clear working
		memset( working, 0, numFrames*numChannels*sizeof(float) );
		
		// render input into working
		inputs[i]->input->audioRequested( working, numFrames, numChannels );
		
		// construct precalculated volume + pan array (for efficiency)
		float vol_l = inputs[i]->volume*(1.0f-inputs[i]->pan);
		float vol_r = inputs[i]->volume*inputs[i]->pan;
		for ( int j=0; j<numChannels; j++ ) {
			volumePerChannel[j] = (j==0?vol_l:vol_r) * masterVolume;
		}
		// mix working into output, respecting pan and volume and preserving interleaving
		for ( int j=0; j<numFrames; j++ ) {
			for ( int k=0; k<numChannels; k++ ) {
				output[j*numChannels+k] += working[j*numChannels+k] * volumePerChannel[k];
			}
		}
	}
	
	mutex.unlock();
		
}



/// Add an input to this unit from the given source unit
bool ofSoundMixer::addInputFrom( ofSoundSource* source )
{
	// check for branches/cycles
	if ( addingInputWouldCreateCycle( source ) )
	{
		ofLog( OF_LOG_ERROR, "ofSoundMixer: can't connect '%s' (%x) to '%s' (%x): this would create a cycle in the DSP graph",
			  source->getName().c_str(), source, this->getName().c_str(), this );
		return false;
	}

	mutex.lock();
	inputs.push_back( new MixerInput( source, 1.0f, 0.5f ) );
	source->setSampleRate( sampleRate );
	mutex.unlock();
	return true;
}


vector<ofSoundSource*> ofSoundMixer::getInputs()
{
	vector<ofSoundSource*> result;
	mutex.lock();
	for ( int i =0; i<(int)inputs.size(); i++ ) {
		result.push_back( inputs[i]->input );
	}
	mutex.unlock();
	return result;
}


void ofSoundMixer::setSampleRate( int rate )
{
	mutex.lock();
	for ( int i =0; i<(int)inputs.size(); i++ ) {
		inputs[i]->input->setSampleRate( rate );
	}
	mutex.unlock();
	ofSoundSource::setSampleRate( rate );
	ofSoundSink::setSampleRate( rate );
}




void ofSoundSourceTestTone::setSampleRate( int rate )
{
//	ofLog( OF_LOG_NOTICE, "ofSoundSourceTestTone got sample rate: %i", rate );
	sampleRate = rate;
	setFrequency( frequency );
}

void ofSoundSourceTestTone::setFrequency( float freq )
{
	frequency = freq;
	// calculate a phase advance per audio frame (sample)
	// basically, every OFSYNTH_SAMPLE_RATE frames (1s of audio), we want
	// to advance phase by frequency*TWO_PI.
	phaseAdvancePerFrame = (1.0f/sampleRate)*frequency*TWO_PI;
	// for the sawtooth, we want to go from -1..1
	sawAdvancePerFrame = (1.0f/sampleRate)*frequency;
	
//	ofLog( OF_LOG_NOTICE, "ofSoundSourceTestTone(%x) setFrequency, freq %f, sampleRate %i, advance %f", this, frequency, sampleRate, phaseAdvancePerFrame );
}

void ofSoundSourceTestTone::audioRequested( float* output, int numFrames, int numChannels )
{
//	ofLog( OF_LOG_NOTICE, "ofSoundSourceTestTone(%x) audioRequested, freq %f, sampleRate %i", this, frequency, sampleRate );
	// loop through all the frames
	if ( waveform == TESTTONE_SINE ) {
		for ( int i=0; i<numFrames; i++ ) {
			float value = sinf( phase );
			// write value to all the channels
			for ( int j=0; j<numChannels; j++ ) {
				output[i*numChannels+j] = value;
			}
			// advance phase
			phase += phaseAdvancePerFrame;
		}
		// wrap phase to 0..TWO_PI
		phase = remainderf( phase, TWO_PI );
	}
	else if ( waveform == TESTTONE_SAWTOOTH ) {
		for ( int i=0; i<numFrames; i++ ) {
			float value = sawPhase;
			// write value to all the channels
			// write value to all the channels
			for ( int j=0; j<numChannels; j++ ) {
				output[i*numChannels+j] = value;
			}
			// advance phase
			sawPhase += sawAdvancePerFrame;
			// wrap phase to -1..1
			sawPhase = remainderf( sawPhase, 2.0f );
		}
	}
}





void ofSoundSinkMicrophone::audioRequested( float* output, int numFrames, int numChannels )
{
	// copy from inputBuffer to output; upmix from microphone  
	if ( numFrames != inputBuffer.numFrames ) {
		ofLog( OF_LOG_WARNING, "ofSoundSinkMicrophone: microphone frame count seems different to output frame count; this is probably bad" );
	}
	inputBuffer.copyTo( output, numFrames, numChannels );
}


void ofSoundSourceMultiplexor::audioRequested( float* output, int numFrames, int numChannels ) {
	unsigned long tick = ofSoundStreamGetCurrentTick();
	
	// new tick: render upstream into input buffer
	if ( tick != lastRenderedTick ) {
		fillInputBufferFromUpstream( numFrames, numChannels );
		lastRenderedTick = tick;
	}
	
	// copy to output
	inputBuffer.copyTo( output, numFrames, numChannels );	
}


void ofSoundSourceMultiplexor::setSampleRate( int rate )
{
	ofSoundSink::setSampleRate( rate );
}
