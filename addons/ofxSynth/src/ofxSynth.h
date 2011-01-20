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

#include "ofxSynthDelayLine.h"
#include "ofxSynthFilter.h"
#include "ofxSynthEnvelope.h"
#include "ofxSynthWaveWriter.h"

class maxiOsc {
	
	double frequency;
	double phase;
	double startphase;
	double endphase;
	double output;
	double tri;
	int sampleRate;
	
public:
	maxiOsc();
	double saw(double frequency);
	double triangle(double frequency,double phase);
	double square(double frequency);
	void setSampleRate(int rate);
};

typedef maxiOsc ofxMaxiOsc;


/** ofxSynth
 
 A classic subtractive synth
 
*/
class ofxSynth : public ofSoundSource{
	public:
		ofxSynth();
		virtual string	getName() { return "ofxSynth"; }
		void	setup();
		
		void	trigger();

		virtual void	setFrequency(float freq);
		virtual void	setFrequencyMidiNote(float note);
		void	setPortamento(float p){portamento = p;};
		
		void	setFilter(float _cutoff, float _res);
		void	setFilterLowPass(){filter.setLowPass();filterMode = 1;};
		void	setFilterHighPass(){filter.setHighPass();filterMode = 2;};

		float	getCutoff(){return cutoff;};
		float	getRes(){return res;};
				
		void	setWaveSquare(){waveMode = 0;};
		void	setWaveTri(){waveMode = 1;};
		void	setWaveSaw(){waveMode = 2;};
		
		void		audioRequested( float* buffer, int numFrames, int numChannels );
		void				setSampleRate(int rate);

		ofxMaxiOsc wave;
		ofxSynthADSR env, modEnv;
		ofxSynthFilter filter;
		
	protected:
		float currentFrequency, startFrequency, targetFrequency, currentAmp, noteTime;
		float sustain, gain, cutoff, res, filterMod, portamento;
		int filterMode, waveMode, sampleRate, phase;
		bool usesEnv, hasTrigger;
};