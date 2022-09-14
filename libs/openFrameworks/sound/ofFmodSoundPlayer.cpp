#include "ofFmodSoundPlayer.h"
#ifdef OF_SOUND_PLAYER_FMOD

#include "ofUtils.h"
#include "ofMath.h"
#include "ofLog.h"


static bool bFmodInitialized_ = false;
static float fftInterpValues_[8192];			//
static float fftSpectrum_[8192];		// maximum #ofFmodSoundPlayer is 8192, in fmod....
static unsigned int buffersize = 1024;
static FMOD_DSP* fftDSP = NULL;

// ---------------------  static vars
static FMOD_CHANNELGROUP * channelgroup = NULL;
static FMOD_SYSTEM       * sys = NULL;

// these are global functions, that affect every sound / channel:
// ------------------------------------------------------------
// ------------------------------------------------------------

//--------------------
void ofFmodSoundStopAll(){
	ofFmodSoundPlayer::initializeFmod();
	FMOD_ChannelGroup_Stop(channelgroup);
}

//--------------------
void ofFmodSoundSetVolume(float vol){
	ofFmodSoundPlayer::initializeFmod();
	FMOD_ChannelGroup_SetVolume(channelgroup, vol);
}

//--------------------
void ofFmodSoundUpdate(){
	if (bFmodInitialized_){
		FMOD_System_Update(sys);
	}
}

//--------------------
float * ofFmodSoundGetSpectrum(int nBands){

	ofFmodSoundPlayer::initializeFmod();


	// 	set to 0
	for (int i = 0; i < 8192; i++){
		fftInterpValues_[i] = 0;
        fftSpectrum_[i] = 0;
	}

	// 	check what the user wants vs. what we can do:
	if (nBands > 8192){
		ofLogWarning("ofFmodSoundPlayer") << "ofFmodGetSpectrum(): requested number of bands " << nBands << ", using maximum of 8192";
		nBands = 8192;
	} else if (nBands <= 0){
		ofLogWarning("ofFmodSoundPlayer") << "ofFmodGetSpectrum(): requested number of bands " << nBands << ", using minimum of 1";
		nBands = 1;
		return fftInterpValues_;
	}

    //  get the fft
    //  useful info here: https://www.parallelcube.com/2018/03/10/frequency-spectrum-using-fmod-and-ue4/
    if( fftDSP == NULL ){
        FMOD_System_CreateDSPByType(sys, FMOD_DSP_TYPE_FFT,&fftDSP);
        FMOD_ChannelGroup_AddDSP(channelgroup,0,fftDSP);
        FMOD_DSP_SetParameterInt(fftDSP, FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_HANNING);
    }
    
    if( fftDSP != NULL ){
        FMOD_DSP_PARAMETER_FFT *fft;
        auto result = FMOD_DSP_GetParameterData(fftDSP, FMOD_DSP_FFT_SPECTRUMDATA, (void **)&fft, 0, 0, 0);
        if( result == 0 ){
        
            // Only read / display half of the buffer typically for analysis
            // as the 2nd half is usually the same data reversed due to the nature of the way FFT works. ( comment from link above )
            int length = fft->length/2;
            if( length > 0 ){
        
                std::vector <float> avgValCount;
                avgValCount.assign(nBands, 0.0); 
                
                float normalizedBand = 0;
                float normStep = 1.0 / (float)length;
                
                for (int bin = 0; bin < length; bin++){
                    //should map 0 to nBands but accounting for lower frequency bands being more important
                    int logIndexBand = log10(1.0 + normalizedBand*9.0) * nBands;
                    
                    //get both channels as that is what the old FMOD call did
                    for (int channel = 0; channel < fft->numchannels; channel++){
                        fftSpectrum_[logIndexBand] += fft->spectrum[channel][bin];
                        avgValCount[logIndexBand] += 1.0;
                    }
                    
                    normalizedBand += normStep;
                }
                                
                //average the remapped bands based on how many times we added to each bin
                for(int i = 0; i < nBands; i++){
                    if( avgValCount[i] > 1.0 ){
                        fftSpectrum_[i] /= avgValCount[i];
                    }
                }
            }
        }
    }

	// 	convert to db scale
	for(int i = 0; i < nBands; i++){
        fftInterpValues_[i] = 10.0f * (float)log10(1 + fftSpectrum_[i]) * 2.0f;
	}

	return fftInterpValues_;
}

void ofFmodSetBuffersize(unsigned int bs){
	buffersize = bs;
}

// ------------------------------------------------------------
// ------------------------------------------------------------


// now, the individual sound player:
//------------------------------------------------------------
ofFmodSoundPlayer::ofFmodSoundPlayer(){
	bLoop 			= false;
	bLoadedOk 		= false;
	pan 			= 0.0; // range for oF is -1 to 1
	volume 			= 1.0f;
	internalFreq 	= 44100;
	speed 			= 1;
	bPaused 		= false;
	isStreaming		= false;
}

ofFmodSoundPlayer::~ofFmodSoundPlayer(){
	unload();
}



//---------------------------------------
// this should only be called once
void ofFmodSoundPlayer::initializeFmod(){
	if(!bFmodInitialized_){
		
		FMOD_System_Create(&sys);
		
		// set buffersize, keep number of buffers
		unsigned int bsTmp;
		int nbTmp;
		FMOD_System_GetDSPBufferSize(sys, &bsTmp, &nbTmp);
		FMOD_System_SetDSPBufferSize(sys, buffersize, nbTmp);

		#ifdef TARGET_LINUX
			FMOD_System_SetOutput(sys,FMOD_OUTPUTTYPE_ALSA);
		#endif
		FMOD_System_Init(sys, 512, FMOD_INIT_NORMAL, nullptr);  //do we want just 512 channels?
		FMOD_System_GetMasterChannelGroup(sys, &channelgroup);
		bFmodInitialized_ = true;
	}
}




//---------------------------------------
void ofFmodSoundPlayer::closeFmod(){
	if(bFmodInitialized_){
		FMOD_System_Close(sys);
		bFmodInitialized_ = false;
	}
}

//------------------------------------------------------------
bool ofFmodSoundPlayer::load(const std::filesystem::path& _fileName, bool stream){

	auto fileName = ofToDataPath(_fileName);

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

	unload();

	// [3] load sound

	//choose if we want streaming
	int fmodFlags =  FMOD_DEFAULT;
	if(stream)fmodFlags =  FMOD_DEFAULT | FMOD_CREATESTREAM;

    result = FMOD_System_CreateSound(sys, fileName.data(),  fmodFlags, nullptr, &sound);

	if (result != FMOD_OK){
		bLoadedOk = false;
		ofLogError("ofFmodSoundPlayer") << "loadSound(): could not load \"" << fileName << "\"";
	} else {
		bLoadedOk = true;
		FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_PCM);
		isStreaming = stream;
	}

	return bLoadedOk;
}

//------------------------------------------------------------
void ofFmodSoundPlayer::unload(){
	if (bLoadedOk){
		stop();						// try to stop the sound
		FMOD_Sound_Release(sound);
		bLoadedOk = false;
	}
}

//------------------------------------------------------------
bool ofFmodSoundPlayer::isPlaying() const{

	if (!bLoadedOk) return false;
	if(channel == NULL) return false;
	int playing = 0;
	FMOD_Channel_IsPlaying(channel, &playing);
	return (playing != 0 ? true : false);
}

//------------------------------------------------------------
float ofFmodSoundPlayer::getSpeed() const{
	return speed;
}

//------------------------------------------------------------
float ofFmodSoundPlayer::getPan() const{
	return pan;
}

//------------------------------------------------------------
float ofFmodSoundPlayer::getVolume() const{
	return volume;
}

//------------------------------------------------------------
bool ofFmodSoundPlayer::isLoaded() const{
	return bLoadedOk;
}

//------------------------------------------------------------
void ofFmodSoundPlayer::setVolume(float vol){
	if (isPlaying()){
		FMOD_Channel_SetVolume(channel, vol);
	}
	volume = vol;
}

//------------------------------------------------------------
void ofFmodSoundPlayer::setPosition(float pct){
	if (isPlaying()){
		int sampleToBeAt = (int)(length * pct);
		FMOD_Channel_SetPosition(channel, sampleToBeAt, FMOD_TIMEUNIT_PCM);
	}
}

void ofFmodSoundPlayer::setPositionMS(int ms) {
	if (isPlaying()){
		FMOD_Channel_SetPosition(channel, ms, FMOD_TIMEUNIT_MS);
	}
}

//------------------------------------------------------------
float ofFmodSoundPlayer::getPosition() const{
	if (isPlaying()){
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
int ofFmodSoundPlayer::getPositionMS() const{
	if (isPlaying()){
		unsigned int sampleImAt;

		FMOD_Channel_GetPosition(channel, &sampleImAt, FMOD_TIMEUNIT_MS);

		return sampleImAt;
	} else {
		return 0;
	}
}

//------------------------------------------------------------
void ofFmodSoundPlayer::setPan(float p){
	pan = p;
	p = ofClamp(p, -1, 1);
	if (isPlaying()){
		FMOD_Channel_SetPan(channel,p);
	}
}


//------------------------------------------------------------
void ofFmodSoundPlayer::setPaused(bool bP){
	if (isPlaying()){
		FMOD_Channel_SetPaused(channel,bP);
		bPaused = bP;
	}
}


//------------------------------------------------------------
void ofFmodSoundPlayer::setSpeed(float spd){
	if (isPlaying()){
			FMOD_Channel_SetFrequency(channel, internalFreq * spd);
	}
	speed = spd;
}


//------------------------------------------------------------
void ofFmodSoundPlayer::setLoop(bool bLp){
	if (isPlaying()){
		FMOD_Channel_SetMode(channel,  (bLp == true) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	}
	bLoop = bLp;
}

// ----------------------------------------------------------------------------
void ofFmodSoundPlayer::setMultiPlay(bool bMp){
	bMultiPlay = bMp;		// be careful with this...
}

// ----------------------------------------------------------------------------
void ofFmodSoundPlayer::play(){

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

	FMOD_System_PlaySound(sys, sound, channelgroup, bPaused, &channel);

	FMOD_Channel_GetFrequency(channel, &internalFreq);
	FMOD_Channel_SetVolume(channel,volume);
	setPan(pan);
	FMOD_Channel_SetFrequency(channel, internalFreq * speed);
	FMOD_Channel_SetMode(channel,  (bLoop == true) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

	//fmod update() should be called every frame - according to the docs.
	//we have been using fmod without calling it at all which resulted in channels not being able
	//to be reused.  we should have some sort of global update function but putting it here
	//solves the channel bug
	FMOD_System_Update(sys);

}

// ----------------------------------------------------------------------------
void ofFmodSoundPlayer::stop(){
	FMOD_Channel_Stop(channel);
}

#endif //OF_SOUND_PLAYER_FMOD
