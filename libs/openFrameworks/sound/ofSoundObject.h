
#pragma once

#include "ofMain.h"

/**
 * ofSoundObject is a node in your dsp chain. It can have one input, 
 * and one output. If it doesn't have an input, it's the beginning
 * of the dsp chain (i.e. an oscillator or a microphone input).
 */
class ofSoundObject: public ofBaseSoundOutput {
public:
	

	ofSoundObject();
	virtual ~ofSoundObject() {}
	
	
	
	/// Connects the output of this ofSoundObject to the input of the parameter ofSoundObject
	ofSoundObject &connectTo(ofSoundObject &soundObject);

	
	/// This is the method you implement to process the signal from inputs to outputs.
	virtual void process(float *in, float *out, int length, int numChannels) {
		// default behaviour is pass-through.
		memcpy(out, in, length*numChannels*sizeof(float));
	}
	
	
	
	/// this pulls the audio through from earlier links in the chain.
	/// you can override this to add more interesting functionality
	/// like signal splitters, sidechains etc.
	virtual void audioOut(float *out, int length, int numChannels);
	
	
protected:
	// this is the previous dsp object in the chain
	// that feeds this one with input.
	ofSoundObject *inputObject;
	
private:
	
	// ofSoundObjects reference their source, not their destination
	// because it's not needed in a pullthrough audio architecture.
	// this lets that be set under the hood via connectTo()
	void setInput(ofSoundObject *obj);
	
	// this won't be needed with ofSoundBuffer, but it's just
	// a spare buffer to pass from one sound object to another
	float in_[8192];
	
};



/**
 * This class represents input from the sound card in your dsp chain.
 */
class ofSoundInput: public ofBaseSoundInput, public ofSoundObject {
public:
	ofSoundInput();
	// copy audio in to internal buffer - parameters will
	// be replaced by ofSoundBuffer
	void audioIn(float *in, int length, int numChannels);
	void audioOut(float *out, int length, int numChannels);
	
private:
	float *buff;
	int length;
	int numChannels;
};


/**
 * This class represents the output in your dsp chain.
 */
class ofSoundOutput: public ofSoundObject {};






