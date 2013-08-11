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
	return soundObject;
}

void ofSoundObject::setInput(ofSoundObject *obj) {
	inputObject = obj;
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


