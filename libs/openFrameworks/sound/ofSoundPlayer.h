#pragma once

#include "ofConstants.h"

#ifndef TARGET_OF_IPHONE
extern "C" {
#include "fmod.h"
#include "fmod_errors.h"
}
#endif

//		TO DO :
//		---------------------------
// 		-fft via fmod, as in the last time...
// 		-close fmod if it's up
//		-loadSoundForStreaming(char * fileName);
//		---------------------------

// 		interesting:
//		http://www.compuphase.com/mp3/mp3loops.htm


// ---------------------------------------------------------------------------- SOUND SYSTEM FMOD
// --------------------- global functions:

//TODO: FIX THIS SHIT!!!!!!
#warning FIX THIS. 
void ofSoundStopAll();
void ofSoundSetVolume(float vol);
void ofSoundUpdate();						// calls FMOD update.
float * ofSoundGetSpectrum(int nBands);		// max 512...
void ofSoundShutdown();

#include "ofBaseTypes.h"
#include "ofBaseSoundPlayer.h"

#ifdef TARGET_OF_IPHONE 
	#define OF_SOUND_PLAYER_IPHONE
#else
	#define OF_SOUND_PLAYER_FMOD
#endif

#ifdef OF_SOUND_PLAYER_QUICKTIME
#include "ofQuicktimeSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofQuicktimeSoundPlayer()
#endif

#ifdef OF_SOUND_PLAYER_FMOD
#include "ofFmodSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofFmodSoundPlayer()
#endif

#ifdef TARGET_OF_IPHONE
#include "ofxOpenALSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofxOpenALSoundPlayer()
#endif

//---------------------------------------------
class ofSoundPlayer : public ofBaseSoundPlayer {
	
	public:
		
		ofSoundPlayer();
		~ofSoundPlayer();
		
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

