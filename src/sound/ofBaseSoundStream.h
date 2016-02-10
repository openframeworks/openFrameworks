#pragma once
#include "ofConstants.h"

class ofBaseSoundInput;
class ofBaseSoundOutput;
class ofSoundBuffer;
class ofSoundStreamSettings;

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
