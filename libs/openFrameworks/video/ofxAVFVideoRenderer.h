

//
//  ofxAVFVideoRenderer.h
//  AVFoundationTest
//
//  Created by Sam Kronick on 5/31/13.
//
//


#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>
#import <AVFoundation/AVFoundation.h>
#import <OpenGL/OpenGL.h>

@interface AVFVideoRenderer : NSObject
{
    BOOL _bTheFutureIsNow;
    
    AVPlayer * _player;
    AVPlayerItem * _playerItem;

    // New school video stuff
    id _playerItemVideoOutput;
    CVOpenGLTextureCacheRef _textureCache;
	CVOpenGLTextureRef _latestTextureFrame;
	CVPixelBufferRef _latestPixelFrame;

    // Old school video stuff
    CARenderer * _layerRenderer;
    
	BOOL _useTexture;
    BOOL _useAlpha;
    
    CGSize _videoSize;
    
    CMTime _currentTime;
    CMTime _duration;
    double _frameRate;
    double _playbackRate;
    BOOL _bLoops;
    
    BOOL _bLoading;
    BOOL _bLoaded;
    BOOL _bAudioLoaded;
    BOOL _bPaused;
    BOOL _bMovieDone;
    	
    // New school audio stuff
    NSMutableData *_amplitudes;
    int _numAmplitudes;
    id _periodicTimeObserver;
}

@property (nonatomic, retain) AVPlayer * player;

@property (nonatomic, assign, readonly, getter = theFutureIsNow) BOOL bTheFutureIsNow;

@property (nonatomic, assign, readonly) double width;
@property (nonatomic, assign, readonly) double height;

@property (nonatomic, assign, readonly, getter = isLoading) BOOL bLoading;
@property (nonatomic, assign, readonly, getter = isLoaded) BOOL bLoaded;
@property (nonatomic, assign, readonly, getter = isAudioLoaded) BOOL bAudioLoaded;
@property (nonatomic, assign, getter = isPaused, setter = setPaused:) BOOL bPaused;
@property (nonatomic, assign, readonly, getter = isMovieDone) BOOL bMovieDone;
@property (nonatomic, assign, readonly) BOOL isPlaying;

@property (nonatomic, assign) BOOL useAlpha;
@property (nonatomic, assign) BOOL useTexture;

@property (nonatomic, assign, readonly) BOOL textureAllocated;
@property (nonatomic, assign, readonly) GLuint textureID;
@property (nonatomic, assign, readonly) GLenum textureTarget;

@property (nonatomic, assign, readonly) double frameRate;
@property (nonatomic, assign, readonly) double duration;
@property (nonatomic, assign, readonly) int totalFrames;
@property (nonatomic, assign) double currentTime;
@property (nonatomic, assign) int currentFrame;
@property (nonatomic, assign) double position;
@property (nonatomic, assign) double playbackRate;
@property (nonatomic, assign, getter = loops, setter = setLoops:) BOOL bLoops;
@property (nonatomic, assign) float volume;

@property (nonatomic, retain, readonly) NSMutableData* amplitudes;
@property (nonatomic, assign, readonly) int numAmplitudes;

- (void)loadFilePath:(NSString *)filePath;
- (void)loadURLPath:(NSString *)urlPath;
- (void)loadURL:(NSURL *)url;

- (void)play;
- (void)stop;

// New school video stuff
- (BOOL)update;
- (void)bindTexture;
- (void)unbindTexture;
- (void)pixels:(unsigned char *)outbuf;

// Old school video stuff
- (void)render;

@end
