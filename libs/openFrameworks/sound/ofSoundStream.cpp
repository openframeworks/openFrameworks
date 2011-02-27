#include "ofSoundStream.h"

//------------------------------------------------------------
ofSoundStream::ofSoundStream(){
	soundStream = NULL;
	setSoundStream( new OF_SOUND_STREAM_TYPE );
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
bool ofSoundStream::setup(ofSoundStreamMode soundStreamMode, ofBaseHasSoundStream * hasSoundStream, int numChannels, int sampleRate, int bufferSize, int nBuffers){
	if( soundStream ){
		soundStream->setup(soundStreamMode, hasSoundStream, numChannels, sampleRate, bufferSize, nBuffers);
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

