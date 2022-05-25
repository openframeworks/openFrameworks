//
//  AVVideoPlayer.h
//  iOS+OFLib
//
//  Created by lukasz karluk on 21/05/12.
//

#pragma once

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <Accelerate/Accelerate.h>

@class AVPlayer;
@class AVPlayerItem;
@class AVAsset;
@class AVAssetReader;
@class AVAssetReaderOutput;

//---------------------------------------------------------- video player view.
@interface AVFoundationVideoPlayerView : UIView {
    //
}
@property (nonatomic, retain) AVPlayer * player;

@end

//---------------------------------------------------------- video player delegate.
@protocol AVFoundationVideoPlayerDelegate <NSObject>
@optional
- (void)playerReady;
- (void)playerDidProgress;
- (void)playerDidFinishSeeking;
- (void)playerDidFinishPlayingVideo;
@end

//---------------------------------------------------------- video player.
@interface AVFoundationVideoPlayer : NSObject

@property (nonatomic, weak) id<AVFoundationVideoPlayerDelegate> delegate;
@property (nonatomic, strong) UIView * playerView;
@property (nonatomic, strong) AVPlayer * player;
@property (nonatomic, strong) AVPlayerItem * playerItem;
@property (nonatomic, strong) AVAsset * asset;
@property (nonatomic, strong) AVAssetReader * assetReader;
@property (nonatomic, strong) AVAssetReaderTrackOutput * assetReaderVideoTrackOutput;
@property (nonatomic, strong) AVAssetReaderTrackOutput * assetReaderAudioTrackOutput;

- (BOOL)loadWithFile:(NSString*)file;
- (BOOL)loadWithPath:(NSString*)path;
- (BOOL)loadWithURL:(NSURL*)url;
- (void)unloadVideo;

- (void)setVideoPosition:(CGPoint)position;
- (void)setVideoSize:(CGSize)size;

- (void)update;

- (void)play;
- (void)pause;
- (void)togglePlayPause;

- (void)seekToStart;
- (void)seekToTime:(CMTime)time;
- (void)seekToTime:(CMTime)time withTolerance:(CMTime)tolerance;

- (BOOL)isReady;
- (BOOL)isPlaying;
- (BOOL)isNewFrame;
- (BOOL)isFinished;

- (void)setEnableVideoSampling:(BOOL)value;
- (void)setEnableAudioSampling:(BOOL)value;
- (void)setSynchSampleTime:(CMTime)time;
- (void)setSynchSampleTimeInSec:(double)time;
- (CMTime)getVideoSampleTime;
- (double)getVideoSampleTimeInSec;
- (CMTime)getAudioSampleTime;
- (double)getAudioSampleTimeInSec;
- (CMSampleBufferRef)getVideoSampleBuffer;
- (CMSampleBufferRef)getAudioSampleBuffer;
- (CVImageBufferRef)getCurrentFrame;

- (NSInteger)getWidth;
- (NSInteger)getHeight;
- (CMTime)getCurrentTime;
- (double)getCurrentTimeInSec;
- (CMTime)getDuration;
- (double)getDurationInSec;
- (int)getDurationInFrames;
- (int)getCurrentFrameNum;
- (float)getFrameRate;
- (void)setFrame:(int)frame;
- (void)setPosition:(float)position;
- (float)getPosition;
- (void)setVolume:(float)volume;
- (float)getVolume;
- (void)setLoop:(BOOL)bLoop;
- (BOOL)getLoop;
- (void)setSpeed:(float)speed;
- (float)getSpeed;
- (void)setAutoplay:(BOOL)bAutoplay;
- (BOOL)getAutoplay;
- (void)setWillBeUpdatedExternally:(BOOL)value;

@end
