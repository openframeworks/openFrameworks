//
//  SoundStream.m
//  Created by Lukasz Karluk on 13/06/13.
//  http://julapy.com/blog
//
//

#import "SoundStream.h"

@interface SoundStream() {
    //
}
@end

@implementation SoundStream

@synthesize delegate;
@synthesize streamType;
@synthesize numOfChannels;
@synthesize sampleRate;
@synthesize bufferSize;
@synthesize numOfBuffers;
@synthesize audioUnit;
@synthesize bInterruptedWhileRunning;

- (id)initWithNumOfChannels:(NSInteger)value0
             withSampleRate:(NSInteger)value1
             withBufferSize:(NSInteger)value2 {
    self = [super init];
    if(self) {
        numOfChannels = value0;
        sampleRate = value1;
        bufferSize = value2;
        numOfBuffers = 1; // always 1.
        audioUnit = nil;
        bInterruptedWhileRunning = NO;
    }
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)start {
    //
}

- (void)stop {
    //
}

- (BOOL)isStreaming {
    return (audioUnit != nil);
}

- (BOOL)checkStatus:(OSStatus)status {
    BOOL bSuccess = YES;
    NSString * error = @"";
    
    if(status == kAudioSessionNoError) {
        bSuccess = YES;
    } else if(status == kAudioSessionNotInitialized) {
        bSuccess = NO;
        error = @"Audio Session Not Initialized";
    } else if(status == kAudioSessionAlreadyInitialized) {
        bSuccess = NO;
        error = @"Audio Session Already Initialized";
    } else if(status == kAudioSessionInitializationError) {
        bSuccess = NO;
        error = @"Audio Session Initialization Error";
    } else if(status == kAudioSessionUnsupportedPropertyError) {
        bSuccess = NO;
        error = @"Audio Session Unsupported Property Error";
    } else if(status == kAudioSessionBadPropertySizeError) {
        bSuccess = NO;
        error = @"Audio Session Bad Property Size Error";
    } else if(status == kAudioSessionNotActiveError) {
        bSuccess = NO;
        error = @"Audio Session Not Active Error";
    } else if(status == kAudioServicesNoHardwareError) {
        bSuccess = NO;
        error = @"Audio Services No Hardware Error";
    } else if(status == kAudioSessionNoCategorySet) {
        bSuccess = NO;
        error = @"Audio Session No Category Set";
    } else if(status == kAudioSessionIncompatibleCategory) {
        bSuccess = NO;
        error = @"Audio Session Incompatible Category";
    } else if(status == kAudioSessionUnspecifiedError) {
        bSuccess = NO;
        error = @"Audio Session Unspecified Error";
    } else {
        bSuccess = NO;
        error = @"Audio Session Unspecified Error";
    }

    if(bSuccess) {
        return YES;
    } else {
        if([self.delegate respondsToSelector:@selector(soundStreamError:error:)]) {
            [self.delegate soundStreamError:self
                                      error:error];
        }
    }
    return NO;
}

@end
