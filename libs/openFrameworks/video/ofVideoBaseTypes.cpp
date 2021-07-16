#include "ofVideoBaseTypes.h"

ofBaseVideoGrabber::~ofBaseVideoGrabber() = default;

void ofBaseVideoGrabber::setVerbose(bool bTalkToMe) {

}

void ofBaseVideoGrabber::setDeviceID(int deviceID) {

}

void ofBaseVideoGrabber::setDesiredFrameRate(int framerate) {

}

void ofBaseVideoGrabber::videoSettings() {

}


ofBaseVideoPlayer::~ofBaseVideoPlayer() = default;

void ofBaseVideoPlayer::previousFrame() {

}

void ofBaseVideoPlayer::loadAsync(std::string name) {

}

float ofBaseVideoPlayer::getPosition() const {
    return 0;
}

void ofBaseVideoPlayer::setSpeed(float speed) {

}

void ofBaseVideoPlayer::setFrame(int frame) {

}

int ofBaseVideoPlayer::getCurrentFrame() const {
    return 0;
}

int ofBaseVideoPlayer::getTotalNumFrames() const {
    return 0;
}

ofLoopType ofBaseVideoPlayer::getLoopState() const {
    return OF_LOOP_PALINDROME;
}

void ofBaseVideoPlayer::firstFrame() {

}

void ofBaseVideoPlayer::nextFrame() {

}

float ofBaseVideoPlayer::getSpeed() const {
    return 0;
}

float ofBaseVideoPlayer::getDuration() const {
    return 0;
}

bool ofBaseVideoPlayer::getIsMovieDone() const {
    return false;
}

void ofBaseVideoPlayer::setPaused(bool bPause) {

}

void ofBaseVideoPlayer::setPosition(float pct) {

}

void ofBaseVideoPlayer::setVolume(float volume) {

}

void ofBaseVideoPlayer::setLoopState(ofLoopType state) {

}

