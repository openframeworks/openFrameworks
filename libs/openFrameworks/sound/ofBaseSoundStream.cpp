
#include "ofSoundStream.h"
#include "ofBaseSoundStream.h"
#include "ofSoundBuffer.h"
#include "ofLog.h"

void ofBaseSoundStream::printDeviceList() {
	ofLogNotice("ofBaseSoundStream::printDeviceList") << std::endl << getDeviceList();
}
