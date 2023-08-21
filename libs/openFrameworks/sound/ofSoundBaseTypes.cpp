#include "ofSoundBaseTypes.h"
#include "ofSoundBuffer.h"
#include "ofLog.h"

void ofBaseSoundStream::printDeviceList() const {
	ofLogNotice("ofBaseSoundStream::printDeviceList") << std::endl;
#ifndef TARGET_EMSCRIPTEN
	for(int i=ofSoundDevice::ALSA; i<ofSoundDevice::NUM_APIS; ++i){
		ofSoundDevice::Api api = (ofSoundDevice::Api)i;
		auto devices = getDeviceList(api);
		if(!devices.empty()){
				ofLogNotice("ofBaseSoundStream::printDeviceList") << "Api: " << api;
//				ofLogNotice("ofBaseSoundStream::printDeviceList") << devices;
		}
	}
#else
	ofSoundDevice::Api api = (ofSoundDevice::Api)0;
	getDeviceList(api); // only trigger the java script function once.
#endif
}
#ifndef TARGET_ANDROID

std::string toString(ofSoundDevice::Api api){ switch(api){ case ofSoundDevice::ALSA: return "Alsa"; case ofSoundDevice::PULSE: return "Pulse"; case ofSoundDevice::OSS: return "OSS"; case ofSoundDevice::JACK: return "Jack"; case ofSoundDevice::OSX_CORE: return "OSX Core Audio"; case ofSoundDevice::MS_WASAPI: return "MS WASAPI"; case ofSoundDevice::MS_ASIO: return "MS ASIO"; case ofSoundDevice::MS_DS: return "MS DirectShow"; default: return "Unkown API"; } }
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

#else

void ofBaseSoundInput::audioIn(ofSoundBuffer &buffer) {

}

void ofBaseSoundInput::audioIn(float *input, int bufferSize, int nChannels, int deviceID,
                               unsigned long long int tickCount) {

}

void ofBaseSoundInput::audioIn(float *input, int bufferSize, int nChannels) {

}

void ofBaseSoundOutput::audioOut(ofSoundBuffer &buffer) {

}

void ofBaseSoundOutput::audioOut(float *output, int bufferSize, int nChannels) {

}

void ofBaseSoundOutput::audioOut(float *output, int bufferSize, int nChannels, int deviceID,
								 unsigned long long int tickCount) {

}

#endif
