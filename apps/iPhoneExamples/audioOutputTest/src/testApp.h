#ifndef _TEST_APP
#define _TEST_APP

// (un)comment depending on whether you want to use the microphone or not
//#define USING_INPUT

#define SAMPLERATE 44100
#define BUFFER_SIZE 256

#include "ofMain.h"

class TriangleWave {
public:
	TriangleWave() {
		phase = 0;
		setFrequency(200);
	}
	
	void setFrequency(float frequency) {
		phaseIncrement = frequency*2*PI/SAMPLERATE;		// calculate how much we need to increment the phase for each sample
	}
	
	float getSample() {
		phase += phaseIncrement;						// update the phase of the oscillator
		if(phase>=2*PI) phase -= 2*PI;					// wrap it around
		return phase<PI ? (-1.f + (2.f*phase/PI)):(1.f - (2.f*(phase-PI)/PI)); // generate the signal
	}
	
	float phase;
	float phaseIncrement;
};




class testApp : public ofSimpleApp{
public:
	void setup();
	void update();
	void draw();
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
#ifdef USING_INPUT
	void audioReceived( float * input, int bufferSize, int nChannels );
	float buffer[BUFFER_SIZE];
	
#endif
	
	float inputVolume;
	void audioRequested( float * output, int bufferSize, int nChannels );
	bool playSound;
	TriangleWave leftWave;
	TriangleWave rightWave;
	
	
};

#endif

