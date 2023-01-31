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

#import "ofxOpenALSoundPlayer.h"
#include "ofUtils.h"
#include "ofFileUtils.h"
#include "ofMath.h"

using std::vector;
using std::cerr;
using std::endl;
using std::string;

namespace{
	bool SoundEngineInitialized = false;

	UInt32	numSounds;
	bool	mp3Loaded;
}

static std::mutex& soundPlayerLock() {
  static std::mutex* m = new std::mutex;
    return *m;
}

vector<ofxOpenALSoundPlayer *> soundPlayers;

//--------------------------------------------------------------

ofxOpenALSoundPlayer::ofxOpenALSoundPlayer() {

	volume = 1.0f;
	pitch = 1.0f;
	pan = 0.0f;
	stopped=true;
	bPaused=false;
	
	myPrimedId=-1;
	
	myId = 0;
	bLoadedOk=false;
	bLoop = false;
	bMultiPlay=false;
	iAmAnMp3=false;
	
	numSounds++;
}

//--------------------------------------------------------------

ofxOpenALSoundPlayer::~ofxOpenALSoundPlayer() { 
	
	unload();
	numSounds--;
	
	soundPlayerLock().lock();
	for(int i=0;i<soundPlayers.size();i++)
	{
		if(soundPlayers[i] == this)
		{
			soundPlayers.erase(soundPlayers.begin()+i);
			break;
		}
	}
	soundPlayerLock().unlock();
	
	if(numSounds==0) {
		closeSoundEngine();
		soundPlayers.clear();
	}
}

//--------------------------------------------------------------

bool ofxOpenALSoundPlayer::load(const of::filesystem::path& filePath, bool stream) {
	
	if(!SoundEngineInitialized) {
		ofxOpenALSoundPlayer::initializeSoundEngine();
	}
	
    auto fileName = filePath.string();
	if( fileName.length()-3 == fileName.rfind("mp3") )
		iAmAnMp3=true;
	
	if(iAmAnMp3) {
		bLoadedOk = loadBackgroundMusic(fileName, false, true);
		setLoop(bLoop);
		isStreaming=true;
	}
	else {
		isStreaming=false;
		myId = 0; //assigned by AL
		
		bLoadedOk=true;
		
		if(SoundEngine_LoadEffect(ofToDataPath(fileName).c_str(), &myId) == noErr) {
			length = SoundEngine_GetEffectLength(myId);
		}
		else {
			cerr<<"faied to load sound "<<fileName<<endl;
			bLoadedOk=false;
		}
		
		soundPlayerLock().lock();
		soundPlayers.push_back(this);
		soundPlayerLock().unlock();
	}
	
	return bLoadedOk;
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::unload() {
	if(bLoadedOk)
	{
		if ( isPlaying() )
			stop();
		
		bLoadedOk=false;
		if(iAmAnMp3)
			unloadAllBackgroundMusic();
		else
			SoundEngine_UnloadEffect(myId);
	}
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::play() {
	
	if ( !bLoadedOk ) 
		return;
	
	if(iAmAnMp3)
		SoundEngine_StartBackgroundMusic();
	else
	{
		if(myPrimedId==-1 || bMultiPlay)
			prime();
		SoundEngine_StartEffect(myPrimedId);
	}
	
	stopped = false;
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::stop() {

	if ( !bLoadedOk )
		return;
	
	if(iAmAnMp3)
		SoundEngine_StopBackgroundMusic(false);
	else
		SoundEngine_StopEffect(myPrimedId);
	
	stopped = true;
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::setVolume(float _vol) {
	if ( !bLoadedOk )
		return;

	volume = _vol;
	
	if(iAmAnMp3)
		SoundEngine_SetBackgroundMusicVolume(volume);
	else
		SoundEngine_SetEffectLevel(myPrimedId, (Float32)volume);
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::setPan(float _pan) {
	
	if ( !bLoadedOk )
		return;
	
	if(iAmAnMp3)
		cerr<<"error, cannot set pan on mp3s in openAL"<<endl;
	else {
		float locX = ofClamp(_pan, -1, 1);
		setLocation(locX, location.y, location.z);
	}
	pan = _pan;
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::setPitch(float _pitch) {
	
	if ( !bLoadedOk ) 
		return;

	if(iAmAnMp3)
		cerr<<"error, cannot set pitch on mp3s in openAL"<<endl;
	else {
		pitch = _pitch;
		SoundEngine_SetEffectPitch(myPrimedId, (Float32)pitch);
	}
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::setPaused(bool bP) {
	
	if ( !bLoadedOk )
		return;
	
	if(iAmAnMp3)
		cerr<<"error, cannot set pause on mp3s in openAL"<<endl; // TODO
	else {
		bool bPlaying = isPlaying();
		bPaused = bP;
		
		if(bPaused && bPlaying)
			SoundEngine_PauseEffect(myPrimedId);
		else if(!bPaused && !bPlaying)
			play();
	}
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::setLoop(bool bLp) {
	if ( !bLoadedOk )
		return;
	
	bLoop = bLp;
	
	if(iAmAnMp3)
		SoundEngine_SetBackgroundMusicLooping(bLoop);
	else
		SoundEngine_SetLooping(bLoop, myPrimedId);
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::setMultiPlay(bool bMp) { 
	if ( !bLoadedOk )
		return;
	
	if(iAmAnMp3)
		cerr<<"error, cannot set multiplay on mp3s in openAL"<<endl;
	else
		bMultiPlay = bMp;
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::setPosition(float pct) {
	if ( !bLoadedOk ) 
		return;

	if(iAmAnMp3)
		cerr<<"error, cannot set position on mp3s in openAL"<<endl;
	else
		SoundEngine_SetEffectPosition(myPrimedId, pct*length);
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::setPositionMS(int ms){
	if ( !bLoadedOk ) 
		return;

	if(iAmAnMp3)
		cerr<<"error, cannot set position on mp3s in openAL"<<endl;
	else
		SoundEngine_SetEffectPosition(myPrimedId, float(ms)/1000.f);
}

//--------------------------------------------------------------

float ofxOpenALSoundPlayer::getPosition() const {
	if ( !bLoadedOk ) 
		return 0;

	if(iAmAnMp3)
	{
		cerr<<"error, cannot get position on mp3s in openAL"<<endl;
	}
	else
		return (float)SoundEngine_GetEffectPosition(myPrimedId)/length;
	
	return 0;
}

//--------------------------------------------------------------

int ofxOpenALSoundPlayer::getPositionMS()  const{
	if ( !bLoadedOk ) 
		return 0;

	if(iAmAnMp3)
	{
		cerr<<"error, cannot get position on mp3s in openAL"<<endl;
	}
	else
		return SoundEngine_GetEffectPosition(myPrimedId)*1000.f;

	return 0;
}

//--------------------------------------------------------------

bool ofxOpenALSoundPlayer::isPlaying() {
	if ( !bLoadedOk ) 
		return false;

	if(iAmAnMp3)
		stopped = SoundEngine_getBackgroundMusicStopped();
	
	if(!stopped) // if not stopped, run the update to see if maybe we should be...
		update();
	
	if(stopped || bPaused)
		return false;
	else
		return true;
}

//--------------------------------------------------------------

float ofxOpenALSoundPlayer::getPitch()  const{
	return pitch;
}

//--------------------------------------------------------------

float ofxOpenALSoundPlayer::getPan()  const{
	return pan;
}

//--------------------------------------------------------------

float ofxOpenALSoundPlayer::getVolume()  const{
    return volume;
}

//--------------------------------------------------------------
bool ofxOpenALSoundPlayer::isLoaded()  const{
    return bLoadedOk;
}

//--------------------------------------------------------------


//static calls ---------------------------------------------------------------------------------------------------------------

void ofxOpenALSoundPlayer::initializeSoundEngine() {
	if(!SoundEngineInitialized){
		
		OSStatus err = SoundEngine_Initialize(44100);
				
		if(err)
			cerr<<"ERROR failed to initialize soundEngine."<<endl;
		else {
			numSounds=1;
			mp3Loaded=false;
			
			SoundEngineInitialized = true;
			OSStatus err = SoundEngine_SetListenerPosition(0.0f, 0.0f, 0.0f);
			if(err)
				cerr<<"ERROR failed to set listener position in init..\n (if you are running in the simulator, this is normal, sounds won't work.)"<<endl;
		}
	}
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::closeSoundEngine(){
	if(SoundEngineInitialized){
		
		SoundEngine_Teardown();

		SoundEngineInitialized = false;
	}
}

//--------------------------------------------------------------

void ofxALSoundStopAll(){
	soundPlayerLock().lock();
	for(int i=0;i<soundPlayers.size();i++)
			soundPlayers[i]->stop();
	soundPlayerLock().unlock();
}

//--------------------------------------------------------------

float * ofxALSoundGetSpectrum(){
	cerr<<"unfortunately there is no way to get the sound spectrum out of openAL"<<endl;
	return NULL;
}

//--------------------------------------------------------------

void ofxALSoundSetVolume(float vol){
	if(!SoundEngineInitialized)
		ofxOpenALSoundPlayer::initializeSoundEngine();
	if ( SoundEngineInitialized )
		SoundEngine_SetMasterVolume((Float32)vol);
}

//--------------------------------------------------------------



// internal ------------------------------------------------------------------------------------------------------------------

bool ofxOpenALSoundPlayer::update() {

	bool deletedAnything= false;
	for(int i=retainedBuffers.size()-1;i>=0;i--) {
		if(SoundEngine_Update(retainedBuffers[i]->primedID, retainedBuffers[i]->buffer)) {
			delete retainedBuffers[i];
			retainedBuffers.erase(retainedBuffers.begin()+i);
			deletedAnything = true;
		}
	}
	
	if(retainedBuffers.size()==0)
	{
		myPrimedId=-1;
		stopped=true;
	}
	
	return deletedAnything;
}

//--------------------------------------------------------------

bool ofxOpenALSoundPlayer::prime() {

	soundPlayerLock().lock();
	for(int i=0;i<soundPlayers.size();i++)
		soundPlayers[i]->update();
	soundPlayerLock().unlock();
	
	multiPlaySource * m;
	m = new multiPlaySource();
	ALuint newPrimedId;
	m->buffer = SoundEngine_PrimeEffect(myId, &newPrimedId);

	if(m->buffer != -1) {
		m->primedID = newPrimedId;
		myPrimedId = newPrimedId;
		retainedBuffers.push_back(m);
		updateInternalsForNewPrime();
		return true;
	}
	else
		delete m;
	
	return false;
}

//--------------------------------------------------------------
bool ofxOpenALSoundPlayer::loadBackgroundMusic(string fileName, bool queue, bool loadAtOnce) {
	myId = 0;
	
	if(!mp3Loaded) {
		if(SoundEngine_LoadBackgroundMusicTrack(ofToDataPath(fileName).c_str(), queue, loadAtOnce) == noErr )
		{
			length = SoundEngine_getBackgroundMusicLength();
			bLoadedOk=true;
			mp3Loaded=true;
			
			soundPlayerLock().lock();
				soundPlayers.push_back(this);
			soundPlayerLock().unlock();
		}
		else
		{
			bLoadedOk=false;
			cerr<<"faied to load sound "<<fileName<<endl;
		}
	}
	else {
		cerr<<"more than one mp3 cannot be loaded at the same time"<<endl;
	}
	
	return bLoadedOk;
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::updateInternalsForNewPrime() {
	setPitch(pitch);
	setLocation(location.x, location.y, location.z);
	setPan(pan);
	setVolume(volume);
	setLoop(bLoop);
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::unloadAllBackgroundMusic() {
	SoundEngine_UnloadBackgroundMusicTrack();
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::startBackgroundMusic() {
	SoundEngine_StartBackgroundMusic();
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::stopBackgroundMusic(bool stopNow) {
	SoundEngine_StopBackgroundMusic(!stopNow); //this is confusing but i think stopNow makes more sense than stopAtEnd in terms of how people will likely be calling this
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::setBackgroundMusicVolume(float bgVol) {
	SoundEngine_SetBackgroundMusicVolume((Float32)bgVol);
}

//--------------------------------------------------------------



// beyond ofSoundPlayer ------------------------------------------------------------------------------------------------------

void ofxOpenALSoundPlayer::vibrate() { 
	SoundEngine_Vibrate();
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::setLocation(float x, float y, float z) { 
	
	if ( !bLoadedOk )
		return;

	if(iAmAnMp3)
		cerr<<"error, cannot set location on mp3s in openAL"<<endl;
	else
	{
		location = {x,y,z};
		pan = ofClamp(x,-1,1); // assuming x clamp pan to -1..1
		SoundEngine_SetEffectLocation(myPrimedId, x, y, z);
	}
}

//--------------------------------------------------------------



// beyond ofSoundPlayer static -----------------------------------------------------------------------------------------------

void ofxOpenALSoundPlayer::ofxALSoundSetListenerLocation(float x, float y, float z){	
	if(!SoundEngineInitialized)
		ofxOpenALSoundPlayer::initializeSoundEngine();
	if ( SoundEngineInitialized )
		SoundEngine_SetListenerPosition(x, y, z);
}

//--------------------------------------------------------------
void ofxOpenALSoundPlayer::ofxALSoundSetListenerVelocity(float x, float y, float z){
	if(!SoundEngineInitialized)
		ofxOpenALSoundPlayer::initializeSoundEngine();
	if ( SoundEngineInitialized )
		SoundEngine_SetListenerVelocity(x, y, z); //deprecated
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::ofxALSoundSetListenerGain(float gain) {
	if(!SoundEngineInitialized)
		ofxOpenALSoundPlayer::initializeSoundEngine();
	if ( SoundEngineInitialized )
		SoundEngine_SetListenerGain(gain);
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::ofxALSoundSetReferenceDistance(float dist) {
	if(!SoundEngineInitialized)
		ofxOpenALSoundPlayer::initializeSoundEngine();
	if ( SoundEngineInitialized )
		SoundEngine_SetReferenceDistance(dist);
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::ofxALSoundSetMaxDistance(float dist) {
	if(!SoundEngineInitialized)
		ofxOpenALSoundPlayer::initializeSoundEngine();
	if ( SoundEngineInitialized )
		SoundEngine_SetMaxDistance(dist);
}

//--------------------------------------------------------------

void ofxOpenALSoundPlayer::ofxALSoundSetDistanceModel(ALenum model) {
	if(!SoundEngineInitialized)
		ofxOpenALSoundPlayer::initializeSoundEngine();
	if ( SoundEngineInitialized )
		alDistanceModel(model);
}

