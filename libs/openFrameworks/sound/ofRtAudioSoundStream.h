#pragma once

#include "ofConstants.h"

#include "ofSoundBuffer.h"
#include "ofBaseSoundStream.h"
#include "ofTypes.h"

class RtAudio;
typedef unsigned int RtAudioStreamStatus;

class ofRtAudioSoundStream : public ofBaseSoundStream{
	public:
		ofRtAudioSoundStream();
		~ofRtAudioSoundStream();
		
		void listDevices();
		void setDeviceID(int deviceID);
        void setInDeviceID(int deviceID);
        void setOutDeviceID(int deviceID);

		bool setup(int outChannels, int inChannels, int sampleRate, int nFramesPerBuffer, int nBuffers);
		bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int nFramesPerBuffer, int nBuffers);
		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);
		
		void start();
		void stop();
		void close();
		
		long unsigned long getTickCount();		

		int getNumInputChannels();
		int getNumOutputChannels();
		int getSampleRate();
		int getBufferSize();
		int getNumBuffers() { return nBuffers; }

		// uh, which? in or out?
		int getDeviceID() { return outDeviceID; }
	
		
	private:
		long unsigned long	tickCount;
		ofPtr<RtAudio>		audio;
		int					sampleRate;
		int					outDeviceID, inDeviceID;
		int					nFramesPerBuffer;
		int					nInputChannels;
		int					nOutputChannels;
		int					nBuffers;
		ofBaseSoundInput *  soundInputPtr;
		ofBaseSoundOutput * soundOutputPtr;
	
		ofSoundBuffer		outputBuffer;
		ofSoundBuffer		inputBuffer;
	
		// flags to determine whether audio*BuffersChanged methods should be called on the soundInputPtr/soundOutputPtr
		bool				newBuffersNeededForInput;
		bool				newBuffersNeededForOutput;
		
		static int rtAudioCallback(void *outputBuffer, void *inputBuffer, unsigned int nFramesPerBuffer, double streamTime, RtAudioStreamStatus status, void *data);

};


