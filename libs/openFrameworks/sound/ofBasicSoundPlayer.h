/*
 * ofBasicSoundPlayer.h
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#ifndef OFBASICSOUNDPLAYER_H_
#define OFBASICSOUNDPLAYER_H_

#include "ofBaseSoundPlayer.h"
#include "ofBaseTypes.h"
#include "ofConstants.h"
#include "ofSoundBuffer.h"
#include "ofThread.h"
#include "ofSoundFile.h"
#include "ofSoundStream.h"
#include "ofSoundMixer.h"
#include "ofEvents.h"

class ofBasicSoundPlayer: public ofBaseSoundPlayer, public ofBaseSoundOutput {
public:
	ofBasicSoundPlayer();
	virtual ~ofBasicSoundPlayer();


	bool loadSound(string fileName, bool stream = false);
	void unloadSound();
	void play();
	void stop();

	void setVolume(float vol);
	void setPan(float vol); // -1 = left, 1 = right
	void setSpeed(float spd);
	void setPaused(bool bP);
	void setLoop(bool bLp);
	void setMultiPlay(bool bMp);
	void setPosition(float pct); // 0 = start, 1 = end;
	void setPositionMS(int ms);

	float getPosition();
	int getPositionMS();
	bool getIsPlaying();
	float getSpeed();
	float getPan();
	bool isLoaded();
	float getVolume();

	ofEvent<ofSoundBuffer> resampledBufferE;

private:
	void audioOut(float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
	void updatePositions(int bufferSize);
	static ofSoundStream stream;
	static ofSoundMixer mixer;
	static bool initialized;
	static int samplerate;
	static int bufferSize;
	static int channels;
	float volume;
	ofSoundBuffer buffer, resampledBuffer;
	ofSoundFile soundFile;
	bool streaming;
	bool isPlaying;
	bool multiplay;
	bool bIsLoaded;
	bool loop;
	float speed;
	float pan;
	vector<float> relativeSpeed;
	vector<unsigned int> positions;
	vector<float> volumesLeft;
	vector<float> volumesRight;
};

#endif /* OFBASICSOUNDPLAYER_H_ */
