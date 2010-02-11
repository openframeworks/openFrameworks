#ifndef _TEST_APP
#define _TEST_APP

// (un)comment depending on whether you want to use the microphone or not
//#define USING_INPUT

#define SAMPLERATE 44100
#define BUFFER_SIZE 256

#include "ofMain.h"




class testApp : public ofSimpleApp{
public:
	void setup();
	void update();
	void draw();
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
	void audioReceived( float * input, int bufferSize, int nChannels );
	float buffer[BUFFER_SIZE];
	float inputVolume;
	
	
};

#endif

