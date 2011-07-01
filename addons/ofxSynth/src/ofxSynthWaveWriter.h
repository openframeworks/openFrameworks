#pragma once

#include "ofxSoundEffect.h"

const int wav_header_size = 0x2C;

/** ofxSynthWaveWriter
 
 Class to write sample streams to the file system.
 Streams to disk, and can write as long as you have hard drive space
 
 */
 
class ofxSynthWaveWriter : public ofxSoundEffectPassthrough {
	public:
		bool startWriting(string filename = "out.wav");
		void stopWriting();

		string getName() { return "ofxSynthWaveWriter"; }
		virtual void process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels );
		
	private:
		void flush();

		bool writing;
		int offset, numChannels, numFrames;
		
		enum { buf_size = 32768 * 2 };
		unsigned char* buf;
		FILE*   file;
		long    sample_count_;
		long    rate;
		long    buf_pos;
		int     chan_count;

};
