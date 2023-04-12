//
//  SoundStream.h
//  Created by Lukasz Karluk on 13/06/13.
//  http://julapy.com/blog
//
//

#pragma once

#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>

#define MAX_BUFFER_SIZE 4096

#define kOutputBus 0
#define kInputBus 1

typedef enum {
    SoundStreamTypeOutput = 0,
    SoundStreamTypeInput = 1
} SoundStreamType;

@protocol SoundStreamDelegate <NSObject>
@optional
- (void)soundStreamRequested:(id)sender output:(float *)output bufferSize:(NSInteger)bufferSize numOfChannels:(NSInteger)numOfChannels;
- (void)soundStreamReceived:(id)sender input:(float *)input bufferSize:(NSInteger)bufferSize numOfChannels:(NSInteger)numOfChannels;
- (void)soundStreamBeginInterruption:(id)sender;
- (void)soundStreamEndInterruption:(id)sender;
- (void)soundStreamError:(id)sender error:(NSString *)error;
@end

@interface SoundStream : NSObject {
    SoundStreamType streamType;
    NSInteger numOfChannels;
    NSInteger sampleRate;
    NSInteger bufferSize;
    NSInteger numOfBuffers;
    AudioUnit audioUnit;
    BOOL bInterruptedWhileRunning;
}

@property (nonatomic, strong) id<SoundStreamDelegate> delegate;
@property (readonly) SoundStreamType streamType;
@property (readonly) NSInteger numOfChannels;
@property (readonly) NSInteger sampleRate;
@property (readonly) NSInteger bufferSize;
@property (readonly) NSInteger numOfBuffers;
@property (readonly) AudioUnit audioUnit;
@property (assign) BOOL bInterruptedWhileRunning;

- (instancetype)initWithNumOfChannels:(NSInteger)numOfChannels
                       withSampleRate:(NSInteger)sampleRate
                       withBufferSize:(NSInteger)bufferSize;

- (void)start;
- (void)stop;
- (BOOL)isStreaming;

- (BOOL)checkStatus:(OSStatus)status;
- (void)reportError:(NSError *)error;

- (void)configureAudioSession;
+ (BOOL)shouldUseAudioSessionNotifications;

@end
