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
void ofSoundObject::audioOut(float *out, int length, int numChannels) {
	if(inputObject!=NULL) {
		inputObject->audioOut(in_, length, numChannels);
	}
	this->process(in_, out, length, numChannels);
}



ofSoundInput::ofSoundInput() {
	buff = NULL;
}
	
// copy audio in to internal buffer - parameters will
// be replaced by ofSoundBuffer
void ofSoundInput::audioIn(float *in, int length, int numChannels) {
	if(buff==NULL) buff = new float[length * numChannels];
	memcpy(buff, in, length*sizeof(float)*numChannels);
	this->length = length;
	this->numChannels = numChannels;
}
	
void ofSoundInput::audioOut(float *out, int length, int numChannels) {
	
	// there should be an ofLogWarning() here to say
	// that the number of channels doesn't match
	assert(length==this->length);
	assert(numChannels==this->numChannels);
	
	
	memcpy(out, buff, length * numChannels * sizeof(float));
}
