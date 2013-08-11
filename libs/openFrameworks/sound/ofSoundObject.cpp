/**
 *  ofSoundObject.cpp
 *
 *  Created by Marek Bereza on 10/08/2013.
 */

#include "ofSoundObject.h"


ofSoundObject::ofSoundObject() {
	inputObject = NULL;
}


ofSoundObject &ofSoundObject::connectTo(ofSoundObject &soundObject) {
	soundObject.setInput(this);
	
	// if we find an infinite loop, we want to disconnect and provide an error
	if(!checkForInfiniteLoops()) {
		ofLogError("ofSoundObject") << "There's an infinite loop in your chain of ofSoundObjects";
		soundObject.setInput(NULL);
	}
	return soundObject;
}

void ofSoundObject::setInput(ofSoundObject *obj) {
	inputObject = obj;
}


ofSoundObject *ofSoundObject::getInputObject() {
	return inputObject;
}

bool ofSoundObject::checkForInfiniteLoops() {
	
	ofSoundObject *prev = inputObject;
	
	// move up the dsp chain until we find ourselves or the beginning of the chain (input==NULL)
	while(prev!=this && prev!=NULL) {
		prev = prev->getInputObject();
	}
	
	// if we found ourselves, return false (to indicate there's an infinite loop)
	return (prev==NULL);
}




// this pulls the audio through from earlier links in the chain
void ofSoundObject::audioOut(ofSoundBuffer &output) {
	if(inputObject!=NULL) {
		
		if(workingBuffer.size()!=output.size()) {
			workingBuffer.resize(output.size());
		}
		
		inputObject->audioOut(workingBuffer);
	}
	this->process(workingBuffer, output);
}




ofSoundInput::ofSoundInput() {
}

	
// copy audio in to internal buffer
void ofSoundInput::audioIn(ofSoundBuffer &input) {
	if(inputBuffer.size()!=input.size()) {
		inputBuffer.resize(input.size());
	}
	input.copyTo(inputBuffer);
}
	
void ofSoundInput::audioOut(ofSoundBuffer &output) {
	
	// there should be an ofLogWarning() here to say
	// that the number of channels doesn't match
	assert(output.getNumFrames()==inputBuffer.getNumFrames());
	inputBuffer.copyTo(output);
}


