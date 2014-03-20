#pragma once

#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofPixels.h"

#ifdef OF_VIDEO_PLAYER_QUICKTIME
	#include "ofQtUtils.h"

class ofQuickTimePlayer : public ofBaseVideoPlayer{

	public:

		ofQuickTimePlayer();
		~ofQuickTimePlayer();

		 bool			loadMovie(string name);
		 void			closeMovie();	
		 void			close();
		 void			update();

		 void			play();
		 void			stop();
		 
		 void			clearMemory();
		 
		bool            setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat   getPixelFormat();		 
	
		 bool 			isFrameNew();
		 unsigned char * 	getPixels();
		 ofPixelsRef		getPixelsRef();
		 const ofPixels&	getPixelsRef() const;
		
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
		 ofLoopType 	getLoopState();

		 void 			setPosition(float pct);
		 void 			setVolume(float volume);
		 void 			setLoopState(ofLoopType state);
		 void   		setSpeed(float speed);
		 void			setFrame(int frame);  // frame 0 = first frame...
		 void 			setPaused(bool bPause);

		 int			getCurrentFrame();

		 void			firstFrame();
		 void			nextFrame();
		 void			previousFrame();
		 
		bool 				bHavePixelsChanged;
		 
		 
		
	protected:
		void createImgMemAndGWorld();
		void start();

		ofPixels		 	pixels;
		int					width, height;
		bool				bLoaded;

		//these are public because the ofQuickTimePlayer implementation has some callback functions that need access
		//todo - fix this

		int					nFrames;				// number of frames
		bool				allocated;				// so we know to free pixels or not

		ofLoopType			currentLoopState;
		bool 				bStarted;
		bool 				bPlaying;
		bool 				bPaused;
		bool 				bIsFrameNew;			// if we are new
		float				speed;		
		
		MovieDrawingCompleteUPP myDrawCompleteProc;
		MovieController  	thePlayer;
		GWorldPtr 			offscreenGWorld;
		Movie 			 	moviePtr;
		unsigned char * 	offscreenGWorldPixels;	// 32 bit: argb (qt k32ARGBPixelFormat)
		void				qtGetFrameCount(Movie & movForcount);

};

#endif





