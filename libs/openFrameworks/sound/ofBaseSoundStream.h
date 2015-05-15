#pragma once
#include "ofConstants.h"

class ofBaseSoundInput;
class ofBaseSoundOutput;
class ofSoundBuffer;

/// \class ofSoundDevice
/// \brief Represents information about a sound device on the system.
class ofSoundDevice {
public:

	ofSoundDevice();

	friend std::ostream& operator << (std::ostream& os, const ofSoundDevice& dev);
	friend std::ostream& operator << (std::ostream& os, const std::vector<ofSoundDevice>& devs);

	/// \brief Descriptive name for the device
	/// This is the same string that ofSoundStream::getMatchingDevices() will be looking for
	std::string name;

	/// \brief The device's unique ID (to be used in ofSoundStream::setDeviceID() )
	unsigned int deviceID;

	/// \brief Number of input channels the device supports
	unsigned int inputChannels;

	/// \brief Number of output channels the device supports
	unsigned int outputChannels;

	/// \brief If true, this device will be used by ofSoundStream unless changed with setDeviceID()
	bool isDefaultInput;

	/// \brief If true, this device will be used by ofSoundStream unless changed with setDeviceID()
	bool isDefaultOutput;

	/// \brief List of sample rates the device claims to support
	std::vector<unsigned int> sampleRates;
};

class ofBaseSoundStream{
	public:
		virtual ~ofBaseSoundStream(){}
	
		virtual void setDeviceID(int deviceID) = 0;
		virtual bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers)=0;
		virtual bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers)=0;
		virtual void setInput(ofBaseSoundInput * soundInput) = 0;
		virtual void setOutput(ofBaseSoundOutput * soundOutput) = 0;
	
		virtual std::vector<ofSoundDevice> getDeviceList() const = 0;
		virtual void printDeviceList() const;
	
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void close() = 0;

		virtual long unsigned long getTickCount() const = 0;
		virtual int getNumInputChannels() const = 0;
		virtual int getNumOutputChannels() const = 0;
		virtual int getSampleRate() const = 0;
		virtual int getBufferSize() const = 0;
		virtual int getDeviceID() const = 0;
};
