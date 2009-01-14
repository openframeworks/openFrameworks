#ifndef _OF_SOUND_STREAM
#define _OF_SOUND_STREAM

#include "ofConstants.h"
#include "ofBaseApp.h"
#include "ofEvents.h"
#include "ofMath.h"

void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * OFSA);
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * OFSA, int sampleRate, int bufferSize, int nBuffers);
void ofSoundStreamStop();
void ofSoundStreamStart();
void ofSoundStreamClose();
void ofSoundStreamListDevices();

#endif
