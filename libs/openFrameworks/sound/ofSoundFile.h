/*
 * ofSoundFile.h
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#ifndef OFSOUNDFILE_H_
#define OFSOUNDFILE_H_

#include "ofConstants.h"
#include "ofSoundBuffer.h"

#if defined (TARGET_OSX) || defined (TARGET_WIN32) || defined (TARGET_OS_IPHONE)
	#define OF_USING_LAD // libaudiodecoder
#elif defined (TARGET_LINUX)
	#define OF_USING_SNDFILE // libsndfile
#endif

#if defined (OF_USING_SNDFILE)
	#include <sndfile.h>
#elif defined (OF_USING_MPG123)
	#include <mpg123.h>
#elif defined (OF_USING_LAD)
	#include "audiodecoder.h"
#endif

/// reads a sound file into an ofSoundBuffer.
/// encoding support varies by platform.
/// Windows and OSX use libaudiodecoder for decoding, Linux uses libsndfile
bool ofLoadSound(ofSoundBuffer &buffer, string path);

/// writes an ofSoundBuffer as a 16-bit PCM WAV file.
bool ofSaveSound(const ofSoundBuffer &buffer, string path);

class ofSoundFile {
public:
	ofSoundFile();
	ofSoundFile(string path);

	virtual ~ofSoundFile();
	void close();

	/// opens a sound file for reading with readTo().
	/// encoding support varies by platform.
	/// Windows and OSX use libaudiodecoder for decoding, Linux uses libsndfile
	bool loadSound(string _path);
	
	/// writes an ofSoundBuffer as a 16-bit PCM WAV file
	bool saveSound(string _path, const ofSoundBuffer &buffer);

	/// reads a file into an ofSoundBuffer.
	/// by default, this will resize the buffer to fit the entire file.
	/// supplying a "samples" argument will read only the given number of samples
	bool readTo(ofSoundBuffer &buffer, unsigned int samples = 0);
	
	/// seek to the sample at the requested index
	bool seekTo(unsigned int sampleIndex);

	/// returns sound file duration in milliseconds
	unsigned long getDuration();
	int getNumChannels();
	int getSampleRate();
	unsigned long getNumSamples();
	bool isCompressed();
	bool isLoaded();
	string getPath();

private:
	
	bool sfReadFile(ofSoundBuffer & buffer);
	bool mpg123ReadFile(ofSoundBuffer & buffer);
	bool ladReadFile(ofSoundBuffer & buffer);
	
	bool sfOpen(string path);
	bool mpg123Open(string path);
	bool ladOpen(string path);
	
	void initDecodeLib();
	
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
	bool bCompressed;
	bool bLoaded;
};

#endif /* OFSOUNDFILE_H_ */
