/*
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

#include "maxiFFT.h"
#include <iostream>
#include "math.h"

using namespace std;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//F F T
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void maxiFFT::setup(int _fftSize, int _windowSize, int _hopSize) {
	_fft = new fft(_fftSize);
	fftSize = _fftSize;
	windowSize = _windowSize;
	bins = fftSize / 2;
	hopSize = _hopSize;
	buffer = (float *) malloc(fftSize * sizeof(float));
	magnitudes = (float *) malloc(bins * sizeof(float));
	phases = (float *) malloc(bins * sizeof(float));
	powers = (float *) malloc(bins * sizeof(float));
	avgPower = new float;
	memset(buffer, 0, fftSize * sizeof(float));
	memset(magnitudes, 0, bins * sizeof(float));
	memset(phases, 0, bins * sizeof(float));
	memset(powers, 0, bins * sizeof(float));
	*avgPower = 0;
	pos =windowSize - hopSize;
	newFFT = 0;
	window = (float*) malloc(fftSize * sizeof(float));
	memset(window, 0, fftSize * sizeof(float));
	fft::genWindow(3, windowSize, window);
}

bool maxiFFT::process(float value) {
	//add value to buffer at current pos
	buffer[pos++] = value;
	//if buffer full, run fft
	newFFT = pos == windowSize;
	if (newFFT) {
#if defined(__APPLE_CC__) && !defined(_NO_VDSP)
		_fft->powerSpectrum_vdsp(0, buffer, window, magnitudes, phases);
#else
		_fft->powerSpectrum(0, buffer, window, magnitudes, phases, powers, avgPower);		
#endif
		//shift buffer back by one hop size
		memcpy(buffer, buffer + hopSize, (windowSize - hopSize) * sizeof(float));
		//set the new data to zero (is this necessary?, it will get overwritten)
		memset(buffer + windowSize - hopSize, 0, hopSize * sizeof(float));
		//reset pos to start of hop
		pos= windowSize - hopSize;
	}
	return newFFT;
}

maxiFFT::~maxiFFT() {
	delete _fft;
	delete[] buffer,magnitudes,phases,powers, window;
	delete avgPower;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//I N V E R S E  F F T
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void maxiIFFT::setup(int _fftSize, int _windowSize, int _hopSize) {
	_fft = new fft(_fftSize);	
	fftSize = _fftSize;
	windowSize = _windowSize;
	bins = fftSize / 2;
	hopSize = _hopSize;
	buffer = (float *) malloc(fftSize * sizeof(float));
	ifftOut = (float *) malloc(fftSize * sizeof(float));
	memset(buffer, 0, windowSize * sizeof(float));
	pos =0;
	window = (float*) malloc(fftSize * sizeof(float));
	memset(window, 0, fftSize * sizeof(float));
	fft::genWindow(3, windowSize, window);
}

float maxiIFFT::process(float *magnitudes, float *phases) {
	if (0==pos) {
		//do ifft
		memset(ifftOut, 0, fftSize * sizeof(float));
#if defined(__APPLE_CC__) && !defined(_NO_VDSP)
		_fft->inversePowerSpectrum_vdsp(0, ifftOut, window, magnitudes, phases);
#else
		_fft->inversePowerSpectrum(0, ifftOut, window, magnitudes, phases);
#endif
		//add to output
		//shift back by one hop
		memcpy(buffer, buffer+hopSize, (fftSize - hopSize) * sizeof(float));
		//clear the end chunk
		memset(buffer + (fftSize - hopSize), 0, hopSize * sizeof(float)); 
		//merge new output
		for(int i=0; i < fftSize; i++) {
			buffer[i] += ifftOut[i];
		}
	}
	
	nextValue = buffer[pos];
	//limit the values, this alg seems to spike occasionally (and break the audio drivers)
	if (nextValue > 0.99999f) nextValue = 0.99999f;
	if (nextValue < -0.99999f) nextValue = -0.99999f;
	if (hopSize == ++pos ) {
		pos=0;
	}
	
	return nextValue;
}

maxiIFFT::~maxiIFFT() {
	delete _fft;
	delete[] ifftOut, buffer, window;
};







//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//O C T A V E  A N A L Y S E R
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void maxiFFTOctaveAnalyzer::setup(float samplingRate, int nBandsInTheFFT, int nAveragesPerOctave){
	
    samplingRate = samplingRate;
    nSpectrum = nBandsInTheFFT;
    spectrumFrequencySpan = (samplingRate / 2.0f) / (float)(nSpectrum);
    nAverages = nBandsInTheFFT;
    // fe:  2f for octave bands, sqrt(2) for half-octave bands, cuberoot(2) for third-octave bands, etc
    if (nAveragesPerOctave==0) // um, wtf?
		nAveragesPerOctave = 1;
    nAveragesPerOctave = nAveragesPerOctave;
    averageFrequencyIncrement = pow(2.0f, 1.0f/(float)(nAveragesPerOctave));
    // this isn't currently configurable (used once here then no effect), but here's some reasoning:
    // 43 is a good value if you want to approximate "computer" octaves: 44100/2/2/2/2/2/2/2/2/2/2
    // 55 is a good value if you'd rather approximate A-440 octaves: 440/2/2/2
    // 65 is a good value if you'd rather approximate "middle-C" octaves:  ~262/2/2
    // you could easily double it if you felt the lowest band was just rumble noise (as it probably is)
    // but don't go much smaller unless you have a huge fft window size (see below for more why)
    // keep in mind, if you change it, that the number of actual bands may change +/-1, and
    // for some values, the last averaging band may not be very useful (may extend above nyquist)
    firstOctaveFrequency = 55.0f;
    // for each spectrum[] bin, calculate the mapping into the appropriate average[] bin.
    // this gives us roughly log-sized averaging bins, subject to how "fine" the spectrum bins are.
    // with more spectrum bins, you can better map into the averaging bins (especially at low
    // frequencies) or use more averaging bins per octave.  with an fft window size of 2048,
    // sampling rate of 44100, and first octave around 55, that's about enough to do half-octave
    // analysis.  if you don't have enough spectrum bins to map adequately into averaging bins
    // at the requested number per octave then you'll end up with "empty" averaging bins, where
    // there is no spectrum available to map into it.  (so... if you have "nonreactive" averages,
    // either increase fft buffer size, or decrease number of averages per octave, etc)
    spe2avg = new int[nSpectrum];
    int avgidx = 0;
    float averageFreq = firstOctaveFrequency; // the "top" of the first averaging bin
    // we're looking for the "top" of the first spectrum bin, and i'm just sort of
    // guessing that this is where it is (or possibly spectrumFrequencySpan/2?)
    // ... either way it's probably close enough for these purposes
    float spectrumFreq = spectrumFrequencySpan;
    for (int speidx=0; speidx < nSpectrum; speidx++) {
		while (spectrumFreq > averageFreq) {
			avgidx++;
			averageFreq *= averageFrequencyIncrement;
		}
		spe2avg[speidx] = avgidx;
		spectrumFreq += spectrumFrequencySpan;
    }
    nAverages = avgidx;
    averages = new float[nAverages];
    peaks = new float[nAverages];
    peakHoldTimes = new int[nAverages];
    peakHoldTime = 0; // arbitrary
    peakDecayRate = 0.9f; // arbitrary
    linearEQIntercept = 1.0f; // unity -- no eq by default
    linearEQSlope = 0.0f; // unity -- no eq by default
}

void maxiFFTOctaveAnalyzer::calculate(float * fftData){
	
	int last_avgidx = 0; // tracks when we've crossed into a new averaging bin, so store current average
    float sum = 0.0f; // running total of spectrum data
    int count = 0; // count of spectrums accumulated (for averaging)
    for (int speidx=0; speidx < nSpectrum; speidx++) {
		count++;
		sum += fftData[speidx] * (linearEQIntercept + (float)(speidx) * linearEQSlope);
		int avgidx = spe2avg[speidx];
		if (avgidx != last_avgidx) {
			
			for (int j = last_avgidx; j < avgidx; j++){
				averages[j] = sum / (float)(count);
			}
			count = 0;
			sum = 0.0f;
		}
		last_avgidx = avgidx;
    }
    // the last average was probably not calculated...
    if ((count > 0) && (last_avgidx < nAverages)){
		averages[last_avgidx] = sum / (float)(count);
	}
	
    // update the peaks separately
    for (int i=0; i < nAverages; i++) {
		if (averages[i] >= peaks[i]) {
			// save new peak level, also reset the hold timer
			peaks[i] = averages[i];
			peakHoldTimes[i] = peakHoldTime;
		} else {
			// current average does not exceed peak, so hold or decay the peak
			if (peakHoldTimes[i] > 0) {
				peakHoldTimes[i]--;
			} else {
				peaks[i] *= peakDecayRate;
			}
		}
    }
}
