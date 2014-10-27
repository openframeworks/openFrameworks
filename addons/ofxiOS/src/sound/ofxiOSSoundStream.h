//
//  SoundInputStream.h
//  Created by Lukasz Karluk on 13/06/13.
//  http://julapy.com/blog
//

#pragma once

#include "ofBaseSoundStream.h"

class ofxiOSSoundStream : public ofBaseSoundStream {

    public:
		ofxiOSSoundStream();
		~ofxiOSSoundStream();
		
		/// these are not implemented on iOS
		void listDevices();
		void setDeviceID(int deviceID);

		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);
		ofBaseSoundInput * getInput();
		ofBaseSoundOutput * getOutput();
		
		/// currently, the number of buffers is always 1 on iOS and setting nBuffers has no effect
		/// the max buffersize is 4096 
		bool setup(int numOfOutChannels, int numOfInChannels, int sampleRate, int bufferSize, int numOfBuffers);
		bool setup(ofBaseApp * app, int numOfOutChannels, int numOfInChannels, int sampleRate, int bufferSize, int numOfBuffers);
		
		void start();
		void stop();
		void close();
		
		// not implemented on iOS, always returns 0
		long unsigned long getTickCount();
	
		int getNumInputChannels();
		int getNumOutputChannels();
		int getSampleRate();
		int getBufferSize();
		int getDeviceID();
	
		static bool setMixWithOtherApps(bool bMix);
		
	private:
    
        ofBaseSoundInput * soundInputPtr;
        ofBaseSoundOutput * soundOutputPtr;
    
        void * soundInputStream;
        void * soundOutputStream;
    
        int numOfInChannels;
        int numOfOutChannels;
        int sampleRate;
        int bufferSize;
        int numOfBuffers;
};

#define ofxiPhoneSoundStream ofxiOSSoundStream