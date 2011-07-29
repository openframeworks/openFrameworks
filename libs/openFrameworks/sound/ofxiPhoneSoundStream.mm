/***********************************************************************
 
 Copyright (c) 2009
 Memo Akten, http://www.memo.tv
 Marek Bareza http://mrkbrz.com/
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ***********************************************************************/
// mostly taken from http://michael.tyson.id.au/2008/11/04/using-remoteio-audio-unit/

#include "ofxiPhoneSoundStream.h"

#ifdef OF_SOUNDSTREAM_IPHONE
#include "ofSoundStream.h"
#include "ofMath.h"
#include "ofUtils.h"
#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>
#import "ofxiPhone.h"



static bool							isSetup			= false;
static bool							isRunning		= false;
AudioStreamBasicDescription			format, audioFormat;
AudioUnit							audioUnit = NULL;
static ofBaseSoundInput *			soundInputPtr = NULL;
static ofBaseSoundOutput *			soundOutputPtr = NULL;

#define kOutputBus	0
#define kInputBus	1

static inline void checkSoundStreamIsRunning() {
	if(!isSetup) ofSoundStreamSetup(0, 1, 22050, 22050 * 2/60, 2);
}

bool checkStatus(OSStatus err) {
	if(err!=noErr) {
		ofLog(OF_LOG_ERROR, "There was an error: " + err);
		//		soundInputPtr->error(err);
		return true;
	}
	return false;
}

// intermediate buffer for sample scaling
#define MAX_BUFFER_SIZE 4096
float tempBuffer[MAX_BUFFER_SIZE];

void rioInterruptionListener(void *inClientData, UInt32 inInterruption) {}

static OSStatus playbackCallback(void *inRefCon, 
								 AudioUnitRenderActionFlags *ioActionFlags, 
								 const AudioTimeStamp *inTimeStamp, 
								 UInt32 inBusNumber, 
								 UInt32 inNumberFrames, 
								 AudioBufferList *ioData) {   
	
	//	soundInputPtr->error(ioData->mNumberBuffers);
	for(int i = 0; i < ioData->mNumberBuffers; i++) {
		
		if(i==0) { // there should be only one buffer
			short int *buffer = (short int *)ioData->mBuffers[i].mData;
			
			// check to see if our buffer is big enough to store the data:
			if(ioData->mBuffers[i].mDataByteSize>MAX_BUFFER_SIZE*2) {
				int len = ioData->mBuffers[i].mDataByteSize/2;
				for(int j = 0; j < len; j++) {
					buffer[j] = 0;
				}
				//				if(soundInputPtr!=NULL) soundInputPtr->error(-1);
			} else {	
				
				// get floats from openframeworks
				if(soundOutputPtr!=NULL) soundOutputPtr->audioRequested(tempBuffer, ioData->mBuffers[i].mDataByteSize/(ioData->mBuffers[i].mNumberChannels*2), ioData->mBuffers[i].mNumberChannels);
				
				// truncate to 16bit fixed point data
				int len = ioData->mBuffers[i].mDataByteSize/2;
				for(int j = 0; j < len; j++) {
					buffer[j] = (int)((tempBuffer[j]) * 32767.f);
				}
			}
		} else {
			ioData->mBuffers[i].mDataByteSize = 0;
		}
	}
	
    return noErr;
	
}

short int *sampleBuffer;
bool done = false;
static OSStatus recordingCallback(void *inRefCon, 
                                  AudioUnitRenderActionFlags *ioActionFlags, 
                                  const AudioTimeStamp *inTimeStamp, 
                                  UInt32 inBusNumber, 
                                  UInt32 inNumberFrames, 
								  AudioBufferList *ioData) {
	
	
	AudioBufferList list;
	
	// redundant
	list.mNumberBuffers = 1;
	list.mBuffers[0].mData = sampleBuffer;
	list.mBuffers[0].mDataByteSize = 2 * inNumberFrames;
	list.mBuffers[0].mNumberChannels = 1;
	
	ioData = &list;
	//printf("No buffers: %d, buffer length: %d bus number: %d\n", ioData->mNumberBuffers, ioData->mBuffers[0].mDataByteSize, inBusNumber);
	
	
    // Then:
    // Obtain recorded samples
	
	OSStatus status = AudioUnitRender(audioUnit, ioActionFlags, inTimeStamp, 1, inNumberFrames, ioData);
    checkStatus(status);
	if(status!=noErr) return status;
	if(ioData->mNumberBuffers>0) {
		int i = 0;
		short int *buffer = (short int *) list.mBuffers[i].mData;
		for(int j = 0; j < ioData->mBuffers[i].mDataByteSize/2; j++) {
			// go through each sample and turn it into a float
			tempBuffer[j] = (float)buffer[j]/32767.f;
			
		}
		done = true;
		if(soundInputPtr!=NULL) soundInputPtr->audioReceived(tempBuffer, ioData->mBuffers[i].mDataByteSize/2, 1);
		
	}
	return noErr;
}

//------------------------------------------------------------------------------
ofxiPhoneSoundStream::ofxiPhoneSoundStream(){
}

//------------------------------------------------------------------------------
ofxiPhoneSoundStream::~ofxiPhoneSoundStream(){
}

//------------------------------------------------------------------------------
void ofxiPhoneSoundStream::listDevices(){	
	
}

//------------------------------------------------------------------------------
void ofxiPhoneSoundStream::setDeviceID(int _deviceID){
}

//------------------------------------------------------------------------------
void ofxiPhoneSoundStream::setInput(ofBaseSoundInput * soundInput){
	soundInputPtr = soundInput;
}

//------------------------------------------------------------------------------
void ofxiPhoneSoundStream::setOutput(ofBaseSoundOutput * soundOutput){
	soundOutputPtr = soundOutput;
}

//------------------------------------------------------------------------------
bool ofxiPhoneSoundStream::setup(int outChannels, int inChannels, int _sampleRate, int bufferSize, int nBuffers){
	
	nInputChannels = inChannels;
	nOutputChannels = outChannels;
	tickCount = 0;
	sampleRate = _sampleRate;
	
	if(isRunning) {
		OSStatus status = AudioOutputUnitStop(audioUnit);
		checkStatus(status);
	}
	
	isRunning = false;	
	OSStatus status;
	
	
	// Initialize and configure the audio session
	status = AudioSessionInitialize(NULL, NULL, rioInterruptionListener, NULL);
	if(checkStatus(status)) {
		ofLog(OF_LOG_ERROR, "couldn't initialize audio session");
	}
	status = AudioSessionSetActive(true);
	if(checkStatus(status)) {
		ofLog(OF_LOG_ERROR, "couldn't set audio session active");
	}
	
	Float32 preferredBufferSize = (float)bufferSize/sampleRate; 
	
	
	status = AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, sizeof(preferredBufferSize), &preferredBufferSize);
	if(checkStatus(status)) {
		ofLog(OF_LOG_ERROR, "couldn't set i/o buffer duration");
	}
	
	
	// Describe audio component
	AudioComponentDescription desc;
	desc.componentType = kAudioUnitType_Output; // this is for  output, input or input-output
	desc.componentSubType = kAudioUnitSubType_RemoteIO;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	
	// Get component
	AudioComponent inputComponent = AudioComponentFindNext(NULL, &desc);
	
	// Get audio units
	status = AudioComponentInstanceNew(inputComponent, &audioUnit);
	checkStatus(status);
	
	UInt32 flag = 1;
	
	checkStatus(status);
	
	// this is supposed to make the audio come out of the speaker rather
	// than the receiver, but I don't think it works when using the microphone as well.
	//	UInt32 category = kAudioSessionOverrideAudioRoute_Speaker;
	//	AudioSessionSetProperty(kAudioSessionProperty_OverrideAudioRoute, sizeof(category), &category);
	
	UInt32 category = 1;
	AudioSessionSetProperty(kAudioSessionOverrideAudioRoute_Speaker, sizeof(category), &category);
	
	
	// Describe format
	audioFormat.mSampleRate			= (double)sampleRate;
	audioFormat.mFormatID			= kAudioFormatLinearPCM;
	audioFormat.mFormatFlags		= kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
	audioFormat.mFramesPerPacket	= 1;
	audioFormat.mBitsPerChannel		= 16;
	
	AURenderCallbackStruct callbackStruct;
	
	if(outChannels>0) {
		// Enable IO for playback
		status = AudioUnitSetProperty(audioUnit, 
									  kAudioOutputUnitProperty_EnableIO, 
									  kAudioUnitScope_Output, 
									  kOutputBus,
									  &flag, 
									  sizeof(flag));
		checkStatus(status);
		
		audioFormat.mChannelsPerFrame	= outChannels;
		audioFormat.mBytesPerPacket		= outChannels*2;
		audioFormat.mBytesPerFrame		= outChannels*2;
		
		status = AudioUnitSetProperty(audioUnit, 
									  kAudioUnitProperty_StreamFormat, 
									  kAudioUnitScope_Input, 
									  kOutputBus, 
									  &audioFormat, 
									  sizeof(audioFormat));
		checkStatus(status);
		
		// Set output callback
		callbackStruct.inputProc = playbackCallback;
		callbackStruct.inputProcRefCon = NULL;
		status = AudioUnitSetProperty(audioUnit, 
									  kAudioUnitProperty_SetRenderCallback, 
									  kAudioUnitScope_Global, 
									  kOutputBus,
									  &callbackStruct, 
									  sizeof(callbackStruct));
		checkStatus(status);
	}
	if(inChannels>0) {
		sampleBuffer = (short int*) malloc(MAX_BUFFER_SIZE*2);
		// Enable IO for recording
		
		status = AudioUnitSetProperty(audioUnit, 
									  kAudioOutputUnitProperty_EnableIO, 
									  kAudioUnitScope_Input, 
									  kInputBus,
									  &flag, 
									  sizeof(flag));
		
		audioFormat.mChannelsPerFrame	= inChannels;
		audioFormat.mBytesPerPacket		= inChannels*2;
		audioFormat.mBytesPerFrame		= inChannels*2;
		
		// Apply format
		status = AudioUnitSetProperty(audioUnit, 
									  kAudioUnitProperty_StreamFormat, 
									  kAudioUnitScope_Output, 
									  kInputBus, 
									  &audioFormat, 
									  sizeof(audioFormat));
		checkStatus(status);
		
		
		
		
		// Set input callback
		callbackStruct.inputProc = recordingCallback;
		callbackStruct.inputProcRefCon = NULL;
		status = AudioUnitSetProperty(audioUnit, 
									  kAudioOutputUnitProperty_SetInputCallback, 
									  kAudioUnitScope_Global, 
									  kInputBus, 
									  &callbackStruct, 
									  sizeof(callbackStruct));
		checkStatus(status);
	}
	
	UInt32 shouldAllocateBuffer = 1;
	AudioUnitSetProperty(audioUnit, kAudioUnitProperty_ShouldAllocateBuffer, kAudioUnitScope_Global, 1, &shouldAllocateBuffer, sizeof(shouldAllocateBuffer));
	
	// Initialise
	status = AudioUnitInitialize(audioUnit);
	checkStatus(status);
	
	isSetup = true;
	ofSoundStreamStart();
}

//------------------------------------------------------------------------------
bool ofxiPhoneSoundStream::setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers){
	setInput(app);
	setOutput(app);
	setup(outChannels, inChannels, sampleRate, bufferSize, nBuffers);
}

//------------------------------------------------------------------------------
void ofxiPhoneSoundStream::start(){
	if(isRunning) ofSoundStreamStop();
	if(audioUnit!=NULL) {
		OSStatus status = AudioOutputUnitStart(audioUnit);
		checkStatus(status);
		isRunning = true;
	}
}

//------------------------------------------------------------------------------
void ofxiPhoneSoundStream::stop(){
	if(isRunning) {
		OSStatus status = AudioOutputUnitStop(audioUnit);
		checkStatus(status);
	}
	isRunning = false;	
}

//------------------------------------------------------------------------------
void ofxiPhoneSoundStream::close(){
	AudioUnitUninitialize(audioUnit);
	free(sampleBuffer);	
}

//------------------------------------------------------------------------------
long unsigned long ofxiPhoneSoundStream::getTickCount(){
	return tickCount;
}

//------------------------------------------------------------------------------
int ofxiPhoneSoundStream::getNumOutputChannels(){
	return nOutputChannels;
}

//------------------------------------------------------------------------------
int ofxiPhoneSoundStream::getNumInputChannels(){
	return nInputChannels;
}

#endif
