#include "ofRtAudioSoundStream.h"
#include "ofConstants.h"

#ifdef OF_SOUNDSTREAM_RTAUDIO
#include "ofSoundStream.h"
#include "ofMath.h"
#include "ofUtils.h"
#include "RtAudio.h"


//------------------------------------------------------------------------------
int receiveAudioBufferAndCallSimpleApp(void *outputBuffer, void *inputBuffer, unsigned int bufferSize,
           double streamTime, RtAudioStreamStatus status, void *data);


//------------------------------------------------------------------------------
ofRtAudioSoundStream::ofRtAudioSoundStream(){
	outDeviceID		= -1;
    inDeviceID		= -1;
	soundOutputPtr	= NULL;
	soundInputPtr	= NULL;
	tickCount= 0;
}

//------------------------------------------------------------------------------
ofRtAudioSoundStream::~ofRtAudioSoundStream(){
	close();
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::listDevices(){
	ofPtr<RtAudio> audioTemp;
	try {
		audioTemp = ofPtr<RtAudio>(new RtAudio());
	} catch (RtError &error) {
		error.printMessage();
		return;
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
		ofLogNotice("ofRtAudioSoundStream") << "device " << i << " " << info.name << "";
		if (info.isDefaultInput) ofLogNotice("ofRtAudioSoundStream") << "----* default ----*";
		ofLogNotice("ofRtAudioSoundStream") << "maximum output channels " << info.outputChannels;
		ofLogNotice("ofRtAudioSoundStream") << "maximum input channels " << info.inputChannels;
		ofLogNotice("ofRtAudioSoundStream") << "-----------------------------------------";
	}
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::setDeviceID(int _deviceID){
	//deviceID = _deviceID;
    inDeviceID = outDeviceID = _deviceID;
}

void ofRtAudioSoundStream::setInDeviceID(int _deviceID){
	inDeviceID = _deviceID;
}

void ofRtAudioSoundStream::setOutDeviceID(int _deviceID){
	outDeviceID = _deviceID;
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::setInput(ofBaseSoundInput * soundInput){
	soundInputPtr		= soundInput;
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::setOutput(ofBaseSoundOutput * soundOutput){
	soundOutputPtr		= soundOutput;
}

//------------------------------------------------------------------------------
bool ofRtAudioSoundStream::setup(int outChannels, int inChannels, int _sampleRate, int _bufferSize, int nBuffers){
	if( audio != NULL ){
		close();
	}

	nInputChannels		= inChannels;
	nOutputChannels		= outChannels;

	sampleRate			=  _sampleRate;
	tickCount			=  0;
	bufferSize			= ofNextPow2(_bufferSize);	// must be pow2

	try {
		audio = ofPtr<RtAudio>(new RtAudio);
	}	catch (RtError &error) {
		error.printMessage();
		return false;
	}

	RtAudio::StreamParameters outputParameters;
	RtAudio::StreamParameters inputParameters;
	if(nInputChannels>0){
		if( inDeviceID >= 0 ){
			inputParameters.deviceId = inDeviceID;
		}else{
			inputParameters.deviceId = audio->getDefaultInputDevice();
		}
		inputParameters.nChannels = nInputChannels;
	}

	if(nOutputChannels>0){
		if( outDeviceID >= 0 ){
			outputParameters.deviceId = outDeviceID;
		}else{
			outputParameters.deviceId = audio->getDefaultOutputDevice();
		}
		outputParameters.nChannels = nOutputChannels;
	}

	unsigned int bufferFrames = (unsigned int)bufferSize; // 256 sample frames

	RtAudio::StreamOptions options;
	options.flags = RTAUDIO_SCHEDULE_REALTIME;
	options.numberOfBuffers = nBuffers;
	options.priority = 1;

	try {
		audio ->openStream( (nOutputChannels>0)?&outputParameters:NULL, (nInputChannels>0)?&inputParameters:NULL, RTAUDIO_FLOAT32,
							sampleRate, &bufferFrames, &rtAudioCallback, this, &options);
		audio->startStream();
	} catch (RtError &error) {
		error.printMessage();
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool ofRtAudioSoundStream::setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers){
	setInput(app);
	setOutput(app);
	return setup(outChannels,inChannels,sampleRate,bufferSize,nBuffers);
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::start(){
	if( audio == NULL ) return;

	try{
		audio->startStream();
	} catch (RtError &error) {
		error.printMessage();
	}
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::stop(){
	if( audio == NULL ) return;

	try {
		if(audio->isStreamRunning()) {
    		audio->stopStream();
		}
  	} catch (RtError &error) {
   		error.printMessage();
 	}
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::close(){
	if( audio == NULL ) return;

	try {
		if(audio->isStreamOpen()) {
    		audio->closeStream();
		}
  	} catch (RtError &error) {
   		error.printMessage();
 	}
	soundOutputPtr	= NULL;
	soundInputPtr	= NULL;
	audio = ofPtr<RtAudio>();	// delete
}

//------------------------------------------------------------------------------
long unsigned long ofRtAudioSoundStream::getTickCount(){
	return tickCount;
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
int ofRtAudioSoundStream::getSampleRate(){
	return sampleRate;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getBufferSize(){
	return bufferSize;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::rtAudioCallback(void *outputBuffer, void *inputBuffer, unsigned int bufferSize, double streamTime, RtAudioStreamStatus status, void *data){
	ofRtAudioSoundStream * rtStreamPtr = (ofRtAudioSoundStream *)data;

	if ( status ) {
		ofLogWarning("ofRtAudioSoundStream") << "stream over/underflow detected";
	}

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
		if( rtStreamPtr->soundInputPtr != NULL ){
			rtStreamPtr->soundInputPtr->audioIn((float*)inputBuffer, bufferSize, nInputChannels, rtStreamPtr->inDeviceID, rtStreamPtr->tickCount);
		}
		memset(fPtrIn, 0, bufferSize * nInputChannels * sizeof(float));
	}

	if (nOutputChannels > 0) {
		memset(fPtrOut, 0, sizeof(float) * bufferSize * nOutputChannels);
		if( rtStreamPtr->soundOutputPtr != NULL ){
			rtStreamPtr->soundOutputPtr->audioOut((float*)outputBuffer, bufferSize, nOutputChannels, rtStreamPtr->outDeviceID, rtStreamPtr->tickCount);
		}
	}

	// increment tick count
	rtStreamPtr->tickCount++;

	return 0;
}
#endif
