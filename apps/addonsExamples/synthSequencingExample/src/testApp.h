#pragma once

#include "ofMain.h"
#include "ofxSynth.h"
#include "ofxSynthSampler.h"

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		
		void audioRequested(float * output, int bufferSize, int nChannels);	
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		int		sampleRate;
		float 	* lAudio;
		float   * rAudio;

		float volume;
		
		//ofSoundSourceTestTone tone;
		
		ofSoundStream soundStream;
		ofxSynthDelayline delay;
		
		ofxSynth synth;
		ofxSynthSampler sampler;
		ofxSynthFilter filter;
		ofxSoundSourceMultiplexor multiplex;
		ofxSoundEffectPassthrough passthrough;
		ofxSoundMixer mixer;
		ofxSynthWaveWriter writer;

		int beatLength, frameCounter;
				
		int effectIndex, beatPos;
		float cutStart, cutEnd;
};
