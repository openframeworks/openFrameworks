/*
 * ofSoundBuffer.h
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#ifndef OFSOUNDBUFFER_H_
#define OFSOUNDBUFFER_H_

#include "ofConstants.h"

class ofSoundBuffer {
public:
	ofSoundBuffer();
	ofSoundBuffer(short * shortBuffer, unsigned int length, int channels, int samplerate);

	enum InterpolationAlgorithm{
		Linear,
		Hermite
	};

	static InterpolationAlgorithm defaultAlgorithm;  //defaults to Linear for mobile, Hermite for desktop

	vector<float> & getBuffer();

	int getNumChannels() const;
	// millisecs
	unsigned long getDuration() const;
	int getSampleRate() const;
	/// total number of samples (number of frames * number of channels)
	unsigned long size() const;

	/// the number of frames (ie the number of sets of getNumChannels() samples)
	unsigned long bufferSize() const;
	unsigned long getNumFrames() const { return bufferSize(); }

	/// resize this buffer to exactly this many samples. it's up to you make sure samples matches the channel count.
	void resize(unsigned int samples, float val=float());
	void clear();
	void swap(ofSoundBuffer & buffer);
	void set(float value);
	void set(float * buffer, unsigned int size, unsigned int nChannels);

	void setNumChannels(int channels);
	void setSampleRate(int rate);

	float & operator[](unsigned int pos);
	const float & operator[](unsigned int pos) const;

	ofSoundBuffer operator*(float value);
	ofSoundBuffer & operator*=(float value);

	void stereoPan(float left, float right);


	void copyFrom(short * shortBuffer, unsigned int length, int channels, int samplerate);

	void copyTo(ofSoundBuffer & soundBuffer, unsigned int bufferSize, unsigned int outChannels,unsigned int fromSample,bool loop=false) const;
	void addTo(ofSoundBuffer & soundBuffer, unsigned int bufferSize, unsigned int outChannels,unsigned int fromSample,bool loop=false) const;

	void copyTo(float * out, unsigned int bufferSize, unsigned int outChannels,unsigned int fromSample,bool loop=false) const;
	void addTo(float * out, unsigned int bufferSize, unsigned int outChannels,unsigned int fromSample,bool loop=false) const;

	void resampleTo(ofSoundBuffer & buffer, unsigned int sampleBegin, unsigned int numSamples, float speed, bool loop=false, InterpolationAlgorithm algorithm=defaultAlgorithm);

	void resample(float speed, InterpolationAlgorithm algorithm=defaultAlgorithm);

	void getChannel(ofSoundBuffer & buffer, int channel) const;
	void setChannel(const ofSoundBuffer & buffer, int channel);

	float getRMSAmplitude();
	float getRMSAmplitudeChannel(unsigned int channel);
	
private:
	void linearResampleTo(ofSoundBuffer & buffer, unsigned int sampleBegin, unsigned int numSamples, float speed, bool loop);
	void hermiteResampleTo(ofSoundBuffer & buffer, unsigned int sampleBegin, unsigned int numSamples, float speed, bool loop);
	vector<float> buffer;
	int channels;
	int samplerate;
};

#endif /* OFSOUNDBUFFER_H_ */
