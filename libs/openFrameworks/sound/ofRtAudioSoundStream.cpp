#include "ofRtAudioSoundStream.h"

#ifdef OF_SOUNDSTREAM_RTAUDIO
#include "ofSoundStream.h"
#include "ofMath.h"
#include "ofUtils.h"
#include "RtAudio.h"


//------------------------------------------------------------------------------
int receiveAudioBufferAndCallSimpleApp(void *outputBuf, void *inputBuf, unsigned int nFramesPerBuffer,
           double streamTime, RtAudioStreamStatus status, void *data);


//------------------------------------------------------------------------------
ofRtAudioSoundStream::ofRtAudioSoundStream(){
	outDeviceID		= -1;
    inDeviceID		= -1;
	soundOutputPtr	= NULL;
	soundInputPtr	= NULL;
	nInputChannels = 0;
	nOutputChannels = 0;
	tickCount= 0;
	newBuffersNeededForOutput = true;
	newBuffersNeededForInput = true;


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
		std::cout << "device = " << i << " (" << info.name << ")\n";
		if (info.isDefaultInput) std::cout << "----* default ----* \n";
		std::cout << "maximum output channels = " << info.outputChannels << "\n";
		std::cout << "maximum input channels = " << info.inputChannels << "\n";
		std::cout << "-----------------------------------------\n";

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
	newBuffersNeededForInput = true;

}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::setOutput(ofBaseSoundOutput * soundOutput){
	soundOutputPtr		= soundOutput;
	newBuffersNeededForOutput = true;
}

//------------------------------------------------------------------------------
bool ofRtAudioSoundStream::setup(int outChannels, int inChannels, int _sampleRate, int _bufferSize, int _nBuffers){
	if( audio != NULL ){
		close();
	}

	nInputChannels		= inChannels;
	nOutputChannels		= outChannels;
	nBuffers			= _nBuffers;
	

	sampleRate			=  _sampleRate;
	tickCount			=  0;
	nFramesPerBuffer			= ofNextPow2(_bufferSize);	// must be pow2

	inputBuffer.setSampleRate(sampleRate);
	outputBuffer.setSampleRate(sampleRate);

	try {
		audio = ofPtr<RtAudio>(new RtAudio());
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

	unsigned int bufferFrames = (unsigned int)nFramesPerBuffer; // 256 sample frames

	RtAudio::StreamOptions options;
	options.flags = RTAUDIO_SCHEDULE_REALTIME;
	options.numberOfBuffers = nBuffers;
	options.priority = 1;

	try {
		audio ->openStream( (nOutputChannels>0)?&outputParameters:NULL, (nInputChannels>0)?&inputParameters:NULL, RTAUDIO_FLOAT32,
							sampleRate, &bufferFrames, &rtAudioCallback, this, &options);
	} catch (RtError &error) {
		error.printMessage();
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool ofRtAudioSoundStream::setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int nFramesPerBuffer, int nBuffers){
	setInput(app);
	setOutput(app);
	return setup(outChannels,inChannels,sampleRate,nFramesPerBuffer,nBuffers);
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
	return nFramesPerBuffer;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::rtAudioCallback(void *outputBuf, void *inputBuf, unsigned int nFramesPerBuffer, double streamTime, RtAudioStreamStatus status, void *data){
	ofRtAudioSoundStream * rtStreamPtr = (ofRtAudioSoundStream *)data;
	
	if ( status ) std::cout << "Stream over/underflow detected." << std::endl;

	// 	rtAudio uses a system by which the audio
	// 	can be of different formats
	// 	char, float, etc.
	// 	we choose float
	float * fPtrOut = (float *)outputBuf;
	float * fPtrIn  = (float *)inputBuf;
	// [zach] memset output to zero before output call
	// this is because of how rtAudio works: duplex w/ one callback
	// you need to cut in the middle. if the simpleApp
	// doesn't produce audio, we pass silence instead of duplex...
	
	int nInputChannels = rtStreamPtr->getNumInputChannels();
	int nOutputChannels = rtStreamPtr->getNumOutputChannels();

	
	if(nInputChannels > 0){
		if( rtStreamPtr->soundInputPtr != NULL ){
			if (rtStreamPtr->newBuffersNeededForInput){
				rtStreamPtr->soundInputPtr->audioInBuffersChanged( nFramesPerBuffer, nInputChannels, rtStreamPtr->sampleRate );
				rtStreamPtr->newBuffersNeededForInput=false;
			}
			rtStreamPtr->inputBuffer.set( fPtrIn, nFramesPerBuffer, nInputChannels );
			rtStreamPtr->applySoundStreamOriginInfo(&rtStreamPtr->inputBuffer);
			rtStreamPtr->soundInputPtr->audioIn(rtStreamPtr->inputBuffer);
			//rtStreamPtr->soundInputPtr->audioIn(fPtrIn, nFramesPerBuffer, nInputChannels, rtStreamPtr->inDeviceID, rtStreamPtr->tickCount );
		}
		// [damian] not sure what this is for? assuming it's for underruns? or for when the sound system becomes broken?
		memset(fPtrIn, 0, nFramesPerBuffer * nInputChannels * sizeof(float));
	}

	if (nOutputChannels > 0) {
		if( rtStreamPtr->soundOutputPtr != NULL ){
			// send 
			if (rtStreamPtr->newBuffersNeededForOutput){
				rtStreamPtr->soundOutputPtr->audioOutBuffersChanged( nFramesPerBuffer, nOutputChannels, rtStreamPtr->sampleRate );
				rtStreamPtr->newBuffersNeededForOutput=false;
			}
			if ( rtStreamPtr->outputBuffer.size() != nFramesPerBuffer*nOutputChannels || rtStreamPtr->outputBuffer.getNumChannels()!=nOutputChannels ){
				rtStreamPtr->outputBuffer.setNumChannels(nOutputChannels);
				rtStreamPtr->outputBuffer.resize(nFramesPerBuffer*nOutputChannels);
			}
			rtStreamPtr->applySoundStreamOriginInfo(&rtStreamPtr->outputBuffer);
			rtStreamPtr->soundOutputPtr->audioOut(rtStreamPtr->outputBuffer);
			//rtStreamPtr->soundOutputPtr->audioOut(fPtrOut, nFramesPerBuffer, nOutputChannels, rtStreamPtr->outDeviceID, rtStreamPtr->tickCount );
		}
		rtStreamPtr->outputBuffer.copyTo(fPtrOut, nFramesPerBuffer, nOutputChannels,0);
		rtStreamPtr->outputBuffer.set(0);
	}
	
	// increment tick count
	rtStreamPtr->tickCount++;

	return 0;
}
#endif
