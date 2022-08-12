#pragma once

#include "ofSoundBaseTypes.h"
#include <jni.h>

class ofxAndroidSoundPlayer: public ofBaseSoundPlayer{
public:
	ofxAndroidSoundPlayer();
	virtual ~ofxAndroidSoundPlayer();

	bool load(const of::filesystem::path& fileName, bool stream = false);
	void unload();
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

	float getPosition() const;
	int getPositionMS() const;
	bool isPlaying() const;
	float getSpeed() const;
	float getPan() const;
	bool isPaused() const;
	float getVolume() const;
	bool isLoaded() const;

private:
	jobject javaSoundPlayer;
	jclass javaClass;
};
