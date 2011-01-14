#include "ofSoundPlayer_.h"
#include "ofMain.h" // for ofLog?
//---------------------------------------------------------------------------
ofSoundPlayer_::ofSoundPlayer_ (){
	player		= NULL;
}

//---------------------------------------------------------------------------
ofSoundPlayer_::~ofSoundPlayer_(){
	if( player != NULL ){
		delete player;
		player = NULL;
	}
}

//---------------------------------------------------------------------------
bool ofSoundPlayer_::setPlayer(ofBaseSoundPlayer * newPlayer){
	if( player == NULL ){
		player = newPlayer;
		return true;
	}else{
		//TODO: should we delete newPlayer? This is why we need smart pointers. 
		ofLog(OF_LOG_ERROR, "ofSoundPlayer::setPlayer - player already set!");
	}
	return false;
}

//--------------------------------------------------------------------
ofBaseSoundPlayer * ofSoundPlayer_::getPlayer(){
	return player;
}

//--------------------------------------------------------------------
void ofSoundPlayer_::loadSound(string fileName, bool stream){
	
	if( player == NULL ){
		setPlayer( new OF_SOUND_PLAYER_TYPE );
	}
		
	if( player != NULL ){
		player->loadSound(fileName, stream);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer_::unloadSound(){
	if( player != NULL ){
		player->unloadSound();
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer_::play(){
	if( player != NULL ){
		player->play();
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer_::stop(){
	if( player != NULL ){
		player->stop();
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer_::setVolume(float vol){
	if( player != NULL ){
		player->setVolume(vol);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer_::setPan(float pan){
	if( player != NULL ){
		player->setPan(pan);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer_::setSpeed(float spd){
	if( player != NULL ){
		player->setSpeed(spd);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer_::setPaused(bool bP){
	if( player != NULL ){
		player->setPaused(bP);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer_::setLoop(bool bLp){
	if( player != NULL ){
		player->setLoop(bLp);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer_::setMultiPlay(bool bMp){
	if( player != NULL ){
		player->setMultiPlay(bMp);
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer_::setPosition(float pct){
	if( player != NULL ){
		player->setPosition(pct);
	}
} 

//--------------------------------------------------------------------
float ofSoundPlayer_::getPosition(){
	if( player != NULL ){
		return player->getPosition();
	} else {
		return 0; 
	}
}

//--------------------------------------------------------------------
bool ofSoundPlayer_::getIsPlaying(){
	if( player != NULL ){
		return player->getIsPlaying();
	} else {
		return false; 
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer_::getSpeed(){
	if( player != NULL ){
		return player->getSpeed();
	} else {
		return 0; 
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer_::getPan(){
	if( player != NULL ){
		return player->getPan();
	} else {
		return 0; 
	}
}