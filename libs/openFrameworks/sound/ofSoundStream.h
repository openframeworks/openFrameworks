#pragma once

#include "ofConstants.h"
class ofBaseApp;

// defined in ofSoundUnit.h
class ofSoundSource;
class ofSoundSink;

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
/// Get a list off all sound sources
vector<ofSoundSource*> ofSoundStreamGetAllSoundSources();

/// Return the current tick count
long unsigned long ofSoundStreamGetCurrentTick();

void ofSoundStreamStop();
void ofSoundStreamStart();
void ofSoundStreamClose();
void ofSoundStreamListDevices();
