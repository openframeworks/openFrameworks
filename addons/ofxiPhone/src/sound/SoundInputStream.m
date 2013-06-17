//
//  SoundInputStream.m
//  Created by Lukasz Karluk on 13/06/13.
//  http://julapy.com/blog
//
//  Original code by,
//  Memo Akten, http://www.memo.tv
//  Marek Bareza http://mrkbrz.com/
//  Updated 2012 by Dan Wilcox <danomatika@gmail.com>
//
//  references,
//  http://www.cocoawithlove.com/2010/10/ios-tone-generator-introduction-to.html
//  http://atastypixel.com/blog/using-remoteio-audio-unit/
//  http://www.stefanpopp.de/2011/capture-iphone-microphone/
//

#import "SoundInputStream.h"

void soundInputStreamInterruptionListener(void *inRefCon, UInt32 inInterruptionState) {
    SoundInputStream * stream = (SoundInputStream *)inRefCon;
    
	if(inInterruptionState == kAudioSessionBeginInterruption) {
        if([stream isStreaming]) {
            stream.bInterruptedWhileRunning = YES;
        }
        [stream stop];
        
        if([stream.delegate respondsToSelector:@selector(soundStreamBeginInterruption:)]) {
            [stream.delegate soundStreamBeginInterruption:stream];
        }
	} else if(inInterruptionState == kAudioSessionEndInterruption) {
        if(stream.bInterruptedWhileRunning) {
            stream.bInterruptedWhileRunning = NO;
            [stream start];
        }
        
        if([stream.delegate respondsToSelector:@selector(soundStreamEndInterruption:)]) {
            [stream.delegate soundStreamEndInterruption:stream];
        }
    }
}

static OSStatus soundInputStreamRenderCallback(void *inRefCon,
                                               AudioUnitRenderActionFlags *ioActionFlags,
                                               const AudioTimeStamp *inTimeStamp,
                                               UInt32 inBusNumber,
                                               UInt32 inNumberFrames,
                                               AudioBufferList *ioData) {

    SoundInputStream * stream = (SoundInputStream *)inRefCon;
    float * scaleBuffer = stream.scaleBuffer;
    AudioBufferList * bufferList = stream.bufferList;
    AudioBuffer * audioBuffer = &bufferList->mBuffers[0];
    
    // for the iphone the number of frames is usally 512 or 1024.
    int dataByteSize = inNumberFrames * 2;
	if(audioBuffer->mDataByteSize != dataByteSize) {
		free(audioBuffer->mData); // clear old buffer
		audioBuffer->mDataByteSize = dataByteSize; // assing new byte size
		audioBuffer->mData = (short int *)malloc(dataByteSize);
	}
    
	OSStatus status = AudioUnitRender(stream.audioUnit,
                                      ioActionFlags,
                                      inTimeStamp,
                                      inBusNumber,
                                      inNumberFrames,
                                      bufferList);
    if(status != kAudioSessionNoError) {
        if([stream.delegate respondsToSelector:@selector(soundStreamError:error:)]) {
            [stream.delegate soundStreamError:stream error:@"Could not render input audio samples"];
        }
		return status;
	}
	
    short int * buffer = (short int *)audioBuffer->mData;
    for(int i=0; i<audioBuffer->mDataByteSize/2; i++) {
        scaleBuffer[i] = (float)buffer[i] / 32767.f;	// convert each sample into a float
    }
    
    int bufferSize = audioBuffer->mDataByteSize / (audioBuffer->mNumberChannels * 2);
    int numOfChannels = audioBuffer->mNumberChannels;
    
    if([stream.delegate respondsToSelector:@selector(soundStreamReceived:input:bufferSize:numOfChannels:)]) {
        [stream.delegate soundStreamReceived:stream
                                       input:scaleBuffer
                                  bufferSize:bufferSize
                               numOfChannels:numOfChannels];
    }
    
	return noErr;
}

//----------------------------------------------------------------
@implementation SoundInputStream

@synthesize bufferList;

- (id)initWithNumOfChannels:(NSInteger)value0
             withSampleRate:(NSInteger)value1
             withBufferSize:(NSInteger)value2 {
    self = [super initWithNumOfChannels:value0
                         withSampleRate:value1
                         withBufferSize:value2];
    if(self) {
        streamType = SoundStreamTypeInput;
    }
    
    return self;
}

- (void)dealloc {
    [self stop];
    [super dealloc];
}

- (void)start {
    [super start];
    
    if([self isStreaming] == YES) {
        return; // already running.
    }
    
	OSStatus status = NULL;
    status = AudioSessionInitialize(NULL, NULL, soundInputStreamInterruptionListener, self);
	if([self checkStatus:status] == YES) {
		//
	}
	status = AudioSessionSetActive(true);
    if([self checkStatus:status] == NO) {
        //
    }
    
    //---------------------------------------------------------- audio session config.
    
    // this is needed to request the bufferSize we want,
    // otherwise the default bufferSize (usally 1024) will be returned.
	Float32 preferredBufferLength = bufferSize / (float)sampleRate;
	status = AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration,
                                     sizeof(preferredBufferLength),
                                     &preferredBufferLength);
	if([self checkStatus:status] == NO) {
		//
	}
    
    // Used when recording and playing back audio.
    UInt32 sessionType = kAudioSessionCategory_PlayAndRecord;
    status = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory,
                                     sizeof(sessionType),
                                     &sessionType);
	if([self checkStatus:status] == NO) {
		//
	}
    
    // Route audio output to speaker.  Use this override with the kAudioSessionCategory_PlayAndRecord
    // category, which by default routes the output to the receiver.
    UInt32 overrideAudioRoute = kAudioSessionOverrideAudioRoute_Speaker;
    status = AudioSessionSetProperty(kAudioSessionProperty_OverrideAudioRoute,
                                     sizeof(overrideAudioRoute),
                                     &overrideAudioRoute);
	if([self checkStatus:status] == NO) {
		//
	}
    
    //---------------------------------------------------------- audio unit.
    
	// Configure the search parameters to find the default playback output unit
	// (called the kAudioUnitSubType_RemoteIO on iOS but
	// kAudioUnitSubType_DefaultOutput on Mac OS X)
	AudioComponentDescription desc;
	desc.componentType = kAudioUnitType_Output;
	desc.componentSubType = kAudioUnitSubType_RemoteIO;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
    
    // get component and get audio units.
	AudioComponent inputComponent = AudioComponentFindNext(NULL, &desc);
	status = AudioComponentInstanceNew(inputComponent, &audioUnit);
	if([self checkStatus:status] == NO) {
		//
	}
    
    //---------------------------------------------------------- enable io.

    UInt32 on = 1;
    UInt32 off = 0;
    
    // enable input to AudioUnit.
	status = AudioUnitSetProperty(audioUnit,
								  kAudioOutputUnitProperty_EnableIO,
								  kAudioUnitScope_Input,
								  kInputBus,
								  &on,
								  sizeof(on));
    if([self checkStatus:status]) {
        //
    }
    
    // enable output out of AudioUnit.
	status = AudioUnitSetProperty(audioUnit,
								  kAudioOutputUnitProperty_EnableIO,
								  kAudioUnitScope_Output,
								  kOutputBus,
								  &on,
								  sizeof(on));
    if([self checkStatus:status]) {
        //
    }
    
    //---------------------------------------------------------- format.
    
    // Describe format
    AudioStreamBasicDescription audioFormat;
	audioFormat.mSampleRate = (double)sampleRate;
	audioFormat.mFormatID = kAudioFormatLinearPCM;
	audioFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
	audioFormat.mFramesPerPacket = 1;
    audioFormat.mChannelsPerFrame = numOfChannels;
	audioFormat.mBitsPerChannel = 16;
    audioFormat.mBytesPerPacket = numOfChannels * 2;
    audioFormat.mBytesPerFrame = numOfChannels * 2;
    
    // Apply format
	status = AudioUnitSetProperty(audioUnit,
								  kAudioUnitProperty_StreamFormat,
								  kAudioUnitScope_Output,
								  kInputBus,
								  &audioFormat,
								  sizeof(audioFormat));
    if([self checkStatus:status] == NO) {
        //
    }
    
    //---------------------------------------------------------- callback.
    
    // input callback
    AURenderCallbackStruct callback;
	callback.inputProc = soundInputStreamRenderCallback;
	callback.inputProcRefCon = self;
	status = AudioUnitSetProperty(audioUnit,
                                  kAudioOutputUnitProperty_SetInputCallback,
								  kAudioUnitScope_Global,
								  kInputBus,
								  &callback,
								  sizeof(callback));
    if([self checkStatus:status] == NO) {
        // 
    }
    
    //---------------------------------------------------------- make buffers.
    
    bufferList = (AudioBufferList*)malloc(sizeof(AudioBufferList) * numOfChannels);
    bufferList->mNumberBuffers = numOfBuffers;
    for(int i=0; i<bufferList->mNumberBuffers; i++) {
        bufferList->mBuffers[i].mNumberChannels = numOfChannels;
        bufferList->mBuffers[i].mDataByteSize = bufferSize * 2;
        bufferList->mBuffers[i].mData = (short int *)malloc(bufferSize * 2);
    }
    
    //---------------------------------------------------------- go!
    status = AudioUnitInitialize(audioUnit);
    if([self checkStatus:status] == NO) {
        //
    }
    
    status = AudioOutputUnitStart(audioUnit);
    if([self checkStatus:status] == NO) {
        //
    }
}

- (void)stop {
    [super stop];
    
    if([self isStreaming] == NO) {
        return;
    }
    
    AudioOutputUnitStop(audioUnit);
    AudioUnitUninitialize(audioUnit);
    AudioComponentInstanceDispose(audioUnit);
    audioUnit = nil;
    AudioSessionSetActive(false);
    
	for(int i=0; i<bufferList->mNumberBuffers; i++) {
		free(bufferList->mBuffers[i].mData);
	}
    free(bufferList);
}

@end
