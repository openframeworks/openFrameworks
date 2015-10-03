#pragma once

#include "ofConstants.h"

//#include "ofBaseSoundStream.h"
#include "ofSoundStream.h"
#include "ofTypes.h"
#include "ofSoundBuffer.h"
#include "RtAudio.h"

typedef unsigned int RtAudioStreamStatus;

class ofRtAudioSoundStream : public ofBaseSoundStream {
public:
	class ofRtSoundStreamSettings : public ofBaseSoundStream::ofBaseStreamSettings {
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
			:sampleRate(44100)
			, bufferSize(1024)
			, numBuffers(4)
			, outDeviceID(-1)
			, inDeviceID(-1)
			, soundOutputPtr(nullptr)
			, soundInputPtr(nullptr)
			, tickCount(0)
			, numOutputChannels(0)
			, numInputChannels(0)
			, api(RtAudio::Api::UNSPECIFIED) {};

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
		RtAudio::Api api;

	};

	ofRtAudioSoundStream();
	~ofRtAudioSoundStream();

	std::vector<ofSoundDevice> getDeviceList() const;
	void setDeviceID(int deviceID);
	void setInDeviceID(int deviceID);
	void setOutDeviceID(int deviceID);

	void setInput(ofBaseSoundInput * soundInput);
	void setOutput(ofBaseSoundOutput * soundOutput);
	bool setup(const ofRtSoundStreamSettings & settings);
	bool setup(const ofBaseSoundStream::ofBaseStreamSettings & settings) {
		const ofRtSoundStreamSettings * soundSettings = dynamic_cast<const ofRtSoundStreamSettings*>(&settings);
		return setup(*soundSettings);
	}
	/*bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
	bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
	bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers, RtAudio::Api api);*/

	void start();
	void stop();
	void close();

	long unsigned long getTickCount() const;

	int getNumInputChannels() const;
	int getNumOutputChannels() const;
	int getSampleRate() const;
	int getBufferSize() const;
	int getDeviceID() const;

	ofRtSoundStreamSettings _settings;

private:
	long unsigned long tickCount;
	shared_ptr<RtAudio>	audio;

	int sampleRate;
	int outDeviceID;
	int inDeviceID;
	int bufferSize;
	int nInputChannels;
	int nOutputChannels;
	ofBaseSoundInput * soundInputPtr;
	ofBaseSoundOutput * soundOutputPtr;
	ofSoundBuffer inputBuffer;
	ofSoundBuffer outputBuffer;

	static int rtAudioCallback(void *outputBuffer, void *inputBuffer, unsigned int bufferSize, double streamTime, RtAudioStreamStatus status, void *data);

};


