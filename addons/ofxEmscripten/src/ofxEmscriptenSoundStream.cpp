/*
 * ofxEmscriptenSoundStream.cpp
 *
 *  Created on: May 16, 2014
 *      Author: arturo
 */

#include "ofxEmscriptenSoundStream.h"
#include "html5audio.h"
#include "ofBaseApp.h"
#include "ofBaseTypes.h"

int ofxEmscriptenAudioContext();

ofxEmscriptenSoundStream::ofxEmscriptenSoundStream()
:context(ofxEmscriptenAudioContext())
,stream(-1)
,tickCount(0)
,bufferSize(0)
,inChannels(0)
,outChannels(0)
,soundInput(NULL)
,soundOutput(NULL)
{

}

ofxEmscriptenSoundStream::~ofxEmscriptenSoundStream() {
	close();
}

void ofxEmscriptenSoundStream::setDeviceID(int deviceID) {
	ofLogWarning() << "ofSoundStream::setDeviceID() not supported in emscripten";
}

std::vector<ofSoundDevice> ofxEmscriptenSoundStream::getDeviceList() const{
	ofLogWarning() << "ofSoundStream::getDeviceList() not supported in emscripten";
	return vector<ofSoundDevice>();
}

int ofxEmscriptenSoundStream::getDeviceID() const{
	return 0;
}

bool ofxEmscriptenSoundStream::setup(int outChannels, int inChannels,
		int sampleRate, int bufferSize, int nBuffers) {
	inbuffer.resize(bufferSize*inChannels);
	outbuffer.resize(bufferSize*outChannels);
	this->bufferSize = bufferSize;
	this->outChannels = outChannels;
	this->inChannels = inChannels;
	stream = html5audio_stream_create(context,bufferSize,inChannels,outChannels,inbuffer.data(),outbuffer.data(),&audio_cb,this);
	return true;
}

bool ofxEmscriptenSoundStream::setup(ofBaseApp* app, int outChannels,
		int inChannels, int sampleRate, int bufferSize, int nBuffers) {
	setup(outChannels,inChannels,sampleRate,bufferSize,nBuffers);
	setInput(app);
	setOutput(app);
	return true;
}

void ofxEmscriptenSoundStream::setInput(ofBaseSoundInput* soundInput) {
	this->soundInput = soundInput;
}

void ofxEmscriptenSoundStream::setOutput(ofBaseSoundOutput* soundOutput) {
	this->soundOutput = soundOutput;
}

void ofxEmscriptenSoundStream::start() {

}

void ofxEmscriptenSoundStream::stop() {

}

void ofxEmscriptenSoundStream::close() {
	stream = -1;
	html5audio_stream_free(stream);
}

unsigned long long ofxEmscriptenSoundStream::getTickCount() const{
	return tickCount;
}

int ofxEmscriptenSoundStream::getNumInputChannels() const{
	return inChannels;
}

int ofxEmscriptenSoundStream::getNumOutputChannels() const{
	return outChannels;
}

int ofxEmscriptenSoundStream::getSampleRate() const{
	return html5audio_context_samplerate(context);
}

int ofxEmscriptenSoundStream::getBufferSize() const{
	return bufferSize;
}

void ofxEmscriptenSoundStream::audio_cb( int bufferSize, int inputChannels, int outputChannels, void * userData){
	ofxEmscriptenSoundStream * stream = (ofxEmscriptenSoundStream*) userData;
	stream->audioCB(bufferSize,inputChannels,outputChannels);
}

void ofxEmscriptenSoundStream::audioCB(int bufferSize, int inputChannels, int outputChannels){
	if(inputChannels>0) soundInput->audioIn(inbuffer.data(),bufferSize,inputChannels,0,tickCount);
	if(outputChannels>0) soundOutput->audioOut(outbuffer.data(),bufferSize,outputChannels,0,tickCount);
	tickCount++;
}
