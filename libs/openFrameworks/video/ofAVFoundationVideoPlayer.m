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


static const NSString * ItemStatusContext;


- (id)init {
	self = [super init];
	if(self) {
		
		// create avplayer
		self.player = [[[AVPlayer alloc] init] autorelease];
		
		[_player addObserver:self
				  forKeyPath:kRateKey
					 options:NSKeyValueObservingOptionNew
					 context:nil];
		
		
#if USE_VIDEO_OUTPUT
		// create videooutput queue
		_myVideoOutputQueue = dispatch_queue_create(NULL, NULL);
		
		// create videooutput
		[self createVideoOutput];
		
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
		
		// do not sample audio by default
		// we are lacking interfaces for audiodata
		bSampleAudio = NO;
	}
	return self;
}

#if USE_VIDEO_OUTPUT
- (void)createVideoOutput {
#ifdef TARGET_IOS
	NSDictionary *pixBuffAttributes = @{(id)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_32BGRA)};
#elif defined(TARGET_OSX)
	NSDictionary *pixBuffAttributes = @{(id)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_32ARGB)};
#endif
	
	self.videoOutput = [[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:pixBuffAttributes];
	if (!self.videoOutput) {
		NSLog(@"error creating video output");
		return;
	}
	
	self.videoOutput.suppressesPlayerRendering = YES;
	[self.videoOutput setDelegate:self queue:_myVideoOutputQueue];
}
#endif


//---------------------------------------------------------- cleanup / dispose.
- (void)dealloc
{
	// unload current video
	[self unloadVideo];
	
	// destroy player
	if(self.player != nil) {
		[_player removeObserver:self forKeyPath:kRateKey];
		self.player = nil;
		[_player release];
	}

#if USE_VIDEO_OUTPUT
	if (self.videoOutput != nil) {
		self.videoOutput = nil;
	}
	dispatch_release(_myVideoOutputQueue);

#endif

	if(self.assetReaderVideoTrackOutput != nil) {
		self.assetReaderVideoTrackOutput = nil;
	}

	if(self.assetReader != nil) {
		[self.assetReader cancelReading];
		self.assetReader = nil;
		self.assetReaderVideoTrackOutput = nil;
		self.assetReaderAudioTrackOutput = nil;
	}

	if(self.asset != nil) {
		self.asset = nil;
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
	
	[self unloadVideo];     // unload video if one is already loaded.
	
	NSDictionary *options = @{(id)AVURLAssetPreferPreciseDurationAndTimingKey:@(YES)};
	self.asset = [AVURLAsset URLAssetWithURL:url options:options];
	
	if(self.asset == nil) {
		NSLog(@"error loading asset: %@", [url description]);
		return NO;
	}
	
	dispatch_semaphore_t sema = dispatch_semaphore_create(0);
	dispatch_queue_t queue;
	if(bAsync == YES){
		queue = dispatch_get_main_queue();
	} else {
		queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
	}
	
	dispatch_async(queue, ^{
		[self.asset loadValuesAsynchronouslyForKeys:[NSArray arrayWithObject:kTracksKey] completionHandler:^{
			NSError * error = nil;
			AVKeyValueStatus status = [self.asset statusOfValueForKey:kTracksKey error:&error];
			
			if(status != AVKeyValueStatusLoaded) {
				NSLog(@"error loading asset tracks: %@", [error localizedDescription]);
				if(bAsync == NO){
					dispatch_semaphore_signal(sema);
				}
				return;
			}
			
			duration = [self.asset duration];
			
			if(CMTimeCompare(duration, kCMTimeZero) == 0) {
				NSLog(@"track loaded with zero duration.");
				if(bAsync == NO){
					dispatch_semaphore_signal(sema);
				}
				return;
			}
			
			// TODO
			// why not reading infinite media?
			// how about playing back HLS streams?
			if(isfinite([self getDurationInSec]) == NO) {
				NSLog(@"track loaded with infinite duration.");
				if(bAsync == NO){
					dispatch_semaphore_signal(sema);
				}
				return;
			}
			
			BOOL bOk = [self createAssetReaderWithTimeRange:CMTimeRangeMake(kCMTimeZero, duration)];
			if(bOk == NO) {
				NSLog(@"problem with creating asset reader.");
				if(bAsync == NO){
					dispatch_semaphore_signal(sema);
				}
				return;
			}
			
			NSArray * videoTracks = [self.asset tracksWithMediaType:AVMediaTypeVideo];
			if([videoTracks count] == 0) {
				NSLog(@"no video tracks found.");
				if(bAsync == NO){
					dispatch_semaphore_signal(sema);
				}
				return;
			}
			
			AVAssetTrack * videoTrack = [videoTracks objectAtIndex:0];
			frameRate = videoTrack.nominalFrameRate;
			videoWidth = [videoTrack naturalSize].width;
			videoHeight = [videoTrack naturalSize].height;
			
			NSLog(@"video loaded at %li x %li @ %f fps", (long)videoWidth, (long)videoHeight, frameRate);
			
			currentTime = CMTimeMakeWithSeconds((1.0/frameRate), NSEC_PER_SEC);//kCMTimeZero;
			
			
			//------------------------------------------------------------ create player item.
			self.playerItem = [AVPlayerItem playerItemWithAsset:self.asset];
			
			if (self.playerItem) {
				
				[self.playerItem addObserver:self
								  forKeyPath:kStatusKey
									 options:0
									 context:&ItemStatusContext];
				
				NSNotificationCenter * notificationCenter = [NSNotificationCenter defaultCenter];
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
				
				// set playerItem
				[_player replaceCurrentItemWithPlayerItem:self.playerItem];
				
				// add timeobserver?
				[self addTimeObserverToPlayer];
				
				// loaded
				bLoaded = true;
				
			} else {
				NSLog(@"could not create AVPlayerItem");
			}
			
			if(bAsync == NO){
				dispatch_semaphore_signal(sema);
			}
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


#pragma mark - AVPlayerItemOutputPullDelegate

- (void)outputMediaDataWillChange:(AVPlayerItemOutput *)sender
{
	NSLog(@"outputMediaDataWillChange");
}


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




- (void)unloadVideo {
	
	bReady = NO;
	bLoaded = NO;
	bPlayStateBeforeLoad = NO;
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
	
	if(self.playerItem != nil) {
		[self.playerItem removeObserver:self forKeyPath:kStatusKey];
		
		NSNotificationCenter * notificationCenter = [NSNotificationCenter defaultCenter];
		[notificationCenter removeObserver:self
									  name:AVPlayerItemDidPlayToEndTimeNotification
									object:self.playerItem];
		
		//AVPlayerItemPlaybackStalledNotification only exists from OS X 10.9 or iOS 6.0 and up
		#if (__MAC_OS_X_VERSION_MIN_REQUIRED >= 1090) || (__IPHONE_OS_VERSION_MIN_REQUIRED >= 60000)
		[notificationCenter removeObserver:self
									  name:AVPlayerItemPlaybackStalledNotification
									object:self.playerItem];
		#endif
		
		self.playerItem = nil;
	}

	
	if(videoSampleBuffer) {
		CFRelease(videoSampleBuffer);
		videoSampleBuffer = nil;
	}
	
	if(audioSampleBuffer) {
		CFRelease(audioSampleBuffer);
		audioSampleBuffer = nil;
	}
	
	[self removeTimeObserverFromPlayer];
	
#if USE_VIDEO_OUTPUT
	// destroy video info
	if (_videoInfo) {
		CFRelease(_videoInfo);
	}
#endif
}

//---------------------------------------------------------- player callbacks.
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	
	if(context == &ItemStatusContext) {
		dispatch_async(dispatch_get_main_queue(), ^{
			bReady = true;
			
			[self update]; // update as soon is ready so pixels are loaded.
			[self setVolume:volume]; // set volume for current video.
			
			if(bAutoPlayOnLoad || bPlayStateBeforeLoad) {
				[self play];
			}
		});
		return;
	}
	
	if(![self isReady]) {
		return;
	}
	
	BOOL b1 = _player != nil;
	BOOL b2 = object == _player;
	BOOL b3 = [keyPath isEqualToString:kRateKey];
	
	if(b1 && b2 && b3) {
		float rate = [[change objectForKey:@"new"] floatValue];
		bPlaying = rate != 0;
		return;
	}
	
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
	if(![self isReady] || [self isFinished]) {
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

- (void)updateFromVideoOutput {
#if USE_VIDEO_OUTPUT
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
			return;
		}
		
		// release temp buffer
		CVBufferRelease(buffer);
		
		videoSampleTime = time;
		
		
	} else {
		// no new frame for time
		bNewFrame = NO;
	}
#endif
}

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
	
	if(self.assetReader.status != AVAssetReaderStatusReading) {
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
		NSLog(@"not ready");
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

- (float)getFrameRate{
	return frameRate;
}

- (int)getDurationInFrames{
	return [self getDurationInSec] * [self getFrameRate];
}

- (int)getCurrentFrameNum{
	return [self getCurrentTimeInSec] * [self getFrameRate];
}

- (void)setPosition:(float)position {
	double time = [self getDurationInSec] * position;
	[self seekToTime:CMTimeMakeWithSeconds(time, NSEC_PER_SEC)];
}

- (void)setFrame:(int)frame{
	float position = frame / (float)[self getDurationInFrames];
	[self setPosition:position];
}

- (float)getPosition {
	return ([self getCurrentTimeInSec] / [self getDurationInSec]);
}

- (void)setVolume:(float)value {
	
	if (self.playerItem == nil) {
		return;
	}
	if(![self isReady]) {
		return;
	}

	volume = value;
	
	NSArray * audioTracks = [self.playerItem.asset tracksWithMediaType:AVMediaTypeAudio];
	NSMutableArray * allAudioParams = [NSMutableArray array];
	for(AVAssetTrack * track in audioTracks) {
		AVMutableAudioMixInputParameters * audioInputParams = [AVMutableAudioMixInputParameters audioMixInputParameters];
		[audioInputParams setVolume:volume atTime:kCMTimeZero];
		[audioInputParams setTrackID:[track trackID]];
		[allAudioParams addObject:audioInputParams];
	}
	
	AVMutableAudioMix * audioMix = [AVMutableAudioMix audioMix];
	[audioMix setInputParameters:allAudioParams];
	
	[self.playerItem setAudioMix:audioMix];
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