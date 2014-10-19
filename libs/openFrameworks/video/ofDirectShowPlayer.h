//DirectShowVideo and ofDirectShowPlayer written by Theodore Watson, Jan 2014
//See the cpp file for the DirectShow implementation 
//To allow for QuickTime video playback install the K-Lite Mega Codec Pack 10.2

#pragma once 
#include "ofMain.h"

class DirectShowVideo; 

class ofDirectShowPlayer : public ofBaseVideoPlayer{
	public:

		ofDirectShowPlayer();
		~ofDirectShowPlayer();

		bool loadMovie(string path);
		void update();
		void draw(float x, float y);

		 void				close();
	
		 void				play();
		 void				stop();		
	
		 bool 				isFrameNew();
		 unsigned char * 	getPixels();
	     ofPixelsRef		getPixelsRef();
	
		 float 				getWidth();
		 float 				getHeight();
	
		 bool				isPaused();
		 bool				isLoaded();
		 bool				isPlaying();
	
		 bool setPixelFormat(ofPixelFormat pixelFormat);
		 ofPixelFormat 		getPixelFormat();

		 float 				getPosition();
		 float 				getSpeed();
		 float 				getDuration();
		 bool				getIsMovieDone();
	
		 void 				setPaused(bool bPause);
		 void 				setPosition(float pct);
		 void 				setVolume(float volume); // 0..1
		 void 				setLoopState(ofLoopType state);
		 void   			setSpeed(float speed);
		 void				setFrame(int frame);  // frame 0 = first frame...
	
		 int				getCurrentFrame();
		 int				getTotalNumFrames();
		 ofLoopType			getLoopState();
	
		 void				firstFrame();
		 void				nextFrame();
		 void				previousFrame();

	protected:
		DirectShowVideo * player; 
		ofPixels pix;
};