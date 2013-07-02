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

typedef struct {
	AudioBufferList * bufferList;
	AudioUnit remoteIO;
	SoundInputStream * stream;
}
SoundInputStreamContext;

@interface SoundInputStream() {
	SoundInputStreamContext context;
}
@end

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

    SoundInputStreamContext * context = (SoundInputStreamContext *)inRefCon;
	AudioBufferList * bufferList = context->bufferList;
	AudioBuffer * buffer = &bufferList->mBuffers[0];
	
	// make sure our buffer is big enough
	UInt32 necessaryBufferSize = inNumberFrames * sizeof(Float32);
	if(buffer->mDataByteSize < necessaryBufferSize) {
		free(buffer->mData);
		buffer->mDataByteSize = necessaryBufferSize;
		buffer->mData = malloc(necessaryBufferSize);
	}
	
	// we need to store the original buffer size, since AudioUnitRender seems to change the value
	// of the AudioBufferList's mDataByteSize (at least in the simulator). We need to write it back
	// later, or else we'll end up reallocating continuously in the render callback (BAD!)
	UInt32 bufferSize = buffer->mDataByteSize;
    
	OSStatus status = AudioUnitRender(context->remoteIO,
                                      ioActionFlags,
                                      inTimeStamp,
                                      inBusNumber,
                                      inNumberFrames,
                                      context->bufferList);
    
	if(status != noErr) {
		@autoreleasepool {
			if([context->stream.delegate respondsToSelector:@selector(soundStreamError:error:)]) {
				[context->stream.delegate soundStreamError:context->stream error:@"Could not render input audio samples"];
			}
		}
		return status;
	}

    if([context->stream.delegate respondsToSelector:@selector(soundStreamReceived:input:bufferSize:numOfChannels:)]) {
        [context->stream.delegate soundStreamReceived:context->stream
												input:bufferList->mBuffers[0].mData
										   bufferSize:bufferList->mBuffers[0].mDataByteSize / sizeof(Float32)
										numOfChannels:bufferList->mBuffers[0].mNumberChannels];
    }
	
	bufferList->mBuffers[0].mDataByteSize = bufferSize;
    
	return noErr;
}

//----------------------------------------------------------------
@implementation SoundInputStream

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
    AudioStreamBasicDescription audioFormat = {
		.mSampleRate       = sampleRate,
		.mFormatID         = kAudioFormatLinearPCM,
		.mFormatFlags      = kAudioFormatFlagsNativeFloatPacked,
		.mFramesPerPacket  = 1,
		.mChannelsPerFrame = numOfChannels,
		.mBytesPerFrame    = sizeof(Float32),
		.mBytesPerPacket   = sizeof(Float32),
		.mBitsPerChannel   = sizeof(Float32) * 8
	};
    
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
    AURenderCallbackStruct callback = {soundInputStreamRenderCallback, &context};
	context.remoteIO = self.audioUnit;
	context.stream = self;
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
    
	UInt32 bufferListSize = offsetof(AudioBufferList, mBuffers[0]) + (sizeof(AudioBuffer) * numOfChannels);
    context.bufferList = (AudioBufferList *)malloc(bufferListSize);
    context.bufferList->mNumberBuffers = numOfChannels;
    
	for(int i=0; i<context.bufferList->mNumberBuffers; i++) {
        context.bufferList->mBuffers[i].mNumberChannels = 1;
        context.bufferList->mBuffers[i].mDataByteSize = bufferSize * sizeof(Float32);
        context.bufferList->mBuffers[i].mData = calloc(bufferSize, sizeof(Float32));
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
    
	for(int i=0; i<context.bufferList->mNumberBuffers; i++) {
		free(context.bufferList->mBuffers[i].mData);
	}
    free(context.bufferList);
}

@end
