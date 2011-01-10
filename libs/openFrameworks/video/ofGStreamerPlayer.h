#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofBaseVideoPlayer.h"

#ifdef OF_VIDEO_PLAYER_GSTREAMER
	#include "ofGstUtils.h"
#endif

class ofGStreamerPlayer : public ofBaseVideoPlayer{

	public:

		ofGStreamerPlayer();
		~ofGStreamerPlayer();

		 bool			loadMovie(string name);
		 void			closeMovie();	
		 void			idleMovie();

		 void			play();
		 void			stop();
		 
		 void			clearMemory();
	
		 bool 			isFrameNew();
		 unsigned char * 	getPixels();
		
		 float 			getWidth();
		 float 			getHeight();

		 bool			isPaused();
		 bool			isLoaded();
		 bool			isPlaying();		 

		 float 			getPosition();
		 float 			getDuration();
		 int			getTotalNumFrames();
		 float			getSpeed();
		 bool			getIsMovieDone();

		 void 			setPosition(float pct);
		 void 			setVolume(int volume);
		 void 			setLoopState(int state);
		 void   		setSpeed(float speed);
		 void			setFrame(int frame);  // frame 0 = first frame...
		 void 			setPaused(bool bPause);

		 int			getCurrentFrame();

		 void			firstFrame();
		 void			nextFrame();
		 void			previousFrame();
				
	protected:
		
		int				width, height;
		bool			bLoaded;
		bool			bHavePixelsChanged;
		int				nFrames;				// number of frames
		int				currentLoopState;
		bool 			bStarted;
		bool 			bPlaying;
		bool 			bPaused;
		bool 			bIsFrameNew;			// if we are new
		float			speed;			
	
		//--------------------------------------
		#ifdef OF_VIDEO_PLAYER_GSTREAMER
		//--------------------------------------
		ofGstUtils 			gstUtils;
		//--------------------------------------
		#endif
		//--------------------------------------


};





