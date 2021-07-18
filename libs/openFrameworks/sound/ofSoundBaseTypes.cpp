#include "ofSoundBaseTypes.h"
#include "ofSoundBuffer.h"
#include "ofLog.h"

void ofBaseSoundStream::printDeviceList() const {
	ofLogNotice("ofBaseSoundStream::printDeviceList") << std::endl;
	for(int i=ofSoundDevice::ALSA; i<ofSoundDevice::NUM_APIS; ++i){
		ofSoundDevice::Api api = (ofSoundDevice::Api)i;
		auto devices = getDeviceList(api);
		if(!devices.empty()){
				ofLogNotice("ofBaseSoundStream::printDeviceList") << "Api: " << api;
//				ofLogNotice("ofBaseSoundStream::printDeviceList") << devices;
		}
	}
}

ofBaseSoundStream::~ofBaseSoundStream() {

}

#ifndef TARGET_ANDROID
//------------------------------------------------------------
std::ostream& operator << (std::ostream& os, const ofSoundDevice& dev) {
	os << "[" << toString(dev.api) << ": " << dev.deviceID << "] " << dev.name;
	os << " [in:" << dev.inputChannels << " out:" << dev.outputChannels << "]";
	if(dev.isDefaultInput) os << " (default in)";
	if(dev.isDefaultOutput) os << " (default out)";
	return os;
}

////------------------------------------------------------------
std::ostream& operator << (std::ostream& os, const std::vector<ofSoundDevice>& devs) {
	for(std::size_t i = 0; i < devs.size(); i++) {
		os << devs[i] << std::endl;
	}
	return os;
}

#endif


void ofBaseSoundInput::audioIn(ofSoundBuffer &buffer) {

}

ofBaseSoundOutput::~ofBaseSoundOutput() {

}

ofBaseSoundPlayer::~ofBaseSoundPlayer() {

}
