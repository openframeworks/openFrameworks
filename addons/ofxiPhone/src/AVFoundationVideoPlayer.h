//
//  AVVideoPlayer.h
//  iOS+OFLib
//
//  Created by lukasz karluk on 21/05/12.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@class AVPlayer;
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
- (void)playerReady;
- (void)playerDidProgress;
- (void)playerDidFinishPlayingVideo;
@end

//---------------------------------------------------------- video player.
@interface AVFoundationVideoPlayer : NSObject {
    id<AVFoundationVideoPlayerDelegate> delegate;
}

@property (nonatomic, assign) id delegate;
@property (nonatomic, retain) UIView * playerView;
@property (nonatomic, retain) AVPlayer * player;
@property (nonatomic, retain) AVAsset * asset;
@property (nonatomic, retain) AVAssetReader * assetReader;
@property (nonatomic, retain) AVAssetReaderOutput * assetReaderVideoOutput;

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

- (CVImageBufferRef)getCurrentFrame;

- (NSInteger)getWidth;
- (NSInteger)getHeight;
- (CMTime)getCurrentTime;
- (double)getCurrentTimeInSec;
- (CMTime)getDuration;
- (double)getDurationInSec;
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
