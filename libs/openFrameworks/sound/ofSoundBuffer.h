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

	/// sample rate of the audio in this buffer
	int getSampleRate() const;
	void setSampleRate(int rate);
	/// resample by changing the playback speed, keeping the same sampleRate
	void resample(float speed, InterpolationAlgorithm algorithm=defaultAlgorithm);
	
	/// the number of channels per frame
	int getNumChannels() const;
	/// set the number of channels. does not resize the underlying buffer.
	void setNumChannels(int channels);
	
	/// the number of frames, ie the number of sets of (getNumChannels()) samples
	unsigned long getNumFrames() const;

	/// the total number of samples in this buffer (==getNumFrames()*getNumChannels())
	unsigned long size() const;
	
	/// duration in milliseconds
	unsigned long getDurationMS() const;


	/// resize this buffer to exactly this many samples. it's up to you make sure samples matches the channel count.
	void resize(unsigned int numSamples, float val=float());
	void clear();
	void swap(ofSoundBuffer & buffer);
	void set(float value);
	void set(float * buffer, unsigned int numSamples, unsigned int nChannels);


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

	void getChannel(ofSoundBuffer & buffer, int channel) const;
	void setChannel(const ofSoundBuffer & buffer, int channel);

	float getRMSAmplitude();
	float getRMSAmplitudeChannel(unsigned int channel);
	
	
	void fillWithNoise();
	float fillWithTone( float pitchHz, float phase=0 );
	
	
private:
	void linearResampleTo(ofSoundBuffer & buffer, unsigned int sampleBegin, unsigned int numSamples, float speed, bool loop);
	void hermiteResampleTo(ofSoundBuffer & buffer, unsigned int sampleBegin, unsigned int numSamples, float speed, bool loop);
	vector<float> buffer;
	int channels;
	int samplerate;
};

#endif /* OFSOUNDBUFFER_H_ */
