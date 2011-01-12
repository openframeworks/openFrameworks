#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSynth.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
		void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
		
		int		initialBufferSize; /* buffer size */ 
		int		sampleRate;
		
		ofxSynth syn;
		ofxSynthSampler sampler;
		ofxSynthDelayline dl;
		
		ofxMaxiMix mymix;
		float * mixBuffer, * mixBuffer2;
		double outputs[2];
		int beatLength, framecounter;


};

#endif
