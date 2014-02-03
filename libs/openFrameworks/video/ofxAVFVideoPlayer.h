//
//  ofxAVFVideoPlayer.h
//  AVFoundationTest
//
//  Created by Sam Kronick on 5/31/13.
//
//

#pragma once

#include "ofMain.h"

#ifdef __OBJC__
#import "ofxAVFVideoRenderer.h"
#endif

class ofxAVFVideoPlayer  : public ofBaseVideoPlayer {
    
public:
    
    ofxAVFVideoPlayer();
    ~ofxAVFVideoPlayer();
    
    bool                loadMovie(string path);
    
    void                closeMovie();
    void                close();
    
    void                idleMovie();
    void                update();
    void                play();
    void                stop();
    
    float               getAmplitude(int channel = 0);
    float               getAmplitudeAt(float pos, int channel = 0);
    float *             getAllAmplitudes();
    int                 getNumAmplitudes();
	
    bool                isFrameNew(); //returns true if the frame has changed in this update cycle
    
    // Returns openFrameworks compatible RGBA pixels.
    // Be aware of your current render mode.
    
    unsigned char *     getPixels();
    ofPixelsRef         getPixelsRef();
    
    // Returns openFrameworks compatible ofTexture pointer.
    // if decodeMode == OF_QTKIT_DECODE_PIXELS_ONLY,
    // the returned pointer will be NULL.
    ofTexture *         getTexture();
    ofTexture&          getTextureReference();
    
    bool                isLoading();
    bool                isLoaded();
    bool                isAudioLoaded();
    bool                errorLoading();
    
    bool                isPlaying();
    bool                getIsMovieDone();
    
    float               getPosition();
    float               getCurrentTime();
    float               getPositionInSeconds();
    int                 getCurrentFrame();
    float               getDuration();
    int                 getTotalNumFrames();
    bool                isPaused();
    float               getSpeed();
    ofLoopType          getLoopState();
    float               getVolume();
    
    void                setPosition(float pct);
	void                setTime(float seconds);
    void                setPositionInSeconds(float seconds);
    void                setFrame(int frame); // frame 0 = first frame...
    void                setBalance(float balance);
    void                setPaused(bool bPaused);
    void                setSpeed(float speed);
    void                setLoopState(ofLoopType state);
    void                setVolume(float volume);
    
    // ofxAVFVideoPlayer only supports OF_PIXELS_RGB and OF_PIXELS_RGBA.
    bool                setPixelFormat(ofPixelFormat pixelFormat);
    ofPixelFormat       getPixelFormat();
    
    void                draw(float x, float y, float w, float h);
    void                draw(float x, float y);
    
    float               getWidth();
    float               getHeight();
    
    void                firstFrame();
    void                nextFrame();
    void                previousFrame();
    
protected:
    
    ofLoopType currentLoopState;
    
    bool bTheFutureIsNow;
    
    bool bPaused;
	bool bShouldPlay;
	
	float scrubToTime;
    bool bNewFrame;
    bool bHavePixelsChanged;
    
    float duration;
    float speed;
    
    string moviePath;
    
    bool bInitialized;
    void exit(ofEventArgs& args);
    
    void updateTexture();
    void reallocatePixels();
    
    ofFbo fbo;
    ofTexture tex;
    ofPixels pixels;
    ofPixelFormat pixelFormat;
    
    // This #ifdef is so you can include this .h file in .cpp files
    // and avoid ugly casts in the .m file
#ifdef __OBJC__
    AVFVideoRenderer* moviePlayer;
#else
    void * moviePlayer;
#endif
    
};
