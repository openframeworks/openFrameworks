//
//  ofAVFoundationPlayer.h
//  Created by Lukasz Karluk on 06/07/14.
//	Merged with code by Sam Kronick, James George and Elie Zananiri.
//

#pragma once

#include "ofBaseTypes.h"
#include "ofPixels.h"
#include "ofTexture.h"

#ifdef __OBJC__
#import "ofAVFoundationVideoPlayer.h"
#endif

class ofAVFoundationPlayer : public ofBaseVideoPlayer {
	
public:
	
	ofAVFoundationPlayer();
	~ofAVFoundationPlayer();
	   
    bool load(string name);
	void loadAsync(string name);
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

#ifdef __OBJC__
	ofAVFoundationVideoPlayer * getAVFoundationVideoPlayer();
#else
	void * getAVFoundationVideoPlayer();
#endif
    
    OF_DEPRECATED_MSG("ofAVFoundationPlayer::loadMovie() is deprecated, use load() instead.", bool loadMovie(string name));
    OF_DEPRECATED_MSG("ofAVFoundationPlayer::getPixelsRef() is deprecated, use getPixels() instead.", ofPixels & getPixelsRef());
    OF_DEPRECATED_MSG("ofAVFoundationPlayer::getPixelsRef() is deprecated, use getPixels() instead.", const ofPixels & getPixelsRef() const);
    OF_DEPRECATED_MSG("ofAVFoundationPlayer::getTexture() is deprecated, use getTexturePtr() instead.", ofTexture * getTexture());
    
protected:
	
	bool loadPlayer(string name, bool bAsync);

#ifdef __OBJC__
	ofAVFoundationVideoPlayer * videoPlayer;
#else
	void * videoPlayer;
#endif
    
    bool bFrameNew;
    bool bResetPixels;
    bool bUpdatePixels;
    bool bUpdateTexture;
    bool bTextureCacheSupported;
	
    ofPixels pixels;
	ofPixelFormat pixelFormat;
	ofTexture videoTexture;
};

