#include "ofBaseVideoPlayer.h"
#include "ofUtils.h"

//---------------------------------------------------------------------------
ofBaseVideoPlayer::ofBaseVideoPlayer (){

}

//---------------------------------------------------------------------------
ofBaseVideoPlayer::~ofBaseVideoPlayer(){

}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getPosition(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::getPosition not implemented");
	return 0.0;
}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getSpeed(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::getSpeed not implemented");
	return 0.0;
}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getDuration(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::getDuration not implemented");
	return 0.0;
}

//---------------------------------------------------------------------------
bool ofBaseVideoPlayer::getIsMovieDone(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::getIsMovieDone not implemented");
	return false;
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setPaused(bool bPause){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::setPaused not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setPosition(float pct){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::setPosition not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setVolume(int volume){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::setVolume not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setLoopState(int state){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::setLoopState not implemented");
}

//---------------------------------------------------------------------------
void   ofBaseVideoPlayer::setSpeed(float speed){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::setSpeed not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setFrame(int frame){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::setFrame not implemented");
}

//---------------------------------------------------------------------------
int	ofBaseVideoPlayer::getCurrentFrame(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::nextFrame not implemented");
}

//---------------------------------------------------------------------------
int	ofBaseVideoPlayer::getTotalNumFrames(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::getTotalNumFrames not implemented");
	return 0;
}

//---------------------------------------------------------------------------
int	ofBaseVideoPlayer::getLoopState(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::getLoopState not implemented");
	return 0;
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::firstFrame(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::firstFrame not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::nextFrame(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::nextFrame not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::previousFrame(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::previousFrame not implemented");
}

