#pragma once

#include "ofGstUtils.h"


class ofGstVideoPlayer: public ofBaseVideoPlayer, public ofGstAppSink{
public:

	ofGstVideoPlayer();
	~ofGstVideoPlayer();

	/// needs to be called before loadMovie
	bool 	setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat	getPixelFormat();
	
	bool 	loadMovie(string uri);

	void 	update();

	int		getCurrentFrame();
	int		getTotalNumFrames();

	void 	firstFrame();
	void 	nextFrame();
	void 	previousFrame();
	void 	setFrame(int frame);  // frame 0 = first frame...

	bool	isStream();

	void 	play();
	void 	stop();
	void 	setPaused(bool bPause);
	bool 	isPaused();
	bool 	isLoaded();
	bool 	isPlaying();

	float	getPosition();
	float 	getSpeed();
	float 	getDuration();
	bool  	getIsMovieDone();

	void 	setPosition(float pct);
	void 	setVolume(float volume);
	void 	setLoopState(ofLoopType state);
	ofLoopType 	getLoopState();
	void 	setSpeed(float speed);
	void 	close();

	bool 			isFrameNew();

	unsigned char * getPixels();
	ofPixelsRef		getPixelsRef();

	float 			getHeight();
	float 			getWidth();

	void setFrameByFrame(bool frameByFrame);
	void setThreadAppSink(bool threaded);

	ofGstVideoUtils * getGstVideoUtils();

protected:
	bool	allocate(int bpp);
	void	on_stream_prepared();

	// return true to set the message as attended so upstream doesn't try to process it
	virtual bool on_message(GstMessage* msg){return false;};

private:
	ofPixelFormat		internalPixelFormat;
	guint64				nFrames;
	int 				fps_n, fps_d;
	bool				bIsStream;
	bool				bIsAllocated;
	bool				threadAppSink;
	ofGstVideoUtils		videoUtils;
};
