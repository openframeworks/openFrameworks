
#include "ofSoundStream.h"
#include "ofBaseSoundStream.h"
#include "ofSoundBuffer.h"
#include "ofLog.h"

void ofBaseSoundStream::printDeviceList() const {
	ofLogNotice("ofBaseSoundStream::printDeviceList") << std::endl << getDeviceList();
}
