

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <UIKit/UIKit.h>
#include "ofMain.h"


static NSString* const VIDEO_FRAME_EXTRACTOR_PAUSED = @"paused";
static NSString* const VIDEO_FRAME_EXTRACTOR_PROGRESS = @"progress";
static NSString* const VIDEO_FRAME_EXTRACTOR_MIDPOINT = @"midpoint";


@protocol AVFoundationVideoPlayerStatusListener

- (void)AVFoundationVideoPlayerStatusChanged:(NSString*)message withArgument:(float)argument;

@end

#include <vector>
using namespace std;

#define NUM_AUDIO_BUFFERS 6

@interface AVFoundationVideoPlayer: NSObject {

	AVURLAsset* asset;
	AVAssetReader* asset_reader;
	AVAssetReaderOutput* asset_reader_video_output;
	AVAssetReaderOutput* asset_reader_audio_output;
	
	CMSampleBufferRef buffer;
	Float64 presentation_timestamp_s;
	Float64 last_returned_frame_presentation_timestamp_s;
	
	Float64 video_duration;
	
	AudioQueueBufferRef	audio_buffer[NUM_AUDIO_BUFFERS];
	AudioQueueRef		audio_queue;
	CMSampleBufferRef	audio_from_avasset_buffer;
	UInt32				audio_from_avasset_samples_given_out;
	ofMutex			audio_mutex;
	bool				audio_finished;
	int					audio_n_channels;
	
	float audio_lag;
	bool paused;
	bool finished;
	bool told_midpoint;
	bool initialised;

	bool playOnLoad;
	bool bDoAudio;
	
	float current_video_time;
	float vol;
	
	vector<UIViewController<AVFoundationVideoPlayerStatusListener>*> status_listeners;
	
}

- (id)initWithURL:(NSURL*)url;
- (id)initWithPath:(NSString*)path;
- (id)initWithURL:(NSURL*)url;
- (void)dealloc;

- (void)addStatusListener:(UIViewController<AVFoundationVideoPlayerStatusListener>*) listener;

- (void)play;
- (void)pause;
- (bool)isFinished;
- (bool)isPaused;
- (bool)isInErrorState;
- (bool)canPlay;
- (void)setVolume:(float)volume;

- (void)updateWithElapsedTime:(float)elapsed;
- (bool)hasNewFrame;
/// return the current frame. caller is responsible for both UIImage and CGImageRef. 
/// in addition, caller must destroy UIImage before CGImageRef.
- (pair<UIImage*,CGImageRef>)getCopyOfCurrentFrame;
- (CVImageBufferRef)getCurrentFrame;

- (void)audioCallbackWithQueue:(AudioQueueRef)queue output:(AudioQueueBufferRef)output_buffer;

/// return the current video time (on-screen time)
- (float)getVideoTime;
- (float)getVideoPosition;
- (Float64)getDuration;

- (void)continueInit;


@end




