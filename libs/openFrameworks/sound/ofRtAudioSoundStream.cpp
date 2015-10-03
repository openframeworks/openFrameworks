#include "ofRtAudioSoundStream.h"

#ifdef OF_SOUNDSTREAM_RTAUDIO
#include "ofMath.h"
#include "ofUtils.h"
#include "ofAppRunner.h"


//------------------------------------------------------------------------------
ofRtAudioSoundStream::ofRtAudioSoundStream() {
	/*outDeviceID = -1;
	inDeviceID = -1;*/
	soundOutputPtr = nullptr;
	soundInputPtr = nullptr;
	tickCount = 0;
	/*nOutputChannels = 0;
	nInputChannels = 0;
	bufferSize = 0;
	sampleRate = 0;*/
	//setup(_settings);
}

//------------------------------------------------------------------------------
ofRtAudioSoundStream::~ofRtAudioSoundStream() {
	stop();
	close();
}

//------------------------------------------------------------------------------
vector<ofSoundDevice> ofRtAudioSoundStream::getDeviceList() const {
	shared_ptr<RtAudio> audioTemp;
	try {
		if (_settings.getApi() == RtAudio::Api::UNSPECIFIED) {
			audioTemp = std::make_shared<RtAudio>();
		}
		else {
			audioTemp = std::make_shared<RtAudio>(_settings.getApi());
		}
	}
	catch (std::exception &error) {
		ofLogError() << error.what();
		return vector<ofSoundDevice>();
	}
	int deviceCount = audioTemp->getDeviceCount();
	RtAudio::DeviceInfo info;
	vector<ofSoundDevice> deviceList;
	for (int i = 0; i < deviceCount; i++) {
		try {
			info = audioTemp->getDeviceInfo(i);
		}
		catch (std::exception &error) {
			ofLogError("ofRtAudioSoundStream") << "Error retrieving info for device " << i;
			ofLogError() << error.what();
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
void ofRtAudioSoundStream::setDeviceID(int _deviceID) {
	_settings.setInDeviceID(_deviceID);
	_settings.setOutDeviceID(_deviceID);
	//inDeviceID = outDeviceID = _deviceID;
}

int ofRtAudioSoundStream::getDeviceID()  const {
	return _settings.getInDeviceID();//inDeviceID;
}

void ofRtAudioSoundStream::setInDeviceID(int _deviceID) {
	_settings.setInDeviceID(_deviceID); 
	//inDeviceID = _deviceID;
}

void ofRtAudioSoundStream::setOutDeviceID(int _deviceID) {
	_settings.setOutDeviceID(_deviceID); 
	//outDeviceID = _deviceID;
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::setInput(ofBaseSoundInput * soundInput) {
	soundInputPtr = soundInput;
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::setOutput(ofBaseSoundOutput * soundOutput) {
	soundOutputPtr = soundOutput;
}

bool ofRtAudioSoundStream::setup(const ofRtSoundStreamSettings & settings)
{
	if (audio != nullptr) {
		close();
	}

	_settings = settings;

	/*nInputChannels = settings.getNumInputChannels();
	nOutputChannels = settings.getNumOutputChannels();

	sampleRate = settings.getSampleRate();*/
	tickCount = 0;
	//bufferSize = ofNextPow2(settings.getBufferSize());	// must be pow2

	try {
		if (settings.getApi() == RtAudio::Api::UNSPECIFIED) {
			audio = std::make_shared<RtAudio>();
		}
		else {
			audio = std::make_shared<RtAudio>(settings.getApi());
		}
	}
	catch (std::exception &error) {
		ofLogError() << error.what();
		return false;
	}

	RtAudio::StreamParameters outputParameters;
	RtAudio::StreamParameters inputParameters;
	if (settings.getNumInputChannels() > 0) {
		if (settings.getInDeviceID() >= 0) {
			inputParameters.deviceId = settings.getInDeviceID();
		}
		else {
			inputParameters.deviceId = audio->getDefaultInputDevice();
		}
		inputParameters.nChannels = settings.getNumInputChannels();
		setInput(ofGetAppPtr());
	}

	if (settings.getNumOutputChannels() > 0) {
		if (settings.getOutDeviceID() >= 0) {
			outputParameters.deviceId = settings.getOutDeviceID();
		}
		else {
			outputParameters.deviceId = audio->getDefaultOutputDevice();
		}
		outputParameters.nChannels = settings.getNumOutputChannels();
		setOutput(ofGetAppPtr());
	}

	unsigned int bufferFrames = (unsigned int)settings.getBufferSize(); // 256 sample frames

	RtAudio::StreamOptions options;
	options.flags = RTAUDIO_SCHEDULE_REALTIME;
	options.numberOfBuffers = settings.getNumBuffers();
	options.priority = 1;
	outputBuffer.setDeviceID(settings.getOutDeviceID());
	inputBuffer.setDeviceID(settings.getInDeviceID());

	try {
		audio->openStream((settings.getNumOutputChannels() > 0) ? &outputParameters : nullptr, (settings.getNumInputChannels() > 0) ? &inputParameters : nullptr, RTAUDIO_FLOAT32,
			settings.getSampleRate(), &bufferFrames, &rtAudioCallback, this, &options);
		audio->startStream();
	}
	catch (std::exception &error) {
		ofLogError() << error.what();
		return false;
	}
	return true;
}

////------------------------------------------------------------------------------
//bool ofRtAudioSoundStream::setup(int outChannels, int inChannels, int _sampleRate, int _bufferSize, int nBuffers){
//	return setup(outChannels, inChannels, _sampleRate, _bufferSize, nBuffers, RtAudio::Api::UNSPECIFIED);
//}
//
////------------------------------------------------------------------------------
//bool ofRtAudioSoundStream::setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers){
//	setInput(app);
//	setOutput(app);
//	return setup(outChannels,inChannels,sampleRate,bufferSize,nBuffers, RtAudio::Api::UNSPECIFIED);
//}
//
//bool ofRtAudioSoundStream::setup(int outChannels, int inChannels, int _sampleRate, int _bufferSize, int nBuffers, RtAudio::Api api)
//{
//	if (audio != nullptr) {
//		close();
//	}
//
//	nInputChannels = inChannels;
//	nOutputChannels = outChannels;
//
//	sampleRate = _sampleRate;
//	tickCount = 0;
//	bufferSize = ofNextPow2(_bufferSize);	// must be pow2
//
//	try {
//		if (api == RtAudio::Api::UNSPECIFIED) {
//			audio = std::make_shared<RtAudio>();
//		}
//		else {
//			audio = std::make_shared<RtAudio>(api);
//		}
//	}
//	catch (std::exception &error) {
//		ofLogError() << error.what();
//		return false;
//	}
//
//	RtAudio::StreamParameters outputParameters;
//	RtAudio::StreamParameters inputParameters;
//	if (nInputChannels>0) {
//		if (inDeviceID >= 0) {
//			inputParameters.deviceId = inDeviceID;
//		}
//		else {
//			inputParameters.deviceId = audio->getDefaultInputDevice();
//		}
//		inputParameters.nChannels = nInputChannels;
//	}
//
//	if (nOutputChannels>0) {
//		if (outDeviceID >= 0) {
//			outputParameters.deviceId = outDeviceID;
//		}
//		else {
//			outputParameters.deviceId = audio->getDefaultOutputDevice();
//		}
//		outputParameters.nChannels = nOutputChannels;
//	}
//
//	unsigned int bufferFrames = (unsigned int)bufferSize; // 256 sample frames
//
//	RtAudio::StreamOptions options;
//	options.flags = RTAUDIO_SCHEDULE_REALTIME;
//	options.numberOfBuffers = nBuffers;
//	options.priority = 1;
//	outputBuffer.setDeviceID(outDeviceID);
//	inputBuffer.setDeviceID(inDeviceID);
//
//	try {
//		audio->openStream((nOutputChannels>0) ? &outputParameters : nullptr, (nInputChannels>0) ? &inputParameters : nullptr, RTAUDIO_FLOAT32,
//			sampleRate, &bufferFrames, &rtAudioCallback, this, &options);
//		audio->startStream();
//	}
//	catch (std::exception &error) {
//		ofLogError() << error.what();
//		return false;
//	}
//	return true;
//}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::start() {
	if (audio == nullptr) return;

	try {
		audio->startStream();
	}
	catch (std::exception &error) {
		ofLogError() << error.what();
	}
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::stop() {
	if (audio == nullptr) return;

	try {
		if (audio->isStreamRunning()) {
			audio->stopStream();
		}
	}
	catch (std::exception &error) {
		ofLogError() << error.what();
	}
}

//------------------------------------------------------------------------------
void ofRtAudioSoundStream::close() {
	if (audio == nullptr) return;

	try {
		if (audio->isStreamOpen()) {
			audio->closeStream();
		}
	}
	catch (std::exception &error) {
		ofLogError() << error.what();
	}
	soundOutputPtr = nullptr;
	soundInputPtr = nullptr;
	audio.reset();	// delete
}

//------------------------------------------------------------------------------
long unsigned long ofRtAudioSoundStream::getTickCount() const {
	return tickCount;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getNumInputChannels() const {
	return _settings.getNumInputChannels();
	//nInputChannels;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getNumOutputChannels() const {
	return  _settings.getNumOutputChannels();  //nOutputChannels;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getSampleRate() const {
	return _settings.getSampleRate(); // sampleRate;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::getBufferSize() const {
	return _settings.getBufferSize();// bufferSize;
}

//------------------------------------------------------------------------------
int ofRtAudioSoundStream::rtAudioCallback(void *outputBuffer, void *inputBuffer, unsigned int nFramesPerBuffer, double streamTime, RtAudioStreamStatus status, void *data) {
	ofRtAudioSoundStream * rtStreamPtr = (ofRtAudioSoundStream *)data;

	if (status) {
		ofLogWarning("ofRtAudioSoundStream") << "stream over/underflow detected";
	}

	// 	rtAudio uses a system by which the audio
	// 	can be of different formats
	// 	char, float, etc.
	// 	we choose float
	float * fPtrOut = (float *)outputBuffer;
	float * fPtrIn = (float *)inputBuffer;
	// [zach] memset output to zero before output call
	// this is because of how rtAudio works: duplex w/ one callback
	// you need to cut in the middle. if the simpleApp
	// doesn't produce audio, we pass silence instead of duplex...

	unsigned int nInputChannels = rtStreamPtr->getNumInputChannels();
	unsigned int nOutputChannels = rtStreamPtr->getNumOutputChannels();

	if (nInputChannels > 0) {
		if (rtStreamPtr->soundInputPtr != nullptr) {
			rtStreamPtr->inputBuffer.copyFrom(fPtrIn, nFramesPerBuffer, nInputChannels, rtStreamPtr->getSampleRate());
			rtStreamPtr->inputBuffer.setTickCount(rtStreamPtr->tickCount);
			rtStreamPtr->soundInputPtr->audioIn(rtStreamPtr->inputBuffer);
		}
		// [damian] not sure what this is for? assuming it's for underruns? or for when the sound system becomes broken?
		memset(fPtrIn, 0, nFramesPerBuffer * nInputChannels * sizeof(float));
	}

	if (nOutputChannels > 0) {
		if (rtStreamPtr->soundOutputPtr != nullptr) {

			if (rtStreamPtr->outputBuffer.size() != nFramesPerBuffer*nOutputChannels || rtStreamPtr->outputBuffer.getNumChannels() != nOutputChannels) {
				rtStreamPtr->outputBuffer.setNumChannels(nOutputChannels);
				rtStreamPtr->outputBuffer.resize(nFramesPerBuffer*nOutputChannels);
			}
			rtStreamPtr->outputBuffer.setTickCount(rtStreamPtr->tickCount);
			rtStreamPtr->soundOutputPtr->audioOut(rtStreamPtr->outputBuffer);
		}
		rtStreamPtr->outputBuffer.copyTo(fPtrOut, nFramesPerBuffer, nOutputChannels, 0);
		rtStreamPtr->outputBuffer.set(0);
	}

	// increment tick count
	rtStreamPtr->tickCount++;

	return 0;
}
#endif
