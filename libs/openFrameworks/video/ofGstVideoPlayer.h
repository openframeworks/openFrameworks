#pragma once

#include "ofGstUtils.h"


class ofGstVideoPlayer: public ofGstVideoUtils, public ofBaseVideoPlayer{
public:

	ofGstVideoPlayer();
	~ofGstVideoPlayer();

	/// needs to be called before loadMovie
	void 	setPixelFormat(ofPixelFormat pixelFormat);
	bool 	loadMovie(string uri);

	void 	update(){ ofGstVideoUtils::update(); }

	int		getCurrentFrame();
	int		getTotalNumFrames();

	void 	firstFrame();
	void 	nextFrame();
	void 	previousFrame();
	void 	setFrame(int frame);  // frame 0 = first frame...

	bool	isStream();

	void 	play(){ofGstVideoUtils::play();}
	void 	stop(){ofGstVideoUtils::stop();}
	void 	setPaused(bool bPause){ofGstVideoUtils::setPaused(bPause);}
	bool 	isPaused(){return ofGstVideoUtils::isPaused();}
	bool 	isLoaded(){return ofGstVideoUtils::isLoaded();}
	bool 	isPlaying(){return ofGstVideoUtils::isPlaying();}

	float	getPosition(){ return ofGstVideoUtils::getPosition();}
	float 	getSpeed(){ return ofGstVideoUtils::getSpeed();}
	float 	getDuration(){ return ofGstVideoUtils::getDuration();}
	bool  	getIsMovieDone(){ return ofGstVideoUtils::getIsMovieDone();}

	void 	setPosition(float pct){ ofGstVideoUtils::setPosition(pct);}
	void 	setVolume(int volume){ ofGstVideoUtils::setVolume(volume);}
	void 	setLoopState(ofLoopType state){ ofGstVideoUtils::setLoopState(state);}
	int		getLoopState(){ return ofGstVideoUtils::getLoopState();}
	void 	setSpeed(float speed){ ofGstVideoUtils::setSpeed(speed);}
	void 	close(){ ofGstUtils::close();}

	bool 			isFrameNew(){ return ofGstVideoUtils::isFrameNew();}
	unsigned char * getPixels(){ return ofGstVideoUtils::getPixels();}
	float 			getHeight(){ return ofGstVideoUtils::getHeight();}
	float 			getWidth(){ return ofGstVideoUtils::getWidth();}
protected:
	bool	allocate();

private:
	ofPixelFormat		internalPixelFormat;
	guint64				nFrames;
	int 				fps_n, fps_d;
	bool				bIsStream;
};
