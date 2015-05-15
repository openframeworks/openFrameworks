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
	bool				load(string name);
	void				close();
	void				update();

	void				play();
	void				stop();

	bool 				isFrameNew() const;
	ofPixels & 			getPixels();
	const ofPixels & 	getPixels() const;
	ofTexture *			getTexture(); // if your videoplayer needs to implement seperate texture and pixel returns for performance, implement this function to return a texture instead of a pixel array. see iPhoneVideoGrabber for reference

	float 				getWidth() const;
	float 				getHeight() const;

	bool				isPaused() const;
	bool				isLoaded() const;
	bool				isPlaying() const;

	bool				setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat 		getPixelFormat() const;

	//should implement!
	float 				getPosition() const;
	float 				getSpeed() const;
	float 				getDuration() const;
	bool				getIsMovieDone() const;

	void 				setPaused(bool bPause);
	void 				setPosition(float pct);
	void 				setVolume(float volume); // 0..1
	void 				setLoopState(ofLoopType state);
	void   				setSpeed(float speed);
	void				setFrame(int frame);  // frame 0 = first frame...

	int					getCurrentFrame() const;
	int					getTotalNumFrames() const;
	ofLoopType			getLoopState() const;

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

