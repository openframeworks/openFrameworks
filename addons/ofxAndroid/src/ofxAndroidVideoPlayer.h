/*
 * ofxAndroidVideoPlayer.h
 *
 *  Created on: 25/04/2013
 *      Author: p
 */

#pragma once

#include "ofVideoBaseTypes.h"
#include "ofPixels.h"
#include "ofEvents.h"
#include "ofTexture.h"
#include <jni.h>

class ofxAndroidVideoPlayer: public ofBaseVideoPlayer{

	public:

		ofxAndroidVideoPlayer();
		virtual ~ofxAndroidVideoPlayer();

		bool load(std::string fileName);
		void close(); // empty!
		void update();

		void play();
		void stop();

		bool isLoaded() const;
		bool isPlaying() const;
		bool isPaused() const;
		bool isFrameNew() const { return bIsFrameNew;};

		ofTexture *	getTexturePtr();

		float getWidth() const;
		float getHeight() const;

        ofPixels& getPixels() {return pixels;}
        const ofPixels& getPixels() const {return pixels;}

		bool setPixelFormat(ofPixelFormat pixelFormat){ return false;};
		ofPixelFormat getPixelFormat() const { return OF_PIXELS_RGBA;};

		//should implement!
		//float getSpeed();
		float getPosition() const;
		float getDuration() const;
		bool  getIsMovieDone() const;

		void setPosition(float pct);
		void setPaused(bool bPause);
		void setVolume(float volume); // 0..1
		void setLoopState(ofLoopType state);
		//void setSpeed(float speed);
		//void setFrame(int frame);  // frame 0 = first frame...

		ofLoopType getLoopState() const;
		//int	getCurrentFrame();
		//int	getTotalNumFrames();

		//void firstFrame();
		//void nextFrame();
		//void previousFrame();

	private:
		void reloadTexture();
		void unloadTexture();

		jobject javaVideoPlayer;
		jclass javaClass;

		int width;
		int height;
		ofTexture texture;

		ofPixels pixels;

		jfloatArray matrixJava;

		bool bIsFrameNew;

};
