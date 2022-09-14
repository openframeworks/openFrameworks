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

uint8_t wasmAudioWorkletStack[4096];

// This callback will fire after the Audio Worklet Processor has finished being added to the Worklet global scope.
void AudioWorkletProcessorCreated(EMSCRIPTEN_WEBAUDIO_T audioContext, EM_BOOL success, int inputChannels, int outputChannels, int inbuffer, int outbuffer, int stream_callback, int userData) 
{
  if (!success) return; 
  // Specify the input and output node configurations for the Wasm Audio Worklet. A simple setup with single mono output channel here, and no inputs.
  int outputChannelCounts[1] = { outputChannels };

  EmscriptenAudioWorkletNodeCreateOptions options = {
    .numberOfInputs = 1,
    .numberOfOutputs = 1,
    .outputChannelCounts = outputChannelCounts
  };

  // Instantiate the noise-generator Audio Worklet Processor.
  EMSCRIPTEN_AUDIO_WORKLET_NODE_T audioWorklet = emscripten_create_wasm_audio_worklet_node(audioContext, "noise-generator", &options, inputChannels, outputChannels, inbuffer, outbuffer, stream_callback, userData);
  html5audio_stream_connect(audioContext, audioWorklet, inputChannels);
}

// This callback will fire when the Wasm Module has been shared to the AudioWorklet global scope, and is now ready to begin adding Audio Worklet Processors.
void WebAudioWorkletThreadInitialized(EMSCRIPTEN_WEBAUDIO_T audioContext, EM_BOOL success, int inputChannels, int outputChannels, int inbuffer, int outbuffer, int stream_callback, int userData)
{
  if (!success) return;
  WebAudioWorkletProcessorCreateOptions opts = {
    .name = "noise-generator",
  };
  emscripten_create_wasm_audio_worklet_processor_async(audioContext, &opts, AudioWorkletProcessorCreated, inputChannels, outputChannels, inbuffer, outbuffer, stream_callback, userData);
}

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

std::vector<ofSoundDevice> ofxEmscriptenSoundStream::getDeviceList(ofSoundDevice::Api api) const{
	ofLogWarning() << "ofSoundStream::getDeviceList() not supported in emscripten";
	return vector<ofSoundDevice>();
}

bool ofxEmscriptenSoundStream::setup(const ofSoundStreamSettings & settings) {
	inbuffer.allocate(settings.bufferSize, settings.numInputChannels);
	outbuffer.allocate(settings.bufferSize, settings.numOutputChannels);
	this->settings = settings;
	emscripten_start_wasm_audio_worklet_thread_async(context, wasmAudioWorkletStack, sizeof(wasmAudioWorkletStack), WebAudioWorkletThreadInitialized, settings.numInputChannels, settings.numOutputChannels, inbuffer.getBuffer().data(), outbuffer.getBuffer().data(), &audio_cb, this);
	stream = html5audio_stream_create();
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
