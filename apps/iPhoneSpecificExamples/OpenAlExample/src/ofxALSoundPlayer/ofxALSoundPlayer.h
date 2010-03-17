


#ifndef OFX_AL_SOUND_PLAYER_H
#define OFX_AL_SOUND_PLAYER_H

/* the iphone needs special sound formats to play. apparently wavs, aifs, mp3s, or cafs will work, but i've only had luck with cafs.
 to convert an audio file to a caf is pretty easy thankfully, just go to the terminal, go to the directory your file is in, and then
 type:
 
 afconvert -f caff -d LEI16 FILEIN FILEOUT
 
 where FILEIN is the name of your file, and FILEOUT is the new name (something.caf)
 */


#include "ofUtils.h"
#include <CoreAudio/CoreAudioTypes.h>

//globals

void ofxALSoundStopAll();
void ofxALSoundSetVolume(float vol);

class ofxALSoundPlayer
{
public:
	
	ofxALSoundPlayer();
	~ofxALSoundPlayer();
	
	void	loadSound(string fileName);
	void	loadSoundFromData(int * data, int dataSize, int dataSampleRate, bool looping);
	void	loadLoopingSound(string fileName); //setLoop no longer exists, this should be used to define a looping sound
	void	loadLoopingSound(string loopingFileName, string attackFileName, string decayFileName);
	void	unloadSound();
	void	play();
	void	stop();
	void	stopWithDecay(); //if you loaded a loop with a decay sound attached you should use this to stop it.
	void	vibrate();

	void	setVolume(float _vol);
	void	setPan(float _pan); // i may not have implemented 
	void	setPitch(float _pitch);
	
	static void	ofxALSoundPlayerSetListenerLocation(float x, float y, float z);
	static void	ofxALSoundPlayerSetListenerGain(float gain);
	static void	ofxALSoundPlayerSetListenerPitch(float pitch);
	
	static void	ofxALSoundPlayerSetListenerVelocity(float x, float y, float z);
	
	static void	ofxALSoundPlayerSetReferenceDistance(float dist); // sets the distance after which attenuation is applied
	static void	ofxALSoundPlayerSetMaxDistance(float dist); // sets the maximum distance for which attenuation is applied
	
	void	setLocation(float x, float y, float z);
	

	float	getPosition();
	float	getPitch();
	float	getPan();
	
	static void initializeSoundEngine();				
	static void closeSoundEngine();	
	UInt32 ofxALSoundGetNextID();


	//bool	multiPlay;
	float	pan;
	float	pitch;
	float	volume;

	UInt32	myId;
	
	bool	stopped;
	
	
	void	loadBackgroundMusic(string fileName, bool queue, bool loadAtOnce);
	void	unloadAllBackgroundMusic();
	void	startBackgroundMusic();
	void	stopBackgroundMusic(bool stopNow);
	void	setBackgroundMusicVolume(float bgVol);
	
};

#endif