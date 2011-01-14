/*
 *  ofSound.cpp
 *  openFrameworksLib
 *
 *  Created by damian on 12/01/11.
 *  Copyright 2011 frey damian@frey.co.nz. All rights reserved.
 *
 */

#include "ofSoundEffect.h"



void ofSoundEffect::audioRequested( float* buffer, int numFrames, int numChannels )
{
	fillInputBufferFromUpstream( numFrames, numChannels );
	
	// call process
	process( inputBuffer.buffer, buffer, numFrames, inputBuffer.numChannels, numChannels );
}


void ofSoundEffect::setSampleRate( int rate )
{
	ofSoundSource::setSampleRate( rate );
	ofSoundSink::setSampleRate( rate );
}



void ofSoundEffectPassthrough::process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels )
{
	// passthrough
	
	memcpy( output, input, numFrames*numOutChannels*sizeof(float) );
}




void ofSoundEffectVolume::process( float* input, float* output, int numFrames, int numInChannels, int numOutChannels )
{
	volume.rebuildRampIfNecessary();
	
	// loop through all the frames
	for ( int i=0; i<numFrames; i++ ) 
	{
		// write value to all the channels
		for ( int j=0; j<numOutChannels; j++ )
		{
			output[i*numOutChannels+j] = input[i*numInChannels+j] * volume.getDeclickedValue(); 
		}
		volume.frameTick();
	}
}
