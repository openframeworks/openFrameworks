#pragma once

#include "ofBaseSoundPlayer.h"
#include <jni.h>

class ofxAndroidSoundPlayer: public ofBaseSoundPlayer{
public:
	ofxAndroidSoundPlayer();
	virtual ~ofxAndroidSoundPlayer();

	bool loadSound(string fileName, bool stream = false);
	void unloadSound();
	void play();
	void stop();

	void setVolume(float vol);
	void setPan(float vol);
	void setSpeed(float spd);
	void setPaused(bool bP);
	void setLoop(bool bLp);
	void setMultiPlay(bool bMp);
	void setPosition(float pct); // 0 = start, 1 = end;
	void setPositionMS(int ms);

	float getPosition();
	int getPositionMS();
	bool getIsPlaying();
	float getSpeed();
	float getPan();
	bool getIsPaused();
	float getVolume();
	bool isLoaded();

private:
	jobject javaSoundPlayer;
	jclass javaClass;
};
