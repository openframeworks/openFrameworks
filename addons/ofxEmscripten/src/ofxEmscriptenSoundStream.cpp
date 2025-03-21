/*
 * ofxEmscriptenSoundStream.cpp
 *
 *  Created on: May 16, 2014
 *      Author: arturo
 */

#include "ofxEmscriptenSoundStream.h"
#include "ofBaseApp.h"
#include "ofLog.h"
#include "html5audio.h"
#include "emscripten/webaudio.h"

using namespace std;

EM_BOOL ProcessAudio(int numInputs, const AudioSampleFrame *inputs, int numOutputs, AudioSampleFrame *outputs, int numParams, const AudioParamFrame *params, void *userData) {
	ofxEmscriptenSoundStream * stream = (ofxEmscriptenSoundStream *)userData;
	++stream->audioProcessedCount;
	if (stream->settings.numInputChannels > 0 && stream->settings.inCallback) {
		stream->settings.inCallback(stream->inbuffer);
		for (int o = 0; o < numInputs; ++o) {
			for (int i = 0; i < 128; ++i) {
				for (int ch = 0; ch < inputs[o].numberOfChannels; ++ch) {
					stream->inbuffer[i * inputs[o].numberOfChannels + ch] = inputs[o].data[ch * 128 + i];
				}
			}
		}
	}
	if (stream->settings.numOutputChannels > 0 && stream->settings.outCallback) {
		stream->settings.outCallback(stream->outbuffer);
		for (int o = 0; o < numOutputs; ++o) {
			for (int i = 0; i < 128; ++i) {
				for (int ch = 0; ch < outputs[o].numberOfChannels; ++ch) {
					outputs[o].data[ch * 128 + i] = stream->outbuffer[i * outputs[o].numberOfChannels + ch];
				}
			}
		}
	}
	return EM_TRUE;
}

void AudioWorkletProcessorCreated(EMSCRIPTEN_WEBAUDIO_T audioContext, EM_BOOL success, void *userData) {
	if (!success) return;
	ofxEmscriptenSoundStream * stream = (ofxEmscriptenSoundStream *)userData;
	int outputChannelCounts[1] = { static_cast<int>(stream->settings.numOutputChannels) };
	EmscriptenAudioWorkletNodeCreateOptions options = {
		.numberOfInputs = 1,
		.numberOfOutputs = 1,
		.outputChannelCounts = outputChannelCounts
	};
	EMSCRIPTEN_AUDIO_WORKLET_NODE_T audioWorklet = emscripten_create_wasm_audio_worklet_node(audioContext, "audio-processor", &options, &ProcessAudio, userData);
	html5audio_stream_create(audioWorklet, stream->settings.numInputChannels);
}

void WebAudioWorkletThreadInitialized(EMSCRIPTEN_WEBAUDIO_T audioContext, EM_BOOL success, void *userData) {
	if (!success) return;
	WebAudioWorkletProcessorCreateOptions opts = {
		.name = "audio-processor",
	};
	emscripten_create_wasm_audio_worklet_processor_async(audioContext, &opts, AudioWorkletProcessorCreated, userData);
}

int ofxEmscriptenAudioContext();

ofxEmscriptenSoundStream::ofxEmscriptenSoundStream()
:context(ofxEmscriptenAudioContext())
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
	audioProcessedCount = 0;
	this->settings = settings;
	emscripten_start_wasm_audio_worklet_thread_async(context, wasmAudioWorkletStack, sizeof(wasmAudioWorkletStack), WebAudioWorkletThreadInitialized, this);
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
	return audioProcessedCount;
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
