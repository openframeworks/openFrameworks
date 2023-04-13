//
//  ofAVEngineSoundPlayer.cpp
//  soundPlayerExample
//
//  Created by Theo Watson on 3/24/21.
//  Modified by Dan Rosser 9/5/22

#include "ofAVEngineSoundPlayer.h"

#ifdef OF_SOUND_PLAYER_AV_ENGINE

#include "ofUtils.h"
#include "ofMath.h"
#include "ofLog.h"

//REFS: https://github.com/ooper-shlab/AVAEMixerSample-Swift/blob/master/AVAEMixerSample/AudioEngine.m
// https://developer.apple.com/documentation/avfaudio/avaudioengine
// https://developer.apple.com/forums/thread/14138
// https://developer.apple.com/forums/thread/48442
// https://github.com/garynewby/XYAudioView/blob/master/XYAudioView/BasicAVAudioEngine.m
// https://github.com/twilio/video-quickstart-ios/blob/master/AudioDeviceExample/AudioDevices/ExampleAVAudioEngineDevice.m

static BOOL audioSessionSetup = NO;
static AVAudioEngine * _engine = nullptr;

static NSString *kShouldEnginePauseNotification = @"kShouldEnginePauseNotification";

@interface AVEnginePlayer : NSObject

@property(nonatomic, retain) NSTimer * timer;

- (BOOL)loadWithFile:(NSString*)file;
- (BOOL)loadWithPath:(NSString*)path;
- (BOOL)loadWithURL:(NSURL*)url;
- (BOOL)loadWithSoundFile:(AVAudioFile*)aSoundFile;

- (void)unloadSound;

- (void)play;
- (void)play: (float)startTime;
- (void)pause;
- (void)stop;

- (BOOL)isLoaded;
- (BOOL)isPlaying;

- (void)volume:(float)value;
- (float)volume;

- (void)pan:(float)value;
- (float)pan;

- (void)speed:(float)value;
- (float)speed;

- (void)loop:(BOOL)value;
- (BOOL)loop;

- (void)multiPlay:(BOOL)value;
- (BOOL)multiPlay;

- (void)position:(float)value;
- (float)position;

- (void)positionMs:(int)value;
- (int)positionMs;

- (float)positionSeconds;
- (float)soundDurationSeconds;


- (void)sessionInterupted;

- (AVAudioFile *)getSoundFile;

- (AVAudioEngine *)engine;

- (void)beginInterruption; /* something has caused your audio session to be interrupted */

- (void)endInterruption; /* endInterruptionWithFlags: will be called instead if implemented. */

/* notification for input become available or unavailable */
- (void)inputIsAvailableChanged:(BOOL)isInputAvailable;

@end


@interface AVEnginePlayer ()

//@property(nonatomic, strong) AVAudioEngine *engine;
@property(nonatomic, strong) AVAudioMixerNode *mainMixer;
@property(nonatomic, strong) AVAudioUnitVarispeed *variSpeed;
@property(nonatomic, strong) AVAudioPlayerNode *soundPlayer;
@property(nonatomic, strong) AVAudioFile *soundFile;
@property(nonatomic, assign) bool mShouldLoop;
@property(nonatomic, assign) BOOL bInterruptedWhileRunning;
@property(nonatomic, assign) bool bIsPlaying;
@property(nonatomic, assign) int mGaurdCount;
@property(nonatomic, assign) int mRestorePlayCount;
@property(nonatomic, assign) bool mMultiPlay;
@property(nonatomic, assign) bool isSessionInterrupted;
@property(nonatomic, assign) bool isConfigChangePending;
@property(nonatomic, assign) float mRequestedPositonSeconds;
@property(nonatomic, assign) AVAudioFramePosition startedSampleOffset;

@property(nonatomic, assign) bool mPlayingAtInterruption;
@property(nonatomic, assign) float mPositonSecondsAtInterruption;

@property(nonatomic, assign) BOOL resetAudioEngine;

@end

@implementation AVEnginePlayer

@synthesize timer;

- (AVAudioEngine *) engine {

  if( _engine == nullptr ){
    @autoreleasepool {
        _engine = [[AVAudioEngine alloc] init];
    }
    self.resetAudioEngine = NO;
  }
  
  return _engine;
}


- (void) engineReconnect {
    NSLog(@"engineReconnect");
    
    if( [self engine] != nil && [[self engine] isRunning] ){
        NSLog(@"engineReconnect isRunning");
    } else {
        NSLog(@"engineReconnect is NOT Running");
    }
    if([self engine]) {
        BOOL found = NO;
        for(AVAudioPlayerNode* node in [self engine].attachedNodes) {
            if(node == self.soundPlayer) {
                break;
            }
        }
        if(found) {
            NSLog(@"engineReconnect found Node AVAudioPlayerNode - Disconnecting");
            [[self engine] detachNode:self.soundPlayer];
        }
        found = NO;
        for(AVAudioUnitVarispeed* node in [self engine].attachedNodes) {
            if(node == self.variSpeed) {
                break;
            }
        }
        if(found) {
            NSLog(@"engineReconnect found Node  variSpeed- Disconnecting");
            [[self engine] detachNode:self.variSpeed];
        }
    }
}

- (void) engineReset {
    if( [self engine] != nil && [[self engine] isRunning] ){
        NSLog(@"engineReset isRunning");
    } else {
        NSLog(@"engineReset is NOT Running");
    }
    if([self engine] && [[self engine] isRunning]) {
        [_engine stop];
        self.resetAudioEngine = NO;
    }
    @autoreleasepool {
        if(_engine != nil) {
            _engine = nil;
        }
    }
    [self engine];
}


- (void)sessionInterupted {
    self.isSessionInterrupted = YES;
}


/* the interruption is over */
- (void)endInterruptionWithFlags:(NSUInteger)flags API_AVAILABLE(ios(4.0), watchos(2.0), tvos(9.0)) { /* Currently the only flag is AVAudioSessionInterruptionFlags_ShouldResume. */
    NSLog(@"AVEnginePlayer::endInterruptionWithFlags");
    if(flags == AVAudioSessionInterruptionTypeBegan) {
        [self beginInterruption];
    } else if(flags == AVAudioSessionInterruptionTypeEnded) {
        [self endInterruption];
    }
}

/* notification for input become available or unavailable */
- (void)inputIsAvailableChanged:(BOOL)isInputAvailable {
    NSLog(@"AVEnginePlayer::inputIsAvailableChanged");
}

// setupSharedSession is to prevent other iOS Classes closing the audio feed, such as AVAssetReader, when reading from disk
// It is set once on first launch of a AVAudioPlayer and remains as a set property from then on
- (void) setupSharedSession {
#ifndef TARGET_OSX
    if(audioSessionSetup) {
            return;
    }
    NSString * playbackCategory = AVAudioSessionCategoryPlayback;
	
    AVAudioSession * audioSession = [AVAudioSession sharedInstance];
    NSError * err = nil;
  
    
    if(![audioSession setCategory:playbackCategory
                                                      withOptions:AVAudioSessionCategoryOptionInterruptSpokenAudioAndMixWithOthers
                                    error:&err]) {
        
        NSLog(@"Unable to setCategory: withOptions error %@, %@", err, [err userInfo]);
        err = nil;
        
    }
    
    if(![[AVAudioSession sharedInstance] setActive: YES withOptions:AVAudioSessionSetActiveOptionNotifyOthersOnDeactivation error: &err]) {
        NSLog(@"Unable to setActive: error %@, %@", err, [err userInfo]);
        err = nil;
    }
    
    double hwSampleRate = 44100.0;
    BOOL success = [[AVAudioSession sharedInstance] setPreferredSampleRate:hwSampleRate error:&err];
        if (!success) NSLog(@"Error setting preferred sample rate! %@\n", [err localizedDescription]);
    
    
    audioSessionSetup = YES;
#endif
}

- (instancetype)init
{
    self = [super init];
    if (self) {
		[self setupSharedSession];
		NSError * error = nil;
        
        _mainMixer = [[self engine] mainMixerNode];
		_mainMixer.outputVolume = 0.98;
						
		_mShouldLoop = false;
		_mGaurdCount = 0;
		_mMultiPlay = false;
        _bIsPlaying = false;
        _isSessionInterrupted = NO;
		_mRequestedPositonSeconds = 0.0f;
		_startedSampleOffset = 0;
        _bInterruptedWhileRunning = NO;
        _resetAudioEngine = NO;
        _mPlayingAtInterruption = NO;
        _mPositonSecondsAtInterruption = 0;
        _isConfigChangePending = NO;
		

#ifndef TARGET_OSX

		//from: https://github.com/robovm/apple-ios-samples/blob/master/UsingAVAudioEngineforPlaybackMixingandRecording/AVAEMixerSample/AudioEngine.m
   
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(handleInterruption:)
                                                     name:AVAudioSessionInterruptionNotification
                                                   object:[AVAudioSession sharedInstance]];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(handleRouteChange:)
                                                     name:AVAudioSessionRouteChangeNotification
                                                   object:[AVAudioSession sharedInstance]];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(handleMediaServicesReset:)
                                                     name:AVAudioSessionMediaServicesWereLostNotification
                                                   object:[AVAudioSession sharedInstance]];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(handleMediaServicesReset:)
                                                     name:AVAudioSessionMediaServicesWereResetNotification
                                                   object:[AVAudioSession sharedInstance]];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(handleInterruption:)
                                                     name:AVAudioSessionSilenceSecondaryAudioHintNotification
                                                   object:[AVAudioSession sharedInstance]];
        
        
        
        if (@available(iOS 14.5, *)) {
            if(![[AVAudioSession sharedInstance] setPrefersNoInterruptionsFromSystemAlerts:YES error:&error]) {
                NSLog(@"Unable to setPrefersNoInterruptionsFromSystemAlerts: error %@, %@", error, [error userInfo]);
                error = nil;
            }
        } else {
            // Fallback on earlier versions
        }
        
#endif
        
        __typeof(self) __weak weak_self = self;

        [[NSNotificationCenter defaultCenter] addObserverForName:kShouldEnginePauseNotification object:nil queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *note) {
                   
                   /* pausing stops the audio engine and the audio hardware, but does not deallocate the resources allocated by prepare().
                      When your app does not need to play audio, you should pause or stop the engine (as applicable), to minimize power consumption.
                   */
                    bool isPlaying = [weak_self isPlaying] || weak_self.bIsPlaying == YES;
                   if (!weak_self.isSessionInterrupted && !weak_self.isConfigChangePending) {
                       
                       
                       NSLog(@"Pausing Engine");
                       [[weak_self engine] pause];
                       [[weak_self engine] reset];
                       
                   }
               }];
		        
        // sign up for notifications from the engine if there's a hardware config change
        [[NSNotificationCenter defaultCenter] addObserverForName:AVAudioEngineConfigurationChangeNotification object:nil queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *note) {
        
            weak_self.resetAudioEngine = YES;
            
            NSLog(@"Received a AVAudioEngineConfigurationChangeNotification %@ notification! NOTE: %@", AVAudioEngineConfigurationChangeNotification, note.description);
			
            bool isPlaying = [weak_self isPlaying] || weak_self.bIsPlaying == YES;
            float posSecs = [weak_self positionSeconds];
            
            weak_self.mPlayingAtInterruption = isPlaying;
            weak_self.mPositonSecondsAtInterruption = posSecs;
            weak_self.bInterruptedWhileRunning = YES;
            
            [weak_self engineReconnect];
            
            weak_self.isConfigChangePending = YES;
                       
           if (!weak_self.isSessionInterrupted) {
               NSLog(@"Received a %@ notification!", AVAudioEngineConfigurationChangeNotification);
               NSLog(@"Re-wiring connections");
               [weak_self makeEngineConnections];
           } else {
               NSLog(@"Session is interrupted, deferring changes");
           }
            
            
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.033f), dispatch_get_main_queue(), ^{
                [weak_self handleInterruption:note];
            });
            
        }];
        
        

    }
        
    return self;
}

- (void) handleMediaServicesReset:(NSNotification *)notification {
    
    NSUInteger interruptionType = [notification.userInfo[AVAudioSessionInterruptionTypeKey] unsignedIntegerValue];
    
        NSLog(@"Media services have been reset!");
       NSLog(@"Re-wiring connections and starting once again");
      
    // Re-configure the audio session per QA1749
       audioSessionSetup = NO;
       [self setupSharedSession];
    
    [self engineReset];
    
    if(_mainMixer != nil) {
        _mainMixer = nil;
    }
    [self engine];
    

    [self attachNodes];
    [self makeEngineConnections];
    
    
    [self startEngine];
    
}

- (void) handleRouteChange:(NSNotification *)notification {
    
    NSUInteger interruptionType = [notification.userInfo[AVAudioSessionInterruptionTypeKey] unsignedIntegerValue];
    
    UInt8 reasonValue = [[notification.userInfo valueForKey:AVAudioSessionRouteChangeReasonKey] intValue];
    
#ifndef TARGET_OSX
        AVAudioSessionRouteDescription *routeDescription = [notification.userInfo valueForKey:AVAudioSessionRouteChangePreviousRouteKey];
#endif
    
        NSLog(@"Route change:");
        switch (reasonValue) {
            case AVAudioSessionRouteChangeReasonNewDeviceAvailable:
                NSLog(@"     NewDeviceAvailable");
                break;
            case AVAudioSessionRouteChangeReasonOldDeviceUnavailable:
                NSLog(@"     OldDeviceUnavailable");
                break;
            case AVAudioSessionRouteChangeReasonCategoryChange:
                NSLog(@"     CategoryChange");
#ifndef TARGET_OSX
                NSLog(@"     New Category: %@", [[AVAudioSession sharedInstance] category]);
#endif
                break;
            case AVAudioSessionRouteChangeReasonOverride:
                NSLog(@"     Override");
                break;
            case AVAudioSessionRouteChangeReasonWakeFromSleep:
                NSLog(@"     WakeFromSleep");
                break;
            case AVAudioSessionRouteChangeReasonNoSuitableRouteForCategory:
                NSLog(@"     NoSuitableReasonForCategory");
                break;
            default:
                NSLog(@"     ReasonUnknown");
        }
    
#ifndef TARGET_OSX
        NSLog(@"Previous route:\n");
        NSLog(@"%@", routeDescription);
#endif
}

- (void) handleInterruption:(NSNotification *)notification {
    
    NSUInteger interruptionType = [notification.userInfo[AVAudioSessionInterruptionTypeKey] unsignedIntegerValue];

    
    NSLog(@"AVEnginePlayer::handleInterruption: notification:%@ %@ interruptionType: %lu", notification.name, notification.description, (unsigned long)interruptionType);
       

    if(interruptionType == AVAudioSessionInterruptionTypeBegan) {
        [self beginInterruption];
    } else if(interruptionType == AVAudioSessionInterruptionTypeEnded) {
        [self endInterruption];
        
        [self startEngine];
    }

}

- (void)beginInterruption {
    
    NSLog(@"AVEnginePlayer::beginInterruption");
    
    _isSessionInterrupted = YES;
    
    if([self isPlaying] || _bIsPlaying == YES) {
        self.bInterruptedWhileRunning = YES;
    }
    
    if([self isValid]) {
        [self.soundPlayer stop];
    }
    
//    if([self.delegate respondsToSelector:@selector(soundStreamBeginInterruption:)]) {
//        [self.delegate soundStreamBeginInterruption:self];
//    }
}


- (void) attachNodes {
    if( self.variSpeed == nullptr ){
        // Speed manipulator
        @autoreleasepool {
            self.variSpeed = [[AVAudioUnitVarispeed alloc] init];
        }
        self.variSpeed.rate = 1.0;

    }
    
    if( self.soundPlayer == nil ){
        // Sound player
        @autoreleasepool {
            self.soundPlayer = [[AVAudioPlayerNode alloc] init];
        }
    }
    
    if(self.soundPlayer != nil && self.variSpeed != nil) {
        [[self engine] attachNode:self.soundPlayer];
        [[self engine] attachNode:self.variSpeed];
    }
}

- (void) makeEngineConnections {
    _mainMixer = [[self engine] mainMixerNode];
    
    AVAudioFormat * stereoFormat;
    @autoreleasepool {
       stereoFormat = [[AVAudioFormat alloc] initStandardFormatWithSampleRate:44100 channels:2];
    }
    if(self.soundPlayer != nil) {
        [[self engine] connect:self.soundPlayer to:self.variSpeed format:stereoFormat];
    }
    if(self.variSpeed != nil) {
        [[self engine] connect:self.variSpeed to:self.mainMixer format:stereoFormat];
    }
}

- (void)endInterruption {
    NSLog(@"AVEnginePlayer::endInterruption");
    
    NSError *error;
#ifndef TARGET_OSX
    bool success = [[AVAudioSession sharedInstance] setActive:YES error:&error];
    if (!success)
        NSLog(@"AVAudioSession set active failed with error: %@", [error localizedDescription]);
    else {
#endif
        _isSessionInterrupted = NO;
        if (_isConfigChangePending) {
            // there is a pending config changed notification
            NSLog(@"Responding to earlier engine config changed notification. Re-wiring connections");
            [self startEngine];
            [self makeEngineConnections];
            
            _isConfigChangePending = NO;
        }
#ifndef TARGET_OSX
    }
#endif
//    [self engineReconnect];
    

    
    
    if(self.bInterruptedWhileRunning || _bIsPlaying == YES) {
        self.bInterruptedWhileRunning = NO;
        bool isPlaying = [self isPlaying] || _bIsPlaying == YES;
        float posSecs = [self positionSeconds] > 0 ? [self positionSeconds] : _mPositonSecondsAtInterruption;
        
//                std::cout << " isPlaying is " << isPlaying << " pos secs is " << posSecs << std::endl;
        
        
        if( isPlaying && posSecs >= 0 && posSecs < ([self soundDurationSeconds] + 0.017f) && self.mRestorePlayCount == 0){
            self.mRestorePlayCount++;
            
        __typeof(self) __weak weak_self = self;
    
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.017f), dispatch_get_main_queue(), ^{
                weak_self.mRestorePlayCount--;
                if(weak_self.bIsPlaying == NO) return;
                [weak_self play:posSecs];
            });
        }
    }
    
//    if([self.delegate respondsToSelector:@selector(soundStreamEndInterruption:)]) {
//        [self.delegate soundStreamEndInterruption:self];
//    }
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
    [self stop];

	NSError *error;
    @autoreleasepool {
        self.soundFile = [[AVAudioFile alloc] initForReading:url error:&error];
    }
    if (error) {
        NSLog(@"Error: %@", [error localizedDescription]);
        self.soundFile = nil;
		return NO;
    }else{
		NSLog(@"Sound file %@ loaded!", url);
	}
	
	return [self loadWithSoundFile:self.soundFile];
}

- (void)startEngine{
    
    NSError * error = nil;
    BOOL engineRunning = NO;
    BOOL problem = NO;
    if( ![[self engine] isRunning] ){
        [[self engine] startAndReturnError:&error];
        if (error) {
            NSLog(@"Error starting engine: %@", [error localizedDescription]);
//            if(self.resetAudioEngine) {
//                [self engineReset];
//            }
            problem = YES;
            
        } else {
            NSLog(@"Engine start successful");
            if(self.resetAudioEngine) {
//                [self engineReset];
                if(self.resetAudioEngine == NO)
                    problem = YES;
                
            }
            engineRunning = YES;
//            [self engineReconnect];
        }
    }else{
//        NSLog(@"Engine already running");
        engineRunning = YES;
    }
    
    if( self.variSpeed == nullptr ){
        // Speed manipulator
        @autoreleasepool {
            self.variSpeed = [[AVAudioUnitVarispeed alloc] init];
        }
        self.variSpeed.rate = 1.0;
        problem = YES;
    }
    
    if( self.soundPlayer == nil ){
        // Sound player
        @autoreleasepool {
            self.soundPlayer = [[AVAudioPlayerNode alloc] init];
        }
        problem = YES;
    }

    if(_mainMixer != [[self engine] mainMixerNode]) {
        _mainMixer = [[self engine] mainMixerNode];
        problem = YES;
    }
    
    if(problem == YES) {
        //[[self engine] reset];
        //NSLog(@"Engine start successful - re-attaching nodes");
        [[self engine] attachNode:self.variSpeed];
        [[self engine] attachNode:self.soundPlayer];
        
        [[self engine]  connect:self.variSpeed to:self.mainMixer format:[_mainMixer outputFormatForBus:0]];
        [[self engine]  connect:self.soundPlayer to:self.variSpeed format:[_mainMixer outputFormatForBus:0]];
    }
}

- (BOOL)loadWithSoundFile:(AVAudioFile *)aSoundFile {
    [self stop];

    self.soundFile = aSoundFile;
	
	[self startEngine];
	
	self.mGaurdCount=0;
	self.mRequestedPositonSeconds = 0;
	self.startedSampleOffset = 0;
	self.mRestorePlayCount =0;
	
    return YES;
}

- (void)dealloc {
    [self unloadSound];
}

- (BOOL)isValid {
    if(self.soundPlayer != nil && self.soundPlayer.engine != nil) {
        return YES;
    }
    return NO;
}

- (void)unloadSound {
    [self stop];
    if([self isValid]) {
        [[self engine] detachNode:self.soundPlayer];
    }
    self.soundPlayer = nil;
    self.soundFile = nil;
    self.variSpeed = nil;
}

- (void)play{
    [self stop];
    self.mRequestedPositonSeconds = 0.0;
    [self play:self.mRequestedPositonSeconds];
}

- (void)playloop{
    self.mRequestedPositonSeconds = 0.0;
    [self play:self.mRequestedPositonSeconds];
}

//----------------------------------------------------------- play / pause / stop.
- (void)play:(float)startTime{
    if([self isPlaying]) {
        [self pause];
    }
    if(self.soundPlayer == nil) {
        NSLog(@"play - soundPlayer is null");
        return;
    }
    if(_isSessionInterrupted || _isConfigChangePending){
        __typeof(self) __weak weak_self = self;
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.3f), dispatch_get_main_queue(), ^{
            [weak_self play:startTime];
        });
        return;
    }
    
    if( self.engine == nil || ![[self engine] isRunning] ){
        NSLog(@"play - engine is null or not running - starting");
        [self startEngine];
    }
    
    if( self.engine == nil || ![[self engine] isRunning] ){
        NSLog(@"play - engine is NULL or not running");
        return;
    }
    BOOL found = NO;
    
    for(AVAudioPlayerNode* node in [self engine].attachedNodes) {
        if(node == self.soundPlayer) {
            found = YES;
            break;
        }
    }
    
    if(self.soundPlayer.engine == nil || found == NO) {
        
        if(found == NO) {
            NSLog(@"play - engine is valid - however NO attachedNode for soundPlayer found - reseting");
        } else {
            NSLog(@"play - engine is valid - however soundPlayer.engine is NULL - reseting");
        }
        _mainMixer = [[self engine] mainMixerNode];
        
        [[self engine] attachNode:self.soundPlayer];
        [[self engine] connect:self.soundPlayer to:self.variSpeed format:[_mainMixer outputFormatForBus:0]];
        
        [[self engine] attachNode:self.variSpeed];
        [[self engine] connect:self.variSpeed to:self.mainMixer format:[_mainMixer outputFormatForBus:0]];
    }
    
    //we do this as we can't cancel completion handlers.
    //and queded completion handlers can interupt a playing track if we have retriggered it
    //so we basically do this to execute only the last completion handler ( the one for the current playing track ), and ignore the earlier ones.
	self.mGaurdCount++;
    _mPositonSecondsAtInterruption = 0;
    _isSessionInterrupted = NO;
    _mPlayingAtInterruption = NO;
    self.mRestorePlayCount = 0;
	
	self.startedSampleOffset = self.soundFile.processingFormat.sampleRate * startTime;
	AVAudioFramePosition numFramesToPlay = self.soundFile.length - self.startedSampleOffset;
    const float epsilon = 0.0000001f;
	if( startTime <= epsilon){
		self.startedSampleOffset = 0;
		numFramesToPlay = self.soundFile.length;
	}
	
//	std::cout << " startedSampleOffset is " << self.startedSampleOffset <<  " numFrames is " << numFramesToPlay << " self.mGaurdCount is " << self.mGaurdCount << std::endl;
	
	self.mRestorePlayCount = 0;
    
   
	
    [self.soundPlayer play];
    _bIsPlaying = YES;
    
        __typeof(self) __weak weak_self = self;

	[self.soundPlayer scheduleSegment:self.soundFile startingFrame:self.startedSampleOffset frameCount:numFramesToPlay atTime:0 completionHandler:^{
	
		dispatch_async(dispatch_get_main_queue(), ^{
			weak_self.mGaurdCount--;
			
            if(weak_self.bIsPlaying == NO) return;
            
			//std::cout << " need gaurd is " << self.mGaurdCount << std::endl;

			if( weak_self.mGaurdCount == 0 ){
				float time = [weak_self positionSeconds];
				
				float duration = [weak_self soundDurationSeconds];
				
				//have to do all this because the completion handler fires before the player is actually finished - which isn't very helpful
				float remainingTime = duration-time;
				if( remainingTime < 0 ){
					remainingTime = 0;
				}

				//all the other time stuff accounts for the speed / rate, except the remaining time delay
				remainingTime /= ofClamp(self.variSpeed.rate, 0.01, 100.0);
                
				if( weak_self.mShouldLoop ){
                    [NSObject cancelPreviousPerformRequestsWithTarget:weak_self selector:@selector(playloop) object: weak_self.soundPlayer];
					[weak_self performSelector:@selector(playloop) withObject:self.soundPlayer afterDelay:remainingTime];
				}else{
                    [NSObject cancelPreviousPerformRequestsWithTarget:weak_self selector:@selector(stop) object: weak_self.soundPlayer];
					[weak_self performSelector:@selector(stop) withObject:weak_self.soundPlayer afterDelay:remainingTime];
				}
                
//                NSLog(@"play - scheduleSegment mShouldLoop:%i - mGaurdCount:%i", _mShouldLoop, _mGaurdCount);
            } else {
//                NSLog(@"play - scheduleSegment - mGaurdCount:%i", _mGaurdCount);
            }
			
			if( weak_self.mGaurdCount < 0 ){
				weak_self.mGaurdCount=0;
			}
			
		});
		
	}];
}

- (AVAudioFile *)getSoundFile{
	return self.soundFile;
}

- (void)pause {
    _bIsPlaying = NO;
    if([self isValid]) {
        [self.soundPlayer pause];
    }
    [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(playloop) object: self.soundPlayer];
    [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(stop) object: self.soundPlayer];
}

- (void)stop {
    
    __typeof(self) __weak weak_self = self;

    if(_isSessionInterrupted || _isConfigChangePending){
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.3f), dispatch_get_main_queue(), ^{
            [weak_self stop];
        });
        return;
    }
    
    if([self isValid]) {
        [self.soundPlayer stop];
    }
    _bIsPlaying = NO;
   
    [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(playloop) object: self.soundPlayer];
    [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(stop) object: self.soundPlayer];

    self.startedSampleOffset = 0;
}

//----------------------------------------------------------- states.
- (BOOL)isLoaded {
    return (self.soundPlayer != nil);
}

- (BOOL)isPlaying {
    if(![self isValid]) return NO;
    return self.soundPlayer.isPlaying;
}

//----------------------------------------------------------- properties.
- (void)volume:(float)value {
    if(![self isValid]) return;
    self.soundPlayer.volume = value;
}

- (float)volume {
    if(![self isValid]) return 0;
    return self.soundPlayer.volume;
}

- (void)pan:(float)value {
    if(![self isValid]) return;
    self.soundPlayer.pan = value;
}

- (float)pan {
    if(![self isValid]) return 0;
    return self.soundPlayer.pan;
}

- (void)speed:(float)value {
    if([self isValid]) {
		self.variSpeed.rate = value;
    }
}

- (float)speed {
    if(![self isValid]) return 0;
    return self.variSpeed.rate;
}

- (void)loop:(BOOL)bLoop {
    self.mShouldLoop = bLoop;
}

- (BOOL)loop {
    return self.mShouldLoop;
}

- (void)multiPlay:(BOOL)value {
    self.mMultiPlay = value;
}

- (BOOL)multiPlay {
    return self.mMultiPlay;
}

- (void)position:(float)value {
	if( [self isLoaded] ){
		self.mRequestedPositonSeconds = ofClamp(value, 0, 1) * [self soundDurationSeconds];
		
		if( [self isPlaying] ){
			[self play];
		}
	}
}

- (float)position {
    if(self.soundPlayer == nil) {
        return 0;
    }
    
    if( [self isPlaying] ){
		float time = [self positionSeconds];
		float duration = [self soundDurationSeconds];
		
		if( duration > 0 ){
			float pct = ofClamp(time/duration, 0, 1);
			//NSLog(@"time is %f out of %f pct is %f", time, duration, pct );
			return pct;
		}
	}
	return 0;
}

- (void)positionMs:(int)value {
	if( [self isLoaded] ){
        float oldDuration = [self positionSeconds];
		self.mRequestedPositonSeconds = ofClamp(((float)value)/1000.0, 0, [self soundDurationSeconds]);
        
        NSLog(@"positionMS: from: %f toNewPos: %f", oldDuration, self.mRequestedPositonSeconds );
		
		if( [self isPlaying] ){
			[self play];
		}
	}
}

- (int)positionMs {
	float timeSeconds = [self positionSeconds];
	return timeSeconds/1000.0;
}

- (float)positionSeconds{
	if( [self isPlaying] && self.variSpeed != nil && self.engine != nil){
		AVAudioTime * playerTime =  [self.soundPlayer playerTimeForNodeTime:self.soundPlayer.lastRenderTime];
        float time = 0;
        if(playerTime != nil)
            time =((self.startedSampleOffset + playerTime.sampleTime) / playerTime.sampleRate);
		return time;
	}
	return 0.0;
}

- (float)soundDurationSeconds{
	if( [self isLoaded] && self.variSpeed != nil && self.engine != nil){
        float duration = 0.0;
        if(self.soundFile.processingFormat != nil)
            duration = self.soundFile.length / self.soundFile.processingFormat.sampleRate;
		return duration;
	}
	return 0.0;
}

@end



using namespace std;

ofAVEngineSoundPlayer::ofAVEngineSoundPlayer() {
    soundPlayer = NULL;
}

ofAVEngineSoundPlayer::~ofAVEngineSoundPlayer() {
    unload();
}

bool ofAVEngineSoundPlayer::load(const std::filesystem::path& fileName, bool stream) {
    if(soundPlayer != NULL) {
        unload();
    }

    string filePath = ofToDataPath(fileName);
    @autoreleasepool {
        soundPlayer = [[AVEnginePlayer alloc] init];
    }
    BOOL bOk = [(AVEnginePlayer *)soundPlayer loadWithPath:[NSString stringWithUTF8String:filePath.c_str()]];
    
    return bOk;
}

void ofAVEngineSoundPlayer::unload() {
    if(soundPlayer != NULL) {
        
        [(AVEnginePlayer *)soundPlayer unloadSound];
        @autoreleasepool {
            soundPlayer = nil;
        }
    }
    if( bAddedUpdate ){
        ofRemoveListener(ofEvents().update, this, &ofAVEngineSoundPlayer::updateFunction);
        bAddedUpdate = false;
    }
    cleanupMultiplayers();
}

void ofAVEngineSoundPlayer::play() {
    if(soundPlayer == NULL) {
        return;
    }
    
    auto mainPlayer = (AVEnginePlayer *)soundPlayer;
    if( [mainPlayer multiPlay] && [mainPlayer isPlaying] ){
            
        AVEnginePlayer * extraPlayer;
        @autoreleasepool {
           extraPlayer = [[AVEnginePlayer alloc] init];
        }
        BOOL bOk = [extraPlayer loadWithSoundFile:[mainPlayer getSoundFile]];
        if( bOk ){
                [extraPlayer speed:[mainPlayer speed]];
                [extraPlayer pan:[mainPlayer pan]];
                [extraPlayer volume:[mainPlayer volume]];
                [extraPlayer play];
                
                mMultiplayerSoundPlayers.push_back(extraPlayer);
                
                if( !bAddedUpdate ){
                    ofAddListener(ofEvents().update, this, &ofAVEngineSoundPlayer::updateFunction);
                    bAddedUpdate = true;
                }
        }
            
    }else{
        [(AVEnginePlayer *)soundPlayer play];
    }
}

void ofAVEngineSoundPlayer::cleanupMultiplayers(){
	for( auto mMultiPlayer : mMultiplayerSoundPlayers ){
		if( mMultiPlayer != NULL ){
			[(AVEnginePlayer *)mMultiPlayer stop];
			[(AVEnginePlayer *)mMultiPlayer unloadSound];
                        @autoreleasepool {
                            mMultiPlayer = nil;
                        }
		}
	}
	mMultiplayerSoundPlayers.clear();
}

bool ofAVEngineSoundPlayer::removeMultiPlayer(void * aPlayer){
	return( aPlayer == NULL );
}

//better do do this in a thread?
//feels safer to use ofEvents().update so we don't need to lock.
void ofAVEngineSoundPlayer::updateFunction( ofEventArgs & args ){
		
	vector <ObjectType> playerPlayingList;

	for( auto mMultiPlayerPtr : mMultiplayerSoundPlayers ){
            if( mMultiPlayerPtr != NULL ){
                if( [(AVEnginePlayer *)mMultiPlayerPtr isLoaded] && [(AVEnginePlayer *)mMultiPlayerPtr isPlaying] ){
                    playerPlayingList.push_back(mMultiPlayerPtr);
                }else{
                    [(AVEnginePlayer *)mMultiPlayerPtr unloadSound];
                    @autoreleasepool {
                        mMultiPlayerPtr = nil;
                    }
                }
            }
	}
	
	mMultiplayerSoundPlayers = playerPlayingList;
}

void ofAVEngineSoundPlayer::stop() {
    if(soundPlayer == NULL) {
        return;
    }
    [(AVEnginePlayer *)soundPlayer stop];
	cleanupMultiplayers();
}

void ofAVEngineSoundPlayer::setVolume(float value) {
    if(soundPlayer == NULL) {
        return;
    }
    [(AVEnginePlayer *)soundPlayer volume:value];
}

void ofAVEngineSoundPlayer::setPan(float value) {
    if(soundPlayer == NULL) {
        return;
    }
    [(AVEnginePlayer *)soundPlayer pan:value];
}

void ofAVEngineSoundPlayer::setSpeed(float value) {
    if(soundPlayer == NULL) {
        return;
    }
    [(AVEnginePlayer *)soundPlayer speed:value];
}

void ofAVEngineSoundPlayer::setPaused(bool bPause) {
    if(soundPlayer == NULL) {
        return;
    }
    if(bPause) {
        [(AVEnginePlayer *)soundPlayer pause];
    } else {
        [(AVEnginePlayer *)soundPlayer play];
    }
}

void ofAVEngineSoundPlayer::setLoop(bool bLoop) {
    if(soundPlayer == NULL) {
        return;
    }
    [(AVEnginePlayer *)soundPlayer loop:bLoop];
}

void ofAVEngineSoundPlayer::setMultiPlay(bool bMultiPlay) {
    if(soundPlayer == NULL) {
        return;
    }
    [(AVEnginePlayer *)soundPlayer multiPlay:bMultiPlay];
}

void ofAVEngineSoundPlayer::setPosition(float position) {
    if(soundPlayer == NULL) {
        return;
    }
    [(AVEnginePlayer *)soundPlayer position:position];
}

void ofAVEngineSoundPlayer::setPositionMS(int positionMS) {
    if(soundPlayer == NULL) {
        return;
    }
    [(AVEnginePlayer *)soundPlayer positionMs:positionMS];
}

float ofAVEngineSoundPlayer::getPosition()  const{
    if(soundPlayer == NULL) {
        return 0;
    }
    return [(AVEnginePlayer *)soundPlayer position];
}

int ofAVEngineSoundPlayer::getPositionMS() const {
    if(soundPlayer == NULL) {
        return 0;
    }
    return [(AVEnginePlayer *)soundPlayer positionMs];
}

bool ofAVEngineSoundPlayer::isPlaying()  const{
    if(soundPlayer == NULL) {
        return false;
    }
    
    bool bMainPlaying = [(AVEnginePlayer *)soundPlayer isPlaying];
    if( !bMainPlaying && mMultiplayerSoundPlayers.size() ){
		return true;
	}
	
	return bMainPlaying;
}

float ofAVEngineSoundPlayer::getSpeed()  const{
    if(soundPlayer == NULL) {
        return 0;
    }
    return [(AVEnginePlayer *)soundPlayer speed];
}

float ofAVEngineSoundPlayer::getPan()  const{
    if(soundPlayer == NULL) {
        return 0;
    }
    return [(AVEnginePlayer *)soundPlayer pan];
}

bool ofAVEngineSoundPlayer::isLoaded()  const{
    if(soundPlayer == NULL) {
        return false;
    }
    return [(AVEnginePlayer *)soundPlayer isLoaded];
}

float ofAVEngineSoundPlayer::getVolume()  const{
    if(soundPlayer == NULL) {
        return false;
    }
    return [(AVEnginePlayer *)soundPlayer volume];
}

void * ofAVEngineSoundPlayer::getAVEnginePlayer() {
    return (__bridge void *)soundPlayer;
}

#endif
