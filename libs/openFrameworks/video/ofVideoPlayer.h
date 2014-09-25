#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofBaseTypes.h"
#include "ofTypes.h"

#ifdef OF_VIDEO_PLAYER_GSTREAMER
	#include "ofGstVideoPlayer.h"
	#define OF_VID_PLAYER_TYPE ofGstVideoPlayer
#endif

#ifdef OF_VIDEO_PLAYER_QUICKTIME
	#include "ofQuickTimePlayer.h"
	#define OF_VID_PLAYER_TYPE ofQuickTimePlayer
#endif

#ifdef OF_VIDEO_PLAYER_QTKIT
	#include "ofQTKitPlayer.h"
	#define OF_VID_PLAYER_TYPE ofQTKitPlayer
#endif

#ifdef OF_VIDEO_PLAYER_AVFOUNDATION
    #include "ofAVFoundationPlayer.h"
    #define OF_VID_PLAYER_TYPE ofAVFoundationPlayer
#endif

#ifdef OF_VIDEO_PLAYER_IOS
	#include "ofxiOSVideoPlayer.h"
	#define OF_VID_PLAYER_TYPE ofxiOSVideoPlayer
#endif

#ifdef OF_VIDEO_PLAYER_ANDROID
	#include "ofxAndroidVideoPlayer.h"
	#define OF_VID_PLAYER_TYPE ofxAndroidVideoPlayer
#endif

#ifdef OF_VIDEO_PLAYER_EMSCRIPTEN
	#include "ofxEmscriptenVideoPlayer.h"
	#define OF_VID_PLAYER_TYPE ofxEmscriptenVideoPlayer
#endif

//---------------------------------------------
class ofVideoPlayer : public ofBaseVideoPlayer,public ofBaseVideoDraws{

	public:

		ofVideoPlayer ();

		bool 				loadMovie(string name);
	    string				getMoviePath() const;

		bool				setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat		getPixelFormat() const;
		
		void 				closeMovie();
		void 				close();		

		void				update();
		void 				play();
		void 				stop();

		bool 				isFrameNew() const;
		unsigned char * 	getPixels();
        ofPixels&			getPixelsRef();
        const ofPixels&     getPixelsRef() const;
		float 				getPosition() const;
		float 				getSpeed() const;
		float 				getDuration() const;
		bool				getIsMovieDone() const;

		void 				setPosition(float pct);
		void 				setVolume(float volume); // 0..1
		void 				setLoopState(ofLoopType state);
		ofLoopType			getLoopState() const;
		void   				setSpeed(float speed);
		void				setFrame(int frame);  // frame 0 = first frame...

		void 				setUseTexture(bool bUse);
		bool 				isUsingTexture() const;
		ofTexture &			getTextureReference();
		vector<ofTexture> & getTexturePlanes();
		const ofTexture &	getTextureReference() const;
		const vector<ofTexture> & getTexturePlanes() const;
		void 				draw(float x, float y, float w, float h) const;
		void 				draw(float x, float y) const;
		using ofBaseDraws::draw;

		void 				bind() const;
		void 				unbind() const;

		//the anchor is the point the image is drawn around.
		//this can be useful if you want to rotate an image around a particular point.
        void				setAnchorPercent(float xPct, float yPct);	//set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
        void				setAnchorPoint(float x, float y);				//set the anchor point in pixels
        void				resetAnchor();								//resets the anchor to (0, 0)

		void 				setPaused(bool bPause);

		int					getCurrentFrame() const;
		int					getTotalNumFrames() const;

		void				firstFrame();
		void				nextFrame();
		void				previousFrame();

		float 				getHeight() const;
		float 				getWidth() const;

		bool				isPaused() const;
		bool				isLoaded() const;
		bool				isPlaying() const;
		bool 				isInitialized() const;

		void				setPlayer(shared_ptr<ofBaseVideoPlayer> newPlayer);
		shared_ptr<ofBaseVideoPlayer>	getPlayer();
		const shared_ptr<ofBaseVideoPlayer>	getPlayer() const;

		template<typename PlayerType>
		shared_ptr<PlayerType> getPlayer(){
			return dynamic_pointer_cast<PlayerType>(getPlayer());
		}

		template<typename PlayerType>
		const shared_ptr<PlayerType> getPlayer() const{
			return dynamic_pointer_cast<PlayerType>(getPlayer());
		}

		//this is kept as legacy to support people accessing width and height directly. 
		mutable int         height;
		mutable int         width;

	private:
		shared_ptr<ofBaseVideoPlayer>		player;
		
		vector<ofTexture> tex;
		ofTexture * playerTex; // a seperate texture that may be optionally implemented by the player to avoid excessive pixel copying.
		bool bUseTexture;
		mutable ofPixelFormat internalPixelFormat;
	    string moviePath;
};





