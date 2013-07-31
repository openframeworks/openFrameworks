#if defined  __arm__

#include "AVFoundationVideoGrabber.h"
#include "ofxiOSVideoGrabber.h"

ofxiOSVideoGrabber::ofxiOSVideoGrabber() {
	grabber = ofPtr<AVFoundationVideoGrabber>(new AVFoundationVideoGrabber());
}

//needs implementing
vector <ofVideoDevice> ofxiOSVideoGrabber::listDevices() {
	return grabber->listDevices();
}

bool ofxiOSVideoGrabber::initGrabber(int w, int h) {
	return grabber->initGrabber(w, h);
}

bool ofxiOSVideoGrabber::isFrameNew() {
	return grabber->isFrameNew();
}

unsigned char * ofxiOSVideoGrabber::getPixels() {
	return grabber->getPixels();
}

ofPixelsRef ofxiOSVideoGrabber::getPixelsRef(){
    static ofPixels dummy;
    //@TODO implement me
    return dummy;
}

void ofxiOSVideoGrabber::close() {
}

float ofxiOSVideoGrabber::getHeight() {
	return grabber->getHeight();
}

float ofxiOSVideoGrabber::getWidth() {
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


ofPixelFormat ofxiOSVideoGrabber::getPixelFormat(){
    return grabber->getPixelFormat();
}

#endif
