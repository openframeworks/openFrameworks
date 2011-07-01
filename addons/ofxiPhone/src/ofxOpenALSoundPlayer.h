/***********************************************************************
 
 Copyright (C) 2011 by Zach Gage
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ************************************************************************/ 


#ifndef OFX_OPEN_AL_SOUND_PLAYER_H
#define OFX_OPEN_AL_SOUND_PLAYER_H

/* convert your sounds to caf for them to work best:
 
 afconvert -f caff -d LEI16 FILEIN
 
 where FILEIN is the name of your file.
 
 mp3's can be played but only load and play one at a time.
 
 */


#include "SoundEngine.h"
#include <CoreAudio/CoreAudioTypes.h>
#include "ofBaseSoundPlayer.h"
#include "ofUtils.h"
#include "ofPoint.h"
#include "ofTypes.h"

//globals

void ofxALSoundStopAll();
void ofxALSoundSetVolume(float vol);
float * ofxALSoundGetSpectrum(int nBands);

struct multiPlaySource {
	ALuint primedID;
	int buffer;
};

class ofxOpenALSoundPlayer : public ofBaseSoundPlayer{
public:
	
	static void initializeSoundEngine();				
	static void closeSoundEngine();	
	
	ofxOpenALSoundPlayer();
	~ofxOpenALSoundPlayer();
	
	void	loadSound(string fileName, bool stream=false);
	void	unloadSound();

	void	play();
	void	stop();
	void	setVolume(float _vol);
	void	setPan(float _pan);
	void	setPitch(float _pitch);
	void	setSpeed(float _speed){setPitch(_speed);};// same as pitch. mapped for ofSoundPlayer compatibility
	
	void	setPaused(bool bP);
	void	setLoop(bool bLp);
	void	setMultiPlay(bool bMp);
	void	setPosition(float pct);
	float	getPosition();
	bool	getIsPlaying();
	float	getPitch();
	float	getSpeed(){return getPitch();}; // same as pitch. mapped for ofSoundPlayer compatibility
	
	float	getPan();
	
	// IPHONE EXTRA FUNCTIONS
	static void	vibrate();
	
	static void	ofxALSoundSetListenerLocation(float x, float y, float z);
	static void	ofxALSoundSetListenerGain(float gain);
	static void	ofxALSoundSetListenerVelocity(float x, float y, float z);
	
	static void	ofxALSoundSetReferenceDistance(float dist); // sets the distance after which attenuation is applied
	static void	ofxALSoundSetMaxDistance(float dist); // sets the maximum distance for which attenuation is applied
	
	void	setLocation(float x, float y, float z);
	
	bool	update(); // can this be called at a different time? maybe should be a static function

	bool isStreaming; //always false for openAL
	bool bMultiPlay;
	bool bLoop;
	bool bLoadedOk;
	
	bool bPaused;

	float	pan;
	float	pitch;
	float	volume;
	unsigned int length;
	ofPoint location;
	
protected: //internal
	
	bool    prime();
	void	updateInternalsForNewPrime();
	void	loadBackgroundMusic(string fileName, bool queue, bool loadAtOnce);
	void	unloadAllBackgroundMusic();
	void	startBackgroundMusic();
	void	stopBackgroundMusic(bool stopNow);
	void	setBackgroundMusicVolume(float bgVol);
	
	UInt32 myId;
	ALuint  myPrimedId;
	bool	stopped; 	
	bool	iAmAnMp3;
	vector <multiPlaySource *> retainedBuffers;
};

#endif