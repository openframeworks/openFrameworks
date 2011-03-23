#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSynth.h"
#include "ofxSynthSampler.h"

class testApp : public ofBaseApp{

	public:


		void setup();
		void audioRequested(float * output, int bufferSize, int nChannels);
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);


		int		sampleRate;
		float 	* lAudio;
		float   * rAudio;

		float volume;
		
		ofxSoundSourceTestTone tone;
		
		ofxSynthDelayline delay;
		
		ofxSynth synth;
		ofxSynthSampler sampler;
		ofxSynthFilter filter;
		ofxSoundSourceMultiplexor multiplex;
		ofxSoundEffectPassthrough passthrough;
		ofxSoundMixer mixer;
		ofxSynthWaveWriter writer;

		ofSoundStream soundStream;

		int beatLength, frameCounter;
		
		
		int effectIndex, beatPos;
		float cutStart, cutEnd;
};

#endif
