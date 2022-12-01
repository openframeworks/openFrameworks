//
//  SoundStream.m
//  Created by Lukasz Karluk on 13/06/13.
//  http://julapy.com/blog
//
//

#import "SoundStream.h"
#import <AVFoundation/AVFoundation.h>

@interface SoundStream() {
    //
}
@end

@implementation SoundStream

@synthesize streamType;
@synthesize numOfChannels;
@synthesize sampleRate;
@synthesize bufferSize;
@synthesize numOfBuffers;
@synthesize audioUnit;
@synthesize bInterruptedWhileRunning;

- (instancetype)initWithNumOfChannels:(NSInteger)value0
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
		
		
		if([SoundStream shouldUseAudioSessionNotifications]) {
			[[NSNotificationCenter defaultCenter] addObserver:self
													 selector:@selector(handleInterruption:)
														 name:AVAudioSessionInterruptionNotification
													   object:nil];
		} else {
			
		}
    
    }
    return self;
}

- (void)dealloc {
    self.delegate = nil;
	if([SoundStream shouldUseAudioSessionNotifications]) {
		[[NSNotificationCenter defaultCenter] removeObserver:self
														name:AVAudioSessionInterruptionNotification
													  object:nil];
	} else {
		
    }
}

- (void)start {
	
}

- (void)stop {
	[[AVAudioSession sharedInstance] setActive:NO error:nil];
}

- (BOOL)isStreaming {
    return (audioUnit != nil);
}

#pragma mark - Audio Session Config

- (void)configureAudioSession {
	NSError * audioSessionError = nil;
	AVAudioSession * audioSession = [AVAudioSession sharedInstance];
	
	if(![audioSession setActive:YES error:&audioSessionError]) {
		[self reportError:audioSessionError];
		
		// if we can't even activate the session, we better abort early
		return;
	}
	
	// setting sample rate (this has different selectors for iOS 5- and iOS 6+)
	double trueSampleRate = sampleRate;
	if([audioSession respondsToSelector:@selector(setPreferredSampleRate:error:)]) {
		if(![audioSession setPreferredSampleRate:sampleRate error:&audioSessionError]) {
			[self reportError:audioSessionError];
			audioSessionError = nil;
		}
		trueSampleRate = [audioSession sampleRate];
	} 
	sampleRate = trueSampleRate;
	
	// setting buffer size
	NSTimeInterval bufferDuration = bufferSize / trueSampleRate;
	if(![audioSession setPreferredIOBufferDuration:bufferDuration error:&audioSessionError]) {
		[self reportError:audioSessionError];
		audioSessionError = nil;
	}
}

#pragma mark - Interruptions

- (void) handleInterruption:(NSNotification *)notification {
    #ifdef __IPHONE_6_0
        NSUInteger interruptionType = [notification.userInfo[AVAudioSessionInterruptionTypeKey] unsignedIntegerValue];
        
        if(interruptionType == AVAudioSessionInterruptionTypeBegan) {
            [self beginInterruption];
        } else if(interruptionType == AVAudioSessionInterruptionTypeEnded) {
            [self endInterruption];
        }
    #endif
}

- (void)beginInterruption {
	if([self isStreaming]) {
		self.bInterruptedWhileRunning = YES;
	}
	[self stop];
	
	if([self.delegate respondsToSelector:@selector(soundStreamBeginInterruption:)]) {
		[self.delegate soundStreamBeginInterruption:self];
	}
}

- (void)endInterruption {
	if(self.bInterruptedWhileRunning) {
		self.bInterruptedWhileRunning = NO;
		[self start];
	}
	
	if([self.delegate respondsToSelector:@selector(soundStreamEndInterruption:)]) {
		[self.delegate soundStreamEndInterruption:self];
	}
}

// iOS 5- needs a delegate for Audio Session interruptions, iOS 6+ can use notifications
+ (BOOL) shouldUseAudioSessionNotifications {
	return [[[UIDevice currentDevice] systemVersion] floatValue] >= 6;
}

#pragma mark - Error Handling

- (BOOL)checkStatus:(OSStatus)status {
    if(status == noErr) {
        return YES;
    } else if([self.delegate respondsToSelector:@selector(soundStreamError:error:)]) {
		NSString * errorCode = [self stringForAudioUnitError:status];
		NSString * fullErrorString = [errorCode stringByAppendingFormat:@" (%i)", (int)status];
		[self.delegate soundStreamError:self error:fullErrorString];
	}
    return NO;
}

- (NSString *)stringForAudioUnitError:(OSStatus)status {
	if(status == kAudioUnitErr_InvalidProperty) {
		return @"kAudioUnitErr_InvalidProperty";
	} else if(status == kAudioUnitErr_InvalidParameter) {
		return @"kAudioUnitErr_InvalidParameter";
	} else if(status == kAudioUnitErr_InvalidElement) {
		return @"kAudioUnitErr_InvalidElement";
	} else if(status == kAudioUnitErr_NoConnection) {
		return @"kAudioUnitErr_NoConnection";
	} else if(status == kAudioUnitErr_FailedInitialization) {
		return @"kAudioUnitErr_FailedInitialization";
	} else if(status == kAudioUnitErr_TooManyFramesToProcess) {
		return @"kAudioUnitErr_TooManyFramesToProcess";
	} else if(status == kAudioUnitErr_InvalidFile) {
		return @"kAudioUnitErr_InvalidFile";
	} else if(status == kAudioUnitErr_FormatNotSupported) {
		return @"kAudioUnitErr_FormatNotSupported";
	} else if(status == kAudioUnitErr_Uninitialized) {
		return @"kAudioUnitErr_Uninitialized";
	} else if(status == kAudioUnitErr_InvalidScope) {
		return @"kAudioUnitErr_InvalidScope";
	} else if(status == kAudioUnitErr_PropertyNotWritable) {
		return @"kAudioUnitErr_PropertyNotWritable";
	} else if(status == kAudioUnitErr_CannotDoInCurrentContext) {
		return @"kAudioUnitErr_CannotDoInCurrentContext";
	} else if(status == kAudioUnitErr_InvalidPropertyValue) {
		return @"kAudioUnitErr_InvalidPropertyValue";
	} else if(status == kAudioUnitErr_PropertyNotInUse) {
		return @"kAudioUnitErr_PropertyNotInUse";
	} else if(status == kAudioUnitErr_Initialized) {
		return @"kAudioUnitErr_Initialized";
	} else if(status == kAudioUnitErr_InvalidOfflineRender) {
		return @"kAudioUnitErr_InvalidOfflineRender";
	} else if(status == kAudioUnitErr_Unauthorized) {
		return @"kAudioUnitErr_Unauthorized";
	} else {
		return @"Unknown";
	}
}

- (void) reportError:(NSError *)error {
	if(error && [self.delegate respondsToSelector:@selector(soundStreamError:error:)]) {
		[self.delegate soundStreamError:self error:[error localizedDescription]];
	}
}

@end
