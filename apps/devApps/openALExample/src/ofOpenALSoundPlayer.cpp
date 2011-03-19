#include "ofOpenALSoundPlayer.h"
#include "ofUtils.h"
#include "ofMath.h"
#include "ofFileUtils.h"
#include <sndfile.h>
#ifdef OF_USING_MPG123
	#include <mpg123.h>
#endif
#include <set>

ALCdevice * ofOpenALSoundPlayer::alDevice = 0;
ALCcontext * ofOpenALSoundPlayer::alContext = 0;
vector<float> ofOpenALSoundPlayer::window;
float ofOpenALSoundPlayer::windowSum=0;


kiss_fftr_cfg ofOpenALSoundPlayer::systemFftCfg=0;
vector<float> ofOpenALSoundPlayer::systemWindowedSignal;
vector<float> ofOpenALSoundPlayer::systemBins;
vector<kiss_fft_cpx> ofOpenALSoundPlayer::systemCx_out;

static set<ofOpenALSoundPlayer*> players;

void ofOpenALSoundUpdate(){
	alcProcessContext(ofOpenALSoundPlayer::alContext);
}

// now, the individual sound player:
//------------------------------------------------------------
ofOpenALSoundPlayer::ofOpenALSoundPlayer(){
	bLoop 			= false;
	bLoadedOk 		= false;
	pan 			= 0.5f;
	volume 			= 1.0f;
	internalFreq 	= 44100;
	speed 			= 1;
	bPaused 		= false;
	isStreaming		= false;
	channels		= 0;
	duration		= 0;
	fftCfg			= 0;
	players.insert(this);
}

ofOpenALSoundPlayer::~ofOpenALSoundPlayer(){
	unloadSound();
	kiss_fftr_free(fftCfg);
	players.erase(this);
}

//---------------------------------------
// this should only be called once
void ofOpenALSoundPlayer::initialize(){
	if(!alDevice){
		alDevice = alcOpenDevice(NULL);
		alContext = alcCreateContext(alDevice,NULL);
		alcMakeContextCurrent (alContext);
		alListener3f(AL_POSITION, 0,0,0);
#ifdef OF_USING_MPG123
		mpg123_init();
#endif

	}
}

//---------------------------------------
void ofOpenALSoundPlayer::createWindow(int size){
	if(int(window.size())!=size){
		windowSum = 0;
		window.resize(size);
		// hanning window
		for(int i = 0; i < 511*2; i++){
			window[i] = .54 - .46 * cos((TWO_PI * i) / (511*2 - 1));
			windowSum += window[i];
		}
	}
}

//---------------------------------------
void ofOpenALSoundPlayer::close(){
	if(alDevice){
		alcCloseDevice(alDevice);
		alDevice = NULL;
		alcDestroyContext(alContext);
		alContext = 0;
#ifdef OF_USING_MPG123
		mpg123_exit();
#endif
	}
}

bool ofOpenALSoundPlayer::sfReadFile(string path, vector<short> & buffer, vector<float> & fftAuxBuffer){
	SF_INFO sfInfo;
	SNDFILE* f = sf_open(path.c_str(),SFM_READ,&sfInfo);
	if(!f){
		ofLog(OF_LOG_ERROR,"ofOpenALSoundPlayer: couldnt read " + path);
		return false;
	}

	buffer.resize(sfInfo.frames*sfInfo.channels);
	fftAuxBuffer.resize(sfInfo.frames*sfInfo.channels);

	int subformat = sfInfo.format & SF_FORMAT_SUBMASK ;
	if (subformat == SF_FORMAT_FLOAT || subformat == SF_FORMAT_DOUBLE){
		double	scale ;
		sf_command (f, SFC_CALC_SIGNAL_MAX, &scale, sizeof (scale)) ;
		if (scale < 1e-10)
			scale = 1.0 ;
		else
			scale = 32700.0 / scale ;

		sf_count_t samples_read = sf_read_float (f, &fftAuxBuffer[0], fftAuxBuffer.size());
		if(samples_read<(int)fftAuxBuffer.size())
			ofLog(OF_LOG_ERROR,"ofOpenALSoundPlayer: couldnt read " + path);
		for (int i = 0 ; i < int(fftAuxBuffer.size()) ; i++){
			fftAuxBuffer[i] *= scale ;
			buffer[i] = 32565.0 * fftAuxBuffer[i];
		}
	}else{
		sf_count_t frames_read = sf_readf_short(f,&buffer[0],sfInfo.frames);
		if(frames_read<sfInfo.frames){
			ofLog(OF_LOG_ERROR,"ofOpenALSoundPlayer: couldnt read buffer for " + path);
			return false;
		}
		sf_seek(f,0,SEEK_SET);
		frames_read = sf_readf_float(f,&fftAuxBuffer[0],sfInfo.frames);
		if(frames_read<sfInfo.frames){
			ofLog(OF_LOG_ERROR,"ofOpenALSoundPlayer: couldnt read fft buffer for " + path);
			return false;
		}
	}
	sf_close(f);

	channels = sfInfo.channels;
	duration = float(sfInfo.frames) / float(sfInfo.samplerate);
	samplerate = sfInfo.samplerate;
	return true;
}

#ifdef OF_USING_MPG123
//------------------------------------------------------------
bool ofOpenALSoundPlayer::mpg123ReadFile(string path,vector<short> & buffer,vector<float> & fftAuxBuffer){
	int err = MPG123_OK;
	mpg123_handle * f = mpg123_new(NULL,&err);
	if(mpg123_open(f,path.c_str())!=MPG123_OK){
		ofLog(OF_LOG_ERROR,"ofOpenALSoundPlayer: couldnt read " + path);
		return false;
	}

	int encoding;
	long int rate;
	mpg123_getformat(f,&rate,&channels,&encoding);
	if(encoding!=MPG123_ENC_SIGNED_16){
		ofLog(OF_LOG_ERROR,"ofOpenALSoundPlayer: unsupported encoding");
		return false;
	}
	samplerate = rate;

	size_t done=0;
	size_t buffer_size = mpg123_outblock( f );
	buffer.resize(buffer_size);
	while(mpg123_read(f,(unsigned char*)&buffer[buffer.size()-buffer_size/2],buffer_size,&done)!=MPG123_DONE){
		buffer.resize(buffer.size()+buffer_size/2);
	};
	buffer.resize(buffer.size()-done/2);
	mpg123_close(f);
	mpg123_delete(f);

	fftAuxBuffer.resize(buffer.size());
	for(int i=0;i<(int)buffer.size();i++){
		fftAuxBuffer[i] = float(buffer[i])/32565.f;
	}
	duration = float(buffer.size()/channels) / float(samplerate);
	return true;
}
#endif

//------------------------------------------------------------
void ofOpenALSoundPlayer::loadSound(string fileName, bool stream){

	fileName = ofToDataPath(fileName);

	bMultiPlay = false;

	// [1] init sound systems, if necessary
	initialize();

	// [2] try to unload any previously loaded sounds
	// & prevent user-created memory leaks
	// if they call "loadSound" repeatedly, for example

	unloadSound();
	vector<short> buffer;
	vector<float> fftAuxBuffer;
	ALenum format=AL_FORMAT_MONO16;
#ifdef OF_USING_MPG123
	if(ofFileUtils::getFileExt(fileName)!="mp3" && ofFileUtils::getFileExt(fileName)!="MP3"){
		if(!sfReadFile(fileName,buffer,fftAuxBuffer)) return;
	}else{
		if(!mpg123ReadFile(fileName,buffer,fftAuxBuffer)) return;
	}
#else
	if(!sfReadFile(fileName,buffer,fftAuxBuffer)) return;
#endif

	fftBuffers.resize(channels);
	buffers.resize(channels);
	int numFrames = buffer.size()/channels;

	for(int i=0;i<channels;i++){
		fftBuffers[i].resize(numFrames);
		for(int j=0;j<numFrames;j++){
			fftBuffers[i][j] = fftAuxBuffer[j*channels+i];
		}
	}


	if(channels==1){
		alGenBuffers(1,&buffers[0]);
		alBufferData(buffers[0],format,&buffer[0],buffer.size()*2,samplerate);
		if (alGetError() != AL_NO_ERROR)
			return;

		sources.resize(1);
		alGenSources(1, &sources[0]);
		if (alGetError() != AL_NO_ERROR)
			return;
		alSourcei (sources[0], AL_BUFFER,   buffers[0]   );
		alSourcef (sources[0], AL_PITCH,    1.0f     );
		alSourcef (sources[0], AL_GAIN,     1.0f     );
	}else{
		vector<vector<short> > multibuffer;
		multibuffer.resize(channels);
		sources.resize(channels);
		alGenBuffers(channels,&buffers[0]);
		alGenSources(channels, &sources[0]);
		for(int i=0;i<channels;i++){
			multibuffer[i].resize(buffer.size()/channels);
			for(int j=0;j<numFrames;j++){
				multibuffer[i][j] = buffer[j*channels+i];
			}

			if (alGetError() != AL_NO_ERROR){
				ofLog(OF_LOG_ERROR,"ofOpenALSoundPlayer: error creating stereo buffers for " + fileName);
				return;
			}

			alBufferData(buffers[i],format,&multibuffer[i][0],buffer.size()/channels*2,samplerate);
			if (alGetError() != AL_NO_ERROR){
				ofLog(OF_LOG_ERROR,"ofOpenALSoundPlayer: error creating stereo buffers for " + fileName);
				return;
			}

			if (alGetError() != AL_NO_ERROR){
				ofLog(OF_LOG_ERROR,"ofOpenALSoundPlayer: error creating stereo sources for " + fileName);
				return;
			}
			alSourcei (sources[i], AL_BUFFER,   buffers[i]   );
			if (alGetError() != AL_NO_ERROR){
				ofLog(OF_LOG_ERROR,"ofOpenALSoundPlayer: error creating stereo sources for " + fileName);
				return;
			}

			// only stereo panning
			if(i==0){
				float pos[3] = {-1,0,0};
				alSourcefv(sources[i],AL_POSITION,pos);
			}else{
				float pos[3] = {1,0,0};
				alSourcefv(sources[i],AL_POSITION,pos);

			}
		}
	}

	if(bMultiPlay){
		ofAddListener(ofEvents.update,this,&ofOpenALSoundPlayer::update);
	}
}

//------------------------------------------------------------
void ofOpenALSoundPlayer::update(ofEventArgs & args){
	for(int i=0; i<int(sources.size())/channels-1; ){
		ALint state;
		alGetSourcei(sources[i*channels],AL_SOURCE_STATE,&state);
		if(state != AL_PLAYING){
			alDeleteSources(channels,&sources[i*channels]);
			for(int j=0;j<channels;j++){
				sources.erase(sources.begin()+i*channels);
			}
		}else{
			i++;
		}
	}
}

//------------------------------------------------------------
void ofOpenALSoundPlayer::unloadSound(){
	ofRemoveListener(ofEvents.update,this,&ofOpenALSoundPlayer::update);
	alDeleteBuffers(buffers.size(),&buffers[0]);
	alDeleteSources(sources.size(),&sources[0]);
}

//------------------------------------------------------------
bool ofOpenALSoundPlayer::getIsPlaying(){
	if(sources.empty()) return false;
	ALint state;
	alGetSourcei(sources[0],AL_SOURCE_STATE,&state);
	return state == AL_PLAYING;
}

//------------------------------------------------------------
bool ofOpenALSoundPlayer::getIsPaused(){
	if(sources.empty()) return false;
	ALint state;
	alGetSourcei(sources[0],AL_SOURCE_STATE,&state);
	return state == AL_PAUSED;
}

//------------------------------------------------------------
float ofOpenALSoundPlayer::getSpeed(){
	return speed;
}

//------------------------------------------------------------
float ofOpenALSoundPlayer::getPan(){
	return pan;
}

//------------------------------------------------------------
void ofOpenALSoundPlayer::setVolume(float vol){
	volume = vol;
	if(sources.empty()) return;
	if(channels==1){
		alSourcef (sources[0], AL_GAIN, vol);
	}else{
		setPan(pan);
	}
}

//------------------------------------------------------------
void ofOpenALSoundPlayer::setPosition(float pct){
	if(sources.empty()) return;
	for(int i=0;i<(int)sources.size();i++){
		alSourcef(sources[i],AL_SEC_OFFSET,pct*duration);
	}
}

//------------------------------------------------------------
float ofOpenALSoundPlayer::getPosition(){
	if(duration==0) return 0;
	if(sources.empty()) return 0;
	float pos;
	alGetSourcef(sources[0],AL_SEC_OFFSET,&pos);
	return pos/duration;
}

//------------------------------------------------------------
void ofOpenALSoundPlayer::setPan(float p){
	if(sources.empty()) return;
	if(channels==1){
		p=p*2-1;
		float pos[3] = {p,0,0};
		alSourcefv(sources[0],AL_POSITION,pos);
	}else{
		for(int i=0;i<(int)channels;i++){
			if(i==0)
				alSourcef(sources[sources.size()-channels+i],AL_GAIN,(1-p)*volume);
			else
				alSourcef(sources[sources.size()-channels+i],AL_GAIN,p*volume);
		}
	}
	pan = p;
}


//------------------------------------------------------------
void ofOpenALSoundPlayer::setPaused(bool bP){
	if(sources.empty()) return;
	if(bP){
		alSourcePausev(sources.size(),&sources[0]);
	}else{
		alSourcePlayv(sources.size(),&sources[0]);
	}

	bPaused = bP;
}


//------------------------------------------------------------
void ofOpenALSoundPlayer::setSpeed(float spd){
	for(int i=0;i<channels;i++){
		alSourcef(sources[sources.size()-channels+i],AL_PITCH,spd);
	}
	speed = spd;
}


//------------------------------------------------------------
void ofOpenALSoundPlayer::setLoop(bool bLp){
	for(int i=0;i<(int)sources.size();i++){
		alSourcei(sources[i],AL_LOOPING,bLp?AL_TRUE:AL_FALSE);
	}
	bLoop = bLp;
}

// ----------------------------------------------------------------------------
void ofOpenALSoundPlayer::setMultiPlay(bool bMp){
	bMultiPlay = bMp;		// be careful with this...
	if(sources.empty()) return;
	if(bMultiPlay){
		ofAddListener(ofEvents.update,this,&ofOpenALSoundPlayer::update);
	}else{
		ofRemoveListener(ofEvents.update,this,&ofOpenALSoundPlayer::update);
	}
}

// ----------------------------------------------------------------------------
void ofOpenALSoundPlayer::play(){

	// if the sound is set to multiplay, then create new sources,
	// do not multiplay on loop or we won't be able to stop it
	if (bMultiPlay && !bLoop){
		sources.resize(sources.size()+channels);
		alGenSources(channels, &sources[sources.size()-channels]);
		if (alGetError() != AL_NO_ERROR){
			ofLog(OF_LOG_ERROR,"ofOpenALSoundPlayer: error creating multiplay stereo sources");
			return;
		}
		for(int i=0;i<channels;i++){
			alSourcei (sources[sources.size()-channels+i], AL_BUFFER,   buffers[i]   );
			// only stereo panning
			if(i==0){
				float pos[3] = {-1,0,0};
				alSourcefv(sources[sources.size()-channels+i],AL_POSITION,pos);
			}else{
				float pos[3] = {1,0,0};
				alSourcefv(sources[sources.size()-channels+i],AL_POSITION,pos);

			}
		}

		if (alGetError() != AL_NO_ERROR){
			ofLog(OF_LOG_ERROR,"ofOpenALSoundPlayer: error assigning multiplay buffers");
			return;
		}
	}
	alSourcePlayv(channels,&sources[sources.size()-channels]);

}

// ----------------------------------------------------------------------------
void ofOpenALSoundPlayer::stop(){
	alSourceStopv(channels,&sources[sources.size()-channels]);
}

void ofOpenALSoundPlayer::initFFT(int bands){
	if(int(bins.size())==bands) return;
	int signalSize = (bands-1)*2;
	if(fftCfg!=0) kiss_fftr_free(fftCfg);
	fftCfg = kiss_fftr_alloc(signalSize, 0, NULL, NULL);
	cx_out.resize(bands);
	bins.resize(bands);
	createWindow(signalSize);
}

void ofOpenALSoundPlayer::initSystemFFT(int bands){
	if(int(systemBins.size())==bands) return;
	int signalSize = (bands-1)*2;
	if(systemFftCfg!=0) kiss_fftr_free(systemFftCfg);
	systemFftCfg = kiss_fftr_alloc(signalSize, 0, NULL, NULL);
	systemCx_out.resize(bands);
	systemBins.resize(bands);
	createWindow(signalSize);
}

float * ofOpenALSoundPlayer::getCurrentBufferSum(int size){
	if(int(windowedSignal.size())!=size){
		windowedSignal.resize(size);
	}
	windowedSignal.assign(windowedSignal.size(),0);
	for(int k=0;k<int(sources.size())/channels;k++){
		ALint state;
		alGetSourcei(sources[k*channels],AL_SOURCE_STATE,&state);
		if( state != AL_PLAYING ) continue;
		int pos;
		alGetSourcei(sources[k*channels],AL_SAMPLE_OFFSET,&pos);
		if(pos+size>=(int)fftBuffers[0].size()) continue;
		for(int i=0;i<channels;i++){
			float gain;
			alGetSourcef(sources[k*channels+i],AL_GAIN,&gain);
			for(int j=0;j<size;j++){
				windowedSignal[j]+=fftBuffers[i][pos+j]*gain;
			}
		}
	}
	return &windowedSignal[0];
}

// ----------------------------------------------------------------------------
float * ofOpenALSoundPlayer::getSpectrum(int bands){
	initFFT(bands);
	bins.assign(bins.size(),0);
	if(sources.empty()) return &bins[0];

	int signalSize = (bands-1)*2;
	getCurrentBufferSum(signalSize);

	float normalizer = 2. / windowSum;
	runWindow(windowedSignal);
	kiss_fftr(fftCfg, &windowedSignal[0], &cx_out[0]);
	for(int i= 0; i < bands; i++) {
		bins[i] += sqrtf(cx_out[i].r * cx_out[i].r + cx_out[i].i * cx_out[i].i) * normalizer;
	}
	return &bins[0];
}


float * ofOpenALSoundPlayer::getSystemSpectrum(int bands){
	initSystemFFT(bands);
	systemBins.assign(systemBins.size(),0);
	if(players.empty()) return &systemBins[0];

	int signalSize = (bands-1)*2;
	if(int(systemWindowedSignal.size())!=signalSize){
		systemWindowedSignal.resize(signalSize);
	}
	systemWindowedSignal.assign(systemWindowedSignal.size(),0);

	set<ofOpenALSoundPlayer*>::iterator it;
	for(it=players.begin();it!=players.end();it++){
		if(!(*it)->getIsPlaying()) continue;
		float * buffer = (*it)->getCurrentBufferSum(signalSize);
		for(int i=0;i<signalSize;i++){
			systemWindowedSignal[i]+=buffer[i];
		}
	}

	float normalizer = 2. / windowSum;
	runWindow(systemWindowedSignal);
	kiss_fftr(systemFftCfg, &systemWindowedSignal[0], &systemCx_out[0]);
	for(int i= 0; i < bands; i++) {
		systemBins[i] += sqrtf(systemCx_out[i].r * systemCx_out[i].r + systemCx_out[i].i * systemCx_out[i].i) * normalizer;
	}
	return &systemBins[0];
}

void ofOpenALSoundPlayer::runWindow(vector<float> & signal){
	for(int i = 0; i < (int)signal.size(); i++)
		signal[i] *= window[i];
}
