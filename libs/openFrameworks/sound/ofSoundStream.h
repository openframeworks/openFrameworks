#ifndef _OF_SOUND_STREAM
#define _OF_SOUND_STREAM

#include "ofConstants.h"
#include "ofBaseApp.h"
#include "ofEvents.h"
#include "ofMath.h"
#include "ofSoundUnit.h"

class ofSoundSink;
class ofSoundSource;

void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * OFSA = NULL);
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, int sampleRate, int bufferSize, int nBuffers);
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * OFSA, int sampleRate, int bufferSize, int nBuffers);

/// Add the given ofSoundSource as an input source for the ofSoundStream system.
void ofSoundStreamAddSoundSource( ofSoundSource* source );
/// Remove the given ofSoundSource
void ofSoundStreamRemoveSoundSource( ofSoundSource* source );
/// Add the given ofSoundSink as a sink for incoming audio data (eg from a microphone)
void ofSoundStreamAddSoundSink( ofSoundSink* sink );
/// Remove the given ofSoundSink
void ofSoundStreamRemoveSoundSink( ofSoundSink* sink );


void ofSoundStreamStop();
void ofSoundStreamStart();
void ofSoundStreamClose();
void ofSoundStreamListDevices();



/*
class ofSoundSink
{
public:
	virtual ~ofSoundSink() { ofSoundStreamRemoveSoundSink( this ); }
	/// receive audio when available
	virtual void audioReceived( float* inputBuffer, int bufferSize, int nInputChannels ) = 0;
};

class ofSoundSource
{
public:
	virtual ~ofSoundSource() { ofSoundStreamRemoveSoundSource( this ); }
	/// set the sample rate to the given
	virtual void setSampleRate( int rate ) {};
	/// generate audio on request
	virtual void audioRequested( float* outputBuffer, int bufferSize, int nOutputChannels ) = 0;
};
*/


#endif
