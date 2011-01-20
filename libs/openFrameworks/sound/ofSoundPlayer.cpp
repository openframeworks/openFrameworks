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

//--------------------
void ofSoundShutdown(){
	#ifdef OF_SOUND_PLAYER_FMOD
		ofFmodSoundPlayer::closeFmod();
	#endif
}


//--------------------
float * ofSoundGetSpectrum(int nBands){
	#ifdef OF_SOUND_PLAYER_FMOD
		return ofFmodSoundGetSpectrum(nBands);
	#else
		ofLog(OF_LOG_ERROR, "ofSoundGetSpectrum returning NULL - no implementation!");
		return NULL;
	#endif
}



#include "ofSoundPlayer.h"
#include "ofMain.h" // for ofLog?
//---------------------------------------------------------------------------
ofSoundPlayer::ofSoundPlayer (){
	player		= new OF_SOUND_PLAYER_TYPE;
}

//---------------------------------------------------------------------------
ofSoundPlayer::~ofSoundPlayer(){
	if( player != NULL ){
		delete player;
		player = NULL;
	}
}

//---------------------------------------------------------------------------
bool ofSoundPlayer::setPlayer(ofBaseSoundPlayer * newPlayer){
	if( player == NULL ){
		player = newPlayer;
	}else{
		delete player;
		player = newPlayer;
	}
	return true;
}

//--------------------------------------------------------------------
ofBaseSoundPlayer * ofSoundPlayer::getPlayer(){
	return player;
}

//--------------------------------------------------------------------
void ofSoundPlayer::loadSound(string fileName, bool stream){
	if( player != NULL ){
		player->loadSound(fileName, stream);
	}
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
		player->setPan(pan);
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
float ofSoundPlayer::getPosition(){
	if( player != NULL ){
		return player->getPosition();
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