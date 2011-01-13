#pragma once


#include "ofConstants.h"

#include <Carbon/Carbon.h>
#include <QuickTime/Movies.h>
#include <CoreFoundation/CoreFoundation.h>


// look at: 
// http://www.meandmark.com/quicktimepart3.html
// http://developer.apple.com/library/mac/#qa/qa2004/qa1371.html
// http://www.cocoadev.com/index.pl?QuickTimeAudioExtractionExample

// FFT: 
// http://developer.apple.com/library/mac/#qa/qa2005/qa1459.html
// http://new.math.uiuc.edu/ivanhoe/public_html/stolarsky/MyController.m.html
// http://developer.apple.com/library/mac/#samplecode/SillyFrequencyLevels/Introduction/Intro.html

// http://eqx.su/distfiles/svn-src/phonon/4.2/qt7/quicktimevideoplayer.cpp


// checking for quicktime 7: 
// http://developer.apple.com/library/mac/#samplecode/QTSetMovieAudioDevice/Listings/QTCode_cpp.html


// need to look at 
// http://developer.apple.com/library/mac/#qa/qa2004/qa1371.html





typedef struct {
	float * fftValuesL;
	float * fftValuesR;
	int numberOfBands;
} FFTResults;


// --------------------- global functions:
void ofQuicktimeSoundStopAll();
void ofQuicktimeSoundSetVolume(float vol);				// use changes in the master volume and set everyone sound that's playing based on
void ofQuicktimeSoundUpdate();						


class ofQuickTimePlayerManager {
	
	public:
		
		static ofQuickTimePlayerManager& Instance(){
			static ofQuickTimePlayerManager singleton;
			return singleton;
		}
		

	private:
		
		ofQuickTimePlayerManager() {};                                 // Private constructor
		ofQuickTimePlayerManager(const ofQuickTimePlayerManager&);                 // Prevent copy-construction
		ofQuickTimePlayerManager& operator=(const ofQuickTimePlayerManager&);      // Prevent assignment
	
		
};


// --------------------- player functions:
class ofQuicktimeSoundPlayer {

	public:

		ofQuicktimeSoundPlayer();
		virtual ~ofQuicktimeSoundPlayer();

		void loadSound(string fileName, bool stream = false);
		void unloadSound();
		void play();
		void stop();

	
		void initializeQuicktime();
		void closeQuicktime();

		void setVolume(float vol);					//
		void setPan(float vol);						//
		void setSpeed(float spd);					//
		void setPaused(bool bP);					//
		
		void setLoop(bool bLp);
		void setMultiPlay(bool bMp);				
		void setPosition(float pct); // 0 = start, 1 = end;
		
		void tryMultiPlay();

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
	
		
};

