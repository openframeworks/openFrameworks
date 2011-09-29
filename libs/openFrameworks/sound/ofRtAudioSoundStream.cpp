#include "ofRtAudioSoundStream.h"

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
	deviceID		= -1;
	soundOutputPtr	= NULL;
	soundInputPtr	= NULL;
	tickCount= 0;
}

//------------------------------------------------------------------------------
ofRtAudioSoundStream::~ofRtAudioSoundStream(){
	close();
}

//---------------------------------------------------------
vector<ofStreamDeviceInfo> ofRtAudioSoundStream::listDevices(){

    vector<ofStreamDeviceInfo> deviceVec;

	RtAudio *audioTemp = 0;

	try {
		audioTemp = new RtAudio();
	} catch (RtError &error) {
		error.printMessage();
	}

 	int devices = audioTemp->getDeviceCount();

	RtAudio::DeviceInfo info;
	ofStreamDeviceInfo deviceInfo;

	for (int i=0; i< devices; i++) {
		try {
			info = audioTemp->getDeviceInfo(i);
		} catch (RtError &error) {
			error.printMessage();
			break;
		}

		deviceInfo.probed = info.probed;
        deviceInfo.name = info.name;
        deviceInfo.inputChannels = info.inputChannels;
        deviceInfo.outputChannels = info.outputChannels;
        deviceInfo.duplexChannels = info.duplexChannels;
        deviceInfo.isDefaultOutput = info.isDefaultOutput;
        deviceInfo.isDefaultInput = info.isDefaultInput;
        deviceInfo.sampleRates = info.sampleRates;
        //deviceInfo.nativeFormats = info.nativeFormats;

		deviceVec.push_back(deviceInfo);

		ofLog(OF_LOG_NOTICE) << "device = " << i << " (" << info.name << ")";
		if (info.isDefaultInput) ofLog(OF_LOG_NOTICE) << "----* default ----*";
		ofLog(OF_LOG_NOTICE) << "maximum output channels = " << info.outputChannels;
		ofLog(OF_LOG_NOTICE) << "maximum input channels = " << info.inputChannels;
		ofLog(OF_LOG_NOTICE) << "-----------------------------------------";

	}

	delete audioTemp;
	return deviceVec;

}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::setDeviceID(int _deviceID){
	deviceID = _deviceID;
	// TODO: return true or false on success and issue log notice and error where appropriate
}

//------------------------------------------------------------------------------
bool ofRtAudioSoundStream::setDeviceID(string _deviceName){
    vector<ofStreamDeviceInfo> deviceVec = listDevices(); //TODO: make the list store in a var and only do listing if it's not already done

    for(int i = 0; i < deviceVec.size(); i++) {
        ofStreamDeviceInfo deviceInfo = deviceVec[i];
        if (deviceInfo.name == _deviceName) {
            deviceID = i;
            ofLog(OF_LOG_NOTICE) << "Succesfully set device to: device " << deviceID << deviceInfo.name;
            return true;
        }
    }

    ofLog(OF_LOG_ERROR) << "Failed to set device to: " << _deviceName;
    return false;
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
bool ofRtAudioSoundStream::setup(int outChannels, int inChannels, int _sampleRate, int bufferSize, int nBuffers){
	if( audio != NULL ){
		close();
	}

	nInputChannels		= inChannels;
	nOutputChannels		= outChannels;

	sampleRate			=  _sampleRate;
	tickCount			=  0;
	bufferSize			= ofNextPow2(bufferSize);	// must be pow2

	try {
		audio = ofPtr<RtAudio>(new RtAudio());
	}	catch (RtError &error) {
		error.printMessage();
		return false;
	}

	RtAudio::StreamParameters outputParameters;
	RtAudio::StreamParameters inputParameters;
	if(nInputChannels>0){
		if( deviceID >= 0 ){
			inputParameters.deviceId = deviceID;
		}else{
			inputParameters.deviceId = audio->getDefaultInputDevice();
		}
		inputParameters.nChannels = nInputChannels;
	}

	if(nOutputChannels>0){
		if( deviceID >= 0 ){
			outputParameters.deviceId = deviceID;
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
int ofRtAudioSoundStream::rtAudioCallback(void *outputBuffer, void *inputBuffer, unsigned int bufferSize, double streamTime, RtAudioStreamStatus status, void *data){
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
		if( rtStreamPtr->soundInputPtr != NULL ){
			rtStreamPtr->soundInputPtr->audioIn((float*)inputBuffer, bufferSize, nInputChannels, rtStreamPtr->deviceID, rtStreamPtr->tickCount);
		}
		memset(fPtrIn, 0, bufferSize * nInputChannels * sizeof(float));
	}

	if (nOutputChannels > 0) {
		memset(fPtrOut, 0, sizeof(float) * bufferSize * nOutputChannels);
		if( rtStreamPtr->soundOutputPtr != NULL ){
			rtStreamPtr->soundOutputPtr->audioOut((float*)outputBuffer, bufferSize, nOutputChannels, rtStreamPtr->deviceID, rtStreamPtr->tickCount);
		}
	}

	// increment tick count
	rtStreamPtr->tickCount++;

	return 0;
}
#endif
