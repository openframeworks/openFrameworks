//
//  ofAVFoundationVideoPlayer.m
//  Created by Lukasz Karluk on 06/07/14.
//	Merged with code by Sam Kronick, James George and Elie Zananiri.
//

#import "ofAVFoundationVideoPlayer.h"

#define IS_OS_6_OR_LATER    ([[[UIDevice currentDevice] systemVersion] floatValue] >= 6.0)



static NSString * const kTracksKey = @"tracks";
static NSString * const kStatusKey = @"status";
static NSString * const kRateKey = @"rate";

//---------------------------------------------------------- video player.
@implementation ofAVFoundationVideoPlayer

@synthesize player = _player;
@synthesize asset = _asset;
@synthesize playerItem = _playerItem;

@synthesize assetReader = _assetReader;
@synthesize assetReaderVideoTrackOutput = _assetReaderVideoTrackOutput;
@synthesize assetReaderAudioTrackOutput = _assetReaderAudioTrackOutput;
#if USE_VIDEO_OUTPUT
@synthesize videoOutput = _videoOutput;
#endif


static const void *ItemStatusContext = &ItemStatusContext;
static const void *PlayerRateContext = &ItemStatusContext;


- (id)init {
	self = [super init];
	if(self) {
		
		// create avplayer
		_player = nil;
		
		asyncLock = [[NSLock alloc] init];
		deallocCond = nil;
		
#if USE_VIDEO_OUTPUT
		// create videooutput
		_videoOutput = nil;
		_videoInfo = nil;
#endif
	
		
		timeObserver = nil;
		
		videoSampleBuffer = nil;
		audioSampleBuffer = nil;
		videoSampleTime = kCMTimeNegativeInfinity;//kCMTimeZero;
		audioSampleTime = kCMTimeNegativeInfinity;//kCMTimeZero;
		synchSampleTime = kCMTimeInvalid;
		duration = kCMTimeZero;
		currentTime = kCMTimeZero;
		volume = 1;
		speed = 1;
		frameRate = 0;
		
		videoWidth = 0;
		videoHeight = 0;
		
		bWillBeUpdatedExternally = NO;
		bReady = NO;
		bLoaded = NO;
		bPlayStateBeforeLoad = NO;
		bUpdateFirstFrame = YES;
		bNewFrame = NO;
		bPlaying = NO;
		bWasPlayingBackwards  = NO;
		bFinished = NO;
		bAutoPlayOnLoad = NO;
		loop = LOOP_NONE;
		bSeeking = NO;
		bSampleVideo = YES;
		bIsUnloaded = NO;
		
		// do not sample audio by default
		// we are lacking interfaces for audiodata
		bSampleAudio = NO;
	}
	return self;
}

#if USE_VIDEO_OUTPUT
- (void)createVideoOutput
{
#ifdef TARGET_IOS
	NSDictionary *pixBuffAttributes = @{(id)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_32BGRA)};
#elif defined(TARGET_OSX)
	NSDictionary *pixBuffAttributes = @{(id)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_32ARGB)};
#endif
	
	self.videoOutput = [[[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:pixBuffAttributes] autorelease];
	if (!self.videoOutput) {
		NSLog(@"error creating video output");
		return;
	}
	
	self.videoOutput.suppressesPlayerRendering = YES;
}
#endif


//---------------------------------------------------------- cleanup / dispose.
- (void)dealloc
{
	if (_player != nil){
		[self unloadVideo];
	}
	
	[asyncLock lock];
	
	[asyncLock unlock];
	
	// release locks
	[asyncLock autorelease];
	
	if (deallocCond != nil) {
		[deallocCond release];
		deallocCond = nil;
	}
	
	
	[super dealloc];
}



//---------------------------------------------------------- load / unload.
- (BOOL)loadWithFile:(NSString*)file async:(BOOL)bAsync{
	NSArray * fileSplit = [file componentsSeparatedByString:@"."];
	NSURL * fileURL = [[NSBundle mainBundle] URLForResource:[fileSplit objectAtIndex:0]
											  withExtension:[fileSplit objectAtIndex:1]];
	
	return [self loadWithURL:fileURL async:bAsync];
}

- (BOOL)loadWithPath:(NSString*)path async:(BOOL)bAsync{
	NSURL * fileURL = [NSURL fileURLWithPath:path];
	return [self loadWithURL:fileURL async:bAsync];
}

- (BOOL)loadWithURL:(NSURL*)url async:(BOOL)bAsync {
	
	NSDictionary *options = @{(id)AVURLAssetPreferPreciseDurationAndTimingKey:@(YES)};
	AVURLAsset* asset = [AVURLAsset URLAssetWithURL:url options:options];
	
	if(asset == nil) {
		NSLog(@"error loading asset: %@", [url description]);
		return NO;
	}
	
	
	// store state
	BOOL _bReady = bReady;
	BOOL _bLoaded = bLoaded;
	BOOL _bPlayStateBeforeLoad = bPlayStateBeforeLoad;
	
	// set internal state
	bIsUnloaded = NO;
	bReady = NO;
	bLoaded = NO;
	bPlayStateBeforeLoad = NO;
	
	// going to load
	dispatch_semaphore_t sema = dispatch_semaphore_create(0);
	dispatch_queue_t queue;
	if(bAsync == YES){
		queue = dispatch_get_main_queue();
	} else {
		queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
	}
	
	dispatch_async(queue, ^{
		[asset loadValuesAsynchronouslyForKeys:[NSArray arrayWithObject:kTracksKey] completionHandler:^{
			
			NSError * error = nil;
			AVKeyValueStatus status = [asset statusOfValueForKey:kTracksKey error:&error];
			
			if(status != AVKeyValueStatusLoaded) {
				NSLog(@"error loading asset tracks: %@", [error localizedDescription]);
				// reset
				bReady = _bReady;
				bLoaded = _bLoaded;
				bPlayStateBeforeLoad = _bPlayStateBeforeLoad;
				if(bAsync == NO){
					dispatch_semaphore_signal(sema);
				}
				return;
			}
			
			CMTime _duration = [asset duration];
			
			if(CMTimeCompare(_duration, kCMTimeZero) == 0) {
				NSLog(@"track loaded with zero duration.");
				// reset
				bReady = _bReady;
				bLoaded = _bLoaded;
				bPlayStateBeforeLoad = _bPlayStateBeforeLoad;
				if(bAsync == NO){
					dispatch_semaphore_signal(sema);
				}
				return;
			}
			
			// TODO
			// why not reading infinite media?
			// how about playing back HLS streams?
			if(isfinite(CMTimeGetSeconds(duration)) == NO) {
				NSLog(@"track loaded with infinite duration.");
				// reset
				bReady = _bReady;
				bLoaded = _bLoaded;
				bPlayStateBeforeLoad = _bPlayStateBeforeLoad;
				if(bAsync == NO){
					dispatch_semaphore_signal(sema);
				}
				return;
			}
			
			NSArray * videoTracks = [asset tracksWithMediaType:AVMediaTypeVideo];
			if([videoTracks count] == 0) {
				NSLog(@"no video tracks found.");
				// reset
				bReady = _bReady;
				bLoaded = _bLoaded;
				bPlayStateBeforeLoad = _bPlayStateBeforeLoad;
				if(bAsync == NO){
					dispatch_semaphore_signal(sema);
				}
				return;
			}
			
			//------------------------------------------------------------
			//------------------------------------------------------------ use asset
			// good to go
			[asyncLock lock];
			
			if (bIsUnloaded) {
				// player was unloaded before we could load everting
				bIsUnloaded = NO;
				if(bAsync == NO){
					dispatch_semaphore_signal(sema);
				}
				[asyncLock unlock];
				return;
			}
			
			// clean up
			[self unloadVideoAsync];     // unload video if one is already loaded.
			
			bIsUnloaded = NO;
			
			// set asset
			self.asset = asset;
			duration = _duration;
			
			// create asset reader
			BOOL bOk = [self createAssetReaderWithTimeRange:CMTimeRangeMake(kCMTimeZero, duration)];
			if(bOk == NO) {
				NSLog(@"problem with creating asset reader.");
				if(bAsync == NO){
					dispatch_semaphore_signal(sema);
				}
				[asyncLock unlock];
				return;
			}
			
			
			AVAssetTrack * videoTrack = [videoTracks objectAtIndex:0];
			frameRate = videoTrack.nominalFrameRate;
			videoWidth = [videoTrack naturalSize].width;
			videoHeight = [videoTrack naturalSize].height;
			
			NSLog(@"video loaded at %li x %li @ %f fps", (long)videoWidth, (long)videoHeight, frameRate);
			
//			currentTime = CMTimeMakeWithSeconds((1.0/frameRate), NSEC_PER_SEC);//kCMTimeZero;
			currentTime = CMTimeMakeWithSeconds(0.0, NSEC_PER_SEC);//kCMTimeZero;
			
			
			//------------------------------------------------------------ create player item.
			AVPlayerItem* playerItem = [AVPlayerItem playerItemWithAsset:self.asset];
			
			if (!playerItem) {
				NSLog(@"could not create AVPlayerItem");
				if(bAsync == NO){
					dispatch_semaphore_signal(sema);
				}
				[asyncLock unlock];
				return;
			}
			
			//------------------------------------------------------------ player item.
			self.playerItem = playerItem;
			[self.playerItem addObserver:self
							  forKeyPath:kStatusKey
								 options:0
								 context:&ItemStatusContext];
			
			NSNotificationCenter* notificationCenter = [NSNotificationCenter defaultCenter];
			[notificationCenter addObserver:self
								   selector:@selector(playerItemDidReachEnd)
									   name:AVPlayerItemDidPlayToEndTimeNotification
									 object:self.playerItem];
			
			//AVPlayerItemPlaybackStalledNotification only exists from OS X 10.9 or iOS 6.0 and up
#if (__MAC_OS_X_VERSION_MIN_REQUIRED >= 1090) || (__IPHONE_OS_VERSION_MIN_REQUIRED >= 60000)
			[notificationCenter addObserver:self
								   selector:@selector(playerItemDidStall)
									   name:AVPlayerItemPlaybackStalledNotification
									 object:self.playerItem];
#endif
			
#if USE_VIDEO_OUTPUT
			// safety
			if (self.videoOutput == nil) {
				[self createVideoOutput];
			}
			
			// add video output
			[self.playerItem addOutput:self.videoOutput];
#endif
			
			
			//------------------------------------------------------------ recreate player.
			// destroy player if any - should never be the case!!
			if(_player != nil) {
				[self removeTimeObserverFromPlayer];
				[self.player removeObserver:self forKeyPath:kRateKey context:&PlayerRateContext];
				self.player = nil;
				[_player release];
			}
			
			// create new player
			_player = [[AVPlayer playerWithPlayerItem:self.playerItem] retain];
			[self.player addObserver:self
						  forKeyPath:kRateKey
							 options:NSKeyValueObservingOptionNew
							 context:&PlayerRateContext];
			// add timeobserver?
			[self addTimeObserverToPlayer];
			
			_player.volume = volume;
			
			// loaded
			bLoaded = true;
			
			if(bAsync == NO){
				dispatch_semaphore_signal(sema);
			}
			
			[asyncLock unlock];

		}];
	});
	
	// Wait for the dispatch semaphore signal
	if(bAsync == NO){
		dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER);
		dispatch_release(sema);
		return bLoaded;
	} else {
		dispatch_release(sema);
		return YES;
	}
}


#pragma mark - unload video
- (void)unloadVideoAsync {

	bIsUnloaded = YES;
	bReady = NO;
	bLoaded = NO;
//	bPlayStateBeforeLoad = NO;
	bUpdateFirstFrame = YES;
	bNewFrame = NO;
	bPlaying = NO;
	bFinished = NO;
	bWasPlayingBackwards = NO;
	
	videoSampleTime = kCMTimeNegativeInfinity;
	audioSampleTime = kCMTimeNegativeInfinity;
	synchSampleTime = kCMTimeInvalid;
	duration = kCMTimeZero;
	currentTime = kCMTimeZero;
	
	videoWidth = 0;
	videoHeight = 0;
	

	// a reference to all the variables for the block
	__block AVAsset* currentAsset = _asset;
	__block AVAssetReader* currentReader = _assetReader;
	__block AVAssetReaderTrackOutput* currentVideoTrack = _assetReaderVideoTrackOutput;
	__block AVAssetReaderTrackOutput* currentAudioTrack = _assetReaderAudioTrackOutput;
	__block AVPlayerItem* currentItem = _playerItem;
	__block AVPlayer* currentPlayer = _player;
	__block id currentTimeObserver = timeObserver;
	
	__block CMSampleBufferRef currentVideoSampleBuffer = videoSampleBuffer;
	__block CMSampleBufferRef currentAudioSampleBuffer = audioSampleBuffer;
	
#if USE_VIDEO_OUTPUT
	__block AVPlayerItemVideoOutput* currentVideoOutput = _videoOutput;
	__block CMVideoFormatDescriptionRef currentVideoInfo = _videoInfo;
	
	_videoOutput = nil;
	self.videoOutput = nil;
	
	_videoInfo = nil;
#endif
	
	// set all to nil
	// cleanup happens in the block
	_asset = nil;
	self.asset = nil;
	
	_assetReader = nil;
	self.assetReader = nil;
	
	_assetReaderVideoTrackOutput = nil;
	self.assetReaderVideoTrackOutput = nil;
	
	_assetReaderAudioTrackOutput = nil;
	self.assetReaderAudioTrackOutput = nil;
	
	_playerItem = nil;
	self.playerItem = nil;
	
	_player = nil;
	self.player = nil;
	timeObserver = nil;
	
	videoSampleBuffer = nil;
	audioSampleBuffer = nil;
	
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		
		@autoreleasepool {
			
			[asyncLock lock];
			
			// relase assetreader
			if (currentReader != nil) {
				[currentReader cancelReading];
				[currentReader autorelease];
				currentReader = nil;
				
				if (currentVideoTrack != nil) {
					[currentVideoTrack autorelease];
					currentVideoTrack = nil;
				}
				
				if (currentAudioTrack != nil) {
					[currentAudioTrack autorelease];
					currentAudioTrack = nil;
				}
			}
			
			// release asset
			if (currentAsset != nil) {
				[currentAsset cancelLoading];
				[currentAsset autorelease];
				currentAsset = nil;
			}
			
			
			// release current player item
			if(currentItem != nil) {
				
				[currentItem cancelPendingSeeks];
				[currentItem removeObserver:self forKeyPath:kStatusKey context:&ItemStatusContext];
				
				NSNotificationCenter* notificationCenter = [NSNotificationCenter defaultCenter];
				[notificationCenter removeObserver:self
											  name:AVPlayerItemDidPlayToEndTimeNotification
											object:currentItem];
				
				//AVPlayerItemPlaybackStalledNotification only exists from OS X 10.9 or iOS 6.0 and up
#if (__MAC_OS_X_VERSION_MIN_REQUIRED >= 1090) || (__IPHONE_OS_VERSION_MIN_REQUIRED >= 60000)
				[notificationCenter removeObserver:self
											  name:AVPlayerItemPlaybackStalledNotification
											object:currentItem];
#endif
				
#if USE_VIDEO_OUTPUT
				// remove output
				[currentItem removeOutput:currentVideoOutput];
				
				// release videouOutput
				if (currentVideoOutput != nil) {
					[currentVideoOutput autorelease];
					currentVideoOutput = nil;
				}
				
				// destroy video info
				if (currentVideoInfo != nil) {
					CFRelease(currentVideoInfo);
					currentVideoInfo = nil;
				}
#endif
				
				[currentItem autorelease];
				currentItem = nil;
			}
			
			
			// destroy current player
			if (currentPlayer != nil) {
				[currentPlayer removeObserver:self forKeyPath:kRateKey context:&PlayerRateContext];

				if (currentTimeObserver != nil) {
					[currentPlayer removeTimeObserver:currentTimeObserver];
					[currentTimeObserver autorelease];
					currentTimeObserver = nil;
				}
				
				[currentPlayer autorelease];
				currentPlayer = nil;
			}
			
			
			if(currentVideoSampleBuffer) {
				CFRelease(currentVideoSampleBuffer);
				currentVideoSampleBuffer = nil;
			}
			
			if(currentAudioSampleBuffer) {
				CFRelease(currentAudioSampleBuffer);
				currentAudioSampleBuffer = nil;
			}
			
			[asyncLock unlock];
			
			if (deallocCond != nil) {
				[deallocCond lock];
				[deallocCond signal];
				[deallocCond unlock];
			}
		}
	});
	
}

- (void)unloadVideo
{
	// create a condition
	deallocCond = [[NSCondition alloc] init];
	[deallocCond lock];
	
	// unload current video
	[self unloadVideoAsync];
	
	// wait for unloadVideoAsync to finish
	[deallocCond wait];
	[deallocCond unlock];
	
	[deallocCond release];
	deallocCond = nil;
}

- (void)close
{
	[asyncLock lock];
	[self unloadVideoAsync];
	[asyncLock unlock];
}


#pragma mark -
- (BOOL)createAssetReaderWithTimeRange:(CMTimeRange)timeRange {
	
	videoSampleTime = videoSampleTimePrev = kCMTimeNegativeInfinity;
	audioSampleTime = kCMTimeNegativeInfinity;
	

	NSError *error = nil;
	
	// safety
	if (self.assetReader != nil) {
		[self.assetReader cancelReading];
		self.assetReader = nil;
	}
	
	// create new asset reader
	self.assetReader = [AVAssetReader assetReaderWithAsset:self.asset error:&error];
	if(error) {
		NSLog(@"assetReader: error during initialisation: %@", [error localizedDescription]);
		return NO;
	}
	
	// set timerange
	self.assetReader.timeRange = timeRange;
	
	
	//------------------------------------------------------------ add video output.
	if (bSampleVideo) {
		NSMutableDictionary * videoOutputSettings = [[[NSMutableDictionary alloc] init] autorelease];
#ifdef TARGET_IOS
		[videoOutputSettings setObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA]
								forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey];
#elif defined(TARGET_OSX)
		[videoOutputSettings setObject:[NSNumber numberWithInt:kCVPixelFormatType_32ARGB]
								forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey];
#endif
		
		
		NSArray * videoTracks = [self.asset tracksWithMediaType:AVMediaTypeVideo];
		if([videoTracks count] > 0) {
			AVAssetTrack * videoTrack = [videoTracks objectAtIndex:0];
			self.assetReaderVideoTrackOutput = [AVAssetReaderTrackOutput assetReaderTrackOutputWithTrack:videoTrack
																						  outputSettings:videoOutputSettings];
			
			if(self.assetReaderVideoTrackOutput != nil) {
				
				// dont copy sample data (might improve performance)
				self.assetReaderVideoTrackOutput.alwaysCopiesSampleData = NO;
			
				if([self.assetReader canAddOutput:self.assetReaderVideoTrackOutput]) {
					[self.assetReader addOutput:self.assetReaderVideoTrackOutput];
				} else {
					NSLog(@"assetReaderVideoTrackOutput cannot be add to assetReader");
				}
				
			} else {
				NSLog(@"assetReaderVideoTrackOutput failed to load.");
			}
		}
	}
	
	//------------------------------------------------------------ add audio output.
	if (bSampleAudio) {
		// really?
		double preferredHardwareSampleRate = 44100;
#ifdef TARGET_IOS
		[[AVAudioSession sharedInstance] currentHardwareSampleRate];
#endif
		
		AudioChannelLayout channelLayout;
		bzero(&channelLayout, sizeof(channelLayout));
		channelLayout.mChannelLayoutTag = kAudioChannelLayoutTag_Stereo;
		
		int numOfChannels = 1;
		if(channelLayout.mChannelLayoutTag == kAudioChannelLayoutTag_Stereo) {
			numOfChannels = 2;
		}
		
		NSDictionary * audioOutputSettings = nil;
		audioOutputSettings = [NSDictionary dictionaryWithObjectsAndKeys:
							   [NSNumber numberWithInt:kAudioFormatLinearPCM], AVFormatIDKey,
							   [NSNumber numberWithFloat:preferredHardwareSampleRate], AVSampleRateKey,
#ifdef IS_OS_6_OR_LATER
							   // including AVNumberOfChannelsKey & AVChannelLayoutKey on iOS5 causes a crash,
							   // check if iOS6 or greater before including these.
							   [NSNumber numberWithInt:numOfChannels], AVNumberOfChannelsKey,
							   [NSData dataWithBytes:&channelLayout length:sizeof(AudioChannelLayout)], AVChannelLayoutKey,
#endif
							   [NSNumber numberWithInt:16], AVLinearPCMBitDepthKey,
							   [NSNumber numberWithBool:NO], AVLinearPCMIsNonInterleaved,
							   [NSNumber numberWithBool:NO], AVLinearPCMIsFloatKey,
							   [NSNumber numberWithBool:NO], AVLinearPCMIsBigEndianKey,
							   nil];
		
		NSArray * audioTracks = [self.asset tracksWithMediaType:AVMediaTypeAudio];
		if([audioTracks count] > 0) {
			AVAssetTrack * audioTrack = [audioTracks objectAtIndex:0];
			
			self.assetReaderAudioTrackOutput = [AVAssetReaderTrackOutput assetReaderTrackOutputWithTrack:audioTrack
																						  outputSettings:audioOutputSettings];
			
			if(self.assetReaderAudioTrackOutput != nil) {
				
				// dont copy sample data
				self.assetReaderAudioTrackOutput.alwaysCopiesSampleData = NO;
				
				if([self.assetReader canAddOutput:self.assetReaderAudioTrackOutput]) {
					[self.assetReader addOutput:self.assetReaderAudioTrackOutput];
				} else {
					NSLog(@"assetReaderAudioTrackOutput cannot be add to assetReader");
				}
				
			} else {
				NSLog(@"assetReaderAudioTrackOutput failed to load.");
			}
		}
	}
	
	//------------------------------------------------------------ start reading.
	BOOL bOk = [self.assetReader startReading];
	if(!bOk ) {
		NSLog(@"assetReader couldn't startReading: %@", [self.assetReader error]);
	}
	
	return YES;
}


//---------------------------------------------------------- player callbacks.
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	
	if(context == &ItemStatusContext) {
		
		if (object == self.playerItem) {
			
			if ([self.playerItem status] == AVPlayerItemStatusReadyToPlay) {
				
				if (bReady) {
					return;
				}

				bReady = true;
				
				[self setVolume:volume]; // set volume for current video.
				if(bAutoPlayOnLoad || bPlayStateBeforeLoad) {
					[self play];
				}
				
				[self update]; // update as soon is ready so pixels are loaded.

				
			} else if ([self.playerItem status] == AVPlayerItemStatusUnknown) {
				NSLog(@"AVPlayerItemStatusUnknown");
			} else if ([self.playerItem status] == AVPlayerItemStatusFailed) {
				NSLog(@"AVPlayerItemStatusFailed");
			} else {
				NSLog(@"AVPlayerItem: such status: %ld", (long)[self.playerItem status]);
			}
			
		} else {
			// ignore other objects
		}
		
		return;
	} else if (context == &PlayerRateContext) {
		
		if (object == self.player) {
		
			if (bReady &&
				[keyPath isEqualToString:kRateKey])
			{
				float rate = [[change objectForKey:@"new"] floatValue];
				bPlaying = (rate != 0);
			}
		} else {
			// ignore other object
		}
		
		return;
	}
	
	// push it up the observer chain
	[super observeValueForKeyPath:keyPath
						 ofObject:object
						   change:change
						  context:context];
}

- (void)playerItemDidReachEnd {
	
	bFinished = YES;
	bPlaying = NO;
	
	if (speed > 0.0) {
		// playing forward
		if (loop == LOOP_NORMAL) {
			[self seekToStart];
			[self play];
		} else if (loop == LOOP_PALINDROME) {
			[self setSpeed:-speed];
			[self play];
		}
		
	} else if (speed < 0.0) {
		// playing backwards
		if (loop == LOOP_NORMAL) {
			[self seekToEnd];
			[self play];
		} else if (loop == LOOP_PALINDROME) {
			[self setSpeed:-speed];
			[self play];
		}
	}
	
	
	if(loop > LOOP_NONE) {
		bFinished = NO;
	}
}


- (void)playerItemDidStall {
	NSLog(@"playerItem did stall - samples did not arrive in time");
}


//---------------------------------------------------------- update.
- (void)update {
	
	/**
	 *  return if,
	 *  video is not yet loaded,
	 *  video is finished playing.
	 */
	if(!bReady || bFinished) {
		bNewFrame = NO;
		return;
	}

#if !USE_VIDEO_OUTPUT
	[self updateFromAssetReader];
#else
	// get new sample
	if (self.player.rate > 0.0) {
		// playing forward
		// pull out frames from assetreader
		[self updateFromAssetReader];
		
	} else {
		// playing paused or playing backwards
		// get samples from videooutput
		[self updateFromVideoOutput];
		if (bNewFrame) {
			videoSampleTimePrev = videoSampleTime;
		}
	}
#endif
}

#if USE_VIDEO_OUTPUT
- (void)updateFromVideoOutput {
	OSStatus err = noErr;
	
	// get time from player
	CMTime time = [_player currentTime];

	if ([self.videoOutput hasNewPixelBufferForItemTime:time]) {
		
		bNewFrame = YES;
		currentTime = time;
		
		CVPixelBufferRef buffer = [self.videoOutput copyPixelBufferForItemTime:time itemTimeForDisplay:NULL];
		
		// create or update video format description
		if (!_videoInfo || !CMVideoFormatDescriptionMatchesImageBuffer(_videoInfo, buffer)) {
			if (_videoInfo) {
				CFRelease(_videoInfo);
				_videoInfo = nil;
			}
			err = CMVideoFormatDescriptionCreateForImageBuffer(NULL, buffer, &_videoInfo);
		}
		if (err) {
			NSLog(@"Error at CMVideoFormatDescriptionCreateForImageBuffer %ld", (long)err);
			bNewFrame = NO;
			// release temp buffer
			CVBufferRelease(buffer);
			return;
		}
		
		// decodeTimeStamp is set to kCMTimeInvalid since we already receive decoded frames
		CMSampleTimingInfo sampleTimingInfo = {
			.duration = kCMTimeInvalid,
			.presentationTimeStamp = time,
			.decodeTimeStamp = kCMTimeInvalid
		};
		
	
		// release old buffer.
		if(videoSampleBuffer) {
			CFRelease(videoSampleBuffer);
			videoSampleBuffer = nil;
		}
		
		// create new sampleBuffer
		err = CMSampleBufferCreateForImageBuffer(kCFAllocatorDefault,
												 buffer,
												 true,
												 NULL,
												 NULL,
												 _videoInfo,
												 &sampleTimingInfo,
												 &videoSampleBuffer);
		if (err) {
			NSLog(@"Error at CMSampleBufferCreateForImageBuffer %ld", (long)err);
			bNewFrame = NO;
			// release temp buffer
			CVBufferRelease(buffer);
			return;
		}
		
		// release temp buffer
		CVBufferRelease(buffer);
		
		videoSampleTime = time;
		
		
	} else {
		// no new frame for time
		bNewFrame = NO;
	}
}
#endif

- (void)updateFromAssetReader {
	
	/**
	 *  in most cases we check at what time the video player is up to,
	 *  and use the time for sampling buffers in the code below.
	 *  but if a synchSampleTime is provided then we use it instead of video player time.
	 *  synchSampleTime is used when synchronous time is needed (video player plays asynchronously),
	 *  such as when needing to access the video frames, frame by frame.
	 *  synchSampleTime is cleared (invalidated) on every frame so it must be set before calling update.
	 */
	CMTime time = kCMTimeZero;
	if(CMTIME_IS_VALID(synchSampleTime)) {
		time = synchSampleTime;
		time = CMTimeMaximum(time, kCMTimeZero);
		time = CMTimeMinimum(time, duration);
		synchSampleTime = kCMTimeInvalid;
	} else {
		time = [_player currentTime];
	}
	
	BOOL bTimeChanged = CMTimeCompare(time, currentTime) != 0;
	currentTime = time;
	
	if(bUpdateFirstFrame) {
		
		// this forces the first frame to be updated.
		// here the values for time and currentTime are both zero.
		// so this is to get around the progress check below.
		
		bUpdateFirstFrame = NO;
		
	} else if(bTimeChanged == NO) {
		
		// current time has not changed,
		// so the video has not progressed.
		
		if(self.assetReader != nil) {
			
			// check that assetReader has been created.
			// if assetReader is nil, the video must still be seeking to a new position.
			// so even though the time has not changed, the assetReader needs to be created.
			
			bNewFrame = NO;
			return;
		}
	}
	
	if(self.assetReader == nil) {
		if(bSeeking == true) {
			
			// video player is seeking to new position.
			// asset reader can only be created when seeking has finished.
			
			bNewFrame = NO;
			return;
		}
		
		[self createAssetReaderWithTimeRange:CMTimeRangeMake(currentTime, duration)];
	}
	
	if (self.assetReader.status == AVAssetReaderStatusFailed) {
		NSLog(@"assetReader error: %@", self.assetReader.error);
	}
	
	if(self.assetReader.status != AVAssetReaderStatusReading)
	{
		bNewFrame = NO;
		return;
	}
	
	//---------------------------------------------------------- video buffer.
	BOOL bCopiedNewSamples = NO;
	while(bSampleVideo == true &&                                       // video sampling is on.
		  self.assetReaderVideoTrackOutput != nil &&                    // asset has a video track.
		  self.assetReader.status == AVAssetReaderStatusReading &&      // asset read is in reading state.
		  ((CMTimeCompare(videoSampleTime, currentTime) == -1) ))       // timestamp is less then currentTime.
		   
	{
		CMSampleBufferRef videoBufferTemp;

		@try {
			videoBufferTemp = [self.assetReaderVideoTrackOutput copyNextSampleBuffer];
		} @catch (NSException * e) {
			NSLog(@"error: %@", e);
			break;
		}
		
		if(videoBufferTemp) {
			if(videoSampleBuffer) { // release old buffer.
				CFRelease(videoSampleBuffer);
				videoSampleBuffer = nil;
			}
			videoSampleBuffer = videoBufferTemp; // save reference to new buffer.
			
			videoSampleTime = CMSampleBufferGetPresentationTimeStamp(videoSampleBuffer);
			
			bCopiedNewSamples = YES;
		} else {
			bNewFrame = NO;
			videoSampleTime = videoSampleTimePrev = kCMTimeNegativeInfinity;
			bUpdateFirstFrame = YES;
			break;
		}
	}
	
	//---------------------------------------------------------- audio buffer.
	while(bSampleAudio == true &&                                   // audio sampling is on.
		  self.assetReaderAudioTrackOutput != nil &&                // asset has a audio track.
		  self.assetReader.status == AVAssetReaderStatusReading &&  // asset read is in reading state.
		  ((CMTimeCompare(audioSampleTime, currentTime) == -1) ||    // timestamp is less then currentTime.
		   (CMTimeCompare(audioSampleTime, currentTime) == 0)))      // timestamp is equal currentTime.
	{
		CMSampleBufferRef audioBufferTemp;
		@try {
			audioBufferTemp = [self.assetReaderAudioTrackOutput copyNextSampleBuffer];
		} @catch (NSException * e) {
			break;
		}
		
		if(audioBufferTemp) {
			if(audioSampleBuffer) { // release old buffer.
				CFRelease(audioSampleBuffer);
				audioSampleBuffer = nil;
			}
			audioSampleBuffer = audioBufferTemp; // save reference to new buffer.
			
			audioSampleTime = CMSampleBufferGetPresentationTimeStamp(audioSampleBuffer);
		} else {
			audioSampleTime = kCMTimeNegativeInfinity;
			break;
		}
	}
	
	if(bCopiedNewSamples == true) {
		bNewFrame = CMTimeCompare(videoSampleTime, videoSampleTimePrev) == 1;
		
		if(bNewFrame) {
			videoSampleTimePrev = videoSampleTime;
		}
	}
}


- (void)addTimeObserverToPlayer {
	if(bWillBeUpdatedExternally) {
		return;
	}
	
	if(timeObserver != nil){
		return;
	}
	
	double interval = 1.0 / (double)frameRate;
	
	__block ofAVFoundationVideoPlayer* refToSelf = self;
	timeObserver = [[_player addPeriodicTimeObserverForInterval:CMTimeMakeWithSeconds(interval, NSEC_PER_SEC)
														  queue:dispatch_get_main_queue() usingBlock:
					 ^(CMTime time) {
						 [refToSelf update];
					 }] retain];
}

- (void)removeTimeObserverFromPlayer {
	if(timeObserver != nil) {
		[_player removeTimeObserver:timeObserver];
		[timeObserver release];
		timeObserver = nil;
	}
}

//---------------------------------------------------------- play / pause.
- (void)play {
	if([self isReady]) {
		if(![self isPlaying]) {
			[self togglePlayPause];
		}
	} else {
		bPlayStateBeforeLoad = YES;
	}
}

- (void)pause {
	if([self isReady]) {
		if([self isPlaying]) {
			[self togglePlayPause];
		}
	} else {
		bPlayStateBeforeLoad = NO;
	}
}

- (void)togglePlayPause {
	bPlaying = !bPlaying;
	if([self isPlaying]) {
		if([self isFinished]) {
			[self seekToStart];
			bFinished = NO;
		}
		[_player setRate:speed];
	} else {
		[_player pause];
	}
}

- (void)stepByCount:(long)frames
{
	if(![self isReady]) {
		return;
	}
	
#if USE_VIDEO_OUTPUT
	[_player.currentItem stepByCount:frames];
#else
	if (frames < 0) {

		double timeSec = CMTimeGetSeconds(currentTime) - (1.0/frameRate);
		[self seekToTime:CMTimeMakeWithSeconds(timeSec, NSEC_PER_SEC) withTolerance:kCMTimeZero];
		
	} else if (![self isFinished] && frames > 0) {

		double timeSec = CMTimeGetSeconds(currentTime) + (1.0/frameRate);
		CMTime time = CMTimeMakeWithSeconds(timeSec, NSEC_PER_SEC);
		
		bSeeking = YES;
		time = CMTimeMaximum(time, kCMTimeZero);
		time = CMTimeMinimum(time, duration);
		
		// frames are preloaded, only seek player
		[_player seekToTime:time
			toleranceBefore:kCMTimePositiveInfinity
			 toleranceAfter:kCMTimePositiveInfinity
		  completionHandler:^(BOOL finished) {
			  bSeeking = NO;
		  }];
	}
#endif
}

//---------------------------------------------------------- seek.
- (void)seekToStart {
	[self seekToTime:kCMTimeZero withTolerance:kCMTimeZero];
}

- (void)seekToEnd {
	[self seekToTime:duration withTolerance:kCMTimeZero];
}

- (void)seekToTime:(CMTime)time {
	[self seekToTime:time withTolerance:kCMTimePositiveInfinity];
}

- (void)seekToTime:(CMTime)time
	 withTolerance:(CMTime)tolerance {
	
	if(![self isReady]) {
		return;
	}
	
	if([self isFinished]) {
		bFinished = NO;
	}
	
	// TODO?
	// expensive call?
	// destroy it on a thread?
	[self.assetReader cancelReading];
	self.assetReader = nil;
	self.assetReaderVideoTrackOutput = nil;
	self.assetReaderAudioTrackOutput = nil;
	
	bSeeking = YES;
	
	// restrict time
	time = CMTimeMaximum(time, kCMTimeZero);
	time = CMTimeMinimum(time, duration);
	
	[self createAssetReaderWithTimeRange:CMTimeRangeMake(time, duration)];
	
	// set reader to real requested time
	[_player seekToTime:time
		toleranceBefore:tolerance
		 toleranceAfter:tolerance
	  completionHandler:^(BOOL finished) {
		  bSeeking = NO;
	  }];
}

//---------------------------------------------------------- states.
- (BOOL)isReady {
	return bReady;
}

- (BOOL)isLoaded {
	return bLoaded;
}

- (BOOL)isPlaying {
	return bPlaying;
}

- (BOOL)isNewFrame {
	return bNewFrame;
}

- (BOOL)isFinished {
	return bFinished;
}

//---------------------------------------------------------- sampling getters / setters.
- (void)setEnableVideoSampling:(BOOL)value {
	bSampleVideo = value;
}
- (void)setEnableAudioSampling:(BOOL)value {
	bSampleAudio = value;
}

- (void)setSynchSampleTime:(CMTime)time {
	synchSampleTime = time;
}

- (void)setSynchSampleTimeInSec:(double)time {
	[self setSynchSampleTime:CMTimeMakeWithSeconds(time, NSEC_PER_SEC)];
}

- (CMTime)getVideoSampleTime {
	return videoSampleTime;
}

- (double)getVideoSampleTimeInSec {
	return CMTimeGetSeconds(videoSampleTime);
}

- (CMTime)getAudioSampleTime {
	return audioSampleTime;
}

- (double)getAudioSampleTimeInSec {
	return CMTimeGetSeconds(audioSampleTime);
}

- (CMSampleBufferRef)getVideoSampleBuffer {
	return videoSampleBuffer;
}

- (CMSampleBufferRef)getAudioSampleBuffer {
	return audioSampleBuffer;
}

- (CVImageBufferRef)getCurrentFrame {
	return CMSampleBufferGetImageBuffer(videoSampleBuffer);
}

//---------------------------------------------------------- getters / setters.
- (NSInteger)getWidth {
	return videoWidth;
}

- (NSInteger)getHeight {
	return videoHeight;
}

- (CMTime)getCurrentTime {
	return currentTime;
}

- (double)getCurrentTimeInSec {
	return CMTimeGetSeconds(videoSampleTime);
}

- (CMTime)getDuration {
	return duration;
}

- (double)getDurationInSec {
	return CMTimeGetSeconds(duration);
}

- (float)getFrameRate {
	return frameRate;
}

- (int)getDurationInFrames {
	return [self getDurationInSec] * [self getFrameRate];
}

- (int)getCurrentFrameNum {
	return [self getCurrentTimeInSec] * [self getFrameRate];
}

- (void)setPosition:(float)position {
	if ([self isReady]) {
		double time = [self getDurationInSec] * position;
		[self seekToTime:CMTimeMakeWithSeconds(time, NSEC_PER_SEC)];
	}
}

- (void)setFrame:(int)frame {
	if ([self isReady]) {
		float position = frame / (float)[self getDurationInFrames];
		[self setPosition:position];
	}
}

- (float)getPosition {
	return ([self getCurrentTimeInSec] / [self getDurationInSec]);
}

- (void)setVolume:(float)value {
	
	volume = value;
	
	if(![self isReady]) {
		return;
	}
	
	if (self.playerItem == nil) {
		return;
	}
	
	_player.volume = volume;
}

- (float)getVolume {
	return volume;
}

- (void)setLoop:(playerLoopType)value {
	loop = value;
}

- (playerLoopType)getLoop {
	return loop;
}

- (void)setSpeed:(float)value {
	
	if(![self isReady]) {
		return;
	}
	
	if (_player == nil) {
		return;
	}
	
	if (!bSeeking && bWasPlayingBackwards && value > 0.0) {
		// create assetReaders if we played backwards earlier
		[self createAssetReaderWithTimeRange:CMTimeRangeMake(currentTime, duration)];
		bWasPlayingBackwards = NO;
	}
	
	if (!bWasPlayingBackwards && value < 0.0) {
		
#if !USE_VIDEO_OUTPUT
		// not supported
		NSLog(@"ERROR: Backwards playback is not supported. Minimum requirement is OSX 10.8 or iOS 6.0");
		value = 0.0;
#else
		if (!self.playerItem.canPlayReverse) {
			NSLog(@"ERROR: can not play backwards: not supported (check your codec)");
			value = 0.0;
		}
		if (self.videoOutput == nil) {
			NSLog(@"ERROR: can not play backwards: no video output");
			value = 0.0;
		}
		
		bWasPlayingBackwards = YES;
#endif
	}
	
	speed = value;
	[_player setRate:value];
}

- (float)getSpeed {
	return speed;
}

- (void)setAutoplay:(BOOL)value {
	bAutoPlayOnLoad = value;
}

- (BOOL)getAutoplay {
	return bAutoPlayOnLoad;
}

- (void)setWillBeUpdatedExternally:(BOOL)value {
	bWillBeUpdatedExternally = value;
}

@end