#ifndef _OF_SAMPLE_SOUND
#define _OF_SAMPLE_SOUND

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
void ofSoundStopAll();
void ofSoundSetVolume(float vol);
void ofSoundUpdate();						// calls FMOD update.
float * ofSoundGetSpectrum(int nBands);		// max 512...


// --------------------- player functions:
class ofSoundPlayer {

	public:

		ofSoundPlayer();
		virtual ~ofSoundPlayer();

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

		static void initializeFmod();
		static void closeFmod();

		bool isStreaming;
		bool bMultiPlay;
		bool bLoop;
		bool bLoadedOk;
		bool bPaused;
		float pan; // 0 - 1
		float volume; // 0 - 1
		float internalFreq; // 44100 ?
		float speed; // -n to n, 1 = normal, -1 backwards
		unsigned int length; // in samples;

		#ifndef TARGET_OF_IPHONE
			FMOD_RESULT result;
			FMOD_CHANNEL * channel;
			FMOD_SOUND * sound;
		#endif
};

#endif // _OF_SAMPLE_SOUND
