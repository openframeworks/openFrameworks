#include "ofSoundStream.h"
#include "RtAudio.h"

//----------------------------------- static variables:
static ofBaseApp 	* 		OFSAptr;
RtAudio				*		audio;
int 						nInputChannels;
int 						nOutputChannels;
ofAudioEventArgs 			audioEventArgs;
int 	receiveAudioBufferAndCallSimpleApp(char *buffer, int bufferSize, void *data);





//------------------------------------------------------------------------------
int receiveAudioBufferAndCallSimpleApp(char *buffer, int bufferSize, void *data){
	// 	rtAudio uses a system by which the audio
	// 	can be of different formats
	// 	char, float, etc.
	// 	we choose float

	float * fPtr = (float *)buffer;

	// [zach] memset output to zero before output call
	// this is because of how rtAudio works: duplex w/ one callback
	// you need to cut in the middle. if the simpleApp
	// doesn't produce audio, we pass silence instead of duplex...

	if (nInputChannels > 0){
		if(OFSAptr)
			OFSAptr->audioReceived(fPtr, bufferSize, nInputChannels);

		#ifdef OF_CORE_EVENTS_ENABLED
			audioEventArgs.buffer = fPtr;
			audioEventArgs.bufferSize = bufferSize;
			audioEventArgs.nChannels = nInputChannels;
			ofNotifyEvent( ofEvents.audioReceived, audioEventArgs );
		#endif

		memset(fPtr, 0, bufferSize * nInputChannels * sizeof(float));
	}
	if (nOutputChannels > 0){
		if(OFSAptr)
			OFSAptr->audioRequested(fPtr, bufferSize, nOutputChannels);

		#ifdef OF_CORE_EVENTS_ENABLED
			audioEventArgs.buffer = fPtr;
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
	int device 			=  0;        // default
	OFSAptr 			=  OFSA;

	bufferSize = ofNextPow2(bufferSize);	// must be pow2

	try {
		audio = new RtAudio();
		audio->openStream(	device, nOutputs, device, nInputs, RTAUDIO_FLOAT32,
                      		sampleRate, &bufferSize, nBuffers);
	} catch (RtError &error) {
		error.printMessage();
		//std::exit(EXIT_FAILURE); // need case here
	}

	try {
		audio->setStreamCallback(&receiveAudioBufferAndCallSimpleApp, (void *)NULL);
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
	RtAudioDeviceInfo info;
	for (int i=1; i<=devices; i++) {
		try {
			info = audioTemp->getDeviceInfo(i);
		} catch (RtError &error) {
			error.printMessage();
			break;
		}
		std::cout << "device = " << i << " (" << info.name << ")\n";
		if (info.isDefault) std::cout << "----* default ----* \n";
		std::cout << "maximum output channels = " << info.outputChannels << "\n";
		std::cout << "maximum input channels = " << info.inputChannels << "\n";
		std::cout << "-----------------------------------------\n";

	}
	delete audioTemp;
}

