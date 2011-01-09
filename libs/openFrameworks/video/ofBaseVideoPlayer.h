#pragma once
#include "ofConstants.h"

#ifdef OF_VIDEO_PLAYER_GSTREAMER
	#include "ofGstUtils.h"
#else
	#include "ofQtUtils.h"
#endif


//---------------------------------------------

class ofBaseVideoPlayer{

	public:

		ofBaseVideoPlayer();
		virtual ~ofBaseVideoPlayer();

		//needs implementing
		virtual bool				loadMovie(string name) = 0;
		virtual void				closeMovie() = 0;
		virtual void				idleMovie() = 0;

		virtual void				play() = 0;
		virtual void				stop() = 0;		
	
		virtual void				clearMemory() = 0;
			
		virtual bool 				isFrameNew() = 0;
		virtual unsigned char * 	getPixels() = 0;
		
		virtual float 				getWidth() = 0;
		virtual float 				getHeight() = 0;

		virtual bool				isPaused() = 0;
		virtual bool				isLoaded() = 0;
		virtual bool				isPlaying() = 0;
		
		//should implement!
		virtual float 				getPosition();
		virtual float 				getSpeed();
		virtual float 				getDuration();
		virtual bool				getIsMovieDone();
		
		virtual void 				setPaused(bool bPause);
		virtual void 				setPosition(float pct);
		virtual void 				setVolume(int volume);
		virtual void 				setLoopState(int state);
		virtual void   				setSpeed(float speed);
		virtual void				setFrame(int frame);  // frame 0 = first frame...

		virtual int					getCurrentFrame();
		virtual int					getTotalNumFrames();
		virtual int					getLoopState();

		virtual void				firstFrame();
		virtual void				nextFrame();
		virtual void				previousFrame();
		

};






