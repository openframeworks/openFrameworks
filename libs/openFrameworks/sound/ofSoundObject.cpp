/**
 *  ofSoundObject.cpp
 *
 *  Created by Marek Bereza on 10/08/2013.
 */

#include "ofSoundObject.h"

//----------------------------------------------------
// ofSoundObject

ofSoundObject::ofSoundObject(): inputObject(NULL), outputObjectPtr(NULL){
}


ofSoundObject &ofSoundObject::connectTo(ofSoundObject& soundObject) {
	if (outputObjectPtr) {
		disconnect();
	}
	outputObjectPtr = &soundObject;

	soundObject.setInput(*this);
	
	// if we find an infinite loop, we want to disconnect and provide an error
	if(!checkForInfiniteLoops()) {
		ofLogError("ofSoundObject") << "There's an infinite loop in your chain of ofSoundObjects";
		disconnect();
	}
	return soundObject;
}

void ofSoundObject::disconnectInput(ofSoundObject& input){
	if (inputObject) {
		inputObject = NULL;
	}
}

void ofSoundObject::disconnect(){
	outputObjectPtr->disconnectInput(*this);
	outputObjectPtr = NULL;
}

void ofSoundObject::setInput(ofSoundObject& obj) {
	inputObject = &obj;
}

ofSoundObject& ofSoundObject::getInputObject() {
	return *inputObject;
}

const ofSoundObject& ofSoundObject::getInputObject() const
{
    return *inputObject;
}


bool ofSoundObject::checkForInfiniteLoops() const {
	ofSoundObject* prev = inputObject;

	// move up the dsp chain until we find ourselves or the beginning of the chain (input==NULL)
	while(prev!=this && prev!=NULL) {
		prev = &prev->getInputObject();
	}

	// if we found ourselves, return false (to indicate there's an infinite loop)
	return (prev==NULL);
}

// this pulls the audio through from earlier links in the chain
void ofSoundObject::audioOut(ofSoundBuffer& output) {
	if(inputObject) {
		if(workingBuffer.size()!=output.size()) {
			ofLogVerbose("ofSoundObject") << "working buffer size != output buffer size.";
			workingBuffer.resize(output.size());
			workingBuffer.setNumChannels(output.getNumChannels());
			workingBuffer.setSampleRate(output.getSampleRate());
		}
		
		inputObject->audioOut(workingBuffer);
	}
	this->process(workingBuffer, output);
}

//----------------------------------------------------
// ofSoundInput

ofSoundInput::ofSoundInput() {

}

// copy audio in to internal buffer
void ofSoundInput::audioIn(ofSoundBuffer& input) {
	if(inputBuffer.size()!=input.size()) {
		ofLogVerbose("ofSoundinput::audioIn") << "input buffer size != output buffer size.";
		inputBuffer.resize(input.size());
		inputBuffer.setNumChannels(inputBuffer.getNumChannels());
		inputBuffer.setSampleRate(inputBuffer.getSampleRate());
	}
	input.copyTo(inputBuffer);
}

void ofSoundInput::audioOut(ofSoundBuffer& output) {
	if(output.getNumFrames()==inputBuffer.getNumFrames()){
		ofLogVerbose("ofSoundinput::audioOut") << "input buffer size != output buffer size.";
		inputBuffer.resize(output.size());
		inputBuffer.setNumChannels(output.getNumChannels());
		inputBuffer.setSampleRate(output.getSampleRate());
	}
	inputBuffer.copyTo(output);
}

//----------------------------------------------------
// ofSoundMixer

ofSoundMixer::ofSoundMixer(){
	masterVolume = 1.0f;
	masterPan = 0.5f;
}

ofSoundMixer::~ofSoundMixer(){

}

ofSoundMixer &ofGetSystemSoundMixer(){
	static ofSoundMixer systemSoundMixer;
	return systemSoundMixer;
}

std::shared_ptr<ofBaseSoundOutput> ofSoundMixer::getChannelSource(int channelNumber) {
	if (channelNumber < channels.size()) {
        return std::shared_ptr<ofBaseSoundOutput>(channels[channelNumber]);
	}else{
		return std::shared_ptr<ofBaseSoundOutput>();
	}
}

void ofSoundMixer::disconnectInput(ofSoundObject& input){
	for (std::size_t i =0; i<channels.size(); i++) {
		if (&input == channels[i]) {
			channels.erase(channels.begin() + i);
			break;
		}
	}
}

void ofSoundMixer::setInput(ofSoundObject& obj){
    for (std::size_t i =0; i<channels.size(); i++) {
		if (&obj == channels[i]) {
			ofLogNotice("ofSoundMixer::setInput") << " already connected" << endl;
			return;
		}
	}
	channels.push_back(&obj);
}

std::size_t ofSoundMixer::getNumChannels() const{
	return channels.size();
}

void ofSoundMixer::setMasterVolume(float vol){
	masterVolume = vol;
}

float ofSoundMixer::getMasterVolume() const{
	return masterVolume;
}

float ofSoundMixer::getMasterPan() const{
	return masterPan;
}

void ofSoundMixer::setMasterPan(float pan){
	masterPan = pan;
}

bool ofSoundMixer::isConnectedTo(const ofSoundObject& obj) const{
	return std::find(channels.begin(), channels.end(), &obj) != channels.end();
}

// this pulls the audio through from earlier links in the chain and sums up the total output
void ofSoundMixer::audioOut(ofSoundBuffer& output) {
	if(!channels.empty()) {
        for(std::size_t i = 0; i < channels.size(); i++){
			if (channels[i]) {
				ofSoundBuffer tempBuffer;
				tempBuffer.resize(output.size());
				tempBuffer.setNumChannels(output.getNumChannels());
				tempBuffer.setSampleRate(output.getSampleRate());
				channels[i]->audioOut(tempBuffer);
				
                for (std::size_t i = 0; i < tempBuffer.size(); i++) {
					output.getBuffer()[i] += tempBuffer.getBuffer()[i];
				}
			}
		}
		if(output.getNumChannels() == 2) {
			output.stereoPan(1 - masterPan, masterPan);
		}
		output*=masterVolume;
	}
}
