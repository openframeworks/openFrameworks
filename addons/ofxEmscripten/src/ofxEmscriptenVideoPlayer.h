/*
 * ofEmscriptenVideoPlayer.h
 *
 *  Created on: May 10, 2014
 *      Author: arturo
 */
#pragma once

#include "ofBaseTypes.h"
#include "ofTexture.h"
#include "ofPixels.h"

class ofxEmscriptenVideoPlayer: public ofBaseVideoPlayer {
public:
	ofxEmscriptenVideoPlayer();
	~ofxEmscriptenVideoPlayer();

	//needs implementing
	bool				loadMovie(string name);
	void				close();
	void				update();

	void				play();
	void				stop();

	bool 				isFrameNew();
	unsigned char * 	getPixels();
	ofPixels & 			getPixelsRef();
	ofTexture *			getTexture(); // if your videoplayer needs to implement seperate texture and pixel returns for performance, implement this function to return a texture instead of a pixel array. see iPhoneVideoGrabber for reference

	float 				getWidth();
	float 				getHeight();

	bool				isPaused();
	bool				isLoaded();
	bool				isPlaying();

	bool				setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat 		getPixelFormat();

	//should implement!
	float 				getPosition();
	float 				getSpeed();
	float 				getDuration();
	bool				getIsMovieDone();

	void 				setPaused(bool bPause);
	void 				setPosition(float pct);
	void 				setVolume(float volume); // 0..1
	void 				setLoopState(ofLoopType state);
	void   				setSpeed(float speed);
	void				setFrame(int frame);  // frame 0 = first frame...

	int					getCurrentFrame();
	int					getTotalNumFrames();
	ofLoopType			getLoopState();

	void				firstFrame();
	void				nextFrame();
	void				previousFrame();

	void				setUsePixels(bool usePixels);
private:
	int id;
	ofTexture texture;
	ofPixels pixels;
	bool gotFirstFrame;
	bool usePixels;
};

