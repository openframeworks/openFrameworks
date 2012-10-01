/*
 * ofSoundFile.cpp
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#include "ofSoundFile.h"
#include "ofLog.h"
#include "ofUtils.h"

#ifdef OF_USING_MPG123
bool ofSoundFile::mpg123Inited = false;
#endif

ofSoundFile::ofSoundFile() {
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

ofSoundFile::~ofSoundFile() {
	close();
}


bool ofSoundFile::open(string _path){
	path = ofToDataPath(_path);

	bool result = false;
	if(ofFilePath::getFileExt(path)=="mp3"){
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
	duration = float(samples/channels) / float(samplerate);
	return result;
}

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
}
#endif

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
}
#endif

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


int ofSoundFile::getNumChannels(){
	return channels;
}

unsigned long ofSoundFile::getDuration(){
	return duration*1000;
}

int ofSoundFile::getSampleRate(){
	return samplerate;
}

bool ofSoundFile::readTo(ofSoundBuffer & buffer, unsigned int _samples){
	buffer.setNumChannels(channels);
	buffer.setSampleRate(samplerate);
	if(_samples!=0){
		buffer.resize(_samples*channels);
	}
#ifdef OF_USING_SNDFILE
	else if (sndFile){
		buffer.resize(samples);
	}
#endif
#ifdef OF_USING_LAD
	else if ( audioDecoder ) {
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

bool ofSoundFile::seekTo(unsigned int sample){
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

#ifdef OF_USING_LAD
bool ofSoundFile::ladReadFile(ofSoundBuffer &outBuffer){
	
	int samplesRead = audioDecoder->read( outBuffer.size(), &outBuffer[0] );
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
