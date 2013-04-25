/*
 * ofxAndroidVideoPlayer.h
 *
 *  Created on: 25/04/2013
 *      Author: p
 */

#pragma once

#include "ofBaseTypes.h"
#include "ofPixels.h"
#include "ofEvents.h"
#include "ofTexture.h"
#include <jni.h>

class ofxAndroidVideoPlayer: public ofBaseVideoPlayer{

	public:

		ofxAndroidVideoPlayer();
		virtual ~ofxAndroidVideoPlayer();

		bool loadMovie(string fileName);
		void close();
		void update();

		void play();
		void stop();

		ofTexture *	getTexture();
		ofMatrix4x4 & getTextureMatrix();

		float getWidth();
		float getHeight();

		//

		bool isFrameNew(){ return bIsFrameNew;};
		unsigned char * getPixels(){ return NULL;};
		ofPixels& getPixelsRef(){return pixels;}

		bool isPaused(){ return false;};
		bool isLoaded(){ return false;};
		bool isPlaying(){ return false;};

		bool setPixelFormat(ofPixelFormat pixelFormat){ return false;};
		ofPixelFormat getPixelFormat(){ return OF_PIXELS_RGBA;};


	/*
		virtual void				close() = 0;
		virtual void				update() = 0;

		virtual void				play() = 0;
		virtual void				stop() = 0;

		virtual bool 				isFrameNew() = 0;
		virtual unsigned char * 	getPixels() = 0;
		virtual ofTexture *			getTexture(){return NULL;}; // if your videoplayer needs to implement seperate texture and pixel returns for performance, implement this function to return a texture instead of a pixel array. see iPhoneVideoGrabber for reference

		virtual float 				getWidth() = 0;
		virtual float 				getHeight() = 0;

		virtual bool				isPaused() = 0;
		virtual bool				isLoaded() = 0;
		virtual bool				isPlaying() = 0;

		virtual bool				setPixelFormat(ofPixelFormat pixelFormat) = 0;
		virtual ofPixelFormat 		getPixelFormat() = 0;

		//should implement!
		virtual float 				getPosition();
		virtual float 				getSpeed();
		virtual float 				getDuration();
		virtual bool				getIsMovieDone();

		virtual void 				setPaused(bool bPause);
		virtual void 				setPosition(float pct);
		virtual void 				setVolume(float volume); // 0..1
		virtual void 				setLoopState(ofLoopType state);
		virtual void   				setSpeed(float speed);
		virtual void				setFrame(int frame);  // frame 0 = first frame...

		virtual int					getCurrentFrame();
		virtual int					getTotalNumFrames();
		virtual ofLoopType			getLoopState();

		virtual void				firstFrame();
		virtual void				nextFrame();
		virtual void				previousFrame();
	*/

	/*

	//needs implementing
	void	listDevices();
	bool	initGrabber(int w, int h);

	bool	isFrameNew();
	void	update();

	unsigned char 	* getPixels();
	ofPixelsRef		getPixelsRef();

	void	close();

	float	getHeight();
	float	getWidth();

	//should implement!
	void setVerbose(bool bTalkToMe);
	void setDeviceID(int _deviceID);
	void setDesiredFrameRate(int framerate);
	void videoSettings();
	bool setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat getPixelFormat();

	// specifics android

	bool setAutoFocus(bool autofocus);

	ofEvent<ofPixels> newFrameE;

	// only to be used internally to resize;
	ofPixelsRef getAuxBuffer();

	*/

	private:

		jobject javaVideoPlayer;
		jclass javaClass;

		int width;
		int height;
		ofTexture texture;
		ofPixels pixels;

		jfloatArray matrixJava;
		ofMatrix4x4 matrix;

		bool  bIsFrameNew;

	/*

	int attemptFramerate;
	bool bIsFrameNew;
	bool bGrabberInited;
	ofPixelFormat internalPixelFormat;
	ofPixels pixels;
	ofPixels auxBuffer;
	*/

};
