
#include "ofSoundStream.h"
#include "ofBaseSoundStream.h"
#include "ofSoundBuffer.h"
#include "ofLog.h"

void ofBaseSoundStream::applySoundStreamOriginInfo( ofSoundBuffer& buffer ) {
	buffer.tickCount = getTickCount();
	buffer.soundStreamDeviceID = getDeviceID();
}

void ofBaseSoundStream::printDeviceList() {
	ofLogNotice("ofBaseSoundStream::printDeviceList") << std::endl << getDeviceList();
}

std::vector<ofSoundDevice> ofBaseSoundStream::listDevices() {
	return getDeviceList();
}
