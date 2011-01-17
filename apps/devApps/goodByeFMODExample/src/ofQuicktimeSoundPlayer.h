#pragma once

#include "ofConstants.h"
#include "ofBaseSoundPlayer.h"

#include <Carbon/Carbon.h>
#include <QuickTime/Movies.h>
#include <CoreFoundation/CoreFoundation.h>



//-------------------------------------------------------- 
typedef struct {
	float * fftValuesL;
	float * fftValuesR;
	int numberOfBands;
} FFTResults;

void ofQuicktimeSoundStopAll();
void ofQuicktimeSoundSetVolume(float vol);				// use changes in the master volume and set everyone sound that's playing based on
void ofQuicktimeSoundUpdate();						

//-------------------------------------------------------- the manager for quicktime players is a singleton
//class ofQuickTimePlayerManager {
//	
//	public:
//		
//		static ofQuickTimePlayerManager& Instance(){
//			static ofQuickTimePlayerManager singleton;
//			return singleton;
//		}
//	private:
//		
//		ofQuickTimePlayerManager() {};                                 // Private constructor
//		ofQuickTimePlayerManager(const ofQuickTimePlayerManager&);                 // Prevent copy-construction
//		ofQuickTimePlayerManager& operator=(const ofQuickTimePlayerManager&);      // Prevent assignment
//	
//		
//};


//-------------------------------------------------------- 
class ofQuicktimeSoundPlayer : public ofBaseSoundPlayer {

	public:

		ofQuicktimeSoundPlayer();
		virtual ~ofQuicktimeSoundPlayer();

		void loadSound(string fileName, bool stream = false);
		void unloadSound();
		void play();
		void stop();

	
		void initializeQuicktime();
		void closeQuicktime();

		void setVolume(float vol);
		void setPan(float vol);	
		void setSpeed(float spd);
		void setPaused(bool bP);
		
		void setLoop(bool bLp);
		void setMultiPlay(bool bMp);				
		void setPosition(float pct);
		
		void tryMultiPlay();
		float getPosition();
		bool getIsPlaying();
		float getSpeed();
		float getPan();

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
		bool bIWasPlayingAndMySpeedWasSetToZero;
		
		Movie soundToPlay;
		vector < Movie > soundsForMultiPlay;				// this is a vector of reference movies created for "multiplay" audio --
															// if they are done playing, we will remove them in the update function.
	
		
		//------------------------------
		// FFT
		UInt32 numberOfBandLevels;
		UInt32 numberOfChannels;
		QTAudioFrequencyLevels *freqResults;
		FFTResults fftValues;
		// TODO: this is different then fmod API. 
		// for quicktime, it needs to be written per sound.  
		// for most people, this is fine -- and not much of a change....
		void enableSpectrumCalculation(int nBands);	// just stereo for now
		void disableSpectrumCalculation();
		FFTResults getSpectrum();	
		//------------------------------
	
	
	
};

