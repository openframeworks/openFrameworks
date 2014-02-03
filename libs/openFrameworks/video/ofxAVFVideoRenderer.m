//
//  ofxAVFVideoRenderer.m
//  AVFoundationTest
//
//  Created by Sam Kronick on 5/31/13.
//
//

#import "ofxAVFVideoRenderer.h"
#import <Accelerate/Accelerate.h>

@interface AVFVideoRenderer ()

- (void)playerItemDidReachEnd:(NSNotification *) notification;
- (NSDictionary *)pixelBufferAttributes;
- (void)render;

@property (nonatomic, retain) AVPlayerItem * playerItem;
@property (nonatomic, retain) id playerItemVideoOutput;

@end

@implementation AVFVideoRenderer

@synthesize player = _player;
@synthesize playerItem = _playerItem;
@synthesize playerItemVideoOutput = _playerItemVideoOutput;
@synthesize bTheFutureIsNow = _bTheFutureIsNow;

@synthesize useTexture = _useTexture;
@synthesize useAlpha = _useAlpha;

@synthesize bLoading = _bLoading;
@synthesize bLoaded = _bLoaded;
@synthesize bAudioLoaded = _bAudioLoaded;
@synthesize bPaused = _bPaused;
@synthesize bMovieDone = _bMovieDone;

@synthesize frameRate = _frameRate;
@synthesize playbackRate = _playbackRate;
@synthesize bLoops = _bLoops;

@synthesize amplitudes = _amplitudes;
@synthesize numAmplitudes = _numAmplitudes;

int count = 0;

//--------------------------------------------------------------
- (id)init
{
    self = [super init];
    if (self) {
        _bTheFutureIsNow = (NSClassFromString(@"AVPlayerItemVideoOutput") != nil);
        NSLog(@"Is this the future? %d", _bTheFutureIsNow);
        
        if (self.theFutureIsNow) {
            self.player = [[AVPlayer alloc] init];
			[self.player autorelease];
            _amplitudes = [[NSMutableData data] retain];
        }
        
        _bLoading = NO;
        _bLoaded = NO;
        _bAudioLoaded = NO;
        _bPaused = NO;
        _bMovieDone = NO;
        
        _useTexture = YES;
        _useAlpha = NO;
        
        _frameRate = 0.0;
        _playbackRate = 1.0;
        _bLoops = false;
    }
    return self;
}

//--------------------------------------------------------------
- (NSDictionary *)pixelBufferAttributes
{
    // kCVPixelFormatType_32ARGB, kCVPixelFormatType_32BGRA, kCVPixelFormatType_422YpCbCr8
    return @{
             (NSString *)kCVPixelBufferOpenGLCompatibilityKey : [NSNumber numberWithBool:self.useTexture],
             (NSString *)kCVPixelBufferPixelFormatTypeKey     : [NSNumber numberWithInt:kCVPixelFormatType_32ARGB]  //[NSNumber numberWithInt:kCVPixelFormatType_422YpCbCr8]
            };
}

//--------------------------------------------------------------
- (void)loadFilePath:(NSString *)filePath
{
    [self loadURL:[NSURL fileURLWithPath:[filePath stringByStandardizingPath]]];
}

//--------------------------------------------------------------
- (void)loadURLPath:(NSString *)urlPath
{
    [self loadURL:[NSURL URLWithString:urlPath]];
}

//--------------------------------------------------------------
- (void)loadURL:(NSURL *)url
{
    _bLoading = YES;
    _bLoaded = NO;
    _bAudioLoaded = NO;
    _bPaused = NO;
    _bMovieDone = NO;
    
    _frameRate = 0.0;
    _playbackRate = 1.0;
    
//    _useTexture = YES;
//    _useAlpha = NO;
    
    if (_amplitudes) {
        [_amplitudes setLength:0];
    }
    _numAmplitudes = 0;
        
    NSLog(@"Loading %@", [url absoluteString]);
    
    AVURLAsset *asset = [AVURLAsset URLAssetWithURL:url options:nil];
    NSString *tracksKey = @"tracks";
    
    [asset loadValuesAsynchronouslyForKeys:@[tracksKey] completionHandler: ^{
        static const NSString *kItemStatusContext;
        // Perform the following back on the main thread
        dispatch_async(dispatch_get_main_queue(), ^{
            // Check to see if the file loaded
            NSError *error;
            AVKeyValueStatus status = [asset statusOfValueForKey:tracksKey error:&error];
            
            if (status == AVKeyValueStatusLoaded) {
                // Asset metadata has been loaded, set up the player.
                
                // Extract the video track to get the video size and other properties.
                AVAssetTrack *videoTrack = [[asset tracksWithMediaType:AVMediaTypeVideo] objectAtIndex:0];
                _videoSize = [videoTrack naturalSize];
                _currentTime = kCMTimeZero;
                _duration = asset.duration;
                _frameRate = [videoTrack nominalFrameRate];
                
                self.playerItem = [AVPlayerItem playerItemWithAsset:asset];
                [self.playerItem addObserver:self forKeyPath:@"status" options:0 context:&kItemStatusContext];
                
                // Notify this object when the player reaches the end
                // This allows us to loop the video
                [[NSNotificationCenter defaultCenter] addObserver:self
                                                         selector:@selector(playerItemDidReachEnd:)
                                                             name:AVPlayerItemDidPlayToEndTimeNotification
                                                           object:self.playerItem];
                
                if (self.theFutureIsNow) {
                    [self.player replaceCurrentItemWithPlayerItem:self.playerItem];

                    // Create and attach video output. 10.8 Only!!!
                    self.playerItemVideoOutput = [[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:[self pixelBufferAttributes]];
					[self.playerItemVideoOutput autorelease];
                    if (self.playerItemVideoOutput) {
                        [(AVPlayerItemVideoOutput *)self.playerItemVideoOutput setSuppressesPlayerRendering:YES];
                    }
                    [self.player.currentItem addOutput:self.playerItemVideoOutput];
                    
                    // Create CVOpenGLTextureCacheRef for optimal CVPixelBufferRef to GL texture conversion.
                    if (self.useTexture && !_textureCache) {
                        CVReturn err = CVOpenGLTextureCacheCreate(kCFAllocatorDefault, NULL,
                                                                  CGLGetCurrentContext(), CGLGetPixelFormat(CGLGetCurrentContext()),
                                                                  NULL, &_textureCache);
                                                                  //(CFDictionaryRef)ctxAttributes, &_textureCache);
                        if (err != noErr) {
                            NSLog(@"Error at CVOpenGLTextureCacheCreate %d", err);
                        }
                    }
                }
                else {
                    self.player = [AVPlayer playerWithPlayerItem:self.playerItem];
                    
                    AVPlayerLayer * playerLayer = [AVPlayerLayer playerLayerWithPlayer:self.player];
                    _layerRenderer = [CARenderer rendererWithCGLContext:CGLGetCurrentContext() options:nil];
                    _layerRenderer.layer = playerLayer;
                    
                    // Video is centered on 0,0 for some reason so layer bounds have to start at -width/2,-height/2
                    _layerRenderer.bounds = CGRectMake(_videoSize.width * -0.5, _videoSize.height * -0.5, _videoSize.width, _videoSize.height);
                    playerLayer.bounds = _layerRenderer.bounds;
                }
                
                if (self.theFutureIsNow) {
                    // Only monitor audio if the file is local and has audio tracks.
                    NSArray *audioTracks = [asset tracksWithMediaType:AVMediaTypeAudio];
                    if ([url isFileURL] && [audioTracks count] > 0) {
                        AVAssetTrack *audioTrack = [[asset tracksWithMediaType:AVMediaTypeAudio] objectAtIndex:0];

                        NSError *error = nil;
                        AVAssetReader *assetReader = [[AVAssetReader alloc] initWithAsset:asset error:&error];
                        if (error != nil) {
                            NSLog(@"Unable to create asset reader %@", [error localizedDescription]);
                        }
                        else if (audioTrack != nil) {
                            // Read the audio track data
                            NSMutableDictionary *bufferOptions = [NSMutableDictionary dictionary];
                            [bufferOptions setObject:[NSNumber numberWithInt:kAudioFormatLinearPCM] forKey:AVFormatIDKey];
                            [bufferOptions setObject:@44100 forKey:AVSampleRateKey];
                            [bufferOptions setObject:@2 forKey:AVNumberOfChannelsKey];
    //                        [bufferOptions setObject:[NSData dataWithBytes:&channelLayout length:sizeof(AudioChannelLayout)] forKey:AVChannelLayoutKey];
                            [bufferOptions setObject:@32 forKey:AVLinearPCMBitDepthKey];
                            [bufferOptions setObject:@NO forKey:AVLinearPCMIsBigEndianKey];
                            [bufferOptions setObject:@YES forKey:AVLinearPCMIsFloatKey];
                            [bufferOptions setObject:@NO forKey:AVLinearPCMIsNonInterleaved];
                            [assetReader addOutput:[AVAssetReaderTrackOutput assetReaderTrackOutputWithTrack:audioTrack
                                                                                              outputSettings:bufferOptions]];
                            [assetReader startReading];
                            
                            count = 0;
                        
                            // Add a periodic time observer that will store the audio track data in a buffer that we can access later
                            _periodicTimeObserver = [self.player addPeriodicTimeObserverForInterval:CMTimeMake(1001, [audioTrack nominalFrameRate] * 1001)
                                                                                              queue:dispatch_queue_create("eventQueue", NULL)
                                                                                         usingBlock:^(CMTime time) {
                                                                                             if ([assetReader status] == AVAssetReaderStatusCompleted) {
                                                                                                 // Got all the data we need, kill this block.
                                                                                                 [self.player removeTimeObserver:_periodicTimeObserver];
                                                                                                 
                                                                                                 _numAmplitudes = [_amplitudes length] / sizeof(float);
                                                                                                 _bAudioLoaded = YES;
                                                                                                 
                                                                                                 return;
                                                                                             }
                                                                                             
                                                                                             if ([assetReader status] == AVAssetReaderStatusReading) {
                                                                                                 AVAssetReaderTrackOutput *output = [[assetReader outputs] objectAtIndex:0];
                                                                                                 CMSampleBufferRef sampleBuffer = [output copyNextSampleBuffer];
                                                                                                 
                                                                                                 while (sampleBuffer != NULL) {
                                                                                                     sampleBuffer = [output copyNextSampleBuffer];
                                                                                                     
                                                                                                     if (sampleBuffer == NULL)
                                                                                                         continue;
                                                                                                     
                                                                                                     CMBlockBufferRef buffer = CMSampleBufferGetDataBuffer(sampleBuffer);
                                                                                                     
                                                                                                     size_t lengthAtOffset;
                                                                                                     size_t totalLength;
                                                                                                     char* data;
                                                                                                     
                                                                                                     if (CMBlockBufferGetDataPointer(buffer, 0, &lengthAtOffset, &totalLength, &data) != noErr) {
                                                                                                         NSLog(@"error!");
                                                                                                         break;
                                                                                                     }
                                                                                                     
                                                                                                     CMItemCount numSamplesInBuffer = CMSampleBufferGetNumSamples(sampleBuffer);
                                                                                                     
                                                                                                     AudioBufferList audioBufferList;
                                                                                                     
                                                                                                     CMSampleBufferGetAudioBufferListWithRetainedBlockBuffer(sampleBuffer,
                                                                                                                                                             NULL,
                                                                                                                                                             &audioBufferList,
                                                                                                                                                             sizeof(audioBufferList),
                                                                                                                                                             NULL,
                                                                                                                                                             NULL,
                                                                                                                                                             kCMSampleBufferFlag_AudioBufferList_Assure16ByteAlignment,  // pass in something else
                                                                                                                                                             &buffer);
                                                                                                     
                                                                                                     for (int bufferCount = 0; bufferCount < audioBufferList.mNumberBuffers; bufferCount++) {
                                                                                                         [_amplitudes appendBytes:audioBufferList.mBuffers[bufferCount].mData
                                                                                                                           length:audioBufferList.mBuffers[bufferCount].mDataByteSize];
                                                                                                     }
                                                                                                     
                                                                                                     CFRelease(buffer);
                                                                                                     CFRelease(sampleBuffer);
                                                                                                 }
                                                                                             }
                                                                                         }];
                        }
                    }
                }
                
                _bLoading = NO;
                _bLoaded = YES;
            }
            else {
                _bLoading = NO;
                _bLoaded = NO;
                NSLog(@"There was an error loading the file: %@", error);
            }
        });
    }];
}

//--------------------------------------------------------------
- (void)dealloc
{
    [self stop];
            
    if (self.theFutureIsNow) {
        self.playerItemVideoOutput = nil;
    
        if (_textureCache != NULL) {
            CVOpenGLTextureCacheRelease(_textureCache);
            _textureCache = NULL;
        }
        if (_latestTextureFrame != NULL) {
            CVOpenGLTextureRelease(_latestTextureFrame);
            _latestTextureFrame = NULL;
        }
        if (_latestPixelFrame != NULL) {
            CVPixelBufferRelease(_latestPixelFrame);
            _latestPixelFrame = NULL;
        }
        
        if (_amplitudes) {
            [_amplitudes release];
            _amplitudes = nil;
        }
        _numAmplitudes = 0;
    }
    else {
        // SK: Releasing the CARenderer is slow for some reason
        //     It will freeze the main thread for a few dozen mS.
        //     If you're swapping in and out videos a lot, the loadFile:
        //     method should be re-written to just re-use and re-size
        //     these layers/objects rather than releasing and reallocating
        //     them every time a new file is needed.
        
        if (_layerRenderer) {
            [_layerRenderer release];
            _layerRenderer = nil;
        }
    }
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    if (self.playerItem) {
        [self.playerItem removeObserver:self forKeyPath:@"status"];
        self.playerItem = nil;
    }

    [self.player replaceCurrentItemWithPlayerItem:nil];
    self.player = nil;

    [super dealloc];
}

//--------------------------------------------------------------
- (void)play
{
    [self.player play];
    self.player.rate = _playbackRate;
}

//--------------------------------------------------------------
- (void)stop
{
    // Pause and rewind.
    [self.player pause];
    [self.player seekToTime:kCMTimeZero];
}

//--------------------------------------------------------------
- (void)setPaused:(BOOL)bPaused
{
    _bPaused = bPaused;
    if (_bPaused) {
        [self.player pause];
    }
    else {
        [self.player play];
        self.player.rate = _playbackRate;
    }
}

//--------------------------------------------------------------
- (BOOL)isPlaying
{
    if (![self isLoaded]) return NO;
    
	return ![self isMovieDone] && ![self isPaused];
}

//--------------------------------------------------------------
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    // Keep this around for now, maybe we'll need it.
}

//--------------------------------------------------------------
- (void)playerItemDidReachEnd:(NSNotification *)notification
{
    _bMovieDone = YES;
    
    if (self.bLoops) {
        //start over
        _bMovieDone = NO;
        [self stop];
        [self play];
    }
}

//--------------------------------------------------------------
- (BOOL)update
{
    if (self.theFutureIsNow == NO) return YES;
    
    if (![self isLoaded]) return NO;

    // Check our video output for new frames.
    CMTime outputItemTime = [self.playerItemVideoOutput itemTimeForHostTime:CACurrentMediaTime()];
    if ([self.playerItemVideoOutput hasNewPixelBufferForItemTime:outputItemTime]) {
        // Get pixels.
        if (_latestPixelFrame != NULL) {
            CVPixelBufferRelease(_latestPixelFrame);
            _latestPixelFrame = NULL;
        }
        _latestPixelFrame = [self.playerItemVideoOutput copyPixelBufferForItemTime:outputItemTime
                                                            itemTimeForDisplay:NULL];
        
        if (self.useTexture) {
            // Create GL texture.
            if (_latestTextureFrame != NULL) {
                CVOpenGLTextureRelease(_latestTextureFrame);
                _latestTextureFrame = NULL;
                CVOpenGLTextureCacheFlush(_textureCache, 0);
            }
            
            CVReturn err = CVOpenGLTextureCacheCreateTextureFromImage(NULL, _textureCache, _latestPixelFrame, NULL, &_latestTextureFrame);
            if (err != noErr) {
                NSLog(@"Error creating OpenGL texture %d", err);
            }
        }
                
        // Update time.
        _currentTime = self.player.currentItem.currentTime;
        _duration = self.player.currentItem.duration;
        
        return YES;
    }
    
    return NO;
}

//--------------------------------------------------------------
- (void)render
{
    if (self.theFutureIsNow) return;
    
    // From https://qt.gitorious.org/qt/qtmultimedia/blobs/700b4cdf42335ad02ff308cddbfc37b8d49a1e71/src/plugins/avfoundation/mediaplayer/avfvideoframerenderer.mm
    
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glViewport(0, 0, _videoSize.width, _videoSize.height);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    glOrtho(0.0f, _videoSize.width, _videoSize.height, 0.0f, 0.0f, 1.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glTranslatef(_videoSize.width * 0.5, _videoSize.height * 0.5, 0);
    
    [_layerRenderer beginFrameAtTime:CACurrentMediaTime() timeStamp:NULL];
    [_layerRenderer addUpdateRect:_layerRenderer.layer.bounds];
    [_layerRenderer render];
    [_layerRenderer endFrame];
    
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glPopAttrib();
    
    glFinish(); //Rendering needs to be done before passing texture to video frame
}

#pragma mark - Pixels and Texture

//--------------------------------------------------------------
- (double)width
{
    return _videoSize.width;
}

//--------------------------------------------------------------
- (double)height
{
    return _videoSize.height;
}

//--------------------------------------------------------------
- (void)pixels:(unsigned char *)outbuf
{
    if (self.theFutureIsNow == NO) return;
    
    if (_latestPixelFrame == NULL) return;
		
//    NSLog(@"pixel buffer width is %ld height %ld and bpr %ld, movie size is %d x %d ",
//      CVPixelBufferGetWidth(_latestPixelFrame),
//      CVPixelBufferGetHeight(_latestPixelFrame),
//      CVPixelBufferGetBytesPerRow(_latestPixelFrame),
//      (NSInteger)movieSize.width, (NSInteger)movieSize.height);
    if ((NSInteger)self.width != CVPixelBufferGetWidth(_latestPixelFrame) || (NSInteger)self.height != CVPixelBufferGetHeight(_latestPixelFrame)) {
        NSLog(@"CoreVideo pixel buffer is %ld x %ld while self reports size of %d x %d. This is most likely caused by a non-square pixel video format such as HDV. Open this video in texture only mode to view it at the appropriate size",
              CVPixelBufferGetWidth(_latestPixelFrame), CVPixelBufferGetHeight(_latestPixelFrame), (NSInteger)self.width, (NSInteger)self.height);
        return;
    }
    
    if (CVPixelBufferGetPixelFormatType(_latestPixelFrame) != kCVPixelFormatType_32ARGB) {
        NSLog(@"QTKitMovieRenderer - Frame pixelformat not kCVPixelFormatType_32ARGB: %d, instead %ld", kCVPixelFormatType_32ARGB, CVPixelBufferGetPixelFormatType(_latestPixelFrame));
        return;
    }
    
    CVPixelBufferLockBaseAddress(_latestPixelFrame, kCVPixelBufferLock_ReadOnly);
    //If we are using alpha, the ofxAVFVideoPlayer class will have allocated a buffer of size
    //video.width * video.height * 4
    //CoreVideo creates alpha video in the format ARGB, and openFrameworks expects RGBA,
    //so we need to swap the alpha around using a vImage permutation
    vImage_Buffer src = {
        CVPixelBufferGetBaseAddress(_latestPixelFrame),
        CVPixelBufferGetHeight(_latestPixelFrame),
        CVPixelBufferGetWidth(_latestPixelFrame),
        CVPixelBufferGetBytesPerRow(_latestPixelFrame)
    };
    vImage_Error err;
    if (self.useAlpha) {
        vImage_Buffer dest = { outbuf, self.height, self.width, self.width * 4 };
        uint8_t permuteMap[4] = { 1, 2, 3, 0 }; //swizzle the alpha around to the end to make ARGB -> RGBA
        err = vImagePermuteChannels_ARGB8888(&src, &dest, permuteMap, 0);
    }
    //If we are are doing RGB then ofxAVFVideoPlayer will have created a buffer of size video.width * video.height * 3
    //so we use vImage to copy them into the out buffer
    else {
        vImage_Buffer dest = { outbuf, self.height, self.width, self.width * 3 };
        err = vImageConvert_ARGB8888toRGB888(&src, &dest, 0);
    }
    
    CVPixelBufferUnlockBaseAddress(_latestPixelFrame, kCVPixelBufferLock_ReadOnly);
    
    if (err != kvImageNoError) {
        NSLog(@"Error in Pixel Copy vImage_error %ld", err);
    }
}

//--------------------------------------------------------------
- (BOOL)textureAllocated
{
    if (self.theFutureIsNow == NO) return NO;
    
    return self.useTexture && _latestTextureFrame != NULL;
}

//--------------------------------------------------------------
- (GLuint)textureID
{
    if (self.theFutureIsNow == NO) return -1;
    
    return CVOpenGLTextureGetName(_latestTextureFrame);
}

//--------------------------------------------------------------
- (GLenum)textureTarget
{
    if (self.theFutureIsNow == NO) return 0;

    return CVOpenGLTextureGetTarget(_latestTextureFrame);
}

//--------------------------------------------------------------
- (void)bindTexture
{
    if (self.theFutureIsNow == NO) return;
    
    if (!self.textureAllocated) return;
    
	GLuint texID = [self textureID];
	GLenum target = [self textureTarget];
	
	glEnable(target);
	glBindTexture(target, texID);
}

//--------------------------------------------------------------
- (void)unbindTexture
{
    if (self.theFutureIsNow == NO) return;

    if (!self.textureAllocated) return;
	
	GLenum target = [self textureTarget];
	glDisable(target);
}

#pragma mark - Playhead

//--------------------------------------------------------------
- (double)duration
{
    return CMTimeGetSeconds(_duration);
}

//--------------------------------------------------------------
- (int)totalFrames
{
    return self.duration * self.frameRate;
}

//--------------------------------------------------------------
- (double)currentTime
{
    if (self.theFutureIsNow) {
        return CMTimeGetSeconds(_currentTime);
    }

    return CMTimeGetSeconds(self.player.currentTime);
}

//--------------------------------------------------------------
- (void)setCurrentTime:(double)currentTime
{
    [_player seekToTime:CMTimeMakeWithSeconds(currentTime, _duration.timescale)];
}

//--------------------------------------------------------------
- (int)currentFrame
{
    return self.currentTime * self.frameRate;
}

//--------------------------------------------------------------
- (void)setCurrentFrame:(int)currentFrame
{
    float position = currentFrame / (float)self.totalFrames;
    [self setPosition:position];
}

//--------------------------------------------------------------
- (double)position
{
    return self.currentTime / self.duration;
}

//--------------------------------------------------------------
- (void)setPosition:(double)position
{
    double time = self.duration * position;
//    [self.player seekToTime:CMTimeMakeWithSeconds(time, NSEC_PER_SEC)];
    [_player seekToTime:CMTimeMakeWithSeconds(time, _duration.timescale)];
}

//--------------------------------------------------------------
- (void)setPlaybackRate:(double)playbackRate
{
    _playbackRate = playbackRate;
    [_player setRate:_playbackRate];
}

//--------------------------------------------------------------
- (float)volume
{
    return self.player.volume;
}

//--------------------------------------------------------------
- (void)setVolume:(float)volume
{
    self.player.volume = volume;
}

@end
