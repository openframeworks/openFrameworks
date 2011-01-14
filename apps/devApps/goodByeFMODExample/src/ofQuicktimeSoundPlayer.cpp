#include "ofQuicktimeSoundPlayer.h"
#include "ofUtils.h"



// look at: 
// http://www.meandmark.com/quicktimepart3.html
// http://developer.apple.com/library/mac/#qa/qa2004/qa1371.html
// http://www.cocoadev.com/index.pl?QuickTimeAudioExtractionExample

// FFT: 
// http://developer.apple.com/library/mac/#qa/qa2005/qa1459.html
// http://new.math.uiuc.edu/ivanhoe/public_html/stolarsky/MyController.m.html
// http://developer.apple.com/library/mac/#samplecode/SillyFrequencyLevels/Introduction/Intro.html

// http://eqx.su/distfiles/svn-src/phonon/4.2/qt7/quicktimevideoplayer.cpp


// checking for quicktime 7: 
// http://developer.apple.com/library/mac/#samplecode/QTSetMovieAudioDevice/Listings/QTCode_cpp.html


// need to look at 
// http://developer.apple.com/library/mac/#qa/qa2004/qa1371.html





bool bQuicktimeInitialized = false;

void ofQuicktimeSoundPlayer::enableSpectrumCalculation(int nBands){
	
	
	
	UInt32 numberOfBandLevels = nBands;			// increase this number for more frequency bands
	UInt32 numberOfChannels = 2;				// for StereoMix - if using DeviceMix,
	
	if (freqResults != NULL) free(freqResults);
	freqResults = NULL;
	
	// call this once per movie to establish metering
	OSErr err = SetMovieAudioFrequencyMeteringNumBands(soundToPlay,
												 kQTAudioMeter_StereoMix,
												 &numberOfBandLevels);
	if (err){
		printf("error ! \n");
		return;
	}
	freqResults = (QTAudioFrequencyLevels *  ) malloc(sizeof(QTAudioFrequencyLevels) + sizeof(Float32)*numberOfBandLevels * numberOfChannels);
	
	// Arturo did this!  :) 
	// typical mac code looks strange and doesn't work with dynamically choosing num bands: 
	// malloc(offsetof(QTAudioFrequencyLevels, level[numberOfBandLevels * numberOfChannels]));
	
	//delete [] freqResults->level;	
	//Float32* temp;
	//temp = freqResults->level;
	//temp =  new Float32[ numberOfBandLevels * numberOfChannels];
	
	
	if (freqResults == NULL) {
		err = memFullErr;
		printf("here \n");
		return;
		//goto bail:
	}
	
	freqResults->numChannels = numberOfChannels;
	freqResults->numFrequencyBands = numberOfBandLevels;
	
	if (fftValues.fftValuesL != NULL){
		delete [] fftValues.fftValuesL;
		delete [] fftValues.fftValuesR;
		fftValues.numberOfBands = 0;
	}
	
	fftValues.fftValuesL = new float[numberOfBandLevels];
	fftValues.fftValuesR = new float[numberOfBandLevels];
	fftValues.numberOfBands = numberOfBandLevels;
	
	//case bail: 
		// TODO error checking here
	
}

void ofQuicktimeSoundPlayer::disableSpectrumCalculation(){
	// TODO: don't know how to do this... 
}



FFTResults ofQuicktimeSoundPlayer::getSpectrum(){
	
	if (freqResults != NULL){
		OSErr err  = GetMovieAudioFrequencyLevels(soundToPlay, kQTAudioMeter_StereoMix, freqResults);	
		if (err){
			printf("error \n");
		}
	}
	for (int i = 0; i < freqResults->numChannels; i++) {
		for (int j = 0; j < freqResults->numFrequencyBands; j++) {
			
			// the frequency levels are Float32 values between 0. and 1.
			Float32 value = freqResults->level[(i * freqResults->numFrequencyBands) + j];
			
			//printf("value %f \n", (float)value);
			if (i == 0){
				fftValues.fftValuesL[j] = value;
			} else {
				fftValues.fftValuesR[j] = value;
			}
		}
	}
	
	return fftValues;
}



// these are global functions, that affect every sound / channel:
// ------------------------------------------------------------
// ------------------------------------------------------------


// TODO: this is just a test at the moment...  needs to be fixed up and work with memory checking...
// 
void ofQuicktimeSoundPlayer::tryMultiPlay(){
	
	//soundToPlay;
	//TODO: error checking here! 
	
    TimeValue   srcMovDuration;
    OSErr       err = noErr;
    Handle      audioFileDataRef;
    OSType      audioFileDataRefType;
    short       i, resID = 0;
	
	Movie tempMovie;
	// initialize our movie
    tempMovie = nil;
	
	// create a temporary movie -- we'll add
	// references to the audio file to this movie
    tempMovie = NewMovie(newMovieActive);
    assert(tempMovie != nil);
	
    srcMovDuration = GetMovieDuration(soundToPlay);
	
	// add multiple references to our audio file
	
	// Note:
	// by not calling BeginMediaEdits/EndMediaEdits
	// we are telling QuickTime to *not* copy the
	// actual data - this will give us only references
	// to the data which are small
    
		// add a reference to the source audio file to
		// our temporary movie
	
	     err =  InsertMovieSegment (
								   soundToPlay,   // our audio file
								   tempMovie,  // temporary movie
								   0,
								   srcMovDuration,
								   GetMovieDuration(tempMovie) );
   
	//printf("here \n");
	// for completeness we'll copy the movie settings
	// from the source file
    err = CopyMovieSettings(soundToPlay, tempMovie);
   
	
	GoToBeginningOfMovie(tempMovie);
	StartMovie(tempMovie);
	
	soundsForMultiPlay.push_back(tempMovie);
	
	//TODO: copy speed, volume, pan settings to this movie?
	
}

//--------------------
void ofQuicktimeSoundStopAll(){
	// TODO: how to stop all?
}

//--------------------
void ofQuicktimeSoundSetVolume(float vol){
	// TODO: how to set global volume?
}

//--------------------
void ofQuicktimeSoundUpdate(){
	if (bQuicktimeInitialized){
		// ?
	}
}


// now, the individual sound player:
//------------------------------------------------------------
ofQuicktimeSoundPlayer::ofQuicktimeSoundPlayer(){
	bLoop 			= false;
	bLoadedOk 		= false;
	pan 			= 0.5f;
	volume 			= 1.0f;
	internalFreq 	= 44100;
	speed 			= 1;
	bPaused 		= false;
	isStreaming		= false;
}

ofQuicktimeSoundPlayer::~ofQuicktimeSoundPlayer(){
	if (bLoadedOk){
		unloadSound();
	}
}


//
////---------------------------------------
//// this should only be called once
void ofQuicktimeSoundPlayer::initializeQuicktime(){
	if (bQuicktimeInitialized == false){
		EnterMovies();
		bQuicktimeInitialized = false;
	}
}


//---------------------------------------
void ofQuicktimeSoundPlayer::closeQuicktime(){
	ExitMovies();		
	// TODO: a problem for video players?
}

//------------------------------------------------------------
void ofQuicktimeSoundPlayer::loadSound(string fileName, bool stream){

	
	fileName = ofToDataPath(fileName);
	
	// TODO: hmm?
	bMultiPlay = false;

	// [1] init fmod, if necessary

	initializeQuicktime();

	// [2] try to unload any previously loaded sounds
	// & prevent user-created memory leaks
	// if they call "loadSound" repeatedly, for example

	if (bLoadedOk == true){
		unloadSound();
	}
	// [3] load sound

	OSErr error;
    
    //CFBundleRef gameBundle = CFBundleGetMainBundle();
    
    // Find the file in the application bundle.
    CFURLRef movieFileLocation;
    movieFileLocation = CFURLCreateFromFileSystemRepresentation(NULL, (UInt8*)fileName.c_str(), strlen(fileName.c_str()), false);
	
	//CFBundleCopyResourceURL(gameBundle, filename, fileExtension, subdirectory); 
    
    if (movieFileLocation == NULL)
        return;
    
    Handle dataRef;
    OSType dataRefType;
    
    dataRef = NewHandle(sizeof(AliasHandle));
    
    // Get the movie file set up so we can load it in memory.
    // The second parameter to QTNewDataReferenceFromCFURL is flags.
    // It should be set to 0.
    error = QTNewDataReferenceFromCFURL(movieFileLocation, 0, &dataRef, &dataRefType);
    if(error != noErr) {
        DisposeHandle(dataRef);
        CFRelease(movieFileLocation);
        return;
    }
    
    // Get the movie into memory
    short fileID = movieInDataForkResID;
    short flags = 0;
    error = NewMovieFromDataRef(&soundToPlay, flags, &fileID, dataRef, dataRefType);
    
    // Dispose of the memory we allocated.
    DisposeHandle(dataRef);
    CFRelease(movieFileLocation);
	
	// TODO: check here!  
	bLoadedOk = true;
	

}

//------------------------------------------------------------
void ofQuicktimeSoundPlayer::unloadSound(){
	stop();
	DisposeMovie(soundToPlay);
}

//------------------------------------------------------------
bool ofQuicktimeSoundPlayer::getIsPlaying(){

	if (!bLoadedOk) return false;

	int playing = 0;
	
	Fixed rate = GetMovieRate(soundToPlay);
	bool bDone = IsMovieDone(soundToPlay);
	
	if (bDone == true){
		return false;
	}
	
	if (bIWasPlayingAndMySpeedWasSetToZero == true){
		return true;
	}
	
	return (rate != 0 ? true : false);
}

//------------------------------------------------------------
float ofQuicktimeSoundPlayer::getSpeed(){
	return speed;
}

//------------------------------------------------------------
float ofQuicktimeSoundPlayer::getPan(){
	return pan;
}

//------------------------------------------------------------
void ofQuicktimeSoundPlayer::setVolume(float vol){
	if (getIsPlaying() == true){
		SetMovieVolume(soundToPlay, vol*255);
	}
	volume = vol;
}

//------------------------------------------------------------
void ofQuicktimeSoundPlayer::setPosition(float pct){
	if (getIsPlaying() == true){
		int sampleToBeAt = (int)(length * pct);
		//FMOD_Channel_SetPosition(channel, sampleToBeAt, //FMOD_TIMEUNIT_PCM);
	}
}

//------------------------------------------------------------
float ofQuicktimeSoundPlayer::getPosition(){
	if (getIsPlaying() == true){
		unsigned int sampleImAt;

		
		// TODO: get position
		
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
void ofQuicktimeSoundPlayer::setPan(float p){
	if (getIsPlaying() == true){
		SetMovieAudioBalance(soundToPlay, pan, 0);
	}
	pan = p;
}


//------------------------------------------------------------
void ofQuicktimeSoundPlayer::setPaused(bool bP){
	if (getIsPlaying() == true){
		bPaused = bP;
		if (bP == true){
			StopMovie(soundToPlay);
		} else {
			StartMovie(soundToPlay);
		}
		
	}
}


//------------------------------------------------------------
void ofQuicktimeSoundPlayer::setSpeed(float spd){
	if (getIsPlaying() == true){
		if (fabs(spd - 0) < FLT_EPSILON){
			bIWasPlayingAndMySpeedWasSetToZero = true;	
		} else {
			bIWasPlayingAndMySpeedWasSetToZero = false;
		}
		SetMovieRate(soundToPlay, X2Fix(spd));
	}
	speed = spd;
}


//------------------------------------------------------------
void ofQuicktimeSoundPlayer::setLoop(bool bLp){
	if (getIsPlaying() == true){
		// TODO: set looping
	}
	bLoop = bLp;
}

// ----------------------------------------------------------------------------
void ofQuicktimeSoundPlayer::setMultiPlay(bool bMp){
	bMultiPlay = bMp;		
	// TODO: deal with some looping vs multiplay logic?
}

// ----------------------------------------------------------------------------
void ofQuicktimeSoundPlayer::play(){

	
	// TODO: some logic in play needs work
	
	// if it's a looping sound, we should try to kill it, no?
	// or else people will have orphan channels that are looping
	if (bLoop == true){
		//FMOD_Channel_Stop(channel);
	}

	// if the sound is not set to multiplay, then stop the current,
	// before we start another
	if (!bMultiPlay){
		////FMOD_Channel_Stop(channel);
	}

	GoToBeginningOfMovie(soundToPlay);
	StartMovie(soundToPlay);
	
	if (speed == 0){
		bIWasPlayingAndMySpeedWasSetToZero = true;	
	} else {
		bIWasPlayingAndMySpeedWasSetToZero = false;
	}
	
	////FMOD_System_PlaySound(sys, ////FMOD_CHANNEL_FREE, sound, bPaused, &channel);

	////FMOD_Channel_GetFrequency(channel, &internalFreq);
	//FMOD_Channel_SetVolume(channel,volume);
	//FMOD_Channel_SetPan(channel,pan);
	//FMOD_Channel_SetFrequency(channel, internalFreq * speed);
	//FMOD_Channel_SetMode(channel,  (bLoop == true) ? //FMOD_LOOP_NORMAL : //FMOD_LOOP_OFF);

	//fmod update() should be called every frame - according to the docs.
	//we have been using fmod without calling it at all which resulted in channels not being able
	//to be reused.  we should have some sort of global update function but putting it here
	//solves the channel bug
	//FMOD_System_Update(sys);

}

// ----------------------------------------------------------------------------
void ofQuicktimeSoundPlayer::stop(){
	StopMovie(soundToPlay);
}


