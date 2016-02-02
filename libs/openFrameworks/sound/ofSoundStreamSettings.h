#pragma once

class ofBaseStreamSettings {
public:
	ofBaseStreamSettings()
		:sampleRate(44100)
		, bufferSize(1024)
		, numBuffers(4)
		, outDeviceID(-1)
		, inDeviceID(-1)
		, soundOutputPtr(nullptr)
		, soundInputPtr(nullptr)
		, tickCount(0)
		, numOutputChannels(0)
		, numInputChannels(0) {};

	virtual ~ofBaseStreamSettings() {}

	virtual void setSampleRate(const int sampleRate) = 0;

	virtual const int & getSampleRate() const = 0;

	virtual void setBufferSize(const int bufferSize) = 0;

	virtual const int & getBufferSize() const = 0;

	virtual void setNumBuffers(const int numBuffers) = 0;

	virtual const int & getNumBuffers() const = 0;

	virtual void setOutDeviceID(const int outDeviceID) = 0;

	virtual const int & getOutDeviceID() const = 0;

	virtual void setInDeviceID(const int inDeviceID) = 0;

	virtual const int & getInDeviceID() const = 0;

	virtual void setNumInputChannels(const int numInputChannels) = 0;

	virtual const int & getNumInputChannels() const = 0;

	virtual void setNumOutputChannels(const int numOutputChannels) = 0;

	virtual const int & getNumOutputChannels() const = 0;

	virtual void setSoundInputPtr(ofBaseSoundInput * soundInputPtr) = 0;

	virtual const ofBaseSoundInput & getSoundInputPtr() const = 0;

	virtual void setSoundOutputPtr(ofBaseSoundOutput * soundOutputPtr) = 0;

	virtual const ofBaseSoundOutput & getSoundOutputPtr() const = 0;

	//void setInputBuffer(const ofSoundBuffer inputBuffer) {
	//	this->inputBuffer = inputBuffer;
	//}

	//const ofSoundBuffer & getInputBuffer() const {
	//	return inputBuffer;
	//}

	//void setOutputBuffer(const ofSoundBuffer outputBuffer) {
	//	this->outputBuffer = outputBuffer;
	//}

	//const ofSoundBuffer & getOutputBuffer() const {
	//	return outputBuffer;
	//}

protected:
	int sampleRate;
	int outDeviceID;
	int inDeviceID;
	int bufferSize;
	int numBuffers;
	int numInputChannels;
	int numOutputChannels;
	ofBaseSoundInput * soundInputPtr;
	ofBaseSoundOutput * soundOutputPtr;
	//ofSoundBuffer inputBuffer;
	//ofSoundBuffer outputBuffer;
	long unsigned long tickCount;
};

#include "ofBaseTypes.h"

class ofSoundStreamSettings : public ofBaseStreamSettings {
public:
	ofSoundStreamSettings() {};

	ofSoundStreamSettings(const ofBaseStreamSettings & settings)
		:ofBaseStreamSettings(settings)
		//we can set base settings here too if there is anything specific
	{
		const ofSoundStreamSettings * streamSettings = dynamic_cast<const ofSoundStreamSettings*>(&settings);
		if (streamSettings) {
			//if there are any specific settings we set them here
		}
	}

	~ofSoundStreamSettings() {};

	void setSampleRate(const int sampleRate) {
		this->sampleRate = sampleRate;
	}

	const int & getSampleRate() const {
		return sampleRate;
	}

	void setBufferSize(const int bufferSize) {
		this->bufferSize = bufferSize;
	}

	const int & getBufferSize() const {
		return bufferSize;
	}

	void setNumBuffers(const int numBuffers) {
		this->numBuffers = numBuffers;
	}

	const int & getNumBuffers() const {
		return numBuffers;
	}

	void setOutDeviceID(const int outDeviceID) {
		this->outDeviceID = outDeviceID;
	}

	const int & getOutDeviceID() const {
		return outDeviceID;
	}

	void setInDeviceID(const int inDeviceID) {
		this->inDeviceID = inDeviceID;
	}

	const int & getInDeviceID() const {
		return inDeviceID;
	}

	void setNumInputChannels(const int numInputChannels) {
		this->numInputChannels = numInputChannels;
	}

	const int & getNumInputChannels() const {
		return numInputChannels;
	}

	void setNumOutputChannels(const int numOutputChannels) {
		this->numOutputChannels = numOutputChannels;
	}

	const int & getNumOutputChannels() const {
		return numOutputChannels;
	}

	void setSoundInputPtr(ofBaseSoundInput * soundInputPtr) {
		this->soundInputPtr = soundInputPtr;
	}

	const ofBaseSoundInput & getSoundInputPtr() const {
		return *soundInputPtr;
	}

	void setSoundOutputPtr(ofBaseSoundOutput * soundOutputPtr) {
		this->soundOutputPtr = soundOutputPtr;
	}

	const ofBaseSoundOutput & getSoundOutputPtr() const {
		return *soundOutputPtr;
	}

	//void setInputBuffer(const ofSoundBuffer inputBuffer) {
	//	this->inputBuffer = inputBuffer;
	//}

	//const ofSoundBuffer & getInputBuffer() const {
	//	return inputBuffer;
	//}

	//void setOutputBuffer(const ofSoundBuffer outputBuffer) {
	//	this->outputBuffer = outputBuffer;
	//}

	//const ofSoundBuffer & getOutputBuffer() const {
	//	return outputBuffer;
	//}

	/*void setTickCount(const long unsigned long tickCount) {
	this->tickCount = tickCount;
	}

	const long unsigned long & getTickCount() const {
	return tickCount;
	}*/


};

#include "RtAudio.h"

class ofRtSoundStreamSettings : public ofSoundStreamSettings {
public:

	enum RtApis {
		DEFAULT,    /*!< Search for a working compiled API. */
		ASLA,     /*!< The Advanced Linux Sound Architecture API. */
		PULSE,    /*!< The Linux PulseAudio API. */
		OSS,      /*!< The Linux Open Sound System API. */
		JACK,      /*!< The Jack Low-Latency Audio Server API. */
		OSX_CORE,    /*!< Macintosh OS-X Core Audio API. */
		MS_WASAPI, /*!< The Microsoft WASAPI API. */
		MS_ASIO,   /*!< The Steinberg Audio Stream I/O API. */
		MS_DS     /*!< The Microsoft Direct Sound API. */
	};

	ofRtSoundStreamSettings()
		:api(RtAudio::Api::UNSPECIFIED) {};

	ofRtSoundStreamSettings(const ofBaseStreamSettings & settings)
		:ofSoundStreamSettings(settings)
		, api(RtAudio::Api::UNSPECIFIED)
		//we can set base settings here too if there is anything specific
	{
		const ofRtSoundStreamSettings * streamSettings = dynamic_cast<const ofRtSoundStreamSettings*>(&settings);
		if (streamSettings) {
			//if there are any specific settings we set them here
			api = streamSettings->api;
		}
	}

	void setApi(const RtApis api) {
		switch (api) {
		case ASLA:
			this->api = RtAudio::Api::LINUX_ALSA;
			break;
		case PULSE:
			this->api = RtAudio::Api::LINUX_PULSE;
			break;
		case OSS:
			this->api = RtAudio::Api::LINUX_OSS;
			break;
		case JACK:
			this->api = RtAudio::Api::UNIX_JACK;
			break;
		case OSX_CORE:
			this->api = RtAudio::Api::MACOSX_CORE;
			break;
		case MS_WASAPI:
			this->api = RtAudio::Api::WINDOWS_WASAPI;
			break;
		case MS_ASIO:
			this->api = RtAudio::Api::WINDOWS_ASIO;
			break;
		case MS_DS:
			this->api = RtAudio::Api::WINDOWS_DS;
			break;
		default:
			this->api = RtAudio::Api::UNSPECIFIED;
			break;
		}
	}

	const RtAudio::Api & getApi() const {
		return api;
	}

protected:
	RtAudio::Api api;

};