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
#import <AVFoundation/AVFoundation.h>

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
												input:(float *)bufferList->mBuffers[0].mData
										   bufferSize:bufferList->mBuffers[0].mDataByteSize / sizeof(Float32)
										numOfChannels:bufferList->mBuffers[0].mNumberChannels];
    }
	
	bufferList->mBuffers[0].mDataByteSize = bufferSize;
    
	return noErr;
}

//----------------------------------------------------------------
@implementation SoundInputStream

- (instancetype)initWithNumOfChannels:(NSInteger)value0
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
}

- (void)start {
    [super start];
    
    if([self isStreaming] == YES) {
        return; // already running.
    }
	
	[self configureAudioSession];
    
    //---------------------------------------------------------- audio session category config.
    
	AVAudioSession * audioSession = [AVAudioSession sharedInstance];
	NSError * err = nil;
	
	// need to configure set the audio category, and override to it route the audio to the speaker
	if([audioSession respondsToSelector:@selector(setCategory:withOptions:error:)]) {
		// we're on iOS 6 or greater, so use the AVFoundation API
		if(![audioSession setCategory:AVAudioSessionCategoryPlayAndRecord
						  withOptions:AVAudioSessionCategoryOptionMixWithOthers
								error:&err]) {
			[self reportError:err];
			err = nil;
		}
	} else {
 
	}

    
    //---------------------------------------------------------- audio unit.
    
	// Configure the search parameters to find the default playback output unit
	// (called the kAudioUnitSubType_RemoteIO on iOS but
	// kAudioUnitSubType_DefaultOutput on Mac OS X)
	AudioComponentDescription desc = {
		.componentType = kAudioUnitType_Output,
		.componentSubType = kAudioUnitSubType_RemoteIO,
		.componentManufacturer = kAudioUnitManufacturer_Apple
	};
    
    // get component and get audio units.
	AudioComponent inputComponent = AudioComponentFindNext(NULL, &desc);
	[self checkStatus:AudioComponentInstanceNew(inputComponent, &audioUnit)];
    
    //---------------------------------------------------------- enable io.

    UInt32 on = 1;
    UInt32 off = 0;
    
    // enable input to AudioUnit.
	[self checkStatus:AudioUnitSetProperty(audioUnit,
										   kAudioOutputUnitProperty_EnableIO,
										   kAudioUnitScope_Input,
										   kInputBus,
										   &on,
										   sizeof(on))];
    
    // enable output out of AudioUnit.
	[self checkStatus:AudioUnitSetProperty(audioUnit,
										   kAudioOutputUnitProperty_EnableIO,
										   kAudioUnitScope_Output,
										   kOutputBus,
										   &on,
										   sizeof(on))];
    
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
	[self checkStatus:AudioUnitSetProperty(audioUnit,
										   kAudioUnitProperty_StreamFormat,
										   kAudioUnitScope_Output,
										   kInputBus,
										   &audioFormat,
										   sizeof(audioFormat))];
    
    //---------------------------------------------------------- callback.
    
    // input callback
    AURenderCallbackStruct callback = {soundInputStreamRenderCallback, &context};
	context.remoteIO = self.audioUnit;
	context.stream = self;
	[self checkStatus:AudioUnitSetProperty(audioUnit,
										   kAudioOutputUnitProperty_SetInputCallback,
										   kAudioUnitScope_Global,
										   kInputBus,
										   &callback,
										   sizeof(callback))];
    
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
    
	[self checkStatus:AudioUnitInitialize(audioUnit)];
    [self checkStatus:AudioOutputUnitStart(audioUnit)];
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
    
	for(int i=0; i<context.bufferList->mNumberBuffers; i++) {
		free(context.bufferList->mBuffers[i].mData);
	}
    free(context.bufferList);
}

@end
