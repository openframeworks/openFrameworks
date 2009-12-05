#include "ofSoundPlayer.h"
#include "ofUtils.h"


bool bFmodInitialized = false;
bool bUseSpectrum = false;
float fftValues[8192];			//
float fftInterpValues[8192];			//
float fftSpectrum[8192];		// maximum # is 8192, in fmodex....


// ---------------------  static vars
static FMOD_CHANNELGROUP * channelgroup;
static FMOD_SYSTEM       * sys;

// these are global functions, that affect every sound / channel:
// ------------------------------------------------------------
// ------------------------------------------------------------

//--------------------
void ofSoundStopAll(){
	ofSoundPlayer::initializeFmod();
	FMOD_ChannelGroup_Stop(channelgroup);
}

//--------------------
void ofSoundSetVolume(float vol){
	ofSoundPlayer::initializeFmod();
	FMOD_ChannelGroup_SetVolume(channelgroup, vol);
}

//--------------------
void ofSoundUpdate(){
	if (bFmodInitialized){
		FMOD_System_Update(sys);
	}
}

//--------------------
float * ofSoundGetSpectrum(int nBands){

	ofSoundPlayer::initializeFmod();


	// 	set to 0
	for (int i = 0; i < 8192; i++){
		fftInterpValues[i] = 0;
	}

	// 	check what the user wants vs. what we can do:
	if (nBands > 8192){
		ofLog(OF_LOG_ERROR, "error in ofSoundGetSpectrum, the maximum number of bands is 8192 - you asked for %i we will return 8192", nBands);
		nBands = 8192;
	} else if (nBands <= 0){
		ofLog(OF_LOG_ERROR, "error in ofSoundSpectrum, you've asked for %i bands, minimum is 1", nBands);
		return fftInterpValues;
	}

	// 	FMOD needs pow2
	int nBandsToGet = ofNextPow2(nBands);
	if (nBandsToGet < 64) nBandsToGet = 64;  // can't seem to get fft of 32, etc from fmodex

	// 	get the fft
	FMOD_System_GetSpectrum(sys, fftSpectrum, nBandsToGet, 0, FMOD_DSP_FFT_WINDOW_HANNING);

	// 	convert to db scale
	for(int i = 0; i < nBandsToGet; i++){
        fftValues[i] = 10.0f * (float)log10(1 + fftSpectrum[i]) * 2.0f;
	}

	// 	try to put all of the values (nBandsToGet) into (nBands)
	//  in a way which is accurate and preserves the data:
	//

	if (nBandsToGet == nBands){

		for(int i = 0; i < nBandsToGet; i++){
			fftInterpValues[i] = fftValues[i];
		}

	} else {

		float step 		= (float)nBandsToGet / (float)nBands;
		//float pos 		= 0;
		// so for example, if nBands = 33, nBandsToGet = 64, step = 1.93f;
		int currentBand = 0;

		for(int i = 0; i < nBandsToGet; i++){

			// if I am current band = 0, I care about (0+1) * step, my end pos
			// if i > endPos, then split i with me and my neighbor

			if (i >= ((currentBand+1)*step)){

				// do some fractional thing here...
				float fraction = ((currentBand+1)*step) - (i-1);
				float one_m_fraction = 1 - fraction;
				fftInterpValues[currentBand] += fraction * fftValues[i];
				currentBand++;
				// safety check:
				if (currentBand >= nBands){
					ofLog(OF_LOG_ERROR, "ofSoundGetSpectrum - currentBand >= nBands");
				}

				fftInterpValues[currentBand] += one_m_fraction * fftValues[i];

			} else {
				// do normal things
				fftInterpValues[currentBand] += fftValues[i];
			}
		}

		// because we added "step" amount per band, divide to get the mean:
		for (int i = 0; i < nBands; i++){
			fftInterpValues[i] /= step;
			if (fftInterpValues[i] > 1)fftInterpValues[i] = 1; 	// this seems "wrong"
		}

	}

	return fftInterpValues;
}

// ------------------------------------------------------------
// ------------------------------------------------------------


// now, the individual sound player:
//------------------------------------------------------------
ofSoundPlayer::ofSoundPlayer(){
	bLoop 			= false;
	bLoadedOk 		= false;
	pan 			= 0.5f;
	volume 			= 1.0f;
	internalFreq 	= 44100;
	speed 			= 1;
	bPaused 		= false;
	isStreaming		= false;
}

ofSoundPlayer::~ofSoundPlayer(){
	unloadSound();
}



//---------------------------------------
// this should only be called once
void ofSoundPlayer::initializeFmod(){
	if(!bFmodInitialized){
		FMOD_System_Create(&sys);
		#ifdef TARGET_LINUX
			FMOD_System_SetOutput(sys,FMOD_OUTPUTTYPE_ALSA);
		#endif
		FMOD_System_Init(sys, 32, FMOD_INIT_NORMAL, NULL);  //do we want just 32 channels?
		FMOD_System_GetMasterChannelGroup(sys, &channelgroup);
		bFmodInitialized = true;
	}
}


//---------------------------------------
void ofSoundPlayer::closeFmod(){
	if(bFmodInitialized){
		FMOD_System_Close(sys);
		bFmodInitialized = false;
	}
}

//------------------------------------------------------------
void ofSoundPlayer::loadSound(string fileName, bool stream){

	fileName = ofToDataPath(fileName);

	// fmod uses IO posix internally, might have trouble
	// with unicode paths...
	// says this code:
	// http://66.102.9.104/search?q=cache:LM47mq8hytwJ:www.cleeker.com/doxygen/audioengine__fmod_8cpp-source.html+FSOUND_Sample_Load+cpp&hl=en&ct=clnk&cd=18&client=firefox-a
	// for now we use FMODs way, but we could switch if
	// there are problems:

	bMultiPlay = false;

	// [1] init fmod, if necessary

	initializeFmod();

	// [2] try to unload any previously loaded sounds
	// & prevent user-created memory leaks
	// if they call "loadSound" repeatedly, for example

	unloadSound();

	// [3] load sound

	//choose if we want streaming
	int fmodFlags =  FMOD_SOFTWARE;
	if(stream)fmodFlags =  FMOD_SOFTWARE | FMOD_CREATESTREAM;

	result = FMOD_System_CreateSound(sys, fileName.c_str(),  fmodFlags, NULL, &sound);

	if (result != FMOD_OK){
		bLoadedOk = false;
		ofLog(OF_LOG_ERROR,"ofSoundPlayer: Could not load sound file %s", fileName.c_str() );
	} else {
		bLoadedOk = true;
		FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_PCM);
		isStreaming = stream;
	}

}

//------------------------------------------------------------
void ofSoundPlayer::unloadSound(){
	if (bLoadedOk){
		stop();						// try to stop the sound
		if(!isStreaming)FMOD_Sound_Release(sound);
	}
}

//------------------------------------------------------------
bool ofSoundPlayer::getIsPlaying(){

	if (!bLoadedOk) return false;

	int playing = 0;
	FMOD_Channel_IsPlaying(channel, &playing);
	return (playing != 0 ? true : false);
}

//------------------------------------------------------------
float ofSoundPlayer::getSpeed(){
	return speed;
}

//------------------------------------------------------------
float ofSoundPlayer::getPan(){
	return pan;
}

//------------------------------------------------------------
void ofSoundPlayer::setVolume(float vol){
	if (getIsPlaying() == true){
		FMOD_Channel_SetVolume(channel, vol);
	}
	volume = vol;
}

//------------------------------------------------------------
void ofSoundPlayer::setPosition(float pct){
	if (getIsPlaying() == true){
		int sampleToBeAt = (int)(length * pct);
		FMOD_Channel_SetPosition(channel, sampleToBeAt, FMOD_TIMEUNIT_PCM);
	}
}

//------------------------------------------------------------
float ofSoundPlayer::getPosition(){
	if (getIsPlaying() == true){
		unsigned int sampleImAt;

		FMOD_Channel_GetPosition(channel, &sampleImAt, FMOD_TIMEUNIT_PCM);

		float pct = 0.0f;
		if (length > 0){
			pct = sampleImAt / (float)length;
		}
		return pct;
	} else {
		return 0;
	}
}

//------------------------------------------------------------
void ofSoundPlayer::setPan(float p){
	if (getIsPlaying() == true){
		FMOD_Channel_SetPan(channel,p);
	}
	pan = p;
}


//------------------------------------------------------------
void ofSoundPlayer::setPaused(bool bP){
	if (getIsPlaying() == true){
		FMOD_Channel_SetPaused(channel,bP);
		bPaused = bP;
	}
}


//------------------------------------------------------------
void ofSoundPlayer::setSpeed(float spd){
	if (getIsPlaying() == true){
			FMOD_Channel_SetFrequency(channel, internalFreq * spd);
	}
	speed = spd;
}


//------------------------------------------------------------
void ofSoundPlayer::setLoop(bool bLp){
	if (getIsPlaying() == true){
		FMOD_Channel_SetMode(channel,  (bLp == true) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	}
	bLoop = bLp;
}

// ----------------------------------------------------------------------------
void ofSoundPlayer::setMultiPlay(bool bMp){
	bMultiPlay = bMp;		// be careful with this...
}

// ----------------------------------------------------------------------------
void ofSoundPlayer::play(){

	// if it's a looping sound, we should try to kill it, no?
	// or else people will have orphan channels that are looping
	if (bLoop == true){
		FMOD_Channel_Stop(channel);
	}

	// if the sound is not set to multiplay, then stop the current,
	// before we start another
	if (!bMultiPlay){
		FMOD_Channel_Stop(channel);
	}

	FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound, bPaused, &channel);

	FMOD_Channel_GetFrequency(channel, &internalFreq);
	FMOD_Channel_SetVolume(channel,volume);
	FMOD_Channel_SetPan(channel,pan);
	FMOD_Channel_SetFrequency(channel, internalFreq * speed);
	FMOD_Channel_SetMode(channel,  (bLoop == true) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

	//fmod update() should be called every frame - according to the docs.
	//we have been using fmod without calling it at all which resulted in channels not being able
	//to be reused.  we should have some sort of global update function but putting it here
	//solves the channel bug
	FMOD_System_Update(sys);

}

// ----------------------------------------------------------------------------
void ofSoundPlayer::stop(){
	FMOD_Channel_Stop(channel);
}
