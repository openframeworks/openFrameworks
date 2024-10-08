#pragma once
#include "ofxiOSConstants.h"
#if defined(TARGET_OS_IOS)
#include "ofVideoBaseTypes.h"
#include "ofTexture.h"
#include "ofPixels.h"

class ofxiOSVideoPlayer : public ofBaseVideoPlayer {
	
public:
	
	ofxiOSVideoPlayer();
	~ofxiOSVideoPlayer();
	   
	void enableTextureCache();
	void disableTextureCache();
	
	bool load(const of::filesystem::path & fileName);
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
	
	[[deprecated("use load()")]]
	bool loadMovie(const of::filesystem::path & fileName);
	[[deprecated("use getPixels()")]]
	ofPixels & getPixelsRef();
	[[deprecated("use getPixels()")]]
	const ofPixels & getPixelsRef() const;
	[[deprecated("use getTexturePtr()")]]
	ofTexture * getTexture();
	
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
#endif
