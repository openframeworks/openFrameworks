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
#include "ofSoundBuffer.h"

using namespace std;

int stream_callback;
ofSoundBuffer inbuffer;
ofSoundBuffer outbuffer;
uint8_t wasmAudioWorkletStack[4096];

EM_BOOL ProcessAudio(int numInputs, const AudioSampleFrame *inputs, int numOutputs, AudioSampleFrame *outputs, int numParams, const AudioParamFrame *params, void *userData) {
	ofxEmscriptenSoundStream* stream = (ofxEmscriptenSoundStream*)stream_callback;
	stream->ofxEmscriptenSoundStream::audioCB(stream->settings.bufferSize, stream->settings.numInputChannels, stream->settings.numOutputChannels);
	if (stream->settings.numInputChannels > 0) {
		for (int o = 0; o < numInputs; ++o) {
			for (int i = 0; i < 128; ++i) {
				for (int ch = 0; ch < inputs[o].numberOfChannels; ++ch) {
					inbuffer[i * inputs[o].numberOfChannels + ch] = inputs[o].data[ch * 128 + i];
				}
			}
		}
	}
	if (stream->settings.numOutputChannels > 0) {
		for (int o = 0; o < numOutputs; ++o) {
			for (int i = 0; i < 128; ++i) {
				for (int ch = 0; ch < stream->settings.numOutputChannels; ++ch) {
					outputs[o].data[ch * 128 + i] = outbuffer[i * outputs[o].numberOfChannels + ch];
				}
			}
		}
	}
	return EM_TRUE;
}

void AudioWorkletProcessorCreated(EMSCRIPTEN_WEBAUDIO_T audioContext, EM_BOOL success, void *userData) {
	if (!success) return;
	ofxEmscriptenSoundStream* stream = (ofxEmscriptenSoundStream*)stream_callback;
	int outputChannelCounts[1] = { static_cast<int>(stream->settings.numOutputChannels) };
	EmscriptenAudioWorkletNodeCreateOptions options = {
		.numberOfInputs = 1,
		.numberOfOutputs = 1,
		.outputChannelCounts = outputChannelCounts
	};
	EMSCRIPTEN_AUDIO_WORKLET_NODE_T audioWorklet = emscripten_create_wasm_audio_worklet_node(audioContext, "audio-processor", &options, &ProcessAudio, 0);
	html5audio_stream_create(audioWorklet, static_cast<int>(stream->settings.numInputChannels));
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
	stream_callback = reinterpret_cast<std::uintptr_t>(this);
	emscripten_start_wasm_audio_worklet_thread_async(context, wasmAudioWorkletStack, sizeof(wasmAudioWorkletStack), WebAudioWorkletThreadInitialized, 0);
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

void ofxEmscriptenSoundStream::audioCB(int bufferSize, int inputChannels, int outputChannels){
	if(inputChannels>0 && settings.inCallback) settings.inCallback(inbuffer);
	if(outputChannels>0 && settings.outCallback) settings.outCallback(outbuffer);
	tickCount++;
}
