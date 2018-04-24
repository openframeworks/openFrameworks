#pragma once

#include "ofConstants.h"
#include "ofVideoBaseTypes.h"
#include "ofPixels.h"

#if !defined(TARGET_LINUX) && (!defined(MAC_OS_X_VERSION_10_12) || MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_12)

#ifdef OF_VIDEO_PLAYER_QUICKTIME
	#include "ofQtUtils.h"

class ofQuickTimePlayer : public ofBaseVideoPlayer{

	public:

		ofQuickTimePlayer();
		~ofQuickTimePlayer();

		 bool			load(string name);
		 void			closeMovie();
		 void			close();
		 void			update();

		 void			play();
		 void			stop();

		 void			clearMemory();

		bool            setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat   getPixelFormat() const;

		 bool 			isFrameNew() const;

		 ofPixels&			getPixels();
		 const ofPixels&	getPixels() const;

		 float 			getWidth() const;
		 float 			getHeight() const;

		 bool			isPaused() const;
		 bool			isLoaded() const;
		 bool			isPlaying() const;

		 float 			getPosition() const;
		 float 			getDuration() const;
		 int			getTotalNumFrames() const;
		 float			getSpeed() const;
		 bool			getIsMovieDone() const;
		 ofLoopType 	getLoopState() const;

		 void 			setPosition(float pct);
		 void 			setVolume(float volume);
		 void 			setLoopState(ofLoopType state);
		 void   		setSpeed(float speed);
		 void			setFrame(int frame);  // frame 0 = first frame...
		 void 			setPaused(bool bPause);

		 int			getCurrentFrame() const;

		 void			firstFrame();
		 void			nextFrame();
		 void			previousFrame();

		 mutable bool	bHavePixelsChanged;



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

#endif
