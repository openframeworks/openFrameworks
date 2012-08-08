
#pragma once

#include "ofConstants.h"



//----------------------------------------------------------
// ofBaseSoundPlayer
//----------------------------------------------------------
class ofBaseSoundPlayer {
	
public:
	
	ofBaseSoundPlayer(){};
	virtual ~ofBaseSoundPlayer(){};
	
	virtual bool loadSound(string fileName, bool stream = false) = 0;
	virtual void unloadSound() = 0;
	virtual void play() = 0;
	virtual void stop() = 0;
	
	virtual void setVolume(float vol) = 0;
	virtual void setPan(float vol) = 0; // -1 = left, 1 = right
	virtual void setSpeed(float spd) = 0;
	virtual void setPaused(bool bP) = 0;
	virtual void setLoop(bool bLp) = 0;
	virtual void setMultiPlay(bool bMp) = 0;
	virtual void setPosition(float pct) = 0; // 0 = start, 1 = end;
	virtual void setPositionMS(int ms) = 0;

	virtual float getPosition() = 0;
	virtual int getPositionMS() = 0;
	virtual bool getIsPlaying() = 0;
	virtual float getSpeed() = 0;
	virtual float getPan() = 0;
	virtual bool isLoaded() = 0;
	virtual float getVolume() = 0;
	
};
