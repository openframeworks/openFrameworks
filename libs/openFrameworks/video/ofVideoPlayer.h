#ifndef _OF_VIDEO_PLAYER
#define _OF_VIDEO_PLAYER

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofBaseVideoPlayer.h"

#ifdef OF_VIDEO_PLAYER_GSTREAMER
	#include "ofGStreamerPlayer.h"
	#define OF_VID_PLAYER_TYPE ofGStreamerPlayer()
#endif

#ifdef OF_VIDEO_PLAYER_QUICKTIME
	#include "ofQuickTimePlayer.h"
	#define OF_VID_PLAYER_TYPE ofQuickTimePlayer()	
#endif

//---------------------------------------------
class ofVideoPlayer : public ofBaseVideo{

	public:

		ofVideoPlayer ();
		virtual ~ofVideoPlayer();

		bool				setPlayer(ofBaseVideoPlayer * newPlayer);
		ofBaseVideoPlayer *	getPlayer();

		bool 				loadMovie(string name);
		void 				closeMovie();
		void 				close();

		void				update();			//same as idleMovie
		void 				idleMovie();		// rename to updateMovie?
		void 				play();
		void 				stop();

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

		//the anchor is the point the image is drawn around.
		//this can be useful if you want to rotate an image around a particular point.
        void				setAnchorPercent(float xPct, float yPct);	//set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
        void				setAnchorPoint(int x, int y);				//set the anchor point in pixels
        void				resetAnchor();								//resets the anchor to (0, 0)

		void 				setPaused(bool bPause);

		int					getCurrentFrame();
		int					getTotalNumFrames();

		void				firstFrame();
		void				nextFrame();
		void				previousFrame();

		float 				getHeight();
		float 				getWidth();

		bool				isPaused();
		bool				isLoaded();
		bool				isPlaying();

		//this is kept as legacy to support people accessing width and height directly. 
		int					height;
		int					width;

	protected:
		ofBaseVideoPlayer		* player;
		
		ofTexture tex;
		bool bUseTexture;

};
#endif






