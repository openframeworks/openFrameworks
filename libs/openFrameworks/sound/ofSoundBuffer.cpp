/*
 * ofSoundBuffer.cpp
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#include "ofSoundBuffer.h"
#include "ofSoundUtils.h"
#include <limits>

#if !defined(TARGET_ANDROID) && !defined(TARGET_OF_IPHONE)
ofSoundBuffer::InterpolationAlgorithm ofSoundBuffer::defaultAlgorithm = ofSoundBuffer::Hermite;
#else
ofSoundBuffer::InterpolationAlgorithm ofSoundBuffer::defaultAlgorithm = ofSoundBuffer::Linear;
#endif

ofSoundBuffer::ofSoundBuffer() {
	channels=1;
	samplerate=44100;
}

ofSoundBuffer::ofSoundBuffer(short * shortBuffer, unsigned int length, int _channels, int _samplerate){
	copyFrom(shortBuffer,length,_channels,_samplerate);
}


void ofSoundBuffer::copyFrom(short * shortBuffer, unsigned int length, int _channels, int _samplerate){
	channels = _channels;
	samplerate = _samplerate;
	buffer.resize(length*channels);
	for(unsigned int i=0;i<size();i++){
		buffer[i] = shortBuffer[i]/float(numeric_limits<short>::max());
	}
}

vector<float> & ofSoundBuffer::getBuffer(){
	return buffer;
}

int ofSoundBuffer::getNumChannels() const{
	return channels;
}

unsigned long ofSoundBuffer::getDuration() const{
	return float(bufferSize()*1000)/float(samplerate);
}

int ofSoundBuffer::getSampleRate() const{
	return samplerate;
}

void ofSoundBuffer::setNumChannels(int _channels){
	channels = _channels;
}

void ofSoundBuffer::setSampleRate(int rate){
	samplerate = rate;
}

unsigned long ofSoundBuffer::size() const{
	return buffer.size();
}

unsigned long ofSoundBuffer::bufferSize() const{
	return buffer.size()/channels;
}

void ofSoundBuffer::resize(unsigned int samples, float val){
	buffer.resize(samples,val);
}

void ofSoundBuffer::clear(){
	buffer.clear();
}

void ofSoundBuffer::set(float value){
	buffer.assign(buffer.size(),value);
}

void ofSoundBuffer::set(float * _buffer, unsigned int size, unsigned int nChannels){
	buffer.assign(_buffer,_buffer+size*nChannels);
	setNumChannels(nChannels);
}

float & ofSoundBuffer::operator[](unsigned int pos){
	return buffer[pos];
}

const float & ofSoundBuffer::operator[](unsigned int pos) const{
	return buffer[pos];
}

void ofSoundBuffer::swap(ofSoundBuffer & buffer){
	std::swap(channels,buffer.channels);
	std::swap(samplerate,buffer.samplerate);
	std::swap(this->buffer,buffer.buffer);
}

ofSoundBuffer ofSoundBuffer::operator*(float value){
	ofSoundBuffer ret = *this;
	ret *= value;
	return ret;
}

ofSoundBuffer & ofSoundBuffer::operator*=(float value){
	for(unsigned int i=0;i<buffer.size();i++){
		buffer[i] *= value;
	}
	return *this;
}

void ofSoundBuffer::stereoPan(float left, float right){
	if(channels!=2) return;
	float * bufferPtr = &buffer[0];
	for(unsigned int i=0;i<bufferSize();i++){
		*bufferPtr++ *= left;
		*bufferPtr++ *= right;
	}
}

void ofSoundBuffer::copyTo(ofSoundBuffer & soundBuffer, unsigned int bufferSize, unsigned int outChannels,unsigned int fromSample,bool loop) const{
	soundBuffer.setNumChannels(outChannels);
	soundBuffer.setSampleRate(samplerate);
	soundBuffer.resize(bufferSize*outChannels);
	copyTo(&soundBuffer.getBuffer()[0],bufferSize,outChannels,fromSample,loop);
}

void ofSoundBuffer::addTo(ofSoundBuffer & soundBuffer, unsigned int bufferSize, unsigned int outChannels,unsigned int fromSample,bool loop) const{
	soundBuffer.setNumChannels(outChannels);
	soundBuffer.setSampleRate(samplerate);
	soundBuffer.resize(bufferSize*outChannels);
	addTo(&soundBuffer.getBuffer()[0],bufferSize,outChannels,fromSample,loop);
}

void ofSoundBuffer::copyTo(float * out, unsigned int bufferSize, unsigned int outChannels,unsigned int fromSample,bool loop) const{
	if(int(this->bufferSize()-fromSample)>=bufferSize){
		if(channels==(int)outChannels){
			memcpy(out,&buffer[fromSample*channels],bufferSize*channels*sizeof(float));
		}else if(channels>(int)outChannels){
			const float * buffPtr = &buffer[fromSample*channels];
			for(unsigned int i=0;i<bufferSize;i++){
				for(unsigned int j=0;j<outChannels;j++){
					*out++ =  *buffPtr++;
				}
				buffPtr += channels - outChannels;
			}
		}else{ // we get only the first channel and replicate, posible cases?
			const float * buffPtr = &buffer[fromSample*channels];
			for(unsigned int i=0;i<bufferSize;i++){
				for(unsigned int j=0;j<outChannels;j++){
					*out++ =  buffPtr[i];
				}
			}
		}
	}else{
		if(channels==(int)outChannels){
			memcpy(out,&buffer[fromSample*channels],(this->bufferSize()-fromSample)*channels*sizeof(float));
		}else if(channels>(int)outChannels){
			const float * buffPtr = &buffer[fromSample*channels];
			for(unsigned int i=0;i<(int)this->bufferSize()-fromSample;i++){
				for(unsigned int j=0;j<outChannels;j++){
					*out++ =  *buffPtr++;
				}
				buffPtr += channels - outChannels;
			}
		}else{ // we get only the first channel and replicate, possible cases?
			const float * buffPtr = &buffer[fromSample*channels];
			for(unsigned int i=0;i<this->bufferSize()-fromSample;i++){
				for(unsigned int j=0;j<outChannels;j++){
					*out++ =  buffPtr[i];
				}
			}
		}
		if(!loop || size()==0){
			for(unsigned int i=0;i<(bufferSize-(this->bufferSize()-fromSample))*outChannels;i++){
				out[i] = 0;
			}
		}else{
			copyTo(out,(bufferSize-(this->bufferSize()-fromSample)),outChannels,0,loop);
		}
	}
}

void ofSoundBuffer::addTo(float * out, unsigned int bufferSize, unsigned int outChannels,unsigned int fromSample,bool loop) const{
	const float * buffPtr = &buffer[fromSample*channels];
	if(int(this->bufferSize()-fromSample)>=bufferSize){
		if(channels==(int)outChannels){
			for(unsigned int i=0;i<bufferSize*outChannels;i++){
				out[i] += buffPtr[i];
			}
		}else if(channels>(int)outChannels){
			for(unsigned int i=0;i<bufferSize;i++){
				for(unsigned int j=0;j<outChannels;j++){
					*out++ +=  *buffPtr++;
				}
				buffPtr += channels - outChannels;
			}
		}else{ // we get only the first channel and replicate, possible cases?
			for(unsigned int i=0;i<bufferSize;i++){
				for(unsigned int j=0;j<outChannels;j++){
					*out++ +=  buffPtr[i];
				}
			}
		}
	}else{
		if(channels==(int)outChannels){
			for(unsigned int i=0;i<(this->bufferSize()-fromSample)*outChannels;i++){
				out[i]+=buffPtr[i];
			}
		}else if(channels>(int)outChannels){
			for(unsigned int i=0;i<this->bufferSize()-fromSample;i++){
				for(unsigned int j=0;j<outChannels;j++){
					*out++ +=  *buffPtr++;
				}
				buffPtr += channels - outChannels;
			}
		}else{ // we get only the first channel and replicate, possible cases?
			for(unsigned int i=0;i<this->bufferSize()-fromSample;i++){
				for(unsigned int j=0;j<outChannels;j++){
					*out++ +=  buffPtr[i];
				}
			}
		}
		if(loop){
			addTo(out,(bufferSize-(this->bufferSize()-fromSample)),outChannels,0,loop);
		}
	}
}

// based on maximilian optimized for performance.
// might loose 1 or 2 samples when it reaches the end of the buffer
void ofSoundBuffer::linearResampleTo(ofSoundBuffer & resBuffer, unsigned int sampleBegin, unsigned int numsamples, float speed, bool loop){
	resBuffer.resize((unsigned int)numsamples*channels);
	resBuffer.setNumChannels(channels);
	resBuffer.setSampleRate(samplerate);
	unsigned int start = sampleBegin;
	unsigned int end = start*channels + double(numsamples*channels)*speed;
	double position = start;
	unsigned int intPosition = position;
	double increment = speed;
	unsigned int copySize = channels*sizeof(float);
	unsigned int to;
	float a,b;

	if(end<buffer.size()-2*channels){
		to = numsamples;
	}else if(sampleBegin+2>bufferSize()){
		to = 0;
	}else{
		to = float(bufferSize()-2-sampleBegin)/speed;
	}
	double remainder = position - intPosition;
	float * resBufferPtr = &resBuffer[0];

	for(unsigned int i=0;i<to;i++){
		intPosition *= channels;
		for(int j=0;j<channels;j++){
			//*resBufferPtr++ = ((1-remainder) * buffer[intPosition+channels] +  remainder * buffer[intPosition+2*channels]);
			a = buffer[intPosition];
			b = buffer[intPosition+channels];
			*resBufferPtr++ = ofLerp(a,b,remainder);
		}
		position += increment;
		intPosition = position;
		remainder = position - intPosition;
	}
	if(end>=buffer.size()-2*channels){
		to = numsamples-to;
		if(loop){
			intPosition %= bufferSize();
			for(unsigned int i=0;i<to;i++){
				intPosition *= channels;
				for(int j=0;j<channels;j++){
					a = buffer[intPosition];
					b = buffer[intPosition+channels];
					*resBufferPtr++ = (b-a)*remainder+a;
				}
				resBufferPtr+=channels;
				position += increment;
				intPosition = position;
			}
		}else{
			memset(resBufferPtr,0,to*copySize);
		}
	}
}

// based on maximilian optimized for performance.
// might loose 1 to 3 samples when it reaches the end of the buffer
void ofSoundBuffer::hermiteResampleTo(ofSoundBuffer & resBuffer, unsigned int sampleBegin, unsigned int numsamples, float speed, bool loop){
	resBuffer.resize((unsigned int)numsamples*channels,0);
	resBuffer.setNumChannels(channels);
	resBuffer.setSampleRate(samplerate);
	unsigned int start = sampleBegin;
	unsigned int end = start*channels + double(numsamples*channels)*speed;
	double position = start;
	unsigned int intPosition = position;
	double remainder = position - intPosition;
	double increment = speed;
	unsigned int copySize = channels*sizeof(float);
	unsigned int to;

	if(end<buffer.size()-3*channels){
		to = numsamples;
	}else if(sampleBegin+3>bufferSize()){
		to = 0;
	}else{
		to = double(bufferSize()-3-sampleBegin)/speed;
	}
	float * resBufferPtr = &resBuffer[0];

	float a,b,c,d;

	unsigned int from = 0;

	while(intPosition==0){
		intPosition *= channels;
		for(int j=0;j<channels;++j){
			a=loop?buffer[j]:0;
			b=buffer[intPosition+j];
			c=buffer[intPosition+j+channels];
			d=buffer[intPosition+j+channels*2];
			*resBufferPtr++ = ofHermiteInterpolate(remainder,a,b,c,d);
		}
		position += increment;
		intPosition = position;
		remainder = position - intPosition;
		from++;
	}

	for(unsigned int i=from;i<to;++i){
		intPosition *= channels;
		for(int j=0;j<channels;++j){
			a=buffer[intPosition+j-channels];
			b=buffer[intPosition+j];
			c=buffer[intPosition+j+channels];
			d=buffer[intPosition+j+channels*2];
			*resBufferPtr++ = ofHermiteInterpolate(remainder,a,b,c,d);
		}
		position += increment;
		intPosition = position;
		remainder = position - intPosition;
	}

	if(end>=buffer.size()-3*channels){
		to = numsamples-to;
		if(loop){
			intPosition %= buffer.size();
			for(unsigned int i=0;i<to;++i){
				for(int j=0;j<channels;++j){
					a=buffer[intPosition+j-channels];
					b=buffer[intPosition+j];
					c=buffer[intPosition+j+channels];
					d=buffer[intPosition+j+channels*2];
					*resBufferPtr++ = ofHermiteInterpolate(remainder,a,b,c,d);
				}
				position += increment;
				intPosition = position;
				remainder = position - intPosition;
				intPosition *= channels;
			}
		}else{
			memset(resBufferPtr,0,to*copySize);
		}
	}
}


void ofSoundBuffer::resampleTo(ofSoundBuffer & buffer, unsigned int sampleBegin, unsigned int numSamples, float speed, bool loop, InterpolationAlgorithm algorithm){
	switch(algorithm){
	case Linear:
		linearResampleTo(buffer,sampleBegin,numSamples,speed,loop);
		break;
	case Hermite:
		hermiteResampleTo(buffer,sampleBegin,numSamples,speed,loop);
		break;
	}
}

void ofSoundBuffer::resample(float speed, InterpolationAlgorithm algorithm){
	ofSoundBuffer resampled;
	resampleTo(resampled,0,bufferSize()/speed,speed,false,algorithm);
	*this = resampled;
}


void ofSoundBuffer::getChannel(ofSoundBuffer & buffer, int channel) const{
	buffer.setNumChannels(1);
	buffer.setSampleRate(samplerate);
	if(channels==1){
		copyTo(buffer,size(),1,0);
	}else{
		buffer.resize(bufferSize());
		const float * bufferPtr = &this->buffer[0];
		for(unsigned int i=0;i<bufferSize();i++){
			buffer[i] = *bufferPtr;
			bufferPtr+=channels;
		}
	}
}

void ofSoundBuffer::setChannel(const ofSoundBuffer & buffer, int channel){
	if(channels==1){
		buffer.copyTo(*this,buffer.size(),1,0);
	}else{
		resize(buffer.bufferSize()*channels);
		float * bufferPtr = &this->buffer[0];
		for(unsigned int i=0;i<bufferSize();i++){
			*bufferPtr = buffer[i];
			bufferPtr+=channels;
		}
	}
}

float ofSoundBuffer::getRMSAmplitude(){
	float rmsAmplitude  = 0;
	for(unsigned int i=0;i<size();i++){
		rmsAmplitude += abs(buffer[i]);
	}
	rmsAmplitude /= float(size());
	return rmsAmplitude;
}

float ofSoundBuffer::getRMSAmplitudeChannel(unsigned int channel){
	if(channel>getNumChannels()-1) return 0;
	float rmsAmplitude  = 0;
	for(unsigned int i=0;i<bufferSize();i++){
		rmsAmplitude += abs(buffer[i*getNumChannels()+channel]);
	}
	rmsAmplitude /= float(bufferSize());
	return rmsAmplitude;
}



