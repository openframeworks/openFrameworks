//
//  SoundOutputStream.m
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

#import "SoundOutputStream.h"

static OSStatus soundOutputStreamRenderCallback(void *inRefCon,
                                                AudioUnitRenderActionFlags *ioActionFlags,
                                                const AudioTimeStamp *inTimeStamp,
                                                UInt32 inBusNumber,
                                                UInt32 inNumberFrames,
                                                AudioBufferList *ioData) {

    SoundOutputStream * stream = (__bridge SoundOutputStream *)inRefCon;
    AudioBuffer * audioBuffer = &ioData->mBuffers[0];
	
	// clearing the buffer before handing it off to the user
	// this saves us from horrible noises if the user chooses not to write anything
	memset(audioBuffer->mData, 0, audioBuffer->mDataByteSize);
    
    int bufferSize = (audioBuffer->mDataByteSize / sizeof(Float32)) / audioBuffer->mNumberChannels;
    bufferSize = MIN(bufferSize, MAX_BUFFER_SIZE / audioBuffer->mNumberChannels);
    
    if([stream.delegate respondsToSelector:@selector(soundStreamRequested:output:bufferSize:numOfChannels:)]) {
        [stream.delegate soundStreamRequested:stream
                                       output:(float*)audioBuffer->mData
                                   bufferSize:bufferSize
                                numOfChannels:audioBuffer->mNumberChannels];
    }
	
    return noErr;
}

//----------------------------------------------------------------
@interface SoundOutputStream() {
    //
}
@end

@implementation SoundOutputStream

- (instancetype)initWithNumOfChannels:(NSInteger)value0
                       withSampleRate:(NSInteger)value1
                       withBufferSize:(NSInteger)value2 {
    self = [super initWithNumOfChannels:value0
                         withSampleRate:value1
                         withBufferSize:value2];
    if(self) {
        streamType = SoundStreamTypeOutput;
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
    
    //---------------------------------------------------------- audio unit.
	
	// Configure the search parameters to find the default playback output unit
	// (called the kAudioUnitSubType_RemoteIO on iOS but
	// kAudioUnitSubType_DefaultOutput on Mac OS X)
	AudioComponentDescription desc = {
		.componentType         = kAudioUnitType_Output,
		.componentSubType      = kAudioUnitSubType_RemoteIO,
		.componentManufacturer = kAudioUnitManufacturer_Apple
	};
    
    // get component and get audio units.
	AudioComponent inputComponent = AudioComponentFindNext(NULL, &desc);
	[self checkStatus:AudioComponentInstanceNew(inputComponent, &audioUnit)];
    
    //---------------------------------------------------------- enable io.
    
    // enable output out of AudioUnit.
	UInt32 on = 1;
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
		.mBytesPerFrame    = sizeof(Float32) * numOfChannels,
		.mBytesPerPacket   = sizeof(Float32) * numOfChannels,
		.mBitsPerChannel   = sizeof(Float32) * 8
	};
    
    // Apply format
	[self checkStatus:AudioUnitSetProperty(audioUnit,
										   kAudioUnitProperty_StreamFormat,
										   kAudioUnitScope_Input,
										   kOutputBus,
										   &audioFormat,
										   sizeof(AudioStreamBasicDescription))];
    
    //---------------------------------------------------------- render callback.
    
    AURenderCallbackStruct callback = {soundOutputStreamRenderCallback, (__bridge void * _Nullable)(self)};
	[self checkStatus:AudioUnitSetProperty(audioUnit,
										   kAudioUnitProperty_SetRenderCallback,
										   kAudioUnitScope_Global,
										   kOutputBus,
										   &callback,
										   sizeof(callback))];
     
    //---------------------------------------------------------- go!
    
	[self checkStatus:AudioUnitInitialize(audioUnit)];
    [self checkStatus:AudioOutputUnitStart(audioUnit)];
}

- (void)stop {
    [super stop];
    
    if([self isStreaming] == NO) {
        return;
    }
    
    [self checkStatus:AudioOutputUnitStop(audioUnit)];
    [self checkStatus:AudioUnitUninitialize(audioUnit)];
    [self checkStatus:AudioComponentInstanceDispose(audioUnit)];
    audioUnit = nil;
}

@end
