#pragma once

#include "ofBaseSoundPlayer.h"

class ofxEmscriptenSoundPlayer: public ofBaseSoundPlayer {
public:
	ofxEmscriptenSoundPlayer();
	~ofxEmscriptenSoundPlayer();


	bool loadSound(string fileName, bool stream = false);
	void unloadSound();
	void play();
	void stop();

	void setVolume(float vol);
	void setPan(float vol); // -1 = left, 1 = right
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
	bool isLoaded();
	float getVolume();
	int getDurationMS();
	double getDurationSecs();

	static float * getSystemSpectrum(int bands);

private:
	void setPositionSecs(double s);
	static vector<float> systemSpectrum;
	int context;
	int sound;
	bool multiplay;
	float speed;
	float volume;
	bool playing;
};
