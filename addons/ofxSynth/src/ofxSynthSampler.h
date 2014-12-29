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

#include "ofxSynth.h"

class ofxSynthSample  {
	public:
	
		ofxSynthSample();
		
		double	position;
		short 	myChannels;
		int   	mySampleRate;
		long	length;
		void getLength();

		char* 	myData;
		
		// get/set for the Path property

		~ofxSynthSample()
		{
			delete myData;
			myChunkSize = 0;
			mySubChunk1Size = 0;
			myFormat = 0;
			myChannels = 0;
			mySampleRate = 0;
			myByteRate = 0;
			myBlockAlign = 0;
			myBitsPerSample = 0;
			myDataSize = 0;
		}
		
		bool load(string fileName);

		// read a wav file into this class
		bool read();
		
		double play4(double frequency, double start, double end);
		double play(double frequency, double start, double end);
		
		// return a printable summary of the wav file
		char *getSummary();
		void				setSampleRate(int rate);

	private:
		string 	myPath;
		int		sampleRate;
		int 	myChunkSize;
		int		mySubChunk1Size;
		short 	myFormat;
		int   	myByteRate;
		short 	myBlockAlign;
		short 	myBitsPerSample;
		int		myDataSize;
		double	speed;
		double	output;
	
};

class ofxSynthSampler : public ofxSynth {
	public:
		ofxSynthSampler()	{direction=1;inPoint=0.0;outPoint=1.0;};
		virtual string		getName() { return "ofxSynthSampler"; }

		void				loadFile(string file);
		
		void				trigger();
		void				setFrequencyMidiNote(float note);
		void				setFrequencySyncToLength(int length);
		void				setDirectionForward(){direction = 1;};
		void				setDirectionBackward(){direction = -1;};
		void				setLoopPoints(float i, float o);

		void				setSampleRate(int rate);
		virtual void		audioOut( float* buffer, int numFrames, int numChannels,long unsigned long tickCount  );

	private:
		int					sampleRate, direction;
		float				inPoint, outPoint;
		ofxSynthSample		sample;
};
