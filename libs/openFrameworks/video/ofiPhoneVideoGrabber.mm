#if defined  __arm__

#include "ofiPhoneVideoGrabber.h"
#import "AVFoundationVideoGrabber.h"


ofiPhoneVideoGrabber::ofiPhoneVideoGrabber() {
	grabber = new AVFoundationVideoGrabber();
}

ofiPhoneVideoGrabber::~ofiPhoneVideoGrabber() {
	delete grabber;
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

void ofiPhoneVideoGrabber::close() {
}

float ofiPhoneVideoGrabber::getHeight() {
	return grabber->getHeight();
}

float ofiPhoneVideoGrabber::getWidth() {
	return grabber->getWidth();
}

void ofiPhoneVideoGrabber::update() {
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

#endif