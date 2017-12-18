//
//  SoundInputStream.h
//  Created by Lukasz Karluk on 13/06/13.
//  http://julapy.com/blog
//

#pragma once

#include "ofSoundBaseTypes.h"
#include "ofSoundStream.h"

class ofxiOSSoundStream : public ofBaseSoundStream {

	public:
		ofxiOSSoundStream();
		~ofxiOSSoundStream();
		
		/// these are not implemented on iOS
		std::vector<ofSoundDevice> getDeviceList(ofSoundDevice::Api api) const;
		void setDeviceID(int deviceID);

		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);
		
		/// currently, the number of buffers is always 1 on iOS and setting nBuffers has no effect
		/// the max buffersize is 4096 
		bool setup(const ofSoundStreamSettings & settings);
	
		ofSoundDevice getInDevice() const{
			return ofSoundDevice();
		}
		
		ofSoundDevice getOutDevice() const{
			return ofSoundDevice();
		}
	
		void start();
		void stop();
		void close();
		
		// not implemented on iOS, always returns 0
		long unsigned long getTickCount() const;
	
		int getNumInputChannels() const;
		int getNumOutputChannels() const;
		int getSampleRate() const;
		int getBufferSize() const;
		int getDeviceID() const;
	
		static bool setMixWithOtherApps(bool bMix);
		
	private:

		void * soundInputStream;
		void * soundOutputStream;

		ofSoundStreamSettings settings;
};

#define ofxiPhoneSoundStream ofxiOSSoundStream
