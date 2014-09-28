#if defined  __arm__

#include "AVFoundationVideoGrabber.h"
#include "ofxiOSVideoGrabber.h"

ofxiOSVideoGrabber::ofxiOSVideoGrabber() {
	grabber = shared_ptr<AVFoundationVideoGrabber>(new AVFoundationVideoGrabber());
}

ofxiOSVideoGrabber::~ofxiOSVideoGrabber() {
}

//needs implementing
vector <ofVideoDevice> ofxiOSVideoGrabber::listDevices() const {
	return grabber->listDevices();
}

bool ofxiOSVideoGrabber::initGrabber(int w, int h) {
	return grabber->initGrabber(w, h);
}

bool ofxiOSVideoGrabber::isInitialized() const{
    return grabber->isInitialized();
}

bool ofxiOSVideoGrabber::isFrameNew() const {
	return grabber->isFrameNew();
}

unsigned char * ofxiOSVideoGrabber::getPixels() {
	return grabber->getPixels();
}

ofPixels& ofxiOSVideoGrabber::getPixelsRef(){
    static ofPixels dummy;
    //@TODO implement me
    return dummy;
}

const ofPixels& ofxiOSVideoGrabber::getPixelsRef() const {
    return getPixelsRef();
}

void ofxiOSVideoGrabber::close() {
}

float ofxiOSVideoGrabber::getHeight() const {
	return grabber->getHeight();
}

float ofxiOSVideoGrabber::getWidth() const {
	return grabber->getWidth();
}

void ofxiOSVideoGrabber::update() {
	grabber->update();
}

void ofxiOSVideoGrabber::setDeviceID(int _deviceID) {
	grabber->setDevice(_deviceID);
}

void ofxiOSVideoGrabber::setDesiredFrameRate(int framerate) {
	grabber->setCaptureRate(framerate);
}

bool ofxiOSVideoGrabber::setPixelFormat(ofPixelFormat internalPixelFormat) {
	return grabber->setPixelFormat(internalPixelFormat);
}


ofPixelFormat ofxiOSVideoGrabber::getPixelFormat() const {
    return grabber->getPixelFormat();
}

#endif
