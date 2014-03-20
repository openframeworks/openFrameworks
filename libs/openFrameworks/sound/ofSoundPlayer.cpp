#include "ofSoundPlayer.h"
#include "ofUtils.h"

// these are global functions, that affect every sound / channel:
// ------------------------------------------------------------
// ------------------------------------------------------------

//--------------------
void ofSoundStopAll(){
	#ifdef OF_SOUND_PLAYER_FMOD
		ofFmodSoundStopAll();
	#endif
}

//--------------------
void ofSoundSetVolume(float vol){
	#ifdef OF_SOUND_PLAYER_FMOD
		ofFmodSoundSetVolume(vol);
	#endif
}

//--------------------
void ofSoundUpdate(){
	#ifdef OF_SOUND_PLAYER_FMOD
		ofFmodSoundUpdate();
	#endif
}

#if !defined(TARGET_ANDROID) && !defined(TARGET_LINUX_ARM)
//--------------------
void ofSoundShutdown(){
	#ifdef OF_SOUND_PLAYER_FMOD
		ofFmodSoundPlayer::closeFmod();
	#endif
}
#endif

//--------------------
float * ofSoundGetSpectrum(int nBands){
	#ifdef OF_SOUND_PLAYER_FMOD
		return ofFmodSoundGetSpectrum(nBands);
	#elif defined(OF_SOUND_PLAYER_OPENAL)
		return ofOpenALSoundPlayer::getSystemSpectrum(nBands);
	#else
		ofLogError("ofSoundPlayer") << "ofSoundGetSpectrum(): not implemented, returning NULL";
		return NULL;
	#endif
}



#include "ofSoundPlayer.h"
//---------------------------------------------------------------------------
ofSoundPlayer::ofSoundPlayer (){
	player	= ofPtr<OF_SOUND_PLAYER_TYPE>(new OF_SOUND_PLAYER_TYPE);
}

//---------------------------------------------------------------------------
void ofSoundPlayer::setPlayer(ofPtr<ofBaseSoundPlayer> newPlayer){
	player = newPlayer;
}

//--------------------------------------------------------------------
ofPtr<ofBaseSoundPlayer> ofSoundPlayer::getPlayer(){
	return player;
}

//--------------------------------------------------------------------
bool ofSoundPlayer::loadSound(string fileName, bool stream){
	if( player != NULL ){
		return player->loadSound(fileName, stream);
	}
	return false;
}

//--------------------------------------------------------------------
void ofSoundPlayer::unloadSound(){
	if( player != NULL ){
		player->unloadSound();
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::play(){
	if( player != NULL ){
		player->play();
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::stop(){
	if( player != NULL ){
		player->stop();
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setVolume(float vol){
	if( player != NULL ){
		player->setVolume(vol);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setPan(float pan){
	if( player != NULL ){
		player->setPan(CLAMP(pan,-1.0f,1.0f));
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setSpeed(float spd){
	if( player != NULL ){
		player->setSpeed(spd);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setPaused(bool bP){
	if( player != NULL ){
		player->setPaused(bP);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setLoop(bool bLp){
	if( player != NULL ){
		player->setLoop(bLp);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setMultiPlay(bool bMp){
	if( player != NULL ){
		player->setMultiPlay(bMp);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setPosition(float pct){
	if( player != NULL ){
		player->setPosition(pct);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::setPositionMS(int ms){
	if( player != NULL ){
		player->setPositionMS(ms);
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer::getPosition(){
	if( player != NULL ){
		return player->getPosition();
	} else {
		return 0;
	}
}

//--------------------------------------------------------------------
int ofSoundPlayer::getPositionMS(){
	if( player != NULL ){
		return player->getPositionMS();
	} else {
		return 0;
	}
}

//--------------------------------------------------------------------
bool ofSoundPlayer::getIsPlaying(){
	if( player != NULL ){
		return player->getIsPlaying();
	} else {
		return false;
	}
}

//--------------------------------------------------------------------
bool ofSoundPlayer::isLoaded(){
	if( player != NULL ){
		return player->isLoaded();
	} else {
		return false; 
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer::getSpeed(){
	if( player != NULL ){
		return player->getSpeed();
	} else {
		return 0;
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer::getPan(){
	if( player != NULL ){
		return player->getPan();
	} else {
		return 0;
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer::getVolume(){
	if( player != NULL ){
		return player->getVolume();
	} else {
		return 0;
	}
}
