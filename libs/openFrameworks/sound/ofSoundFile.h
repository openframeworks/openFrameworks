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
#elif defined (TARGET_LINUX)
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
#ifdef OF_USING_LAD
#include "audiodecoder.h"
#endif
enum ofID3Tag{
    OF_ID3_TITLE,
    OF_ID3_ALBUM,
    OF_ID3_ARTIST,
    OF_ID3_TIME,
    OF_ID3_TRACK
};

bool ofLoadSound(ofSoundBuffer &buff, string path);
bool ofSaveSound(ofSoundBuffer &buff,  string path);

class ofSoundFile {
public:
	ofSoundFile();
    ofSoundFile(string path);

	virtual ~ofSoundFile();

	bool open(string path);
	void close();

    bool loadSound(string _path);
    bool saveSound(string _path);
    
	/// read file to buffer. 
	/// if samples is 0, resize the buffer to fit the entire file, otherwise just read the requested number of samples.
	bool readTo(ofSoundBuffer & buffer, unsigned int samples=0);//roy: this method name is a bit confusing. I'd rather change it.
	/// seek to the requested sample index
	bool seekTo(unsigned int sample);

	int getNumChannels();
	unsigned long getDuration(); // millisecs
	int getSampleRate();
    unsigned long getNumSamples();
    int getBitDepth();
    bool isCompressed();
    string getID3Tag(ofID3Tag tag);
    bool isLoaded();
    
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
    int bitDepth;
	int samplerate;
	string path;
    bool bCompressed;
    bool bLoaded;
};

#endif /* OFSOUNDFILE_H_ */
