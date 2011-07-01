/*
 *  ofSoundPlayer.cpp
 *  goodbyeFmod
 *
 *  Created by zachary lieberman on 1/13/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofBaseTypes.h"
#include "ofBaseSoundPlayer.h"


#define OF_SOUND_PLAYER_FMOD

#ifdef OF_SOUND_PLAYER_QUICKTIME
#include "ofQuicktimeSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofQuicktimeSoundPlayer()
#endif

#ifdef OF_SOUND_PLAYER_FMOD
#include "ofFmodSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofFmodSoundPlayer()
#endif



//---------------------------------------------
class ofSoundPlayer_ : public ofBaseSoundPlayer {
	
public:
	
	ofSoundPlayer_ ();
	~ofSoundPlayer_();
	
	bool setPlayer(ofBaseSoundPlayer * newPlayer);
	ofBaseSoundPlayer *	getPlayer();
	
	
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
	
	float getPosition();
	bool getIsPlaying();
	float getSpeed();
	float getPan();
	
	
protected:
	
	
	ofBaseSoundPlayer		* player;
	
	
};

