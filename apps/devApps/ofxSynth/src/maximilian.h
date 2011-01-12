/*
 *  maximilian.h
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

#ifndef MAXIMILIAN_H
#define MAXIMILIAN_H

//#define MAXIMILIAN_PORTAUDIO
#define MAXIMILIAN_RT_AUDIO

#include "ofMain.h"
#include "ofUtils.h"


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;
#define TWOPI 3.14159*2

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


class maxiEnvelope {
	
	double period;
	double output;
	double startval;
	double currentval;
	double nextval;
	
public:	
	double line(int numberofsegments,double segments[100]);
	void trigger(int index,double amp);
	int valindex;
	double amplitude;
	
};


class maxiDelayline {
	double frequency;
	int phase;
	double startphase;
	double endphase;
	double output;
	double memory[88200];
	
public:
	double dl(double input, int size, double feedback);
	double dl(double input, int size, double feedback, int position);

	
};


class maxiFilter { 	
	double gain;
	double input;
	double output;
	double inputs[10];
	double outputs[10];
	double cutoff1;
	double x;//speed
	double y;//pos
	double z;//pole
	double c;//filter coefficient
public:
	double cutoff;
	double resonance;
	double lores(double input,double cutoff1, double resonance);
	double hires(double input,double cutoff1, double resonance);
	double bandpass(double input,double cutoff1, double resonance);
	double lopass(double input,double cutoff);
	double hipass(double input,double cutoff);
	
};

class maxiMix  {
	double input;
	double two[2];
	double four[4];
	double eight[8];
public:
	double x;
	double y;
	double z;
	double *stereo(double input,double two[2],double x);
	double *quad(double input,double four[4], double x,double y);
	double *ambisonic(double input,double eight[8],double x,double y, double z);
	
};


class maxiSample  {
	
private:
	string 	myPath;
	int 	myChunkSize;
	int	mySubChunk1Size;
	short 	myFormat;
	int   	myByteRate;
	short 	myBlockAlign;
	short 	myBitsPerSample;
	int	myDataSize;
	double position;
	double speed;
	double output;
	
public:
	short 	myChannels;
	int   	mySampleRate;
	long length;
	void getLength();

	char* 	myData;
	
	// get/set for the Path property

	~maxiSample()
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
	
//	maxiSample();

	bool load(string fileName);
	
	void trigger();
	
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
	
	
	double play();
	
	double play(double speed);
	
	double play(double frequency, double start, double end);
		
	double play4(double frequency, double start, double end);
	
	double bufferPlay(unsigned char &bufferin,long length);
	
	double bufferPlay(unsigned char &bufferin,double speed,long length);
	
	double bufferPlay(unsigned char &bufferin,double frequency, double start, double end);
	
	double bufferPlay4(unsigned char &bufferin,double frequency, double start, double end);
	
	//int open(char *filename) {
	//		// open the wav file
	//		char *path = new char[50];
	//		strcpy(path, filename);
	//		sample *myWav = new sample(path);
	//		return 0;
	//	}	
	//		// print a summary of the wav file
	//	int info() {
	//		char *summary = myWav->getSummary();
	//		printf("Summary:\n%s", summary);	
	//		return 0;
	//	}
	//	int savefile() {	
	//		// write the summary back out
	//		strcpy(path, "testout.wav");
	//		myWav->setPath(path);
	//		myWav->save();
	//		return 0:
	//	}
	//	int close() {
	//		// collect the garbage
	//		delete summary;
	//		delete path;
	//		delete myWav;
	//		
	//		return 0;
	//	}		
	// write out the wav file
	bool save()
	{
		fstream myFile (myPath.c_str(), ios::out | ios::binary);
		
		// write the wav file per the wav file format
		myFile.seekp (0, ios::beg); 
		myFile.write ("RIFF", 4);
		myFile.write ((char*) &myChunkSize, 4);
		myFile.write ("WAVE", 4);
		myFile.write ("fmt ", 4);
		myFile.write ((char*) &mySubChunk1Size, 4);
		myFile.write ((char*) &myFormat, 2);
		myFile.write ((char*) &myChannels, 2);
		myFile.write ((char*) &mySampleRate, 4);
		myFile.write ((char*) &myByteRate, 4);
		myFile.write ((char*) &myBlockAlign, 2);
		myFile.write ((char*) &myBitsPerSample, 2);
		myFile.write ("data", 4);
		myFile.write ((char*) &myDataSize, 4);
		myFile.write (myData, myDataSize);
		
		return true;
	}
	
	// return a printable summary of the wav file
	char *getSummary()
	{
		char *summary = new char[250];
		sprintf(summary, " Format: %d\n Channels: %d\n SampleRate: %d\n ByteRate: %d\n BlockAlign: %d\n BitsPerSample: %d\n DataSize: %d\n", myFormat, myChannels, mySampleRate, myByteRate, myBlockAlign, myBitsPerSample, myDataSize);
		std::cout << myDataSize;
		return summary;
	}
};


class maxiMap {
public:
	static double inline linlin(double val, double inMin, double inMax, double outMin, double outMax) {
		return ((val - inMin) / (inMax - inMin) * (outMax - outMin)) + outMin;
	}
	
	static double inline linexp(double val, double inMin, double inMax, double outMin, double outMax) {
		//clipping
		val = max(min(val, inMax), inMin);
		return pow((outMax / outMin), (val - inMin) / (inMax - inMin)) * outMin;
	}
};


#endif
