//
//  ofAVFoundationVideoPlayer.h
//  Created by Lukasz Karluk on 06/07/14.
//	Merged with code by Sam Kronick, James George and Elie Zananiri.
//

//----------------------------------------------------------
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <Accelerate/Accelerate.h>
#import <CoreMedia/CoreMedia.h>


//----------------------------------------------------------
#include <TargetConditionals.h>
#if (TARGET_OS_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
#define TARGET_IOS
#else
#define TARGET_OSX
#endif


#define USE_VIDEO_OUTPUT (defined(MAC_OS_X_VERSION_10_8) || defined(iOS6))

// so we are independend from oF in this class
typedef enum _playerLoopType{
    LOOP_NONE=0x01,
    LOOP_PALINDROME=0x02,
    LOOP_NORMAL=0x03
} playerLoopType;


//---------------------------------------------------------- video player.
@interface ofAVFoundationVideoPlayer : NSObject {
	
    AVPlayer * _player;
	AVAsset * _asset;
    AVPlayerItem * _playerItem;
	
	
	AVAssetReader * _assetReader;
	AVAssetReaderTrackOutput * _assetReaderVideoTrackOutput;
	AVAssetReaderTrackOutput * _assetReaderAudioTrackOutput;
	
#if defined(USE_VIDEO_OUTPUT)
	CMVideoFormatDescriptionRef _videoInfo;
	AVPlayerItemVideoOutput * _videoOutput;
#endif
	
	
    id timeObserver;
    
	CMSampleBufferRef videoSampleBuffer;
    CMSampleBufferRef audioSampleBuffer;
    CMTime videoSampleTime;
	CMTime videoSampleTimePrev;
    CMTime audioSampleTime;
    CMTime synchSampleTime;
	CMTime duration;
    CMTime currentTime;
    float volume;
    float speed;
    float frameRate;
    
    NSInteger videoWidth;
    NSInteger videoHeight;
	
    playerLoopType loop;
	
    BOOL bWillBeUpdatedExternally;
    BOOL bReady;
	BOOL bLoaded;
    BOOL bPlayStateBeforeLoad;
    BOOL bUpdateFirstFrame;
    BOOL bNewFrame;
    BOOL bPlaying;
	BOOL bWasPlayingBackwards; // for optimisation
    BOOL bFinished;
    BOOL bAutoPlayOnLoad;
    BOOL bSeeking;
    BOOL bSampleVideo; // default to YES
    BOOL bSampleAudio; // default to NO
	BOOL bIsUnloaded;
	BOOL bStream;
	int frameBeforeReady;
	float positionBeforeReady;
	
	NSLock* asyncLock;
	NSCondition* deallocCond;
}

@property (nonatomic, retain) AVPlayer * player;
@property (nonatomic, retain) AVAsset * asset;
@property (nonatomic, retain) AVPlayerItem * playerItem;


@property (nonatomic, retain) AVAssetReader * assetReader;
@property (nonatomic, retain) AVAssetReaderTrackOutput * assetReaderVideoTrackOutput;
@property (nonatomic, retain) AVAssetReaderTrackOutput * assetReaderAudioTrackOutput;

#if defined(USE_VIDEO_OUTPUT)
@property (nonatomic, retain) AVPlayerItemVideoOutput *videoOutput;
#endif


- (BOOL)loadWithFile:(NSString*)file async:(BOOL)bAsync;
- (BOOL)loadWithPath:(NSString*)path async:(BOOL)bAsync;
- (BOOL)loadWithURL:(NSURL*)url async:(BOOL)bAsync stream:(BOOL)isStream;
- (void)unloadVideoAsync;
- (void)unloadVideo;

- (void)update;

- (void)play;
- (void)pause;
- (void)togglePlayPause;

- (void)stepByCount:(long)frames;

- (void)seekToStart;
- (void)seekToEnd;
- (void)seekToTime:(CMTime)time;
- (void)seekToTime:(CMTime)time withTolerance:(CMTime)tolerance;

- (BOOL)isReady;
- (BOOL)isLoaded;
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
- (void)setLoop:(playerLoopType)loop;
- (playerLoopType)getLoop;
- (void)setSpeed:(float)speed;
- (float)getSpeed;
- (void)setAutoplay:(BOOL)bAutoplay;
- (BOOL)getAutoplay;
- (void)setWillBeUpdatedExternally:(BOOL)value;
- (void)close;
- (void)setStreaming:(BOOL)value;


@end
