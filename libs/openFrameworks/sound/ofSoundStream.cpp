#include "ofSoundStream.h"
#include "ofAppRunner.h"

ofSoundStream soundStreamInput;
ofSoundStream soundStreamOutput;	

//------------------------------------------------------------
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * appPtr){
	if( appPtr == NULL ){
		appPtr = ofGetAppPtr();
	}
	ofSoundStreamSetup(nOutputChannels, nInputChannels, appPtr, 44100, 256, 4);
}

//------------------------------------------------------------
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, int sampleRate, int bufferSize, int nBuffers){
	ofSoundStreamSetup(nOutputChannels, nInputChannels, ofGetAppPtr(), sampleRate, bufferSize, nBuffers);
}

//------------------------------------------------------------
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * appPtr, int sampleRate, int bufferSize, int nBuffers){
	if( nOutputChannels > 0 ){
		//wow very strange bug with multiple inheritance - we have to covert to a pointer of ofBaseSound rather than in the call below. 	
		ofBaseSoundOutput * pOutput = (ofBaseSoundOutput*)appPtr;
		soundStreamOutput.setupOutput(pOutput, nOutputChannels, sampleRate, bufferSize, nBuffers);
	}
	if( nInputChannels > 0 ){
		//wow very strange bug with multiple inheritance - we have to covert to a pointer of ofBaseSound rather than in the call below. 
		ofBaseSoundInput * pInput = (ofBaseSoundInput *)appPtr;
		soundStreamInput.setupInput(pInput, nInputChannels,  sampleRate, bufferSize, nBuffers);
	}
}

//------------------------------------------------------------
void ofSoundStreamStop(){
	soundStreamOutput.stop();
	soundStreamInput.stop();
}

//------------------------------------------------------------
void ofSoundStreamStart(){
	soundStreamOutput.start();
	soundStreamInput.start();
}

//------------------------------------------------------------
void ofSoundStreamClose(){
	soundStreamOutput.close();
	soundStreamInput.close();
}

//------------------------------------------------------------
void ofSoundStreamListDevices(){
	soundStreamOutput.listDevices();
}


//------------------------------------------------------------
//------------------------------------------------------------
ofSoundStream::ofSoundStream(){
	soundStream = NULL;
	setSoundStream( new OF_SOUND_STREAM_TYPE );
}

//------------------------------------------------------------
ofSoundStream::~ofSoundStream(){
	close();
	if( soundStream ){
		delete soundStream;
		soundStream = NULL;
	}
}

//------------------------------------------------------------
void ofSoundStream::setSoundStream(ofBaseSoundStream * soundStreamPtr){
	if( soundStream ){
		soundStream->stop();
		soundStream->close();
		delete soundStream;
	}
	soundStream = soundStreamPtr;
}

//------------------------------------------------------------
void ofSoundStream::listDevices(){
	if( soundStream ){
		soundStream->listDevices();
	}
}

//------------------------------------------------------------
void ofSoundStream::setDeviceID(int deviceID){
	if( soundStream ){
		soundStream->setDeviceID(deviceID);
	}	
}

//------------------------------------------------------------
bool ofSoundStream::setupInput(ofBaseSoundInput * soundInputPtr, int numChannels, int sampleRate, int bufferSize, int nBuffers){
	if( soundStream ){
		soundStream->setupInput(soundInputPtr, numChannels, sampleRate, bufferSize, nBuffers);
	}
}

//------------------------------------------------------------
bool ofSoundStream::setupOutput(ofBaseSoundOutput * soundOutputPtr, int numChannels, int sampleRate, int bufferSize, int nBuffers){
	if( soundStream ){
		soundStream->setupOutput(soundOutputPtr, numChannels, sampleRate, bufferSize, nBuffers);
	}
}

//------------------------------------------------------------
void ofSoundStream::start(){
	if( soundStream ){
		soundStream->start();
	}
}

//------------------------------------------------------------
void ofSoundStream::stop(){
	if( soundStream ){
		soundStream->stop();
	}
}

//------------------------------------------------------------
void ofSoundStream::close(){
	if( soundStream ){
		soundStream->close();
	}
}

//------------------------------------------------------------
long unsigned long ofSoundStream::getTickCount(){
	if( soundStream ){
		soundStream->getTickCount();
	}
}

