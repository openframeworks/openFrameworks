/*  Uses portions of
 * 
 *  maximilian
 *  platform independent synthesis library using portaudio or rtaudio
 *
 *  Created by Mick Grierson on 29/12/2009.
 *  Copyright 2009 Mick Grierson & Strangeloop Limited. All rights reserved.
 *	Thanks to the Goldsmiths Creative Computing Team.
 *	Special thanks to Arturo Castro for the PortAudio implementation.
 * 
 *	Permission is hereby granted, free of charge, to any person
 *	obtaining a copy of this software and associated documentation
 *	files (the "Software"), to deal in the Software without
 *	restriction, including without limitation the rights to use,
 *	copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the
 *	Software is furnished to do so, subject to the following
 *	conditions:
 *	
 *	The above copyright notice and this permission notice shall be
 *	included in all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,	
 *	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *	OTHER DEALINGS IN THE SOFTWARE.
 *
 */
 
#pragma once

#include "ofMain.h"

#include "ofxSynthDelayLine.h"
#include "ofxSynthFilter.h"

#define OFXSYNTHONESHOT 1
#define OFXSYNTHADR 2
#define TWOPI 3.14159*2


const int wav_header_size = 0x2C;

class maxiOsc {
	
	double frequency;
	double phase;
	double startphase;
	double endphase;
	double output;
	double tri;

	
public:
	maxiOsc();
	double sinewave(double frequency);
	double coswave(double frequency);
	double phasor(double frequency);
	double phasor(double frequency, double startphase, double endphase);
	double saw(double frequency);
	double triangle(double frequency,double phase);
	double square(double frequency);
	double pulse(double frequency, double duty);
	double noise();
	double sinebuf(double frequency);
	double sinebuf4(double frequency);
	
};

typedef maxiOsc ofxMaxiOsc;

/** ofxSynthADSR
 
 produces a 3 point envelope with sustain
 
*/
class ofxSynthADSR : public ofSoundSource {
	public:
		void trigger();
		void release();
		void setADSR(float _a, float _d, float _s, float _r){
			// TODO: make sure that we don't overrun if we set this too short all of a sudden
			// might be as simple as making the output level a smoothed float
			a=_a; d=_d; s=_s; r=_r;
		}
		string getName() { return "ofxSynthADSR"; }
		void audioRequested( float* buffer, int numFrames, int numChannels );
	protected:
		float a, d, s, r;
		int offset;
		bool noteOn;
};

/** ofxSynth
 
 A classic subtractive synth
 
*/
class ofxSynth : public ofSoundSource{
	public:
		ofxSynth();
		string getName() { return "ofxSynth"; }
		void setup();
		
		void trigger();

		void setFrequency(float freq);
		void setFrequencyMidiNote(float note);
		void setPortamento(float p){portamento = p;};
		
		void setFilter(float _cutoff, float _res);
		float getCutoff(){return cutoff;};
		float getRes(){return res;};
		void setFilterLowPass(){filter.setLowPass();filterMode = 1;};
		void setFilterHighPass(){filter.setHighPass();filterMode = 2;};
				
		void setWaveSquare(){waveMode = 0;};
		void setWaveTri(){waveMode = 1;};
		void setWaveSaw(){waveMode = 2;};
		void setWaveSin(){};
		
		virtual void audioRequested( float* buffer, int numFrames, int numChannels );
		void				setSampleRate(int rate);

		ofxMaxiOsc wave;
		ofxSynthADSR env, modEnv;
		ofxSynthFilter filter;
		int ampMode;
		bool usesEnv, hasTrigger;
		
	protected:
		float currentFrequency,startFrequency, targetFrequency, currentAmp, noteTime;
		float sustain, gain, cutoff, res, filterMod, portamento;
		int filterMode, waveMode, sampleRate;
		double ampEnv[8];
};

class ofxSynthSample  {
	
private:
	string 	myPath;
	int 	myChunkSize;
	int	mySubChunk1Size;
	short 	myFormat;
	int   	myByteRate;
	short 	myBlockAlign;
	short 	myBitsPerSample;
	int	myDataSize;
	double speed;
	double output;
	
public:
	double position;
	short 	myChannels;
	int   	mySampleRate;
	long length;
	void getLength();

	char* 	myData;
	
	// get/set for the Path property

	~ofxSynthSample()
	{
		delete myData;
		myChunkSize = NULL;
		mySubChunk1Size = NULL;
		myFormat = NULL;
		myChannels = NULL;
		mySampleRate = NULL;
		myByteRate = NULL;
		myBlockAlign = NULL;
		myBitsPerSample = NULL;
		myDataSize = NULL;
	}
	
	bool load(string fileName);

	// read a wav file into this class
	bool read()
	{
		bool result;
		ifstream inFile( myPath.c_str(), ios::in | ios::binary);
		result = inFile;
		
		if (inFile) {
			//printf("Reading wav file...\n"); // for debugging only
			
			inFile.seekg(4, ios::beg);
			inFile.read( (char*) &myChunkSize, 4 ); // read the ChunkSize
			
			inFile.seekg(16, ios::beg);
			inFile.read( (char*) &mySubChunk1Size, 4 ); // read the SubChunk1Size
			
			//inFile.seekg(20, ios::beg);
			inFile.read( (char*) &myFormat, sizeof(short) ); // read the file format.  This should be 1 for PCM
			
			//inFile.seekg(22, ios::beg);
			inFile.read( (char*) &myChannels, sizeof(short) ); // read the # of channels (1 or 2)
			
			//inFile.seekg(24, ios::beg);
			inFile.read( (char*) &mySampleRate, sizeof(int) ); // read the samplerate
			
			//inFile.seekg(28, ios::beg);
			inFile.read( (char*) &myByteRate, sizeof(int) ); // read the byterate
			
			//inFile.seekg(32, ios::beg);
			inFile.read( (char*) &myBlockAlign, sizeof(short) ); // read the blockalign
			
			//inFile.seekg(34, ios::beg);
			inFile.read( (char*) &myBitsPerSample, sizeof(short) ); // read the bitspersample
			
			//ignore any extra chunks
			char chunkID[4]="";
			int filePos = 36;
			bool found = false;
			while(!found && !inFile.eof()) {
				inFile.seekg(filePos, ios::beg);
				inFile.read((char*) &chunkID, sizeof(char) * 4);
				inFile.seekg(filePos + 4, ios::beg);
				inFile.read( (char*) &myDataSize, sizeof(int) ); // read the size of the data
				filePos += 8;
				if (strcmp(chunkID,"data") == 0) {
					found = true;
				}else{
					filePos += myDataSize;
				}
			}
			
			
			
			// read the data chunk
			myData = (char*) malloc(myDataSize * sizeof(char));
			inFile.seekg(filePos, ios::beg);
			inFile.read(myData, myDataSize);
			length=myDataSize*(0.5/myChannels);
			
			inFile.close(); // close the input file
		}
		
		
		return result; // this should probably be something more descriptive
	}
	
	double play4(double frequency, double start, double end, int sampleRate);
	
	// return a printable summary of the wav file
	char *getSummary()
	{
		char *summary = new char[250];
		sprintf(summary, " Format: %d\n Channels: %d\n SampleRate: %d\n ByteRate: %d\n BlockAlign: %d\n BitsPerSample: %d\n DataSize: %d\n", myFormat, myChannels, mySampleRate, myByteRate, myBlockAlign, myBitsPerSample, myDataSize);
		std::cout << myDataSize;
		return summary;
	}
};

class ofxSynthSampler : public ofxSynth {
	public:
		ofxSynthSampler()	{direction=1;inPoint=0.0;outPoint=1.0;};
		void				loadFile(string file);
		
		void				trigger();
		void				setFrequencyMidiNote(float note);
		void				setFrequencySyncToLength(int length);
		void				setDirectionForward(){direction = 1;};
		void				setDirectionBackward(){direction = -1;};
		void				setLoopPoints(float i, float o);

		void				setSampleRate(int rate);
		void				audioRequested( float* buffer, int numFrames, int numChannels );

	private:
		int					sampleRate, direction;
		float				inPoint, outPoint;
		ofxSynthSample		sample;
};

/** ofxSynthWaveWriter
 
 Class to write sample streams to the file system.
 Streams to disk, and can write as long as you have hard drive space
 //TODO: figure out why waves generated by this class are not opened by quicklook
 
 */
 
class ofxSynthWaveWriter : public ofSoundEffectPassthrough {
	public:
		string getName() { return "ofxSynthWaveWriter"; }
		bool startWriting(string filename = "out.wav");
		void stopWriting();
		virtual void process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels );
	private:
		void flush();

		bool writing;
		int offset, numChannels, numFrames;
		
		// the copied stuff from this code: https://github.com/nevyn/game-music-emu
		enum { buf_size = 32768 * 2 };
		unsigned char* buf;
		FILE*   file;
		long    sample_count_;
		long    rate;
		long    buf_pos;
		int     chan_count;

};