#ifndef _OF_IPHONE_VIDEO_PLAYER
#define _OF_IPHONE_VIDEO_PLAYER

#include "ofPixels.h"
#include "ofBaseTypes.h"
#include "ofTexture.h"

#include "ofVideoPlayer.h"

class ofiPhoneVideoPlayer : public ofBaseVideoPlayer {
	
public:
	
	ofiPhoneVideoPlayer();
	~ofiPhoneVideoPlayer();
    
    bool loadMovie(string name);
    void close();
    void update();
	
    void play();
    void stop();
	
    bool isFrameNew();
    unsigned char * getPixels();
    ofPixelsRef	getPixelsRef();
    ofTexture *	getTexture();
	
    float getWidth();
    float getHeight();
	
    bool isPaused();
    bool isLoaded();
    bool isPlaying();
	
    float getPosition();
    float getSpeed();
    float getDuration();
    bool getIsMovieDone();
	
    void setPaused(bool bPause);
    void setPosition(float pct);
    void setVolume(float volume); // 0..1
    void setLoopState(ofLoopType state);
    void setSpeed(float speed);
    void setFrame(int frame);  // frame 0 = first frame...
	
    int	getCurrentFrame();
    int	getTotalNumFrames();
    int	getLoopState();
	
    void firstFrame();
    void nextFrame();
    void previousFrame();
    void setPixelFormat(ofPixelFormat pixelFormat);
    
	void * getAVFoundationVideoPlayer();
    
protected:
    
    void updatePixelsToRGB();
	
	void * videoPlayer; // super hack to forward declare an objective c class inside a header file that can only handle c classes.
	
    bool bFrameNew;
    bool bResetPixels;
    bool bResetTexture;
    bool bUpdatePixels;
    bool bUpdatePixelsToRgb;
    bool bUpdateTexture;
    bool bTextureCacheSupported;
    bool bTextureHack;
	
	GLubyte * pixelsRGB;
    GLubyte * pixelsRGBA;
    GLint internalGLFormat;
	ofTexture videoTexture;
};

#endif
