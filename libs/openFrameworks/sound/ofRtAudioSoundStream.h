#pragma once

#include "ofConstants.h"

#include "ofBaseSoundStream.h"
#include "ofTypes.h"
#include "ofSoundBuffer.h"

class RtAudio;
typedef unsigned int RtAudioStreamStatus;

enum ofRtAPI {
	OF_RT_API_UNSPECIFIED,	
	OF_LINUX_ALSA, /// \The Advanced Linux Sound Architecture API.
	OF_LINUX_PULSE, /// \The Linux PulseAudio API.
	OF_LINUX_OSS, /// \The Linux Open Sound System API.
	OF_UNIX_JACK, /// \The Jack Low-Latency Audio Server API.
	OF_OSX_CORE, /// \Macintosh OS-X Core Audio API.
	OF_WINDOWS_WASAPI, /// \The Microsoft WASAPI API.
	OF_WINDOWS_ASIO, /// \The Steinberg Audio Stream I/O API.
	OF_WINDOWS_DS /// \The Microsoft Direct Sound API.
};

class ofRtAudioSoundStream : public ofBaseSoundStream{
	public:
		ofRtAudioSoundStream();
		~ofRtAudioSoundStream();
		
		std::vector<ofSoundDevice> getDeviceList() const;
		void setDeviceID(int deviceID);
		void setInDeviceID(int deviceID);
		void setOutDeviceID(int deviceID);

		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);
		bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers, ofRtAPI api);
		
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


