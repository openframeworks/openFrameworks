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
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, int sampleRate, int nFramesPerBuffer, int nBuffers){
	ofSoundStreamSetup(nOutputChannels, nInputChannels, ofGetAppPtr(), sampleRate, nFramesPerBuffer, nBuffers);
}

//------------------------------------------------------------
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * appPtr, int sampleRate, int nFramesPerBuffer, int nBuffers){
	ofSoundStreamGetSystemStream()->setup(appPtr, nOutputChannels, nInputChannels, sampleRate, nFramesPerBuffer, nBuffers);
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
	started = false;
	inputPtr = NULL;
	outputPtr = NULL;
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
		// tear down the existing stream if necessary, storing parameters and state
		bool wasSetup = false;
		bool wasStarted = started;
		int outChannels, inChannels, sampleRate, nFramesPerBuffer, nBuffers;
		if (isSetup()){
			wasSetup = true;
			outChannels = getNumOutputChannels();
			inChannels = getNumInputChannels();
			sampleRate = getSampleRate();
			nFramesPerBuffer = getNumFramesPerBuffer();
			nBuffers = getNumBuffers();
			close();
		}
		
		// switch device ids
		soundStream->setDeviceID(deviceID);
		
		// setup the stream again, if necessary, with the same parameters
		// inputPtr and outputPtr are preserved over this call to setupInternal
		if (wasSetup){
			setupInternal( outChannels, inChannels, sampleRate, nFramesPerBuffer, nBuffers, wasStarted );
		}
	}
}

int ofSoundStream::getDeviceID(){
	if (soundStream){
		return soundStream->getDeviceID();
	}
	return 0;
}

//------------------------------------------------------------
void ofSoundStream::setInput(ofBaseSoundInput * soundInput){
	if( soundStream ){
		// setup the system stream if necessary
		if (getNumInputChannels()==0){
			setupInput();
		}
		soundStream->setInput(soundInput);
		inputPtr = soundInput;
	}
}

//------------------------------------------------------------
void ofSoundStream::setOutput(ofBaseSoundOutput * soundOutput){
	if( soundStream ){
		if (getNumOutputChannels()==0){
			setupOutput();
		}
		soundStream->setOutput(soundOutput);
		outputPtr = soundOutput;
	}
}

//------------------------------------------------------------
bool ofSoundStream::isSetup(){
	return getNumInputChannels()!=0 || getNumOutputChannels()!=0;
}


//------------------------------------------------------------
bool ofSoundStream::setupInput( int nChannels, int sampleRate, int nFramesPerBuffer, int nBuffers){
	if ( isSetup() ){
		if (sampleRate == -1){
			sampleRate = getSampleRate();
		}
		if (nFramesPerBuffer==-1){
			nFramesPerBuffer = getNumFramesPerBuffer();
		}
		if (nBuffers==-1){
			nBuffers = getNumBuffers();
		}
	}else{
		if (sampleRate == -1){
			sampleRate = DEFAULT_SAMPLE_RATE;
		}
		if (nFramesPerBuffer==-1){
			nFramesPerBuffer = DEFAULT_BUFFER_SIZE;
		}
		if (nBuffers==-1){
			nBuffers = DEFAULT_NUM_BUFFERS;
		}
	}
	// try to be clever about preserving input/output pointers

	bool success = setup( getNumOutputChannels(), nChannels, sampleRate, nFramesPerBuffer, nBuffers );
	return success;
}

//------------------------------------------------------------
bool ofSoundStream::setupOutput( int nChannels, int sampleRate, int nFramesPerBuffer, int nBuffers ){
	if ( isSetup() ){
		if (sampleRate == -1){
			sampleRate = getSampleRate();
		}
		if (nFramesPerBuffer==-1){
			nFramesPerBuffer = getNumFramesPerBuffer();
		}
		if (nBuffers==-1){
			nBuffers = getNumBuffers();
		}
	}else{
		if (sampleRate == -1){
			sampleRate = DEFAULT_SAMPLE_RATE;
		}
		if (nFramesPerBuffer==-1){
			nFramesPerBuffer = DEFAULT_BUFFER_SIZE;
		}
		if (nBuffers==-1){
			nBuffers = DEFAULT_NUM_BUFFERS;
		}
	}
	bool success = setup( nChannels, getNumInputChannels(), sampleRate, nFramesPerBuffer, nBuffers );
	return success;
}

//------------------------------------------------------------
bool ofSoundStream::setup(int outChannels, int inChannels, int sampleRate, int nFramesPerBuffer, int nBuffers ){
	return setupInternal( outChannels, inChannels, sampleRate, nFramesPerBuffer, nBuffers, true );
}

//------------------------------------------------------------
bool ofSoundStream::setup(ofBaseApp *app, int outChannels, int inChannels, int sampleRate, int nFramesPerBuffer, int nBuffers) {
	return setupInternal( app, outChannels, inChannels, sampleRate, nFramesPerBuffer, nBuffers, true );
}


//------------------------------------------------------------
bool ofSoundStream::setupInternal(int outChannels, int inChannels, int sampleRate, int nFramesPerBuffer, int nBuffers, bool autoStart ){
	if( soundStream ){
		if (isSetup()){
			close();
		}
		ofBaseSoundOutput* prevOutputPtr = outputPtr;
		ofBaseSoundInput* prevInputPtr = inputPtr;
		bool success = soundStream->setup(outChannels, inChannels, sampleRate, nFramesPerBuffer, nBuffers);
		if ( success ){
			if (prevOutputPtr){
				soundStream->setOutput(prevOutputPtr);
				outputPtr = prevOutputPtr;
			}
			if (prevInputPtr){
				soundStream->setInput(prevInputPtr);
				inputPtr = prevInputPtr;
			}
			if (autoStart){
				start();
			}
		}
		
		return success;
	}
	return false;
}

//------------------------------------------------------------
bool ofSoundStream::setupInternal(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int nFramesPerBuffer, int nBuffers, bool autoStart){
	if( soundStream ){
		if (isSetup()){
			close();
		}
		bool success = soundStream->setup(app, outChannels, inChannels, sampleRate, nFramesPerBuffer, nBuffers);
		if (success && autoStart){
			start();
		}
		return success;
	}
	return false;
}

//------------------------------------------------------------
void ofSoundStream::start(){
	if( soundStream ){
		if (!started){
			soundStream->start();
			started = true;
		}
		
	}
}

//------------------------------------------------------------
void ofSoundStream::stop(){
	if( soundStream ){
		if (started){
			soundStream->stop();
			started = false;
		}
	}
}

//------------------------------------------------------------
void ofSoundStream::close(){
	if( soundStream ){
		soundStream->close();
		started = false;
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
int ofSoundStream::getNumFramesPerBuffer(){
	if( soundStream ){
		return soundStream->getNumFramesPerBuffer();
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
