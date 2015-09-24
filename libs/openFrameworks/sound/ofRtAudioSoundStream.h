#pragma once

#include "ofConstants.h"

#include "ofBaseSoundStream.h"
#include "ofSoundStream.h"
#include "ofTypes.h"
#include "ofSoundBuffer.h"
#include "RtAudio.h"

typedef unsigned int RtAudioStreamStatus;

class ofRtAudioSoundStream : public ofBaseSoundStream {
public:
	class ofRtSoundStreamSettings : public ofSoundStream::ofSoundStreamSettings {
	public:
		ofRtSoundStreamSettings()
			:api(RtAudio::Api::UNSPECIFIED) {};

		void setApi(const RtAudio::Api api) {
			this->api = api;
		}

		const RtAudio::Api & getApi() const {
			return api;
		}

	protected:
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

private:
	long unsigned long tickCount;
	shared_ptr<RtAudio>	audio;
	ofRtSoundStreamSettings settings;
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


