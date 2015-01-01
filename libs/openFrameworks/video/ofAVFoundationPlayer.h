//
//  ofAVFoundationPlayer.h
//  AVFoundationTest
//
//  Created by Sam Kronick on 5/31/13.
//  Modified by James George and Elie Zananiri.
//

#pragma once

#include "ofMain.h"

#ifdef __OBJC__
#import "ofAVFMovieRenderer.h"
#endif

class ofAVFoundationPlayer  : public ofBaseVideoPlayer
{
    
public:
    
    ofAVFoundationPlayer();
    ~ofAVFoundationPlayer();
    
    bool                load(string path);
    
    void                close();
    
    void                idleMovie();
    void                update();
    void                play();
    void                stop();
    
    bool                isFrameNew() const; //returns true if the frame has changed in this update cycle
    
    // Returns openFrameworks compatible RGBA pixels.
    // Be aware of your current render mode.
    const ofPixels &    getPixels() const;
    ofPixels &          getPixels();
    
    ofTexture *         getTexturePtr();
    ofTexture &         getTextureReference();
    
    bool                isLoading() const;
    bool                isLoaded() const;
    
    bool                isPlaying() const;
    bool                getIsMovieDone() const;
    
    float               getPosition() const;
    float               getCurrentTime() const;
    float               getPositionInSeconds() const;
    int                 getCurrentFrame() const;
    float               getDuration() const;
    int                 getTotalNumFrames() const;
    bool                isPaused() const;
    float               getSpeed() const;
    ofLoopType          getLoopState() const;
    float               getVolume() const;
    
    void                setPosition(float pct);
    void                setTime(float seconds);
    void                setPositionInSeconds(float seconds);
    void                setFrame(int frame); // frame 0 = first frame...
    void                setBalance(float balance);
    void                setPaused(bool bPaused);
    void                setSpeed(float speed);
    void                setLoopState(ofLoopType state);
    void                setVolume(float volume);
    
    // ofAVFoundationPlayer only supports OF_PIXELS_RGB and OF_PIXELS_RGBA.
    bool                setPixelFormat(ofPixelFormat pixelFormat);
    ofPixelFormat       getPixelFormat() const;
    
    void                draw(float x, float y, float w, float h);
    void                draw(float x, float y);
    
    float               getWidth() const;
    float               getHeight() const;
    
    void                firstFrame();
    void                nextFrame();
    void                previousFrame();
    
protected:
    
    ofLoopType currentLoopState;
        
    bool bPaused;
    bool bShouldPlay;
    
    float scrubToTime;
    bool bNewFrame;
    bool bHavePixelsChanged;
    
    float duration;
    float speed;
    
    string moviePath;
    
    bool bInitialized;
    
    void updateTexture();
    void reallocatePixels();
    
    ofTexture tex;
    ofPixels pixels;
    ofPixelFormat pixelFormat;
    
    // This #ifdef is so you can include this .h file in .cpp files
    // and avoid ugly casts in the .m file
#ifdef __OBJC__
    AVFMovieRenderer * moviePlayer;
#else
    void * moviePlayer;
#endif
    
};
