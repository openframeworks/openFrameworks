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

#ifdef OF_VIDEO_PLAYER_DIRECTSHOW
    #include "ofDirectShowPlayer.h"
    #define OF_VID_PLAYER_TYPE ofDirectShowPlayer
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


		bool 				load(string name);
		void				loadAsync(string name);
		OF_DEPRECATED_MSG("Use load instead",bool loadMovie(string name));


		/// \brief Get the path to the loaded video file.
		///
		/// If no video file is loaded this returns an empty string.
		/// \returns A path to the loaded video or an empty string if the video
		/// is not loaded.
		string				getMoviePath() const;

		bool				setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat		getPixelFormat() const;
		
		/// \brief Closes the movie file and de-allocates its resources.
		///
		/// This is an alias of close()
		///
		/// \sa close()
		void 				closeMovie();
		/// \brief Closes the movie file and de-allocates its resources.
		///
		/// This is an alias of closeMovie()
		///
		/// \sa closeMovie()
		void 				close();

		/// \brief Update the video player's internal state to continue
		/// playback.
		///
		/// If normal video playback is desired, this method is usually called
		/// once per animation frame inside of ofApp::update().
		void				update();
		void 				play();
		void 				stop();

		bool 				isFrameNew() const;
		ofPixels& 			getPixels();
		const ofPixels&		getPixels() const;
        OF_DEPRECATED_MSG("Use getPixels() instead", ofPixels&	getPixelsRef());
        OF_DEPRECATED_MSG("Use getPixels() instead", const ofPixels&  getPixelsRef() const);
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
		ofTexture &			getTexture();
		const ofTexture &	getTexture() const;
		OF_DEPRECATED_MSG("Use getTexture",ofTexture &			getTextureReference());
		OF_DEPRECATED_MSG("Use getTexture",const ofTexture &	getTextureReference() const);
		vector<ofTexture> & getTexturePlanes();
		const vector<ofTexture> & getTexturePlanes() const;
		void 				draw(float x, float y, float w, float h) const;
		void 				draw(float x, float y) const;
		using ofBaseDraws::draw;
		/// \brief Binds the video texture to the current rendering context.
		///
		/// For advanced users who need to manually manage texture drawing
		/// without calling draw(). Only binds the texture if one exists.
		///
		/// \sa ofTexture::bind()
		/// \sa http://www.opengl.org/sdk/docs/man4/html/glBindTexture.xhtml
		void 				bind() const;
		/// \brief Unbinds the video texture from the current rendering context.
		///
		/// For advanced users who need to manually manage texture drawing
		/// without calling draw(). Only binds the texture if one exists.
		///
		/// \sa ofTexture::unbind()
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

		/// \brief Explicitly set the video player to use for playback.
		///
		/// newPlayer must extend from ofBaseVideoPlayer.
		///
		/// \param newPlayer Shared pointer to the new video player that extends
		/// from ofBaseVideoPlayer.
		void				setPlayer(shared_ptr<ofBaseVideoPlayer> newPlayer);
		/// \brief Get a shared pointer to the video player currently being used
		/// for playback.
		/// \returns A shared pointer to the video player currently being used
		/// for playback.
		shared_ptr<ofBaseVideoPlayer>	getPlayer();
		/// \brief Get a const shared pointer to the video player currently
		/// being used.
		/// \returns A const shared pointer to the video player currently
		/// being used for playback.
		const shared_ptr<ofBaseVideoPlayer>	getPlayer() const;

		/// \brief Get a shared pointer to the video player currently being used
		/// for playback.
		///
		/// This is a templated version of ofVideoPlayer::getPlayer()
		/// that dynamically casts the shared player pointer to the PlayerType
		/// template type.
		///
		/// \returns A shared pointer to the video player currently being used
		/// for playback that is dynamically casted to the PlayerType template
		/// type.
		template<typename PlayerType>
		shared_ptr<PlayerType> getPlayer(){
			return dynamic_pointer_cast<PlayerType>(getPlayer());
		}

		/// \brief Get a const shared pointer to the video player currently
		/// being used for playback.
		///
		/// This is a templated version of ofVideoPlayer::getPlayer()
		/// that dynamically casts the shared player pointer to the PlayerType
		/// template type.
		///
		/// \returns A const shared pointer to the video player currently being
		/// used for playback that is dynamically casted to the PlayerType
		/// template type.
		template<typename PlayerType>
		const shared_ptr<PlayerType> getPlayer() const{
			return dynamic_pointer_cast<PlayerType>(getPlayer());
		}

	private:
		void initDefaultPlayer();
		shared_ptr<ofBaseVideoPlayer>		player;
		
		vector<ofTexture> tex;
		ofTexture * playerTex; // a seperate texture that may be optionally implemented by the player to avoid excessive pixel copying.
		bool bUseTexture;
		mutable ofPixelFormat internalPixelFormat;
	    string moviePath;
};





