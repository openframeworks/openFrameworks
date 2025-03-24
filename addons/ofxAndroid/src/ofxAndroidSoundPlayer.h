#pragma once

#include "ofSoundBaseTypes.h"
#include <jni.h>

class ofxAndroidSoundPlayer: public ofBaseSoundPlayer{
public:
	ofxAndroidSoundPlayer();
	~ofxAndroidSoundPlayer() override;

	bool load(const of::filesystem::path& fileName, bool stream) override;
	void unload() override;
	void play() override;
	void stop() override;

	void setVolume(float vol) override;
	void setPan(float vol) override;
	void setSpeed(float spd) override;
	void setPaused(bool bP) override;
	void setLoop(bool bLp) override;
	void setMultiPlay(bool bMp) override;
	void setPosition(float pct) override; // 0 = start, 1 = end;
	void setPositionMS(int ms) override;

	float getPosition() const override;
    int getPositionMS() const override;
	bool isPlaying() const override;
	float getSpeed() const override;
	float getPan() const override;
	bool isPaused() const;
	float getVolume() const override;
	bool isLoaded() const override;
    float getDuration() const override;
    unsigned int getDurationMS() const override;

private:
	jobject javaSoundPlayer;
	jclass javaClass;
};
