#include "ofBaseTypes.h"
#include "ofUtils.h"


//---------------------------------------------------------------------------
ofBaseVideoGrabber::~ofBaseVideoGrabber(){
	
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::setVerbose(bool bTalkToMe){
	ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::setVerbose not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::setDeviceID(int _deviceID){
	ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::setDeviceID not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::setDesiredFrameRate(int framerate){
	ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::setDesiredFrameRate not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::videoSettings(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::videoSettings not implemented");
}

//---------------------------------------------------------------------------
//void ofBaseVideoGrabber::setPixelFormat(ofPixelFormat pixelFormat){
//	ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::setPixelFormat not implemented");
//}
//
//ofPixelFormat ofBaseVideoGrabber::getPixelFormat(){
//	ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::getPixelFormat not implemented");
//	return OF_PIXELS_RGB;
//}

//---------------------------------------------------------------------------


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
void ofBaseVideoPlayer::setVolume(float volume){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::setVolume not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setLoopState(ofLoopType state){
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
	return 0;
}

//---------------------------------------------------------------------------
int	ofBaseVideoPlayer::getTotalNumFrames(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::getTotalNumFrames not implemented");
	return 0;
}

//---------------------------------------------------------------------------
ofLoopType	ofBaseVideoPlayer::getLoopState(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::getLoopState not implemented");
	return OF_LOOP_NONE;
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

//---------------------------------------------------------------------------
//void ofBaseVideoPlayer::setPixelFormat(ofPixelFormat pixelFormat){
//	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::setPixelFormat not implemented");
//}
//---------------------------------------------------------------------------
//ofPixelFormat ofBaseVideoPlayer::getPixelFormat(){
//	ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer::getPixelFormat not implemented");
//	return OF_PIXELS_RGB;
//}
