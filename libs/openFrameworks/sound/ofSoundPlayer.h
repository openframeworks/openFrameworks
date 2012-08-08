#pragma once

#include "ofConstants.h"
#include "ofTypes.h"


// --------------------- global functions:

//TODO: FIX THIS SHIT!!!!!!
//#warning FIX THIS.
void ofSoundStopAll();
void ofSoundSetVolume(float vol);
void ofSoundUpdate();						// calls FMOD update.
float * ofSoundGetSpectrum(int nBands);		// max 512...
void ofSoundShutdown();

#include "ofBaseTypes.h"
#include "ofBaseSoundPlayer.h"


#ifdef OF_SOUND_PLAYER_QUICKTIME
#include "ofQuicktimeSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofQuicktimeSoundPlayer

#elif defined( OF_SOUND_PLAYER_FMOD )
#include "ofFmodSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofFmodSoundPlayer

#elif defined( OF_SOUND_PLAYER_OPENAL )
#include "ofOpenALSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofOpenALSoundPlayer

#elif defined( TARGET_OF_IPHONE )
#include "ofxOpenALSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofxOpenALSoundPlayer

#else
/*#include "ofxAndroidSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofxAndroidSoundPlayer*/
#include "ofBasicSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofBasicSoundPlayer

#ifdef TARGET_ANDROID
inline void ofSoundShutdown(){}
#endif
#endif

//---------------------------------------------
class ofSoundPlayer : public ofBaseSoundPlayer {

	public:

		ofSoundPlayer();

		void setPlayer(ofPtr<ofBaseSoundPlayer> newPlayer);
		ofPtr<ofBaseSoundPlayer> getPlayer();

		bool loadSound(string fileName, bool stream = false);
		void unloadSound();
		void play();
		void stop();

		void setVolume(float vol);
		void setPan(float vol); // -1 to 1 
		void setSpeed(float spd);
		void setPaused(bool bP);
		void setLoop(bool bLp);
		void setMultiPlay(bool bMp);
		void setPosition(float pct); // 0 = start, 1 = end;
		void setPositionMS(int ms);
		int getPositionMS();

		float getPosition();
		bool getIsPlaying();
		float getSpeed();
		float getPan();
		float getVolume();

		bool isLoaded(); 		

	protected:

		ofPtr<ofBaseSoundPlayer> player;


};

