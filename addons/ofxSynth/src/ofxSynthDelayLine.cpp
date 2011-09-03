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

#include "ofxSynthDelayLine.h"

ofxSynthDelayline::ofxSynthDelayline(){
	setSize(1.1);
	setFeedback(0.5);
	setMix(0.5);
}
void ofxSynthDelayline::process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels )
{
	for (int i = 0; i < numFrames; i++){
		if (phase>=size) {
			phase=0;
		}
		memory[phase]=(memory[phase]*feedback)+((*input)*feedback)*0.5;
		phase+=1;
		for (int j=0; j<numOutChannels; j++) {
			// puts the sound in memory on to all the output channels
			*output++=memory[phase+1]*mix+(*input)*(1.0-mix);
		}
		input+=numInChannels;
	}
}
void ofxSynthDelayline::setSize(float _size){
	if(_size>2) _size=2;
	size = _size*sampleRate;
}
void ofxSynthDelayline::setFeedback(float _feedback){
	feedback = _feedback;
}
void ofxSynthDelayline::setMix(float _mix){
	mix = _mix;
}
