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

#include "ofxSynthFilter.h"
void ofxSynthFilter::setup(){
	cutoff = 1.0;
	resonance = 0.0;
	b0 = b1 = b2 = b3 = b4 = 0.0;  //filter buffers (beware denormals!)
	t1 = t2 = 0.0;					//temporary buffers
	lowPass = true;
	calc();
}
void ofxSynthFilter::setRes(float _res)
{
	resonance = _res;
	calc();
}
void ofxSynthFilter::setCutoff(float _cutoff)
{
	if(lowPass){
		cutoff = _cutoff;
	}else{
		cutoff = _cutoff-1.0;
	}
	cutoff = fmin(fmax(cutoff, 0.0), 1.0);
	calc();
}
void ofxSynthFilter::setLowPass(){
	lowPass = true;
}
void ofxSynthFilter::setHighPass(){
	lowPass = false;
}

void ofxSynthFilter::calc()
{
	q = 1.0 - cutoff;
	p = cutoff + 0.8 * cutoff * q;
	f = p + p - 1.0;
	q = resonance * (1.0 + 0.5 * q * (1.0f - q + 5.6 * q * q));
}
void ofxSynthFilter::process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels ){
	float currValue;
	for (int i = 0; i<numFrames; i++) {
		currValue = *input;
		currValue -= q * b4;				//feedback
		t1 = b1;  b1 = (currValue + b0) * p - b1 * f;
		t2 = b2;  b2 = (b1 + t1) * p - b2 * f;
		t1 = b3;  b3 = (b2 + t2) * p - b3 * f;
		b4 = (b3 + t1) * p - b4 * f;
		b4 -= b4 * b4 * b4 * 0.166667;		//clipping
		b0 = currValue;
		if(lowPass){
			currValue = b4;
		}else{
			currValue-=b4;
		}
		input+=numInChannels;
//		processSample(&output[i*numOutChannels]);

		for (int j=0; j<numOutChannels; j++) {
			*output++=currValue;
		}

	}
}
