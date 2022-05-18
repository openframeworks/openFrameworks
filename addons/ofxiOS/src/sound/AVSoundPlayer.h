//
//  AVSoundPlayer.h
//  Created by lukasz karluk on 14/06/12.
//  http://julapy.com/blog
//

#pragma once

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@protocol AVSoundPlayerDelegate <NSObject>
@optional
- (void)soundPlayerDidChange;
- (void)soundPlayerDidFinish;
- (void)soundPlayerError:(NSError *)error;
@end

@interface AVSoundPlayer : NSObject <AVAudioPlayerDelegate> 

@property(nonatomic, weak) id<AVSoundPlayerDelegate> delegate;
@property(nonatomic, strong) AVAudioPlayer * player;
@property(nonatomic, strong) NSTimer * timer;

- (BOOL)loadWithFile:(NSString*)file;
- (BOOL)loadWithPath:(NSString*)path;
- (BOOL)loadWithURL:(NSURL*)url;
- (void)unloadSound;

- (void)play;
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

@end
