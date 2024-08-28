#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"
#include <map>

class ofApp : public ofxAndroidApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

		void audioOut(ofSoundBuffer & buffer);
		void audioIn(ofSoundBuffer & buffer);

		float 	pan;
		int		sampleRate;
		bool 	bNoise;
		float 	volume;

		float 	* lAudio;
		float   * rAudio;

		//------------------- for the simple sine wave synthesis
		float 	targetFrequency;
		float 	phase;
		float 	phaseAdder;
		float 	phaseAdderTarget;

		ofSoundStream soundStream;
};
