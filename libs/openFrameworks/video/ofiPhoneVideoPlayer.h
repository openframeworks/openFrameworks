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
	void setPixelFormat(ofPixelFormat PixelFormat);
	void close();
	
	void play();
	void stop();
	
	bool isFrameNew();
	unsigned char * getPixels();
	ofPixelsRef		getPixelsRef();
	ofTexture * getTexture();
	
	float getWidth();
	float getHeight();
	
	bool isPaused();
	bool isLoaded();
	bool isPlaying();
	
	void update();
	
	float getPosition();
	float getDuration();
	bool getIsMovieDone();
	void setPaused(bool bPause);
	
	void setVolume(int volume);
	
	/*should implement! (but cannot on iphone)
	float getSpeed();
	void setPosition(float pct);
	void setLoopState(int state);
	void setSpeed(float speed);
	void setFrame(int frame);  // frame 0 = first frame...
	
	int getCurrentFrame();
	int getTotalNumFrames();
	int getLoopState();
	
	void firstFrame();
	void nextFrame();
	void previousFrame();*/
	
protected:
	
	void initWithPath(string path);
	void updateCurrentFrameRef();
	
	void * videoPlayer; // super hack to forward declare an objective c class inside a header file that can only handle c classes.
	
	string videoPath;	
	bool videoWasStopped;
	int width;
	int height;
	float playbackSpeed;
    
    bool bFrameNew;
    bool bUpdatePixels;
    int frameCount;
	
	int vol;
		
	GLubyte * pixels;
	ofTexture videoTexture;
	
	float lastUpdateTime;
	GLint internalGLFormat;
	GLubyte *pixelsTmp;
};

#endif
