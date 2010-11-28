#include "ofSoundStream.h"
#include "RtAudio.h"

//----------------------------------- static variables:
static ofBaseApp 	* 		OFSAptr = NULL;
RtAudio				*		audio = NULL;
int 						nInputChannels;
int 						nOutputChannels;
ofAudioEventArgs 			audioEventArgs;


int receiveAudioBufferAndCallSimpleApp(void *outputBuffer, void *inputBuffer, unsigned int bufferSize,
           double streamTime, RtAudioStreamStatus status, void *data);


//------------------------------------------------------------------------------
int receiveAudioBufferAndCallSimpleApp(void *outputBuffer, void *inputBuffer, unsigned int bufferSize,
           double streamTime, RtAudioStreamStatus status, void *data){


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


	if (nInputChannels > 0){
		if(OFSAptr) OFSAptr->audioReceived(fPtrIn, bufferSize, nInputChannels);
		#ifdef OF_USING_POCO
			audioEventArgs.buffer = fPtrIn;
			audioEventArgs.bufferSize = bufferSize;
			audioEventArgs.nChannels = nInputChannels;
			ofNotifyEvent( ofEvents.audioReceived, audioEventArgs );
		#endif
		memset(fPtrIn, 0, bufferSize * nInputChannels * sizeof(float));
	}


	if (nOutputChannels > 0) {
		if(OFSAptr) OFSAptr->audioRequested(fPtrOut, bufferSize, nOutputChannels);
		#ifdef OF_USING_POCO
			audioEventArgs.buffer = fPtrOut;
			audioEventArgs.bufferSize = bufferSize;
			audioEventArgs.nChannels = nOutputChannels;
			ofNotifyEvent( ofEvents.audioRequested, audioEventArgs );
		#endif
	}

	return 0;
}
//---------------------------------------------------------
void ofSoundStreamSetup(int nOutputs, int nInputs, ofBaseApp * OFSA){
	ofSoundStreamSetup(nOutputs, nInputs, OFSA, 44100, 256, 4);
}

//---------------------------------------------------------
void ofSoundStreamSetup(int nOutputs, int nInputs, int sampleRate, int bufferSize, int nBuffers){
	ofSoundStreamSetup(nOutputs, nInputs, NULL, sampleRate, bufferSize, nBuffers);
}

//---------------------------------------------------------
void ofSoundStreamSetup(int nOutputs, int nInputs, ofBaseApp * OFSA, int sampleRate, int bufferSize, int nBuffers){

	nInputChannels 		=  nInputs;
	nOutputChannels 	=  nOutputs;
	OFSAptr 			=  OFSA;
	bufferSize = ofNextPow2(bufferSize);	// must be pow2


	try {
		audio = new RtAudio();
	}	catch (RtError &error) {
		error.printMessage();
		//std::exit(EXIT_FAILURE); // need case here
	}


	RtAudio::StreamParameters * outputParameters=NULL;
	if(nOutputChannels >0){
		outputParameters = new RtAudio::StreamParameters();
		outputParameters->deviceId = audio->getDefaultOutputDevice();
		outputParameters->nChannels = nOutputChannels;
	}

	RtAudio::StreamParameters * inputParameters = NULL;
	if(nInputChannels>0){
		inputParameters = new RtAudio::StreamParameters;
		inputParameters->deviceId = audio->getDefaultInputDevice();
		inputParameters->nChannels = nInputChannels;
	}

	unsigned int bufferFrames = (unsigned int)bufferSize; // 256 sample frames

	RtAudio::StreamOptions options;
	options.flags = RTAUDIO_SCHEDULE_REALTIME;
	options.numberOfBuffers = nBuffers;
	options.priority = 1;

	try {

		audio ->openStream( outputParameters, inputParameters, RTAUDIO_FLOAT32,
							sampleRate, &bufferFrames, &receiveAudioBufferAndCallSimpleApp, &options);
		audio->startStream();
	} catch (RtError &error) {
		error.printMessage();
	}

}

//---------------------------------------------------------
void ofSoundStreamStop(){
	try {
    	audio->stopStream();
  	} catch (RtError &error) {
   		error.printMessage();
 	}
}



//---------------------------------------------------------
void ofSoundStreamStart(){
	try{
		audio->startStream();
	} catch (RtError &error) {
		error.printMessage();
	}
}


//---------------------------------------------------------
void ofSoundStreamClose(){
	if(!audio) return;
	try {
    	audio->stopStream();
    	audio->closeStream();
  	} catch (RtError &error) {
   		error.printMessage();
 	}
	delete audio;
}


//---------------------------------------------------------
void ofSoundStreamListDevices(){
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
