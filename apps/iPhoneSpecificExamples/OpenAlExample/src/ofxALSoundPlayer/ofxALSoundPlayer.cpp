/*
 *  ofxALSoundPlayer.cpp
 *  Mobile Frameworks
 *
 *  Created by Zach Gage on 9/6/08.
 *  Copyright 2008 stfj. All rights reserved.
 *
 */

#import "ofxALSoundPlayer.h"

#import "SoundEngine.h"

bool SoundEngineInitialized = false;

UInt32	numSounds;
UInt32  soundIDs;

ofxALSoundPlayer::ofxALSoundPlayer()
{
	volume = 1.0f;
	pitch = 0.0f;
	pan = 1.0f;
	stopped=true;
	
	
	ofxALSoundPlayer::initializeSoundEngine();
	
}

ofxALSoundPlayer::~ofxALSoundPlayer()
{
	numSounds--;
	if(numSounds <= 0)
		closeSoundEngine();
}

//--------------------

void ofxALSoundPlayer::loadSound(string fileName)
{
	if(!SoundEngineInitialized)
	{
		ofxALSoundPlayer::initializeSoundEngine();
	}
	
	myId = ofxALSoundGetNextID(); //get the next ID to assosciate to a sound... 
	/*right now it wont remove old ID's, so once an ID is used its gone.. this should be fixed maybe?
	it wont really effect anything, its just used as a reference name for the sound, so as long as people
	dont load and unload thousands of sounds it should be fine...*/
	
	SoundEngine_LoadEffect(ofToDataPath(fileName).c_str(), &myId);

}

//--------------------

void ofxALSoundPlayer::loadSoundFromData(int * data, int dataSize, int dataSampleRate, bool looping)
{
	if(!SoundEngineInitialized)
	{
		ofxALSoundPlayer::initializeSoundEngine();
	}
	
	myId = ofxALSoundGetNextID(); //get the next ID to assosciate to a sound... 
	/*right now it wont remove old ID's, so once an ID is used its gone.. this should be fixed maybe?
	 it wont really effect anything, its just used as a reference name for the sound, so as long as people
	 dont load and unload thousands of sounds it should be fine...*/
	
	SoundEngine_LoadEffectFromData(data, dataSize, dataSampleRate, looping, &myId);
	
}

//--------------------

void ofxALSoundPlayer::loadLoopingSound(string fileName)
{
	if(!SoundEngineInitialized)
	{
		ofxALSoundPlayer::initializeSoundEngine();
	}
	
	myId = ofxALSoundGetNextID(); //get the next ID to assosciate to a sound... 
	/*right now it wont remove old ID's, so once an ID is used its gone.. this should be fixed maybe?
	 it wont really effect anything, its just used as a reference name for the sound, so as long as people
	 dont load and unload thousands of sounds it should be fine...*/
	
	SoundEngine_LoadLoopingEffect(ofToDataPath(fileName).c_str(), NULL, NULL, &myId);
}

//--------------------

void ofxALSoundPlayer::loadLoopingSound(string loopingFileName, string attackFileName, string decayFileName)
{
	if(!SoundEngineInitialized)
	{
		ofxALSoundPlayer::initializeSoundEngine();
	}
	
	myId = ofxALSoundGetNextID(); //get the next ID to assosciate to a sound... 
	/*right now it wont remove old ID's, so once an ID is used its gone.. this should be fixed maybe?
	 it wont really effect anything, its just used as a reference name for the sound, so as long as people
	 dont load and unload thousands of sounds it should be fine...*/
	
	SoundEngine_LoadLoopingEffect(ofToDataPath(loopingFileName).c_str(), ofToDataPath(attackFileName).c_str(), ofToDataPath(decayFileName).c_str(), &myId);
}

//--------------------

void ofxALSoundPlayer::unloadSound()
{
	SoundEngine_UnloadEffect(myId);
}

//--------------------

void ofxALSoundPlayer::play()
{
	SoundEngine_StartEffect(myId);
	stopped = false;
}

//--------------------

void ofxALSoundPlayer::stop()
{
	SoundEngine_StopEffect(myId, false);
	stopped = true;
}

//--------------------

void ofxALSoundPlayer::stopWithDecay()
{
	SoundEngine_StopEffect(myId, true);
	stopped = true;
}

//--------------------

void ofxALSoundPlayer::vibrate()
{
	SoundEngine_Vibrate();
}

//--------------------

void ofxALSoundPlayer::setVolume(float _vol)
{
	volume = _vol;
	SoundEngine_SetEffectLevel(myId, (Float32)volume);
}

//--------------------

void ofxALSoundPlayer::setPan(float _pan) // this is totally legacy and shouldn't really be used. SetLocation is much better
{
	//also this could be confusing. In oF pan is between 0 and 1.0 with .5 as the center. in ofxAL it is between -1.0 and 1.0.
	//i compensate for it here and in getPan, but im not sure if thats the best strategy. I guess it's ok as long as you don't
	//use setPan and setLocation in the same program
	_pan = _pan*2 - 1.0f;
	setLocation(_pan, 0, 0);
}

//--------------------

void ofxALSoundPlayer::setPitch(float _pitch)
{
	pitch = _pitch;
	SoundEngine_SetEffectPitch(myId, (Float32)pitch);
}

//--------------------
/*
void ofxALSoundPlayer::setMultiPlay(bool _multiPlay) //within the SoundEngine class I couldn't find a way to do this...
{
	//to do
}

//--------------------

void ofxALSoundPlayer::setPosition(float pct)
{
	//to do
}
*/
//--------------------

void ofxALSoundPlayer::setLocation(float x, float y, float z)
{
	pan = (x+1.0)/2; // this is where im compensating for the scale of oF... (pan in oF is 0-1.0, in ofxAL it's -1.0 to 1.0)
	SoundEngine_SetEffectPosition(myId, x, y, z);
}


//---------------------------------------
// this should only be called once
void ofxALSoundPlayer::initializeSoundEngine(){
	if(!SoundEngineInitialized){
		
		OSStatus err = SoundEngine_Initialize(44100);
		
		//SoundEngine_SetMasterVolume(0.2f);
		
		if(err)
		{
			printf("ERROR failed to initialize soundEngine.");
		}
		else
		{
			printf("SoundEngine initialized \n");
			numSounds=0;
			soundIDs=0;
			SoundEngineInitialized = true;
			OSStatus err = SoundEngine_SetListenerPosition(0.0f, 0.0f, 0.0f);
			if(err)
			{
				printf("ERROR failed to set listener position..\n (if you are running in the simulator, this is normal, sounds won't work.) \n");
			}
		}
	}
}


//---------------------------------------
void ofxALSoundPlayer::closeSoundEngine(){
	if(SoundEngineInitialized){
		
		SoundEngine_Teardown();
		
		SoundEngineInitialized = false;
	}
}

float ofxALSoundPlayer::getPitch()
{
	return pitch;
}

//---------------------------------------

float ofxALSoundPlayer::getPan()
{
	return pan;
}

//---------------------------------------
void ofxALSoundStopAll(){
	ofxALSoundPlayer::initializeSoundEngine();
	
	//to do stop all sounds
}

//--------------------
void ofxALSoundSetVolume(float vol){
	ofxALSoundPlayer::initializeSoundEngine();
	
	SoundEngine_SetMasterVolume((Float32)vol);
}

//--------------------
void ofxALSoundPlayer::ofxALSoundPlayerSetListenerLocation(float x, float y, float z){
	ofxALSoundPlayer::initializeSoundEngine();
	
	SoundEngine_SetListenerPosition(x, y, z);
}

//--------------------
void ofxALSoundPlayer::ofxALSoundPlayerSetListenerVelocity(float x, float y, float z){
	ofxALSoundPlayer::initializeSoundEngine();
	
	SoundEngine_SetListenerVelocity(x, y, z);
}

//--------------------

void ofxALSoundPlayer::ofxALSoundPlayerSetListenerGain(float gain)
{
	SoundEngine_SetListenerGain(gain);
}

//--------------------

void ofxALSoundPlayer::ofxALSoundPlayerSetListenerPitch(float pitch)
{
	//SoundEngine_SetListenerPitch(pitch);
}

//--------------------

void ofxALSoundPlayer::ofxALSoundPlayerSetReferenceDistance(float dist)
{
	SoundEngine_SetReferenceDistance(dist);
}

//--------------------

void ofxALSoundPlayer::ofxALSoundPlayerSetMaxDistance(float dist)
{
	SoundEngine_SetMaxDistance(dist);
}

//--------------------

UInt32 ofxALSoundPlayer::ofxALSoundGetNextID()
{
	numSounds++;
	return soundIDs++;
}

//--------------------

void ofxALSoundPlayer::loadBackgroundMusic(string fileName, bool queue, bool loadAtOnce)
{
	
	if(!SoundEngineInitialized)
	{
		ofxALSoundPlayer::initializeSoundEngine();
	}
	
	myId = ofxALSoundGetNextID(); //get the next ID to assosciate to a sound... 
	/*right now it wont remove old ID's, so once an ID is used its gone.. this should be fixed maybe?
	 it wont really effect anything, its just used as a reference name for the sound, so as long as people
	 dont load and unload thousands of sounds it should be fine...*/
	
	SoundEngine_LoadBackgroundMusicTrack(ofToDataPath(fileName).c_str(), queue, loadAtOnce);

	
		
}

//--------------------

void ofxALSoundPlayer::unloadAllBackgroundMusic()
{
	SoundEngine_UnloadBackgroundMusicTrack();
}

//--------------------

void ofxALSoundPlayer::startBackgroundMusic()
{
	SoundEngine_StartBackgroundMusic();
}

//--------------------

void ofxALSoundPlayer::stopBackgroundMusic(bool stopNow)
{
	SoundEngine_StopBackgroundMusic(!stopNow); //this is confusing but i think stopNow makes more sense than stopAtEnd in terms of how people will likely be calling this
}

//--------------------

void ofxALSoundPlayer::setBackgroundMusicVolume(float bgVol)
{
	SoundEngine_SetBackgroundMusicVolume((Float32)bgVol);
}