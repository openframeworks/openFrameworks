#pragma once

#include "ofEvents.h"
#include "ofxGPS.h"


class ofxAndroidGPS{
public:
	static void startGPS();
	static void stopGPS();

	static ofEvent<ofxLocation> locationChangedE;
};

template<class Listener>
void ofRegisterGPSEvent(Listener * listener){
	ofAddListener(ofxAndroidGPS::locationChangedE,listener,&Listener::locationChanged);
}

template<class Listener>
void ofUnregisterGPSEvent(Listener * listener){
	ofRemoveListener(ofxAndroidGPS::locationChangedE,listener,&Listener::locationChanged);
}


