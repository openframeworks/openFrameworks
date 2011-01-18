#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "FloatingSine.h"
#include "ofxSynthFilter.h"

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


	int		sampleRate;
	float 	* lAudio;
	float   * rAudio;
	float volume;
	int baseNote;
	bool bBackgroundAuto;

	ofSoundEffectPassthrough passthrough;
	ofxSynthFilter filter;	
	ofSoundMixer mixer;
	
	vector<FloatingSine*> floating;
	
	ofImage particle;

	int scale; // 0 = pentatonic, 1 = lydian
	void setScale( int scale /* 0=pentatonic, 1=lydian */ );
};

#endif
