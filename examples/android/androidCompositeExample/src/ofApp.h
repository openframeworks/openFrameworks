#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"
#include <map>

class ofApp : public ofxAndroidApp{
	
	public:
		
		void setup();
		void exit();
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

        void deviceRefreshRateChanged(int refreshRate);
        void deviceHighestRefreshRateChanged(int refreshRate);
        void deviceRefreshRateChangedEvent(int &refreshRate);
        void deviceHighestRefreshRateChangedEvent(int & refreshRate);

		// Using Shaders and fonts
		ofPath text;
		ofTrueTypeFont font;
		ofShader shader;
		bool doShader;

		// Using ofSoundPlayer
	    ofSoundPlayer  beats;
	    ofSoundPlayer  synth;
	    ofSoundPlayer  vocals;

		// Using ofSoundStream
	    void audioOut(ofSoundBuffer & buffer);
		void audioIn(ofSoundBuffer & buffer);

	    float pan;
		int	sampleRate;
		bool bNoise;
		float volume;
		int initialBufferSize;
		float * lAudio;
		float * rAudio;
		// For the simple sine wave synthesis
		float targetFrequency;
		float phase;
		float phaseAdder;
		float phaseAdderTarget;

		ofSoundStream soundStream;
};
