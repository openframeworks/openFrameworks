/***********************************************************************
 
 Copyright (c) 2009
 Memo Akten, http://www.memo.tv
 Marek Bareza http://mrkbrz.com/
 
 Updated 2012 by Dan Wilcox <danomatika@gmail.com>
 
 references:
	- http://michael.tyson.id.au/2008/11/04/using-remoteio-audio-unit/
 
 ***********************************************************************/

#include "ofxiPhoneSoundStream.h"

#ifdef OF_SOUNDSTREAM_IPHONE

#include "ofSoundStream.h"
#include "ofMath.h"
#include "ofUtils.h"
#import <AudioToolbox/AudioToolbox.h>
#import "ofxiPhone.h"

static bool							isSetup			= false;
static bool							isRunning		= false;
AudioStreamBasicDescription			format, audioFormat;
AudioUnit							audioUnit		= NULL;
AudioBufferList						inputBufferList;		// input buffer
static ofBaseSoundInput *			soundInputPtr	= NULL;
static ofBaseSoundOutput *			soundOutputPtr	= NULL;

// intermediate buffer for sample scaling
#define MAX_BUFFER_SIZE 8192
float scaleBuffer[MAX_BUFFER_SIZE];

#define kOutputBus	0
#define kInputBus	1

//------------------------------------------------------------------------------

// returns true on error
//
// see general error codes here:
// http://www.opensource.apple.com/source/CarbonHeaders/CarbonHeaders-18.1/MacErrors.h
//
// error string conversion from:
// http://stackoverflow.com/questions/2196869/how-do-you-convert-an-iphone-osstatus-code-to-something-useful
//
bool checkStatus(OSStatus error) {
	if(error != noErr) {
		char* str = new char[32];
		// see if it appears to be a 4-char-code
		*(UInt32 *)(str + 1) = CFSwapInt32HostToBig(error);
		if(isprint(str[1]) && isprint(str[2]) && isprint(str[3]) && isprint(str[4])) {
			str[0] = str[5] = '\'';
			str[6] = '\0';
		} else {
			// no, format it as an integer
			sprintf(str, "%d", (int) error);
		}
		ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: OS status error code %s", str);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------

// called when the audio system is interrupted (backgrounded, etc)
static void rioInterruptionListener(void *inClientData, UInt32 inInterruption) {
	if(inInterruption == kAudioSessionBeginInterruption)
		ofLog(OF_LOG_VERBOSE, "ofxiPhoneSoundStream: Audio session interrupted");
	else if(inInterruption == kAudioSessionEndInterruption)
		ofLog(OF_LOG_VERBOSE, "ofxiPhoneSoundStream: Audio session resumed");
}

static OSStatus playbackCallback(void *inRefCon, 
								 AudioUnitRenderActionFlags *ioActionFlags, 
								 const AudioTimeStamp *inTimeStamp, 
								 UInt32 inBusNumber, 
								 UInt32 inNumberFrames, 
								 AudioBufferList *ioData) {
	if(soundInputPtr == NULL)
		return noErr;
	
	for(int i = 0; i < ioData->mNumberBuffers; i++) {
		
		short int *buffer = (short int *)ioData->mBuffers[i].mData;
		
		// check to see if our buffer is big enough to store the data:
		if(ioData->mBuffers[i].mDataByteSize > MAX_BUFFER_SIZE*2) {
			int len = ioData->mBuffers[i].mDataByteSize/2;
			for(int j = 0; j < len; j++) {
				buffer[j] = 0;
			}
		}
		else {
			// get floats from app
			soundOutputPtr->audioOut(scaleBuffer,
				ioData->mBuffers[i].mDataByteSize/(ioData->mBuffers[i].mNumberChannels*2),
				ioData->mBuffers[i].mNumberChannels);
			
			// truncate to 16bit fixed point data
			int len = ioData->mBuffers[i].mDataByteSize/2;
			for(int j = 0; j < len; j++) {
				buffer[j] = (int) (scaleBuffer[j] * 32767.f);
			}
		}
	}
	
    return noErr;
}

static OSStatus recordingCallback(void *inRefCon, 
                                  AudioUnitRenderActionFlags *ioActionFlags, 
                                  const AudioTimeStamp *inTimeStamp, 
                                  UInt32 inBusNumber, 
                                  UInt32 inNumberFrames, 
								  AudioBufferList *ioData) {
	
	// set input buffer params
	inputBufferList.mBuffers[0].mDataByteSize = 2*inNumberFrames*inputBufferList.mBuffers[0].mNumberChannels;
	ioData = &inputBufferList;
	
    // obtain recorded samples
	OSStatus status = AudioUnitRender(audioUnit, ioActionFlags, inTimeStamp, 1, inNumberFrames, ioData);
    if(checkStatus(status)) {
		ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't render input audio samples");
		return status;
	}
	
	// send data to app
	if(soundInputPtr != NULL) {
		for(int i = 0; i < ioData->mNumberBuffers; ++i) {
			short int *buffer = (short int *) ioData->mBuffers[i].mData;
			for(int j = 0; j < ioData->mBuffers[i].mDataByteSize/2; ++j) {
				scaleBuffer[j] = (float) buffer[j] / 32767.f;	// convert each sample into a float
			}
			soundInputPtr->audioIn(scaleBuffer,
				ioData->mBuffers[i].mDataByteSize/(ioData->mBuffers[i].mNumberChannels*2),
				ioData->mBuffers[i].mNumberChannels);
		}
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
	
	// nBuffers is always 1  (see CoreAudio AudioBuffer struct)
	// this may change in the future ...
	nBuffers = 1;
	
	if(isRunning) {
		stop();
		close();
	}

	OSStatus status;
	
	// initialize and configure the audio session
	status = AudioSessionInitialize(NULL, NULL, rioInterruptionListener, NULL);
	if(checkStatus(status)) {
		ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't initialize audio session");
	}
	status = AudioSessionSetActive(true);
	if(checkStatus(status)) {
		ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't set audio session active");
	}
	
	Float32 preferredBufferSize = (float) bufferSize/sampleRate; 
	
	
	status = AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, sizeof(preferredBufferSize), &preferredBufferSize);
	if(checkStatus(status)) {
		ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't set i/o buffer duration");
	}
	
	
	// describe audio component
	AudioComponentDescription desc;
	desc.componentType = kAudioUnitType_Output; // this is for output, input, or input-output
	desc.componentSubType = kAudioUnitSubType_RemoteIO;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	
	// get component
	AudioComponent inputComponent = AudioComponentFindNext(NULL, &desc);
	
	// get audio units
	status = AudioComponentInstanceNew(inputComponent, &audioUnit);
	if(checkStatus(status)) {
		ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't create audio unit");
	}
	
	// this is supposed to make the audio come out of the speaker rather
	// than the receiver, but I don't think it works when using the microphone as well.
	//	UInt32 category = kAudioSessionOverrideAudioRoute_Speaker;
	//	AudioSessionSetProperty(kAudioSessionProperty_OverrideAudioRoute, sizeof(category), &category);
	
	UInt32 category = 1;
	status = AudioSessionSetProperty(kAudioSessionOverrideAudioRoute_Speaker, sizeof(category), &category);
	if(checkStatus(status)) {
		ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't set ignore speaker routing");
	}
	
	// describe format
	audioFormat.mSampleRate			= (double)sampleRate;
	audioFormat.mFormatID			= kAudioFormatLinearPCM;
	audioFormat.mFormatFlags		= kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
	audioFormat.mFramesPerPacket	= 1;
	audioFormat.mBitsPerChannel		= 16;
	
	AURenderCallbackStruct callbackStruct;
	UInt32 flag = 1;
	
	if(outChannels > 0) {
	
		// enable IO for playback
		status = AudioUnitSetProperty(audioUnit, 
									  kAudioOutputUnitProperty_EnableIO, 
									  kAudioUnitScope_Output, 
									  kOutputBus,
									  &flag, 
									  sizeof(flag));
		if(checkStatus(status)) {
			ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't enable audio output");
		}
		
		// set output format
		audioFormat.mChannelsPerFrame	= outChannels;
		audioFormat.mBytesPerPacket		= outChannels*2;
		audioFormat.mBytesPerFrame		= outChannels*2;
		
		status = AudioUnitSetProperty(audioUnit, 
									  kAudioUnitProperty_StreamFormat, 
									  kAudioUnitScope_Input, 
									  kOutputBus, 
									  &audioFormat, 
									  sizeof(audioFormat));
		if(checkStatus(status)) {
			ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't set output format");
		}
		
		// set output callback
		callbackStruct.inputProc = playbackCallback;
		callbackStruct.inputProcRefCon = NULL;
		status = AudioUnitSetProperty(audioUnit, 
									  kAudioUnitProperty_SetRenderCallback, 
									  kAudioUnitScope_Global, 
									  kOutputBus,
									  &callbackStruct, 
									  sizeof(callbackStruct));
		if(checkStatus(status)) {
			ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't set output callback");
		}
	}
	
	if(inChannels > 0) {
		
		// enable IO for recording
		status = AudioUnitSetProperty(audioUnit, 
									  kAudioOutputUnitProperty_EnableIO, 
									  kAudioUnitScope_Input, 
									  kInputBus,
									  &flag, 
									  sizeof(flag));
		if(checkStatus(status)) {
			ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't enable audio input");
		}
		
		audioFormat.mChannelsPerFrame	= inChannels;
		audioFormat.mBytesPerPacket		= inChannels*2;
		audioFormat.mBytesPerFrame		= inChannels*2;
		
		// set input format
		status = AudioUnitSetProperty(audioUnit, 
									  kAudioUnitProperty_StreamFormat, 
									  kAudioUnitScope_Output, 
									  kInputBus, 
									  &audioFormat, 
									  sizeof(audioFormat));
		if(checkStatus(status)) {
			ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't enable set input format");
		}
		
		// setup input buffer
		inputBufferList.mNumberBuffers = nBuffers;
		for(int i = 0; i < inputBufferList.mNumberBuffers; ++i) {
			inputBufferList.mBuffers[i].mData = (short int*) malloc(MAX_BUFFER_SIZE*2*inChannels);
			inputBufferList.mBuffers[i].mDataByteSize = audioFormat.mBytesPerFrame;
			inputBufferList.mBuffers[i].mNumberChannels = inChannels;
		}
		
		// set input callback
		callbackStruct.inputProc = recordingCallback;
		callbackStruct.inputProcRefCon = NULL;
		status = AudioUnitSetProperty(audioUnit, 
									  kAudioOutputUnitProperty_SetInputCallback, 
									  kAudioUnitScope_Global, 
									  kInputBus, 
									  &callbackStruct, 
									  sizeof(callbackStruct));
		if(checkStatus(status)) {
			ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't set input callback");
		}
	}
	
	UInt32 shouldAllocateBuffer = 1;
	AudioUnitSetProperty(audioUnit, kAudioUnitProperty_ShouldAllocateBuffer,
						 kAudioUnitScope_Global, 1, &shouldAllocateBuffer,
						 sizeof(shouldAllocateBuffer));
	
	// Initialise
	status = AudioUnitInitialize(audioUnit);
	if(checkStatus(status)) {
		ofLog(OF_LOG_ERROR, "ofxiPhoneSoundStream: Couldn't initialize audio unit");
	}
	
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
	if(isRunning)
		ofSoundStreamStop();
	if(audioUnit != NULL) {
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
	if(audioUnit != NULL)
		AudioUnitUninitialize(audioUnit);
	
	// clear input buffer
	for(int i = 0; i < inputBufferList.mNumberBuffers; ++i) {
		if(inputBufferList.mBuffers[i].mData != NULL)
			free(inputBufferList.mBuffers[i].mData);
	}
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
