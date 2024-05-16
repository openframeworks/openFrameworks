#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

#include "ofxSoundMixer.h"
#include "ofxSoundPlayerObject.h"

#include "SineWaveGenerator.h"
#include "NoiseGenerator.h"
#include "LowPassFilter.h"
#include "DigitalDelay.h"
#include "ofxSoundObjects.h"

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

    ofShader shader;

    bool bUseShader;
    ofTrueTypeFont font;
    ofPoint mousePoint;
    int fontSize;


    // these are all subclasses of ofSoundObject
    NoiseGenerator noise;
    LowPassFilter filter;
    DigitalDelay delay;

    float filterCutoff;
    float delayFeedback;

    ofxSoundMixer mixer;
    vector<unique_ptr<ofxSoundPlayerObject>> players;
    vector<unique_ptr<ofxSoundPlayerObject>> playersphase2;

    int sizedir1, sizedir2;

    int phase;

    ofSoundStream stream;

    ofEventListener playerEndListener;
    void playerEnded(size_t & id);

};
