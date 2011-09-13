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
#include "ofxSoundEffect.h"

class ofxSynthDelayline : public ofxSoundEffect {
	public:
		
		ofxSynthDelayline();
		string getName() { return "ofxSynthDelayLine"; }
		virtual void process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels );
		void setSize(float _size);
		void setFeedback(float _feedback);
		void setMix(float _mix);
		
	private:
		float frequency;
		int phase;
		float startphase;
		float endphase;
		float output;
		float memory[88200];
		int size;
		float feedback;
		float mix;
};
