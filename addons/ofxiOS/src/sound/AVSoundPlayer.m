//
//  AVSoundPlayer.m
//  Created by lukasz karluk on 14/06/12.
//  http://julapy.com/blog
//

#import "AVSoundPlayer.h"
#include <TargetConditionals.h>

@interface AVSoundPlayer() {
    BOOL bMultiPlay;
}
@end

@implementation AVSoundPlayer

- (instancetype)init {
    self = [super init];
    if(self) {
        bMultiPlay = NO;
    }
    return self;
}

// setupSharedSession is to prevent other iOS Classes closing the audio feed, such as AVAssetReader, when reading from disk
// It is set once on first launch of a AVAudioPlayer and remains as a set property from then on
- (void) setupSharedSession {
	static BOOL audioSessionSetup = NO;
	if(audioSessionSetup) {
		return;
	}
	NSString * playbackCategory = AVAudioSessionCategoryPlayAndRecord;
#ifdef TARGET_OS_TV
	playbackCategory = AVAudioSessionCategoryPlayback;
#endif
	[[AVAudioSession sharedInstance] setCategory:playbackCategory error: nil];
    AVAudioSession * audioSession = [AVAudioSession sharedInstance];
    NSError * err = nil;
    // need to configure set the audio category, and override to it route the audio to the speaker
    if([audioSession respondsToSelector:@selector(setCategory:withOptions:error:)]) {
#if defined (TARGET_OS_TV) || defined (TARGET_OS_WATCH)
        if(![audioSession setCategory:playbackCategory
                                       withOptions:(AVAudioSessionCategoryOptionMixWithOthers)
                                error:&err]) { err = nil; }
#else
        if(![audioSession setCategory:playbackCategory
                                       withOptions:(AVAudioSessionCategoryOptionMixWithOthers |
                                                   AVAudioSessionCategoryOptionAllowAirPlay |
                                                   AVAudioSessionCategoryOptionAllowBluetooth |
                                                   AVAudioSessionCategoryOptionAllowBluetoothA2DP)
                                        error:&err]) { err = nil; }
#endif
    }
	[[AVAudioSession sharedInstance] setActive: YES error: nil];
	audioSessionSetup = YES;
}

- (void)dealloc {
    [self unloadSound];
}

//----------------------------------------------------------- load / unload.
- (BOOL)loadWithFile:(NSString*)file {
    NSArray * fileSplit = [file componentsSeparatedByString:@"."];
    NSURL * fileURL = [[NSBundle mainBundle] URLForResource:[fileSplit objectAtIndex:0] 
                                              withExtension:[fileSplit objectAtIndex:1]];
	return [self loadWithURL:fileURL];
}

- (BOOL)loadWithPath:(NSString*)path {
    NSURL * fileURL = [NSURL fileURLWithPath:path];
	return [self loadWithURL:fileURL];
}

- (BOOL)loadWithURL:(NSURL*)url {
    [self unloadSound];
	[self setupSharedSession];
    NSError * error = nil;
    self.player = [[AVAudioPlayer alloc] initWithContentsOfURL:url
                                                         error:&error];
    if([self.player respondsToSelector:@selector(setEnableRate:)]) {
        [self.player setEnableRate:YES];
    }
    [self.player prepareToPlay];
    if(error) {
        if([self.delegate respondsToSelector:@selector(soundPlayerError:)]) {
            [self.delegate soundPlayerError:error];
        }
        return NO;
    }
    
    self.player.delegate = self;
    return YES;
}

- (void)unloadSound {
    [self stop];
    self.player.delegate = nil;
    self.player = nil;
}

//----------------------------------------------------------- play / pause / stop.
- (void)play {
    if([self isPlaying]) {
        [self position:0];
        return;
    }
    BOOL bOk = [self.player play];
    if(bOk) {
        [self startTimer];
    }
}

- (void)pause {
    [self.player pause];
    [self stopTimer];
}

- (void)stop {
    [self.player stop];
    [self stopTimer];
}

//----------------------------------------------------------- states.
- (BOOL)isLoaded {
    return (self.player != nil);
}

- (BOOL)isPlaying {
    if(self.player == nil) {
        return NO;
    }
    return self.player.isPlaying;
}

//----------------------------------------------------------- properties.
- (void)volume:(float)value {
    self.player.volume = value;
}

- (float)volume {
    if(self.player == nil) {
        return 0;
    }
    return self.player.volume;
}

- (void)pan:(float)value {
    self.player.pan = value;
}

- (float)pan {
    if(self.player == nil) {
        return 0;
    }
    return self.player.pan;
}

- (void)speed:(float)value {
    if(value < 0.5) { // min play speed is 0.5 and max speed is 2.0 as per apple docs.
        value = 0.5;
    } else if(value > 2.0) {
        value = 2.0;
    }
    self.player.rate = value;
}

- (float)speed {
    if(self.player == nil) {
        return 0;
    }
    return self.player.rate;
}

- (void)loop:(BOOL)bLoop {
    if(bLoop) {
        self.player.numberOfLoops = -1;
    } else {
        self.player.numberOfLoops = 0;
    }
}

- (BOOL)loop {
    return self.player.numberOfLoops < 0;
}

- (void)multiPlay:(BOOL)value {
    bMultiPlay = value;
}

- (BOOL)multiPlay {
    return bMultiPlay;
}

- (void)position:(float)value {
    self.player.currentTime = value * self.player.duration;
}

- (float)position {
    if(self.player == nil) {
        return 0;
    }
    return self.player.currentTime / (float)self.player.duration;
}

- (void)positionMs:(int)value {
    self.player.currentTime = value / 1000.0;
}

- (int)positionMs {
    if(self.player == nil) {
        return 0;
    }
    return self.player.currentTime * 1000;
}

//----------------------------------------------------------- timer.
- (void)updateTimer {
    if([self.delegate respondsToSelector:@selector(soundPlayerDidChange)]) {
        [self.delegate soundPlayerDidChange];
    }
}

- (void)stopTimer {
    [self.timer invalidate];
    self.timer = nil;
}

- (void)startTimer {
    [self stopTimer];
	self.timer = [NSTimer scheduledTimerWithTimeInterval:1.0/30.0
                                                  target:self 
                                                selector:@selector(updateTimer) 
                                                userInfo:nil 
                                                 repeats:YES];
}

//----------------------------------------------------------- audio player events.
- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player 
                                 error:(NSError *)error {
    if([self.delegate respondsToSelector:@selector(soundPlayerError:)]) {
        [self.delegate soundPlayerError:error];
    }
}

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player 
                       successfully:(BOOL)flag {
    [self stopTimer];
    
    if([self.delegate respondsToSelector:@selector(soundPlayerDidFinish)]) {
        [self.delegate soundPlayerDidFinish];
    }
}

- (void) audioPlayerEndInterruption:(AVAudioPlayer *)player withFlags:(NSUInteger)flags {
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
	if(flags == AVAudioSessionInterruptionFlags_ShouldResume) {
		[self.player play];
	}
#elif TARGET_OS_TV
	//
#endif
}

@end
