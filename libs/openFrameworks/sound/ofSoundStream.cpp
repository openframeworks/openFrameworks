#include "ofSoundStream.h"
#include "ofAppRunner.h"

static ofSoundStream globalSoundStream;	
ofSoundStream & ofGetSoundStream(){
    return globalSoundStream;
}
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
   globalSoundStream.setup(appPtr, nOutputChannels, nInputChannels, sampleRate, bufferSize, nBuffers);
    if (globalSoundStream.getOutput() != &ofGetSystemSoundMixer() ) {
        globalSoundStream.setOutput(&ofGetSystemSoundMixer());
    }
}

//------------------------------------------------------------
void ofSoundStreamStop(){
	globalSoundStream.stop();
}

//------------------------------------------------------------
void ofSoundStreamStart(){
	globalSoundStream.start();
}

//------------------------------------------------------------
void ofSoundStreamClose(){
	globalSoundStream.close();
}

//------------------------------------------------------------
void ofSoundStreamListDevices(){
	globalSoundStream.listDevices();
}

//------------------------------------------------------------
ofSoundStream::ofSoundStream(){
	#ifdef OF_SOUND_STREAM_TYPE
		setSoundStream( ofPtr<OF_SOUND_STREAM_TYPE>(new OF_SOUND_STREAM_TYPE) );
	#endif
}

//------------------------------------------------------------
void ofSoundStream::setSoundStream(ofPtr<ofBaseSoundStream> soundStreamPtr){
	soundStream = soundStreamPtr;
}

//------------------------------------------------------------
ofPtr<ofBaseSoundStream> ofSoundStream::getSoundStream(){
	return soundStream;
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
bool ofSoundStream::setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers){
	if( soundStream ){
		return soundStream->setup(app, outChannels, inChannels, sampleRate, bufferSize, nBuffers);
	}
	return false;
}

//------------------------------------------------------------
void ofSoundStream::setInput(ofBaseSoundInput * soundInput){
	if( soundStream ){
		soundStream->setInput(soundInput);
	}
}

//------------------------------------------------------------
void ofSoundStream::setOutput(ofBaseSoundOutput * soundOutput){
	if( soundStream ){
		soundStream->setOutput(soundOutput);
	}
}
//------------------------------------------------------------
ofBaseSoundInput * ofSoundStream::getInput(){
    if( soundStream ){
		return soundStream->getInput();
	}else{
        return NULL;
    }
}
//------------------------------------------------------------
ofBaseSoundOutput * ofSoundStream::getOutput(){
    if( soundStream ){
		return soundStream->getOutput();
	}else{
        return NULL;
    }
}
//------------------------------------------------------------
bool ofSoundStream::setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers){
	if( soundStream ){
		return soundStream->setup(outChannels, inChannels, sampleRate, bufferSize, nBuffers);
	}
	return false;
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
		return soundStream->getTickCount();
	}
	return 0;
}

//------------------------------------------------------------
int ofSoundStream::getNumInputChannels(){
	if( soundStream ){
		return soundStream->getNumInputChannels();
	}
	return 0;
}

//------------------------------------------------------------
int ofSoundStream::getNumOutputChannels(){
	if( soundStream ){
		return soundStream->getNumOutputChannels();
	}
	return 0;
}

//------------------------------------------------------------
int ofSoundStream::getSampleRate(){
	if( soundStream ){
		return soundStream->getSampleRate();
	}
	return 0;
}

//------------------------------------------------------------
int ofSoundStream::getBufferSize(){
	if( soundStream ){
		return soundStream->getBufferSize();
	}
	return 0;
}
