#include "ofSoundStream.h"
#include "ofAppRunner.h"


static const int DEFAULT_SAMPLE_RATE = 44100;
// default to 512*4=2048 frames or 21.5ms buffer at 44100Hz
static const int DEFAULT_BUFFER_SIZE = 512;
static const int DEFAULT_NUM_BUFFERS = 4;


ofSoundStream* ofSoundStreamGetSystemStream() {
	static ofSoundStream* systemStream = new ofSoundStream();
	return systemStream;
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
	ofSoundStreamGetSystemStream()->setup(appPtr, nOutputChannels, nInputChannels, sampleRate, bufferSize, nBuffers);
}

//------------------------------------------------------------
void ofSoundStreamStop(){
	ofSoundStreamGetSystemStream()->stop();
}

//------------------------------------------------------------
void ofSoundStreamStart(){
	ofSoundStreamGetSystemStream()->start();
}

//------------------------------------------------------------
void ofSoundStreamClose(){
	ofSoundStreamGetSystemStream()->close();
}

//------------------------------------------------------------
void ofSoundStreamListDevices(){
	ofSoundStreamGetSystemStream()->listDevices();
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
		bool wasSetup = false;
		int outChannels, inChannels, sampleRate, bufferSize, nBuffers;
		if (isSetup()){
			wasSetup = true;
			outChannels = getNumOutputChannels();
			inChannels = getNumInputChannels();
			sampleRate = getSampleRate();
			bufferSize = getBufferSize();
			nBuffers = getNumBuffers();
			close();
		}
		
		soundStream->setDeviceID(deviceID);
		
		if (wasSetup){
			setup( outChannels, inChannels, sampleRate, bufferSize, nBuffers );
		}
	}
}

//------------------------------------------------------------
void ofSoundStream::setInput(ofBaseSoundInput * soundInput){
	if( soundStream ){
		if (getNumInputChannels()==0){
			setupInput();
		}
		soundStream->setInput(soundInput);
	}
}

//------------------------------------------------------------
void ofSoundStream::setOutput(ofBaseSoundOutput * soundOutput){
	if( soundStream ){
		if (getNumOutputChannels()==0){
			setupOutput();
		}
		soundStream->setOutput(soundOutput);
	}
}

//------------------------------------------------------------
bool ofSoundStream::isSetup(){
	return getNumInputChannels()!=0 || getNumOutputChannels()!=0;
}


//------------------------------------------------------------
bool ofSoundStream::setupInput( int nChannels, int sampleRate, int bufferSize, int nBuffers ){
	if ( isSetup() ){
		sampleRate = getSampleRate();
		bufferSize = getBufferSize();
		nBuffers = getNumBuffers();
	}else{
		if (sampleRate == -1){
			sampleRate = DEFAULT_SAMPLE_RATE;
		}
		if (bufferSize==-1){
			bufferSize = DEFAULT_BUFFER_SIZE;
		}
		if (nBuffers==-1){
			nBuffers = DEFAULT_NUM_BUFFERS;
		}
	}
	return setup( getNumOutputChannels(), nChannels, sampleRate, bufferSize, nBuffers );
}

//------------------------------------------------------------
bool ofSoundStream::setupOutput( int nChannels, int sampleRate, int bufferSize, int nBuffers ){
	if ( isSetup() ){
		sampleRate = getSampleRate();
		bufferSize = getBufferSize();
		nBuffers = getNumBuffers();
	}else{
		if (sampleRate == -1){
			sampleRate = DEFAULT_SAMPLE_RATE;
		}
		if (bufferSize==-1){
			bufferSize = DEFAULT_BUFFER_SIZE;
		}
		if (nBuffers==-1){
			nBuffers = DEFAULT_NUM_BUFFERS;
		}
	}
	return setup( nChannels, getNumInputChannels(), sampleRate, bufferSize, nBuffers );
}

//------------------------------------------------------------
bool ofSoundStream::setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers){
	if( soundStream ){
		if ( isSetup() )
			close();
		return soundStream->setup(outChannels, inChannels, sampleRate, bufferSize, nBuffers);
	}
	return false;
}

//------------------------------------------------------------
bool ofSoundStream::setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers){
	if( soundStream ){
		if ( isSetup() )
			close();
		return soundStream->setup(app, outChannels, inChannels, sampleRate, bufferSize, nBuffers);
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

//------------------------------------------------------------
int ofSoundStream::getNumBuffers(){
	if( soundStream ){
		return soundStream->getNumBuffers();
	}
	return 0;
}
