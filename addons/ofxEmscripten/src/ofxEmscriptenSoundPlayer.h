#pragma once

#include "ofSoundBaseTypes.h"
#include "ofConstants.h"

class ofxEmscriptenSoundPlayer: public ofBaseSoundPlayer {
public:
	ofxEmscriptenSoundPlayer();
	~ofxEmscriptenSoundPlayer();

	bool load(const std::filesystem::path& fileName, bool stream = false);
	bool load(const std::string& fileName, bool stream = false);
	void unload();
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

	float getPosition() const;
	int getPositionMS() const;
	bool isPlaying() const;
	float getSpeed() const;
	float getPan() const;
	bool isLoaded() const;
	float getVolume() const;
	int getDurationMS() const;
	double getDurationSecs() const;

	static float * getSystemSpectrum(int bands);

private:
	void setPositionSecs(double s);
	static std::vector<float> systemSpectrum;
	int context;
	bool multiplay;
	float volume;
	float speed;
	float pan;
	bool playing;
	int player_id;
};
