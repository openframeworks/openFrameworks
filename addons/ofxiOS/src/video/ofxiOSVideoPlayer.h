#pragma once

#include "ofPixels.h"
#include "ofBaseTypes.h"
#include "ofTexture.h"

#include "ofVideoPlayer.h"

class ofxiOSVideoPlayer : public ofBaseVideoPlayer {
	
public:
	
	ofxiOSVideoPlayer();
	~ofxiOSVideoPlayer();
	   
    void enableTextureCache();
    void disableTextureCache();
    
    bool load(string name);
    void close();
    void update();
	
	bool setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat getPixelFormat() const;
	
    void play();
    void stop();
	
    bool isFrameNew() const;
    ofPixels & getPixels();
    const ofPixels & getPixels() const;
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
    ofLoopType	getLoopState() const;
	
    void firstFrame();
    void nextFrame();
    void previousFrame();
    
	void * getAVFoundationVideoPlayer();
    
    OF_DEPRECATED_MSG("ofxiOSVideoPlayer::loadMovie() is deprecated, use load() instead.", bool loadMovie(string name));
    OF_DEPRECATED_MSG("ofxiOSVideoPlayer::getPixelsRef() is deprecated, use getPixels() instead.", ofPixels & getPixelsRef());
    OF_DEPRECATED_MSG("ofxiOSVideoPlayer::getPixelsRef() is deprecated, use getPixels() instead.", const ofPixels & getPixelsRef() const);
    OF_DEPRECATED_MSG("ofxiOSVideoPlayer::getTexture() is deprecated, use getTexturePtr() instead.", ofTexture * getTexture());
    
protected:
    
	void * videoPlayer; // super hack to forward declare an objective c class inside a header file that can only handle c classes.
	
    bool bFrameNew;
    bool bResetPixels;
    bool bUpdatePixels;
    bool bUpdateTexture;
    bool bTextureCacheSupported;
    bool bTextureCacheEnabled;
	
    ofPixels pixels;
	ofPixelFormat pixelFormat;
	ofTexture videoTexture;
};

#define ofxiPhoneVideoPlayer ofxiOSVideoPlayer

