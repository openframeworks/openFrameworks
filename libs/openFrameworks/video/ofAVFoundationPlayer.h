//
//  ofAVFoundationPlayer.h
//  Created by Lukasz Karluk on 06/07/14.
//	Merged with code by Sam Kronick, James George and Elie Zananiri.
//

#pragma once

#include "ofVideoBaseTypes.h"
#include "ofTexture.h"
#include "ofThread.h"
#include "ofPixels.h"

#ifdef __OBJC__
#import "ofAVFoundationVideoPlayer.h"
#endif

#if defined TARGET_OF_IOS || defined TARGET_OSX
#import <CoreVideo/CoreVideo.h>
#endif

class ofAVFoundationPlayer : public ofBaseVideoPlayer {
	
public:
	
	ofAVFoundationPlayer();
	~ofAVFoundationPlayer();
	   
    bool load(std::string name);
	void loadAsync(std::string name);
    void close();
    void update();

    void draw();
    void draw(float x, float y);
    void draw(const ofRectangle & rect);
    void draw(float x, float y, float w, float h);
    
	bool setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat getPixelFormat() const;
	
    void play();
    void stop();
	
    bool isFrameNew() const;
    const ofPixels & getPixels() const;
    ofPixels & getPixels();
    ofTexture * getTexturePtr();
    void initTextureCache();
    void killTexture();
    void killTextureCache();
	
    float getWidth() const;
    float getHeight() const;
	
    bool isPaused() const;
    bool isLoaded() const;
    bool isPlaying() const;
	
    float getPosition() const;
    float getSpeed() const;
    float getDuration() const;
    bool getIsMovieDone() const;
	
    void setPaused(bool bPause);
    void setPosition(float pct);
    void setVolume(float volume); // 0..1
    void setLoopState(ofLoopType state);
    void setSpeed(float speed);
    void setFrame(int frame);  // frame 0 = first frame...
	
    int	getCurrentFrame() const;
    int	getTotalNumFrames() const;
    ofLoopType getLoopState() const;
	
    void firstFrame();
    void nextFrame();
    void previousFrame();

	ofAVFoundationPlayer& operator=(ofAVFoundationPlayer other);
	
#ifdef __OBJC__
	ofAVFoundationVideoPlayer * getAVFoundationVideoPlayer();
#else
	void * getAVFoundationVideoPlayer();
#endif
    
    OF_DEPRECATED_MSG("ofAVFoundationPlayer::loadMovie() is deprecated, use load() instead.", bool loadMovie(std::string name));
    OF_DEPRECATED_MSG("ofAVFoundationPlayer::getPixelsRef() is deprecated, use getPixels() instead.", ofPixels & getPixelsRef());
    OF_DEPRECATED_MSG("ofAVFoundationPlayer::getPixelsRef() is deprecated, use getPixels() instead.", const ofPixels & getPixelsRef() const);
    OF_DEPRECATED_MSG("ofAVFoundationPlayer::getTexture() is deprecated, use getTexturePtr() instead.", ofTexture * getTexture());
    
protected:
	
    bool loadPlayer(std::string name, bool bAsync);
	void disposePlayer();
    bool isReady() const;

#ifdef __OBJC__
    ofAVFoundationVideoPlayer * videoPlayer;
#else
    void * videoPlayer;
#endif
    
    bool bFrameNew;
    bool bResetPixels;
    bool bUpdatePixels;
    bool bUpdateTexture;
	bool bUseTextureCache;
	
    ofPixels pixels;
	ofPixelFormat pixelFormat;
	ofTexture videoTexture;
	
#ifdef TARGET_OF_IOS
	CVOpenGLESTextureCacheRef _videoTextureCache = nullptr;
	CVOpenGLESTextureRef _videoTextureRef = nullptr;
#endif
	
#ifdef TARGET_OSX
	CVOpenGLTextureCacheRef _videoTextureCache = nullptr;
	CVOpenGLTextureRef _videoTextureRef = nullptr;
#endif
};

