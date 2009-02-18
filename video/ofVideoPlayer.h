#ifndef _OF_VIDEO_PLAYER
#define _OF_VIDEO_PLAYER

#include "ofConstants.h"
#include "ofTexture.h"


#ifdef OF_VIDEO_PLAYER_GSTREAMER
	#include <gst/gst.h>
	#include <pthread.h>

	typedef struct{
		GMainLoop 		*	loop;
		GstElement 		*	pipeline;
		unsigned char 	*	pixels;
		pthread_mutex_t 	buffer_mutex;
		bool				bHasPixelsChanged;

		guint64				durationNanos;
		guint64				nFrames;
		int					pipelineState;
		float				speed;
	}ofGstVideoData;

#else
	#include "ofQtUtils.h"
#endif



#define OF_LOOP_NONE					0x01
#define OF_LOOP_PALINDROME				0x02
#define OF_LOOP_NORMAL					0x03


//---------------------------------------------

class ofVideoPlayer : public ofBaseVideo{

	public:


		ofVideoPlayer ();
		virtual ~ofVideoPlayer();

		bool 				loadMovie(string name);
		void 				closeMovie();
		void 				close();

		void				update();			//same as idleMovie
		void 				idleMovie();		// rename to updateMovie?
		void 				play();
		void 				stop();

		int 				width, height;
		float  				speed;
		bool 				bLoaded;

		bool 				isFrameNew();
		unsigned char * 	getPixels();
		float 				getPosition();
		float 				getSpeed();
		float 				getDuration();
		bool				getIsMovieDone();

		void 				setPosition(float pct);
		void 				setVolume(int volume);
		void 				setLoopState(int state);
		void   				setSpeed(float speed);
		void				setFrame(int frame);  // frame 0 = first frame...

		void 				setUseTexture(bool bUse);
		ofTexture &			getTextureReference();
		void 				draw(float x, float y, float w, float h);
		void 				draw(float x, float y);

		//a more accurate way to set the point which the image is drawn from
		//this makes rotating an image around a point easier.
        void                setAnchorPct(float xPct, float yPct);
        void                setAnchorPt(int x, int y);
        void                resetAnchor();

		void 				setPaused(bool bPause);

		int					getCurrentFrame();
		int					getTotalNumFrames();

		void				firstFrame();
		void				nextFrame();
		void				previousFrame();

		float 				getHeight();
		float 				getWidth();

		//--------------------------------------
		#ifdef OF_VIDEO_PLAYER_QUICKTIME
		//--------------------------------------
			MovieController  	thePlayer;
			GWorldPtr 			offscreenGWorld;
			Movie 			 	moviePtr;
			unsigned char * 	offscreenGWorldPixels;	// 32 bit: argb (qt k32ARGBPixelFormat)
			void				qtGetFrameCount(Movie & movForcount);
		//--------------------------------------
		#endif
		//--------------------------------------

		int					nFrames;				// number of frames
		unsigned char * 	pixels;					// 24 bit: rgb
		bool 				bHavePixelsChanged;
		ofTexture 			tex;					// a ptr to the texture we are utilizing
		bool 				bUseTexture;			// are we using a texture
		bool				allocated;				// so we know to free pixels or not

	protected:


		void 				start();
		void 				createImgMemAndGWorld();
		bool 				bStarted;
		bool 				bPlaying;
		bool 				bPaused;
		bool 				bIsFrameNew;			// if we are new

		//--------------------------------------
		#ifdef OF_VIDEO_PLAYER_GSTREAMER
		//--------------------------------------
		ofGstVideoData 		gstData;
		bool				bIsMovieDone;
		bool				isStream;
		GstElement	* 		gstPipeline;
		GstElement  *		gstSink;
		gint64          	durationNanos;
		int					loopMode;

		bool				posChangingPaused;


		void 				gstHandleMessage();
		bool 				allocate();
		//--------------------------------------
		#endif
		//--------------------------------------


};
#endif






