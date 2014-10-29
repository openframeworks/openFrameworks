/*
 * ofSoundUnit.cpp
 * ofSound
 *
 * Created by damian on 10/01/11.
 * Copyright 2011 frey damian@frey.co.nz. All rights reserved.
 *
 */

#include "ofxSoundUnit.h"
#include "ofUtils.h"

#include <deque>
using namespace std;

/*
vector<ofSoundSource*> ofSoundUnit::getInputs()
{
ofLogWarning("ofxSoundUnit") << "getInputs(): called";
// no inputs
return vector<ofSoundSource*>();
}*/



void ofxSoundSink::setSampleRate( int rate )
{

	vector<ofxSoundSource*> inputs = getInputs();
	for ( int i=0; i<(int)inputs.size(); i++ )
	{
		inputs[i]->setSampleRate( rate );
	}
	sampleRate = rate;
}

bool ofxSoundSink::addInputFrom( ofxSoundSource* from )
{
	// check for existing input
	if ( input != NULL )
	{
		ofLogError("ofxSoundSink") << "addInputFrom(): can't connect \"" << from->getName() << "\""" "
			<< "(" << ios::hex << from << ios::dec << ") to \"" << this->getName() << "\" "
			<< "(" << ios::hex << this << ios::dec << "): it already has an input";
		return false;
	}
	// check for branches/cycles
	if ( addingInputWouldCreateCycle( from ) )
	{
		ofLogError("ofxSoundSink") << "addInputFrom(): can't connect \"" << from->getName() << "\""" "
			<< "(" << ios::hex << from << ios::dec << ") to \"" << this->getName() << "\" "
			<< "(" << ios::hex << this << ios::dec << "): this would create a cycle in the DSP graph";
		return false;
	}

	input = from;
	from->setSampleRate( sampleRate );
	return true;
}

vector<ofxSoundSource*> ofxSoundSink::getInputs()
{
	lock();
	vector<ofxSoundSource*> result;
	if ( input )
	{
		result.push_back( input );
	}
	unlock();
	return result;

}

/// Receive incoming audio from elsewhere (eg coming from a microphone or line input source)
void ofxSoundSink::audioIn( float* buffer, int numFrames, int numChannels, long unsigned long  tickCount )
{
	lock();
	inputBuffer.allocate( numFrames, numChannels );
	memcpy( inputBuffer.buffer, buffer, numFrames*numChannels*sizeof(float) );
	unlock();
}


void ofxSoundSink::fillInputBufferFromUpstream( int numFrames, int numChannels,long unsigned long tickCount   )
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
		input->audioOut( inputBuffer.buffer, inputBuffer.numFrames, inputBuffer.numChannels, tickCount );
	}

}


// Return true if adding this edge to the graph would create a cycle
bool ofxSoundSink::addingInputWouldCreateCycle( ofxSoundSource* test )
{
	// assuming the graph has no cycles from the start, can we already trace a path
	// from test_input to ourselves? if we can, then adding test_input will create
	// a cycle.

	// do a depth-first traversal
	deque<ofxSoundUnit*> stack;
	stack.push_back( test );
	while ( !stack.empty() )
	{
		ofxSoundUnit* u = stack.back();
		stack.pop_back();
		// if u is this, then we have looped round to the beginning
		if ( u == this ) {
			return true;
		}

		// fetch all immediate upstream neighbours
		vector<ofxSoundSource*> upstream = u->getInputs();
		// copy downstream to the back of stack
		copy( upstream.begin(), upstream.end(), back_inserter( stack ) );
	}


	// if we made it here, the cycle test has failed to find a cycle
	return false;
}





ofxSoundBuffer::ofxSoundBuffer() {
	numFrames = 0;
	numChannels = 0;
	buffer = NULL;
}
ofxSoundBuffer::ofxSoundBuffer( const ofxSoundBuffer& other ) {
	copyFrom( other );
}
ofxSoundBuffer::ofxSoundBuffer( int nFrames, int nChannels ) {
	numFrames = nFrames;
	numChannels = nChannels;
	buffer = new float[nFrames*nChannels];
}
ofxSoundBuffer& ofxSoundBuffer::operator=( const ofxSoundBuffer& other ) {
	copyFrom( other );
	return *this;
}

ofxSoundBuffer::~ofxSoundBuffer() {
	if ( buffer ) {
		delete[] buffer;
	}
}

void ofxSoundBuffer::copyFrom( const ofxSoundBuffer& other ) {
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
void ofxSoundBuffer::clear() {
	if ( buffer ) {
		memset( buffer, 0, sizeof(float)*numFrames*numChannels);
	}
}

/// Allocate the buffer to the given size if necessary.
void ofxSoundBuffer::allocate( int nFrames, int nChannels ) {
	if ( !buffer || numFrames != nFrames || numChannels != nChannels ) {
		numFrames = nFrames; numChannels = nChannels;
		if ( buffer ) {
			delete[] buffer;
		}
		buffer = new float[numFrames*numChannels];
	}
}

/// Copy just a single channel to output. output should have space for numFrames floats.
void ofxSoundBuffer::copyChannel( int channel, float* output ) const {
	if ( buffer && channel < numChannels ) {
		float * buffer_ptr = buffer;
		for ( int i=0; i<numFrames; i++ ) {
			*output++ = *(buffer_ptr+channel);
			buffer_ptr += numChannels;
		}
	}
}

/// Copy safely to out. Copy as many frames as possible, repeat channels as necessary.
void ofxSoundBuffer::copyTo( float* outBuffer, int outNumFrames, int outNumChannels ) {
	if ( numFrames == 0 || numChannels == 0 ) {
		ofLogWarning("ofxSoundBuffer") << "copyTo(): copy requested on empty buffer, returning nothing (check your addInputTo() methods!)";
		memset( outBuffer, 0, sizeof(float)*outNumFrames*outNumChannels );
	}
	if ( outNumFrames>numFrames || outNumChannels>numChannels ) {
		ofLogWarning("ofxSoundBuffer") << "copyTo(): " << outNumFrames << " frames requested but only " << numFrames << " are available";
		ofLogWarning("ofxSoundBuffer") << "copyTo(): " << outNumChannels << " channels requested but only " << numChannels << " are available, looping to make up the difference";

		for ( int i=0; i<min(numFrames,outNumFrames); i++ ) {
			for ( int j=0; j<outNumChannels; j++ ) {
				// copy input to output; in cases where input has fewer channels than output, loop through input frames repeatedly
				*outBuffer++ = buffer[i*numChannels+(j%numChannels)];
			}
		}
	}else{
		memcpy(outBuffer,buffer,outNumFrames*outNumChannels*sizeof(float));
	}
}





ofxSoundMixer::~ofxSoundMixer() {
	if ( working )
		delete[] working;
	for ( int i =0; i<(int)inputs.size(); i++ ) {
		delete inputs[i];
	}
}

void ofxSoundMixer::copyFrom( const ofxSoundMixer& other ) {
	ofLogWarning("ofxSoundMixer") << "copyFrom(): copying an ofSoundMixer will probably make things break";
	working = NULL;
	for ( int i=0; i<(int)other.inputs.size(); i++ ) {
		addInputFrom( other.inputs[i]->input );
		setVolume( other.inputs[i]->input, other.inputs[i]->volume );
		setPan( other.inputs[i]->input, other.inputs[i]->pan );
	}
	masterVolume = other.masterVolume;
}

void ofxSoundMixer::setVolume( ofxSoundSource* input, float vol ) {
	if ( isnan(vol) || !isfinite(vol) ) {
		return;
	}
	for ( int i=0; i<(int)inputs.size(); i++ ) {
		if ( inputs[i]->input == input ) {
			inputs[i]->volume = vol;
		}
	}
}

void ofxSoundMixer::setPan( ofxSoundSource* input, float pan ) {
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
		ofLogWarning("ofxSoundMixer") << "setPan(): couldn't find input for \"" << input->getName() << "\" "
			<< "(" << ios::hex << input << ios::dec << ")";
	}
}




// Render the DSP chain. output is interleaved and has space for
// numFrames*numChannels floats.
void ofxSoundMixer::audioOut( float* output, int numFrames, int numChannels, long unsigned long tickCount )
{
	// advance DSP
	if ( numChannels != 1 && numChannels != 2 ) {
		ofLogError("ofxSoundMixer") << "audioOut(): only 1 or 2 channels supported, requested " << numChannels;
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
		inputs[i]->input->audioOut( working, numFrames, numChannels, tickCount );

		// construct precalculated volume + pan array (for efficiency)
		float vol_l = inputs[i]->volume*(1.0f-inputs[i]->pan);
		float vol_r = inputs[i]->volume*inputs[i]->pan;
		for ( int j=0; j<numChannels; j++ ) {
			volumePerChannel[j] = (j==0?vol_l:vol_r) * masterVolume;
		}
		// mix working into output, respecting pan and volume and preserving interleaving
		float *working_ptr = working;
		float *output_ptr = output;
		for ( int j=0; j<numFrames; j++ ) {
			for ( int k=0; k<numChannels; k++ ) {
				*output_ptr++ += *working_ptr++ * volumePerChannel[k];
			}
		}
	}
	if(source){
		memset( working, 0, numFrames*numChannels*sizeof(float) );
		source->audioOut( working, numFrames, numChannels, 0, tickCount ); // TODO fix: deviceId is hardcoded here
		float *working_ptr = working;
		float *output_ptr = output;
		for ( int j=0; j<numFrames; j++ ) {
			for ( int k=0; k<numChannels; k++ ) {
				*output_ptr++ += *working_ptr++;
			}
		}
	}
	mutex.unlock();

}



/// Add an input to this unit from the given source unit
bool ofxSoundMixer::addInputFrom( ofxSoundSource* source )
{
	// check for branches/cycles
	if ( addingInputWouldCreateCycle( source ) )
	{
		ofLogError("ofxSoundMixer") <<
			"addInputFrom(): can't connect '" << source->getName() << "' (" << ofToString(source) << ") " <<
			"to '" << this->getName() << "' (" << ofToString(this) << "): this would create a cycle in the DSP graph";
		return false;
	}

	mutex.lock();
	inputs.push_back( new MixerInput( source, 1.0f, 0.5f ) );
	source->setSampleRate( sampleRate );
	mutex.unlock();
	return true;
}


vector<ofxSoundSource*> ofxSoundMixer::getInputs()
{
	vector<ofxSoundSource*> result;
	mutex.lock();
	for ( int i =0; i<(int)inputs.size(); i++ ) {
		result.push_back( inputs[i]->input );
	}
	mutex.unlock();
	return result;
}


void ofxSoundMixer::setSampleRate( int rate )
{
	mutex.lock();
	for ( int i =0; i<(int)inputs.size(); i++ ) {
		inputs[i]->input->setSampleRate( rate );
	}
	mutex.unlock();
	ofxSoundSource::setSampleRate( rate );
	ofxSoundSink::setSampleRate( rate );
}




void ofxSoundSourceTestTone::setSampleRate( int rate )
{
	// ofLogNotice("ofxSoundSourceTestTone") << "setSampleRate(): new sample rate: " << rate;
	sampleRate = rate;
	setFrequency( frequency );
}

void ofxSoundSourceTestTone::setFrequency( float freq )
{
	frequency = freq;
	// calculate a phase advance per audio frame (sample)
	// basically, every OFSYNTH_SAMPLE_RATE frames (1s of audio), we want
	// to advance phase by frequency*TWO_PI.
	phaseAdvancePerFrame = (1.0f/sampleRate)*frequency*TWO_PI;
	// for the sawtooth, we want to go from -1..1
	sawAdvancePerFrame = (1.0f/sampleRate)*frequency;

	// ofLogNotice("ofxSoundSourceTestTone") << "setFrequency(): " << "(" << ios::hex << this << ios::dec << ") "
	//	<< "freq " << frequency << ", sampleRate " << sampleRate << ", advance " << phaseAdvancePerFrame;
}

void ofxSoundSourceTestTone::audioOut( float* output, int numFrames, int numChannels, long unsigned long tickCount )
{
	// ofLogNotice("ofxSoundSourceTestTone") << "audioOut(): " << "(" << ios::hex << this << ios::dec << ") "
	//	<< "freq " << frequency << ", sampleRate " << sampleRate;
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



void ofxSoundSinkMicrophone::audioOut( float* output, int numFrames, int numChannels, long unsigned long tickCount )
{
	// copy from inputBuffer to output; upmix from microphone
	if ( numFrames != inputBuffer.numFrames ) {
		ofLogWarning("ofxSoundSinkMicrophone") << "audioOut(): microphone frame count of " << numFrames
			<< " seems different to output frame count of " << inputBuffer.numFrames << ", this is probably bad";
	}
	inputBuffer.copyTo( output, numFrames, numChannels );
}



void ofxSoundSourceMultiplexor::audioOut( float* output, int numFrames, int numChannels, long unsigned long tickCount ) {

	// new tick: render upstream into input buffer
	if ( tickCount != lastRenderedTick ) {
		fillInputBufferFromUpstream( numFrames, numChannels, tickCount );
		lastRenderedTick = tickCount;
	}

	// copy to output
	inputBuffer.copyTo( output, numFrames, numChannels );
}


void ofxSoundSourceMultiplexor::setSampleRate( int rate )
{
	ofxSoundSink::setSampleRate( rate );
}
