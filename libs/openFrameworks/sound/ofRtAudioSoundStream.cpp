#include "ofRtAudioSoundStream.h"
#include "ofConstants.h"

#ifdef OF_SOUNDSTREAM_RTAUDIO
#include "ofSoundStream.h"
#include "ofMath.h"
#include "ofUtils.h"
#include "RtAudio.h"


//------------------------------------------------------------------------------
ofRtAudioSoundStream::ofRtAudioSoundStream(){
	outDeviceID		= -1;
    inDeviceID		= -1;
	soundOutputPtr	= NULL;
	soundInputPtr	= NULL;
	tickCount= 0;
	nOutputChannels = 0;
	nInputChannels = 0;
	bufferSize = 0;
	sampleRate = 0;
}

//------------------------------------------------------------------------------
ofRtAudioSoundStream::~ofRtAudioSoundStream(){
	close();
}

//------------------------------------------------------------------------------
vector<ofSoundDevice> ofRtAudioSoundStream::getDeviceList() const{
	shared_ptr<RtAudio> audioTemp;
	try {
		audioTemp = shared_ptr<RtAudio>(new RtAudio());
	} catch (RtError &error) {
		error.printMessage();
		return vector<ofSoundDevice>();
	}
 	int deviceCount = audioTemp->getDeviceCount();
	RtAudio::DeviceInfo info;
	vector<ofSoundDevice> deviceList;
	for (int i=0; i< deviceCount; i++) {
		try {
			info = audioTemp->getDeviceInfo(i);
		} catch (RtError &error) {
			ofLogError("ofRtAudioSoundStream") << "Error retrieving info for device " << i;
			error.printMessage();
			break;
		}
		
		ofSoundDevice dev;
		dev.deviceID = i;
		dev.name = info.name;
		dev.outputChannels = info.outputChannels;
		dev.inputChannels = info.inputChannels;
		dev.sampleRates = info.sampleRates;
		dev.isDefaultInput = info.isDefaultInput;
		dev.isDefaultOutput = info.isDefaultOutput;
		deviceList.push_back(dev);
	}
	
	return deviceList;
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::setDeviceID(int _deviceID){
    inDeviceID = outDeviceID = _deviceID;
}

int ofRtAudioSoundStream::getDeviceID()  const{
	return inDeviceID;
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
		audio = shared_ptr<RtAudio>(new RtAudio());
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
	outputBuffer.setDeviceID(outDeviceID);
	inputBuffer.setDeviceID(inDeviceID);

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
	audio.reset();	// delete
}

//------------------------------------------------------------------------------
long unsigned long ofRtAudioSoundStream::getTickCount() const{
	return tickCount;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getNumInputChannels() const{
	return nInputChannels;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getNumOutputChannels() const{
	return nOutputChannels;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getSampleRate() const{
	return sampleRate;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getBufferSize() const{
	return bufferSize;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::rtAudioCallback(void *outputBuffer, void *inputBuffer, unsigned int nFramesPerBuffer, double streamTime, RtAudioStreamStatus status, void *data){
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
			rtStreamPtr->inputBuffer.copyFrom(fPtrIn, nFramesPerBuffer, nInputChannels, rtStreamPtr->getSampleRate());
			rtStreamPtr->inputBuffer.setTickCount(rtStreamPtr->tickCount);
			rtStreamPtr->soundInputPtr->audioIn(rtStreamPtr->inputBuffer);
		}
		// [damian] not sure what this is for? assuming it's for underruns? or for when the sound system becomes broken?
		memset(fPtrIn, 0, nFramesPerBuffer * nInputChannels * sizeof(float));
	}
	
	if (nOutputChannels > 0) {
		if( rtStreamPtr->soundOutputPtr != NULL ){
			
			if ( rtStreamPtr->outputBuffer.size() != nFramesPerBuffer*nOutputChannels || rtStreamPtr->outputBuffer.getNumChannels()!=nOutputChannels ){
				rtStreamPtr->outputBuffer.setNumChannels(nOutputChannels);
				rtStreamPtr->outputBuffer.resize(nFramesPerBuffer*nOutputChannels);
			}
			rtStreamPtr->outputBuffer.setTickCount(rtStreamPtr->tickCount);
			rtStreamPtr->soundOutputPtr->audioOut(rtStreamPtr->outputBuffer);
		}
		rtStreamPtr->outputBuffer.copyTo(fPtrOut, nFramesPerBuffer, nOutputChannels,0);
		rtStreamPtr->outputBuffer.set(0);
	}
	
	// increment tick count
	rtStreamPtr->tickCount++;
	
	return 0;
}
#endif
