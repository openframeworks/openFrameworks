#pragma once

#include "ofxiOS.h"

class ofApp : public ofxiOSApp{
	
	public:
		void setup();
		void update();
		void draw();
	
		void exit();
	
		void audioOut(ofSoundBuffer & buffer);

		void touchDown(ofTouchEventArgs & touch);
		void touchMoved(ofTouchEventArgs & touch);
		void touchUp(ofTouchEventArgs & touch);
		void touchDoubleTap(ofTouchEventArgs & touch);
		void touchCancelled(ofTouchEventArgs & touch);
	
		void lostFocus();
		void gotFocus();
		void gotMemoryWarning();
		void deviceOrientationChanged(int newOrientation);

		float pan;
		int sampleRate;
		bool bNoise;
		float volume;
        vector <float> lAudio;
        vector <float> rAudio;

		//------------------- for the simple sine wave synthesis
		float targetFrequency;
		float phase;
		float phaseAdder;
		float phaseAdderTarget;

	ofSoundStream soundStream;

};

