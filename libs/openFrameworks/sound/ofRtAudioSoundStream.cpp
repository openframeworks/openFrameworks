#include "ofRtAudioSoundStream.h"
#include "ofMath.h"
#include "ofUtils.h"
#include "ofAppRunner.h"
#include "ofLog.h"
#include "RtAudio.h"

using namespace std;

//------------------------------------------------------------------------------
RtAudio::Api toRtAudio(ofSoundDevice::Api api){
	switch (api) {
	case ofSoundDevice::Api::ALSA:
		return RtAudio::Api::LINUX_ALSA;
	case ofSoundDevice::Api::PULSE:
		return RtAudio::Api::LINUX_PULSE;
	case ofSoundDevice::Api::OSS:
		return RtAudio::Api::LINUX_OSS;
	case ofSoundDevice::Api::JACK:
		return RtAudio::Api::UNIX_JACK;
#ifndef TARGET_LINUX
	case ofSoundDevice::Api::OSX_CORE:
		return RtAudio::Api::MACOSX_CORE;
	case ofSoundDevice::Api::MS_WASAPI:
		return RtAudio::Api::WINDOWS_WASAPI;
	case ofSoundDevice::Api::MS_ASIO:
		return RtAudio::Api::WINDOWS_ASIO;
	case ofSoundDevice::Api::MS_DS:
		return RtAudio::Api::WINDOWS_DS;
#endif
	default:
		return RtAudio::Api::UNSPECIFIED;
	}
}

//------------------------------------------------------------------------------
ofSoundDevice::Api toOf(RtAudio::Api api){
	switch (api) {
	case RtAudio::Api::LINUX_ALSA:
		return ofSoundDevice::Api::ALSA;
	case RtAudio::Api::LINUX_PULSE:
		return ofSoundDevice::Api::PULSE;
	case RtAudio::Api::LINUX_OSS:
		return ofSoundDevice::Api::OSS;
	case RtAudio::Api::UNIX_JACK:
		return ofSoundDevice::Api::JACK;
#ifndef TARGET_LINUX
	case RtAudio::Api::MACOSX_CORE:
		return ofSoundDevice::Api::OSX_CORE;
	case RtAudio::Api::WINDOWS_WASAPI:
		return ofSoundDevice::Api::MS_WASAPI;
	case RtAudio::Api::WINDOWS_ASIO:
		return ofSoundDevice::Api::MS_ASIO;
	case RtAudio::Api::WINDOWS_DS:
		return ofSoundDevice::Api::MS_DS;
#endif
	default:
		return ofSoundDevice::Api::UNSPECIFIED;
	}
}

//------------------------------------------------------------------------------
ofRtAudioSoundStream::ofRtAudioSoundStream() {
	tickCount = 0;
}

//------------------------------------------------------------------------------
ofRtAudioSoundStream::~ofRtAudioSoundStream() {
	stop();
	close();
}

//------------------------------------------------------------------------------
std::vector<ofSoundDevice> ofRtAudioSoundStream::getDeviceList(ofSoundDevice::Api api) const{
	vector<ofSoundDevice> deviceList;
	try {
		auto rtAudioApi = toRtAudio(api);
		RtAudio audioTemp(toRtAudio(api));
		audioTemp.showWarnings(false);
		if(audioTemp.getCurrentApi()!=rtAudioApi && rtAudioApi!=RtAudio::Api::UNSPECIFIED){
			return deviceList;
		}
		auto deviceCount = audioTemp.getDeviceCount();
		RtAudio::DeviceInfo info;
		for (unsigned int i = 0; i < deviceCount; i++) {
			try {
				info = audioTemp.getDeviceInfo(i);
			}
			catch (std::exception &error) {
				ofLogError("ofRtAudioSoundStream") << "Error retrieving info for device " << i;
				ofLogError() << error.what();
				break;
			}

			ofSoundDevice dev;
			dev.deviceID = i;
			dev.name = info.name;
			dev.outputChannels = info.outputChannels;
			dev.inputChannels = info.inputChannels;
			dev.sampleRates = info.sampleRates;
			dev.isDefaultInput = info.isDefaultInput;
			dev.isDefaultOutput = info.isDefaultOutput;
			dev.api = api;
			deviceList.push_back(dev);
		}
	}catch (std::exception &error) {
		ofLogError() << error.what();
	}

	return deviceList;

}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::setInput(ofBaseSoundInput * soundInput) {
	settings.setInListener(soundInput);
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::setOutput(ofBaseSoundOutput * soundOutput) {
	settings.setOutListener(soundOutput);
}

//------------------------------------------------------------------------------
bool ofRtAudioSoundStream::setup(const ofSoundStreamSettings & settings_)
{
	if (audio != nullptr) {
		close();
	}

	this->settings = settings_;

	tickCount = 0;
	this->settings.bufferSize = ofNextPow2(settings.bufferSize);	// must be pow2

	try {
		if (settings.getApi() != ofSoundDevice::Api::UNSPECIFIED) {
			audio = std::make_shared<RtAudio>(toRtAudio(settings.getApi()));
		}else{
			audio = std::make_shared<RtAudio>();
		}
	}
	catch (std::exception &error) {
		ofLogError() << error.what();
		return false;
	}

	RtAudio::StreamParameters outputParameters;
	RtAudio::StreamParameters inputParameters;
	if (settings.numInputChannels > 0) {
		if (!settings.getInDevice()) {
			ofSoundDevice device;
			device.deviceID = audio->getDefaultInputDevice();
			settings.setInDevice(device);
		}
		inputParameters.deviceId = settings.getInDevice()->deviceID;
		inputParameters.nChannels = settings.numInputChannels;
	}

	if (settings.numOutputChannels > 0) {
		if (!settings.getOutDevice()) {
			ofSoundDevice device;
			device.deviceID = audio->getDefaultOutputDevice();
			settings.setOutDevice(device);
		}
		outputParameters.deviceId = settings.getOutDevice()->deviceID;
		outputParameters.nChannels = settings.numOutputChannels;
	}

	RtAudio::StreamOptions options;
	options.flags = RTAUDIO_SCHEDULE_REALTIME;
	options.numberOfBuffers = settings.numBuffers;
	options.priority = 1;
	outputBuffer.setDeviceID(outputParameters.deviceId);
	inputBuffer.setDeviceID(inputParameters.deviceId);
	outputBuffer.setSampleRate(settings.sampleRate);
	inputBuffer.setSampleRate(settings.sampleRate);
	unsigned int bufferSize = settings.bufferSize;
	try {
		audio->openStream((settings.numOutputChannels > 0) ? &outputParameters : nullptr, (settings.numInputChannels > 0) ? &inputParameters : nullptr, RTAUDIO_FLOAT32,
			settings.sampleRate, &bufferSize, &rtAudioCallback, this, &options);
		audio->startStream();
	}
	catch (std::exception &error) {
		ofLogError() << error.what();
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::start() {
	if (audio == nullptr) return;

	try {
		audio->startStream();
	}
	catch (std::exception &error) {
		ofLogError() << error.what();
	}
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::stop() {
	if (audio == nullptr) return;

	try {
		if (audio->isStreamRunning()) {
			audio->stopStream();
		}
	}
	catch (std::exception &error) {
		ofLogError() << error.what();
	}
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::close() {
	if (audio == nullptr) return;

	try {
		if (audio->isStreamOpen()) {
			audio->closeStream();
		}
	}
	catch (std::exception &error) {
		ofLogError() << error.what();
	}
	settings.outCallback = nullptr;
	settings.inCallback = nullptr;
	audio.reset();	// delete
}

//------------------------------------------------------------------------------
uint64_t ofRtAudioSoundStream::getTickCount() const {
	return tickCount;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getNumInputChannels() const {
	return settings.numInputChannels;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getNumOutputChannels() const {
	return  settings.numOutputChannels;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getSampleRate() const {
	return settings.sampleRate;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getBufferSize() const {
	return settings.bufferSize;
}

ofSoundDevice ofRtAudioSoundStream::getInDevice() const{
	return *settings.getInDevice();
}

ofSoundDevice ofRtAudioSoundStream::getOutDevice() const{
	return *settings.getOutDevice();
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::rtAudioCallback(void *outputBuffer, void *inputBuffer, unsigned int nFramesPerBuffer, double streamTime, RtAudioStreamStatus status, void *data) {
	ofRtAudioSoundStream * rtStreamPtr = (ofRtAudioSoundStream *)data;

	if (status) {
		ofLogWarning("ofRtAudioSoundStream") << "stream over/underflow detected";
	}

	// 	rtAudio uses a system by which the audio
	// 	can be of different formats
	// 	char, float, etc.
	// 	we choose float
	float * fPtrOut = (float *)outputBuffer;
	float * fPtrIn = (float *)inputBuffer;
	// [zach] memset output to zero before output call
	// this is because of how rtAudio works: duplex w/ one callback
	// you need to cut in the middle. if the simpleApp
	// doesn't produce audio, we pass silence instead of duplex...

	size_t nInputChannels  = rtStreamPtr->getNumInputChannels();
	size_t nOutputChannels = rtStreamPtr->getNumOutputChannels();

	if (nInputChannels > 0) {
		if (rtStreamPtr->settings.inCallback) {
			rtStreamPtr->inputBuffer.copyFrom(fPtrIn, nFramesPerBuffer, nInputChannels, rtStreamPtr->getSampleRate());
			rtStreamPtr->inputBuffer.setTickCount(rtStreamPtr->tickCount);
			rtStreamPtr->settings.inCallback(rtStreamPtr->inputBuffer);
		}
		// [damian] not sure what this is for? assuming it's for underruns? or for when the sound system becomes broken?
		memset(fPtrIn, 0, nFramesPerBuffer * nInputChannels * sizeof(float));
	}

	if (nOutputChannels > 0) {
		if (rtStreamPtr->settings.outCallback) {

			if (rtStreamPtr->outputBuffer.size() != nFramesPerBuffer*nOutputChannels || rtStreamPtr->outputBuffer.getNumChannels() != nOutputChannels) {
				rtStreamPtr->outputBuffer.setNumChannels(nOutputChannels);
				rtStreamPtr->outputBuffer.resize(nFramesPerBuffer*nOutputChannels);
			}
			rtStreamPtr->outputBuffer.setTickCount(rtStreamPtr->tickCount);
			rtStreamPtr->settings.outCallback(rtStreamPtr->outputBuffer);
		}
		rtStreamPtr->outputBuffer.copyTo(fPtrOut, nFramesPerBuffer, nOutputChannels, 0);
		rtStreamPtr->outputBuffer.set(0);
	}

	// increment tick count
	rtStreamPtr->tickCount++;

	return 0;
}
