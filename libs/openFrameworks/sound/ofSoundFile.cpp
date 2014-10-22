 /*
 * ofSoundFile.cpp
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#include "ofSoundFile.h"
#include "ofLog.h"
#include "ofUtils.h"

//--------------------------------------------------------------
bool ofLoadSound(ofSoundBuffer &buff, string path){
    ofSoundFile sf(path);
    if(sf.isLoaded()){
        sf.readTo(buff);
        return true;
    }else{
        return false;
    }
}

//--------------------------------------------------------------
// for now this only write 16 bit PCM WAV files.
// It can't really live in ofSoundFile yet because
// ofSoundFile doesn't hold a complete representation
// the sound file that can be written to disk. You'd
// need something that would let you stream the data to
// it via writeTo() or similar. Doesn't really fit wtih
// the current model.
bool ofSaveSound(const ofSoundBuffer &buff,  string path){
	ofSoundFile soundFile;
	return soundFile.saveSound(path, buff);
}

#ifdef OF_USING_MPG123
bool ofSoundFile::mpg123Inited = false;
#endif

//--------------------------------------------------------------
ofSoundFile::ofSoundFile() {
    bCompressed = false;
    bLoaded = false;

#ifdef OF_USING_SNDFILE
	sndFile = NULL;
#endif
	
#ifdef OF_USING_LAD
	audioDecoder = NULL;
#endif

#ifdef OF_USING_MPG123
	mp3File = NULL;
	if(!mpg123Inited){
		mpg123_init();
		mpg123Inited = true;
	}
#endif
	
	close();
}

//--------------------------------------------------------------
ofSoundFile::ofSoundFile(string path) {
    ofSoundFile();
    loadSound(path);
}

//--------------------------------------------------------------
ofSoundFile::~ofSoundFile() {
	close();
}

//--------------------------------------------------------------
bool ofSoundFile::loadSound(string _path){
 	path = ofToDataPath(_path);
	bool result = false;
	if(ofFilePath::getFileExt(path)=="mp3"){
        bCompressed=true;
#ifdef OF_USING_MPG123
        result = mpg123Open(path);
#elif defined (OF_USING_LAD)
        result = ladOpen(path);
#else
        ofLogError() << "mp3 files not supported" << endl;
#endif
	}else{
#ifdef OF_USING_LAD
        result = ladOpen(path);
#else
        result = sfOpen(path);
#endif
	}
	if(result) { // prevent div by zero if file doesn't open.
		duration = float(samples/channels) / float(samplerate);
	}
	bLoaded = result;
	return result;   
}

//--------------------------------------------------------------
bool ofSoundFile::saveSound(string path, const ofSoundBuffer &buff){
	// check that we're writing a wav and complain if the file extension is wrong.
	ofFile f(path);
	if(ofToLower(f.getExtension())!="wav") {
		path += ".wav";
		ofLogWarning() << "Can only write wav files - will save file as " << path;
	}
	
	fstream file(ofToDataPath(path).c_str(), ios::out | ios::binary);
	if(!file.is_open()) {
		ofLogError() << "Error opening sound file '" << path << "' for writing";
		return false;
	}
	
	// write a wav header
	short myFormat = 1; // for pcm
	int mySubChunk1Size = 16;
	int bitsPerSample = 16; // assume 16 bit pcm
	int myByteRate = buff.getSampleRate() * buff.getNumChannels() * bitsPerSample/8;
	short myBlockAlign = buff.getNumChannels() * bitsPerSample/8;
	int myChunkSize = 36 + buff.size()*bitsPerSample/8;
	int myDataSize = buff.size()*bitsPerSample/8;
	int channels = buff.getNumChannels();
	int samplerate = buff.getSampleRate();
	
	file.seekp (0, ios::beg);
	file.write ("RIFF", 4);
	file.write ((char*) &myChunkSize, 4);
	file.write ("WAVE", 4);
	file.write ("fmt ", 4);
	file.write ((char*) &mySubChunk1Size, 4);
	file.write ((char*) &myFormat, 2); // should be 1 for PCM
	file.write ((char*) &channels, 2); // # channels (1 or 2)
	file.write ((char*) &samplerate, 4); // 44100
	file.write ((char*) &myByteRate, 4); //
	file.write ((char*) &myBlockAlign, 2);
	file.write ((char*) &bitsPerSample, 2); //16
	file.write ("data", 4);
	file.write ((char*) &myDataSize, 4);
	
	// write the wav file per the wav file format, 4096 bytes of data at a time.
	#define WRITE_BUFF_SIZE 4096
	
	short writeBuff[WRITE_BUFF_SIZE];
	int pos = 0;
	while(pos<buff.size()) {
		int len = MIN(WRITE_BUFF_SIZE, buff.size()-pos);
		for(int i = 0; i < len; i++) {
			writeBuff[i] = (int)(buff[pos]*32767.f);
			pos++;
		}
		file.write((char*)writeBuff, len*bitsPerSample/8);
	}
	
	file.close();
	return true;
}

//--------------------------------------------------------------                  
#ifdef OF_USING_LAD
bool ofSoundFile::ladOpen(string path){
	audioDecoder = new AudioDecoder(ofToDataPath(path));
	int result = audioDecoder->open();

	samples = audioDecoder->numSamples();
	channels = audioDecoder->channels();
	samplerate = audioDecoder->sampleRate();
	duration = audioDecoder->duration();
	return result == AUDIODECODER_OK;
}
#endif

//--------------------------------------------------------------
#ifdef OF_USING_MPG123
bool ofSoundFile::mpg123Open(string path){
	int err;
	mp3File = mpg123_new(NULL,&err);

	mpg123_param(mp3File, MPG123_ADD_FLAGS, MPG123_FORCE_FLOAT, 0.);

	if(mpg123_open(mp3File,path.c_str())!=MPG123_OK){
		ofLogError() <<  "ofSoundFile: couldnt read " + path;
		return false;
	}

	int encoding;
	long int rate;
	mpg123_getformat(mp3File,&rate,&channels,&encoding);
	if(encoding!=MPG123_ENC_SIGNED_16 && encoding != MPG123_ENC_FLOAT_32){
		ofLogError() <<  "ofSoundFile: unsupported encoding";
		return false;
	}

	mpg123_format_none(mp3File);
	mpg123_format(mp3File, rate, channels, encoding);

	samplerate = rate;


	mpg123_seek(mp3File,0,SEEK_END);
	samples = mpg123_tell(mp3File);
	mpg123_seek(mp3File,0,SEEK_SET);
    
    bitDepth = 16; //TODO:get real bitdepth;.
}
#endif

//--------------------------------------------------------------
#ifdef OF_USING_SNDFILE
bool ofSoundFile::sfOpen(string path){

	SF_INFO sfInfo;
	ofLogNotice() << "opening descriptor " << path;
	sndFile = sf_open(path.c_str(),SFM_READ,&sfInfo);
	if(!sndFile){
		ofLogError() <<  "ofSoundFile: couldnt read " + path;
		return false;
	}
	ofLogNotice() << "odescriptor opened ";

	subformat = sfInfo.format & SF_FORMAT_SUBMASK ;
	if (subformat == SF_FORMAT_FLOAT || subformat == SF_FORMAT_DOUBLE){
		ofLogNotice() << "calculating scale ";
		sf_command (sndFile, SFC_CALC_SIGNAL_MAX, &scale, sizeof (scale)) ;
		if (scale < 1e-10){
			scale = 1.0 ;
		}else{
			scale = 32700.0 / scale ;
		}
	}

	channels = sfInfo.channels;
	samples = sfInfo.frames;
	samplerate = sfInfo.samplerate;
    bitDepth = 16; //fix
}
#endif

//--------------------------------------------------------------
void ofSoundFile::close(){
#ifdef OF_USING_SNDFILE
	if(sndFile){
		sf_close(sndFile);
		sndFile = NULL;
	}
	samples_read = 0;
#endif
#ifdef OF_USING_LAD
	if ( audioDecoder )
		delete audioDecoder;
	audioDecoder = NULL;
#endif
#ifdef OF_USING_MPG123
	if(mp3File){
		mpg123_close(mp3File);
		mpg123_delete(mp3File);
		mp3File = NULL;
	}
#endif
	channels = 1;
	duration = 0; //in secs
	samplerate = 0;
	samples = 0;
}

//--------------------------------------------------------------
bool ofSoundFile::isLoaded(){
    return bLoaded;
}

//--------------------------------------------------------------
int ofSoundFile::getNumChannels(){
	return channels;
}

//--------------------------------------------------------------
unsigned long ofSoundFile::getDuration(){
	return duration*1000;
}

//--------------------------------------------------------------
int ofSoundFile::getSampleRate(){
	return samplerate;
}

//--------------------------------------------------------------
unsigned long ofSoundFile::getNumSamples(){
    return samples;
}

//--------------------------------------------------------------
bool ofSoundFile::isCompressed(){
    return bCompressed;
}

//--------------------------------------------------------------
string ofSoundFile::getPath(){
    return path;
}

//--------------------------------------------------------------
bool ofSoundFile::readTo(ofSoundBuffer & buffer, unsigned int _samples){
	buffer.setNumChannels(channels);
	buffer.setSampleRate(samplerate);
	if(_samples!=0){
		// will read the requested number of samples
		// clamp to the number of samples we actually have
		_samples = min(_samples,samples);
		buffer.resize(_samples*channels);
	}
#ifdef OF_USING_SNDFILE
	else if (sndFile){
		// will read entire file
		buffer.resize(samples);
	}
#endif
#ifdef OF_USING_LAD
	else if (audioDecoder) {
		// will read entire file
		buffer.resize(samples);
	}
#endif
#ifdef OF_USING_MPG123
	else if(mp3File){
		buffer.clear();
	}
#endif
	
#ifdef OF_USING_SNDFILE
	if(sndFile) return sfReadFile(buffer);
#elif defined( OF_USING_MPG123 )
	if(mp3File) return mpg123ReadFile(buffer);
#elif defined( OF_USING_LAD )
	if(audioDecoder) return ladReadFile(buffer);
#endif
	return false;
}
//--------------------------------------------------------------
bool ofSoundFile::seekTo(unsigned int sample){
	sample = min(samples,sample);
#ifdef OF_USING_SNDFILE
	if(sndFile) sf_seek(sndFile,sample,SEEK_SET);
#endif
#ifdef OF_USING_LAD
	if(audioDecoder) audioDecoder->seek(sample);
#endif
	
#ifdef OF_USING_MPG123
	else if(mp3File) mpg123_seek(mp3File,sample,SEEK_SET);
#endif
	else return false;
	
	return true; //TODO: check errors
}
//--------------------------------------------------------------
#ifdef OF_USING_SNDFILE
bool ofSoundFile::sfReadFile(ofSoundBuffer & buffer){
	samples_read = sf_read_float (sndFile, &buffer[0], buffer.size());
	/*if(samples_read<(int)buffer.size()){
		ofLogError() <<  "ofSoundFile: couldnt read " << path;
		return false;
	}*/
	if (subformat == SF_FORMAT_FLOAT || subformat == SF_FORMAT_DOUBLE){
		for (int i = 0 ; i < int(buffer.size()) ; i++){
			buffer[i] *= scale ;
		}
	}
	return true;
}
#endif
//--------------------------------------------------------------
#ifdef OF_USING_LAD
bool ofSoundFile::ladReadFile(ofSoundBuffer &buffer){
	
	int samplesRead = audioDecoder->read( buffer.size(), &buffer[0] );
	return samplesRead;
}
#endif


#ifdef OF_USING_MPG123
//------------------------------------------------------------
bool ofSoundFile::mpg123ReadFile(ofSoundBuffer & buffer){
	size_t done=0;
	size_t block_size = mpg123_outblock( mp3File );
	int err;
	if(buffer.size()==0){
		buffer.resize(block_size);
		do{
			err = mpg123_read(mp3File,(unsigned char*)&buffer[buffer.size()-block_size],block_size*4,&done);
			buffer.resize(buffer.size()+block_size);
		}while(err==MPG123_OK);
		buffer.resize(buffer.size()-(block_size-done/4));
		if(err != MPG123_DONE){
			ofLogError() <<  "Warning: Decoding ended prematurely because: " << (err == MPG123_ERR ? mpg123_strerror(mp3File) : mpg123_plain_strerror(err));
			return false;
		}
		duration = float(buffer.size())/float(channels) / float(samplerate);
	}else{
		err = mpg123_read(mp3File,(unsigned char*)&buffer[0],buffer.size()*sizeof(float),&done);
		if(err != MPG123_OK){
			ofLogError() <<  "Warning: Error decoding mp3: " << (err == MPG123_ERR ? mpg123_strerror(mp3File) : mpg123_plain_strerror(err));
			return false;
		}
	}

	return true;
}
#endif
