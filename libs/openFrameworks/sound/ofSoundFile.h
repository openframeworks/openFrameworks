/*
 * ofSoundFile.h
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#ifndef OFSOUNDFILE_H_
#define OFSOUNDFILE_H_

#include "ofConstants.h"

#if defined (TARGET_OSX) || defined (TARGET_WIN32)
	// libaudiodecoder
	#define OF_USING_LAD
#else
	// libsndfile
	#define OF_USING_SNDFILE
#endif

#ifdef OF_USING_SNDFILE
	#include <sndfile.h>
#endif
#ifdef OF_USING_MPG123
	#include <mpg123.h>
#endif
#include "ofSoundBuffer.h"
#include "audiodecoder.h"

class ofSoundFile {
public:
	ofSoundFile();
	virtual ~ofSoundFile();

	bool open(string path);
	void close();

	bool readTo(ofSoundBuffer & buffer, unsigned int samples=0);
	bool seekTo(unsigned int sample);


	int getNumChannels();
	unsigned long getDuration(); // millisecs
	int getSampleRate();

private:
	
	bool sfReadFile(ofSoundBuffer & buffer);
	bool mpg123ReadFile(ofSoundBuffer & buffer);
	bool ladReadFile(ofSoundBuffer & buffer);
	
	bool sfOpen(string path);
	bool mpg123Open(string path);
	bool ladOpen(string path);
	


#ifdef OF_USING_SNDFILE
	// soundfilelib info
	SNDFILE* sndFile;
	int subformat;
	double scale;
	sf_count_t samples_read;
#endif
	
#ifdef OF_USING_MPG123
	// mpg123 info
	mpg123_handle * mp3File;
	static bool mpg123Inited;
#endif
	
#ifdef OF_USING_LAD
	AudioDecoder* audioDecoder;
#endif


	// common info
	int channels;
	float duration; //in secs
	unsigned int samples;
	int samplerate;
	string path;
};

#endif /* OFSOUNDFILE_H_ */
