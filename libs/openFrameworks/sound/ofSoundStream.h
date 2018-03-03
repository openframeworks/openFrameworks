#pragma once

#include "ofConstants.h"
#include "ofBaseApp.h"
#include "ofSoundBaseTypes.h"
#include <climits>
#include <functional>


class ofSoundStreamSettings;


///// \brief Sets up and starts a global ofSoundStream.
/////
///// This will set up a sound stream with a default sample rate of 44100, a
///// buffer size of 256 samples, and a queue of 4 buffers.
/////
///// \param nOutputChannels number of requested output channels (i.e. 2 for stereo).
///// \param nInputChannels number of requested input channels.
///// \param appPtr pointer to the app which will own the sound stream (optional).
OF_DEPRECATED_MSG("Use an ofSoundStreamSettings object instead of directly passing the parameters",
	void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * appPtr = nullptr));

///// \brief Sets up and starts a global ofSoundStream.
///// \param nOutputChannels number of requested output channels (i.e. 2 for stereo).
///// \param nInputChannels number of requested input channels.
///// \param sampleRate requested sample rate (44100 is typical).
///// \param bufferSize requested buffer size (256 is typical). Smaller values
/////        will be more responsive, but less stable.
///// \param nBuffers number of buffers to queue. Less buffers will be more responsive, but less stable.
OF_DEPRECATED_MSG("Use an ofSoundStreamSettings object instead of directly passing the parameters",
	void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, int sampleRate, int bufferSize, int nBuffers));

///// \brief Sets up and starts a global ofSoundStream.
///// \param nOutputChannels number of requested output channels (i.e. 2 for stereo).
///// \param nInputChannels number of requested input channels.
///// \param appPtr pointer to the app which will own the sound stream (optional).
///// \param sampleRate requested sample rate (44100 is typical).
///// \param bufferSize requested buffer size (256 is typical). Smaller values
/////        will be more responsive, but less stable.
///// \param nBuffers number of buffers to queue. Less buffers will be more
/////        responsive, but less stable.
OF_DEPRECATED_MSG("Use an ofSoundStreamSettings object instead of directly passing the parameters",
	void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * appPtr, int sampleRate, int bufferSize, int nBuffers));

void ofSoundStreamSetup(ofSoundStreamSettings & settings);

/// \brief Stops the sound stream (audioIn() / audioOut() will stop being called)
void ofSoundStreamStop();

/// \brief Starts the sound stream (audioIn() / audioOut() will start being called)
void ofSoundStreamStart();

/// \brief Stops the sound stream and also cleans up the stream's resources
void ofSoundStreamClose();

/// \brief Prints a list of all available audio devices
/// \return all sound devices found on the system
std::vector<ofSoundDevice> ofSoundStreamListDevices();

/// \class ofSoundStream
/// \brief Gives access to audio input and output devices
///
/// ofSoundStream controls access to your computer's audio input and output
/// devices. For example, you could use an ofSoundStream to get live input from
/// a microphone, or generate sound in realtime for your computer's speakers.
///
/// A typical openFrameworks app will use just one ofSoundStream, and you might
/// not even need to use this class directly at all if you don't need to query
/// things like the sample rate or buffer size. In order to start receiving or
/// generating audio, your ofApp should implement either ofBaseApp::audioIn() or
/// ofBaseApp::audioOut() respectively, and then call ofSoundStreamSetup(). You
/// can find examples of this in either the audioInputExample or the
/// audioOutputExample.
///
/// Starting a stream with 0 input or output channels will prevent audioIn() or
/// audioOut() from being called, respectively.
///
/// Some platforms (iOS, for example) will expose additional platform-specific
/// sound stream functionality. See the platform-specific examples for demos.
///
/// \warning Be aware that audioIn() and audioOut() will be called on a different
/// thread from your app's update() / draw() thread.
class ofSoundStream {
public:
	ofSoundStream();

	void setSoundStream(std::shared_ptr<ofBaseSoundStream> soundStreamPtr);
	std::shared_ptr<ofBaseSoundStream> getSoundStream();

	/// \brief Prints a list of available audio devices to the console
	void printDeviceList() const;

	/// \brief Retrieves a list of available audio devices
	std::vector<ofSoundDevice> getDeviceList(ofSoundDevice::Api api = ofSoundDevice::Api::DEFAULT) const;

	/// \brief Get all devices which match the arguments (name can be a partial match)
	std::vector<ofSoundDevice> getMatchingDevices(const std::string& name, unsigned int inChannels = UINT_MAX, unsigned int outChannels = UINT_MAX, ofSoundDevice::Api api = ofSoundDevice::Api::DEFAULT) const;

	/// \brief sets the device represented by the stream, see ofSoundStream::getDeviceList().
	OF_DEPRECATED_MSG("Use an ofSoundStreamSettings object instead of directly passing the parameters",
		void setDeviceID(int deviceID));

	/// \brief sets the device represented by the stream, see ofSoundStream::getDeviceList().
	OF_DEPRECATED_MSG("Use an ofSoundStreamSettings object instead of directly passing the parameters",
		void setDevice(const ofSoundDevice& device));

	bool setup(const ofSoundStreamSettings & settings);

	///// \brief Sets up and starts the stream.
	///// \param app pointer to the app which will own the sound stream.
	///// \param outChannels number of requested output channels (i.e. 2 for stereo).
	///// \param inChannels number of requested input channels.
	///// \param sampleRate requested sample rate (44100 is typical).
	///// \param bufferSize requested buffer size (256 is typical). Smaller
	/////        values will be more responsive, but less stable.
	///// \param nBuffers number of buffers to queue. Less buffers will be more
	/////        responsive, but less stable.
	///// \return true on success
	OF_DEPRECATED_MSG("Use an ofSoundStreamSettings object instead of directly passing the parameters",
		bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers));

	///// \brief Sets up and starts the stream.
	///// \param outChannels number of requested output channels (i.e. 2 for stereo).
	///// \param inChannels number of requested input channels.
	///// \param sampleRate requested sample rate (44100 is typical).
	///// \param bufferSize requested buffer size (256 is typical). Smaller values
	/////        will be more responsive, but less stable.
	///// \param nBuffers number of buffers to queue. Less buffers will be more
	/////        responsive, but less stable.
	///// \return true on success
	OF_DEPRECATED_MSG("Use an ofSoundStreamSettings object instead of directly passing the parameters",
		bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers));

	/// \brief Sets the object which will have audioIn() called when the device receives audio.
	void setInput(ofBaseSoundInput * soundInput);

	/// \brief Sets the object which will have audioIn() called when the device receives audio.
	void setInput(ofBaseSoundInput &soundInput);

	/// \brief Sets the object which will have audioOut() called when the device requests audio.
	void setOutput(ofBaseSoundOutput * soundOutput);

	/// \brief Sets the object which will have audioOut() called when the device requests audio.
	void setOutput(ofBaseSoundOutput &soundOutput);

	/// \brief Starts a stream (note that setup() will start the stream on its own).
	void start();

	/// \brief Stops the stream.
	void stop();

	/// \brief stops the stream and cleans up its resources.
	void close();

	/// \brief Queries the number of "ticks" passed since the stream started.
	///
	/// This is a representation of how many buffers have passed through the
	/// stream since it started. This can be converted to seconds with the
	/// following formula:
	///
	///    secondsOfPlayback = (tickCount * bufferSize) / sampleRate
	///
	/// \return number of buffers passed through the stream since it started.
	uint64_t getTickCount() const;

	/// \brief Queries the stream's number of input channels.
	/// \return the number of input channels (e.g. 2 for stereo).
	int getNumInputChannels() const;

	/// \brief Queries the stream's number of output channels.
	/// \return the number of output channels (e.g. 2 for stereo).
	int getNumOutputChannels() const;

	/// \brief Queries the stream's sample rate
	/// \return the current sample rate of the stream
	/// \note The returned sample rate may differ from the requested sample rate.
	int getSampleRate() const;

	/// \brief Queries the stream's buffer size.
	/// \return the current buffer size of the stream.
	int getBufferSize() const;

	/// \brief Retrieves a list of available audio devices and prints device descriptions to the console
	OF_DEPRECATED_MSG("Use printDeviceList instead", std::vector<ofSoundDevice> listDevices() const);

protected:
	std::shared_ptr<ofBaseSoundStream> soundStream;
	int tmpDeviceId = -1;

};

