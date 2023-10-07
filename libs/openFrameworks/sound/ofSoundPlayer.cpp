#include "ofSoundPlayer.h"
#include "ofLog.h"
#define GLM_FORCE_CTOR_INIT
#include "glm/common.hpp"


#ifdef OF_SOUND_PLAYER_AV_ENGINE
#include "ofAVEngineSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofAVEngineSoundPlayer
#endif

#ifdef OF_SOUND_PLAYER_QUICKTIME
#include "ofQuicktimeSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofQuicktimeSoundPlayer
#endif

#ifdef OF_SOUND_PLAYER_FMOD
#include "ofFmodSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofFmodSoundPlayer
#endif

#ifdef OF_SOUND_PLAYER_MEDIA_FOUNDATION
#include "ofMediaFoundationSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofMediaFoundationSoundPlayer
#endif

#ifdef OF_SOUND_PLAYER_OPENAL
#include "ofOpenALSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofOpenALSoundPlayer
#endif

#ifdef TARGET_OF_IOS
#include "ofxiOSSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofxiOSSoundPlayer
#endif

#ifdef OF_SOUND_PLAYER_EMSCRIPTEN
#include "ofxEmscriptenSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofxEmscriptenSoundPlayer
#endif

#ifdef TARGET_ANDROID
#include "ofxAndroidSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofxAndroidSoundPlayer
#endif


// these are global functions, that affect every sound / channel:
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
	#elif defined(OF_SOUND_PLAYER_MEDIA_FOUNDATION)
		ofMediaFoundationSoundPlayer::SetMasterVolume(vol);
	#else
		ofLogWarning("ofSoundPlayer") << "ofSoundSetVolume() not implemented on this platform";
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
		ofLogWarning("ofSoundPlayer") << "ofSoundGetSpectrum() not implemented on this platform, returning nullptr";
		return nullptr;
	#endif
}

//---------------------------------------------------------------------------
ofSoundPlayer::ofSoundPlayer (){
#ifdef OF_SOUND_PLAYER_TYPE
	player	= std::make_shared<OF_SOUND_PLAYER_TYPE>();
#endif
}

//---------------------------------------------------------------------------
void ofSoundPlayer::setPlayer(std::shared_ptr<ofBaseSoundPlayer> newPlayer){
	player = std::move(newPlayer);
}

//--------------------------------------------------------------------
std::shared_ptr<ofBaseSoundPlayer> ofSoundPlayer::getPlayer(){
	return player;
}

//--------------------------------------------------------------------
bool ofSoundPlayer::load(const of::filesystem::path& fileName, bool stream){
	if( player ){
		return player->load(fileName, stream);
	}
	return false;
}

//--------------------------------------------------------------------
bool ofSoundPlayer::loadSound(std::string fileName, bool stream){
	return load(fileName,stream);
}

//--------------------------------------------------------------------
void ofSoundPlayer::unload(){
	if( player ){
		player->unload();
	}
}

//--------------------------------------------------------------------
void ofSoundPlayer::unloadSound(){
	unload();
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
		player->setPan(glm::clamp(pan,-1.0f,1.0f));
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
float ofSoundPlayer::getPosition() const{
	if( player ){
		return player->getPosition();
	} else {
		return 0;
	}
}

//--------------------------------------------------------------------
int ofSoundPlayer::getPositionMS() const{
	if( player ){
		return player->getPositionMS();
	} else {
		return 0;
	}
}

//--------------------------------------------------------------------
bool ofSoundPlayer::isPlaying() const{
	if( player ){
		return player->isPlaying();
	} else {
		return false;
	}
}

//--------------------------------------------------------------------
bool ofSoundPlayer::getIsPlaying() const{
	return isPlaying();
}

//--------------------------------------------------------------------
bool ofSoundPlayer::isLoaded() const{
	if( player ){
		return player->isLoaded();
	} else {
		return false; 
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer::getSpeed() const{
	if( player ){
		return player->getSpeed();
	} else {
		return 0;
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer::getPan() const{
	if( player ){
		return player->getPan();
	} else {
		return 0;
	}
}

//--------------------------------------------------------------------
float ofSoundPlayer::getVolume() const{
	if( player ){
		return player->getVolume();
	} else {
		return 0;
	}
}
