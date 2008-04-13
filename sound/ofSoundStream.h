#ifndef _OF_SOUND_STREAM
#define _OF_SOUND_STREAM

#include "ofConstants.h"
#include "ofSimpleApp.h"

void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofSimpleApp * OFSA);
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofSimpleApp * OFSA, int sampleRate, int bufferSize, int nBuffers);
void ofSoundStreamStop();
void ofSoundStreamStart();
void ofSoundStreamClose();
void ofSoundStreamListDevices();

#endif
