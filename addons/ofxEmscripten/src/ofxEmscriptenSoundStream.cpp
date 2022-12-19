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

using namespace std;

int ofxEmscriptenAudioContext();

ofxEmscriptenSoundStream::ofxEmscriptenSoundStream()
:context(ofxEmscriptenAudioContext())
,tickCount(0)
{

}

ofxEmscriptenSoundStream::~ofxEmscriptenSoundStream() {
	close();
}

std::vector<ofSoundDevice> ofxEmscriptenSoundStream::getDeviceList(ofSoundDevice::Api api) const{
	html5audio_list_devices();
	return vector<ofSoundDevice>();
}

bool ofxEmscriptenSoundStream::setup(const ofSoundStreamSettings & settings) {
	inbuffer.allocate(settings.bufferSize, settings.numInputChannels);
	outbuffer.allocate(settings.bufferSize, settings.numOutputChannels);
	this->settings = settings;
	html5audio_stream_create(settings.bufferSize,settings.numInputChannels,settings.numOutputChannels,inbuffer.getBuffer().data(),outbuffer.getBuffer().data(),&audio_cb,this);
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
	html5audio_context_start();
}

void ofxEmscriptenSoundStream::stop() {
	html5audio_context_stop();
}

void ofxEmscriptenSoundStream::close() {
	html5audio_stream_free();
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
	return html5audio_context_samplerate();
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
