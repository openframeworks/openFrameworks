/*
 * ofSoundFile.h
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#ifndef OFSOUNDFILE_H_
#define OFSOUNDFILE_H_


#include <sndfile.h>
#ifdef OF_USING_MPG123
	#include <mpg123.h>
#endif
#include "ofConstants.h"
#include "ofSoundBuffer.h"

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
	bool sfOpen(string path);
	bool mpg123Open(string path);

	// soundfilelib info
	SNDFILE* sndFile;
	int subformat;
	double scale;
	sf_count_t samples_read;

	// mpg123 info
#ifdef OF_USING_MPG123
	mpg123_handle * mp3File;
#endif


	// common info
	int channels;
	float duration; //in secs
	unsigned int samples;
	int samplerate;
	string path;
	static bool mpg123Inited;
};

#endif /* OFSOUNDFILE_H_ */
