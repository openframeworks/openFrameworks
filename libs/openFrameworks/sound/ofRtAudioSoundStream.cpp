#include "ofRtAudioSoundStream.h"

#include "ofSoundStream.h"
#include "RtAudio.h"
#include "ofMath.h"
#include "ofUtils.h"


//------------------------------------------------------------------------------
int receiveAudioBufferAndCallSimpleApp(void *outputBuffer, void *inputBuffer, unsigned int bufferSize,
           double streamTime, RtAudioStreamStatus status, void *data);


//------------------------------------------------------------------------------
ofRtAudioSoundStream::ofRtAudioSoundStream(){
	deviceID		= -1;
	audio			= NULL;
	soundOutputPtr	= NULL;
	soundInputPtr	= NULL;
	tickCount= 0;
}

//------------------------------------------------------------------------------
ofRtAudioSoundStream::~ofRtAudioSoundStream(){
	stop();
	close();
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::listDevices(){	
	RtAudio *audioTemp = 0;
	try {
		audioTemp = new RtAudio();
	} catch (RtError &error) {
		error.printMessage();
	}
 	int devices = audioTemp->getDeviceCount();
	RtAudio::DeviceInfo info;
	for (int i=0; i< devices; i++) {
		try {
			info = audioTemp->getDeviceInfo(i);
		} catch (RtError &error) {
			error.printMessage();
			break;
		}
		std::cout << "device = " << i << " (" << info.name << ")\n";
		if (info.isDefaultInput) std::cout << "----* default ----* \n";
		std::cout << "maximum output channels = " << info.outputChannels << "\n";
		std::cout << "maximum input channels = " << info.inputChannels << "\n";
		std::cout << "-----------------------------------------\n";

	}
	delete audioTemp;
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::setDeviceID(int _deviceID){
	deviceID = _deviceID;
}

//------------------------------------------------------------------------------
bool ofRtAudioSoundStream::setupInput(ofBaseSoundInput * soundInput, int numChannels, int _sampleRate, int bufferSize, int nBuffers){	
	if( audio != NULL ){
		close();
	}
	
	soundInputPtr		= soundInput;
	
	nInputChannels		=  numChannels;		
	nOutputChannels		= 0;
	
	sampleRate			=  _sampleRate;
	tickCount			=  0;
	bufferSize			= ofNextPow2(bufferSize);	// must be pow2
	
	try {
		audio = new RtAudio();
	}	catch (RtError &error) {
		error.printMessage();
		return false;
	}

	RtAudio::StreamParameters * outputParameters=NULL;

	RtAudio::StreamParameters * inputParameters = NULL;
	if(nInputChannels>0){
		inputParameters = new RtAudio::StreamParameters;
		if( deviceID >= 0 ){
			inputParameters->deviceId = deviceID;
		}else{
			inputParameters->deviceId = audio->getDefaultInputDevice();		
		}
		inputParameters->nChannels = nInputChannels;
	}

	unsigned int bufferFrames = (unsigned int)bufferSize; // 256 sample frames

	RtAudio::StreamOptions options;
	options.flags = RTAUDIO_SCHEDULE_REALTIME;
	options.numberOfBuffers = nBuffers;
	options.priority = 1;

	try {
		audio ->openStream( outputParameters, inputParameters, RTAUDIO_FLOAT32,
							sampleRate, &bufferFrames, &receiveAudioBufferAndCallSimpleApp, this, &options);
		audio->startStream();
	} catch (RtError &error) {
		error.printMessage();
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool ofRtAudioSoundStream::setupOutput(ofBaseSoundOutput * soundOutput, int numChannels, int _sampleRate, int bufferSize, int nBuffers){	
	if( audio != NULL ){
		close();
	}
	
	soundOutputPtr		= soundOutput;
	
	nOutputChannels 	=  numChannels;	
	nInputChannels		= 0;
	sampleRate			=  _sampleRate;
	tickCount			=  0;
	bufferSize			= ofNextPow2(bufferSize);	// must be pow2
	
	try {
		audio = new RtAudio();
	}	catch (RtError &error) {
		error.printMessage();
		return false;
	}

	RtAudio::StreamParameters * outputParameters=NULL;
	if(nOutputChannels >0){
		outputParameters = new RtAudio::StreamParameters();
		if( deviceID >= 0 ){
			outputParameters->deviceId = deviceID;
		}else{
			outputParameters->deviceId = audio->getDefaultOutputDevice();		
		}
		outputParameters->nChannels = nOutputChannels;
	}

	RtAudio::StreamParameters * inputParameters = NULL;

	unsigned int bufferFrames = (unsigned int)bufferSize; // 256 sample frames

	RtAudio::StreamOptions options;
	options.flags = RTAUDIO_SCHEDULE_REALTIME;
	options.numberOfBuffers = nBuffers;
	options.priority = 1;

	try {
		audio ->openStream( outputParameters, inputParameters, RTAUDIO_FLOAT32,
							sampleRate, &bufferFrames, &receiveAudioBufferAndCallSimpleApp, this, &options);
		audio->startStream();
	} catch (RtError &error) {
		error.printMessage();
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::start(){
	if( audio == NULL )return;
	
	try{
		audio->startStream();
	} catch (RtError &error) {
		error.printMessage();
	}
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::stop(){
	if( audio == NULL )return;
	
	try {
    	audio->stopStream();
  	} catch (RtError &error) {
   		error.printMessage();
 	}
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::close(){
	if(audio == NULL) return;
	
	try {
    	audio->closeStream();
  	} catch (RtError &error) {
   		error.printMessage();
 	}
	delete audio;
	audio = NULL;
	soundOutputPtr	= NULL;
	soundInputPtr	= NULL;	
}

//------------------------------------------------------------------------------
long unsigned long ofRtAudioSoundStream::getTickCount(){
	return tickCount;
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::incrementTickCount(){
	tickCount++;
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::audioIn(float * buffer, int bufferSize, int nChannels){
	if( soundInputPtr != NULL ){
		soundInputPtr->audioIn(buffer, bufferSize, nChannels);
	}
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::audioOut(float * buffer, int bufferSize, int nChannels){
	if( soundOutputPtr != NULL ){
		soundOutputPtr->audioOut(buffer, bufferSize, nChannels);
	}
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getNumInputChannels(){
	return nInputChannels;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getNumOutputChannels(){
	return nOutputChannels;		
}

//------------------------------------------------------------------------------
int receiveAudioBufferAndCallSimpleApp(void *outputBuffer, void *inputBuffer, unsigned int bufferSize, double streamTime, RtAudioStreamStatus status, void *data){
	ofRtAudioSoundStream * rtStreamPtr = (ofRtAudioSoundStream *)data;
	
	if ( status ) std::cout << "Stream over/underflow detected." << std::endl;

	// 	rtAudio uses a system by which the audio
	// 	can be of different formats
	// 	char, float, etc.
	// 	we choose float
	float * fPtrOut = (float *)outputBuffer;
	float * fPtrIn  = (float *)inputBuffer;
	// [zach] memset output to zero before output call
	// this is because of how rtAudio works: duplex w/ one callback
	// you need to cut in the middle. if the simpleApp
	// doesn't produce audio, we pass silence instead of duplex...
	
	int nInputChannels = rtStreamPtr->getNumInputChannels();
	int nOutputChannels = rtStreamPtr->getNumOutputChannels();

	if(nInputChannels > 0){
		rtStreamPtr->audioIn( fPtrIn, bufferSize, nInputChannels);
		memset(fPtrIn, 0, bufferSize * nInputChannels * sizeof(float));
	}

	if (nOutputChannels > 0) {
		memset(fPtrOut, 0, sizeof(float) * bufferSize * nOutputChannels);
		rtStreamPtr->audioOut( fPtrOut, bufferSize, nOutputChannels );
	}
	
	// increment tick count
	rtStreamPtr->incrementTickCount();	

	return 0;
}

