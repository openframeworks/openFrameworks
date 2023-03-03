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

ofEvent<void> audioWorkletEvent;

// This event will fire on the audio worklet thread.
void MessageReceivedInAudioWorkletThread_1(int inputChannels, int outputChannels, int inbuffer) {
	EM_ASM({globalThis.inputChannels = $0; globalThis.outputChannels = $1; globalThis.inbuffer = $2;}, inputChannels, outputChannels, inbuffer);
}

void MessageReceivedInAudioWorkletThread_2(int outbuffer, int stream_callback, int userData) {
	EM_ASM({globalThis.outbuffer = $0; globalThis.stream_callback = $1; globalThis.userData = $2;}, outbuffer, stream_callback, userData);
}

EM_BOOL ProcessAudio(int numInputs, const AudioSampleFrame *inputs, int numOutputs, AudioSampleFrame *outputs, int numParams, const AudioParamFrame *params, void *userData) {
	return EM_TRUE;
}

// This callback will fire after the Audio Worklet Processor has finished being added to the Worklet global scope.
void AudioWorkletProcessorCreated(EMSCRIPTEN_WEBAUDIO_T audioContext, EM_BOOL success, void *userData) {
	if (!success) return; 
	// Specify the input and output node configurations for the Wasm Audio Worklet. A simple setup with single mono output channel here, and no inputs.
	int outputChannelCounts[1] = { 2 };
	EmscriptenAudioWorkletNodeCreateOptions options = {
		.numberOfInputs = 1,
		.numberOfOutputs = 1,
		.outputChannelCounts = outputChannelCounts
	};
	// Instantiate the noise-generator Audio Worklet Processor.
	EMSCRIPTEN_AUDIO_WORKLET_NODE_T audioWorklet = emscripten_create_wasm_audio_worklet_node(audioContext, "tone-generator", &options, &ProcessAudio, 0);
	html5audio_stream_create(audioWorklet, 0);
}

// This callback will fire when the Wasm Module has been shared to the AudioWorklet global scope, and is now ready to begin adding Audio Worklet Processors.
void WebAudioWorkletThreadInitialized(EMSCRIPTEN_WEBAUDIO_T audioContext, EM_BOOL success, void *userData) {
	if (!success) return;
	audioWorkletEvent.notify();

	WebAudioWorkletProcessorCreateOptions opts = {
		.name = "tone-generator",
	};
	emscripten_create_wasm_audio_worklet_processor_async(audioContext, &opts, AudioWorkletProcessorCreated, 0);
}

void ofxEmscriptenSoundStream::audioWorklet() {
	emscripten_audio_worklet_post_function_viii(context, MessageReceivedInAudioWorkletThread_1, /*inputChannels=*/settings.numInputChannels, /*outputChannels=*/settings.numOutputChannels, /*inbuffer=*/EM_ASM_INT(return $0, inbuffer.getBuffer().data()));
	emscripten_audio_worklet_post_function_viii(context, MessageReceivedInAudioWorkletThread_2, /*outbuffer=*/EM_ASM_INT(return $0, outbuffer.getBuffer().data()), /*stream_callback=*/EM_ASM_INT(return $0, &audio_cb), /*userData=*/EM_ASM_INT(return $0, this));
}

// Define a global stack space for the AudioWorkletGlobalScope. Note that all AudioWorkletProcessors and/or AudioWorkletNodes on the given Audio Context all share the same AudioWorkerGlobalScope,
// i.e. they all run on the same one audio thread (multiple nodes/processors do not each get their own thread). Hence one stack is enough.
uint8_t wasmAudioWorkletStack[4096];

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
	ofLogWarning() << "ofSoundStream::getDeviceList() not supported in emscripten";
	return vector<ofSoundDevice>();
}

bool ofxEmscriptenSoundStream::setup(const ofSoundStreamSettings & settings) {
	ofAddListener(audioWorkletEvent, this, &ofxEmscriptenSoundStream::audioWorklet);
	inbuffer.allocate(settings.bufferSize, settings.numInputChannels);
	outbuffer.allocate(settings.bufferSize, settings.numOutputChannels);
	this->settings = settings;
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

void ofxEmscriptenSoundStream::audio_cb( int bufferSize, int inputChannels, int outputChannels, void * userData){
	ofxEmscriptenSoundStream * stream = (ofxEmscriptenSoundStream*) userData;
	stream->audioCB(bufferSize,inputChannels,outputChannels);
}

void ofxEmscriptenSoundStream::audioCB(int bufferSize, int inputChannels, int outputChannels){
	if(inputChannels>0 && settings.inCallback) settings.inCallback(inbuffer);
	if(outputChannels>0 && settings.outCallback) settings.outCallback(outbuffer);
	tickCount++;
}
