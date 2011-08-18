#pragma once

#include "ofBaseSoundPlayer.h"
#include <jni.h>

class ofxAndroidSoundPlayer: public ofBaseSoundPlayer{
public:
	ofxAndroidSoundPlayer();
	virtual ~ofxAndroidSoundPlayer();

	void loadSound(string fileName, bool stream = false);
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
	bool getIsPlaying();
	float getSpeed();
	float getPan();
	bool getIsPaused();

private:
	jobject javaSoundPlayer;
	jclass javaClass;
};
