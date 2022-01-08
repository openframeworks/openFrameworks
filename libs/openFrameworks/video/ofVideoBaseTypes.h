#pragma once
#include "ofGraphicsBaseTypes.h"
#include "ofGLBaseTypes.h"

enum ofPixelFormat: short;

/// \brief Used to represent the available video looping modes.
///
/// \sa ofVideoPlayer::setLoopState()
/// \sa ofVideoPlayer::getLoopState()
enum ofLoopType : short{
	/// \brief Plays the video once without looping.
	OF_LOOP_NONE=0x01,
	/// \brief Plays the video forwards then backwards repeatedly.
	OF_LOOP_PALINDROME=0x02,
	/// \brief Repeats the video over and over.
	OF_LOOP_NORMAL=0x03
};



/// \brief A structure describing attributes of a video format.
///
/// An ofVideoFormat is used to describe the size, pixel format and frame rates
/// offered by a video device.
///
/// \sa ofVideoDevice
class ofVideoFormat{
public:
	/// \brief The pixel format of the video format.
	ofPixelFormat pixelFormat;

	/// \brief The width of the video format in pixels.
	int width;

	/// \brief The height of the video format in pixels.
	int height;

	/// \brief A list of framerates for this video format in frames per second.
	std::vector<float> framerates;
};

/// \brief A structure describing attributes of a video device.
///
/// An ofVideoDevice can represent a camera, grabber or other frame source.
class ofVideoDevice{
public:
	/// \brief The video device ID.
	int id;

	/// \brief The video device name.
	std::string deviceName;

	/// \brief The video device hardware name.
	std::string hardwareName;

	/// \brief Unique identifier for the device if it has one.
	std::string serialID;

	/// \brief A list of video device formats provided by the device.
	/// \sa ofVideoFormat
	std::vector<ofVideoFormat> formats;

	/// \brief Is true if this video device is available.
	bool bAvailable;
};

/// \brief A base class representing a video source.
class ofBaseVideo: virtual public ofBaseHasPixels, public ofBaseUpdates{
public:
	/// \brief Destroy the ofBaseVideo.
	virtual ~ofBaseVideo(){}

	/// \returns true if the pixel data was updated since the last call to update().
	virtual bool isFrameNew() const =0;

	/// \brief Close the video source.
	virtual void close()=0;

	/// \brief Determine if the video source is initialized.
	///
	/// Video sources such as cameras are often initialized with a
	/// setup() method.  Video sources such as movie players are often
	/// initialized with a load() method.
	///
	/// \returns true if the video source is initialized.
	virtual bool isInitialized() const=0;

	/// \brief Set the requested ofPixelFormat.
	/// \param pixelFormat the requested ofPixelFormat.
	/// \returns true if the format was successfully changed.
	virtual bool setPixelFormat(ofPixelFormat pixelFormat) = 0;

	/// \returns the current ofPixelFormat.
	virtual ofPixelFormat getPixelFormat() const = 0;
};


/// \brief A base class representing a drawable video source.
class ofBaseVideoDraws:
	public ofBaseVideo,
	public ofBaseDraws,
	public ofBaseHasTexturePlanes{
public:
	/// \brief Destroy the ofBaseVideoDraws.
	virtual ~ofBaseVideoDraws(){}
};

/// \brief A base class representing a video device such as a camera.
class ofBaseVideoGrabber: virtual public ofBaseVideo{

	public :
	/// \brief Destroy the ofBaseVideoGrabber
	virtual ~ofBaseVideoGrabber();

	//needs implementing
	/// \brief Get a list of available video grabber devices.
	/// \returns a std::vector of ofVideoDevice objects.
	virtual std::vector<ofVideoDevice>	listDevices() const = 0;

	/// \brief Set up the grabber with the requested width and height.
	///
	/// Some video grabbers may take the requested width and height as
	/// a hint and choose the closest dimensions to those requested.
	/// Users can check the actual width and height by calling getWidth() and
	/// getHeight() respectively after a successful setup.
	///
	/// \param w the requested width.
	/// \param h the requested height.
	/// \returns true if the video grabber was set up successfully.
	virtual bool setup(int w, int h) = 0;

	/// \brief Get the video grabber's height.
	/// \returns the video grabber's height.
	virtual float getHeight() const = 0;

	/// \brief Get the video grabber's width.
	/// \returns the video grabber's width.
	virtual float getWidth() const = 0;

	/// \brief Get the video grabber's internal ofTexture pointer if available.
	///
	/// \note Subclasses should implement this method only if internal API can
	/// upload video grabber pixels directly to an ofTexture.
	///
	/// \returns the internal ofTexture pointer or nullptr if not available.
	virtual ofTexture * getTexturePtr(){ return nullptr; }

	/// \brief Set the video grabber's hardware verbosity level.
	/// \param bTalkToMe true if verbose grabber logging feedback is required.
	virtual void setVerbose(bool bTalkToMe);

	/// \brief Set the video grabber's device ID.
	///
	/// In most cases, a user can choose a specific grabber source by ID.  This
	/// device ID information should be available to the user via the
	/// listDevices() method.
	///
	/// \param deviceID The device ID provided by listDevices().
	virtual void setDeviceID(int deviceID);

	/// \brief Set the video grabber's desired frame rate.
	///
	/// Many video grabbers support user-specified frame rates.  This frame rate
	/// should be considered a hint for the video grabber and is not guaranteed.
	///
	/// \param framerate the desired frame rate.
	virtual void setDesiredFrameRate(int framerate);

	/// \brief Request a native GUI for video grabber settings.
	/// \note This feature may not be implemented by all video grabbers.
	virtual void videoSettings();

};


/// \brief A base class representing a video player.
class ofBaseVideoPlayer: virtual public ofBaseVideo{

public:
	/// \brief Destroys the ofBaseVideoPlayer.
	virtual ~ofBaseVideoPlayer();

	/// \brief Load a video resource by name.
	///
	/// The list of supported video types and sources (e.g. rtsp:// sources) is
	/// implementation dependent.
	///
	/// \param name The name of the video resource to load.
	/// \returns True if the video was loaded successfully.
	/// \sa loadAsync()
	virtual bool				load(std::string name) = 0;
	/// \brief Asynchronously load a video resource by name.
	///
	/// The list of supported video types and sources (e.g. rtsp:// sources) is
	/// implementation dependent.
	///
	/// When this method is used to load a video resouce, users can determine
	/// when the video is loaded by calling isLoaded().
	///
	/// \param name The name of the video resource to load.
	/// \sa isLoaded()
	virtual void				loadAsync(std::string name);

	/// \brief Play the video from the current playhead position.
	/// \sa getPosition()
	/// \sa setPostion()
	virtual void				play() = 0;
	/// \brief Pause and reset the playhead position to the first frame.
	virtual void				stop() = 0;
	/// \brief Get a pointer to the video texture used internally if it exists.
	///
	/// If the video player implementation supports direct-to-texture rendering,
	/// this method will return a pointer to the internal texture. If
	/// direct-to-texture rendering is not supported, nullptr is returned.
	///
	/// \returns A valid pointer to the internal texture, otherwise a nullptr.
	virtual ofTexture *			getTexturePtr(){return nullptr;};

	/// \brief Get the width in pixels of the loaded video.
	/// \returns The width in pixels of the loaded video or 0 if none is loaded.
	virtual float 				getWidth() const = 0;
	/// \brief Get the height in pixels of the loaded video.
	/// \returns The height in pixels of the loaded video or 0 if none is loaded.
	virtual float 				getHeight() const = 0;

	/// \brief Returns true if the video is paused.
	/// \returns True if the video is paused.
	virtual bool				isPaused() const = 0;
	/// \brief Returns true if a video is loaded.
	///
	/// This is helpful when loading a video with loadAsync(). This is also an
	/// alias of isInitialized().
	///
	/// \sa loadAsync()
	/// \returns True if a video is loaded.
	virtual bool				isLoaded() const = 0;
	/// \brief Returns true if the loaded video is playing.
	/// \returns True if the loaded video is playing.
	virtual bool				isPlaying() const = 0;
	/// \brief Returns true if a video is loaded.
	///
	/// This is helpful when loading a video with loadAsync(). This is also
	/// an alias of isLoaded().
	///
	/// \sa loadAsync()
	/// \returns True if a video is loaded.
	virtual bool				isInitialized() const{ return isLoaded(); }

	/// \brief Get the current playhead position of the loaded video.
	///
	/// This value is a normalized floating point value between 0.0 and 1.0 that
	/// represents the position of the playhead. 0.0 maps to the first frame of
	/// the loaded video and 1.0 maps to the last frame of the loaded video.
	///
	/// \returns A value between 0.0 and 1.0 representing playhead position.
	virtual float 				getPosition() const;
	/// \brief Get the playback speed of the video player.
	///
	/// When the loop state is OF_LOOP_NONE or OF_LOOP_NORMAL, positive speed
	/// will scale a forward playback rate while a negative speed will scale a
	/// a backward playback rate. When the loop state is OF_LOOP_PALINDROME,
	/// the direction of playback will change each loop, but the playback rate
	/// will still be scaled by the absolute value of the speed.
	///
	/// \returns The playback speed of the video player.
	virtual float 				getSpeed() const;
	/// \brief Get the duration of the loaded video in seconds.
	/// \returns The duration of the loaded video in seconds.
	virtual float 				getDuration() const;
	/// \brief Returns true if the loaded video has finished playing.
	/// \returns True if the loaded video has finished playing.
	virtual bool				getIsMovieDone() const;

	/// \brief Set the paused state of the video.
	/// \param bPause True to pause the video, false to play.
	virtual void 				setPaused(bool bPause);
	/// \brief Set the position of the playhead.
	///
	/// This value is a normalized floating point value between 0.0 and 1.0 that
	/// represents the position of the playhead. 0.0 maps to the first frame of
	/// the loaded video and 1.0 maps to the last frame of the loaded video.
	///
	/// \param pct A value between 0.0 and 1.0 representing playhead position.
	virtual void 				setPosition(float pct);
	/// \brief Set the volume of the video player.
	///
	/// This value is a normalized floating point value between 0.0 and 1.0 that
	/// represents the video player volume. 0.0 maps to silence and 1.0 maps to
	/// maximum volume.
	///
	/// \param volume A value between 0.0 and 1.0 representing volume.
	virtual void 				setVolume(float volume);
	/// \brief Set the video loop state.
	/// \param state The loop state of the video.
	/// \sa ::ofLoopType
	virtual void 				setLoopState(ofLoopType state);
	/// \brief Set the video playback speed.
	///
	/// When the loop state is OF_LOOP_NONE or OF_LOOP_NORMAL, positive speed
	/// will scale a forward playback rate while a negative speed will scale a
	/// a backward playback rate. When the loop state is OF_LOOP_PALINDROME,
	/// the direction of playback will change each loop, but the playback rate
	/// will still be scaled by the absolute value of the speed.
	///
	/// To play a video forward at normal speed, set the loop state to
	/// OF_LOOP_NONE or OF_LOOP_NORMAL and a speed of 1.0. To double the
	/// playback rate, set the speed to 2.0. To play a video backward, set the
	/// speed to a negative number. A speed 0.25 will play the video at 1/4 the
	/// the normal rate and a rate of 0.0 will effectively stop playback.
	///
	/// \param speed The desired playback speed of the video.
	virtual void   				setSpeed(float speed);
	/// \brief Set the current frame by frame number.
	///
	/// Similar to setPosition(), but accepts a frame number instead of
	/// a normalized floating point value. Frame count begins with the first
	/// frame as 0 and the last frame as getTotalNumFrames() - 1.
	///
	/// \param frame The frame number to set the new playhead to.
	virtual void				setFrame(int frame);

	/// \brief Get the current playhead position as a frame number.
	/// \returns The current playhead position as a frame number.
	virtual int					getCurrentFrame() const;
	/// \brief Get the total number of frames in the currently loaded video.
	/// \returns The total number of frames in the currently loaded video.
	virtual int					getTotalNumFrames() const;
	/// \brief Get the current loop state of the video.
	/// \sa ::ofLoopType
	virtual ofLoopType			getLoopState() const;

	/// \brief Set the playhead position to the first frame.
	///
	/// This is functionally equivalent to setFrame(0) or setPosition(0.0).
	virtual void				firstFrame();
	/// \brief Advance the playhead forward one frame.
	///
	/// This allows the user to advance through the video manually one frame at
	/// a time without calling play().
	virtual void				nextFrame();
	/// \brief Advance the playhead backward one frame.
	///
	/// This allows the user to advance backward through the video manually one
	/// frame at a time without calling play().
	virtual void				previousFrame();
};
