#include "ofBaseTypes.h"
#include "ofUtils.h"
#include "ofGLUtils.h"
#include "ofGLProgrammableRenderer.h"


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
float ofBaseVideoPlayer::getPosition() const {
	ofLogWarning("ofBaseVideoPlayer") << "getPosition() not implemented";
	return 0.0;
}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getSpeed() const {
	ofLogWarning("ofBaseVideoPlayer") << "getSpeed() not implemented";
	return 0.0;
}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getDuration() const {
	ofLogWarning("ofBaseVideoPlayer") << "getDuration() not implemented";
	return 0.0;
}

//---------------------------------------------------------------------------
bool ofBaseVideoPlayer::getIsMovieDone() const {
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
int	ofBaseVideoPlayer::getCurrentFrame() const {
	ofLogWarning("ofBaseVideoPlayer") << "getCurrentFrame() not implemented";
	return 0;
}

//---------------------------------------------------------------------------
int	ofBaseVideoPlayer::getTotalNumFrames() const {
	ofLogWarning("ofBaseVideoPlayer") << "getTotalNumFrames() not implemented";
	return 0;
}

//---------------------------------------------------------------------------
ofLoopType ofBaseVideoPlayer::getLoopState() const {
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
ofMatrix4x4 ofBaseRenderer::getCurrentOrientationMatrix() const {
	ofLogWarning() << "getCurrentOrientationMatrix() Not implemented for this renderer. Returning Identity matrix.";
	return ofMatrix4x4();
}


//---------------------------------------------------------------------------
void ofBaseMaterial::begin(){
	if(ofIsGLProgrammableRenderer()){
		ofGetGLProgrammableRenderer()->setCurrentMaterial(this);
	}
}

void ofBaseMaterial::end(){
	if(ofIsGLProgrammableRenderer()){
		ofGetGLProgrammableRenderer()->setCurrentMaterial(NULL);
	}
}

//---------------------------------------------------------------------------
//void ofBaseVideoPlayer::setPixelFormat(ofPixelFormat pixelFormat){
//	ofLogWarning("ofBaseVideoPlayer") << "setPixelFormat() not implemented";
//}
//---------------------------------------------------------------------------
//ofPixelFormat ofBaseVideoPlayer::getPixelFormat() const {
//	ofLogWarning("ofBaseVideoPlayer") << "getPixelFormat() not implemented";
//	return OF_PIXELS_RGB;
//}
