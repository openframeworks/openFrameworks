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

void soundOutputStreamInterruptionListener(void *inRefCon, UInt32 inInterruptionState) {
    SoundOutputStream * stream = (SoundOutputStream *)inRefCon;
    
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

static OSStatus soundOutputStreamRenderCallback(void *inRefCon,
                                                AudioUnitRenderActionFlags *ioActionFlags,
                                                const AudioTimeStamp *inTimeStamp,
                                                UInt32 inBusNumber,
                                                UInt32 inNumberFrames,
                                                AudioBufferList *ioData) {

    SoundOutputStream * stream = (SoundOutputStream *)inRefCon;
    float * scaleBuffer = stream.scaleBuffer;
    
    AudioBuffer * audioBuffer = &ioData->mBuffers[0];
    short int * buffer = (short int *)audioBuffer->mData;
    
    int numOfChannels = audioBuffer->mNumberChannels;
    int maxBufferSize = MAX_BUFFER_SIZE / numOfChannels;
    int bufferSize = audioBuffer->mDataByteSize / (numOfChannels * 2);
    bufferSize = MIN(bufferSize, maxBufferSize);
    
    if([stream.delegate respondsToSelector:@selector(soundStreamRequested:output:bufferSize:numOfChannels:)]) {
        [stream.delegate soundStreamRequested:stream
                                       output:scaleBuffer
                                   bufferSize:bufferSize
                                numOfChannels:numOfChannels];
    }
    
    int len = bufferSize * numOfChannels;
    for(int i=0; i<len; i++) { // truncate to 16bit fixed point data
        buffer[i] = (int)(scaleBuffer[i] * 32767.f);
    }
	
    return noErr;
}

//----------------------------------------------------------------
@interface SoundOutputStream() {
    //
}
@end

@implementation SoundOutputStream

- (id)initWithNumOfChannels:(NSInteger)value0
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
    [super dealloc];
}

- (void)start {
    [super start];
    
    if([self isStreaming] == YES) {
        return; // already running.
    }
    
	OSStatus status = NULL;
    status = AudioSessionInitialize(NULL, NULL, soundOutputStreamInterruptionListener, self);
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
                                  kAudioUnitScope_Input,
                                  kOutputBus,
                                  &audioFormat,
                                  sizeof(AudioStreamBasicDescription));
	if([self checkStatus:status] == NO) {
        //
    }
    
    //---------------------------------------------------------- callback.
    
    // render callback
	AURenderCallbackStruct callback;
	callback.inputProc = soundOutputStreamRenderCallback;
	callback.inputProcRefCon = self;
	status = AudioUnitSetProperty(audioUnit,
                                  kAudioUnitProperty_SetRenderCallback,
                                  kAudioUnitScope_Global,
                                  kOutputBus,
                                  &callback,
                                  sizeof(callback));
	if([self checkStatus:status] == NO) {
        //
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
}

@end
