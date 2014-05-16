#include "ofBaseTypes.h"
#include "ofUtils.h"


//---------------------------------------------------------------------------
ofBaseVideoGrabber::~ofBaseVideoGrabber(){
	
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::setVerbose(bool bTalkToMe){
	ofLogWarning("ofBaseVideoGrabber") << "setVerbose() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::setDeviceID(int _deviceID){
	ofLogWarning("ofBaseVideoGrabber") << "setDeviceID() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::setDesiredFrameRate(int framerate){
	ofLogWarning("ofBaseVideoGrabber") << "setDesiredFrameRate() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::videoSettings(){
	ofLogWarning("ofBaseVideoGrabber") << "videoSettings() not implemented";
}

//---------------------------------------------------------------------------
//void ofBaseVideoGrabber::setPixelFormat(ofPixelFormat pixelFormat){
//	ofLogWarning("ofBaseVideoGrabber") << "setPixelFormat() not implemented";
//}
//
//ofPixelFormat ofBaseVideoGrabber::getPixelFormat(){
//	ofLogWarning("ofBaseVideoGrabber") << "getPixelFormat() not implemented";
//	return OF_PIXELS_RGB;
//}

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
ofBaseVideoPlayer::~ofBaseVideoPlayer(){

}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getPosition(){
	ofLogWarning("ofBaseVideoPlayer") << "getPosition() not implemented";
	return 0.0;
}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getSpeed(){
	ofLogWarning("ofBaseVideoPlayer") << "getSpeed() not implemented";
	return 0.0;
}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getDuration(){
	ofLogWarning("ofBaseVideoPlayer") << "getDuration() not implemented";
	return 0.0;
}

//---------------------------------------------------------------------------
bool ofBaseVideoPlayer::getIsMovieDone(){
	ofLogWarning("ofBaseVideoPlayer") << "getIsMovieDone() not implemented";
	return false;
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setPaused(bool bPause){
	ofLogWarning("ofBaseVideoPlayer") << "setPaused() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setPosition(float pct){
	ofLogWarning("ofBaseVideoPlayer") << "setPosition() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setVolume(float volume){
	ofLogWarning("ofBaseVideoPlayer") << "setVolume() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setLoopState(ofLoopType state){
	ofLogWarning("ofBaseVideoPlayer") << "setLoopState() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setSpeed(float speed){
	ofLogWarning("ofBaseVideoPlayer") << "setSpeed() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setFrame(int frame){
	ofLogWarning("ofBaseVideoPlayer") << "setFrame() not implemented";
}

//---------------------------------------------------------------------------
int	ofBaseVideoPlayer::getCurrentFrame(){
	ofLogWarning("ofBaseVideoPlayer") << "getCurrentFrame() not implemented";
	return 0;
}

//---------------------------------------------------------------------------
int	ofBaseVideoPlayer::getTotalNumFrames(){
	ofLogWarning("ofBaseVideoPlayer") << "getTotalNumFrames() not implemented";
	return 0;
}

//---------------------------------------------------------------------------
ofLoopType ofBaseVideoPlayer::getLoopState(){
	ofLogWarning("ofBaseVideoPlayer") << "getLoopState() not implemented";
	return OF_LOOP_NONE;
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::firstFrame(){
	ofLogWarning("ofBaseVideoPlayer") << "firstFrame() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::nextFrame(){
	ofLogWarning("ofBaseVideoPlayer") << "nextFrame() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::previousFrame(){
	ofLogWarning("ofBaseVideoPlayer") << "previousFrame() not implemented";
}

//---------------------------------------------------------------------------
//void ofBaseVideoPlayer::setPixelFormat(ofPixelFormat pixelFormat){
//	ofLogWarning("ofBaseVideoPlayer") << "setPixelFormat() not implemented";
//}
//---------------------------------------------------------------------------
//ofPixelFormat ofBaseVideoPlayer::getPixelFormat(){
//	ofLogWarning("ofBaseVideoPlayer") << "getPixelFormat() not implemented";
//	return OF_PIXELS_RGB;
//}

//---------------------------------------------------------------------------
ofMatrix4x4 ofBaseRenderer::getCurrentOrientationMatrix() const {
	ofLogWarning() << "getCurrentOrientationMatrix() Not implemented for this renderer. Returning Identity matrix.";
	return ofMatrix4x4();
}
