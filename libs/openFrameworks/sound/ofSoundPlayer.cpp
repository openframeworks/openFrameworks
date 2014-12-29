#include "ofSoundPlayer.h"
#include "ofUtils.h"

// these are global functions, that affect every sound / channel:
// ------------------------------------------------------------
// ------------------------------------------------------------

//--------------------
void ofSoundStopAll(){
	#ifdef OF_SOUND_PLAYER_FMOD
		ofFmodSoundStopAll();
	#else
		ofLogWarning("ofSoundPlayer") << "ofSoundStopAll() not implemented on this platform";
	#endif
}

//--------------------
void ofSoundSetVolume(float vol){
	#ifdef OF_SOUND_PLAYER_FMOD
		ofFmodSoundSetVolume(vol);
	#else
		ofLogWarning("ofSoundPlayer") << "ofSoundSetVolume() not implemented on this platform";
	#endif
}

//--------------------
void ofSoundUpdate(){
	#ifdef OF_SOUND_PLAYER_FMOD
		ofFmodSoundUpdate();
	#else
		ofLogWarning("ofSoundPlayer") << "ofSoundUpdate() not implemented on this platform";
	#endif
}

#if !defined(TARGET_ANDROID) && !defined(TARGET_LINUX_ARM)
//--------------------
void ofSoundShutdown(){
	#ifdef OF_SOUND_PLAYER_FMOD
		ofFmodSoundPlayer::closeFmod();
	#endif
	// ofSoundShutdown doesn't log an "unimplemented" message like the related functions
	// above, because it's called by the openFrameworks shutdown routine regardless
}
#endif

//--------------------
float * ofSoundGetSpectrum(int nBands){
	#ifdef OF_SOUND_PLAYER_FMOD
		return ofFmodSoundGetSpectrum(nBands);
	#elif defined(OF_SOUND_PLAYER_OPENAL)
		return ofOpenALSoundPlayer::getSystemSpectrum(nBands);
	#elif defined(OF_SOUND_PLAYER_EMSCRIPTEN)
		return ofxEmscriptenSoundPlayer::getSystemSpectrum(nBands);
	#else
		ofLogWarning("ofSoundPlayer") << "ofSoundGetSpectrum() not implemented on this platform, returning NULL";
		return NULL;
	#endif
}

#include "ofSoundPlayer.h"
//---------------------------------------------------------------------------
ofSoundPlayer::ofSoundPlayer (){
	player	= shared_ptr<OF_SOUND_PLAYER_TYPE>(new OF_SOUND_PLAYER_TYPE);
}

//---------------------------------------------------------------------------
void ofSoundPlayer::setPlayer(shared_ptr<ofBaseSoundPlayer> newPlayer){
	player = newPlayer;
}

//--------------------------------------------------------------------
shared_ptr<ofBaseSoundPlayer> ofSoundPlayer::getPlayer(){
	return player;
}

//--------------------------------------------------------------------
bool ofSoundPlayer::loadSound(string fileName, bool stream){
	if( player ){
		return player->loadSound(fileName, stream);
	}
	return false;
}

//--------------------------------------------------------------------
void ofSoundPlayer::unloadSound(){
	if( player ){
		player->unloadSound();
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::play(){
	if( player ){
		player->play();
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::stop(){
	if( player ){
		player->stop();
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setVolume(float vol){
	if( player ){
		player->setVolume(vol);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setPan(float pan){
	if( player ){
		player->setPan(CLAMP(pan,-1.0f,1.0f));
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setSpeed(float spd){
	if( player ){
		player->setSpeed(spd);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setPaused(bool bP){
	if( player ){
		player->setPaused(bP);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setLoop(bool bLp){
	if( player ){
		player->setLoop(bLp);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setMultiPlay(bool bMp){
	if( player ){
		player->setMultiPlay(bMp);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setPosition(float pct){
	if( player ){
		player->setPosition(pct);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setPositionMS(int ms){
	if( player ){
		player->setPositionMS(ms);
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer::getPosition(){
	if( player ){
		return player->getPosition();
	} else {
		return 0;
	}
}

//--------------------------------------------------------------------
int ofSoundPlayer::getPositionMS(){
	if( player ){
		return player->getPositionMS();
	} else {
		return 0;
	}
}

//--------------------------------------------------------------------
bool ofSoundPlayer::getIsPlaying(){
	if( player ){
		return player->getIsPlaying();
	} else {
		return false;
	}
}

//--------------------------------------------------------------------
bool ofSoundPlayer::isLoaded(){
	if( player ){
		return player->isLoaded();
	} else {
		return false; 
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer::getSpeed(){
	if( player ){
		return player->getSpeed();
	} else {
		return 0;
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer::getPan(){
	if( player ){
		return player->getPan();
	} else {
		return 0;
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer::getVolume(){
	if( player ){
		return player->getVolume();
	} else {
		return 0;
	}
}
