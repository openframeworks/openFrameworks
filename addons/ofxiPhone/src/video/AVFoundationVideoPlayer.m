//
//  AVFoundationVideoPlayer.m
//  iOS+OFLib
//
//  Created by lukasz karluk on 21/05/12.
//

#import "AVFoundationVideoPlayer.h"

/* Asset keys */
NSString * const kTracksKey         = @"tracks";
NSString * const kPlayableKey		= @"playable";

/* PlayerItem keys */
NSString * const kStatusKey         = @"status";

/* AVPlayer keys */
NSString * const kRateKey			= @"rate";
NSString * const kCurrentItemKey	= @"currentItem";

//---------------------------------------------------------- video player view.
@implementation AVFoundationVideoPlayerView

+ (Class)layerClass {
	return [AVPlayerLayer class];
}


- (AVPlayer*)player {
	return [(AVPlayerLayer*)[self layer] player];
}


- (void)setPlayer:(AVPlayer*)player {
	[(AVPlayerLayer*)[self layer] setPlayer:player];
}

- (void)dealloc {
    self.player = nil;
    [super dealloc];
}

@end

//---------------------------------------------------------- video player.
@implementation AVFoundationVideoPlayer {
    AVPlayer * _player;
    id timeObserver;
    int timeObserverFps;
    
	CMSampleBufferRef buffer;
	CMTime timestamp;
	CMTime timestampLast;
	CMTime duration;
    CMTime currentTime;
    float volume;
    float speed;
    float rateBeforeSeek;

    NSInteger videoWidth;
    NSInteger videoHeight;
    
    BOOL bWillBeUpdatedExternally;
    BOOL bReady;
    BOOL bPlayStateBeforeLoad;
    BOOL bUpdateFirstFrame;
    BOOL bNewFrame;
    BOOL bPlaying;
    bool bFinished;
    BOOL bAutoPlayOnLoad;
    BOOL bLoop;
}

@synthesize delegate;
@synthesize playerView;
@synthesize player = _player;
@synthesize playerItem;
@synthesize asset;
@synthesize assetReader;
@synthesize assetReaderVideoOutput;

static const NSString * ItemStatusContext;

- (id)init {
    self = [super init];
    if(self) {
        /**
         *  initialise video player view to full screen by default.
         *  later the view frame can be changed if need be.
         */
        self.playerView = [[[AVFoundationVideoPlayerView alloc] initWithFrame:[UIScreen mainScreen].bounds] autorelease];
        self.playerView.backgroundColor = [UIColor blackColor];
        
        self.player = [[[AVPlayer alloc] init] autorelease];
        [(AVFoundationVideoPlayerView *)self.playerView setPlayer:_player];
        
        [_player addObserver:self 
                  forKeyPath:kRateKey 
                     options:NSKeyValueObservingOptionNew 
                     context:nil];
        
        timeObserver = nil;
        timeObserverFps = 30;
        
        buffer = nil;
        timestamp = kCMTimeZero;
        timestampLast = kCMTimeZero;
        duration = kCMTimeZero;
        currentTime = kCMTimeZero;
        volume = 1;
        speed = 1;
        
        videoWidth = 0;
        videoHeight = 0;

        bWillBeUpdatedExternally = NO;
        bReady = NO;
        bPlayStateBeforeLoad = NO;
        bUpdateFirstFrame = YES;
        bNewFrame = NO;
        bPlaying = NO;
        bFinished = NO;
        bAutoPlayOnLoad = NO;
        bLoop = NO;
    }
    return self;
}

- (void)dealloc {
    
    [(AVFoundationVideoPlayerView *)self.playerView setPlayer:nil];
    [self.playerView removeFromSuperview];
    self.playerView = nil;
    
    if(self.playerItem != nil) {
        NSNotificationCenter * notificationCenter = [NSNotificationCenter defaultCenter];
        [notificationCenter removeObserver:self 
                                      name:AVPlayerItemDidPlayToEndTimeNotification 
                                    object:self.playerItem];
        [self.playerItem removeObserver:self forKeyPath:kStatusKey];
        
        self.playerItem = nil;
    }
    
    [self removeTimeObserverFromPlayer];
    [_player removeObserver:self forKeyPath:kRateKey];
    
    self.player = nil;
    [_player release];
    
    [self.assetReader cancelReading];
	self.assetReader = nil;
    self.assetReaderVideoOutput = nil;
    self.asset = nil;
    
    if(buffer) {
        CFRelease(buffer);
        buffer = nil;
    }
    
    [super dealloc];
}

//---------------------------------------------------------- position / size.
- (void)setVideoPosition:(CGPoint)position {
    CGRect playerViewFrame = self.playerView.frame;
    playerViewFrame.origin = position;
    self.playerView.frame = playerViewFrame;
}

- (void)setVideoSize:(CGSize)size {
    CGRect playerViewFrame = self.playerView.frame;
    playerViewFrame.size = size;
    self.playerView.frame = playerViewFrame;
}

//---------------------------------------------------------- load / unload.
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

    [self unloadVideo];     // unload video if one is already loaded.
    
    self.asset = [AVURLAsset URLAssetWithURL:url options:nil];
    if(!self.asset) {
        return NO;
    }
    
    [self.asset loadValuesAsynchronouslyForKeys:[NSArray arrayWithObject:kTracksKey] completionHandler:^{
        dispatch_async(dispatch_get_main_queue(), ^{
            
            NSError * error = nil;
            AVKeyValueStatus status = [self.asset statusOfValueForKey:kTracksKey error:&error];
            
            if(status == AVKeyValueStatusLoaded) {
                
                duration = [self.asset duration];

                if(CMTimeCompare(duration, kCMTimeZero) == 0) {
                    return; // duration is zero.
                }
                
                if(!isfinite([self getDurationInSec])) {
                    return; // duration is infinite.
                }
                
                BOOL bOk = [self createAssetReaderWithTimeRange:CMTimeRangeMake(kCMTimeZero, duration)];
                if(!bOk) {
                    return; // asset reader not created.
                }
                
                buffer = [self.assetReaderVideoOutput copyNextSampleBuffer];
                CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(buffer);
                CVPixelBufferLockBaseAddress(imageBuffer,0);
                videoWidth = CVPixelBufferGetWidth(imageBuffer);
                videoHeight = CVPixelBufferGetHeight(imageBuffer);
                CVPixelBufferUnlockBaseAddress(imageBuffer,0);
                
                NSLog(@"video loaded at %i x %i", videoWidth, videoHeight);
                
                CGRect playerViewFrame = self.playerView.frame;
                playerViewFrame.size.width = videoWidth;
                playerViewFrame.size.height = videoHeight;
                self.playerView.frame = playerViewFrame;
                
                //------------------------------------------------------------ create player item.
                self.playerItem = [AVPlayerItem playerItemWithAsset:self.asset];
                
                [self.playerItem addObserver:self 
                                  forKeyPath:kStatusKey
                                     options:0 
                                     context:&ItemStatusContext];
                
                NSNotificationCenter * notificationCenter = [NSNotificationCenter defaultCenter];
                [notificationCenter addObserver:self
                                       selector:@selector(playerItemDidReachEnd)
                                           name:AVPlayerItemDidPlayToEndTimeNotification
                                         object:self.playerItem];
                
                [_player replaceCurrentItemWithPlayerItem:self.playerItem];
                
                [self addTimeObserverToPlayer];
            }
            else {
                NSLog(@"The asset's tracks were not loaded:\n%@", [error localizedDescription]);
                return;
            }
        });
    }];
    
    return YES;
}

- (BOOL)createAssetReaderWithTimeRange:(CMTimeRange)timeRange {
    
    timestamp = timestampLast = timeRange.start;
    
    NSError *error = nil;
    self.assetReader = [AVAssetReader assetReaderWithAsset:self.asset error:&error];
    
    if(error) {
        NSLog(@"assetReader: error during initialisation: %@", [error localizedDescription]);
        return NO;
    }
    self.assetReader.timeRange = timeRange;
    
    NSMutableDictionary * dictionary = [[[NSMutableDictionary alloc] init] autorelease];
    [dictionary setObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] 
                   forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey];
    
    NSArray * videoTracks = [self.asset tracksWithMediaType:AVMediaTypeVideo];
    AVAssetTrack * videoTrack = [videoTracks objectAtIndex:0];
    self.assetReaderVideoOutput = [[[AVAssetReaderTrackOutput alloc] initWithTrack:videoTrack 
                                                                    outputSettings:dictionary] autorelease];
    
    if(self.assetReaderVideoOutput == nil) {
        NSLog(@"assetReaderVideoOutput initWithTrack failed for video track");
        return NO;
    }
    
    if(![self.assetReader canAddOutput:self.assetReaderVideoOutput]) {
        NSLog(@"cannot add video output");
        return NO;
    }
    
    [self.assetReader addOutput:self.assetReaderVideoOutput];
    
    BOOL bOk = [self.assetReader startReading];
    if(!bOk ) {
        NSLog(@"assetReader couldn't startReading: %@", [self.assetReader error]);
    }
    
    return YES;
}

- (void)unloadVideo {
    
    bReady = NO;
    bPlayStateBeforeLoad = NO;
    bUpdateFirstFrame = YES;
    bNewFrame = NO;
    bPlaying = NO;
    bFinished = NO;
    
    timestamp = kCMTimeZero;
    timestampLast = kCMTimeZero;
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
        self.playerItem = nil;
    }
    
    [self.assetReader cancelReading];
	self.assetReader = nil;
    self.assetReaderVideoOutput = nil;
    self.asset = nil;
    
    if(buffer) {
        CFRelease(buffer);
        buffer = nil;
    }
    
	[self removeTimeObserverFromPlayer];
}

//---------------------------------------------------------- player callbacks.
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
    
    if(context == &ItemStatusContext) {
        dispatch_async(dispatch_get_main_queue(), ^{
            bReady = true;
            [self update]; // update as soon is ready so pixels are loaded.
            [self setVolume:volume]; // set volume for current video.
            [self.delegate playerReady];
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
    [self.delegate playerDidFinishPlayingVideo];
    if(bLoop) {
        bFinished = NO;
        [self seekToStart];
        [self play];
    }
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
    
    CMTime time = [_player currentTime];
    if(bUpdateFirstFrame) {
        /**
         *  this forces the first frame to be updated.
         *  here the values for time and currentTime are both zero.
         *  so this is to get around the progress check below.
         */
        bUpdateFirstFrame = NO;
    } else if(CMTimeCompare(time, currentTime) == 0) {
        bNewFrame = NO;
        return; // no progress made.
    }
    currentTime = time;
    
    if(self.assetReader == nil) {
        [self createAssetReaderWithTimeRange:CMTimeRangeMake(currentTime, duration)];
    }
    
    if(self.assetReader.status != AVAssetReaderStatusReading) {
        bNewFrame = NO;
        return;
    }
    
    BOOL bOk = YES;
    while(self.assetReader.status == AVAssetReaderStatusReading &&  // asset read is in reading state.
          ((CMTimeCompare(timestamp, currentTime) == -1) ||         // timestamp is less then currentTime.
          (CMTimeCompare(timestamp, currentTime) == 0))) {          // timestamp is equal currentTime.

        CMSampleBufferRef bufferTemp = [self.assetReaderVideoOutput copyNextSampleBuffer];
        if(bufferTemp) {
            if(buffer) { // release old buffer.
                CFRelease(buffer);
                buffer = nil;
            }
            buffer = bufferTemp; // save reference to new buffer.

            timestamp = CMSampleBufferGetPresentationTimeStamp(buffer);
            
        } else {
            if(self.assetReader.status == AVAssetReaderStatusCompleted) {
                // video finished.
            }
            bOk = NO;
        }
    }
    
    if(bOk) {
        bNewFrame = CMTimeCompare(timestamp, timestampLast) == 1;
        if(bNewFrame) {
            timestampLast = timestamp;
        }
        
        [self.delegate playerDidProgress];
    }
}

- (void)addTimeObserverToPlayer {
    if(bWillBeUpdatedExternally) {
        return;
    }
    
	if(timeObserver){
		return;
    }
    
	double interval = 1.0 / (double)timeObserverFps;
	
	timeObserver = [[_player addPeriodicTimeObserverForInterval:CMTimeMakeWithSeconds(interval, NSEC_PER_SEC) 
                                                          queue:dispatch_get_main_queue() usingBlock:
                     ^(CMTime time) {
                         [self update];
                     }] retain];
}

- (void)removeTimeObserverFromPlayer {
	if(timeObserver) {
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
        [_player play];
    } else {
        [_player pause];
    }
}

//---------------------------------------------------------- seek.
- (void)seekToStart {
	[self seekToTime:kCMTimeZero withTolerance:kCMTimePositiveInfinity];
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
    
    [self.assetReader cancelReading];
	self.assetReader = nil;
    self.assetReaderVideoOutput = nil;
    
	[_player seekToTime:time toleranceBefore:tolerance toleranceAfter:tolerance];
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

//---------------------------------------------------------- getters / setters.
- (CVImageBufferRef)getCurrentFrame {
	return CMSampleBufferGetImageBuffer(buffer);
}

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
    return CMTimeGetSeconds(currentTime);
}

- (CMTime)getDuration {
    return duration;
}

- (double)getDurationInSec {
    return CMTimeGetSeconds(duration);
}

- (void)setPosition:(float)position {
    double time = [self getDurationInSec] * position;
    [self seekToTime:CMTimeMakeWithSeconds(time, NSEC_PER_SEC)];
}

- (float)getPosition {
    return ([self getCurrentTimeInSec] / [self getDurationInSec]);
}

- (void)setVolume:(float)value {
    volume = value;
    
    if(![self isReady]) {
        return;
    }
    
    NSArray * audioTracks = [self.asset tracksWithMediaType:AVMediaTypeAudio];
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

- (void)setLoop:(BOOL)value {
    bLoop = value;
}

- (BOOL)getLoop {
    return bLoop;
}

- (void)setSpeed:(float)value {
    speed = value;
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

//---------------------------------------------------------- uiimage.
UIImage * imageFromSampleBuffer(CMSampleBufferRef sampleBuffer) {
    
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    // Lock the base address of the pixel buffer.
    CVPixelBufferLockBaseAddress(imageBuffer,0);
    
    // Get the number of bytes per row for the pixel buffer.
    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
    // Get the pixel buffer width and height.
    size_t width = CVPixelBufferGetWidth(imageBuffer);
    size_t height = CVPixelBufferGetHeight(imageBuffer);
    
    // Create a device-dependent RGB color space.
    static CGColorSpaceRef colorSpace = NULL;
    if (colorSpace == NULL) {
        colorSpace = CGColorSpaceCreateDeviceRGB();
        if (colorSpace == NULL) {
            // Handle the error appropriately.
            return nil;
        }
    }
    
    // Get the base address of the pixel buffer.
    void *baseAddress = CVPixelBufferGetBaseAddress(imageBuffer);
    // Get the data size for contiguous planes of the pixel buffer.
    size_t bufferSize = CVPixelBufferGetDataSize(imageBuffer);
    
    // Create a Quartz direct-access data provider that uses data we supply.
    CGDataProviderRef dataProvider =
    CGDataProviderCreateWithData(NULL, baseAddress, bufferSize, NULL);
    // Create a bitmap image from data supplied by the data provider.
    CGImageRef cgImage =
    CGImageCreate(width, height, 8, 32, bytesPerRow,
                  colorSpace, kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Little,
                  dataProvider, NULL, true, kCGRenderingIntentDefault);
    CGDataProviderRelease(dataProvider);
    
    // Create and return an image object to represent the Quartz image.
    UIImage *image = [UIImage imageWithCGImage:cgImage];
    CGImageRelease(cgImage);
    
    CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
    
    return image;
}

@end
