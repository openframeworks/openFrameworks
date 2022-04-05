#pragma once

#include "ofConstants.h"
#include "ofFileUtils.h"
#include <functional>

class ofSoundBuffer;



/// \brief A base class representing a sound input stream.
class ofBaseSoundInput{
	public:
		/// \brief Destroy the ofBaseSoundInput.
		virtual ~ofBaseSoundInput() {};

		/// \brief Receive an audio buffer.
		/// \param buffer An audio buffer.
		virtual void audioIn( ofSoundBuffer& buffer );

		/// \deprecated This legacy method is deprecated and will be removed.
		/// Use void audioIn(ofSoundBuffer& buffer) instead.
		virtual void audioIn( float * input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount );

		/// \deprecated This legacy method is deprecated and will be removed.
		/// Use void audioIn(ofSoundBuffer& buffer) instead.
		virtual void audioIn( float * input, int bufferSize, int nChannels );

		/// \deprecated This legacy method is deprecated and will be removed.
		/// Use void audioIn(ofSoundBuffer& buffer) instead.
		virtual void audioReceived( float * input, int bufferSize, int nChannels ){}
};


/// \brief A base class representing a sound output stream.
class ofBaseSoundOutput{
	public:
		/// \brief Destroy the ofBaseSoundOutput.
		virtual ~ofBaseSoundOutput() {};

		/// \brief Output an audio buffer.
		/// \param buffer An audio buffer.
		virtual void audioOut( ofSoundBuffer& buffer );

		/// \deprecated This legacy method is deprecated and will be removed.
		/// Use void audioOut(ofSoundBuffer& buffer) instead.
		virtual void audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount  );

		/// \deprecated This legacy method is deprecated and will be removed.
		/// Use void audioOut(ofSoundBuffer& buffer) instead.
		virtual void audioOut( float * output, int bufferSize, int nChannels );

		/// \deprecated This legacy method is deprecated and will be removed.
		/// Use void audioOut(ofSoundBuffer& buffer) instead.
		virtual void audioRequested( float * output, int bufferSize, int nChannels ){}
};

/// \class ofSoundDevice
/// \brief Represents information about a sound device on the system.
class ofSoundDevice {
public:
	enum Api {
		UNSPECIFIED,
		DEFAULT,
		ALSA,     /*!< The Advanced Linux Sound Architecture API. */
		PULSE,    /*!< The Linux PulseAudio API. */
		OSS,      /*!< The Linux Open Sound System API. */
		JACK,      /*!< The Jack Low-Latency Audio Server API. */
		OSX_CORE,    /*!< Macintosh OS-X Core Audio API. */
		MS_WASAPI, /*!< The Microsoft WASAPI API. */
		MS_ASIO,   /*!< The Steinberg Audio Stream I/O API. */
		MS_DS,     /*!< The Microsoft Direct Sound API. */
		NUM_APIS
	} api = UNSPECIFIED;

	friend std::ostream& operator << (std::ostream& os, const ofSoundDevice& dev);
	friend std::ostream& operator << (std::ostream& os, const std::vector<ofSoundDevice>& devs);

	/// \brief Descriptive name for the device
	/// This is the same string that ofSoundStream::getMatchingDevices() will be looking for
	std::string name{"Unknown"};

	/// \brief The device's unique ID (to be used in ofSoundStream::setDeviceID() )
	int deviceID = -1;

	/// \brief Number of input channels the device supports
	unsigned int inputChannels = 0;

	/// \brief Number of output channels the device supports
	unsigned int outputChannels = 0;

	/// \brief If true, this device will be used by ofSoundStream unless changed with setDeviceID()
	bool isDefaultInput = false;

	/// \brief If true, this device will be used by ofSoundStream unless changed with setDeviceID()
	bool isDefaultOutput = false;

	/// \brief List of sample rates the device claims to support
	std::vector<unsigned int> sampleRates;
};

class ofSoundStreamSettings {
public:
	virtual ~ofSoundStreamSettings() {}
	size_t sampleRate = 44100;
	size_t bufferSize = 256;
	size_t numBuffers = 4;
	size_t numInputChannels = 0;
	size_t numOutputChannels = 0;
	virtual bool setInDevice(const ofSoundDevice & device);
	virtual bool setOutDevice(const ofSoundDevice & device);
	virtual bool setApi(ofSoundDevice::Api api);
	virtual const ofSoundDevice * getInDevice() const;
	virtual const ofSoundDevice * getOutDevice() const;
	virtual ofSoundDevice::Api getApi() const;

	template<typename Listener>
	void setInListener(Listener * inListener){
		inCallback = std::bind(static_cast<void(Listener::*)(ofSoundBuffer &)>(&Listener::audioIn), inListener, std::placeholders::_1);
	}

	template<typename Listener>
	void setOutListener(Listener * outListener){
		outCallback = std::bind(static_cast<void(Listener::*)(ofSoundBuffer &)>(&Listener::audioOut), outListener, std::placeholders::_1);
	}

	std::function<void(ofSoundBuffer &)> inCallback;
	std::function<void(ofSoundBuffer &)> outCallback;
private:
	ofSoundDevice inDevice;
	ofSoundDevice outDevice;
	ofSoundDevice::Api api = ofSoundDevice::Api::UNSPECIFIED;
};

class ofBaseSoundStream {
public:
	virtual ~ofBaseSoundStream() {}
	virtual bool setup(const ofSoundStreamSettings & settings) = 0;
	virtual void setInput(ofBaseSoundInput * soundInput) = 0;
	virtual void setOutput(ofBaseSoundOutput * soundOutput) = 0;

	virtual std::vector<ofSoundDevice> getDeviceList(ofSoundDevice::Api api) const = 0;
	virtual void printDeviceList() const;

	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void close() = 0;

	virtual uint64_t getTickCount() const = 0;
	virtual int getNumInputChannels() const = 0;
	virtual int getNumOutputChannels() const = 0;
	virtual int getSampleRate() const = 0;
	virtual int getBufferSize() const = 0;
	virtual ofSoundDevice getInDevice() const = 0;
	virtual ofSoundDevice getOutDevice() const = 0;
};

std::string toString(ofSoundDevice::Api api);


//----------------------------------------------------------
// ofBaseSoundPlayer
//----------------------------------------------------------
class ofBaseSoundPlayer {

public:

	ofBaseSoundPlayer(){};
	virtual ~ofBaseSoundPlayer(){};

	virtual bool load(const std::filesystem::path& fileName, bool stream = false)=0;
	virtual void unload()=0;
	virtual void play() = 0;
	virtual void stop() = 0;

	virtual void setVolume(float vol) = 0;
	virtual void setPan(float vol) = 0; // -1 = left, 1 = right
	virtual void setSpeed(float spd) = 0;
	virtual void setPaused(bool bP) = 0;
	virtual void setLoop(bool bLp) = 0;
	virtual void setMultiPlay(bool bMp) = 0;
	virtual void setPosition(float pct) = 0; // 0 = start, 1 = end;
	virtual void setPositionMS(int ms) = 0;

	virtual float getPosition() const = 0;
	virtual int getPositionMS() const = 0;
	virtual bool isPlaying() const = 0;
	virtual float getSpeed() const = 0;
	virtual float getPan() const = 0;
	virtual bool isLoaded() const = 0;
	virtual float getVolume() const = 0;

};
