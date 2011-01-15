#ifndef _OF_IPHONE_VIDEO_PLAYER
#define _OF_IPHONE_VIDEO_PLAYER

#include "ofMain.h"

class ofiPhoneVideoPlayer : public ofBaseVideoPlayer {
	
public:
	
	ofiPhoneVideoPlayer();
	~ofiPhoneVideoPlayer();
	
	bool loadMovie(string name);
	void close();
	
	void play();
	void stop();
	
	bool isFrameNew();
	unsigned char * getPixels();
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
	
	/*should implement! (but cannot on iphone)
	float getSpeed();
	void setPosition(float pct);
	void setVolume(int volume);
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
		
	unsigned char * pixels;
	ofTexture videoTexture;
	
	float lastUpdateTime;
};

#endif