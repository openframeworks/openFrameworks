#include "ofPASoundStream.h"
#include "ofConstants.h"
//#ifdef OF_USING_PORTAUDIO
#include "portaudio.h"
#include "ofUtils.h"
#include "ofEvents.h"
#include "ofBaseApp.h"

#ifdef TARGET_LINUX
	#include "pa_linux_alsa.h"
#endif

bool ofPASoundStream::initialized=false;

ofPASoundStream::ofPASoundStream(){
	deviceID		= -1;
	soundOutputPtr	= NULL;
	soundInputPtr	= NULL;
	tickCount= 0;

	if(!initialized){
		PaError err;
		err = Pa_Initialize();
		if( err != paNoError ){
			ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( err ));
		}else{
			initialized = true;
		}
	}
}

ofPASoundStream::~ofPASoundStream(){
	stop();
	//close();
}

void ofPASoundStream::setInput(ofBaseSoundInput * soundInput){
	soundInputPtr = soundInput;
}

void ofPASoundStream::setOutput(ofBaseSoundOutput * soundOutput){
	soundOutputPtr = soundOutput;
}

bool ofPASoundStream::setup(int outChannels, int inChannels, int _sampleRate, int bufferSize, int nBuffers){
	nInputChannels 		=  inChannels;
	nOutputChannels 	=  outChannels;
	bufferSize = ofNextPow2(bufferSize);	// must be pow2
	sampleRate = _sampleRate;
	tickCount			=  0;


	PaError err;

	PaStreamParameters inputParameters;
	if(deviceID>=0){
		inputParameters.device = deviceID;
	}else{
		inputParameters.device = Pa_GetDefaultInputDevice();// paUseHostApiSpecificDeviceSpecification;
	}
	inputParameters.channelCount = inChannels;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.hostApiSpecificStreamInfo =NULL;

	PaStreamParameters outputParameters;
	if(deviceID>=0){
		outputParameters.device = deviceID;
	}else{
		outputParameters.device = Pa_GetDefaultOutputDevice();// paUseHostApiSpecificDeviceSpecification;
	}
	outputParameters.channelCount = outChannels;
	outputParameters.sampleFormat = paFloat32;
	//outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo=NULL;//&alsaStreamInfo;

	err = Pa_OpenStream( &audio,
							  inChannels==0?NULL:&inputParameters,
							  outChannels==0?NULL:&outputParameters,
							  sampleRate,
							  bufferSize,
							  paNoFlag,
							  &paAudioCallback,
							  this );
#ifdef TARGET_LINUX
	//PaAlsa_EnableRealtimeScheduling(&audio,1);
#endif

	if( err != paNoError ){
		ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( err ));
		return false;
	}
//	err = Pa_OpenDefaultStream( &audio,
//									nInputChannels,          /* no input channels */
//									nOutputChannels,          /* stereo output */
//									paFloat32,  /* 64 bit floating point output */
//									sampleRate,
//									bufferSize,        /* frames per buffer, i.e. the number
//													   of sample frames that PortAudio will
//													   request from the callback. Many apps
//													   may want to use
//													   paFramesPerBufferUnspecified, which
//													   tells PortAudio to pick the best,
//													   possibly changing, buffer size.*/
//									&paAudioCallback, /* this is your callback function */
//									this ); /*This is a pointer that will be passed to
//													   your callback*/

	err = Pa_StartStream( audio );
	if( err != paNoError ){
		ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( err ));
		return false;
	}

	return true;
}

bool ofPASoundStream::setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers){
	if(outChannels) setOutput(app);
	if(inChannels) setInput(app);
	return setup(outChannels,inChannels,sampleRate,bufferSize,nBuffers);
}


//------------------------------------------------------------------------------
int ofPASoundStream::paAudioCallback(const void *inputBuffer,
        void *outputBuffer,
        unsigned long bufferSize,
        const PaStreamCallbackTimeInfo* streamTime,
        PaStreamCallbackFlags status,
        void *data ){

	ofPASoundStream * instance = (ofPASoundStream*)data;

	if ( status ) std::cout << "Stream over/underflow detected." << std::endl;

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

	// increment tick count
	instance->tickCount++;

	if (instance->nInputChannels > 0 && instance->soundInputPtr){
		// send incoming data to all the sinks
		instance->soundInputPtr->audioIn( fPtrIn, bufferSize, instance->nInputChannels, instance->deviceID, instance->tickCount );

		memset(fPtrIn, 0, bufferSize * instance->nInputChannels * sizeof(float));
	}


	if (instance->nOutputChannels > 0) {
		memset( fPtrOut, 0, sizeof(float)*bufferSize*instance->nOutputChannels );
		if(instance->soundOutputPtr){
			instance->soundOutputPtr->audioOut( fPtrOut, bufferSize, instance->nOutputChannels, instance->deviceID, instance->tickCount );
		}
	}

	return 0;
}

//---------------------------------------------------------
void ofPASoundStream::stop(){
	int err = Pa_StopStream(audio);
    if( err != paNoError )
    	ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( err ));
}



//---------------------------------------------------------
void ofPASoundStream::start(){
	int err = Pa_StartStream( audio );
    if( err != paNoError )
    	ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( err ));
}


//---------------------------------------------------------
void ofPASoundStream::close(){
	/*int err = Pa_Terminate();
	if( err != paNoError )
    	ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( err ));*/

	stop();
}


//---------------------------------------------------------
void ofPASoundStream::listDevices(){
	int numDevices;

	numDevices = Pa_GetDeviceCount();
	if( numDevices < 0 )
	{
    	ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( numDevices ));
    	return;
	}
	const   PaDeviceInfo *deviceInfo;

	for( int i=0; i<numDevices; i++ )
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		cout << i << ": " << deviceInfo->name << endl;
		cout << "api: " << deviceInfo->hostApi << endl;
		cout << "max in channels" << deviceInfo->maxInputChannels << endl;
		cout << "max out channels" << deviceInfo->maxOutputChannels << endl;
		cout << "default sample rate:" << deviceInfo->defaultSampleRate << endl;
	}

}


void ofPASoundStream::setDeviceID(int _deviceID){
	deviceID = _deviceID;
}

long unsigned long ofPASoundStream::getTickCount(){
	return tickCount;
}
//#endif
