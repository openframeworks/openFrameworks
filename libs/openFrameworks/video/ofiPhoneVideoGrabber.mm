#if defined  __arm__

#include "AVFoundationVideoGrabber.h"
#include "ofiPhoneVideoGrabber.h"

ofiPhoneVideoGrabber::ofiPhoneVideoGrabber() {
	grabber = ofPtr<AVFoundationVideoGrabber>(new AVFoundationVideoGrabber());
}

//needs implementing
void ofiPhoneVideoGrabber::listDevices() {
	grabber->listDevices();
}

bool ofiPhoneVideoGrabber::initGrabber(int w, int h) {
	return grabber->initGrabber(w, h);
}

bool ofiPhoneVideoGrabber::isFrameNew() {
	return grabber->isFrameNew();
}

unsigned char * ofiPhoneVideoGrabber::getPixels() {
	return grabber->getPixels();
}

ofPixelsRef ofiPhoneVideoGrabber::getPixelsRef(){
    static ofPixels dummy;
    //@TODO implement me
    return dummy;
}

void ofiPhoneVideoGrabber::close() {
}

float ofiPhoneVideoGrabber::getHeight() {
	return grabber->getHeight();
}

float ofiPhoneVideoGrabber::getWidth() {
	return grabber->getWidth();
}

void ofiPhoneVideoGrabber::update() {
	grabber->update();
}

void ofiPhoneVideoGrabber::setDeviceID(int _deviceID) {
	grabber->setDevice(_deviceID);
}

void ofiPhoneVideoGrabber::setDesiredFrameRate(int framerate) {
	grabber->setCaptureRate(framerate);
}

void ofiPhoneVideoGrabber::setPixelFormat(ofPixelFormat internalPixelFormat) {
	grabber->setPixelFormat(internalPixelFormat);
}


ofPixelFormat ofiPhoneVideoGrabber::getPixelFormat(){
    return grabber->getPixelFormat();
}

#endif
