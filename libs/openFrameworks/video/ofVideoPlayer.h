#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofVideoBaseTypes.h"

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
class ofVideoPlayer : public ofBaseVideoDraws{

	public:

		ofVideoPlayer ();


		bool 				load(std::string name);
		void				loadAsync(std::string name);
		OF_DEPRECATED_MSG("Use load instead",bool loadMovie(std::string name));


		/// \brief Get the path to the loaded video file.
		///
		/// If no video file is loaded this returns an empty string.
		///
		/// \returns A path to the loaded video or an empty string if not loaded.
		std::string				getMoviePath() const;

		bool				setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat		getPixelFormat() const;

		/// \brief Closes the movie file and releases its resources.
		///
		/// This is an alias for close().
		///
		/// \sa close()
		void 				closeMovie();
		/// \brief Closes the movie file releases its resources.
		///
		/// This is an alias for closeMovie().
		///
		/// \sa closeMovie()
		void 				close();

		/// \brief Update the video player's internal state to continue playback.
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
		void 				setVolume(float volume);
		void 				setLoopState(ofLoopType state);
		ofLoopType			getLoopState() const;
		void   				setSpeed(float speed);
		void				setFrame(int frame);

		void 				setUseTexture(bool bUse);
		bool 				isUsingTexture() const;
		ofTexture &			getTexture();
		const ofTexture &	getTexture() const;
		OF_DEPRECATED_MSG("Use getTexture",ofTexture &			getTextureReference());
		OF_DEPRECATED_MSG("Use getTexture",const ofTexture &	getTextureReference() const);
		std::vector<ofTexture> & getTexturePlanes();
		const std::vector<ofTexture> & getTexturePlanes() const;
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

        void				setAnchorPercent(float xPct, float yPct);
        void				setAnchorPoint(float x, float y);
        void				resetAnchor();

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

		/// \brief Set the internal video player implementation.
		///
		/// Advanced users may find it useful to set a custom internal video
		/// player implementation. The custom video player must implment the
		/// ofBaseVideoPlayer interface.
		///
		/// \param newPlayer Shared pointer to the new video player that extends
		/// from ofBaseVideoPlayer.
		void				setPlayer(std::shared_ptr<ofBaseVideoPlayer> newPlayer);
		/// \brief Get a pointer to the internal video player implementation.
		///
		/// This returns a pointer to the ofBaseVideoPlayer interface. For
		/// implementation-specfic features, this can be cast to the subtype
		/// using dynamic_cast<MyVideoPlayerImplementation>(getPlayer()) or the
		/// templated getPlayer<MyVideoPlayerImplementation>() method.
		///
		/// \returns A pointer to the internal video player implementation.
		std::shared_ptr<ofBaseVideoPlayer>	getPlayer();
		/// \brief Get a const pointer to the internal video player implementation.
		///
		/// This returns a pointer to the ofBaseVideoPlayer interface. For
		/// implementation-specfic features, this can be cast to the subtype
		/// using dynamic_pointer_cast<MyVideoPlayerImplementation>(getPlayer())
		/// or the templated getPlayer<MyVideoPlayerImplementation>() method.
		///
		/// \returns A const pointer to the internal video player implementation.
		const std::shared_ptr<ofBaseVideoPlayer>	getPlayer() const;

		/// \brief Get a pointer to the internal video player implementation.
		///
		/// Calling getPlayer<MyVideoPlayerImplementation>() is equivalent to
		/// dynamic_pointer_cast<MyVideoPlayerImplementation>(getPlayer()).
		///
		/// \returns A pointer to the internal video player implementation or
		///			 nullptr if the cast fails.
		template<typename PlayerType>
		std::shared_ptr<PlayerType> getPlayer(){
			return std::dynamic_pointer_cast<PlayerType>(getPlayer());
		}

		/// \brief Get a const pointer to the internal video player implementation.
		///
		/// Calling getPlayer<MyVideoPlayerImplementation>() is equivalent to
		/// dynamic_pointer_cast<MyVideoPlayerImplementation>(getPlayer()).
		///
		/// \returns A const pointer to the internal video player implementation
		///			 or nullptr if the cast fails.
		template<typename PlayerType>
		const std::shared_ptr<PlayerType> getPlayer() const{
			return std::dynamic_pointer_cast<PlayerType>(getPlayer());
		}

	private:
		/// \brief Initialize the default player implementations.
		void initDefaultPlayer();
		/// \brief A pointer to the internal video player implementation.
		std::shared_ptr<ofBaseVideoPlayer>		player;
		/// \brief A collection of texture planes used by the video player.
		std::vector<ofTexture> tex;
		/// \brief A pointer to the internal player's texture if available.
		///
		/// Video players that implement ofBaseVideoPlayer::getTexturePtr()
		/// can provide a pointer to an internal texture. When possible,
		/// ofVideoPlayer will use the internal texture to avoid extra pixel
		/// copies.
		ofTexture * playerTex;
		/// \brief True if the video player is using a texture.
		bool bUseTexture;
		/// \brief The internal pixel format.
		mutable ofPixelFormat internalPixelFormat;
		/// \brief The stored path to the video's path.
		std::string moviePath;
};
