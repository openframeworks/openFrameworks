//
//  ofAVEngineSoundPlayer.cpp
//  soundPlayerExample
//
//  Created by Theo Watson on 3/24/21.
//

#include "ofAVEngineSoundPlayer.h"
#include "ofUtils.h"
#include "ofMath.h"
#include "ofLog.h"

#define TARGET_OSX

//REFS: https://github.com/ooper-shlab/AVAEMixerSample-Swift/blob/master/AVAEMixerSample/AudioEngine.m
// https://developer.apple.com/documentation/avfaudio/avaudioengine
// https://developer.apple.com/forums/thread/14138
// https://developer.apple.com/forums/thread/48442
// https://github.com/garynewby/XYAudioView/blob/master/XYAudioView/BasicAVAudioEngine.m
// https://github.com/twilio/video-quickstart-ios/blob/master/AudioDeviceExample/AudioDevices/ExampleAVAudioEngineDevice.m

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface AVEnginePlayer : NSObject

@property(nonatomic, retain) NSTimer * timer;

- (BOOL)loadWithFile:(NSString*)file;
- (BOOL)loadWithPath:(NSString*)path;
- (BOOL)loadWithURL:(NSURL*)url;

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

@end


@interface AVEnginePlayer ()

@property(nonatomic, strong) AVAudioEngine *engine;
@property(nonatomic, strong) AVAudioMixerNode *mainMixer;
@property(nonatomic, strong) AVAudioUnitVarispeed *variSpeed;
@property(nonatomic, strong) AVAudioPlayerNode *soundPlayer;
@property(nonatomic, strong) AVAudioFile *soundFile;
@property(nonatomic, assign) bool mShouldLoop;
@property(nonatomic, assign) int mGaurdCount;
@property(nonatomic, assign) bool mMultiPlay;
@property(nonatomic, assign) float mRequestedPositonSeconds;
@property(nonatomic, assign) AVAudioFramePosition startedSampleOffset;


@end

@implementation AVEnginePlayer

@synthesize timer;

// setupSharedSession is to prevent other iOS Classes closing the audio feed, such as AVAssetReader, when reading from disk
// It is set once on first launch of a AVAudioPlayer and remains as a set property from then on
- (void) setupSharedSession {
#ifndef TARGET_OSX
	static BOOL audioSessionSetup = NO;
	if(audioSessionSetup) {
		return;
	}
	NSString * playbackCategory = AVAudioSessionCategoryPlayAndRecord;
#ifdef TARGET_OF_TVOS
	playbackCategory = AVAudioSessionCategoryPlayback;
#endif
	[[AVAudioSession sharedInstance] setCategory:playbackCategory error: nil];
    AVAudioSession * audioSession = [AVAudioSession sharedInstance];
    NSError * err = nil;
    // need to configure set the audio category, and override to it route the audio to the speaker
    if([audioSession respondsToSelector:@selector(setCategory:withOptions:error:)]) {
        if(![audioSession setCategory:playbackCategory
             						  withOptions:AVAudioSessionCategoryOptionMixWithOthers
                                        error:&err]) { err = nil; }
    }
	[[AVAudioSession sharedInstance] setActive: YES error: nil];
	audioSessionSetup = YES;
#endif
}

- (instancetype)init
{
    self = [super init];
    if (self) {
		[self setupSharedSession];
		NSError * error = nil;
		
		// Engine
		_engine = [[AVAudioEngine alloc] init];
				
		_mainMixer = [_engine mainMixerNode];
		_mainMixer.outputVolume = 1;
						
        //NSLog(@"%@", _engine.description);

		_mShouldLoop = false;
		_mGaurdCount = 0;
		_mMultiPlay = false;
		_mRequestedPositonSeconds = 0.0f;
		_startedSampleOffset = 0;
    }
        
    return self;
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

	NSError *error;
    self.soundFile = [[AVAudioFile alloc] initForReading:url error:&error];
    if (error) {
		return NO;
        NSLog(@"Error: %@", [error localizedDescription]);
    }else{
		NSLog(@"Sound file %@ loaded!", url);
	}
	
	if (!self.engine.isRunning) {

		// Speed manipulator
		self.variSpeed = [[AVAudioUnitVarispeed alloc] init];
		self.variSpeed.rate = 1.0;
		[self.engine attachNode:self.variSpeed];
		[self.engine connect:self.variSpeed to:self.mainMixer fromBus:0 toBus:0  format:self.soundFile.processingFormat];
		
		// Sound player
		self.soundPlayer = [[AVAudioPlayerNode alloc] init];
		[self.engine attachNode:_soundPlayer];
		[self.engine connect:self.soundPlayer to:self.variSpeed format:self.soundFile.processingFormat];
		
		[self.engine startAndReturnError:&error];
		if (error) {
			NSLog(@"Error starting engine: %@", [error localizedDescription]);
		} else {
			NSLog(@"Engine start successful");
		}
	}
	
	self.mGaurdCount=0;
	self.mRequestedPositonSeconds = 0;
	self.startedSampleOffset = 0;

	
    return YES;
}

- (void)dealloc {
    [self unloadSound];
    [super dealloc];
}

- (void)unloadSound {
    [self stop];
	if (self.engine.isRunning) {
		[self.engine stop];
		self.soundPlayer = nil;
		self.soundFile = nil;
		self.variSpeed = nil;
    }
}

- (void)play{
	[self play:self.mRequestedPositonSeconds];
	self.mRequestedPositonSeconds = 0.0;
}

//----------------------------------------------------------- play / pause / stop.
- (void)play:(float)startTime{
    if([self isPlaying]) {
        [self stop];
    }
    
    //we do this as we can't cancel completion handlers.
    //and queded completion handlers can interupt a playing track if we have retriggered it
    //so we basically do this to execute only the last completion handler ( the one for the current playing track ), and ignore the earlier ones.
	self.mGaurdCount++;
	
	self.startedSampleOffset = self.soundFile.processingFormat.sampleRate * startTime;
	AVAudioFramePosition numFramesToPlay = self.soundFile.length - self.startedSampleOffset;
	
//	AVAudioTime * playTime =  [[AVAudioTime alloc] initWithSampleTime:sampleTime atRate:self.soundFile.processingFormat.sampleRate];
	
	if( startTime == 0.0){
		self.startedSampleOffset = 0;
		numFramesToPlay = self.soundFile.length;
	}
	
    [self.soundPlayer play];
	[self.soundPlayer scheduleSegment:self.soundFile startingFrame:self.startedSampleOffset frameCount:numFramesToPlay atTime:0 completionHandler:^{
	
		dispatch_async(dispatch_get_main_queue(), ^{
			self.mGaurdCount--;
			
			//std::cout << " need gaurd is " << self.mGaurdCount << std::endl;

			if( self.mGaurdCount == 0 ){
				float time = [self positionSeconds];
				
				float duration = [self soundDurationSeconds];
				
				//have to do all this because the completion handler fires before the player is actually finished - which isn't very helpful
				float remainingTime = duration-time;
				if( remainingTime < 0 ){
					remainingTime = 0;
				}

				//all the other time stuff accounts for the speed / rate, except the remaining time delay
				remainingTime /= ofClamp(self.variSpeed.rate, 0.01, 100.0);
				
				if( self.mShouldLoop ){
					[self performSelector:@selector(play) withObject:nil afterDelay:remainingTime];
				}else{
					[self performSelector:@selector(stop) withObject:nil afterDelay:remainingTime];
				}
			}
			
			if( self.mGaurdCount < 0 ){
				self.mGaurdCount=0;
			}
			
		});
		
	}];
}

- (void)pause {
    [self.soundPlayer pause];
}

- (void)stop {
    [self.soundPlayer stop];
    self.startedSampleOffset = 0;
}

//----------------------------------------------------------- states.
- (BOOL)isLoaded {
    return (self.soundPlayer != nil);
}

- (BOOL)isPlaying {
    if(self.soundPlayer == nil) {
        return NO;
    }
    return self.soundPlayer.isPlaying;
}

//----------------------------------------------------------- properties.
- (void)volume:(float)value {
    self.soundPlayer.volume = value;
}

- (float)volume {
    if(self.soundPlayer == nil) {
        return 0;
    }
    return self.soundPlayer.volume;
}

- (void)pan:(float)value {
    self.soundPlayer.pan = value;
}

- (float)pan {
    if(self.soundPlayer == nil) {
        return 0;
    }
    return self.soundPlayer.pan;
}

- (void)speed:(float)value {
	if( self.variSpeed != nil ){
		self.variSpeed.rate = value;
    }
}

- (float)speed {
    if(self.soundPlayer == nil) {
        return 0;
    }
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
//    self.soundPlayer.currentTime = value * self.soundPlayer.duration;
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
		self.mRequestedPositonSeconds = ofClamp(((float)value)/1000.0, 0, [self soundDurationSeconds]);
		
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
	if( [self isPlaying] ){
		AVAudioTime * playerTime =  [self.soundPlayer playerTimeForNodeTime:self.soundPlayer.lastRenderTime];
		float time = ((self.startedSampleOffset + playerTime.sampleTime) / playerTime.sampleRate);
		return time;
	}
	return 0.0;
}

- (float)soundDurationSeconds{
	if( [self isLoaded] ){
		float duration = self.soundFile.length / self.soundFile.processingFormat.sampleRate;
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
    soundPlayer = [[AVEnginePlayer alloc] init];
    BOOL bOk = [(AVEnginePlayer *)soundPlayer loadWithPath:[NSString stringWithUTF8String:filePath.c_str()]];
    
    return bOk;
}

void ofAVEngineSoundPlayer::unload() {
    if(soundPlayer != NULL) {
        [(AVEnginePlayer *)soundPlayer unloadSound];
        [(AVEnginePlayer *)soundPlayer release];
        soundPlayer = NULL;
    }
}

void ofAVEngineSoundPlayer::play() {
    if(soundPlayer == NULL) {
        return;
    }
    [(AVEnginePlayer *)soundPlayer play];
}

void ofAVEngineSoundPlayer::stop() {
    if(soundPlayer == NULL) {
        return;
    }
    [(AVEnginePlayer *)soundPlayer stop];
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
    ofLogNotice("ofAVEngineSoundPlayer") << "setMultiPlay(): sorry, no support for multiplay streams";
    if(soundPlayer == NULL) {
        return;
    }
    //[(AVEnginePlayer *)soundPlayer multiPlay:bMultiPlay];
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
    return [(AVEnginePlayer *)soundPlayer isPlaying];
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
    return soundPlayer;
}
