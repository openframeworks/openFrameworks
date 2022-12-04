/*
 * ofxEmscriptenSoundStream.cpp
 *
 *  Created on: May 16, 2014
 *      Author: arturo
 */

#include "ofxEmscriptenSoundStream.h"
#include "html5audio.h"
#include "ofBaseApp.h"
#include "ofLog.h"
#include "emscripten.h"

using namespace std;

int ofxEmscriptenAudioContext();

ofxEmscriptenSoundStream::ofxEmscriptenSoundStream()
:context(ofxEmscriptenAudioContext())
,stream(-1)
,tickCount(0)
{

}

ofxEmscriptenSoundStream::~ofxEmscriptenSoundStream() {
	close();
}

EM_ASYNC_JS(const char*, html5audio_list_devices_em_async_js, (), {
	var string = "";
	if (!navigator.mediaDevices.enumerateDevices) {
		console.log("enumerateDevices() not supported.");
	} else {
		// List cameras and microphones.
		var devices = await (navigator.mediaDevices.enumerateDevices());
		devices.forEach((device) => {
			if (device.kind == "audioinput" || device.kind == "audiooutput"){
				string = string.concat(",", `${device.kind}: ${device.label} id = ${device.deviceId}`);
			}
		});
	}
	return allocate(intArrayFromString(string, ALLOC_STACK -1));
});

std::vector<ofSoundDevice> ofxEmscriptenSoundStream::getDeviceList(ofSoundDevice::Api api) const{
	std::string devices = html5audio_list_devices_em_async_js();
	std::vector<std::string> deviceList = ofSplitString(devices, ",", true);
	for (auto&& device : deviceList){
		ofLogNotice() << device;
	}
	return vector<ofSoundDevice>(); // does nothing
}

bool ofxEmscriptenSoundStream::setup(const ofSoundStreamSettings & settings) {
	inbuffer.allocate(settings.bufferSize, settings.numInputChannels);
	outbuffer.allocate(settings.bufferSize, settings.numOutputChannels);
	this->settings = settings;
	stream = html5audio_stream_create(context,settings.bufferSize,settings.numInputChannels,settings.numOutputChannels,inbuffer.getBuffer().data(),outbuffer.getBuffer().data(),&audio_cb,this);
	return true;
}

void ofxEmscriptenSoundStream::setInput(ofBaseSoundInput* soundInput) {
	this->settings.setInListener(soundInput);
}

void ofxEmscriptenSoundStream::setOutput(ofBaseSoundOutput* soundOutput) {
	this->settings.setOutListener(soundOutput);
}


ofSoundDevice ofxEmscriptenSoundStream::getInDevice() const{
	return ofSoundDevice();
}

ofSoundDevice ofxEmscriptenSoundStream::getOutDevice() const{
	return ofSoundDevice();
}

void ofxEmscriptenSoundStream::start() {
	html5audio_context_start(context);
}

void ofxEmscriptenSoundStream::stop() {
	html5audio_context_stop(context);
}

void ofxEmscriptenSoundStream::close() {
	stream = -1;
	html5audio_stream_free(stream);
}

uint64_t ofxEmscriptenSoundStream::getTickCount() const{
	return tickCount;
}

int ofxEmscriptenSoundStream::getNumInputChannels() const{
	return settings.numInputChannels;
}

int ofxEmscriptenSoundStream::getNumOutputChannels() const{
	return settings.numOutputChannels;
}

int ofxEmscriptenSoundStream::getSampleRate() const{
	return html5audio_context_samplerate(context);
}

int ofxEmscriptenSoundStream::getBufferSize() const{
	return settings.bufferSize;
}

void ofxEmscriptenSoundStream::audio_cb( int bufferSize, int inputChannels, int outputChannels, void * userData){
	ofxEmscriptenSoundStream * stream = (ofxEmscriptenSoundStream*) userData;
	stream->audioCB(bufferSize,inputChannels,outputChannels);
}

void ofxEmscriptenSoundStream::audioCB(int bufferSize, int inputChannels, int outputChannels){
	if(inputChannels>0 && settings.inCallback) settings.inCallback(inbuffer);
	if(outputChannels>0 && settings.outCallback) settings.outCallback(outbuffer);
	tickCount++;
}
